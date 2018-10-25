#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"
#include "StockAsset/StockAsset.h"

// 分笔成交，第一次根据行情成交一部分，第二次根据行情成交一部分，
// 区间段均价1.000元， 买单， 不验股
// account = "A645078963" 
// stock = ("600303") 曙光股份
//		SingleMatchDivideWithQuotation_B.AveragePrice_1
TEST(SingleMatchDivideWithQuotation_B, AveragePrice_1)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600303";
	aStockQuot.zqmc = "曙光股份";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Price = 0;
	char szTemp[10] = { "\0" };
	long lQueryResult = -1;
	long lTemp = 0;
	std::string strTemp = "";
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
	aSHShare.price = "1.050";
	aSHShare.qty = "200000";
	aSHShare.bs = "B";					//买\卖
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	aSHShare.cjsl = aSHShare.qty;
	lTemp = atol(aSHShare.cjsl.c_str());
	aSHShare.cjjg = CalcAvePrice(aStockQuot, ui64Price);
	ui64Cjje = ui64Price * lTemp;
	Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2);

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//确认
	lRes = CheckOrdwth2Match(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//推送第二次行情；
	aStockQuot.cjsl += 100000;
	aStockQuot.cjje += 100000000;
	TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
	aStockQuot.hqsj += ".500";					//毫秒
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	//比较结果；
	//查成交表，比较成交结果是否与预期一致；
	//延时 ，等待第二笔成交
	Sleep(g_iTimeOut * 50);
	lRes = CheckDivideCjhb(con, aSHShare, 2);
	EXPECT_EQ(0, lRes);

	con.Close();
	EzLog::i(__FUNCTION__, "\n\n");
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e("", __FUNCTION__);
	}
}


// 分笔成交，第一次根据行情成交一部分，第二次根据行情成交一部分，
// 区间段均价， 买单， 验股
// aStockQuot.zqdm = "600315";
// aStockQuot.zqmc = "上海家化";
//		SingleMatchDivideWithQuotation_B.AveragePriceCheckAsset_2
TEST(SingleMatchDivideWithQuotation_B, AveragePriceCheckAsset_2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600315";
	aStockQuot.zqmc = "上海家化";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long laShareQty = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Price = 0;
	char szTemp[10] = { "\0" };
	long lQueryResult = -1;
	long lTemp = 0;
	std::string strTemp = "";
	OTLConn40240 con;
	SHShare aSHShare;
	aSHShare.account = "A645078963";	//股票账号
	aSHShare.stock = aStockQuot.zqdm;		// 证券代码
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
	aSHShare.price = "1.050";
	aSHShare.qty = "200000";
	aSHShare.bs = "B";					//买\卖
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	aSHShare.cjsl = aSHShare.qty;
	lTemp = atol(aSHShare.cjsl.c_str());
	aSHShare.cjjg = CalcAvePrice(aStockQuot, ui64Price);
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

		//推送第二次行情；
		aStockQuot.cjsl += 100000;
		aStockQuot.cjje += 100000000;
		TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
		aStockQuot.hqsj += ".500";					//毫秒
		ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

		//比较结果；
		//查成交表，比较成交结果是否与预期一致；
		Sleep(g_iTimeOut * 50);
		lRes = CheckDivideCjhb(con, aSHShare, 2);
		EXPECT_EQ(0, lRes);
	}

	//检查stgw写回stock_aasset表数据
	iRes = CheckStgwWriteAssetBackToMySQL(aSHShare, aSHStockAsset, true);
	EXPECT_EQ(0, iRes);

	con.Close();
	EzLog::i(__FUNCTION__, "\n\n");
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e("", __FUNCTION__);
	}
}