#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"
#include "util/EzLog.h"


// 挂单撤单 ，对应的股票最近成交数量0，订单理应挂单，之后撤下，
//或者买单价格在跌幅和卖一价之间，卖单价格在买一价到涨幅之间，订单不成交，之后撤下
// 区间段均价1.000元，不验股；
// account = "A645078963" 股票账号
// stock = ("600396") 金山股份
//	 BatchGtestNoMatchCancelWithQuotation.BatchNoMatchCancel_AveragePriceCheckAssetNO
TEST(BatchGtestNoMatchCancelWithQuotation, BatchNoMatchCancel_AveragePriceCheckAssetNO)
{
	//切换全部成交模式，开启行情
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情,
	AStockQuot aStockQuot;
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600396";
	aStockQuot.zqmc = "金山股份";

	//推送行情
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//错误订单计数器
	int i = 0;
	int j = 0;
	uint64_t ui64Cjjg = 0;
	uint64_t ui64Price = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;			//aSHShare数组的成员数量
	int iRound = 1;
	std::string strTemp;

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

			lTemp = 100000 + j * 100;
			aSHShare[j].qty = itoa(lTemp, szTemp, 10);
			CalcAvePrice(aStockQuot, ui64Cjjg);
			if (0 == j % 2)
			{
				aSHShare[j].bs = "B";		//买
				ui64Price = ui64Cjjg + rand() % (uint64_t)(ui64Cjjg * 0.1);	//高于等于均价，不高于涨幅
			}
			else
			{
				aSHShare[j].bs = "S";		//卖
				ui64Price = ui64Cjjg - rand() % (uint64_t)(ui64Cjjg *0.1);		//低于等于均价,不低于跌幅
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
		Sleep(g_iTimeOut * 10);
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
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

	}//for (i = 0; i < 1; i++ )	//主循环

	//关闭连接
	con.Close();
	if (0 < lErrorOrderCounter)
	{
		char szTransferBuff[65] = { "\0" };
		std::string strError = "\n=================================================\n";
		strError += __FUNCTION__;
		strError += "\n共计 iRound * iAShareNum ： ";
		strError += itoa(iRound*iAShareNum, szTransferBuff, 10);
		strError += "\n出现错误订单笔数 ：";
		strError += ltoa(lErrorOrderCounter, szTransferBuff, 10);
		strError += "\n=================================================\n";
		EzLog::e(strError, "");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n\n");
	}
}