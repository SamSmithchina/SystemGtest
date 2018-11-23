#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"
#include "util/EzLog.h"

//��һ��1.050Ԫ����һ��1.045Ԫ ����������ʵ���²��ֳɽ�����,  ���
// account = "A645078963" ��Ʊ�˺�
// stock = ("600370") ������
//	 BatchGtestMatchPartWithQuotation.BatchMatchPart_SellBuyPriceCheckAssetYES
TEST(BatchGtestMatchPartWithQuotation, BatchMatchPart_SellBuyPriceCheckAssetYES)
{
	//�л�ģʽ
	ASSERT_EQ(0, TransformMatchMode(SellBuyPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	//��������,
	AStockQuot aStockQuot;
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600370";
	aStockQuot.zqmc = "������";

	//��������
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//���󶩵�������
	int i = 0;
	int j = 0;
	uint64_t ui64Cjjg = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Price = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;			//aSHShare����ĳ�Ա����
	long lAShareQty[10] = { 0 };	//-1 ��ʾ�������������� �� ��Ϊ - 1 ��ʾ ����ɣ������򣬻���������������
	int iRound = 1;
	StockAsset aSHStockAsset;
	aSHStockAsset.account_id = "A645078963";
	aSHStockAsset.Init("A645078963", aStockQuot.zqdm);
	uint64_t ui64BCjsl = 0;
	uint64_t ui64SCjsl = 0;

	//�������ݿ����� ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//��ѭ����������롢ȷ�ϡ��ɽ�
	for (i = 0; i < iRound; i++)
	{
		for (j = 0; j < iAShareNum; j++)
		{
			//��ʼ����Ʊ������	//��ƶ������ͽ����
			aSHShare[j].account = aSHStockAsset.account_id;
			aSHShare[j].stock = aStockQuot.zqdm;
			g_iExternRecNum++;
			aSHShare[j].reff = "J000000000";
			itoa(g_iExternRecNum, szTemp, 10);
			aSHShare[j].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
			aSHShare[j].rec_num = szTemp;

			aSHShare[j].qty = "200000";

			if (0 == j % 2)
			{
				aSHShare[j].bs = "B";		//��
				ui64Cjjg = aStockQuot.SJW1;
				//ui64Price = ui64Cjjg + rand() % 1000;
				//for (; ui64Cjjg > ui64Price || ui64Price > aStockQuot.maxgain;) 	//���ü۸������һ�� �������Ƿ�
				//{
				//	ui64Price = ui64Cjjg + rand() % 1000;
				//}
				ui64Price = aStockQuot.maxgain - j;
			}
			else
			{
				aSHShare[j].bs = "S";		//��
				ui64Cjjg = aStockQuot.BJW1;
				//ui64Price = ui64Cjjg - rand() % 1000;
				//for (; aStockQuot.minfall > ui64Price || ui64Price > ui64Cjjg;) //�۸��ڵ��� �� ��һ��֮��	
				//{
				//	ui64Price = ui64Cjjg - rand() % 1000;
				//}
				ui64Price = aStockQuot.minfall + j;
			}
			Tgw_StringUtil::iLiToStr(ui64Price, aSHShare[j].price, 3);

			//ȷ���ֶΣ�
			aSHShare[j].qty2 = "100000";
			//�ɽ��ֶ�			
			aSHShare[j].gddm = aSHShare[j].account;
			aSHShare[j].zqdm = aSHShare[j].stock;
			aSHShare[j].cjsl = "100000";
			Tgw_StringUtil::iLiToStr(ui64Cjjg, aSHShare[j].cjjg, 3);
			lTemp = atoi(aSHShare[j].cjsl.c_str());
			ui64Cjje = lTemp * ui64Cjjg;
			if (ui64Cjje > 999999999990)
			{
				aSHShare[j].cjje = "-1";
			}
			else
			{
				Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare[j].cjje, 2); //�ɽ�������λС����
			}

			//����
			lRes = InsertOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes);
			con.Commit();

			//���볷��
			Sleep(g_iTimeOut * 5);
			lRes = InsertCancelOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes);
			con.Commit();	// commit

			//���͵ڶ������飻
			if ((iAShareNum - 1) > j)
			{
				Sleep(g_iTimeOut * 10);
				aStockQuot.cjsl += 100000;
				aStockQuot.cjje += 100000000;
				TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
				aStockQuot.hqsj += ".500";					//����
				ASSERT_EQ(0, SendQuotToRedis(aStockQuot));
				Sleep(g_iTimeOut * 50);
			}
		}

		//���
		for (j = 0; j < iAShareNum; j++)
		{
			lAShareQty[j] = 0;		//����Ϊ 0
			lTemp = atol(aSHStockAsset.stock_etf_redemption_balance.c_str());
			lTemp += atol(aSHStockAsset.stock_available.c_str());
			lAShareQty[j] = atol(aSHShare[j].qty.c_str());
			if ("S" == aSHShare[j].bs && lAShareQty[j] > lTemp)
			{
				lAShareQty[j] = -1;
				lRes = CheckOrdwth2Error(con, aSHShare[j]);
				EXPECT_EQ(0, lRes);
			}
		}

		//ȷ��
		for (j = 0; j < iAShareNum; j++)
		{
			if (-1 != lAShareQty[j])
			{
				lRes = CheckOrdwth2Match(con, aSHShare[j]);
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;

				lRes = CheckOrdwth2Cancel(con, aSHShare[j]);
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
			}
		}

		//�ɽ�
		for (j = 0; j < iAShareNum; j++)
		{
			if (-1 != lAShareQty[j])
			{
				lRes = CheckCjhb(con, aSHShare[j]);
				EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
				if (lRes == 0)
				{
					if ("B" == aSHShare[j].bs)
					{
						ui64BCjsl += strtoull(aSHShare[j].cjsl.c_str(), NULL, 10);
					}
					if ("S" == aSHShare[j].bs)
					{
						ui64SCjsl += strtoull(aSHShare[j].cjsl.c_str(), NULL, 10);
					}
				}
			}
		}
	}//for (i = 0; i < 1; i++ )	//��ѭ��

	//У���д�ɷ��ʲ�stock_asset
	Sleep(g_iTimeOut * 10); //�ȴ�tgwд��mysql����
	iRes = CheckStgwWriteAssetBackToMySQL(aSHStockAsset, ui64BCjsl, ui64SCjsl);
	EXPECT_EQ(0, iRes) << ++lErrorOrderCounter;

	if (0 < lErrorOrderCounter)
	{
		EzLog::i("=================================================", "\n");
		EzLog::e(__FUNCTION__, "\n");
		EzLog::Out("g_iTimeOut  : ", (trivial::severity_level)2, g_iTimeOut);
		EzLog::Out("iQueryTimes : ", (trivial::severity_level)2, g_iQueryTimes);
		EzLog::Out("��ִ������  ��", (trivial::severity_level)2, iRound);
		EzLog::Out("ÿ��        ��", (trivial::severity_level)2, iAShareNum);
		EzLog::Out("���� iRound * iAShareNum �� ", (trivial::severity_level)2, iRound*iAShareNum);
		EzLog::Out("���ִ��󶩵����� ��", (trivial::severity_level)2, lErrorOrderCounter);
		EzLog::i("=================================================", "\n");
	}

	//�ر�����
	con.Close();
	EzLog::i(__FUNCTION__, "\n\n");
}


