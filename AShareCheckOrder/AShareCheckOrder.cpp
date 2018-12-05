#include "AShareCheckOrder.h"
#include "SystemGtestConfigs/configs.h"

//插入数据
//输出 ： 0 正确； -1 异常
long InsertOrder(OTLConn40240 &con, SHShare aSHShare)
{
	long lAffectRow = 0;	//记录目标数据在数据库所在的行数

	//执行插入一条订单
	std::string  strInsert = aSHShare.GetSQlInsertSentence();
	con.Exec(strInsert, lAffectRow);
	if (lAffectRow != 1)
	{
		EzLog::e("未正常插入\n", strInsert);
		return -1;
	}
	return 0;
}

//插入撤单
//输出 ： 0 正确； -1 异常
long InsertCancelOrder(OTLConn40240 &con, SHShare aSHShare)
{
	long lAffectRow = 0;	//记录目标数据在数据库所在的行数

	//插入撤单
	std::string strCancle = aSHShare.GetSQLCancelSentence();
	con.Exec(strCancle, lAffectRow);
	if (lAffectRow != 1)
	{
		EzLog::e("未正常插入\n", strCancle);
		return -1;
	}
	return 0;
}

// 查询订单确认、成交回报行数；
// 输入： strQuery - 查询语句 ，con ,otlconnc 连接对像
// 输出： -1，未查询带结果，或者出现异常； 0和正整数表示查询到得记录行数
int QueryRecordNum(OTLConn40240& con, const std::string& strQuery)
{
	otl_stream streamDB;
	std::map<std::string, struct OTLConn_DF::DataInRow> mapRowData;
	int iRes = -1;

	for (int k = 0; k < g_iQueryTimes; ++k)
	{
		if (k == g_iQueryTimes - 1)//达到最大查询次数
		{
			streamDB.close();
			mapRowData.clear();
			return -1;
		}

		long lQueryResult = con.Query(strQuery, &streamDB);
		if (lQueryResult != 0) //查询未完成
		{
			streamDB.clean();
			Sleep(g_iTimeOut);
			continue;
		}

		iRes = con.FetchNextRow(&streamDB, mapRowData);
		if (iRes == -1)
		{
			streamDB.clean();
			Sleep(g_iTimeOut);
			continue;
		}
		else
		{
			iRes = atoi(mapRowData[""].strValue.c_str());
			//std::cout << "iRes" << iRes << std::endl;
			if (iRes == 0)  //查到0条数据
			{
				streamDB.close();
				mapRowData.clear();
				Sleep(g_iTimeOut);
				continue;
			}
			else
			{
				break;
			}
		}
	}

	streamDB.close();
	streamDB.~otl_stream();
	return iRes;
}

