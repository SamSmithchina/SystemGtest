#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"
#include "util/EzLog.h"
#include <boost/log/trivial.hpp>

//区间段均价1.000元 ，批量处理实盘下错误价格订单,  不验股
// account = "A645078963" 股票账号
// stock = ("600312") 平高电气
//	 BatchGtestErrorWithQuotation.BatchError_AveragePriceCheckAssetNO
TEST(BatchGtestErrorWithQuotation, BatchError_AveragePriceCheckAssetNO)
{
	//切换错单模式，开启行情
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//构造行情,
	AStockQuot aStockQuot;
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600312";
	aStockQuot.zqmc = "平高电气";
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
			if (lTemp > 100000)
			{
				lTemp = lTemp % 100000;
			}
			aSHShare[j].qty = itoa(lTemp, szTemp, 10);
			CalcAvePrice(aStockQuot, ui64Cjjg);		//cjjg 
			if (0 == j % 2)
			{
				aSHShare[j].bs = "B";		//买
			}
			else
			{
				aSHShare[j].bs = "S";		//卖
			}
			ui64Price = rand() % 10000000; //price char[8]包含小数点，实际数字7位
			for (; aStockQuot.minfall <= ui64Price && ui64Price <= aStockQuot.maxgain;) 	//设置价格高于涨幅或低于跌幅
			{
				ui64Price = rand() % 10000000;
			}
			Tgw_StringUtil::iLiToStr(ui64Price, aSHShare[j].price, 3);

			//确认字段；
			//成交字段			

			//插入
			lRes = InsertOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}
		con.Commit();	// commit

		//确认错单
		for (j = 0; j < iAShareNum; j++)
		{
			//比较确认结果；
			lRes = CheckOrdwth2Error(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

	}//for (i = 0; i < 1; i++ )	//主循环

	//关闭连接
	con.Close();

	if (0 < lErrorOrderCounter)
	{
		char szTransferBuff[65] = { "\0" };
		std::string strError = "=================================================\n";
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

//区间段均价1.000元 ，批量处理实盘下不合理价格订单, 验股时卖单数量超出股份持有数量，导致错单
// account = "A645078963" 股票账号
// stock = ("600333") 长春燃气
//	 BatchGtestErrorWithQuotation.BatchError_AveragePriceCheckAssetYES
TEST(BatchGtestErrorWithQuotation, BatchError_AveragePriceCheckAssetYES)
{
	//切换错单模式，开启行情
	ASSERT_EQ(0, TransformMatchMode(AveragePrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	//构造行情,
	AStockQuot aStockQuot;
	CreateQuotation(aStockQuot);
	aStockQuot.zqdm = "600333";
	aStockQuot.zqmc = "长春燃气";
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
			if (lTemp > 100000)
			{
				lTemp = lTemp % 100000;
			}
			aSHShare[j].qty = itoa(lTemp, szTemp, 10);
			CalcAvePrice(aStockQuot, ui64Cjjg);		//cjjg 

			aSHShare[j].bs = "S";		//卖

			ui64Price = rand() % 10000000; //price char[8]包含小数点，实际数字7位
			for (; aStockQuot.minfall <= ui64Price && ui64Price <= aStockQuot.maxgain;) 	//设置价格高于涨幅或低于跌幅
			{
				ui64Price = rand() % 10000000;
			}
			Tgw_StringUtil::iLiToStr(ui64Price, aSHShare[j].price, 3);

			//确认字段；
			//成交字段			

			//插入
			lRes = InsertOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;
		}
		con.Commit();	// commit

		//确认错单
		for (j = 0; j < iAShareNum; j++)
		{
			//比较确认结果；
			lRes = CheckOrdwth2Error(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

	}//for (i = 0; i < 1; i++ )	//主循环

	//关闭连接
	con.Close();

	if (0 < lErrorOrderCounter)
	{
		char szTransferBuff[65] = { "\0" };
		std::string strError = "=================================================\n";
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