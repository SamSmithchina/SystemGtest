#include "AShareCheckOrder/AShareCheckOrder.h"
#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest/gtest.h"


//错单测试
/*# 下买单
# 但没有成交 检查确认汇报
*/

//单个测试样例；
//	SingleErrorWithoutQuotation.Error1
TEST(SingleErrorWithoutQuotation_S, Error1)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulErrMatch));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	char szTemp[10] = { " \0 " };
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
	//aSHShare price= 1.010; pty =1100; bs = B
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "1.010";
	aSHShare.qty = "1100";
	aSHShare.bs = "S";					//买卖

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//比较确认结果；
	lRes = CheckOrdwth2Error(con, aSHShare);
	EXPECT_EQ(0, lRes);

	con.Close();
	EzLog::i(__FUNCTION__, "\n\n");
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e("", __FUNCTION__);
	}
}


//单个测试样例；
//	SingleErrorWithoutQuotation.Error2
TEST(SingleErrorWithoutQuotation_S, Error2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulErrMatch));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	char szTemp[10] = { " \0 " };
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
	//aSHShare price= 19.010; pty =10100; bs = B
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.bs = "S";				//买卖
	aSHShare.price = "19.010";
	aSHShare.qty = "10100";

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//比较确认结果；
	lRes = CheckOrdwth2Error(con, aSHShare);
	EXPECT_EQ(0, lRes);

	con.Close();
	EzLog::i(__FUNCTION__, "\n\n");
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e("", __FUNCTION__);
	}
}

//单个测试样例；
//	SingleErrorWithoutQuotation.Error3
TEST(SingleErrorWithoutQuotation_S, Error3)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulErrMatch));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	char szTemp[10] = { " \0 " };
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
	//aSHShare price= 0.010; pty =100; bs = B
	g_iExternRecNum++;
	aSHShare.reff = "J000000000";
	itoa(g_iExternRecNum, szTemp, 10);
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);		//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "109.100";
	aSHShare.qty = "10000";
	aSHShare.bs = "S";					//买卖

	//插入测试样例
	lRes = InsertOrder(con, aSHShare);
	EXPECT_EQ(0, lRes);
	con.Commit();

	//比较确认结果；
	lRes = CheckOrdwth2Error(con, aSHShare);
	EXPECT_EQ(0, lRes);

	con.Close();
	EzLog::i(__FUNCTION__, "\n\n");
	if (iRes != 0 || lRes != 0)
	{
		EzLog::e("", __FUNCTION__);
	}
}
