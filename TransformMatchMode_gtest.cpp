#include "MatchMode/MatchMode.h"
#include "SystemGtestConfigs\configs.h"
#include "gtest\gtest.h"
#include "util/EzLog.h"

#include <iostream>
#include <Windows.h>
#include <time.h>
//#include <tchar.h>

//   TransformMatchMode.Error
TEST(TransformMatchMode, Error)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD iDRes = 0;

	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));	//"cd /d %~dp0"
	system("echo %cd%");	//simutgw_win64

	char TransformCMD[256] = { "\0" };
	//std::string strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_serr.txt");
	CreateProcess(NULL, TransformCMD, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	GetExitCodeProcess(pi.hProcess, &iDRes);
	EXPECT_EQ(iDRes, 0);
	EzLog::i(__FUNCTION__, "\n\n");
	if (iDRes != 0)
	{
		EzLog::e(__FUNCTION__," Transform to MatchAllWithoutQuotation  failed. \n");
		ASSERT_EQ(0, iDRes);
	}
}

//stgw工程在变动，现在发现stgw运行一段时间会自行结束，另外切换工作模式是速度很慢，
//多次切换stgw工作模式， 测试stgw稳定性
//
// TransformMatchMode.RepateTransform
TEST(TransformMatchMode, RepateTransform)
{
	MatchMode mmExample = SimulMatchAll;
	int iRound = 11;
	int iErrorCounter = 0;
	clock_t tTotal = 0;
	std::string strError;
	clock_t tBegin = clock();
	{
		int iModeNumber = 11;
		int i = 0;
		int iRes = 0;
		clock_t tSingleTestBegin;
		clock_t tSingleTestEnd;
		clock_t tTemp = 0;
		for (i = 0; i < iRound; i++)
		{
			if (i == 0 || i == 9) //0清算模式完成时间太长，9启用行情实际上用不到；
			{
				continue;
			}
			mmExample = (enum MatchMode)(i%iModeNumber);
			tSingleTestBegin = clock();
			iRes = TransformMatchMode(mmExample);
			tSingleTestEnd = clock();
			tTemp = tSingleTestEnd - tSingleTestBegin;
			tTotal += tTemp;
			strError = "\n转换次序 iRound :";
			strError += to_string(i);
			strError += "\n本次耗时ms :";
			strError += to_string(tTemp);
			EzLog::i(strError, "");
			if (0 != iRes)
			{
				iErrorCounter++;
				strError = "\nstgw 切换模式为 ： ";
				strError += to_string(mmExample);
				strError += "\nstgw 切换工作模式出错 ！,返回值为 ：";
				strError += to_string(iRes);
				EzLog::e(strError, "");
			}
		}
	}
	clock_t	tEnd = clock();
	strError = "\n转换工作模式总次数 ";
	strError += to_string(iRound);
	strError += "\n出错次数 ：";
	strError += "\n总转换耗时ms  ";
	strError += to_string((long)tTotal / CLOCKS_PER_SEC);
	strError += "\n总循环耗时ms  ";
	strError += to_string((long)(tEnd - tBegin) / CLOCKS_PER_SEC);
	EzLog::i(strError, "");
		
	EXPECT_EQ(0, iErrorCounter);
	if (iErrorCounter != 0)
	{
		EzLog::e(__FUNCTION__, "\n");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n\n");
	}
}