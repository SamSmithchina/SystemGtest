#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"
#include "util/EzLog.h"

//买一价1.050元，卖一价1.045元 ，批量处理实盘下分笔成交订单,  验股
// account = "A645078963" 股票账号
// stock = ("600354") 敦煌种业
//	 BatchGtestMatchDivideWithQuotation.BatchMatchDivide_SellBuyPriceCheckAssetYES
TEST(BatchGtestMatchDivideWithQuotation, BatchMatchDivide_SellBuyPriceCheckAssetYES)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SellBuyPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	//构造行情,
	AStockQuot aStockQuot;
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600354";
	aStockQuot.zqmc = "敦煌种业";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	int i = 0;
	int j = 0;
	uint64_t ui64Cjjg = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Price = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;			//aSHShare数组的成员数量
	long lAShareQty[10] = { 0 };	//-1 表示卖单数量不合理 ， 不为 - 1 表示 不验股，或者买，或者卖的数量合理
	int iRound = 1;
	StockAsset aSHStockAsset;
	aSHStockAsset.account_id = "A645078963";
	aSHStockAsset.Init("A645078963", aStockQuot.zqdm);
	uint64_t ui64BCjsl = 0;
	uint64_t ui64SCjsl = 0;

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
			//初始化股票订单；	//设计订单集和结果集
			aSHShare[j].account = aSHStockAsset.account_id;
			aSHShare[j].stock = aStockQuot.zqdm;
			g_iExternRecNum++;
			aSHShare[j].reff = "J000000000";
			itoa(g_iExternRecNum, szTemp, 10);
			aSHShare[j].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
			aSHShare[j].rec_num = szTemp;

			aSHShare[j].qty = "200000";

			if (0 == j % 2)
			{
				aSHShare[j].bs = "B";		//买
				ui64Cjjg = aStockQuot.SJW1;
				//ui64Price = ui64Cjjg + rand() % 1000;
				//for (; ui64Cjjg > ui64Price || ui64Price > aStockQuot.maxgain;) 	//设置价格高于卖一价 ，低于涨幅
				//{
				//	ui64Price = ui64Cjjg + rand() % 1000;
				//}
				ui64Price = aStockQuot.maxgain - j;
			}
			else
			{
				aSHShare[j].bs = "S";		//卖
				ui64Cjjg = aStockQuot.BJW1;
				//ui64Price = ui64Cjjg - rand() % 1000;
				//for (; aStockQuot.minfall > ui64Price || ui64Price > ui64Cjjg;) //价格在跌幅 到 买一价之间	
				//{
				//	ui64Price = ui64Cjjg - rand() % 1000;
				//}
				ui64Price = aStockQuot.minfall + j;
			}
			Tgw_StringUtil::iLiToStr(ui64Price, aSHShare[j].price, 3);

			//确认字段；

			//成交字段			
			aSHShare[j].gddm = aSHShare[j].account;
			aSHShare[j].zqdm = aSHShare[j].stock;
			aSHShare[j].cjsl = aSHShare[j].qty;
			Tgw_StringUtil::iLiToStr(ui64Cjjg, aSHShare[j].cjjg, 3);
			lTemp = atoi(aSHShare[j].qty.c_str());
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
			lAShareQty[j] = 0;		//先置为 0
			lTemp = atol(aSHStockAsset.stock_etf_redemption_balance.c_str());
			lTemp += atol(aSHStockAsset.stock_available.c_str());
			lAShareQty[j] = atol(aSHShare[j].qty.c_str());
			if ("S" == aSHShare[j].bs && lAShareQty[j] > lTemp)
			{
				lAShareQty[j] = -1;
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
			//推送第二次行情；
			//Sleep(g_iTimeOut * 25);
			aStockQuot.cjsl += 100000;
			aStockQuot.cjje += 100000000;
			TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
			aStockQuot.hqsj += ".500";					//毫秒
			EXPECT_EQ(0, SendQuotToRedis(aStockQuot));
			Sleep(g_iTimeOut * 20);

			aStockQuot.cjsl += 100000;
			aStockQuot.cjje += 100000000;
			TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
			aStockQuot.hqsj += ".500";					//毫秒
			EXPECT_EQ(0, SendQuotToRedis(aStockQuot));
			//	EzLog::i("插入行情 ： ", aStockQuot.OriginStr);
			Sleep(g_iTimeOut * 20);
		}

		//成交
		for (j = 0; j < iAShareNum; j++)
		{
			if (-1 != lAShareQty[j])
			{
				lRes = CheckCjhb(con, aSHShare[j], 2);
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
				if (lRes == 0)
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
		}
	}//for (i = 0; i < 1; i++ )	//主循环

	//校验回写股份资产stock_asset
	Sleep(g_iTimeOut * 10); //等待tgw写完mysql数据
	iRes = CheckStgwWriteAssetBackToMySQL(aSHStockAsset, ui64BCjsl, ui64SCjsl);
	EXPECT_EQ(0, iRes) << ++lErrorOrderCounter;

	//关闭连接
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


