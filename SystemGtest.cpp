#include "SystemGtestConfigs/configs.h"
#include "MatchMode/MatchMode.h"
#include "tool_odbc/OTLConn40240.h"
#include "tool_mysql/MySqlCnnC602.h"
#include "boost/property_tree/ini_parser.hpp"
#include "gtest/gtest.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <Windows.h>

int main(int argc, char* argv[])
{
	clock_t tBeginTimer;
	clock_t tEndTimer;
	clock_t tBeginGTestTimer;
	clock_t tEndGTestTimer;
	int iExternRecNum = 0;

	tBeginTimer = clock();
	{
		//��ʼ����־
		EzLog::Init_log_ern(".\\SystemGtest\\test_data\\gtest\\log_SystemGtest.ini");
		
		//MySQL �ػ�����
		MySqlCnnC602_Guard::MysqlLibraryInit();

		//�ػ�����
		OTLConn_Guard::OTLConn_Init();

		//��������
		if (-1 == ReadConfig())
		{
			//std::cout << "systemgtest.ini �����ļ���ȡ���� \n";
			exit(0);
		}
		
		//��ȡ�������
		int iRes = ReadMaxRecNumFromDB();
		if (iRes == -1)
		{
			EzLog::e(__FUNCTION__, " read max rec_num from sql server failed !");
			Sleep(g_iTimeOut * 10);
			exit(-1);
		}
		iExternRecNum = g_iExternRecNum;

		char szPath[256] = { "\0" };
		DWORD lDRes = GetCurrentDirectory(256, szPath);	//system("echo %cd%");
		if (0 != lDRes)
		{
			EzLog::i("��ȡ��ǰ·�� �� ", szPath);
		}
		else
		{
			EzLog::e("��ȡ��ǰ·�� ", " ʧ�� !");
			exit(-1);
		}
		
		//����redis����ʼ��
		iRes = InitRedis();
		if (iRes == -1)
		{
			EzLog::e(__FUNCTION__, " load redis dll and connect  failed !");
			Sleep(g_iTimeOut * 10);
			exit(-1);
		}

		EzLog::Out("================================\n"
			"SystemGtest Bit ", (trivial::severity_level)2, (int)(8 * sizeof(int *)));

		srand((unsigned)time(NULL));

		//RUN_ALL_TESTS
		tBeginGTestTimer = clock();
		testing::InitGoogleTest(&argc, argv);
		RUN_ALL_TESTS();
		tEndGTestTimer = clock();

		//дg_iExternRecNum;
		if (-1 == WriteConfig())
		{
			EzLog::e("systemgtest.ini д�����ݳ���", "\n");
		}

		StopRedis();
		MySqlCnnC602_Guard::MysqlLibraryEnd();
	}

	//��ʱ
	tEndTimer = clock();
	char szTemp[65] = { "\0" };
	long  lTimeCounter;
	int iExecuteExamples;
	std::string strInfo = "\n=================================================";
	strInfo += "\n���Գ����ʱ   ms = ";
	lTimeCounter = (long)(tEndGTestTimer - tBeginGTestTimer) * 1000 / CLOCKS_PER_SEC;
	strInfo += ltoa(lTimeCounter, szTemp, 10);

	strInfo += "\n�� �� �� ʱ    ms = ";
	lTimeCounter = (long)(tEndTimer - tBeginTimer) * 1000 / CLOCKS_PER_SEC;
	strInfo += ltoa(lTimeCounter, szTemp, 10);

	strInfo += "\nִ �� �� �� �� �� = ";
	iExecuteExamples = g_iExternRecNum - iExternRecNum;
	strInfo += itoa(iExecuteExamples, szTemp, 10);
	EzLog::i(strInfo, "\n=================================================\n");
	//system("pause");
	return 0;
}

