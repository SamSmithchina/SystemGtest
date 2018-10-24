#include "AShareCheckOrder/AShareCheckOrder.h"
#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest/gtest.h"
#include <mutex>
#include <thread>

int g_AShareNum = 0;
std::mutex mutex1;

//多线程执行；
//输出：错误数lErrorOrderCounter		0 表示正常，无错误；		
//									大于0 表示不正常；
int MatchAllWithoutQuotation()
{
	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	int i = 0;
	int j = 0;
	int k = 0;
	uint64_t ui64Cjjg = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lQueryResult = -1;
	long lTemp = 0;
	std::string strTemp = "";
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;		//aSHShare数组的成员数量
	g_AShareNum = iAShareNum;

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	iRes = con.SetAutoCommit(0);

	//主循环体包含插入、确认、成交
	//
	mutex1.lock();
	//	mutex1.try_lock();
	for (j = 0; j < iAShareNum; j++)
	{
		//初始化股票订单；
		g_iExternRecNum++;
		aSHShare[j].reff = "J000000000";
		itoa(g_iExternRecNum, szTemp, 10);
		aSHShare[j].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
		aSHShare[j].rec_num = szTemp;
		//aSHShare[j].account = "A645078963";		//股票账号
		//aSHShare[j].stock = "600302";			// 证券代码
		if (0 == g_iExternRecNum % 2)
		{
			aSHShare[j].bs = "B";					//买
		}
		else
		{
			aSHShare[j].bs = "S";					//卖
		}
		lTemp = g_iExternRecNum * 100;
		if (lTemp > 99999900)
		{
			lTemp = lTemp % 100000000;		//char qty[8]
		}
		aSHShare[j].qty = itoa(lTemp, szTemp, 10);
		lTemp = g_iExternRecNum;
		if (lTemp > 9999)
		{
			lTemp = g_iExternRecNum % 10000;
		}
		aSHShare[j].price = itoa(lTemp, szTemp, 10); 		//整数部分最大9999，后有小数点和三位小数；
		aSHShare[j].price += ".";
		if (lTemp > 999)
		{
			lTemp = g_iExternRecNum % 1000;		//价格三位小数，最大999，
		}
		aSHShare[j].price += itoa(lTemp, szTemp, 10);	//小数部分
		if (lTemp < 100)
		{
			//两位数补充末位的零
			aSHShare[j].price += "0";
			if (lTemp < 10)
			{
				aSHShare[j].price += "0";
			}//一位数再补充0；
		}

		//确认字段；

		//成交字段		
		aSHShare[j].gddm = aSHShare[j].account;
		aSHShare[j].zqdm = aSHShare[j].stock;
		aSHShare[j].cjsl = aSHShare[j].qty;
		aSHShare[j].cjjg = aSHShare[j].price;
		lTemp = atoi(aSHShare[j].qty.c_str());
		iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare[j].price, ui64Cjjg);	//带小数的成交价格安全转换为整形
		EXPECT_EQ(0, iRes);
		ui64Cjje = lTemp * ui64Cjjg;
		if (ui64Cjje > 999999999990)
		{
			aSHShare[j].cjje = "-1";
		}
		else
		{
			Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare[j].cjje, 2); //成交金额带两位小数；
		}

		//插入
		lRes = InsertOrder(con, aSHShare[j]);
		if (lRes == -1)
		{
			EzLog::e("插入异常", aSHShare[j].reff);
		}
		EXPECT_EQ(0, lRes) << i*iAShareNum + j;
	}

	con.Commit();	// commit

	//确认
	k = 0;
	for (j = 0; j < iAShareNum; j++)
	{
		lRes = CheckOrdwth2Match(con, aSHShare[j]);
		if (lRes == -1)
		{
			EzLog::e("确认异常", aSHShare[j].reff);
		}
		EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		if (lRes == 0 || j == iAShareNum - 1)		//当前线程只释放互斥量一次；并且保障会释放
		{
			if (k == 0)
			{
				EzLog::i("mutex1.unlock();", "\n");
				mutex1.unlock();
				k = 1;
			}
		}
	}

	//成交
	for (j = 0; j < iAShareNum; j++)		//比较
	{
		lRes = CheckCjhb(con, aSHShare[j]);
		if (lRes == -1)
		{
			++lErrorOrderCounter;
		}
	}		//比较

	con.Close();
	return lErrorOrderCounter;
}

