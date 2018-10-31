#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"
#include "util/EzLog.h"


// 部分成交，一部分按行情均价成交，另一部分撤单
// 最近成交价1.000元， 卖单，验股
// account = "A645078963" 股票账号
// stock = ("600330") 天通股份
//	SingleMatchPartWithQuotation_S.RecentPrice_1
TEST(SingleMatchPartWithQuotation_S, RecentPrice_1)
{

	//切换模式
	ASSERT_EQ(0, TransformMatchMode(RecentPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600330";
	aStockQuot.zqmc = "天通股份";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Price = 0;
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
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);			//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "0.950";
	aSHShare.qty = "200000";
	aSHShare.bs = "S";					//买\卖
	aSHShare.qty2 = "100000";
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	aSHShare.cjsl = "100000";
	lTemp = atol(aSHShare.cjsl.c_str());
	Tgw_StringUtil::iLiToStr(aStockQuot.zjjg, aSHShare.cjjg, 3); //最近成交价格
	ui64Price = aStockQuot.zjjg;
	ui64Cjje = ui64Price * lTemp;
	Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2);

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//检查确认
	lRes = CheckOrdwth2Match(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//插入撤单
	lRes = InsertCancelOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//检查撤单的确认
	lRes = CheckOrdwth2Cancel(con, aSHShare);	//这里检查部分撤单的确认表，OrderType为5， 
	EXPECT_EQ(0, lRes);

	//查询成交结果
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


// 部分成交，一部分按行情均价成交，另一部分撤单
// 最近成交价1.000元， 卖单，不验股
// account = "A645078963" 股票账号
// stock = ("600332") 白云山
//	SingleMatchPartWithQuotation_S.RecentPriceCheckAsset_2
TEST(SingleMatchPartWithQuotation_S, RecentPriceCheckAsset_2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(RecentPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600332";
	aStockQuot.zqmc = "白云山";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long laShareQty = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Price = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare;
	aSHShare.account = "A645078963";
	aSHShare.stock = aStockQuot.zqdm;
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
	aSHShare.price = "0.950";
	aSHShare.qty = "200000";
	aSHShare.bs = "S";					//买\卖
	aSHShare.qty2 = "100000";
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	aSHShare.cjsl = "100000";
	lTemp = atol(aSHShare.cjsl.c_str());
	Tgw_StringUtil::iLiToStr(aStockQuot.zjjg, aSHShare.cjjg, 3); //最近成交价格
	ui64Price = aStockQuot.zjjg;
	ui64Cjje = ui64Price * lTemp;
	Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2);

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//验股
	lTemp = atol(aSHStockAsset.stock_etf_redemption_balance.c_str());
	lTemp += atol(aSHStockAsset.stock_available.c_str());
	laShareQty = atol(aSHShare.cjsl.c_str());

	if ("S" == aSHShare.bs && laShareQty > lTemp)	//卖出数量超过所持有的股票数量，错单
	{
		iRes = CheckOrdwth2Error(con, aSHShare);
		EXPECT_EQ(0, iRes);
	}
	else
	{
		//检查确认
		lRes = CheckOrdwth2Match(con, aSHShare);
		EXPECT_EQ(0, lRes);

		//插入撤单
		lRes = InsertCancelOrder(con, aSHShare);
		EXPECT_EQ(0, lRes);
		con.Commit();

		//查询成交结果
		lRes = CheckCjhb(con, aSHShare);
		EXPECT_EQ(0, lRes);

		//检查撤单的确认
		lRes = CheckOrdwth2Cancel(con, aSHShare);	//这里检查部分撤单的确认表，OrderType为5， 
		EXPECT_EQ(0, lRes);
	}

	//检查stgw写回stock_aasset表数据
	iRes = CheckStgwWriteAssetBackToMySQL(aSHShare, aSHStockAsset);
	EXPECT_EQ(0, iRes);

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
