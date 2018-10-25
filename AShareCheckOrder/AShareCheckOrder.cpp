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
		EzLog::e(strInsert, "未正常插入");
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
	std::string strTag = aSHShare.GetSQLCancelSentence();
	con.Exec(strTag, lAffectRow);
	if (lAffectRow != 1)
	{
		EzLog::e(strTag, "未正常插入撤单");
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

	long lQueryResult = con.Query(strQuery, &streamDB);
	if (lQueryResult != 0) //查询未完成
	{
		return -1;
	}

	int iRes = con.FetchNextRow(&streamDB, mapRowData);
	if (iRes == -1)
	{
		return -1;
	}
	iRes = atoi(mapRowData[""].strValue.c_str());
	streamDB.close();
	streamDB.~otl_stream();
	return iRes;
}

//检查待成交单的确认；
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,
//输出 ： 0 正确； -1 异常
long CheckOrdwth2Match(OTLConn40240 &con, SHShare aSHShare)
{
	long lQueryResult = 0;
	int k = 0;
	int iRes = 0;
	std::map<std::string, struct OTLConn_DF::DataInRow> mapRowData;
	otl_stream streamDB;	//otl 流
	std::string strTemp = "";
	std::string strQueryOrdwth2 = "";

	//比较确认结果；
	strQueryOrdwth2 = "select * from [Ashare_OIW].[dbo].[ashare_ordwth2] where reff ='";
	strQueryOrdwth2 += aSHShare.reff;
	strQueryOrdwth2 += "' and owflag='LPT';";
	//成交确认 总数量
	std::string strQueryRecordNum = "select count(rec_num) from Ashare_OIW.dbo.ashare_ordwth2 where reff='";
	strQueryRecordNum += aSHShare.reff;
	strQueryRecordNum += "' and owflag='LPT';";

	for (k = 0; k < g_iQueryTimes; k++)
	{
		iRes = QueryRecordNum(con, strQueryRecordNum);
		if (iRes == -1 || iRes == 0) //出现异常 、 没有记录，再查
		{
			if (k == g_iQueryTimes - 1)//达到最大查询次数
			{
				EzLog::e("达到最大查找次数，仍未查到 ：", strQueryOrdwth2);
				return -1;
			}
			Sleep(g_iTimeOut);
			continue;
		}
		else if (iRes > 1) //订单的确认回报有多条
		{
			EzLog::e("订单的确认回报有多条! ", strQueryRecordNum);
			return -1;
		}
		else if (iRes == 1) //一条确认回报
		{
			lQueryResult = con.Query(strQueryOrdwth2, &streamDB);
			if (0 != lQueryResult)	//查询未完成
			{
				EzLog::e(__FUNCTION__, " Query sql server failed !");
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
				strTemp = mapRowData["acc"].strValue;
				if (aSHShare.account.compare(strTemp))
				{
					EzLog::e("Ordwth acc: ", strTemp);
					EzLog::e("预期   acc: ", aSHShare.account);
					EzLog::e("Check Ordwth2_Match ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["stock"].strValue;
				if (strTemp != aSHShare.stock)
				{
					EzLog::e("Ordwth stock: ", strTemp);
					EzLog::e("预期   stock: ", aSHShare.stock);
					EzLog::e("Check Ordwth2_Match ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["bs"].strValue;
				if (strTemp != aSHShare.bs)
				{
					EzLog::e("Ordwth bs: ", strTemp);
					EzLog::e("预期   bs: ", aSHShare.bs);
					EzLog::e("Check Ordwth2_Match ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["price"].strValue;			//该订单成交金额
				iRes = (int)strTemp.find_first_of(" ");
				if (iRes != -1)
				{
					strTemp.erase(iRes, 13);
				}
				if (strTemp != aSHShare.price)
				{
					EzLog::e("Ordwth price: ", strTemp);
					EzLog::e("预期   price: ", aSHShare.price);
					EzLog::e("Check Ordwth2_Match ERROR! reff = ", aSHShare.reff);
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
					EzLog::e("Ordwth qty:", strTemp);
					EzLog::e("预期   qty:", aSHShare.qty);
					EzLog::e("Check Ordwth2_Match ERROR! reff = ", aSHShare.reff);
					return -1;
				}
				break;

			}// end else //校验确认字段
		}//else if (iRes == 1) //一条确认回报
	}	//for ( k = 0; k < g_iQueryTimes; k++)
	streamDB.close();
	mapRowData.clear();
	return 0;
}

//检查撤单确认；
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,
//输出 ： 0 正确； -1 异常
long CheckOrdwth2Cancel(OTLConn40240 &con, SHShare aSHShare)
{
	long lQueryResult = 0;
	int k = 0;
	int iRes = 0;
	std::map<std::string, struct OTLConn_DF::DataInRow> mapRowData;
	otl_stream streamDB;	//otl 流
	std::string strTemp = "";
	std::string strQueryOrdwth2 = "";

	//撤单确认结果；
	strQueryOrdwth2 = "select * from [Ashare_OIW].[dbo].[ashare_ordwth2] where ordrec='";
	strQueryOrdwth2 += aSHShare.rec_num;
	strQueryOrdwth2 += "' and owflag ='WTH';";

	//撤单确认 总数量
	std::string strQueryRecordNum = "select count(rec_num) from Ashare_OIW.dbo.ashare_ordwth2 where ordrec='";
	strQueryRecordNum += aSHShare.rec_num;
	strQueryRecordNum += "' and owflag ='WTH';";

	for (k = 0; k < g_iQueryTimes; k++)
	{
		iRes = QueryRecordNum(con, strQueryRecordNum);
		if (iRes == -1 || iRes == 0) //出现异常 、 没有记录，再查
		{
			if (k == g_iQueryTimes - 1)//达到最大查询次数
			{
				EzLog::e("达到最大查找次数，仍未查到 ：", strQueryOrdwth2);
				return -1;
			}
			Sleep(g_iTimeOut);
			continue;
		}
		else if (iRes > 1) //订单的确认回报有多条
		{
			EzLog::e("订单的确认回报有多条! ", strQueryRecordNum);
			return -1;
		}
		else if (iRes == 1) //一条确认回报
		{
			lQueryResult = con.Query(strQueryOrdwth2, &streamDB);
			if (0 != lQueryResult)	//查询异常
			{
				EzLog::e(__FUNCTION__, " Query sql server failed !");
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
			else   //校验确认字段
			{
				strTemp = mapRowData["acc"].strValue;
				if (strTemp != aSHShare.account)
				{
					EzLog::e("Ordwth acc: ", strTemp);
					EzLog::e("预期   acc: ", aSHShare.account);
					EzLog::e("Check Ordwth2_Cancel ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["stock"].strValue;
				if (strTemp != aSHShare.stock)
				{
					EzLog::e("Ordwth stock:", strTemp);
					EzLog::e("预期   stock:", aSHShare.stock);
					EzLog::e("Check Ordwth2_Cancel ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["bs"].strValue;
				if (strTemp != aSHShare.bs)
				{
					EzLog::e("Ordwth bs:", strTemp);
					EzLog::e("预期   bs:", aSHShare.bs);
					EzLog::e("Check Ordwth2_Cancel ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["price"].strValue;			//该订单成交金额
				iRes = (int)strTemp.find_first_of(" ");
				if (iRes != -1)
				{
					strTemp.erase(iRes, 13);
				}
				if (strTemp != aSHShare.price)
				{
					EzLog::e("Ordwth price:", strTemp);
					EzLog::e("预期   price:", aSHShare.price);
					EzLog::e("Check Ordwth2_Cancel ERROR! reff = ", aSHShare.reff);
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
					EzLog::e("Ordwth qty:", strTemp);
					EzLog::e("预期   qty:", aSHShare.qty);
					EzLog::e("Check Ordwth2_Cancel ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["qty2"].strValue;			//撤单数量；
				iRes = (int)strTemp.find_first_of(" ");
				if (iRes != -1)
				{
					strTemp.erase(iRes, 13);
				}
				if (strTemp != aSHShare.qty2)
				{
					EzLog::e("Ordwth qty2:", strTemp);
					EzLog::e("预期   qty2:", aSHShare.qty2);						//
					EzLog::e("Check Ordwth2_Cancel ERROR! reff = ", aSHShare.reff);
					return -1;
				}
				break;
			}// end else //校验确认字段
		} //else if (iRes == 1) //一条确认回报
	}	//for ( k = 0; k < g_iQueryTimes; k++)

	streamDB.close();
	mapRowData.clear();
	return 0;
}

//检查错单确认；
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,
//输出 ： 0 正确； -1 异常
long CheckOrdwth2Error(OTLConn40240 &con, SHShare aSHShare)
{
	long lQueryResult = 0;
	int k = 0;
	int iRes = 0;
	std::map<std::string, struct OTLConn_DF::DataInRow> mapRowData;
	otl_stream streamDB;	//otl 流
	std::string strTemp = "";
	//错单确认 查询每条记录
	std::string strQueryOrdwth2 = "select * from [Ashare_OIW].[dbo].[ashare_ordwth2] where reff ='";
	strQueryOrdwth2 += aSHShare.reff;
	strQueryOrdwth2 += "' and status = 'F';";

	//错单确认 总数量
	std::string strQueryRecordNum = "select count(rec_num) from Ashare_OIW.dbo.ashare_ordwth2 where reff ='";
	strQueryRecordNum += aSHShare.reff;
	strQueryRecordNum += "' and status = 'F';";

	for (k = 0; k < g_iQueryTimes; k++)
	{
		iRes = QueryRecordNum(con, strQueryRecordNum);
		if (iRes == -1 || iRes == 0) //出现异常 、 没有记录，再查
		{
			if (k == g_iQueryTimes - 1)//达到最大查询次数
			{
				EzLog::e("达到最大查找次数，仍未查到 ：", strQueryOrdwth2);
				return -1;
			}
			Sleep(g_iTimeOut);
			continue;
		}
		else if (iRes > 1) //订单的确认回报有多条
		{
			EzLog::e("订单的确认回报有多条! ", strQueryRecordNum);
			return -1;
		}
		else if (iRes == 1) //一条确认回报
		{
			lQueryResult = con.Query(strQueryOrdwth2, &streamDB);
			if (0 != lQueryResult)	//查询异常
			{
				EzLog::e(__FUNCTION__, " Query sql server failed !");
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
			else   //校验确认字段
			{
				strTemp = mapRowData["acc"].strValue;
				if (strTemp != aSHShare.account)
				{
					EzLog::e("Ordwth acc:", strTemp);
					EzLog::e("预期   acc:", aSHShare.account);
					EzLog::e("Check Ordwth2_error ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["stock"].strValue;
				if (strTemp != aSHShare.stock)
				{
					EzLog::e("Ordwth stock:", strTemp);
					EzLog::e("预期   stock:", aSHShare.stock);
					EzLog::e("Check Ordwth2_error ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["bs"].strValue;
				if (strTemp != aSHShare.bs)
				{
					EzLog::e("Ordwth bs:", strTemp);
					EzLog::e("预期   bs:", aSHShare.bs);
					EzLog::e("Check Ordwth2_error ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["price"].strValue;			//该订单成交金额
				iRes = (int)strTemp.find_first_of(" ");
				if (iRes != -1)
				{
					strTemp.erase(iRes, 13);
				}
				if (strTemp != aSHShare.price)
				{
					EzLog::e("Ordwth price:", strTemp);
					EzLog::e("预期   price:", aSHShare.price);
					EzLog::e("Check Ordwth2_error ERROR! reff = ", aSHShare.reff);
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
					EzLog::e("Ordwth qty:", strTemp);
					EzLog::e("预期   qty:", aSHShare.qty);
					EzLog::e("Check Ordwth2_error ERROR! reff = ", aSHShare.reff);
					return -1;
				}

				strTemp = mapRowData["status"].strValue;
				if ("F" != strTemp)
				{
					EzLog::e("Ordwth status:", strTemp);
					EzLog::e("预期   status:", "F");//错单标志F
					EzLog::e("Check Ordwth2_error ERROR! reff = ", aSHShare.reff);
					return -1;
				}
				break;
			}// end else  //校验确认字段
		}  //else if (iRes == 1) //一条确认回报
	}	//for ( k = 0; k < g_iQueryTimes; k++)

	streamDB.close();
	mapRowData.clear();
	return 0;
}

//检查全部成交、部分成交，挂单成交的成交回报：
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,
//输出 ： 0 正确； -1 异常
long CheckCjhb(OTLConn40240 &con, SHShare aSHShare)
{
	long lQueryResult = 0;
	long lCjsl = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjsl = 0;
	uint64_t ui64Cjje = 0;
	std::map<std::string, struct OTLConn_DF::DataInRow> mapRowData;
	otl_stream streamDB;
	std::string strTemp = "";
	//成交回报 查询每条记录
	std::string strQueryCjhb = "select * from ashare_cjhb where sqbh='";
	strQueryCjhb += aSHShare.reff;
	strQueryCjhb += "';";
	//成交回报 求总数量
	std::string strQueryRecordNum = "select count(rec_num) from Ashare_OIW.dbo.ashare_cjhb where sqbh='";
	strQueryRecordNum += aSHShare.reff;
	strQueryRecordNum += "';";

	int  k = 0;
	int iRes = 0;
	char szTemp[65] = { "\0" };

	for (k = 0; k < g_iQueryTimes; k++)
	{
		iRes = QueryRecordNum(con, strQueryRecordNum);
		if (iRes == -1 || iRes == 0) //出现异常 、 没有记录，再查
		{
			if (k == g_iQueryTimes - 1)//达到最大查询次数
			{
				EzLog::e("达到最大查找次数，仍未查到 ：", strQueryCjhb);
				return -1;
			}
			Sleep(g_iTimeOut);
			continue;
		}
		else if (iRes > 1) //成交订单的成交回报有多条
		{
			EzLog::e("成交订单的成交回报有多条! ", strQueryRecordNum);
			return -1;
		}
		else if (iRes == 1) //一条成交回报
		{
			lQueryResult = con.Query(strQueryCjhb, &streamDB);
			if (0 != lQueryResult)	//查询异常
			{
				EzLog::e(__FUNCTION__, " Query sql server failed !");
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
			else   //校验回报字段
			{
				strTemp = mapRowData["gddm"].strValue;
				//*STREQ*和*STRNE*同时支持char*和wchar_t*类型的，*STRCASEEQ*和*STRCASENE*却只接收char*，
				if (strTemp != aSHShare.gddm)
				{
					EzLog::e("\tCJHB gddm: ", strTemp);
					EzLog::e("\t预期 gddm: ", aSHShare.gddm);
					EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
					return  -1;
				}

				strTemp = mapRowData["zqdm"].strValue;
				if (strTemp != aSHShare.zqdm)
				{
					EzLog::e("\tCJHB zqdm: ", strTemp);
					EzLog::e("\t预期 zqdm: ", aSHShare.zqdm);
					EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
					return  -1;
				}

				strTemp = mapRowData["bs"].strValue;
				if (strTemp != aSHShare.bs)
				{
					EzLog::e("\tCJHB bs: ", strTemp);
					EzLog::e("\t预期 bs: ", aSHShare.bs);
					EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
					return  -1;
				}

				strTemp = mapRowData["cjsl"].strValue;
				iRes = Tgw_StringUtil::String2UInt64_strtoui64(strTemp, ui64Temp);
				if (0 != iRes)
				{
					EzLog::e(__FUNCTION__, "Tgw_StringUtil::String2UInt64_strtoui64 转换出错！");
				}
				ui64Cjsl = ui64Temp;

				strTemp = mapRowData["cjjg"].strValue;		//该订单的成交价格
				iRes = int(strTemp.find_first_of(" "));
				if (iRes != -1)
				{
					strTemp.erase(iRes, 13);
				}
				if (strTemp != aSHShare.cjjg)
				{
					EzLog::e("\tCJHB cjjg: ", strTemp);
					EzLog::e("\t预期 cjjg: ", aSHShare.cjjg);
					EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
					return  -1;
				}
				strTemp = mapRowData["cjje"].strValue;
				iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(strTemp, ui64Temp);
				ui64Cjje = ui64Temp;

				//成交总的成交数量和金额比较
				strTemp = _ui64toa(ui64Cjsl, szTemp, 10);
				if (ui64Cjje > 999999999990)				//cjje
				{
					strTemp = "-1";
					if ("-1" != aSHShare.cjje)
					{
						EzLog::e("CJHB cjje: ", strTemp);
						EzLog::e("预期 cjje: ", aSHShare.cjje);
						EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
						return  -1;
					}
				}
				else
				{
					//	ui64Cjje /= 10;
					Tgw_StringUtil::iLiToStr(ui64Cjje, strTemp, 2);
					if (strTemp != aSHShare.cjje)
					{
						EzLog::e("CJHB cjje: ", strTemp);
						EzLog::e("预期 cjje: ", aSHShare.cjje);
						EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
						return  -1;
					}
				}
				break;
			}// eles 
		}//else if (iRes == 1) //一条确认回报
	}//for (int k = 0; k < g_iQueryTimes; k++)
	streamDB.close();	//显式关闭
	mapRowData.clear();	//显式清空
	return 0;
}

//检查分笔成交的成交回报：
//输入：OTLConn40240 &con 连接数据库, SHShare aShare待检查股票订单,iDivide 目前分笔数为2 
//输出 ： 0 正确； -1 异常
long CheckDivideCjhb(OTLConn40240 &con, SHShare aSHShare, int iDivideNum = 2)
{
	long lQueryResult = 0;
	long lCjsl = 0;
	uint64_t ui64Temp = 0;
	uint64_t ui64Cjsl = 0;
	uint64_t ui64Cjje = 0;
	std::map<std::string, struct OTLConn_DF::DataInRow> mapRowData;
	otl_stream streamDB;
	std::string strTemp = "";
	//成交回报 查询每条记录
	std::string strQueryCjhb = "select * from ashare_cjhb where sqbh='";
	strQueryCjhb += aSHShare.reff;
	strQueryCjhb += "';";
	//成交回报 求总数量
	std::string strQueryRecordNum = "select count(rec_num) from Ashare_OIW.dbo.ashare_cjhb where sqbh='";
	strQueryRecordNum += aSHShare.reff;
	strQueryRecordNum += "';";

	int  k = 0;
	int iRes = 0;
	char szTemp[65] = { "\0" };

	for (k = 0; k < g_iQueryTimes; k++)
	{
		iRes = QueryRecordNum(con, strQueryRecordNum);
		if (iRes == -1 || iRes == 0) //出现异常 、 没有记录，再查
		{
			if (k == g_iQueryTimes - 1)//达到最大查询次数
			{
				EzLog::e("达到最大查找次数，仍未查到 ：", strQueryCjhb);
				return -1;
			}
			Sleep(g_iTimeOut);
			continue;
		}
		else if (iRes > iDivideNum) //分笔成交订单的成交回报有多条
		{
			EzLog::e("分笔成交订单的成交回报有多条! ", strQueryRecordNum);
			return -1;
		}
		else if (iRes > 0 && iRes < iDivideNum) //查到成交回报数量不足
		{
			EzLog::e("分笔成交订单的成交回报数量不足！ ", strQueryRecordNum);
			return -1;
		}
		else if (iRes == iDivideNum) //分笔数等于的成交回报数
		{
			lQueryResult = con.Query(strQueryCjhb, &streamDB);
			if (0 != lQueryResult)	//查询异常
			{
				EzLog::e(__FUNCTION__, " Query sql server failed !");
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
						EzLog::e("\tCJHB gddm: ", strTemp);
						EzLog::e("\t预期 gddm: ", aSHShare.gddm);
						EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
						return  -1;
					}

					strTemp = mapRowData["zqdm"].strValue;
					if (strTemp != aSHShare.zqdm)
					{
						EzLog::e("\tCJHB zqdm: ", strTemp);
						EzLog::e("\t预期 zqdm: ", aSHShare.zqdm);
						EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
						return  -1;
					}

					strTemp = mapRowData["bs"].strValue;
					if (strTemp != aSHShare.bs)
					{
						EzLog::e("\tCJHB bs: ", strTemp);
						EzLog::e("\t预期 bs: ", aSHShare.bs);
						EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
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
						EzLog::e("\tCJHB cjjg: ", strTemp);
						EzLog::e("\t预期 cjjg: ", aSHShare.cjjg);
						EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
						return  -1;
					}
					strTemp = mapRowData["cjje"].strValue;
					iRes = Tgw_StringUtil::String2UInt64MoneyInLi_strtoui64(strTemp, ui64Temp);
					ui64Cjje += ui64Temp;
				}// eles 
			} // for (int i = 0; i < iDivideNum;i++)

			//成交总的成交数量和金额比较
			strTemp = _ui64toa(ui64Cjsl, szTemp, 10);
			if (ui64Cjje > 999999999990)				//cjje
			{
				strTemp = "-1";
				if ("-1" != aSHShare.cjje)
				{
					EzLog::e("CJHB cjje: ", strTemp);
					EzLog::e("预期 cjje: ", aSHShare.cjje);
					EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
					return  -1;
				}
			}
			else
			{
				//	ui64Cjje /= 10;
				Tgw_StringUtil::iLiToStr(ui64Cjje, strTemp, 2);
				if (strTemp != aSHShare.cjje)
				{
					EzLog::e("CJHB cjje: ", strTemp);
					EzLog::e("预期 cjje: ", aSHShare.cjje);
					EzLog::e("Check Cjhb ERROR! reff = ", aSHShare.reff);
					return  -1;
				}
			}
			break;
		} // else if (iRes == iDivideNum) //分笔数等于的成交回报数
	}//for (int k = 0; k < g_iQueryTimes; k++)

	streamDB.close();	//显式关闭
	mapRowData.clear();	//显式清空
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
			EzLog::e("\n\n", beginStockAsset.stock_id);
			EzLog::e("买 交易前stock_auction_purchase_balance ：", beginStockAsset.stock_auction_purchase_balance);
			EzLog::e("买 交易后stock_auction_purchase_balance ：", endStockAsset.stock_auction_purchase_balance);
			EzLog::e("买单前后成交数量差值不符合，实际成交数量 ：", aSHShare.cjsl);
			EzLog::e("stock_asset计算得出数量： ", to_string(ui64Temp));
			iRes = -1;
		}

		//比较 stock_balance 字段
		ui64EndTemp = strtoull(endStockAsset.stock_balance.c_str(), NULL, 10);
		ui64BeginTemp = strtoull(beginStockAsset.stock_balance.c_str(), NULL, 10);
		ui64Temp = ui64EndTemp - ui64BeginTemp;
		if (ui64Temp != ui64ASHShareCjsl)
		{
			EzLog::e("\n\n", beginStockAsset.stock_id);
			EzLog::e("买 交易前stock_balance ：", beginStockAsset.stock_balance);
			EzLog::e("买 交易后stock_balance ：", endStockAsset.stock_balance);
			EzLog::e("买单前后成交数量差值不符合，实际成交数量 ：", aSHShare.cjsl);
			EzLog::e("stock_asset计算得出数量： ", to_string(ui64Temp));
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
			EzLog::e("\n\n", beginStockAsset.stock_id);
			EzLog::e("卖单前后成交可用额度不符合，实际成交数量 ：", aSHShare.cjsl);
			EzLog::e("stock_asset计算得出数量： ", to_string(ui64Temp));
			iRes = -1;
		}

		//比较 stock_balance 字段
		ui64EndTemp = strtoull(endStockAsset.stock_balance.c_str(), NULL, 10);
		ui64BeginTemp = strtoull(beginStockAsset.stock_balance.c_str(), NULL, 10);
		ui64Temp = ui64BeginTemp - ui64EndTemp;
		if (ui64Temp != ui64ASHShareCjsl)
		{
			EzLog::e("\n\n", beginStockAsset.stock_id);
			EzLog::e("卖 交易前stock_balance ：", beginStockAsset.stock_balance);
			EzLog::e("卖 交易后stock_balance ：", endStockAsset.stock_balance);
			EzLog::e("卖单前后成交数量差值不符合，实际成交数量 ：", aSHShare.cjsl);
			EzLog::e("stock_asset计算得出数量： ", to_string(ui64Temp));
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
		EzLog::e("\n\n", beginStockAsset.stock_id);
		EzLog::e("交易前stock_balance      ：", beginStockAsset.stock_balance);
		EzLog::e("交易后stock_last_balance ：", endStockAsset.stock_last_balance);
		EzLog::e("stock_balance数量不符合交易后stock_last_balance\n",
			"请在MySQL Stock_asset表检查各个字段");
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
	std::string strTemp = "";
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
		EzLog::e("\n\n", beginStockAsset.stock_id);
		EzLog::e("买 交易前买入量 ：", to_string(ui64BeginTem));
		EzLog::e("买 交易后买入量 ：", to_string(ui64EndTemp));
		EzLog::e("买单前后成交数量差值不符合，实际成交数量 ：", to_string(ui64BCjsl));
		EzLog::e("stock_asset计算得出数量： ", to_string(ui64Temp));
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
		EzLog::e("\n\n", beginStockAsset.stock_id);
		EzLog::e("卖 交易前可卖余额 ：", to_string(ui64BeginTem));
		EzLog::e("卖 交易后可卖余额 ：", to_string(ui64EndTemp));
		EzLog::e("卖单前后成交数量差值不符合，实际成交数量 ：", to_string(ui64SCjsl));
		EzLog::e("stock_asset计算得出数量： ", to_string(ui64Temp));
		flag = -1;
	}
	return flag;
}