//检查汇报表
//@parameter: con -OTL连接；
//			  aSHShare -待查订单 ； 
//            enumOrdwth2Report -回报类型
//
//@return： -1  -执行有误
//			 0  -执行正常
int CheckOrdwth2(OTLConn40240 &con, SHShare aSHShare, Ordwth2Report enumOrdwth2Report)
{
	otl_stream streamDB;	//otl 流
	std::map<std::string, struct OTLConn_DF::DataInRow> mapRowData;
	std::string strQueryOrdwth2;

	if (Match == enumOrdwth2Report)
	{	//比较确认结果；
		strQueryOrdwth2 = "select * from [Ashare_OIW].[dbo].[ashare_ordwth2] where reff ='";
		strQueryOrdwth2 += aSHShare.reff;
		strQueryOrdwth2 += "' and owflag='LPT';";
	}
	else if (Cancle == enumOrdwth2Report)
	{
		//撤单确认结果；
		strQueryOrdwth2 = "select * from [Ashare_OIW].[dbo].[ashare_ordwth2] where ordrec='";
		strQueryOrdwth2 += aSHShare.rec_num;
		strQueryOrdwth2 += "' and owflag ='WTH';";
	}
	else if (Error == enumOrdwth2Report)
	{
		//错单确认 查询每条记录
		std::string strQueryOrdwth2 = "select * from [Ashare_OIW].[dbo].[ashare_ordwth2] where reff ='";
		strQueryOrdwth2 += aSHShare.reff;
		strQueryOrdwth2 += "' and status = 'F';";
	}

	for (int k = 0; k < g_iQueryTimes; ++k)
	{
		mapRowData.clear();
		long lQueryResult = con.Query(strQueryOrdwth2, &streamDB);
		if (0 != lQueryResult)	//查询未完成
		{
			EzLog::e(__FUNCTION__, " Query sql server failed !");
			streamDB.close();
			Sleep(g_iTimeOut);
			continue;
		}
		long lFetchResult = con.FetchNextRow(&streamDB, mapRowData);
		if (lFetchResult != 1)
		{
			streamDB.close();
			Sleep(g_iTimeOut);
			continue;
		}
		else//校验确认字段
		{
			std::string strTemp = mapRowData["acc"].strValue;
			if (aSHShare.account.compare(strTemp))
			{
				std::string strError = "\nOrdwth acc: ";
				strError += strTemp;
				strError += "\n预期   acc: ";
				strError += aSHShare.account;
				strError += "\nCheck Ordwth2_Match ERROR! reff = ";
				EzLog::e(strError, aSHShare.reff);
				streamDB.close();
				mapRowData.clear();
				return -1;
			}

			strTemp = mapRowData["stock"].strValue;
			if (strTemp != aSHShare.stock)
			{
				std::string strError = "\nOrdwth stock: ";
				strError += strTemp;
				strError += "\n预期   stock: ";
				strError += aSHShare.stock;
				strError += "\nCheck Ordwth2_Match ERROR! reff = ";
				EzLog::e(strError, aSHShare.reff);
				streamDB.close();
				mapRowData.clear();
				return -1;
			}

			strTemp = mapRowData["bs"].strValue;
			if (strTemp != aSHShare.bs)
			{
				std::string strError = "\nOrdwth bs: ";
				strError += strTemp;
				strError += "\n预期   bs: ";
				strError += aSHShare.bs;
				strError += "\nCheck Ordwth2_Match ERROR! reff = ";
				EzLog::e(strError, aSHShare.reff);
				streamDB.close();
				mapRowData.clear();
				return -1;
			}

			strTemp = mapRowData["status"].strValue;
			if (Ordwth2Report::Match == enumOrdwth2Report)
			{
				if ("O" != strTemp)
				{
					std::string strError = "\nOrdwth status:";
					strError += strTemp;
					strError += "\n预期   status:";
					strError += "O";//正常标志F
					strError += "\nCheck Ordwth2_error ERROR! reff = ";
					EzLog::e(strError, aSHShare.reff);
					streamDB.close();
					mapRowData.clear();
					return -1;
				}
			}
			else if (Ordwth2Report::Error == enumOrdwth2Report)
			{
				if ("F" != strTemp)
				{
					std::string strError = "\nOrdwth status:";
					strError += strTemp;
					strError += "\n预期   status:";
					strError += "F";//错单标志F
					strError += "\nCheck Ordwth2_error ERROR! reff = ";
					EzLog::e(strError, aSHShare.reff);
					streamDB.close();
					mapRowData.clear();
					return -1;
				}
			}
			else if(Ordwth2Report::Cancle == enumOrdwth2Report)
			{
				if ("W" != strTemp)
				{
					std::string strError = "\nOrdwth status:";
					strError += strTemp;
					strError += "\n预期   status:";
					strError += "W";	//撤单标志W
					strError += "\nCheck Ordwth2_error ERROR! reff = ";
					EzLog::e(strError, aSHShare.reff);
					streamDB.close();
					mapRowData.clear();
					return -1;
				}
			}

			strTemp = mapRowData["price"].strValue;			//该订单成交金额
			int iRes = (int)strTemp.find_first_of(" ");
			if (iRes != -1)
			{
				strTemp.erase(iRes, 13);
			}
			if (strTemp != aSHShare.price)
			{
				std::string strError = "\nOrdwth price: ";
				strError += strTemp;
				strError += "\n预期   price: ";
				strError += aSHShare.price;
				strError += "\nCheck Ordwth2_Match ERROR! reff = ";
				EzLog::e(strError, aSHShare.reff);
				streamDB.close();
				mapRowData.clear();
				return -1;
			}

			strTemp = mapRowData["qty"].strValue;			//该订单的价格
			iRes = (int)strTemp.find_first_of(" ");
			if (iRes != -1)
			{
				strTemp.erase(iRes, 13);
			}
			if (strTemp != aSHShare.qty)
			{
				std::string strError = "\nOrdwth qty:";
				strError += strTemp;
				strError += "\n预期   qty:";
				strError += aSHShare.qty;
				strError += "\nCheck Ordwth2_Match ERROR! reff = ";
				EzLog::e(strError, aSHShare.reff);
				streamDB.close();
				mapRowData.clear();
				return -1;
			}
			break;

			if (Cancle == enumOrdwth2Report)
			{
				strTemp = mapRowData["qty2"].strValue;			//撤单数量；
				iRes = (int)strTemp.find_first_of(" ");
				if (iRes != -1)
				{
					strTemp.erase(iRes, 13);
				}
				if (strTemp != aSHShare.qty2)
				{
					std::string strError = "\nOrdwth qty2:";
					strError += strTemp;
					strError += "\n预期   qty2:";
					strError += aSHShare.qty2;
					strError += "\nCheck Ordwth2_Cancel ERROR! reff = ";
					EzLog::e(strError, aSHShare.reff);
					streamDB.close();
					mapRowData.clear();
					return -1;
				}
			}
		}// end else //校验确认字段
	}	//for ( k = 0; k < g_iQueryTimes; ++k)
	streamDB.close();
	mapRowData.clear();
	return 0;
}

