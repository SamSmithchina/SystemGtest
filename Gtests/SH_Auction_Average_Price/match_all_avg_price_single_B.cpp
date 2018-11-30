﻿#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest/gtest.h"
#include "StockAsset/StockAsset.h"

// 实盘下推送行情，全部成交，区间段均价 不验股，  
//aSHShare.account = "A645078963";	
//aSHShare.stock = "600372" ;	中航电子
//			SingleGtestMatchAllWithQuotation_B.AveragePrice_1
TEST(SingleGtestMatchAllWithQuotation_B, AveragePrice_1)
{
	int iRes = 0;
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600372";
	aStockQuot.zqmc = "中航电子";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	long lRes = 0;
	char szTemp[10] = { " \0 " };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare;
	uint64_t ui64Price = 0;
	uint64_t ui64Cjje = 0;

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	//	ASSERT_EQ(0, iRes);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//单个测试样例；
	aSHShare.account = "A645078963";	    //股票账号
	aSHShare.stock = aStockQuot.zqdm;		// 证券代码
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.050";				//均价为1
	aSHShare.qty = "1000";
	aSHShare.bs = "B";						//买

	//成交汇回报字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	aSHShare.cjsl = aSHShare.qty;
	aSHShare.cjjg = CalcAvePrice(aStockQuot, ui64Price);	//计算区间段均价
	lTemp = atol(aSHShare.qty.c_str());
	ui64Cjje = ui64Price * lTemp;
	Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2);

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//确认
	lRes = CheckOrdwth2Match(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//比较结果；
	//实际成交价格为行情价格；
	lRes = CheckCjhb(con, aSHShare);
	EXPECT_EQ(0, lRes);

	con.Close();
	if(iRes != 0 || lRes != 0)
	{
		EzLog::e(__FUNCTION__, "\n");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n");
	}
}


// 实盘下推送行情，全部成交，买单，区间段均价 验股，  
// aStockQuot.zqdm = "600372";
// aStockQuot.zqmc = "中航电子";
//			SingleGtestMatchAllWithQuotation_B.AveragePriceCheckAssert_2
TEST(SingleGtestMatchAllWithQuotation_B, AveragePriceCheckAssert_2)
{
	int iRes = 0;
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	//构造行情
	AStockQuot aStockQuot;
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600372";
	aStockQuot.zqmc = "中航电子";
	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	long laShareQty = 0;
	long lRes = 0;
	char szTemp[10] = { " \0 " };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare;
	uint64_t ui64Price = 0;
	uint64_t ui64Cjje = 0;
	aSHShare.account = "A645078963";	//股票账号
	aSHShare.stock = aStockQuot.zqdm;;		// 证券代码
	StockAsset aSHStockAsset;
	aSHStockAsset.Init(aSHShare.account, aSHShare.stock);

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	//	ASSERT_EQ(0, iRes);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//单个测试样例；
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.050";				//均价为1
	aSHShare.qty = "1000";
	aSHShare.bs = "B";					//买

	//成交汇回报字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	aSHShare.cjsl = aSHShare.qty;
	aSHShare.cjjg = CalcAvePrice(aStockQuot, ui64Price);	//计算区间段均价
	lTemp = atol(aSHShare.qty.c_str());
	ui64Cjje = ui64Price * lTemp;
	Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2);

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//验股
	lTemp = atol(aSHStockAsset.stock_etf_redemption_balance.c_str());
	lTemp += atol(aSHStockAsset.stock_available.c_str());
	laShareQty = atol(aSHShare.qty.c_str());

	if ("S" == aSHShare.bs && laShareQty > lTemp)		//卖出数量大于可用股份stock_avaible+stock_etf_redemption_balance之和，则无法卖出；
	{
		lRes = CheckOrdwth2Error(con, aSHShare);
		EXPECT_EQ(0, lRes);
	}
	else
	{
		//确认
		lRes = CheckOrdwth2Match(con, aSHShare);
		EXPECT_EQ(0, lRes);

		//比较结果；
		//实际成交价格为行情价格；
		lRes = CheckCjhb(con, aSHShare);
		EXPECT_EQ(0, lRes);
		iRes = CheckStgwWriteAssetBackToMySQL(aSHShare, aSHStockAsset);
		EXPECT_EQ(0, iRes);
	}
	con.Close();

	if(iRes != 0 || lRes != 0)
	{
		EzLog::e(__FUNCTION__, "\n");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n");
	}
}

