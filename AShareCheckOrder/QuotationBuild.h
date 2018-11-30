#ifndef __QUOTATIONBUILD_H__
#define __QUOTATIONBUILD_H__

#include "SHShare/SHShare.h"
#include "MatchMode/MatchMode.h"
#include "tool_string/TimeStringUtil.h"
#include "tool_redis/Tgw_RedisHelper.h"
#include "util/EzLog.h"
#include "quotation/AStockQuotationHelper.h"
#include "simutgw/stgw_config/g_values_inner.h"
//#include "simutgw/stgw_config/g_values_net.h"

#include <stdlib.h>
#include <string>


//设计特定的行情数据Example
//输入：待处理行情AStockQuot &inout_aStockQuot
//输出：赋值与该行情并返回，
void CreateQuotation(AStockQuot &inout_aStockQuot);

//设计特定的新行情NewExample
//输入：行情结构体AStockQuot & inout_aStockQuot
//输出：赋值于该行情并返回
void CreateNewQuotation(AStockQuot &inout_aStockQuot);

//将行情推送redis
//输入：待推送的行情结构体 in_aStockQuot;
//输出：0，表示程序正确执行；  1，表示出现错误 
int SendQuotToRedis(AStockQuot& in_aStockQuot);


// 删除Redis对应的行情
// 输入 ：strZqdm 待删除的行情的证券代码
// 输出 ：0 正常执行删除 操作 ， -1删除操作失败
int DelRedisKeyNum(const std::string& strZqdm);

//从tgwhqk_tv获取行情容量
//输入： 
//	strZqdm 证券代码	
//输出： 
//	uint64_t ui64MaxGain 涨幅
//  uint64_t ui64MinFall 跌幅
//  uint64_t ui64Cjje 成交金额
//  uint64_t ui64Cjsl 成交数量
//  uint64_t ui64SJW1 卖1价格
//  uint64_t ui64SSL1 卖1数量
//  uint64_t ui64BJW1 买1价格
//  uint64_t ui64BSL1 买一数量
//  uint64_t ui64Zjjg 最近成交价
//  RedisReply emPcbCallRes 查询redis命令的返回状态
RedisReply GetQuotFromTgwHqkTV(
	const std::string& strZqdm, std::string& strTpbz,
	uint64_t& ui64MaxGain, uint64_t& ui64MinFall, uint64_t& ui64Cjje,
	uint64_t& ui64Cjsl, uint64_t& ui64SJW1, uint64_t& ui64SSL1,
	uint64_t& ui64BJW1, uint64_t& ui64BSL1, uint64_t& ui64Zjjg);

//计算区间段均价，只有一个行情时
//@parameter:
//	AStockQuot aStockQuot 输入初始行情
//	ui64OutAvePrice 传递引用要输出的int64_t价格
//@output:
//	strAvePrice  区间段均价；
std::string CalcAvePrice(const AStockQuot aStockQuot, uint64_t& ui64OutAvePrice);

//计算区间段均价，原来的行情后面推送新的行情；
//@parameter：
//	fristAStockQuot 第一个行情结构体；
//	secondAStockQuot 第二个行情结构体；
//@output:
// strAvePrice 两次行情的区间段均价
std::string CalcAvePrice(const AStockQuot firstAStockQuot, const AStockQuot secondAStockQuot, uint64_t& ui64OutAvePrice);

#endif