//检查待成交单的确认；
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,
//输出 ： 0 正确； -1 异常
int CheckOrdwth2Match(OTLConn40240 &con, SHShare aSHShare)
{

	int iCheckRes;
	//成交确认 总数量
	std::string strQueryRecordNum = "select count(rec_num) from Ashare_OIW.dbo.ashare_ordwth2 where reff='";
	strQueryRecordNum += aSHShare.reff;
	strQueryRecordNum += "' and owflag='LPT';";


	int iRes = QueryRecordNum(con, strQueryRecordNum);
	if (iRes <= 0)
	{
		EzLog::e("达到最大查找次数，仍未查到 :", strQueryRecordNum);
		return -1;
	}
	else if (iRes > 1) //订单的确认回报有多条
	{
		EzLog::e("订单的确认回报有多条! \n", strQueryRecordNum);
		return -1;
	}
	else if (iRes == 1) //一条确认回报
	{
		iCheckRes = CheckOrdwth2(con, aSHShare, Ordwth2Report::Match);
	}
	return iCheckRes;
}

//检查撤单确认；
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,
//输出 ： 0 正确； -1 异常
long CheckOrdwth2Cancel(OTLConn40240 &con, SHShare aSHShare)
{
	int iCheckRes;
	//撤单确认 总数量
	std::string strQueryRecordNum = "select count(rec_num) from Ashare_OIW.dbo.ashare_ordwth2 where ordrec='";
	strQueryRecordNum += aSHShare.rec_num;
	strQueryRecordNum += "' and owflag ='WTH';";

	int iRes = QueryRecordNum(con, strQueryRecordNum);
	if (iRes <= 0)
	{
		EzLog::e("达到最大查找次数，仍未查到 :", strQueryRecordNum);
		return -1;
	}
	else if (iRes > 1) //订单的确认回报有多条
	{
		EzLog::e("订单的确认回报有多条! \n", strQueryRecordNum);
		return -1;
	}
	else if (iRes == 1) //一条确认回报
	{
		iCheckRes = CheckOrdwth2(con, aSHShare, Ordwth2Report::Cancle);
	}
	return iCheckRes;
}

//检查错单确认；
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,
//输出 ： 0 正确； -1 异常
long CheckOrdwth2Error(OTLConn40240 &con, SHShare aSHShare)
{
	int iCheckRes;
	//错单确认 总数量
	std::string strQueryRecordNum = "select count(rec_num) from Ashare_OIW.dbo.ashare_ordwth2 where reff ='";
	strQueryRecordNum += aSHShare.reff;
	strQueryRecordNum += "' and status = 'F';";

	int iRes = QueryRecordNum(con, strQueryRecordNum);
	if (iRes <= 0)
	{
		EzLog::e("达到最大查找次数，仍未查到 :", strQueryRecordNum);
		return -1;
	}
	else if (iRes > 1) //订单的确认回报有多条
	{
		EzLog::e("订单的确认回报有多条! \n", strQueryRecordNum);
		return -1;
	}
	else if (iRes == 1) //一条确认回报
	{
		iCheckRes = CheckOrdwth2(con, aSHShare, Ordwth2Report::Error);

	}
	return iCheckRes;
}