// ��һ��1.050Ԫ����һ��1.045Ԫ ����������ʵ���²��ֳɽ�����,  �����
// account = "A645078963" ��Ʊ�˺�
// stock = ("600371") �����ũ
//	 BatchGtestMatchPartWithQuotation.BatchMatchPart_SellBuyPriceCheckAssetNO
TEST(BatchGtestMatchPartWithQuotation, BatchMatchPart_SellBuyPriceCheckAssetNO)
{
	//�л�ģʽ
	ASSERT_EQ(0, TransformMatchMode(SellBuyPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//��������,
	AStockQuot aStockQuot;
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600371";
	aStockQuot.zqmc = "�����ũ";

	//��������
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//���󶩵�������
	int i = 0;
	int j = 0;
	uint64_t ui64Cjjg = 0;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Price = 0;
	char szTemp[10] = { "\0" };
	long lTemp = 0;
	OTLConn40240 con;
	SHShare aSHShare[10];
	int iAShareNum = 10;			//aSHShare����ĳ�Ա����
	int iRound = 1;

	//�������ݿ����� ,0 right , -1 wrong
	iRes = con.Connect(g_strShOdbcConn);
	ASSERT_EQ(0, iRes);
	iRes = con.SetAutoCommit(0);
	ASSERT_EQ(0, iRes);

	//��ѭ����������롢ȷ�ϡ��ɽ�
	for (i = 0; i < iRound; i++)
	{
		for (j = 0; j < iAShareNum; j++)
		{
			//��ʼ����Ʊ������	//��ƶ������ͽ����
			aSHShare[j].account = "A645078963";
			aSHShare[j].stock = aStockQuot.zqdm;
			g_iExternRecNum++;
			aSHShare[j].reff = "J000000000";
			itoa(g_iExternRecNum, szTemp, 10);
			aSHShare[j].reff.replace(10 - strlen(szTemp), strlen(szTemp), szTemp);
			aSHShare[j].rec_num = szTemp;

			aSHShare[j].qty = "200000";

			if (0 == j % 2)
			{
				aSHShare[j].bs = "B";		//��
				ui64Cjjg = aStockQuot.SJW1;
				//ui64Price = ui64Cjjg + rand() % 1000;
				//for (; ui64Cjjg > ui64Price || ui64Price > aStockQuot.maxgain;) 	//���ü۸������һ�� �������Ƿ�
				//{
				//	ui64Price = ui64Cjjg + rand() % 1000;
				//}
				ui64Price = aStockQuot.maxgain - j;
			}
			else
			{
				aSHShare[j].bs = "S";		//��
				ui64Cjjg = aStockQuot.BJW1;
				//ui64Price = ui64Cjjg - rand() % 1000;
				//for (; aStockQuot.minfall > ui64Price || ui64Price > ui64Cjjg;) //�۸��ڵ��� �� ��һ��֮��	
				//{
				//	ui64Price = ui64Cjjg - rand() % 1000;
				//}
				ui64Price = aStockQuot.minfall + j;
			}
			Tgw_StringUtil::iLiToStr(ui64Price, aSHShare[j].price, 3);

			//ȷ���ֶΣ�
			aSHShare[j].qty2 = "100000";
			//�ɽ��ֶ�			
			aSHShare[j].gddm = aSHShare[j].account;
			aSHShare[j].zqdm = aSHShare[j].stock;
			aSHShare[j].cjsl = "100000";
			Tgw_StringUtil::iLiToStr(ui64Cjjg, aSHShare[j].cjjg, 3);
			lTemp = atoi(aSHShare[j].cjsl.c_str());
			ui64Cjje = lTemp * ui64Cjjg;
			if (ui64Cjje > 999999999990)
			{
				aSHShare[j].cjje = "-1";
			}
			else
			{
				Tgw_StringUtil::iLiToStr(ui64Cjje, aSHShare[j].cjje, 2); //�ɽ�������λС����
			}

			//����
			lRes = InsertOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes);
			con.Commit();

			//���볷��
			Sleep(g_iTimeOut * 5);
			lRes = InsertCancelOrder(con, aSHShare[j]);
			EXPECT_EQ(0, lRes);
			con.Commit();	// commit

			//���͵ڶ������飻
			if ((iAShareNum - 1) > j)
			{
				Sleep(g_iTimeOut * 10);
				aStockQuot.cjsl += 100000;
				aStockQuot.cjje += 100000000;
				TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
				aStockQuot.hqsj += ".500";					//����
				ASSERT_EQ(0, SendQuotToRedis(aStockQuot));
				Sleep(g_iTimeOut * 50);
			}
		}

		//ȷ��
		for (j = 0; j < iAShareNum; j++)
		{
			lRes = CheckOrdwth2Match(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;

			lRes = CheckOrdwth2Cancel(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

		//�ɽ�
		for (j = 0; j < iAShareNum; j++)
		{
			lRes = CheckCjhb(con, aSHShare[j]);
			EXPECT_EQ(0, lRes) << "num =  " << i*iAShareNum + j << "\t lErrorOrderCounter = " << ++lErrorOrderCounter;
		}

	}//for (i = 0; i < 1; i++ )	//��ѭ��

	if (0 < lErrorOrderCounter)
	{
		EzLog::i("=================================================", "\n");
		EzLog::e(__FUNCTION__, "\n");
		EzLog::Out("g_iTimeOut  : ", (trivial::severity_level)2, g_iTimeOut);
		EzLog::Out("iQueryTimes : ", (trivial::severity_level)2, g_iQueryTimes);
		EzLog::Out("��ִ������  ��", (trivial::severity_level)2, iRound);
		EzLog::Out("ÿ��        ��", (trivial::severity_level)2, iAShareNum);
		EzLog::Out("���� iRound * iAShareNum �� ", (trivial::severity_level)2, iRound*iAShareNum);
		EzLog::Out("���ִ��󶩵����� ��", (trivial::severity_level)2, lErrorOrderCounter);
		EzLog::i("=================================================", "\n");
	}
	//�ر�����
	con.Close();
	EzLog::i(__FUNCTION__, "\n\n");
}
