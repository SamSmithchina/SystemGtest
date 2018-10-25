﻿#include "AShareCheckOrder/AShareCheckOrder.h"
#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest/gtest.h"



// 挂单且撤单测试
/*
# 下买单
# //无需挂单确认

# 撤单消息
# 是否撤单 (查看订单号，数量、价格)
*/


//	BatchGtestNoMatchCancelWithoutQuotation.BatchNoMatchCancel
TEST(BatchGtestNoMatchCancelWithoutQuotation, BatchNoMatchCancel)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulNotMatch));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	int i = 0;
	int j = 0;
	int k = 0;
	double dCjje = 0;
	char szTemp[10] = { "\0" };
	long lQueryResult = -1;
	long lTemp = 0;
	long lQueryCjsl = 0;
	long lAShareQty = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Cjjg = 0;
	std::string strTemp = "";
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;		//aSHShare数组的成员数量
	int iRound = 1;

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//主循环体包含插入、确认、成交
	//
	for (i = 0; i < iRound; i++)
	{
		for (j = 0; j < iAShareNum; j++)
		{
			//初始化股票订单；
			g_iExternRecNum++;
			aSHShare[j].reff = "J000000000";
			itoa(g_iExternRecNum, szTemp, 10);
			aSHShare[j].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
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
			aSHShare[j].qty2 = aSHShare[j].qty;
			//成交字段			
			aSHShare[j].gddm = aSHShare[j].account;
			aSHShare[j].zqdm = aSHShare[j].stock;

			//插入
			lRes = InsertOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}
		con.Commit();	// commit

		//	Sleep(35000);  // 后期该采用多线程减少等待stgw写入数据库用的35秒时间，
		//插入撤单
		for (j = 0; j < iAShareNum; j++)
		{
			lRes = InsertCancelOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}
		con.Commit();

		//成交
		//挂单撤单确认
		for (j = 0; j < iAShareNum; j++)
		{
			iRes = CheckOrdwth2Cancel(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

	}//for (i = 0; i < 1; i++ )	//主循环

	if (0 < lErrorOrderCounter)
	{
		EzLog::i("=================================================", "\n");
		EzLog::e("", __FUNCTION__);
		EzLog::Out("g_iTimeOut  : ", (trivial::severity_level)2, g_iTimeOut);
		EzLog::Out("iQueryTimes : ", (trivial::severity_level)2, g_iQueryTimes);
		EzLog::Out("共执行组数  ：", (trivial::severity_level)2, iRound);
		EzLog::Out("每组        ：", (trivial::severity_level)2, iAShareNum);
		EzLog::Out("共计 iRound * iAShareNum ： ", (trivial::severity_level)2, iRound*iAShareNum);
		EzLog::Out("出现错误订单笔数 ：", (trivial::severity_level)2, lErrorOrderCounter);
		EzLog::i("=================================================", "\n");
	}

	con.Close();
	EzLog::i(__FUNCTION__, "\n\n");
}
