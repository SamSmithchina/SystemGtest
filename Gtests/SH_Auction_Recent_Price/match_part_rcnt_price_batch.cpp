#include "AShareCheckOrder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "SystemGtestConfigs/configs.h"
#include "gtest/gtest.h"
#include "util/EzLog.h"

// ����ɽ���1.000Ԫ ����������ʵ���²��ֳɽ�����,  ���
// account = "A645078963" ��Ʊ�˺�
// stock = ("600368") ���޽�ͨ
//	 BatchGtestMatchPartWithQuotation.BatchMatchPart_RecentPriceCheckAssetYES
TEST(BatchGtestMatchPartWithQuotation, BatchMatchPart_RecentPriceCheckAssetYES)
{
	//�л�ģʽ
	ASSERT_EQ(0, TransformMatchMode(RecentPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetYES));

	//��������,
	AStockQuot aStockQuot;
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600368";
	aStockQuot.zqmc = "���޽�ͨ";

	//��������
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//���󶩵�������
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
	int iAShareNum = 10;			//aSHShare����ĳ�Ա����
	long lAShareQty[10] = { 0 };	//-1 ��ʾ�������������� �� ��Ϊ - 1 ��ʾ ����ɣ������򣬻���������������
	int iRound = 1;
	StockAsset aSHStockAsset;
	aSHStockAsset.account_id = "A645078963";
	aSHStockAsset.Init("A645078963", aStockQuot.zqdm);

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
			ui64Cjjg = aStockQuot.zjjg;

			if (0 == g_iExternRecNum % 2)
			{
				aSHShare[j].bs = "B";		//��
				ui64Price = ui64Cjjg + rand() % (uint64_t)(ui64Cjjg * 0.1);	//���ڵ�������ɽ��۸񣬲������Ƿ�
			}
			else
			{
				aSHShare[j].bs = "S";		//��
				ui64Price = ui64Cjjg - rand() % (uint64_t)(ui64Cjjg *0.1);		//���ڵ�������ɽ���,�����ڵ���
			}
			Tgw_StringUtil::iLiToStr(ui64Price, aSHShare[j].price, 3);

			//ȷ���ֶΣ�
			aSHShare[j].qty2 = "100000";
			//�ɽ��ֶ�			
			aSHShare[j].gddm = aSHShare[j].account;
			aSHShare[j].zqdm = aSHShare[j].stock;
			aSHShare[j].cjsl = "100000";
			Tgw_StringUtil::iLiToStr(ui64Cjjg, aSHShare[j].cjjg, 3);
			lTemp = atoi(aSHShare[j].qty.c_str());
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
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;

			//���볷��
			Sleep(g_iTimeOut * 10);
			lRes = InsertCancelOrder(con, aSHShare[j]);
			con.Commit();

			//���͵ڶ������飻
			aStockQuot.cjsl += 200000;
			aStockQuot.cjje += 200000000;
			TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
			aStockQuot.hqsj += ".500";					//����
			ASSERT_EQ(0, SendQuotToRedis(aStockQuot));
			Sleep(g_iTimeOut * 50);
		}

		//���
		for (j = 0; j < iAShareNum; j++)
		{
			lAShareQty[j] = 0;		//����Ϊ 0
			lTemp = atol(aSHStockAsset.stock_etf_redemption_balance.c_str());
			lTemp += atol(aSHStockAsset.stock_available.c_str());
			lAShareQty[j] = atol(aSHShare[j].cjsl.c_str());
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
			}
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


//����ɽ���1.000Ԫ ����������ʵ���²��ֳɽ�����,  �����
// account = "A645078963" ��Ʊ�˺�
// stock = ("600369") ����֤ȯ
//	 BatchGtestMatchPartWithQuotation.BatchMatchPart_RecentPriceCheckAssetNO
TEST(BatchGtestMatchPartWithQuotation, BatchMatchPart_RecentPriceCheckAssetNO)
{
	//�л�ģʽ
	ASSERT_EQ(0, TransformMatchMode(RecentPrice));
	ASSERT_EQ(0, TransformMatchMode(CheckAssetNO));

	//��������,
	AStockQuot aStockQuot;
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600369";
	aStockQuot.zqmc = "����֤ȯ";

	//��������
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	int iRes = 0;
	long lRes = 0;
	long lErrorOrderCounter = 0;		//���󶩵�������
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
			ui64Cjjg = aStockQuot.zjjg;

			if (0 == g_iExternRecNum % 2)
			{
				aSHShare[j].bs = "B";		//��
				ui64Price = ui64Cjjg + rand() % (uint64_t)(ui64Cjjg * 0.1);	//���ڵ�������ɽ��۸񣬲������Ƿ�
			}
			else
			{
				aSHShare[j].bs = "S";		//��
				ui64Price = ui64Cjjg - rand() % (uint64_t)(ui64Cjjg *0.1);		//���ڵ�������ɽ���,�����ڵ���
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
			EXPECT_EQ(0, lRes) << i*iAShareNum + j;

			//���볷��
			Sleep(g_iTimeOut * 10);
			lRes = InsertCancelOrder(con, aSHShare[j]);
			con.Commit();

			//���͵ڶ������飻
			aStockQuot.cjsl += 200000;
			aStockQuot.cjje += 200000000;
			TimeStringUtil::GetCurrTimeInTradeType(aStockQuot.hqsj);
			aStockQuot.hqsj += ".500";					//����
			ASSERT_EQ(0, SendQuotToRedis(aStockQuot));
			Sleep(g_iTimeOut * 50);

		}
		con.Commit();	// commit

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
