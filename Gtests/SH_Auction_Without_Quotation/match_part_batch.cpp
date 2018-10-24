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

//批量测试样例；
//	BatchPartMatchWithoutQuotation.CheckAssetNO
TEST(BatchPartMatchWithoutQuotation, CheckAssetNO)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchPart));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	int iRes = 0;
	long lRes = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int n = 0;
	long lErrorOrderCounter = 0;
	long lTemp = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	char szTemp[12] = { "\0" };
	std::string strTemp = "";
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;		//aSHShare[j]数组的成员数量
	int iRound = 1;

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	//	ASSERT_EQ(0, iRes);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//主循环体包含插入、确认、撤单、成交检查、撤单检查
	for (i = 0; i < iRound; i++)
	{
		//插入
		for (j = 0; j < iAShareNum; j++)
		{
			//初始化股票订单；
			g_iExternRecNum++;
			aSHShare[j].reff = "J000000000";
			itoa(g_iExternRecNum, szTemp, 10);
			aSHShare[j].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
			aSHShare[j].rec_num = szTemp;
			//aSHShare[j].account = "A645078963";		//股票账号
			//aSHShare[j].stock = "600302";				// 证券代码
			if (0 == g_iExternRecNum % 2)
			{
				aSHShare[j].bs = "B";					//买
			}
			else
			{
				aSHShare[j].bs = "S";					//卖
			}
			lTemp = g_iExternRecNum * 100;
			if (lTemp > 99999900)
			{
				lTemp = lTemp % 100000000;				//char qty[8]
			}
			aSHShare[j].qty = itoa(lTemp, szTemp, 10);
			lTemp = g_iExternRecNum;
			if (lTemp > 9999)
			{
				lTemp = g_iExternRecNum % 10000;
			}
			aSHShare[j].price = itoa(lTemp, szTemp, 10); //整数部分最大9999，后有小数点和三位小数；
			aSHShare[j].price += ".";
			if (lTemp > 999)
			{
				lTemp = g_iExternRecNum % 1000;			//价格三位小数，最大999，
			}
			aSHShare[j].price += itoa(lTemp, szTemp, 10);	//小数部分
			if (lTemp < 100)
			{
				//两位数补充末位的零
				aSHShare[j].price += "0";
				if (lTemp < 10)
				{
					aSHShare[j].price += "0";//一位数再补充0；
				}
			}

			//确认字段；
			lTemp = atoi(aSHShare[j].qty.c_str());
			strTemp = ltoa((lTemp - long(lTemp * 0.5)), szTemp, 10);
			aSHShare[j].qty2 = strTemp;
			//成交字段			
			aSHShare[j].gddm = aSHShare[j].account;
			aSHShare[j].zqdm = aSHShare[j].stock;
			lTemp = long(lTemp * 0.5);
			strTemp = ltoa(lTemp, szTemp, 10);
			aSHShare[j].cjsl = strTemp;
			aSHShare[j].cjjg = aSHShare[j].price;

			iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare[j].price, ui64Temp);	//带小数的成交价格安全转换为整形
			EXPECT_EQ(0, iRes);
			ui64Cjje = lTemp * ui64Temp;
			if (ui64Cjje > 999999999990)
			{
				aSHShare[j].cjje = "-1";
			}
			else
			{
				Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare[j].cjje, 2); //成交金额带两位小数；
			}

			//插入测试样例
			lRes = InsertOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes);
		}// for (j = 0; j < iAShareNum; j++)
		con.Commit();

		//检查确认
		for (j = 0; j < iAShareNum; j++)
		{
			lRes = CheckOrdwth2Match(con, aSHShare[j]); 
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

		//插入撤单
		for (j = 0; j < iAShareNum; j++)
		{
			lRes = InsertCancelOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}
		con.Commit();

		for (j = 0; j < iAShareNum; j++)
		{
			//查询成交结果
			lRes = CheckCjhb(con, aSHShare[j]);		
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;

			//检查撤单的确认
			lRes = CheckOrdwth2Cancel(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

	}//for (i = 0; i < iRound; i++)

	if (0 < lErrorOrderCounter)
	{
		EzLog::i("=================================================", "\n");
		EzLog::e("", __FUNCTION__);
		EzLog::Out("g_iTimeOut    : ", (trivial::severity_level)2, g_iTimeOut);
		EzLog::Out("iQueryTimes : ", (trivial::severity_level)2, g_iQueryTimes);
		EzLog::Out("共执行组数  ：", (trivial::severity_level)2, iRound);
		EzLog::Out("每组        ：", (trivial::severity_level)2, iAShareNum);
		EzLog::Out("共计 iRound * iAShareNum ： ", (trivial::severity_level)2, iRound*iAShareNum);
		EzLog::Out("出现错误订单笔数 ：", (trivial::severity_level)2, lErrorOrderCounter);
		EzLog::i("=================================================", "\n");
	}

	con.Close();
	EzLog::i("", __FUNCTION__);
}

//批量测试样例；
//	BatchPartMatchWithoutQuotation.CheckAssetYES
TEST(BatchPartMatchWithoutQuotation, CheckAssetYES)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(SimulMatchPart));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	int iRes = 0;
	long lRes = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int n = 0;
	long lErrorOrderCounter = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64BCjsl = 0;
	uint64_t ui64SCjsl = 0;
	char szTemp[12] = { "\0" };
	std::string strTemp = "";
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;		//aSHShare[j]数组的成员数量
	int iRound = 1;
	long lAShareQty[10] = { 0 };		//-1 表示卖单数量不合理 ， 不为 - 1 表示 不验股，或者买，或者卖的数量合理
	StockAsset aSHStockAsset;
	aSHStockAsset.Init("A645078963", "600302");

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	//	ASSERT_EQ(0, iRes);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//主循环体包含插入、确认、撤单、成交检查、撤单检查
	for (i = 0; i < iRound; i++)
	{
		//插入
		for (j = 0; j < iAShareNum; j++)
		{
			//初始化股票订单；
			g_iExternRecNum++;
			aSHShare[j].reff = "J000000000";
			itoa(g_iExternRecNum, szTemp, 10);
			aSHShare[j].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);	//订单编号；利用静态变量保持rec_num从1递增；
			aSHShare[j].rec_num = szTemp;
			//aSHShare[j].account = "A645078963";		//股票账号
			//aSHShare[j].stock = "600302";				// 证券代码
			if (0 == g_iExternRecNum % 2)
			{
				aSHShare[j].bs = "B";					//买
			}
			else
			{
				aSHShare[j].bs = "S";					//卖
			}
			lTemp = g_iExternRecNum * 100;
			if (lTemp > 99999900)
			{
				lTemp = lTemp % 100000000;				//char qty[8]
			}
			aSHShare[j].qty = itoa(lTemp, szTemp, 10);
			lTemp = g_iExternRecNum;
			if (lTemp > 9999)
			{
				lTemp = g_iExternRecNum % 10000;
			}
			aSHShare[j].price = itoa(lTemp, szTemp, 10); //整数部分最大9999，后有小数点和三位小数；
			aSHShare[j].price += ".";
			if (lTemp > 999)
			{
				lTemp = g_iExternRecNum % 1000;		     //价格三位小数，最大999，
			}
			aSHShare[j].price += itoa(lTemp, szTemp, 10);	//小数部分
			if (lTemp < 100)
			{
				//两位数补充末位的零
				aSHShare[j].price += "0";
				if (lTemp < 10)
				{
					aSHShare[j].price += "0";//一位数再补充0；
				}
			}

			//确认字段；
			lTemp = atoi(aSHShare[j].qty.c_str());
			strTemp = ltoa((lTemp - long(lTemp * 0.5)), szTemp, 10);
			aSHShare[j].qty2 = strTemp;
			//成交字段			
			aSHShare[j].gddm = aSHShare[j].account;
			aSHShare[j].zqdm = aSHShare[j].stock;
			lTemp = long(lTemp * 0.5);
			strTemp = ltoa(lTemp, szTemp, 10);
			aSHShare[j].cjsl = strTemp;
			aSHShare[j].cjjg = aSHShare[j].price;

			iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(aSHShare[j].price, ui64Temp);	//带小数的成交价格安全转换为整形
			EXPECT_EQ(0, iRes);
			ui64Cjje = lTemp * ui64Temp;
			if (ui64Cjje > 999999999990)
			{
				aSHShare[j].cjje = "-1";
			}
			else
			{
				Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare[j].cjje, 2); //成交金额带两位小数；
			}

			//插入测试样例
			lRes = InsertOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes);
		}// for (j = 0; j < iAShareNum; j++)
		con.Commit();

		// 验股
		for (j = 0; j< iAShareNum; j++)
		{
			lAShareQty[j] = 0;
			lTemp = atol(aSHStockAsset.stock_etf_redemption_balance.c_str());
			lTemp += atol(aSHStockAsset.stock_available.c_str());
			lAShareQty[j] = atol(aSHShare[j].cjsl.c_str());
			if ("S" == aSHShare[j].bs && lAShareQty[j] > lTemp)
			{
				lAShareQty[j] = -1;
				iRes = CheckOrdwth2Error(con, aSHShare[j]);
				EXPECT_EQ(0, iRes);
			}
		}

		//检查确认
		for (j = 0; j < iAShareNum; j++)
		{
			if (-1 != lAShareQty[j])
			{
				lRes = CheckOrdwth2Match(con, aSHShare[j]); 
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
			}
		}

		//插入撤单
		for (j = 0; j < iAShareNum; j++)
		{
			if (-1 != lAShareQty[j])
			{
				lRes = InsertCancelOrder(con, aSHShare[j]);
				EXPECT_EQ(0, lRes) << i*iAShareNum + j;
			}
		}
		con.Commit();

		for (j = 0; j < iAShareNum; j++)
		{
			if (-1 != lAShareQty[j])
			{
				//查询成交结果
				lRes = CheckCjhb(con, aSHShare[j]);		
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;

				//检查撤单的确认
				lRes = CheckOrdwth2Cancel(con, aSHShare[j]);	
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;

				if ("S" == aSHShare[j].bs)
				{
					ui64SCjsl += strtoull(aSHShare[j].cjsl.c_str(), NULL, 10);
				}
				if ("B" == aSHShare[j].bs)
				{
					ui64BCjsl += strtoull(aSHShare[j].cjsl.c_str(), NULL, 10);
				}
			}
		}

	}//for (i = 0; i < iRound; i++)
	con.Close();

	//校验回写股份资产stock_asset
	iRes = CheckStgwWriteAssetBackToMySQL(aSHStockAsset, ui64BCjsl, ui64SCjsl);
	EXPECT_EQ(0, iRes);

	if (0 < lErrorOrderCounter)
	{
		EzLog::i("=================================================", "\n");
		EzLog::e("", __FUNCTION__);
		EzLog::Out("g_iTimeOut    : ", (trivial::severity_level)2, g_iTimeOut);
		EzLog::Out("iQueryTimes : ", (trivial::severity_level)2, g_iQueryTimes);
		EzLog::Out("共执行组数  ：", (trivial::severity_level)2, iRound);
		EzLog::Out("每组        ：", (trivial::severity_level)2, iAShareNum);
		EzLog::Out("共计 iRound * iAShareNum ： ", (trivial::severity_level)2, iRound*iAShareNum);
		EzLog::Out("出现错误订单笔数 ：", (trivial::severity_level)2, lErrorOrderCounter);
		EzLog::i("=================================================", "\n");
	}
	EzLog::i("", __FUNCTION__);
}
