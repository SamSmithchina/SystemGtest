#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"


// 挂单撤单 ，对应的股票最近成交数量0，订单理应不成交，之后撤下；
//买一价1.050元，卖一价1.045元， 买单，不验股
// account = "A645078963" 股票账号
// stock = ("600311") 荣华实业
// SingleNoMatchCancelWithQuotation_B.SellBuyPrice_1
TEST(SingleNoMatchCancelWithQuotation_B, SellBuyPrice_1)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SellBuyPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600311";
	aStockQuot.zqmc = "荣华实业";

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
	aSHShare.stock = ("600311");		// 证券代码
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.050";
	aSHShare.qty = "100000";
	aSHShare.bs = "B";					//买\卖
	aSHShare.qty2 = "100000";

	lRes = InsertOrder(con, aSHShare);	//消耗行情容量
	EXPECT_EQ(0, lRes);
	//插入订单
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
	aSHShare.rec_num = szTemp;
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//插入撤单
	lRes = InsertCancelOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//查询撤单
	Sleep(g_iTimeOut * 20);
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

// 挂单撤单 ，对应的股票买单价格低于卖一价，订单理应不成交，之后撤下；
//买一价1.050元，卖一价1.045元， 买单，不验股
// account = "A645078963" 股票账号
// stock = ("600311") 荣华实业
// SingleNoMatchCancelWithQuotation_B.SellBuyPrice_2
TEST(SingleNoMatchCancelWithQuotation_B, SellBuyPrice_2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SellBuyPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600311";
	aStockQuot.zqmc = "荣华实业";

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
	aSHShare.stock = ("600311");		// 证券代码
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.005";
	aSHShare.qty = "100000";
	aSHShare.bs = "B";					//买\卖
	aSHShare.qty2 = "100000";

	//插入订单
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//插入撤单
	lRes = InsertCancelOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//查询撤单
	Sleep(g_iTimeOut * 20);
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

// 挂单撤单 ，对应的股票最近成交数量0，订单理应不成交，之后推送新行情成交；
//买一价1.050元，卖一价1.045元， 买单，不验股
// account = "A645078963" 股票账号
// stock = ("600378") 天科股份
// SingleNoMatchCancelWithQuotation_B.SellBuyPrice_3
TEST(SingleNoMatchCancelWithQuotation_B, SellBuyPrice_3)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SellBuyPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600378";
	aStockQuot.zqmc = "天科股份";

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
	aSHShare.stock = ("600311");		// 证券代码
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.050";
	aSHShare.qty = "100000";
	aSHShare.bs = "B";					//买\卖
	//成交字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	aSHShare.cjsl = aSHShare.qty;
	lTemp = atoi(aSHShare.qty.c_str());
	uint64_t ui64Cjjg = aStockQuot.SJW1;
	ui64Cjje = lTemp * ui64Cjjg;
	if (ui64Cjje > 999999999990)
	{
		aSHShare.cjje = "-1";
	}
	else
	{
		Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2); //成交金额带两位小数；
	}


	lRes = InsertOrder(con, aSHShare);	//消耗行情容量
	EXPECT_EQ(0, lRes);
	//插入订单
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
	aSHShare.rec_num = szTemp;
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//推送第二次行情；
	//Sleep(g_iTimeOut * 25);
	aStockQuot.cjsl += 100000;
	aStockQuot.cjje += 100000000;
	TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
	aStockQuot.hqsj += ".500";					//毫秒
	EXPECT_EQ(0, SendQuotToRedis(aStockQuot));
	Sleep(g_iTimeOut * 20);

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
