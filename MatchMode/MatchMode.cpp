#include "MatchMode.h"
#include "util\EzLog.h"
#include "SystemGtestConfigs\configs.h"

#include <Windows.h>

//切换工作模式
//输入： 期望的工作模式
//
int TransformMatchMode(MatchMode simulateMatchMode)
{
	//切换工作模式
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD lDRes = 0;

	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));

	char szTransformCMD[256] = { "\0" };
	std::string strCMD = "";

	switch (simulateMatchMode)
	{
		// 0 --生成清算文件
	case Settlement:
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_settlement.txt");
		break;

		// 1 --不看行情，全部成交 
	case SimulMatchAll:
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_sall.txt");
		break;

		//2 --不看行情，分笔成交
	case SimulMatchByDivide:
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_sdivide.txt");
		break;

		// 3 --不看行情，挂单、不成交，可撤单 
	case SimulNotMatch:
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_snot.txt");
		break;

		// 4 --错误单 
	case SimulErrMatch:
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_serr.txt");
		break;

		// 5 --部分成交，先成交一半，剩余挂单可撤单
	case SimulMatchPart:
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_spart.txt");
		break;

		// 6 --根据变动成交总金额和成交总量计算的均价	
	case AveragePrice:
		g_iQuotationEnable = 1;
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_quotation_averageprice.txt");
		break;

		// 7 --买一卖一
	case SellBuyPrice:
		g_iQuotationEnable = 1;
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_quotation_sellbuyprice.txt");
		break;

		// 8 --最近成交价  
	case RecentPrice:
		g_iQuotationEnable = 1;
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_quotation_recentprice.txt");
		break;

		// 9 --启用行情 
	case Quotaion:
		g_iQuotationEnable = 1;
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_quotation.txt");
		break;

		//10 --验股	
	case CheckAssetYES:
		g_iCheckAsset = 1;
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_checkassets.txt");
		break;

		// 11 --不验股 
	case CheckAssetNO:
		g_iCheckAsset = 0;
		strCMD = ("simutgw_win64\\simutgw_cli.exe -h 127.0.0.1 -p 50000 -f simutgw_win64\\data\\cli_cmd\\cmd_matchmode_dontcheckassets.txt");
		break;

	default:
		strCMD = "";
	}

	//	拷贝string到char[]
	strcpy_s(szTransformCMD, strCMD.c_str());
	CreateProcess(NULL, szTransformCMD, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, 25000);		 // 不能无限等待进程，不能用INFINITE, ，改为25s
	GetExitCodeProcess(pi.hProcess, &lDRes);
	if (0 != lDRes)
	{
		EzLog::e(strCMD, " failed !");
		return -1;
	}
	return 0;
}

