﻿#include "AShareCheckOrder/AShareCheckOrder.h"
#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest/gtest.h"

//完成全部成交的完整流程；
//	BatchGtestMatchAllWithoutQuotation.CheckAssetNO
TEST(BatchGtestMatchAllWithoutQuotation, CheckAssetNO)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchAll));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	int i = 0;
	int j = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	uint64_t ui64Cjjg = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;			//aSHShare数组的成员数量
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
				lTemp = lTemp % 100000000;//char qty[8]
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
			lRes = CheckCjhb(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}		//比较

	}//for (i = 0; i < 1; i++ )	//主循环
	con.Close();

	if (0 < lErrorOrderCounter)
	{
		char szTransferBuff[65] = { "\0" };
		std::string strError = "=================================================\n";
		strError += __FUNCTION__;
		strError += "\n共计 iRound * iAShareNum ： ";
		strError += itoa(iRound*iAShareNum, szTransferBuff, 10);
		strError += "\n出现错误订单笔数 ：";
		strError +=  ltoa(lErrorOrderCounter, szTransferBuff, 10);
		strError += "\n=================================================\n";
		EzLog::e(strError, "");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n\n");
	}
}


//完成全部成交的完整流程；
//	BatchGtestMatchAllWithoutQuotation.CheckAssetYES
TEST(BatchGtestMatchAllWithoutQuotation, CheckAssetYES)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchAll));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	int i = 0;
	int j = 0;
	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	uint64_t ui64Cjjg = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64BCjsl = 0;
	uint64_t ui64SCjsl = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;			//aSHShare数组的成员数量
	int iAShareAssetFlag[10] = { 0 };	//-1 表示卖单数量不合理 ， 不为-1,表示不验股，或者买，或者卖的数量合理
	StockAsset aSHStockAsset;
	int iRound = 1;

	aSHStockAsset.Init("A645078963", "600302");

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//主循环体包含插入、确认、成交
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
				lTemp = lTemp % 100000000;//char qty[8]
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
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}

		con.Commit();	// commit

		//验股
		for (j = 0; j < iAShareNum; j++)
		{
			lTemp = atol(aSHStockAsset.stock_etf_redemption_balance.c_str());
			lTemp += atol(aSHStockAsset.stock_available.c_str());
			iAShareAssetFlag[j] = 0;		//先置为 0
			iAShareAssetFlag[j] = atol(aSHShare[j].qty.c_str());
			if ("S" == aSHShare[j].bs && iAShareAssetFlag[j] > lTemp)
			{
				iAShareAssetFlag[j] = -1;	//卖出数量超过余额，置为-1；
				lRes = CheckOrdwth2Error(con, aSHShare[j]);
				EXPECT_EQ(0, lRes);
			}
		}

		//确认
		for (j = 0; j < iAShareNum; j++)
		{
			if (-1 != iAShareAssetFlag[j])
			{
				lRes = CheckOrdwth2Match(con, aSHShare[j]);
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
			}
		}

		//成交
		for (j = 0; j < iAShareNum; j++)		//比较
		{

			lRes = CheckCjhb(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;

			if (-1 != lRes)
			{
				if ("B" == aSHShare[j].bs)
				{
					ui64BCjsl += strtoull(aSHShare[j].cjsl.c_str(), NULL, 10);
				}
				if ("S" == aSHShare[j].bs)
				{
					ui64SCjsl += strtoull(aSHShare[j].cjsl.c_str(), NULL, 10);
				}
			}
		}

	}//for (i = 0; i < 1; i++ )	//主循环

	con.Close();

	//校验回写股份资产stock_asset
	Sleep(g_iTimeOut * 10); //等待tgw写完mysql数据
	iRes = CheckStgwWriteAssetBackToMySQL(aSHStockAsset, ui64BCjsl, ui64SCjsl);
	EXPECT_EQ(0, iRes) << ++lErrorOrderCounter;

	if (0 < lErrorOrderCounter)
	{
		char szTransferBuff[65] = { "\0" };
		std::string strError = "=================================================\n";
		strError += __FUNCTION__;
		strError += "\n共计 iRound * iAShareNum ： ";
		strError += itoa(iRound*iAShareNum, szTransferBuff, 10);
		strError += "\n出现错误订单笔数 ：";
		strError +=  ltoa(lErrorOrderCounter, szTransferBuff, 10);
		strError += "\n=================================================\n";
		EzLog::e(strError, "");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n\n");
	}
}