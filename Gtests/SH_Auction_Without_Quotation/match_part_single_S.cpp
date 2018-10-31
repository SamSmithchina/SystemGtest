#include "AShareCheckOrder/AShareCheckOrder.h"
#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest/gtest.h"

// 部分成交，部分撤单测试
/*# 下买单
# 检验确认消息

# 撤单
# 检验确认撤单消息
# 检查成交表和确认表  成交数量和撤单数量求和等于下单数量、等等字段)

*/


//单个测试样例；
//	SingleMatchPartWithoutQuotation_S.MatchPart_1
TEST(SingleMatchPartWithoutQuotation_S, MatchPart_1)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchPart));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	std::string strTemp;
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
	aSHShare.price = "1.100";
	aSHShare.qty = "1100";
	aSHShare.bs = "S";					//买卖
	//确认字段；
	lTemp = atoi(aSHShare.qty.c_str());
	strTemp = ltoa((lTemp - long(lTemp * 0.5)), szTemp, 10);
	aSHShare.qty2 = strTemp;
	//成交字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	lTemp = long(lTemp * 0.5);
	strTemp = ltoa(lTemp, szTemp, 10);
	aSHShare.cjsl = strTemp;
	aSHShare.cjjg = aSHShare.price;

	iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare.price, ui64Temp);	//带小数的成交价格安全转换为整形
	EXPECT_EQ(0, iRes);
	ui64Cjje = lTemp * ui64Temp;
	if (ui64Cjje > 999999999990)
	{
		aSHShare.cjje = "-1";
	}
	else
	{
		Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2); //成交金额带两位小数；
	}

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

	//查询成交结果
	lRes = CheckCjhb(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//检查撤单的确认
	lRes = CheckOrdwth2Cancel(con, aSHShare);	//这里检查部分撤单的确认表，OrderType为5， 
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
//	SingleMatchPartWithoutQuotation_S.MatchPart_2
TEST(SingleMatchPartWithoutQuotation_S, MatchPart_2)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchPart));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	std::string strTemp;
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
	aSHShare.price = "19.100";
	aSHShare.qty = "100100";
	aSHShare.bs = "S";					//买卖
	//确认字段；
	lTemp = atoi(aSHShare.qty.c_str());
	strTemp = ltoa((lTemp - long(lTemp * 0.5)), szTemp, 10);
	aSHShare.qty2 = strTemp;
	//成交字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	lTemp = long(lTemp * 0.5);
	strTemp = ltoa(lTemp, szTemp, 10);
	aSHShare.cjsl = strTemp;
	aSHShare.cjjg = aSHShare.price;

	iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare.price, ui64Temp);	//带小数的成交价格安全转换为整形
	EXPECT_EQ(0, iRes);
	ui64Cjje = lTemp * ui64Temp;
	if (ui64Cjje > 999999999990)
	{
		aSHShare.cjje = "-1";
	}
	else
	{
		Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2); //成交金额带两位小数；
	}

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

	//检查成交表和撤单表中的确认订单；
	//查询成交结果
	lRes = CheckCjhb(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//检查撤单的确认
	lRes = CheckOrdwth2Cancel(con, aSHShare);	//这里检查部分撤单的确认表，OrderType为5， 
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
//	SingleMatchPartWithoutQuotation_S.MatchPart_3
TEST(SingleMatchPartWithoutQuotation_S, MatchPart_3)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchPart));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	std::string strTemp;
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
	//确认字段；
	lTemp = atoi(aSHShare.qty.c_str());
	strTemp = ltoa((lTemp - long(lTemp * 0.5)), szTemp, 10);
	aSHShare.qty2 = strTemp;
	//成交字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	lTemp = long(lTemp * 0.5);
	strTemp = ltoa(lTemp, szTemp, 10);
	aSHShare.cjsl = strTemp;
	aSHShare.cjjg = aSHShare.price;

	iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare.price, ui64Temp);	//带小数的成交价格安全转换为整形
	EXPECT_EQ(0, iRes);
	ui64Cjje = lTemp * ui64Temp;
	if (ui64Cjje > 999999999990)
	{
		aSHShare.cjje = "-1";
	}
	else
	{
		Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2); //成交金额带两位小数；
	}

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

	//检查成交表和撤单表中的确认订单；
	//查询成交结果
	lRes = CheckCjhb(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//检查撤单的确认
	lRes = CheckOrdwth2Cancel(con, aSHShare);	//这里检查部分撤单的确认表，OrderType为5， 
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
//	SingleMatchPartWithoutQuotation_S.MatchPart_4
TEST(SingleMatchPartWithoutQuotation_S, MatchPart_4)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchPart));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	std::string strTemp;
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
	aSHShare.bs = "S";					//卖
	//确认字段；
	lTemp = atoi(aSHShare.qty.c_str());
	strTemp = ltoa((lTemp - long(lTemp * 0.5)), szTemp, 10);
	aSHShare.qty2 = strTemp;
	//成交字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	lTemp = long(lTemp * 0.5);
	strTemp = ltoa(lTemp, szTemp, 10);
	aSHShare.cjsl = strTemp;
	aSHShare.cjjg = aSHShare.price;

	iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare.price, ui64Temp);	//带小数的成交价格安全转换为整形
	EXPECT_EQ(0, iRes);
	ui64Cjje = lTemp * ui64Temp;
	if (ui64Cjje > 999999999990)
	{
		aSHShare.cjje = "-1";
	}
	else
	{
		Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2); //成交金额带两位小数；
	}

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

	//检查成交表和撤单表中的确认订单；
	//查询成交结果
	lRes = CheckCjhb(con, aSHShare);
	EXPECT_EQ(0, lRes);

	//检查撤单的确认
	lRes = CheckOrdwth2Cancel(con, aSHShare);	//这里检查部分撤单的确认表，OrderType为5， 
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
//	SingleMatchPartWithoutQuotation_S.MatchPart_5
TEST(SingleMatchPartWithoutQuotation_S, MatchPart_5)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchPart));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	int iRes = 0;
	long lRes = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	long laShareQty = 0;
	std::string strTemp;
	OTLConn40240 con;
	SHShare aSHShare;			
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
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "999.000";
	aSHShare.qty = "99999900";	//最大值
	aSHShare.bs = "S";					//卖
	//确认字段；
	lTemp = atoi(aSHShare.qty.c_str());
	strTemp = ltoa((lTemp - long(lTemp * 0.5)), szTemp, 10);
	aSHShare.qty2 = strTemp;
	//成交字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	lTemp = long(lTemp * 0.5);
	strTemp = ltoa(lTemp, szTemp, 10);
	aSHShare.cjsl = strTemp;
	aSHShare.cjjg = aSHShare.price;

	iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare.price, ui64Temp);	//带小数的成交价格安全转换为整形
	EXPECT_EQ(0, iRes);
	ui64Cjje = lTemp * ui64Temp;
	if (ui64Cjje > 999999999990)
	{
		aSHShare.cjje = "-1";
	}
	else
	{
		Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2); //成交金额带两位小数；
	}

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

		//检查成交表和撤单表中的确认订单；
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


