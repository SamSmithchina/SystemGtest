#include "configs.h"
#include "util/EzLog.h"
#include "tool_odbc/OTLConn40240.h"
#include "boost/property_tree/ini_parser.hpp"

#include <Windows.h>
#include <tchar.h>

//为方便连续g_test测试，不能使rec_num重复，
int g_iExternRecNum = 0;

//（sleep()精度很低，g_iTimeOut设置太小查询频繁会导致数据库访问失败），
//通常数据传输时间很短，在stg上时间阈值为500ms。g_iTimeOut单位毫秒ms,
int g_iTimeOut = 100;		//	设置100ms比较合适

//查询次数；
int g_iQueryTimes = 50;

//验股 0:不验股， 1：验股
int  g_iCheckAsset = 0;

// 行情 0 :不启用行情 ， 1：启用行情;
int g_iQuotationEnable = 0;

//连接数据库语句
std::string g_strShOdbcConn = "Driver={SQL Server};Server={10.81.1.139,1433};Connection Timeout=3;"
"Database=Ashare_OIW;Uid=deve;Pwd=888888;";

// MYSQL连接配置 ，根据实际部署的mysql数据库修改地址、用户名、密码；
//MySql数据库地址
std::string g_strMysqlHostName = "127.0.0.1";

//MySql数据库用户名 ,
std::string g_strMysqlUserName = "admin";

//MySql数据库密码
std::string g_strMysqlPassword = "simutgw";

//MySql数据库端口
int g_strMysqlPort = 3306;

//默认数据库
std::string g_strMysqlCatalog = "simutgw";

// Redis连接
//redis内存数据库地址
std::string g_strRedisHostName = "127.0.0.1";


//redis内存数据库端口
int g_strRedisPort = 6700;

//redis内存数据库密码
std::string g_strRedisPassword = "";

//ReadMaxRecNumFromDB（） : 从数据库中读取最大的订单编号，保证订单编号递增
//输入：  
//输出 ： 0,正常读取到最大订单号， -1 ，失败
int ReadMaxRecNumFromDB()
{
	std::string strMAxRecNum= "select MAX(rec_num) from Ashare_OIW.dbo.ashare_ordwth;";
	std::map<std::string, struct OTLConn_DF::DataInRow> mapRowData;
	OTLConn40240 con;
	con.Connect(g_strShOdbcConn);
	long lTemp = 0;
	otl_stream streamDB;
	int iRes = 0;

	iRes = con.Query(strMAxRecNum, &streamDB);
	if (iRes == -1)
	{
		EzLog::e(__FUNCTION__, " query sql server failed");
		return -1;
	}

	iRes = con.FetchNextRow(&streamDB, mapRowData);
	if (iRes == 1)  //读取库中最大订单编号
	{
		std::string strValue = mapRowData[""].strValue;
		g_iExternRecNum = atoi(strValue.c_str());
	}
	else if (iRes == -1) //Fetch 发生异常
	{
		EzLog::e(__FUNCTION__, " fetch otl_stream failed");
	}
	else //读取库但库中没有数据
	{
		g_iExternRecNum = 0;
	}
	
	std::string strTruncateTable = "TRUNCATE TABLE [Ashare_OIW].[dbo].[ashare_ordwth];";
	con.Exec(strTruncateTable, lTemp);
	strTruncateTable = "TRUNCATE TABLE [Ashare_OIW].[dbo].[ashare_ordwth2];";
	con.Exec(strTruncateTable, lTemp);
	strTruncateTable = "TRUNCATE TABLE [Ashare_OIW].[dbo].[ashare_cjhb];";
	con.Exec(strTruncateTable, lTemp);

	streamDB.close();
	con.Close();
	if (iRes != -1)
	{
		EzLog::Out("systemgtest.g_iExternRecNum : ", (trivial::severity_level)2, g_iExternRecNum);
		return 0;
	}
	else
	{
		return -1;
	}
}