//检查已成交的成交回报：
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,iDivide 目前分笔数为2 
//输出 ： 0 正确； -1 异常
long CheckCjhb(OTLConn40240 &con, SHShare aSHShare, int iDivideNum)
{
	char szTemp[65] = { "\0" };
	//成交回报 求总数量
	std::string strQueryRecordNum = "select count(rec_num) from Ashare_OIW.dbo.ashare_cjhb where sqbh='";
	strQueryRecordNum += aSHShare.reff;
	strQueryRecordNum += "';";

	int iRes = QueryRecordNum(con, strQueryRecordNum);
	if (iRes == -1 || iRes == 0) //出现异常 、 没有记录
	{
		//达到最大查询次数
		EzLog::e("达到最大查找次数，仍未查到 ：", strQueryRecordNum);
		return -1;
	}
	else if (iRes > iDivideNum) //分笔成交订单的成交回报有多条
	{
		std::string strError = strQueryRecordNum;
		strError += "\n分笔成交订单的成交回报数量过多，有  ";
		itoa(iRes, szTemp, 10);
		strError += szTemp;
		EzLog::e(strError, " 条\n");
		return -1;
	}
	else if (iRes > 0 && iRes < iDivideNum) //查到成交回报数量不足
	{
		std::string strError = strQueryRecordNum;
		strError += "\n分笔成交订单的成交回报数量不足，有  ";
		itoa(iRes, szTemp, 10);
		strError += szTemp;
		EzLog::e(strError, " 条\n");
		return -1;
	}
	else if (iRes == iDivideNum) //分笔数等于的成交回报数
	{
		long lQueryResult = 0;
		long lCjsl = 0;
		uint64_t ui64Temp = 0;
		uint64_t ui64Cjsl = 0;
		uint64_t ui64Cjje = 0;
		std::map<std::string, struct OTLConn_DF::DataInRow> mapRowData;
		otl_stream streamDB;
		std::string strTemp;
		//成交回报 查询每条记录
		std::string strQueryCjhb = "select * from ashare_cjhb where sqbh='";
		strQueryCjhb += aSHShare.reff;
		strQueryCjhb += "';";

		for (int k = 0; k < g_iQueryTimes; ++k)
		{
			mapRowData.clear();
			lQueryResult = con.Query(strQueryCjhb, &streamDB);
			if (0 != lQueryResult)	//查询异常
			{
				EzLog::e(__FUNCTION__, " Query sql server failed !");
				streamDB.close();
				Sleep(g_iTimeOut);
				continue;
			}
			for (int i = 0; i < iDivideNum; i++)
			{
				long lFetchResult = con.FetchNextRow(&streamDB, mapRowData);
				if (lFetchResult != 1)
				{
					streamDB.close();
					Sleep(g_iTimeOut);
					continue;
				}
				else   //校验确认字段
				{
					strTemp = mapRowData["gddm"].strValue;
					//*STREQ*和*STRNE*同时支持char*和wchar_t*类型的，*STRCASEEQ*和*STRCASENE*却只接收char*，
					if (strTemp != aSHShare.gddm)
					{
						std::string strError = "\nCJHB gddm: ";
						strError += strTemp;
						strError += "\n预期 gddm: ";
						strError += aSHShare.gddm;
						strError += "\nCheck Cjhb ERROR! reff = ";
						EzLog::e(strError, aSHShare.reff);
						streamDB.close();
						return  -1;
					}

					strTemp = mapRowData["zqdm"].strValue;
					if (strTemp != aSHShare.zqdm)
					{
						std::string strError = "\nCJHB zqdm: ";
						strError += strTemp;
						strError += "\n预期 zqdm: ";
						strError += aSHShare.zqdm;
						strError += "\nCheck Cjhb ERROR! reff = ";
						EzLog::e(strError, aSHShare.reff);
						streamDB.close();
						return  -1;
					}

					strTemp = mapRowData["bs"].strValue;
					if (strTemp != aSHShare.bs)
					{
						std::string strError = "\nCJHB bs: ";
						strError += strTemp;
						strError += "\n预期 bs: ";
						strError += aSHShare.bs;
						strError += "\nCheck Cjhb ERROR! reff = ";
						EzLog::e(strError, aSHShare.reff);
						streamDB.close();
						return  -1;
					}

					strTemp = mapRowData["cjsl"].strValue;
					iRes = Tgw_StringUtil::String2UInt64_strtoui64(strTemp, ui64Temp);
					if (0 != iRes)
					{
						EzLog::e(__FUNCTION__, "Tgw_StringUtil::String2UInt64_strtoui64 转换出错！");
					}
					ui64Cjsl += ui64Temp;

					strTemp = mapRowData["cjjg"].strValue;		//该订单的成交价格
					iRes = int(strTemp.find_first_of(" "));
					if (iRes != -1)
					{
						strTemp.erase(iRes, 13);
					}
					if (strTemp != aSHShare.cjjg)
					{
						std::string strError = "\nCJHB cjjg: ";
						strError += strTemp;
						strError += "\n预期 cjjg: ";
						strError += aSHShare.cjjg;
						strError += "\nCheck Cjhb ERROR! reff = ";
						EzLog::e(strError, aSHShare.reff);
						streamDB.close();
						return  -1;
					}
					strTemp = mapRowData["cjje"].strValue;
					iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(strTemp, ui64Temp);
					ui64Cjje += ui64Temp;
				}// eles 
			} // for (int i = 0; i < iDivideNum;i++)

			//总的成交数量
			strTemp = _ui64toa(ui64Cjsl, szTemp, 10);
			if (strTemp != aSHShare.cjsl)
			{
				std::string strError = "\nCJHB cjsl: ";
				strError += strTemp;
				strError += "\n预期 cjsl: ";
				strError += aSHShare.cjsl;
				strError += "\nCheck Cjhb ERROR! reff = ";
				EzLog::e(strError, aSHShare.reff);
				streamDB.close();
				return  -1;
			}

			if (ui64Cjje > 999999999990)				//cjje
			{
				strTemp = "-1";
			}
			else
			{
				//	ui64Cjje /= 10;
				Tgw_StringUtil::iLiToStr(ui64Cjje, strTemp, 2);
			}
			if (strTemp != aSHShare.cjje)
			{
				std::string strError = "\nCJHB cjje: ";
				strError += strTemp;
				strError += "\n预期 cjje: ";
				strError += aSHShare.cjje;
				strError += "\nCheck Cjhb ERROR! reff = ";
				EzLog::e(strError, aSHShare.reff);
				streamDB.close();
				return  -1;
			}

			break;
		} // else if (iRes == iDivideNum) //分笔数等于的成交回报数
	}//for (int k = 0; k < g_iQueryTimes; ++k)

	return 0;
}

