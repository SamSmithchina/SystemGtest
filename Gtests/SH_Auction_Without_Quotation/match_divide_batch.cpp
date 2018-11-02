#include "AShareCheckOrder/AShareCheckOrder.h"
#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest/gtest.h"

//gtest  for  matching divide orders
//	BatchGtestMatchDivideWithoutQuotation.CheckAssetNO
TEST(BatchGtestMatchDivideWithoutQuotation, CheckAssetNO)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchByDivide));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	int i = 0;
	int j = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	otl_stream streamDB;	//otl 流
	SHShare aSHShare[10];
	int iAShareNum = 10;		//aSHShare数组的成员数量
	int iRound = 2;

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
			if (0 == j % 2)
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
			aSHShare[j].cjjg = aSHShare[j].price;
			iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare[j].price, ui64Temp);	//带小数的成交价格安全转换为整形
			EXPECT_EQ(0, iRes);
			aSHShare[j].cjsl = aSHShare[j].qty;
			lTemp = atoi(aSHShare[j].qty.c_str());
			ui64Cjje = lTemp * ui64Temp;
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
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}
		con.Commit();	// commit

		//确认
		for (j = 0; j < iAShareNum; j++)
		{
			lRes = CheckOrdwth2Match(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

		//成交
		for (j = 0; j < iAShareNum; j++)		//比较
		{
			lRes =CheckDivideCjhb(con, aSHShare[j], 2);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}		//比较

	}//for (i = 0; i < 1; i++ )	//主循环

	if (0 < lErrorOrderCounter)
	{
		EzLog::i("=================================================", "\n");
		EzLog::e(__FUNCTION__, "\n");
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


//	BatchGtestMatchDivideWithoutQuotation.CheckAssetYES
TEST(BatchGtestMatchDivideWithoutQuotation, CheckAssetYES)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchByDivide));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	int i = 0;
	int j = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64BCjsl = 0;
	uint64_t ui64SCjsl = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	otl_stream streamDB;	//otl 流
	SHShare aSHShare[10];
	int iAShareNum = 10;		//aSHShare数组的成员数量
	long lAShareQty[10] = { 0 };	//	-1 表示卖单数量不合理 ， 不为 - 1 表示 不验股，或者买，或者卖的数量合理
	int iRound = 1;
	StockAsset aSHStockAsset;
	aSHStockAsset.Init("A645078963", "600302");

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
			if (0 == j % 2)
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
			aSHShare[j].cjjg = aSHShare[j].price;
			iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare[j].price, ui64Temp);	//带小数的成交价格安全转换为整形
			EXPECT_EQ(0, iRes);
			aSHShare[j].cjsl = aSHShare[j].qty;
			lTemp = atoi(aSHShare[j].qty.c_str());
			ui64Cjje = lTemp * ui64Temp;
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
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}
		con.Commit();	// commit

		//验股
		for (j = 0; j <iAShareNum; j++)
		{
			lAShareQty[j] = 0;
			lTemp = atol(aSHStockAsset.stock_etf_redemption_balance.c_str());
			lTemp += atol(aSHStockAsset.stock_available.c_str());
			lAShareQty[j] = atol(aSHShare[j].qty.c_str());
			if ("S" == aSHShare[j].bs && lAShareQty[j] > lTemp)
			{
				lAShareQty[j] = -1;		 //设置验股标志位，-1 表示卖单数量不合理 ， 不为 -1 表示 不验股，或者买，或者卖的数量合理
				lRes = CheckOrdwth2Error(con, aSHShare[j]);
				EXPECT_EQ(0, lRes);
			}
		}

		//确认
		for (j = 0; j < iAShareNum; j++)
		{
			if (-1 != lAShareQty[j])
			{
				lRes = CheckOrdwth2Match(con, aSHShare[j]);
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
			}
		}

		//成交
		for (j = 0; j < iAShareNum; j++)		//比较
		{
			if (-1 != lAShareQty[j])
			{
				lRes =CheckDivideCjhb(con, aSHShare[j], 2);
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
				if ("S" == aSHShare[j].bs)
				{
					ui64SCjsl += strtoull(aSHShare[j].cjsl.c_str(), NULL, 10);
				}
				if ("B" == aSHShare[j].bs)
				{
					ui64BCjsl += strtoull(aSHShare[j].cjsl.c_str(), NULL, 10);
				}
			}
		}		//比较

	}//for (i = 0; i < 1; i++ )	//主循环

	con.Close();

	//校验回写股份资产stock_asset
	Sleep(g_iTimeOut * 10); //等待tgw写完mysql数据
	iRes = CheckStgwWriteAssetBackToMySQL(aSHStockAsset, ui64BCjsl, ui64SCjsl);
	EXPECT_EQ(0, iRes);

	if (0 < lErrorOrderCounter)
	{
		EzLog::i("=================================================", "\n");
		EzLog::e(__FUNCTION__, "\n");
		EzLog::Out("g_iTimeOut  : ", (trivial::severity_level)2, g_iTimeOut);
		EzLog::Out("iQueryTimes : ", (trivial::severity_level)2, g_iQueryTimes);
		EzLog::Out("共执行组数  ：", (trivial::severity_level)2, iRound);
		EzLog::Out("每组        ：", (trivial::severity_level)2, iAShareNum);
		EzLog::Out("共计 iRound * iAShareNum ： ", (trivial::severity_level)2, iRound*iAShareNum);
		EzLog::Out("出现错误订单笔数 ：", (trivial::severity_level)2, lErrorOrderCounter);
		EzLog::i("=================================================", "\n");
	}
	EzLog::i(__FUNCTION__, "\n\n");
}