//ReadConfig() ： 读取systemgtest.ini文件数据，复制给相关系统参数
//输入：	无
//输出：	0 正常； -1 出现异常；
int  ReadConfig()
{
	boost::property_tree::ptree pt;

	system("echo %cd%");
	EzLog::i("配置文件位置：", " .\\SystemGtest\\test_data\\gtest\\systemgtest.ini");

	//从 systemgtest.ini 读取订单开始编号；
	try
	{
		boost::property_tree::ini_parser::read_ini(".\\SystemGtest\\test_data\\gtest\\systemgtest.ini", pt);
	}
	catch (std::exception& e)
	{
		EzLog::e("ReadConfig()读取systemgtest.ini ", " 失败，请检查systemgtest.ini文件是否存在");
		throw(e);
		return -1;
	}

	//	读进的数据复赋值给相关参数
	try
	{
		//为方便连续g_test测试，不能使rec_num重复，
		/*g_iExternRecNum = pt.get<int>("systemgtest.g_iExternRecNum");
		EzLog::Out("systemgtest.g_iExternRecNum : ", (trivial::severity_level)2, g_iExternRecNum);*/

		///通常数据传输时间很短，在stg上时间阈值为500ms。g_iTimeOut单位毫秒ms,
		g_iTimeOut = pt.get<int>("systemgtest.g_iTimeOut");
		EzLog::Out("systemgtest.g_iTimeOut : ", (trivial::severity_level)2, g_iTimeOut);

		//查询次数；
		g_iQueryTimes = pt.get<int>("systemgtest.g_iQueryTimes");
		EzLog::Out("systemgtest.g_iQueryTimes : ", (trivial::severity_level)2, g_iQueryTimes);

		//验股 0:不验股， 1：验股
		//g_iCheckAsset = pt.get<int>("systemgtest.g_iCheckAsset");
		//EzLog::Out("systemgtest.g_iCheckAsset : ", (trivial::severity_level)2, g_iCheckAsset);

		//行情 ， 0 ：不启用行情； 1：启用行情；
		//g_iQuotationEnable = pt.get<int>("systemgtest.g_iQuotationEnable");
		//EzLog::Out("systemgtest.g_iQuotationEnable : ", (trivial::severity_level)2, g_iQuotationEnable);

		//行情模式；
		//	gQuotationMode = pt.get<int>("systemgtest.gQuotationMode");
		//	std::cout << "systemgtest.gQuotatuonMode : " << gQuotationMode << std::endl;

		//连接数据库语句
		g_strShOdbcConn = pt.get<std::string>("Sh_Conn1.connection");
		EzLog::Out("Sh_Conn1.connection : ", (trivial::severity_level)2, g_strShOdbcConn);


		// MYSQL连接配置 ，根据实际部署的mysql数据库修改地址、用户名、密码；
		//MySql数据库地址
		g_strMysqlHostName = pt.get<std::string>("db.mysql_host_name");
		EzLog::Out("db.mysql_host_name : ", (trivial::severity_level)2, g_strMysqlHostName);

		//MySql数据库用户名 ,
		g_strMysqlUserName = pt.get<std::string>("db.mysql_user_name");
		EzLog::Out("db.mysql_user_name : ", (trivial::severity_level)2, g_strMysqlUserName);

		//MySql数据库密码
		g_strMysqlPassword = pt.get<std::string>("db.mysql_password");
		EzLog::Out("db.mysql_password : ", (trivial::severity_level)2, g_strMysqlPassword);

		//MySql数据库端口
		g_strMysqlPort = pt.get<int>("db.mysql_port");
		EzLog::Out("db.mysql_port : ", (trivial::severity_level)2, g_strMysqlPort);

		// Redis连接
		//redis内存数据库地址
		g_strRedisHostName = pt.get<std::string>("redis.redis_host_name");
		EzLog::Out("redis.redis_host_name : ", (trivial::severity_level)2, g_strRedisHostName);

		//redis内存数据库端口
		g_strRedisPort = pt.get<int>("redis.redis_port");
		EzLog::Out("redis.redis_port : ", (trivial::severity_level)2, g_strRedisPort);

		EzLog::i("systemgtest.ini 配置文件读取完毕 \n", "=================================================\n");
		return 0;
	}
	catch (std::exception & e)
	{
		EzLog::e("ReadConfig()读取systemgtest.ini ", "出错， 请检查systemgtest.ini文件变量名称与程序中变量名称是否一致");
		throw(e);
		return -1;
	}
}

//WriteConfig() ： 写入相关系统参数到systemgtest.ini文件数据
//输入：	无
//输出：	0 正常； -1 出现异常 
int  WriteConfig()
{
	boost::property_tree::ptree pt;
	try
	{
		//如果需要，写回配置信息；
		char szBuff[33] = { "\0" };
		_itoa_s(g_iExternRecNum, szBuff, 10);
		pt.put<std::string>("systemgtest.g_iExternRecNum", szBuff);
		_itoa_s(g_iTimeOut, szBuff, 10);
		pt.put<std::string>("systemgtest.g_iTimeOut", szBuff);
		_itoa_s(g_iQueryTimes, szBuff, 10);
		pt.put<std::string>("systemgtest.g_iQueryTimes", szBuff);
		/*_itoa_s(g_iCheckAsset, szBuff, 10);
		pt.put<std::string>("systemgtest.g_iCheckAsset", szBuff);
		_itoa_s(g_iQuotationEnable, szBuff, 10);
		pt.put<std::string>("systemgtest.g_iQuotationEnable", szBuff);*/
		//	_itoa_s(gQuotationMode, szBuff, 10);
		//	pt.put<std::string>("systemgtest.gQuotationMode", szBuff);

		pt.put<std::string>("Sh_Conn1.connection", g_strShOdbcConn);
		pt.put<std::string>("db.mysql_host_name", g_strMysqlHostName);
		pt.put<std::string>("db.mysql_user_name", g_strMysqlUserName);
		pt.put<std::string>("db.mysql_password", g_strMysqlPassword);
		_itoa_s(g_strMysqlPort, szBuff, 10);
		pt.put<std::string>("db.mysql_port", szBuff);
		pt.put<std::string>("redis.redis_host_name", g_strRedisHostName);
		_itoa_s(g_strRedisPort, szBuff, 10);
		pt.put<std::string>("redis.redis_port", szBuff);
		boost::property_tree::ini_parser::write_ini(".\\SystemGtest\\test_data\\gtest\\systemgtest.ini", pt);
		EzLog::i("=================================================", "");
		EzLog::Out("g_iExternRecNum = ", (trivial::severity_level)2, g_iExternRecNum);
	}
	catch (std::exception & e)
	{
		EzLog::e("RWriteConfig()写回systemgtest.ini ", " 失败，请检查ini文件是否存在，检查变量赋值是否符合预期");
		throw(e);
		return -1;
	}
	return 0;
}

