#ifndef __ASHARECHECKORDER_H__
#define __ASHARECHECKORDER_H__

#include "SHShare/SHShare.h"
#include "MatchMode/MatchMode.h"
#include "StockAsset/StockAsset.h"
#include "tool_odbc/OTLConn40240.h"
#include "tool_string/Tgw_StringUtil.h"
#include "util/EzLog.h"

#include <stdlib.h>
#include <string>

//插入数据
//输出 ： 0 正确； -1 异常
long InsertOrder(OTLConn40240 &con, SHShare aSHShare);

//插入撤单
//输出 ： 0 正确； -1 异常
long InsertCancelOrder(OTLConn40240 &con, SHShare aSHShare);

// 查询订单确认、成交回报行数；
// 输入： strQuery - 查询语句 ，con ,otlconnc 连接对像
// 输出： -1，未查询带结果，或者出现异常； 0和正整数表示查询到得记录行数
int QueryRecordNum(OTLConn40240& con, const std::string& strQuery);

//检查待成交单的确认；
//输入：OTLConn40240 &con 连接数据库, SHShare aSHShare待检查股票订单, 
//输出 ： 0 正确； -1 异常
long CheckOrdwth2Match(OTLConn40240 &con, SHShare aSHShare);

//检查撤单确认；
//输入：OTLConn40240 &con 连接数据库, SHShare aSHShare待检查股票订单, 
//输出 ： 0 正确； -1 异常
long CheckOrdwth2Cancel(OTLConn40240 &con, SHShare aSHShare);

//检查错单确认；
//输入：OTLConn40240 &con 连接数据库, SHShare aSHShare待检查股票订单, 
//输出 ： 0 正确； -1 异常
long CheckOrdwth2Error(OTLConn40240 &con, SHShare aSHShare);

//检查成交单: 
//输入：OTLConn40240 &con 连接数据库, SHShare aSHShare待检查股票订单, 
//输出 ： 0 正确； -1 异常
long CheckCjhb(OTLConn40240 &con, SHShare aSHShare);

//检查分笔成交的成交回报：
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,iDivide 目前分笔数为2 
//输出 ： 0 正确； -1 异常
long CheckCjhb(OTLConn40240 &con, SHShare aSHShare, int iDivideNum);

// 校验simutgw写回本地mysql数据库的数据是否符合实际交易
//@parameter ： SHShare aSHShare STGW处理的订单;
//@output ： -1 simutgw写回数据异常，0 ，写回数据正常;
int CheckStgwWriteAssetBackToMySQL(const SHShare aSHShare, StockAsset beginStockAsset, bool bSimulMatchByDivide = false);

// 校验simutgw写回本地mysql数据库的数据是否符合实际交易
//@parameter ： 大量买的总数量 ui64BCjsl, 大量卖单的总数量ui64SCjsl
//@output ： -1 simutgw写回数据异常，0 ，写回数据正常;
int CheckStgwWriteAssetBackToMySQL(const StockAsset beginStockAsset, const uint64_t ui64BCjsl, const uint64_t ui64SCjsl);

#endif