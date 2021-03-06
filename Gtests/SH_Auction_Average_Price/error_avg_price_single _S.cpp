﻿#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"

//停牌 是不能成交，


//卖单价格小于跌幅， 错单
// 区间段均价1.000元， 卖单，不验股
// account = "A645078963" 股票账号
// stock = ("600312") 平高电气
//	SingleErrorWithQuotation_S.AveragePrice_1
TEST(SingleErrorWithQuotation_S, AveragePrice_1)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600312";
	aStockQuot.zqmc = "平高电气";

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
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "0.850";
	aSHShare.qty = "100000";
	aSHShare.bs = "S";					//买\卖


	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//查询错单
	lRes = CheckOrdwth2Error(con, aSHShare);
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


//卖单数量超出股份持有数量，错单。
// 区间段均价1.000元， 卖单，验股
// account = "A645078963" 股票账号
// stock = "600333"  长春燃气
//	SingleErrorWithQuotation_S.AveragePrice_2
TEST(SingleErrorWithQuotation_S, AveragePrice_2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	//构造行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600333";
	aStockQuot.zqmc = "长春燃气";

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
	aSHShare.account = "A645078963";		//股票账号
	aSHShare.stock = aStockQuot.zqdm;		// 证券代码
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "0.949";				//价格合理
	aSHShare.qty = "99990000";				//char qty[8]， 最大9999 9999， 在本地mysql数据库simtgw 的stock_asset表中600333股票的可卖数量0；
	aSHShare.bs = "S";						//买\卖


	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//查询错单
	lRes = CheckOrdwth2Error(con, aSHShare);
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
//
//// 股票停牌， 错单
//// 区间段均价1.000元， 卖单，不验股
//// account = "A645078963" 股票账号
//// stock = ("600312") 平高电气
//// SingleErrorWithQuotation.AveragePrice_4
//TEST(SingleErrorWithQuotation, AveragePrice_4)
//{
//	//切换模式
//	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
//	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));
//
//	//构造行情
//	AStockQuot aStockQuot;				//行情CJSL = 100000
//	CreateQuotation(aStockQuot);
//	aStockQuot.zqdm = "600312";
//	aStockQuot.zqmc = "平高电气";
//	aStockQuot.TPBZ = "P";	//停牌标志正常是“F”, 改成空P
//
//	//推送行情
//	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));
//
//	int iRes = 0;
//	long lRes = 0;
//	long laShareQty = 0;
//	char szTemp[10] = { "\0" };
//	long lQueryResult = -1;
//	long lTemp = 0;
//	std::string strTemp;
//	OTLConn40240 con;
//	SHShare aSHShare;
//
//	//建立数据库连接 ,0 right , -1 wrong
//	iRes = con.Connect(g_strShOdbcConn);
//	//	ASSERT_EQ(0, iRes);
//	ASSERT_EQ(0, iRes);
//	iRes = con.SetAutoCommit(0);
//	ASSERT_EQ(0, iRes);
//
//	//单个测试样例；
//	aSHShare.account = "A645078963";	//股票账号
//	aSHShare.stock = aStockQuot.zqdm;		// 证券代码
//	g_iExternRecNum++;
//	aSHShare.reff = "J000000000";
//	itoa(g_iExternRecNum, szTemp, 10);
//	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
//	aSHShare.rec_num = szTemp;
//	aSHShare.price = "0.950";		 	//价格合理
//	aSHShare.qty = "100000";
//	aSHShare.bs = "S";					 //买\卖
//	
//
//	//插入测试样例
//	lRes = InsertOrder(con, aSHShare);
//	EXPECT_EQ(0, lRes);
//	con.Commit();
//
//	//查询错单
//	lRes = CheckOrdwth2Error(con, aSHShare);
//	EXPECT_EQ(0, lRes);
//
//	con.Close();
//
//	EzLog::i(__FUNCTION__, "\n\n");
//	if (iRes != 0 || lRes != 0)
//	{
//		EzLog::e(__FUNCTION__, "\n");
//	}
//}
//
//// 股票停牌， 错单
//// 区间段均价1.000元， 买单，不验股
//// account = "A645078963" 股票账号
//// stock = ("600312") 平高电气
//// SingleErrorWithQuotation.AveragePrice_5
//TEST(SingleErrorWithQuotation, AveragePrice_5)		//	2018年9月14日15点06分 目前停牌的错单功能stgw没完成；出现错误暂时不管。
//{
//	//切换模式
//	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
//	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));
//
//	//构造行情
//	AStockQuot aStockQuot;				//行情CJSL = 100000
//	CreateQuotation(aStockQuot);
//	aStockQuot.zqdm = "600312";
//	aStockQuot.zqmc = "平高电气";
//	aStockQuot.TPBZ = "P";	//停牌标志正常是“F”, 改成空P
//
//	//推送行情
//	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));
//
//	int iRes = 0;
//	long lRes = 0;
//	long laShareQty = 0;
//	char szTemp[10] = { "\0" };
//	long lQueryResult = -1;
//	long lTemp = 0;
//	std::string strTemp;
//	OTLConn40240 con;
//	SHShare aSHShare;
//
//	//建立数据库连接 ,0 right , -1 wrong
//	iRes = con.Connect(g_strShOdbcConn);
//	//	ASSERT_EQ(0, iRes);
//	ASSERT_EQ(0, iRes);
//	iRes = con.SetAutoCommit(0);
//	ASSERT_EQ(0, iRes);
//
//	//单个测试样例；
//	aSHShare.account = "A645078963";	//股票账号
//	aSHShare.stock = aStockQuot.zqdm;		// 证券代码
//	g_iExternRecNum++;
//	aSHShare.reff = "J000000000";
//	itoa(g_iExternRecNum, szTemp, 10);
//	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
//	aSHShare.rec_num = szTemp;
//	aSHShare.price = "1.050";		 	//价格合理
//	aSHShare.qty = "100000";
//	aSHShare.bs = "B";					 //买\卖
//	
//
//	//插入测试样例
//	lRes = InsertOrder(con, aSHShare);
//	EXPECT_EQ(0, lRes);
//	con.Commit();
//
//	//查询错单
//	lRes = CheckOrdwth2Error(con, aSHShare);
//	EXPECT_EQ(0, lRes);
//
//	con.Close();
//	
//	EzLog::i(__FUNCTION__, "\n\n");
//	if (iRes != 0 || lRes != 0)
//	{
//		EzLog::e(__FUNCTION__, "\n");
//	}
//}