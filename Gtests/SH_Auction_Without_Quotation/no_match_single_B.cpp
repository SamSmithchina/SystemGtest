#include "AShareCheckOrder/AShareCheckOrder.h"
#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest/gtest.h"

// 挂单且撤单测试
/*
# 下买单
# //无需挂单确认

# 撤单消息
# 是否撤单 (查看订单号，数量、价格)
*/
//单个测试样例；
//	SingleNoMatchCancelWithoutQuotation_B.NoMatchCancel_1
TEST(SingleNoMatchCancelWithoutQuotation_B, NoMatchCancel_1)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulNotMatch));
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
	aSHShare.price = "0.100";
	aSHShare.qty = "100";
	aSHShare.bs = "B";					//买卖
	//确认字段；
	aSHShare.qty2 = aSHShare.qty;

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//插入撤单
	lRes = InsertCancelOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//查询撤单
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


//	SingleNoMatchCancelWithoutQuotation_B.NoMatchCancel_2
TEST(SingleNoMatchCancelWithoutQuotation_B, NoMatchCancel_2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulNotMatch));
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
	aSHShare.price = "9.010";
	aSHShare.qty = "10100";
	aSHShare.bs = "B";					//买卖
	//确认字段；
	aSHShare.qty2 = aSHShare.qty;

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//插入撤单
	lRes = InsertCancelOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//查询撤单//查询撤单
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


//	SingleNoMatchCancelWithoutQuotation_B.NoMatchCancel_3
TEST(SingleNoMatchCancelWithoutQuotation_B, NoMatchCancel_3)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulNotMatch));
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
	aSHShare.price = "109.010";
	aSHShare.qty = "10100";
	aSHShare.bs = "B";					//买卖
	//确认字段；
	aSHShare.qty2 = aSHShare.qty;

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//插入撤单
	lRes = InsertCancelOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//查询撤单//查询撤单
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

