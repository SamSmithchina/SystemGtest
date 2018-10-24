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
	std::string strModel = "Transform to MatchAllWithoutQuotation ";

	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));	//"cd /d %~dp0"
	system("echo %cd%");	//simutgw_win64

	char TransformCMD[256] = { "\0" };
	std::string strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_serr.txt");
	CreateProcess(NULL, TransformCMD, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	GetExitCodeProcess(pi.hProcess, &iDRes);
	EXPECT_EQ(iDRes, 0);
	EzLog::i("", __FUNCTION__);
	if (iDRes != 0)
	{
		EzLog::e("Transform to MatchAllWithoutQuotation ", "failed. \n");
		EzLog::e("", __FUNCTION__);
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
	int iModeNumber = 11;
	int iRound = 11;
	int i = 0;
	int iRes = 0;
	int iErrorCounter = 0;
	time_t tSingleTestBegin;
	time_t tSingleTestEnd;
	time_t tTotal = 0;
	time_t tTemp = 0;
	time_t tBegin = clock();
	{
		for (i = 0; i < iRound; i++)
		{
			if (1 == 0 || i == 9) //0清算模式完成时间太长，9启用行情实际上用不到；
			{
				continue;
			}
			mmExample = (enum MatchMode)(i%iModeNumber);
			tSingleTestBegin = clock();
			iRes = TransformMatchMode(mmExample);
			tSingleTestEnd = clock();
			tTemp = tSingleTestEnd - tSingleTestBegin;
			tTotal += tTemp;
			EzLog::Out("转换次序 iRound :", trivial::severity_level(2), i);
			EzLog::Out("本次耗时ms :", (trivial::severity_level)2, (long)tTemp);
			if (0 != iRes)
			{
				iErrorCounter++;
				EzLog::Out("stgw 切换模式为 ： ", (trivial::severity_level)2, mmExample);
				EzLog::Out("stgw 切换工作模式出错 ！,返回值为 ：", (trivial::severity_level)2, iRes);
			}
		}
	}
	time_t	tEnd = clock();
	EzLog::Out("转换工作模式总次数 ", (trivial::severity_level)2, iRound);
	EzLog::Out("出错次数 ：", (trivial::severity_level)2, iErrorCounter);
	EzLog::Out("总转换耗时ms  ", (trivial::severity_level)2, (long)tTotal);
	EzLog::Out("总循环耗时ms  ", (trivial::severity_level)2, (long)(tEnd - tBegin));
	EzLog::i("\n", "\n");
	EXPECT_EQ(0, iErrorCounter);
	EzLog::i("", __FUNCTION__);
	if (iErrorCounter != 0)
	{
		EzLog::e("", __FUNCTION__);
	}
}