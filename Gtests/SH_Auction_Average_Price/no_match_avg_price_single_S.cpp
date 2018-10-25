#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"

// 挂单撤单 ，对应的股票成交数量0，订单理应不成交，之后撤下；
// 区间段均价1.000元， 卖单，不验股
// account = "A645078963" 股票账号
// stock = ("600311") 荣华实业
//	SingleNoMatchCancelWithQuotation_S.AveragePrice_1
TEST(SingleNoMatchCancelWithQuotation_S, AveragePrice_1)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
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
	long laShareQty = 0;
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
	aSHShare.price = "0.950";
	aSHShare.qty = "100000";
	aSHShare.bs = "S";					//买\卖
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
	Sleep(g_iTimeOut * 50);
	lRes = CheckOrdwth2Cancel(con, aSHShare);
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

// 挂单撤单 ，对应的股票卖单价格高于均价，订单理应不成交，之后撤下；
// 区间段均价1.000元， 卖单，不验股
// account = "A645078963" 股票账号
// stock = ("600311") 荣华实业
//	SingleNoMatchCancelWithQuotation_S.AveragePrice_2
TEST(SingleNoMatchCancelWithQuotation_S, AveragePrice_2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
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
	long laShareQty = 0;
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
	aSHShare.qty = "100000";
	aSHShare.bs = "S";					//买\卖
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
	Sleep(g_iTimeOut * 50);
	lRes = CheckOrdwth2Cancel(con, aSHShare);
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