DWORD WINAPI ThreadFunc1(LPVOID lpParam)
{
	long  lErrorOrderCounter = 0;
	EzLog::i("MutilThreadGtestMatchAll \t", "Thread1\n");
	try
	{
		lErrorOrderCounter = MatchAllWithoutQuotation();
		EzLog::i("\n", "Thread1 finish \n ");
	}
	catch (std::out_of_range &exc) {
		std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
	}
	catch (exception& e)
	{
		throw e;
	}

	return lErrorOrderCounter;
}

DWORD WINAPI ThreadFunc2(LPVOID lpParam)
{
	long  lErrorOrderCounter = 0;
	EzLog::i("MutilThreadGtestMatchAll \t", "Thread2\n");
	try
	{
		lErrorOrderCounter = MatchAllWithoutQuotation();
		EzLog::i("\n", "Thread2 finish \n ");
	}
	catch (std::out_of_range &exc) {
		std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
	}
	catch (exception &e)
	{
		throw e;
	}

	return lErrorOrderCounter;
}

DWORD WINAPI ThreadFunc3(LPVOID lpParam)
{
	long  lErrorOrderCounter = 0;
	EzLog::i("MutilThreadGtestMatchAll \t", "Thread3\n");
	try
	{
		lErrorOrderCounter = MatchAllWithoutQuotation();
		EzLog::i("\n", "Thread3 finish \n ");
	}
	catch (std::out_of_range &exc) {
		std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
	}
	catch (exception& e)
	{
		throw e;
	}

	return lErrorOrderCounter;
}

//	MutilThreadGtestMatchAll.MatchAll
TEST(MutilThreadGtestMatchAll, MatchAll)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchAll));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	DWORD iTheardParam1 = 0;
	DWORD iThreadParam2 = 0;
	DWORD iThreadParam3 = 0;
	DWORD iDRes1 = 0;
	DWORD iDRes2 = 0;
	DWORD iDRes3 = 0;
	HANDLE hThread1;
	HANDLE hThread2;
	HANDLE hThread3;
	int i = 0;
	int iRound = 1;
	long lErrorOrderCounter = 0;

	for (i = 0; i < iRound; i++)
	{
		EzLog::Out("iRound = ", (trivial::severity_level)2, i);
		hThread1 = CreateThread(NULL, 0, ThreadFunc1, &iTheardParam1, 0, NULL);
		hThread2 = CreateThread(NULL, 0, ThreadFunc2, &iThreadParam2, 0, NULL);
		hThread3 = CreateThread(NULL, 0, ThreadFunc3, &iThreadParam3, 0, NULL);
		WaitForSingleObject(hThread1, INFINITE);
		WaitForSingleObject(hThread2, INFINITE);
		WaitForSingleObject(hThread3, INFINITE);

		GetExitCodeThread(ThreadFunc1, &iDRes1);
		GetExitCodeThread(ThreadFunc2, &iDRes2);
		GetExitCodeThread(ThreadFunc3, &iDRes3);
		CloseHandle(hThread1);
		CloseHandle(hThread2);
		CloseHandle(hThread3);
		lErrorOrderCounter += iDRes1;
		lErrorOrderCounter += iDRes2;
		lErrorOrderCounter += iDRes3;
	}
	if (0 < lErrorOrderCounter)
	{
		EzLog::i("=================================================", "\n");
		EzLog::e("", __FUNCTION__);
		EzLog::Out("g_iTimeOut    : ", (trivial::severity_level)2, g_iTimeOut);
		EzLog::Out("iQueryTimes : ", (trivial::severity_level)2, g_iQueryTimes);
		EzLog::Out("共执行组数  ：", (trivial::severity_level)2, iRound);
		EzLog::Out("每组        ：", (trivial::severity_level)2, g_AShareNum);
		EzLog::Out("共计 iRound * iAShareNum ： ", (trivial::severity_level)2, iRound*g_AShareNum);
		EzLog::Out("出现错误订单笔数 ：", (trivial::severity_level)2, lErrorOrderCounter);
		EzLog::i("=================================================", "\n");
		EXPECT_EQ(lErrorOrderCounter, 0);//如果有错误订单就输出错误订单数，
	}
	EzLog::i("", __FUNCTION__);
}