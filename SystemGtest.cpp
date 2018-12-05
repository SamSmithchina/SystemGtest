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
		//初始化日志
		EzLog::Init_log_ern(".\\SystemGtest\\test_data\\gtest\\log_SystemGtest.ini");
		
		//MySQL 守护进程
		MySqlCnnC602_Guard::MysqlLibraryInit();

		//守护进程
		OTLConn_Guard::OTLConn_Init();

		//读入配置
		if (-1 == ReadConfig())
		{
			//std::cout << "systemgtest.ini 配置文件读取错误 \n";
			exit(0);
		}
		
		//读取订单编号
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
			EzLog::i("获取当前路径 ： ", szPath);
		}
		else
		{
			EzLog::e("获取当前路径 ", " 失败 !");
			exit(-1);
		}
		
		//加载redis并初始化
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

		//写g_iExternRecNum;
		if (-1 == WriteConfig())
		{
			EzLog::e("systemgtest.ini 写回数据出错", "\n");
		}

		StopRedis();
		MySqlCnnC602_Guard::MysqlLibraryEnd();
	}

	//计时
	tEndTimer = clock();
	char szTemp[65] = { "\0" };
	long  lTimeCounter;
	int iExecuteExamples;
	std::string strInfo = "\n=================================================";
	strInfo += "\n测试程序耗时   ms = ";
	lTimeCounter = (long)(tEndGTestTimer - tBeginGTestTimer) * 1000 / CLOCKS_PER_SEC;
	strInfo += ltoa(lTimeCounter, szTemp, 10);

	strInfo += "\n程 序 耗 时    ms = ";
	lTimeCounter = (long)(tEndTimer - tBeginTimer) * 1000 / CLOCKS_PER_SEC;
	strInfo += ltoa(lTimeCounter, szTemp, 10);

	strInfo += "\n执 行 测 试 样 例 = ";
	iExecuteExamples = g_iExternRecNum - iExternRecNum;
	strInfo += itoa(iExecuteExamples, szTemp, 10);
	EzLog::i(strInfo, "\n=================================================\n");
	//system("pause");
	return 0;
}

