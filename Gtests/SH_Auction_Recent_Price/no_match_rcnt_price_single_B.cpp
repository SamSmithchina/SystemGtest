﻿#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"


// 挂单撤单 ，对应的股票最近成交数量0，订单理应不成交，之后撤下；
// 最近成交价1.000元， 买单，不验股
// account = "A645078963" 股票账号
// stock = ("600388") 龙净环保
// SingleNoMatchCancelWithQuotation_B.RecentPrice_1
TEST(SingleNoMatchCancelWithQuotation_B, RecentPrice_1)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(RecentPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600388";
	aStockQuot.zqmc = "龙净环保";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	char szTemp[10] = { "\0" };
	OTLConn40240 con;
	SHShare aSHShareConsume;

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	//	ASSERT_EQ(0, iRes);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//单个测试样例；
	aSHShareConsume.account = "A645078963";	//股票账号
	aSHShareConsume.stock = aStockQuot.zqdm;		// 证券代码
	g_iExternRecNum++;
	aSHShareConsume.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShareConsume.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShareConsume.rec_num = szTemp;
	aSHShareConsume.price = "1.055";
	aSHShareConsume.qty = "100000";
	aSHShareConsume.bs = "B";					//买\卖

	lRes = InsertOrder(con, aSHShareConsume);	//消耗行情容量
	EXPECT_EQ(0, lRes);
	con.Commit();

	//插入订单
	Sleep(g_iTimeOut * 10);
	SHShare aSHShare;
	aSHShare.stock = aStockQuot.zqdm;
	aSHShare.account = aSHShareConsume.account;
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.050";
	aSHShare.qty = "100000";
	aSHShare.bs = "B";					//买\卖
	aSHShare.qty2 = "100000";
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//插入撤单
	Sleep(g_iTimeOut * 10);
	lRes = InsertCancelOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//查询撤单
	lRes = CheckOrdwth2Cancel(con, aSHShare);
	EXPECT_EQ(0, lRes);

	con.Close();
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e(__FUNCTION__, "\n");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n");
	}
}

// 挂单撤单 ，对应的股票买单价格低于最近成交价，订单理应不成交，之后撤下；
// 最近成交价1.000元， 买单，不验股
// account = "A645078963" 股票账号
// stock = ("600389") 江山股份
// SingleNoMatchCancelWithQuotation_B.RecentPrice_2
TEST(SingleNoMatchCancelWithQuotation_B, RecentPrice_2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(RecentPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600389";
	aStockQuot.zqmc = "江山股份";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	char szTemp[10] = { "\0" };
	OTLConn40240 con;
	SHShare aSHShare;

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	//	ASSERT_EQ(0, iRes);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//单个测试样例；
	aSHShare.account = "A645078963";	//股票账号
	aSHShare.stock = aStockQuot.zqdm;		// 证券代码
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "0.950";
	aSHShare.qty = "100000";
	aSHShare.bs = "B";					//买\卖
	aSHShare.qty2 = "100000";

	//插入订单
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//插入撤单
	Sleep(g_iTimeOut * 10);
	lRes = InsertCancelOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//查询撤单
	lRes = CheckOrdwth2Cancel(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Close();
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e(__FUNCTION__, "\n");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n");
	}
}


// 挂单撤单 ，对应的股票买单价格低于最近成交价，订单理应不成交，之后推送新行情成交；
// 最近成交价1.000元， 买单，不验股
// account = "A645078963" 股票账号
// stock = ("600376") 首开股份
// SingleNoMatchCancelWithQuotation_B.RecentPrice_3
TEST(SingleNoMatchCancelWithQuotation_B, RecentPrice_3)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(RecentPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600376";
	aStockQuot.zqmc = "首开股份";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare;

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	//	ASSERT_EQ(0, iRes);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//单个测试样例；
	aSHShare.account = "A645078963";	//股票账号
	aSHShare.stock = aStockQuot.zqdm;		// 证券代码
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "0.990";
	aSHShare.qty = "100";
	aSHShare.bs = "B";					//买\卖
	//成交字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	aSHShare.cjsl = aSHShare.qty;
	lTemp = atoi(aSHShare.qty.c_str());
	aSHShare.cjjg = "0.980";	
	uint64_t ui64Cjjg = 980;
	ui64Cjje = lTemp * ui64Cjjg;
	if (ui64Cjje > 999999999990)
	{
		aSHShare.cjje = "-1";
	}
	else
	{
		Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2); //成交金额带两位小数；
	}

	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//推送第二次行情；
	aStockQuot.cjsl += 200000;
	aStockQuot.cjje += 200000000;
	aStockQuot.zjjg = 980;
	TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
	aStockQuot.hqsj += ".500";					//毫秒
	EXPECT_EQ(0, SendQuotToRedis(aStockQuot));
	Sleep(g_iTimeOut * 50);

	//查询确认
	lRes = CheckOrdwth2Match(con, aSHShare);
	EXPECT_EQ(0, lRes);
	//成交
	lRes = CheckCjhb(con, aSHShare);
	EXPECT_EQ(0, lRes);

	con.Close();
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e(__FUNCTION__, "\n");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n");
	}
}