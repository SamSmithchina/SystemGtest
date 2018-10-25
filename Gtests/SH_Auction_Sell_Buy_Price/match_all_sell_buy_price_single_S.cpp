#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest/gtest.h"
#include "StockAsset/StockAsset.h"

// 实盘下推送行情，全部成交，卖单，买一价1.050元，卖一价1.045元， 不验股，  
//			SingleGtestMatchAllWithQuotation_S.SellBuyPrice_1
TEST(SingleGtestMatchAllWithQuotation_S, SellBuyPrice_1)
{
	int iRes = 0;
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SellBuyPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;
	CreateQuotationExample(aStockQuot);

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
	aSHShare.account = "A645078963";	//股票账号
	aSHShare.stock = aStockQuot.zqdm;		// 证券代码
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.038";				//买一价1.050
	aSHShare.qty = "100";
	aSHShare.bs = "S";					// 卖

	//成交汇回报字段
	aSHShare.cjsl = aSHShare.qty;
	Tgw_StringUtil::iLiToStr(aStockQuot.BJW1, aSHShare.cjjg, 3);
	ui64Price = aStockQuot.BJW1;
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
	EzLog::i(__FUNCTION__, "\n\n");
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e("", __FUNCTION__);
	}
}


// 实盘下推送行情，全部成交，卖单，买一价1.050元，卖一价1.045元 ，验股，  
//			SingleGtestMatchAllWithQuotation_S.SellBuyPriceCheckAsset_2
TEST(SingleGtestMatchAllWithQuotation_S, SellBuyPriceCheckAsset_2)
{
	int iRes = 0;
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SellBuyPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	//构造行情
	AStockQuot aStockQuot;
	CreateQuotationExample(aStockQuot);

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	long laShareQty = 0;
	long lRes = 0;
	char szTemp[10] = { " \0 " };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare;
	aSHShare.account = "A645078963";	//股票账号
	aSHShare.stock = aStockQuot.zqdm;;		// 证券代码		
	uint64_t ui64Price = 0;
	uint64_t ui64Cjje = 0;
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
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.022";				//买一价1.050
	aSHShare.qty = "100";
	aSHShare.bs = "S";					//卖

	//成交汇回报字段
	aSHShare.cjsl = aSHShare.qty;
	Tgw_StringUtil::iLiToStr(aStockQuot.BJW1, aSHShare.cjjg, 3);
	ui64Price = aStockQuot.BJW1;
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
	}
	con.Close();

	iRes = CheckStgwWriteAssetBackToMySQL(aSHShare, aSHStockAsset);
	EXPECT_EQ(0, iRes);
	EzLog::i(__FUNCTION__, "\n\n");
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e("", __FUNCTION__);
	}
}