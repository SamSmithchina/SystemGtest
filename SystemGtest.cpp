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
	time_t tBeginCounter;
	time_t tEndCounter;
	time_t tBeginGTest;
	time_t tEndGTest;
	int iExternRecNum = 0;

	tBeginCounter = clock();
	{
		//初始化日志
		EzLog::Init_log_ern(".\\SystemGtest\\test_data\\gtest\\log_SystemGtest.ini");

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

		//MySQL 守护进程
		MySqlCnnC602_Guard::MysqlLibraryInit();

		//守护进程
		OTLConn_Guard::OTLConn_Init();
		

		EzLog::i("================================", "\n");
		EzLog::i("SystemGtest", "");
		EzLog::Out("Bit ", (trivial::severity_level)2, (int)(8 * sizeof(int *)));

		srand((unsigned)time(NULL));

		//RUN_ALL_TESTS
		tBeginGTest = clock();
		testing::InitGoogleTest(&argc, argv);
		RUN_ALL_TESTS();
		tEndGTest = clock();

		//写g_iExternRecNum;
		if (-1 == WriteConfig())
		{
			EzLog::e("systemgtest.ini 写回数据出错", "\n");
		}

		MySqlCnnC602_Guard::MysqlLibraryEnd();
	}

	//计时
	tEndCounter = clock();
	EzLog::i("=================================================", "\n");
	EzLog::Out("测试程序耗时ms = ", (trivial::severity_level)2, (long)(tEndGTest - tBeginGTest));
	EzLog::Out("程 序 耗 时 ms = ", (trivial::severity_level)2, (long)(tEndCounter - tBeginCounter));
	EzLog::Out("执行测试样例 = ", (trivial::severity_level)2, g_iExternRecNum - iExternRecNum);
	EzLog::i("=================================================", "\n");
	//system("pause");
	return 0;
}

