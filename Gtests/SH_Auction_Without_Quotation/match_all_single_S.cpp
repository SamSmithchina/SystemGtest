#include "AShareCheckOrder/AShareCheckOrder.h"
#include "util/EzLog.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"
#include "tool_mysql\MysqlCnnc602.h"

//全部成交模式测试
/*
下买单
检验确认消息
检验成交消息(查看成交订单号，成交数量、成交价格) ，在ASSERT_EQ中完成
*/

//单个测试样例；
//	SingleMatchAllWithoutQuotation_S.MatchAll_1
TEST(SingleMatchAllWithoutQuotation_S, MatchAll_1)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchAll));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	char szTemp[10] = { " \0 " };
	OTLConn40240 con;
	SHShare aSHShare;			

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
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.010";
	aSHShare.qty = "1100";
	aSHShare.bs = "S";					//买卖
	aSHShare.cjsl = aSHShare.qty;
	aSHShare.cjjg = aSHShare.price;
	aSHShare.cjje = "1111.00";

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//确认
	lRes = CheckOrdwth2Match(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//比较结果；
	//查成交表，比较成交结果是否与预期一致；
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

//单个测试样例；
//	SingleMatchAllWithoutQuotation_S.MatchAll_2
TEST(SingleMatchAllWithoutQuotation_S, MatchAll_2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchAll));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	char szTemp[10] = { " \0 " };
	OTLConn40240 con;
	SHShare aSHShare;			

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
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "19.010";
	aSHShare.qty = "100100";
	aSHShare.bs = "S";					//买卖
	aSHShare.cjsl = aSHShare.qty;
	aSHShare.cjjg = aSHShare.price;
	aSHShare.cjje = "1902901.00";

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//确认
	lRes = CheckOrdwth2Match(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//比较结果；
	//查成交表，比较成交结果是否与预期一致；
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

//单个测试样例；
//	SingleMatchAllWithoutQuotation_S.MatchAll_3
TEST(SingleMatchAllWithoutQuotation_S, MatchAll_3)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchAll));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	char szTemp[10] = { " \0 " };
	OTLConn40240 con;
	SHShare aSHShare;			

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
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "109.100";
	aSHShare.qty = "10000";
	aSHShare.bs = "S";					//买卖
	aSHShare.cjsl = aSHShare.qty;
	aSHShare.cjjg = aSHShare.price;
	aSHShare.cjje = "1091000.00";

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//确认
	lRes = CheckOrdwth2Match(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//比较结果；
	//查成交表，比较成交结果是否与预期一致；
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

//单个测试样例；
//	SingleMatchAllWithoutQuotation_S.MatchAll_4
TEST(SingleMatchAllWithoutQuotation_S, MatchAll_4)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchAll));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	char szTemp[10] = { " \0 " };
	OTLConn40240 con;
	SHShare aSHShare;			

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
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "999.000";
	aSHShare.qty = "1001002";
	aSHShare.bs = "S";					//买卖
	aSHShare.cjsl = aSHShare.qty;
	aSHShare.cjjg = aSHShare.price;
	aSHShare.cjje = "-1";

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//确认
	lRes = CheckOrdwth2Match(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//比较结果；
	//查成交表，比较成交结果是否与预期一致；
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

//单个测试样例；
//增加验股测试样例， 
//	SingleMatchAllWithoutQuotation_S.MatchAllCheckAsset_5
TEST(SingleMatchAllWithoutQuotation_S, MatchAllCheckAsset_5)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchAll));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	int iRes = 0;
	long lRes = 0;
	char szTemp[10] = { " \0 " };
	long lTemp = 0;
	long laShareQty = 0;
	OTLConn40240 con;
	SHShare aSHShare;			
	StockAsset aSHStockAsset;
	aSHStockAsset.Init(aSHShare.account , aSHShare.stock);

	//建立上海数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	//	ASSERT_EQ(0, iRes);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//单个测试样例；
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "999.000";
	aSHShare.qty = "99999900";  //最大值
	aSHShare.bs = "S";					//卖
	aSHShare.cjsl = aSHShare.qty;
	aSHShare.cjjg = aSHShare.price;
	aSHShare.cjje = "-1";

	//插入测试样例;
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
		//查成交表，比较成交结果是否与预期一致；
		lRes = CheckCjhb(con, aSHShare);
		EXPECT_EQ(0, lRes);
	}

	//校验回写股份资产stock_asset
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


//单个测试样例；
// 增加验股测试样例，
//	SingleMatchAllWithoutQuotation_S.MatchAllCheckAsset_6
TEST(SingleMatchAllWithoutQuotation_S, MatchAllCheckAsset1_6)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchAll));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	int iRes = 0;
	long lRes = 0;
	char szTemp[10] = { " \0 " };
	long lTemp = 0;
	long laShareQty = 0;
	OTLConn40240 con;
	SHShare aSHShare;			
	StockAsset aSHStockAsset;
	aSHStockAsset.Init(aSHShare.account, aSHShare.stock);

	//建立上海数据库连接 ,0 right , -1 wrong
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
	aSHShare.price = "999.000";
	aSHShare.qty = "100";					 //最小值
	aSHShare.bs = "S";					//卖
	aSHShare.cjsl = aSHShare.qty;
	aSHShare.cjjg = aSHShare.price;
	aSHShare.cjje = "99900.00";

	//插入测试样例;
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
		//查成交表，比较成交结果是否与预期一致；
		lRes = CheckCjhb(con, aSHShare);
		EXPECT_EQ(0, lRes);
	}

	//校验回写股份资产stock_asset
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