// 校验simutgw写回本地mysql数据库的数据是否符合实际交易
//@parameter ： 单个SHShare aSHShare STGW处理的订单;
//@output ： -1 simutgw写回数据异常，0 ，写回数据正常;
int CheckStgwWriteAssetBackToMySQL(const SHShare aSHShare, StockAsset beginStockAsset, bool bSimulMatchByDivide)
{
	int iRes = 0;
	StockAsset endStockAsset;
	uint64_t ui64BeginTemp = 0;
	uint64_t ui64EndTemp = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64ASHShareCjsl = strtoull(aSHShare.cjsl.c_str(), NULL, 10);

	Sleep(g_iTimeOut * 5);
	// SocketAsset::Init函数读取mysql数据库socket_asset表构造股份结构体数据成员；
	iRes = endStockAsset.Init(aSHShare.account, aSHShare.stock);
	if (-1 == iRes)
	{
		EzLog::e("StockAsset 结构体初始化失败", "CheckSTGWWriteBackToMySQL（）exit!");
		return -1;
	}

	iRes = 0;
	//买单
	if ("B" == aSHShare.bs)
	{
		//比较 stock_auction_purchase_balance字段
		ui64EndTemp = strtoull(endStockAsset.stock_auction_purchase_balance.c_str(), NULL, 10);
		ui64BeginTemp = strtoull(beginStockAsset.stock_auction_purchase_balance.c_str(), NULL, 10);
		ui64Temp = ui64EndTemp - ui64BeginTemp;
		if (ui64Temp != ui64ASHShareCjsl)
		{
			std::string strError = "\n交易股票stock_id  ：";
			strError += beginStockAsset.stock_id;
			strError += "\n买 交易后stock_auction_purchase_balance  ：";
			strError += beginStockAsset.stock_auction_purchase_balance;
			strError += "\n买 交易后stock_auction_purchase_balance  ：";
			strError += endStockAsset.stock_auction_purchase_balance;
			strError += "\n实际成交数量                             ：";
			strError += aSHShare.cjsl;
			strError += "\nstock_asset计算得出数量                  ：";
			EzLog::e(strError, to_string(ui64Temp));
			iRes = -1;
		}

		//比较 stock_balance 字段
		ui64EndTemp = strtoull(endStockAsset.stock_balance.c_str(), NULL, 10);
		ui64BeginTemp = strtoull(beginStockAsset.stock_balance.c_str(), NULL, 10);
		ui64Temp = ui64EndTemp - ui64BeginTemp;
		if (ui64Temp != ui64ASHShareCjsl)
		{
			std::string strError = "\n交易股票stock_id  ：";
			strError += beginStockAsset.stock_id;
			strError += "\n买 交易前stock_balance ：";
			strError += beginStockAsset.stock_balance;
			strError += "\n买 交易后stock_balance ：";
			strError += endStockAsset.stock_balance;
			strError += "\n实际成交数量           ：";
			strError += aSHShare.cjsl;
			strError += "\nstock_asset计算得出数量： ";
			EzLog::e(strError, to_string(ui64Temp));
			iRes = -1;
		}
	} // end if( "B" == aSHShare.bs)

	//卖单
	if ("S" == aSHShare.bs)
	{
		//比较stock_available 与stock_etf_redemption_balance总和
		ui64EndTemp = strtoull(endStockAsset.stock_available.c_str(), NULL, 10);
		ui64EndTemp += strtoull(endStockAsset.stock_etf_redemption_balance.c_str(), NULL, 10);
		ui64BeginTemp = strtoull(beginStockAsset.stock_available.c_str(), NULL, 10);
		ui64BeginTemp += strtoull(beginStockAsset.stock_etf_redemption_balance.c_str(), NULL, 10);
		ui64Temp = ui64BeginTemp - ui64EndTemp;
		if (ui64Temp != ui64ASHShareCjsl)
		{
			std::string strError = "\n交易股票stock_id  ：";
			strError += beginStockAsset.stock_id;
			strError += "\n实际成交数量           ：";
			strError += aSHShare.cjsl;
			strError += "\nstock_asset计算得出数量： ";
			EzLog::e(strError, to_string(ui64Temp));
			iRes = -1;
		}

		//比较 stock_balance 字段
		ui64EndTemp = strtoull(endStockAsset.stock_balance.c_str(), NULL, 10);
		ui64BeginTemp = strtoull(beginStockAsset.stock_balance.c_str(), NULL, 10);
		ui64Temp = ui64BeginTemp - ui64EndTemp;
		if (ui64Temp != ui64ASHShareCjsl)
		{
			std::string strError = "\n交易股票stock_id  ：";
			strError += beginStockAsset.stock_id;
			strError += "\n卖 交易前stock_balance ：";
			strError += beginStockAsset.stock_balance;
			strError += "\n卖 交易后stock_balance ：";
			strError += endStockAsset.stock_balance;
			strError += "\n实际成交数量           ：";
			strError += aSHShare.cjsl;
			strError += "\nstock_asset计算得出数量： ";
			EzLog::e(strError, to_string(ui64Temp));
			iRes = -1;
		}
	}//  end id("S" == aSHShare.bs)

	//分笔成交（默认是分成两笔）时stock_last_balance的值是上一次成交订单stock_balance的值，不是下单前的stock_balance值
	if (bSimulMatchByDivide)
	{
		ui64BeginTemp = strtoull(beginStockAsset.stock_balance.c_str(), NULL, 10);
		if ("S" == aSHShare.bs)
		{
			ui64BeginTemp -= uint64_t(strtoull(aSHShare.qty.c_str(), NULL, 10)*(0.5));  //取成交第一笔时的stock_balance
		}
		if ("B" == aSHShare.bs)
		{
			ui64BeginTemp += uint64_t(strtoull(aSHShare.qty.c_str(), NULL, 10)*(0.5));
		}
		beginStockAsset.stock_balance = to_string(ui64BeginTemp);;
	}
	//比较stock_last_balance
	if (endStockAsset.stock_last_balance != beginStockAsset.stock_balance)
	{
		std::string strError = "\n交易股票stock_id  ：";
		strError += beginStockAsset.stock_id;
		strError += "\n交易前stock_balance      ：";
		strError += beginStockAsset.stock_balance;
		strError += "\n交易后stock_last_balance ：";
		strError += endStockAsset.stock_last_balance;
		strError += "\nstock_balance数量不符合交易后stock_last_balance\n ，请在MySQL Stock_asset表检查各个字段";
		EzLog::e("\n分笔成交  ", strError);
		iRes = -1;
	}

	return iRes;
}


