#pragma once
#ifndef __CONFIGS_H__

#define __CONFIGS_H__

#include <iostream>

// 为方便连续g_test测试，不能使rec_num重复，
extern int g_iExternRecNum;

//（sleep()精度很低，g_iTimeOut设置太小查询频繁会导致数据库访问失败），
//通常数据传输时间很短，在stg上时间阈值为500ms。g_iTimeOut单位毫秒ms,
extern int g_iTimeOut;		//	设置100ms比较合适

//查询次数；
extern int g_iQueryTimes;

//验股 0:不验股， 1：验股
extern int  g_iCheckAsset;

// 行情 0 :不启用行情 ， 1：启用行情;
extern int g_iQuotationEnable;

//连接数据库语句
extern std::string g_strShOdbcConn;

// MYSQL连接配置 ，根据实际部署的mysql数据库修改地址、用户名、密码；
//MySql数据库地址
extern std::string g_strMysqlHostName;

//MySql数据库用户名 ,
extern std::string g_strMysqlUserName;

//MySql数据库密码
extern std::string g_strMysqlPassword;

//MySql数据库端口
extern int g_strMysqlPort;

//默认数据库
extern std::string g_strMysqlCatalog;

// Redis连接
//redis内存数据库地址
extern std::string g_strRedisHostName;


//redis内存数据库端口
extern int g_strRedisPort;

//redis内存数据库密码
extern std::string g_strRedisPassword;

//加载redis并初始化
int InitRedis();

// 释放redis连接
void StopRedis();

//ReadMaxRecNumFromDB（） : 从数据库中读取最大的订单编号，保证订单编号递增
//输入：  
//输出 ： 0,正常读取到最大订单号， -1 ，失败
int ReadMaxRecNumFromDB();

//ReadConfig() ： 读取systemgtest.ini文件数据，复制给相关系统参数
//输入：	无
//输出：	0 正常； -1 出现异常 
int  ReadConfig();

//WriteConfig() ： 写入相关系统参数到systemgtest.ini文件数据
//输入：	无
//输出：	0 正常； -1 出现异常 
int  WriteConfig();


#endif