// 买一价1.050元，卖一价1.045元 ，批量处理实盘下分笔成交订单,  不验股
// account = "A645078963" 股票账号
// stock = ("600355") 精伦电子
//	 BatchGtestMatchDivideWithQuotation.BatchMatchDivide_SellBuyPriceCheckAssetNO
TEST(BatchGtestMatchDivideWithQuotation, BatchMatchDivide_SellBuyPriceCheckAssetNO)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SellBuyPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情,
	AStockQuot aStockQuot;
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600355";
	aStockQuot.zqmc = "静伦电子";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	int i = 0;
	int j = 0;
	uint64_t ui64Cjjg = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Price = 0;
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
	for (i = 0; i < iRound; i++)
	{
		for (j = 0; j < iAShareNum; j++)
		{
			//初始化股票订单；	//设计订单集和结果集
			//aSHShare[j].account = "A645078963";
			aSHShare[j].stock = aStockQuot.zqdm;
			g_iExternRecNum++;
			aSHShare[j].reff = "J000000000";
			itoa(g_iExternRecNum, szTemp, 10);
			aSHShare[j].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
			aSHShare[j].rec_num = szTemp;

			aSHShare[j].qty = "200000";

			if (0 == j % 2)
			{
				aSHShare[j].bs = "B";		//买
				ui64Cjjg = aStockQuot.SJW1;
				//ui64Price = ui64Cjjg + rand() % 1000;
				//for (; ui64Cjjg > ui64Price || ui64Price > aStockQuot.maxgain;) 	//设置价格高于卖一价 ，低于涨幅
				//{
				//	ui64Price = ui64Cjjg + rand() % 1000;
				//}
				ui64Price = aStockQuot.maxgain - j;
			}
			else
			{
				aSHShare[j].bs = "S";		//卖
				ui64Cjjg = aStockQuot.BJW1;
				//ui64Price = ui64Cjjg - rand() % 1000;
				//for (; aStockQuot.minfall > ui64Price || ui64Price > ui64Cjjg;) //价格在跌幅 到 买一价之间	
				//{
				//	ui64Price = ui64Cjjg - rand() % 1000;
				//}
				ui64Price = aStockQuot.minfall + j;
			}
			Tgw_StringUtil::iLiToStr(ui64Price, aSHShare[j].price, 3);

			//确认字段；

			//成交字段			
			aSHShare[j].gddm = aSHShare[j].account;
			aSHShare[j].zqdm = aSHShare[j].stock;
			aSHShare[j].cjsl = aSHShare[j].qty;
			Tgw_StringUtil::iLiToStr(ui64Cjjg, aSHShare[j].cjjg, 3);
			lTemp = atoi(aSHShare[j].qty.c_str());
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

			//推送第二次行情；
			//Sleep(g_iTimeOut * 25);
			aStockQuot.cjsl += 100000;
			aStockQuot.cjje += 100000000;
			TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
			aStockQuot.hqsj += ".500";					//毫秒
			EXPECT_EQ(0, SendQuotToRedis(aStockQuot));
			Sleep(g_iTimeOut * 20);

			aStockQuot.cjsl += 100000;
			aStockQuot.cjje += 100000000;
			TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
			aStockQuot.hqsj += ".500";					//毫秒
			EXPECT_EQ(0, SendQuotToRedis(aStockQuot));
			//	EzLog::i("插入行情 ： ", aStockQuot.OriginStr);
			Sleep(g_iTimeOut * 20);
		}

		//成交
		for (j = 0; j < iAShareNum; j++)
		{
			lRes = CheckCjhb(con, aSHShare[j], 2);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

	}//for (i = 0; i < 1; i++ )	//主循环

	//关闭连接
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
