#include "SHShare.h"
#include "SystemGtestConfigs/configs.h"

#include <time.h>

//构造函数
SHShare::SHShare()
{
	//ordwth 接口字段格式初始化，
	time_t tNow;
	tm tmPtr;
	char szTime[9] = { "\0" };
	char szDate[9] = { "\0" };
	time(&tNow);
	localtime_s(&tmPtr, &tNow);
	_Strftime(szDate, 9, "%Y%m%d", &tmPtr, NULL);	//char 8
	date = szDate;									//日期年月日
	_Strftime(szTime, 9, "%H:%M:%S", &tmPtr, NULL);	//char 8  字符串末尾填充空位；
	now = szTime;						 			//时间时分秒

	reff = ("J000000000");		//内部会员订单号
	account = "A645078963";	//股票账号
	stock = "600302";		// 证券代码
	bs = "B";				//买卖
	price = ("0.010");		//价格
	qty = ("0");			// 成交原始数量，多个验股测试样例时，检查股份需要递增
	status = ("R");			//R表示未发送，P表示已发送
	owflag = ("LPT");		//订单类型标志 撤单WTH
	ordrec = ("0");			//撤单号 = rec_num
	frimid = ("");			//B股的发行结算代码，对A股无意义；
	brandchid = ("");		//营业部代码
	checkord = ("0x00");		//binary 校验码
	rec_num = ("0");		//订单编号；\

	//用于比较确认表数据
	qty2 = "0";	//撤单数量；

	//用于比较成交汇报表中的数据；
	gddm = account;
	zqdm = stock;
	cjsl = "0";
	cjjg = "0";
	cjje = "0";

	//用于验股
	iStockAssetStatus = 0;	//验股状态位： 0正常买卖； -1余额不足，不能卖；  默认0；
}

//SHShare::~SHShare()
//{
//	std::ofstream WriteFile("g_iExternRecNum.txt", std::ios::out | std::ios::trunc ); //供写使用，文件不存在则创建，存在则清空原内容
//	WriteFile << g_iExternRecNum;
//	WriteFile.close();
//}


//获取要执行的插入语句
std::string SHShare::GetSQlInsertSentence()
{
	//每一个订单的存储结构,
	//这里说明：checkord 是二进制0x0，在下面的语句写在string里不加‘’才会被当作二进制；
	std::string OrdwthStruct = "('";
	OrdwthStruct += date;
	OrdwthStruct += "','";
	OrdwthStruct += now;
	OrdwthStruct += "','";
	OrdwthStruct += reff;
	OrdwthStruct += "','";
	OrdwthStruct += account;
	OrdwthStruct += "','";
	OrdwthStruct += stock;
	OrdwthStruct += "','";
	OrdwthStruct += bs;
	OrdwthStruct += "','";
	OrdwthStruct += price;
	OrdwthStruct += "','";
	OrdwthStruct += qty;
	OrdwthStruct += "','";
	OrdwthStruct += status;
	OrdwthStruct += "','";
	OrdwthStruct += owflag;
	OrdwthStruct += "','";
	OrdwthStruct += ordrec;
	OrdwthStruct += "','";
	OrdwthStruct += frimid;
	OrdwthStruct += "','";
	OrdwthStruct += brandchid;
	OrdwthStruct += "',";
	OrdwthStruct += checkord;
	OrdwthStruct += ",'";
	OrdwthStruct += rec_num;
	OrdwthStruct += "')";

	//实际在执行函数中用的insert语句
	std::string strInsertSentence = strInsertSql_prefix;
	strInsertSentence += OrdwthStruct;
	return(strInsertSentence);
}

//获取要执行的撤销语句
std::string SHShare::GetSQLCancelSentence()
{
	time_t tNow;
	tm tmPtr;
	char szTime[9] = { "\0" };
	char szDate[9] = { "\0" };
	time(&tNow);
	localtime_s(&tmPtr, &tNow);
	_Strftime(szDate, 9, "%Y%m%d", &tmPtr, NULL);	//char 8
	date = szDate;									//日期年月日
	_Strftime(szTime, 9, "%H:%M:%S", &tmPtr, NULL);	//char 8  字符串末尾填充空位；
	now = szTime;						 			//时间时分秒

	//插入撤单;将owflag 和 orderrec修改
	std::string strCancelOwflag = ("WTH");
	std::string strCancelOrdrec = rec_num;

	g_iExternRecNum++;		//加一操作//避免重单
	//int型转换为string 类型 rec_num
	char szTemp[32];
	_itoa_s(g_iExternRecNum, szTemp, 10);	//int to ascii;
	std::string strTempRecNUm = szTemp;	//rec_num ++;避免重单

	std::string CancelOrderStruct = "('";
	CancelOrderStruct += date;
	CancelOrderStruct += "','";
	CancelOrderStruct += now;
	CancelOrderStruct += "','";
	CancelOrderStruct += reff;
	CancelOrderStruct += "','";
	CancelOrderStruct += account;
	CancelOrderStruct += "','";
	CancelOrderStruct += stock;
	CancelOrderStruct += "','";
	CancelOrderStruct += bs;
	CancelOrderStruct += "','";
	CancelOrderStruct += price;
	CancelOrderStruct += "','";
	CancelOrderStruct += qty;
	CancelOrderStruct += "','";
	CancelOrderStruct += status;
	CancelOrderStruct += "','";
	CancelOrderStruct += strCancelOwflag;
	CancelOrderStruct += "','";
	CancelOrderStruct += strCancelOrdrec;
	CancelOrderStruct += "','";
	CancelOrderStruct += frimid;
	CancelOrderStruct += "','";
	CancelOrderStruct += brandchid;
	CancelOrderStruct += "',";
	CancelOrderStruct += checkord;
	CancelOrderStruct += ",'";
	CancelOrderStruct += strTempRecNUm;
	CancelOrderStruct += "')";

	//执行cancel order语句
	std::string strCancelSentence = strInsertSql_prefix;
	strCancelSentence += CancelOrderStruct;
	return strCancelSentence;
}

