#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"
#include "util/EzLog.h"


// 挂单撤单 ，对应的股票最近成交数量0，订单理应挂单，之后撤下，
//或者买单价格在跌幅和卖一价之间，卖单价格在买一价到涨幅之间，订单不成交，之后撤下
// 最近成交价1.000元，不验股；
// account = "A645078963" 股票账号
// stock = ("600311") 荣华实业
//	 BatchGtestNoMatchCancelWithQuotation.BatchNoMatchCancel_RecentPriceCheckAssetNO
TEST(BatchGtestNoMatchCancelWithQuotation, BatchNoMatchCancel_RecentPriceCheckAssetNO)
{
	//切换模式
	ASSERT_EQ(0, TransformMatchMode(RecentPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情,
	AStockQuot aStockQuot;
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600311";
	aStockQuot.zqmc = "荣华实业";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	int i = 0;
	int j = 0;
	int k = 0;
	uint64_t ui64Cjjg = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Price = 0;
	double dCjje = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;			//aSHShare数组的成员数量
	long lAShareQty[10] = { 0 };	//-1 表示卖单数量不合理 ， 不为 - 1 表示 不验股，或者买，或者卖的数量合理
	int iRound = 1;
	std::string strTemp = "";

	//建立数据库连接 ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//主循环体包含插入、确认、成交
	for (i = 0; i < iRound; i++)
	{
		for (j = 0; j < iAShareNum; j++)
		{
			//初始化股票订单；	//设计订单集和结果集
			aSHShare[j].account = "A645078963";
			aSHShare[j].stock = aStockQuot.zqdm;
			g_iExternRecNum++;
			aSHShare[j].reff = "J000000000";
			itoa(g_iExternRecNum, szTemp, 10);
			aSHShare[j].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
			aSHShare[j].rec_num = szTemp;

			lTemp = g_iExternRecNum * 100;
			if (lTemp > 99999900)
			{
				lTemp = lTemp % 100000000;	//char qty[8]
			}
			aSHShare[j].qty = itoa(lTemp, szTemp, 10);
			ui64Cjjg = aStockQuot.zjjg;
			if (0 == g_iExternRecNum % 2)
			{
				aSHShare[j].bs = "B";		//买
				ui64Price = ui64Cjjg + rand() % (uint64_t)(ui64Cjjg * 0.1);	//高于等于最近成交价，不高于涨幅
			}
			else
			{
				aSHShare[j].bs = "S";		//卖
				ui64Price = ui64Cjjg - rand() % (uint64_t)(ui64Cjjg *0.1);		//低于等于最近成交价,不低于跌幅
			}
			Tgw_StringUtil::iLiToStr(ui64Price, aSHShare[j].price, 3);

			//确认字段；
			aSHShare[j].qty2 = aSHShare[j].qty;

			//成交字段			

			//插入
			if (0 == i && 0 == j)
			{
				strTemp = aSHShare[0].qty;
				aSHShare[0].qty = "100000";
				lRes = InsertOrder(con, aSHShare[j]);	//消耗行情容量
				EXPECT_EQ(0, lRes);

				g_iExternRecNum++;
				aSHShare[0].reff = "J000000000";
				itoa(g_iExternRecNum, szTemp, 10);
				aSHShare[0].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
				aSHShare[0].rec_num = szTemp;
				aSHShare[0].qty = strTemp;
			}
			lRes = InsertOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}
		con.Commit();	// commit

		//插入撤单
		for (j = 0; j < iAShareNum; j++)
		{
			lRes = InsertCancelOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}
		con.Commit();

		//成交
		//挂单撤单确认
		for (j = 0; j < iAShareNum; j++)
		{
			iRes = CheckOrdwth2Cancel(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

	}//for (i = 0; i < 1; i++ )	//主循环

	if (0 < lErrorOrderCounter)
	{
		EzLog::i("=================================================", "\n");
		EzLog::e("", __FUNCTION__);
		EzLog::Out("g_iTimeOut  : ", (trivial::severity_level)2, g_iTimeOut);
		EzLog::Out("iQueryTimes : ", (trivial::severity_level)2, g_iQueryTimes);
		EzLog::Out("共执行组数  ：", (trivial::severity_level)2, iRound);
		EzLog::Out("每组        ：", (trivial::severity_level)2, iAShareNum);
		EzLog::Out("共计 iRound * iAShareNum ： ", (trivial::severity_level)2, iRound*iAShareNum);
		EzLog::Out("出现错误订单笔数 ：", (trivial::severity_level)2, lErrorOrderCounter);
		EzLog::i("=================================================", "\n");
	}
	//关闭连接
	con.Close();
	EzLog::i(__FUNCTION__, "\n\n");
}