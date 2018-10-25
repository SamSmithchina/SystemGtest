#ifndef __MATCHMODE_H__
#define __MATCHMODE_H__

#include <iostream>

//成交模式
enum MatchMode
{
	//生成清算文件
	Settlement = 0,

	//
	// 非实盘 simulateMatchMode
	//
	
	SimulMatchAll = 1,	/* 1 --不看行情，全部成交 */

	SimulMatchByDivide = 2,		/* 2 --不看行情，分笔成交 */
	
	SimulNotMatch = 3,	/* 3 --不看行情，挂单、不成交，可撤单 */
	
	SimulErrMatch = 4,	/* 4 --错误单 */
	
	SimulMatchPart = 5,	/* 5 --部分成交，先成交一半，剩余挂单可撤单 */

	//
	// 实盘下的成交模式 QuotationMode
	//
	AveragePrice = 6,	// 根据变动成交总金额和成交总量计算的均价

	SellBuyPrice = 7,	// 买一卖一
	
	RecentPrice = 8,	// 最近成交价

	//
	//其他成交命令
	//
	Quotaion = 9,	//启用行情
	 
	CheckAssetYES = 10,	//验股

	CheckAssetNO = 11	//不验股
};

//切换工作模式 
//输入： 期望的工作模式
//
int TransformMatchMode(MatchMode simulateMatchMode);


#endif