//单个测试样例；
//	SingleMatchPartWithoutQuotation_S.MatchPart_6
TEST(SingleMatchPartWithoutQuotation_S, MatchPart_6)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchPart));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	int iRes = 0;
	long lRes = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	long laShareQty = 0;
	std::string strTemp;
	OTLConn40240 con;
	SHShare aSHShare;			
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
	aSHShare.reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
	aSHShare.rec_num = szTemp;
	aSHShare.price = "999.000";
	aSHShare.qty = "100";	//最大值
	aSHShare.bs = "S";					//买
	//确认字段；
	lTemp = atoi(aSHShare.qty.c_str());
	strTemp = ltoa((lTemp - long(lTemp * 0.5)), szTemp, 10);
	aSHShare.qty2 = strTemp;
	//成交字段
	aSHShare.gddm = aSHShare.account;
	aSHShare.zqdm = aSHShare.stock;
	lTemp = long(lTemp * 0.5);
	strTemp = ltoa(lTemp, szTemp, 10);
	aSHShare.cjsl = strTemp;
	aSHShare.cjjg = aSHShare.price;

	iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare.price, ui64Temp);	//带小数的成交价格安全转换为整形
	EXPECT_EQ(0, iRes);
	ui64Cjje = lTemp * ui64Temp;
	if (ui64Cjje > 999999999990)
	{
		aSHShare.cjje = "-1";
	}
	else
	{
		Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare.cjje, 2); //成交金额带两位小数；
	}

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

		//检查成交表和撤单表中的确认订单；
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