// 校验simutgw写回本地mysql数据库的数据是否符合实际交易
//@parameter ： 大量买的总数量 ui64BCjsl, 大量卖单的总数量ui64SCjsl,
//@output ： -1 simutgw写回数据异常，0 ，写回数据正常;
int CheckStgwWriteAssetBackToMySQL(const StockAsset beginStockAsset, const uint64_t ui64BCjsl, const uint64_t ui64SCjsl)
{
	StockAsset	endStockAsset;
	uint64_t ui64Temp = 0;
	uint64_t ui64BeginTem = 0;
	uint64_t ui64EndTemp = 0;
	int flag = 0;

	//使用中发现stgw写回stock_asset表时间很长；systemgtest读取stock_asset数据但不是交易的最终结果；
	//这里必须等待stgw
	Sleep(g_iTimeOut * 5);
	endStockAsset.Init(beginStockAsset.account_id, beginStockAsset.stock_id);

	//买入数量
	ui64BeginTem = strtoull(beginStockAsset.stock_auction_purchase_balance.c_str(), NULL, 10);
	ui64EndTemp = strtoull(endStockAsset.stock_auction_purchase_balance.c_str(), NULL, 10);
	ui64Temp = ui64EndTemp - ui64BeginTem;
	//std::cout << "买 ui64EndTemp - ui64BeginTem =" << ui64Temp << " \t ui64BCjsl = " << ui64BCjsl << std::endl;
	if (ui64BCjsl != ui64Temp)
	{
		std::string strError = "\n交易股票stock_id       ：";
		strError += beginStockAsset.stock_id;
		strError += "\n买 交易前买入量        ：";
		strError += to_string(ui64BeginTem);
		strError += "\n买 交易后买入量        ：";
		strError += to_string(ui64EndTemp);
		strError += "\n实际成交数量           ：";
		strError += to_string(ui64BCjsl);
		strError += "\nstock_asset计算得出数量： ";
		EzLog::e(strError, to_string(ui64Temp));
		flag = -1;
	}

	//卖出数量
	ui64BeginTem = strtoull(beginStockAsset.stock_available.c_str(), NULL, 10);
	ui64BeginTem += strtoull(beginStockAsset.stock_etf_redemption_balance.c_str(), NULL, 10);
	ui64EndTemp = strtoull(endStockAsset.stock_available.c_str(), NULL, 10);
	ui64EndTemp += strtoull(endStockAsset.stock_etf_redemption_balance.c_str(), NULL, 10);

	ui64Temp = ui64BeginTem - ui64EndTemp;
	if (ui64Temp != ui64SCjsl)
	{
		std::string strError = "\n交易股票stock_id       ：";
		strError += beginStockAsset.stock_id;
		strError += "\n卖 交易前可卖余额      ：";
		strError += to_string(ui64BeginTem);
		strError += "\n卖 交易后可卖余额      ：";
		strError += to_string(ui64EndTemp);
		strError += "\n实际成交数量           ：";
		strError += to_string(ui64SCjsl);
		strError += "\nstock_asset计算得出数量：";
		EzLog::e(strError, to_string(ui64Temp));
		flag = -1;
	}
	return flag;
}

