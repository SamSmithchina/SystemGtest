#include "StockAsset.h"
#include "SystemGtestConfigs/configs.h"

//构造函数 从mysql数据库中读取数据并构造数据成员
//@paramter : 验股的股票strStock， 账户strAcc
//@output  : 正确读股份资金、构造StockAsset结构体，输出0， 失败输出-1；
int StockAsset::Init(string strAcc, string strStock)
{
	//上海A股的账户和股票代码
	account_id = strAcc;
	stock_id = strStock;

	int iRes = 0;
	unsigned long ulAffectRow = 0;
	MySqlCnnC602 mycon;
	MYSQL_RES *ptrMySQLRes = NULL;
	std::map < std::string, struct MySqlCnnC602_DF::DataInRow> mapMySQLRowData;
	std::string  strTemp = "";
	std::string strQueryMySQL = "select * from simutgw.stock_asset where account_id='";
	strQueryMySQL += account_id;
	strQueryMySQL += "' and stock_id=";
	strQueryMySQL += stock_id;
	
	//连接mysql数据库
	iRes = mycon.Connect(g_strMysqlHostName, g_strMysqlUserName, g_strMysqlPassword, g_strMysqlPort, g_strMysqlCatalog);
	if (-1 == iRes)
	{
		EzLog::e("连接数据库失败","请检查外部配置文件参数是否正确!");
		return -1;
	}

	//查询mysql股份表数据
	iRes = mycon.Query(strQueryMySQL, &ptrMySQLRes, ulAffectRow);
	if (1 == iRes )
	{	//读取验股信息
		if (0 != mycon.FetchNextRow(&ptrMySQLRes, mapMySQLRowData))
		{
		//	EzLog::i("读取MySQL数据库simutgw.stock_asset", "成功");
			strTemp = mapMySQLRowData["trade_market"].strValue;
			trade_market = strTemp;

			strTemp = mapMySQLRowData["stock_balance"].strValue;
			stock_balance = strTemp;

			strTemp = mapMySQLRowData["stock_auction_purchase_balance"].strValue;
			stock_auction_purchase_balance = strTemp;
			
			strTemp = mapMySQLRowData["stock_staple_purchase_balance"].strValue;
			stock_staple_purchase_balance = strTemp;

			strTemp = mapMySQLRowData["stock_available"].strValue;
			stock_available = strTemp;

			strTemp = mapMySQLRowData["stock_etf_redemption_balance"].strValue;
			stock_etf_redemption_balance = strTemp;

			strTemp = mapMySQLRowData["stock_last_balance"].strValue;
			stock_last_balance = strTemp;

			strTemp = mapMySQLRowData["is_close"].strValue;
			is_close = strTemp;
		}
	}
	else
	{
		EzLog::e("读取MySQL数据库simutgw.stock_asset失败", "或者当前验股模式CheckAssetYES未打开");
		return -1;
	}

	mycon.FreeResult(&ptrMySQLRes);		//清空 pResultSet = NULL;
	ptrMySQLRes = NULL;
	if (mycon.IsClosed() == false)
	{
		mycon.Close();
	}
	return 0;
}