/*
深圳行情网关推送的行情格式：
{
"ZQDM": "002112",
"ZQMC": "",
"CJSL": "4056106.00",
"CJJE": "27732136.1000",
"ZJJG": "6.880000",
"ZRSP": "6.7600",
"JRKP": "6.790000",
"ZGJG": "6.900000",
"ZDJG": "6.740000",
"CJBS": "3116",
"SJW1": "6.890000",
"SJW2": "6.900000",
"SJW3": "6.910000",
"SJW4": "6.920000",
"SJW5": "6.930000",
"SSL1": "53600.00",
"SSL2": "49500.00",
"SSL3": "16100.00",
"SSL4": "14700.00",
"SSL5": "37400.00",
"BJW1": "6.880000",
"BJW2": "6.870000",
"BJW3": "6.860000",
"BJW4": "6.850000",
"BJW5": "6.840000",
"BSL1": "20700.00",
"BSL2": "44900.00",
"BSL3": "20100.00",
"BSL4": "7500.00",
"BSL5": "8400.00",
"SYL1": "0.000000",
"SYL2": "0",
"TPBZ": "F",
"PriceUpperLimit": "7.440000",
"PriceLowerLimit": "6.080000",
"OrigTime": "2018-08-27 16:13:03.000",
"hqrq": "20180827",
"hqsj": "161303",
"OrgTradingPhaseCode": "E0",
"hqktype": "SZSTEP.W.010"
}
*/

#include "QuotationBuild.h"

//设计特定的行情数据:
//输入：待处理行情AStockQuot &inout_aStockQuot ，行情价格1.000元，
//输出：返回引用行情，
void CreateQuotationExample(AStockQuot &inout_aStockQuot)
{
	//初始化AStockQuot结构体，构造一条行情的信息；

	//	,sz，zqdm:证券代码，zqmc：证券名称，
	inout_aStockQuot.zqdm = "600302";
	inout_aStockQuot.zqmc = "标准股份";
	//cjsl:成交数量   --变动，  cjje:成交金额   --变动，zjjg:最近成交价   --现价 变动， 
	inout_aStockQuot.cjsl = 100000;
	inout_aStockQuot.cjje = 100000000;
	inout_aStockQuot.zjjg = 1000;
	//zrsp:昨日收盘价，jrkp:今日开盘价，
	inout_aStockQuot.zrsp = 1000;
	inout_aStockQuot.jrkp = 1000;
	//zgjg：最高成交价   --变动 ，zdjg:最低成交价  --变动，
	inout_aStockQuot.zgjg = 1100;
	inout_aStockQuot.zdjg = 900;
	//cjbs:成交笔数 --变动，
	inout_aStockQuot.cjbs = 100;
	//SJW1：卖出价格1 ， ，SJW2：卖出价格2,，，，，，
	inout_aStockQuot.SJW1 = 1045;
	inout_aStockQuot.SJW2 = 1046;
	inout_aStockQuot.SJW3 = 1047;
	inout_aStockQuot.SJW4 = 1048;
	inout_aStockQuot.SJW5 = 1049;
	//卖数量1 ，2， 3，4， 5；
	inout_aStockQuot.SSL1 = 100000000;		//SSL BSL 都放大1000；方便后面用Litostr（）转换
	inout_aStockQuot.SSL2 = 100000000;
	inout_aStockQuot.SSL3 = 100000000;
	inout_aStockQuot.SSL4 = 100000000;
	inout_aStockQuot.SSL5 = 100000000;
	//买价格1， 2 ， 3 ， 4， 5；
	inout_aStockQuot.BJW1 = 1050;
	inout_aStockQuot.BJW2 = 1040;
	inout_aStockQuot.BJW3 = 1030;
	inout_aStockQuot.BJW4 = 1020;
	inout_aStockQuot.BJW5 = 1010;
	//BSL4：买入数量4 --变动， BSL5 ：买入数量5 --变动，
	inout_aStockQuot.BSL1 = 100000000;
	inout_aStockQuot.BSL2 = 100000000;
	inout_aStockQuot.BSL3 = 100000000;
	inout_aStockQuot.BSL4 = 100000000;
	inout_aStockQuot.BSL5 = 100000000;
	//SYL1:市盈率1 --变动，SYL2 : 市盈率2--变动r，
	inout_aStockQuot.SYL1 = 110;
	inout_aStockQuot.SYL2 = 0;
	//TPBZ:停盘标志 --变动，
	inout_aStockQuot.TPBZ = "F";
	inout_aStockQuot.hqktype = "SHHQ";
	// 行情时间
	TimeStringUtil::GetCurrTimeInTradeType(inout_aStockQuot.hqsj);
	inout_aStockQuot.hqsj += ".000";					//毫秒

	inout_aStockQuot.maxgain = 1100;	//会限制涨跌幅 
	inout_aStockQuot.minfall = 900;
	inout_aStockQuot.hqmarket = 1;
	//boost::posix_time::ptime timehqsj;

	//MDStreamID :行情数据类型 
	inout_aStockQuot.MDStreamID = "MD002";
	inout_aStockQuot.PriceUpperLimit = "1.1000";
	inout_aStockQuot.PriceLowerLimit = "0.9000";
	inout_aStockQuot.OriginStr = "";
}

//设计新行情NewExample 
//输入：行情结构体AStockQuot & inout_aStockQuot， 区间段均价0.900元
//输出：赋值于该行情并返回 
void CreateQuotationNewExample(AStockQuot &inout_aStockQuot)
{
	//构造新的行情；

	//	,sz，zqdm:证券代码，zqmc：证券名称，
	inout_aStockQuot.zqdm = "600302";
	inout_aStockQuot.zqmc = "标准股份";
	//cjsl:成交数量   --变动，  cjje:成交金额   --变动，zjjg:最近成交价   --现价 变动， 
	inout_aStockQuot.cjsl = 200000;
	inout_aStockQuot.cjje = 190000000;
	inout_aStockQuot.zjjg = 900;
	//zrsp:昨日收盘价，jrkp:今日开盘价，
	inout_aStockQuot.zrsp = 1000;
	inout_aStockQuot.jrkp = 1000;
	//zgjg：最高成交价   --变动 ，zdjg:最低成交价  --变动，
	inout_aStockQuot.zgjg = 1000;
	inout_aStockQuot.zdjg = 800;
	//cjbs:成交笔数 --变动，
	inout_aStockQuot.cjbs = 200;
	//SJW1：卖出价格1 ， ，SJW2：卖出价格2,，，，，，
	inout_aStockQuot.SJW1 = 895;
	inout_aStockQuot.SJW2 = 896;
	inout_aStockQuot.SJW3 = 897;
	inout_aStockQuot.SJW4 = 898;
	inout_aStockQuot.SJW5 = 899;
	//卖数量1 ，2， 3，4， 5；
	inout_aStockQuot.SSL1 = 100000000;  ///SSL　和 BSL 值比实际值放大1000倍，原因是后面用litostr（）函数会把值默认当作是厘为单位，而不是个位或者元为单位
	inout_aStockQuot.SSL2 = 100000000;
	inout_aStockQuot.SSL3 = 100000000;
	inout_aStockQuot.SSL4 = 100000000;
	inout_aStockQuot.SSL5 = 100000000;
	//买价格1， 2 ， 3 ， 4， 5；
	inout_aStockQuot.BJW1 = 919;
	inout_aStockQuot.BJW2 = 912;
	inout_aStockQuot.BJW3 = 903;
	inout_aStockQuot.BJW4 = 902;
	inout_aStockQuot.BJW5 = 901;
	//BSL4：买入数量4 --变动， BSL5 ：买入数量5 --变动，
	inout_aStockQuot.BSL1 = 100000000;
	inout_aStockQuot.BSL2 = 100000000;
	inout_aStockQuot.BSL3 = 100000000;
	inout_aStockQuot.BSL4 = 100000000;
	inout_aStockQuot.BSL5 = 100000000;
	//SYL1:市盈率1 --变动，SYL2 : 市盈率2--变动r，
	inout_aStockQuot.SYL1 = 110;
	inout_aStockQuot.SYL2 = 0;
	//TPBZ:停盘标志 --变动，
	inout_aStockQuot.TPBZ = "F";
	// 上海市场是SHHQ ， 深圳市场是 SZSTEP.W.010
	inout_aStockQuot.hqktype = "SHHQ";		//
	// 行情时间
	TimeStringUtil::GetCurrTimeInTradeType(inout_aStockQuot.hqsj);
	inout_aStockQuot.hqsj += ".500";					//毫秒

	inout_aStockQuot.maxgain = 1000;	//	限制涨跌幅 
	inout_aStockQuot.minfall = 800;
	inout_aStockQuot.hqmarket = 1;
	//boost::posix_time::ptime timehqsj;

	//MDStreamID :行情数据类型 
	inout_aStockQuot.MDStreamID = "MD002";
	inout_aStockQuot.PriceUpperLimit = "1.0000";
	inout_aStockQuot.PriceLowerLimit = "0.8000";
	inout_aStockQuot.OriginStr = "";
}

//将行情推送redis 
//输入：待推送的行情结构体 in_aStockQuot;
//输出：0，表示程序正确执行；  1，表示出现错误
int SendQuotToRedis(AStockQuot& in_aStockQuot)
{
	int iRes = 0;
	std::string strRedisRes;
	long long llRes = 0;
	int  iStrLen = 0;
	size_t ElemSize = 0;
	vector<string> vctArgs;
	std::string strTran = "";
	std::string strQuotation = "";

	iRes = simutgw::g_redisPool.SetConnection("127.0.0.1", 6600, 10, 0, "");
	if (0 != iRes)
	{
		EzLog::e("SendQuotToRedis( )", "SetConnection failed !");
		return -1;
	}

	iRes = Tgw_RedisHelper::LoadHiredisLibrary();
	if (0 != iRes)
	{
		EzLog::e("SendQuotToRedis( )", "LoadHiredisLibrary failed !");
		return -1;
	}

	iRes = simutgw::g_redisPool.Init();
	if (0 != iRes)
	{
		EzLog::e("SendQuotToRedis( )", "g_redisPool.Init failed !");
		return -1;
	}

	//生成行情string
	strQuotation = "{\"";
	strQuotation += AStockQuotName::zqdm;
	strQuotation += "\":\"";
	strQuotation += in_aStockQuot.zqdm;

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::zqmc;
	strQuotation += "\":\"";
	strQuotation += in_aStockQuot.zqmc;

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::cjsl;
	strQuotation += "\":\"";
	sof_string::itostr(in_aStockQuot.cjsl, strTran);
	strTran += ".00";
	strQuotation += strTran;

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::cjje;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.cjje, strTran, 4);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::zjjg;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.zjjg, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::zrsp;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.zrsp, strTran, 4);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::jrkp;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.jrkp, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::zgjg;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.zgjg, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::zdjg;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.zdjg, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::cjbs;
	strQuotation += "\":\"";
	strQuotation += sof_string::itostr(in_aStockQuot.cjbs, strTran);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SJW1;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SJW1, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SJW2;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SJW2, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SJW3;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SJW3, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SJW4;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SJW4, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SJW5;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SJW5, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SSL1;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SSL1, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SSL2;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SSL2, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SSL3;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SSL3, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SSL4;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SSL4, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SSL5;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.SSL5, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BJW1;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BJW1, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BJW2;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BJW2, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BJW3;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BJW3, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BJW4;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BJW4, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BJW5;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BJW5, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BSL1;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BSL1, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BSL2;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BSL2, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BSL3;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BSL3, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BSL4;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BSL4, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::BSL5;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr(in_aStockQuot.BSL5, strTran, 2);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SYL1;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr((int64_t)in_aStockQuot.SYL1, strTran, 6);


	strQuotation += "\",\"";
	strQuotation += AStockQuotName::SYL2;
	strQuotation += "\":\"";
	strQuotation += Tgw_StringUtil::iLiToStr((int64_t)in_aStockQuot.SYL2, strTran, 6);

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::TPBZ;
	strQuotation += "\":\"";
	strQuotation += in_aStockQuot.TPBZ;

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::hqktype;
	strQuotation += "\":\"";
	strQuotation += in_aStockQuot.hqktype;

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::hqsj;
	strQuotation += "\":\"";
	strQuotation += in_aStockQuot.hqsj;

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::MDStreamID;
	strQuotation += "\":\"";
	strQuotation += in_aStockQuot.MDStreamID;

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::PriceUpperLimit;
	strQuotation += "\":\"";
	strQuotation += in_aStockQuot.PriceUpperLimit;

	strQuotation += "\",\"";
	strQuotation += AStockQuotName::PriceLowerLimit;
	strQuotation += "\":\"";
	strQuotation += in_aStockQuot.PriceLowerLimit;

	strQuotation += "\"}";
	in_aStockQuot.OriginStr = strQuotation;
	vctArgs.push_back("rpush");
	vctArgs.push_back("hqk_change");
	vctArgs.push_back(strQuotation);

	//RunCmdArgv() 推送行情到redis;
	std::vector<StgwRedisReply> vectRedisRes_Array;
	RedisReply emPcbCallRes = Tgw_RedisHelper::RunCmdArgv(vctArgs,
		&llRes,
		&strRedisRes,
		&iStrLen,
		&vectRedisRes_Array,
		&ElemSize
		);
	if (RedisReply_integer != emPcbCallRes)
	{
		EzLog::e("SendQuotToRedis( )", " RunCmdArgv failed !");
		return -1;
	}

	simutgw::g_redisPool.Stop();

	Tgw_RedisHelper::FreeHiredisLibrary();
	return 0;
}


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
	const std::string strZqdm, std::string& strTpbz,
	uint64_t& ui64MaxGain, uint64_t& ui64MinFall, uint64_t& ui64Cjje,
	uint64_t& ui64Cjsl, uint64_t& ui64SJW1, uint64_t& ui64SSL1,
	uint64_t& ui64BJW1, uint64_t& ui64BSL1, uint64_t& ui64Zjjg)
{
	//连接Redis数据库
	int iRes = 0;
	simutgw::g_redisPool.SetConnection("127.0.0.1", 6600, 10, 0, "");

	//加载Redis.dll
	Tgw_RedisHelper::LoadHiredisLibrary();

	//redis 连接池初始化
	simutgw::g_redisPool.Init();

	// 实时行情	
	string strMaxGain;
	string strMinFall;
	string strLive_Cjsl;
	string strLive_Cjje;
	string strLive_SJW1;
	string strLive_SSL1;
	string strLive_BJW1;
	string strLive_BSL1;
	string strLive_Zjjg;
	string strLive_Hqsj;

	// 交易圈值
	//	string strCircle_Cjsl;
	//	string strCircle_Cjje;
	//	string strCircle_Hqsj;

	// 从自己缓存的行情中读取
	vector<std::string> vctCmd;
	std::string strCmdTmp;
	vctCmd.push_back("HMGET");
	vctCmd.push_back(simutgw::g_Key_AStockQuotTGW_TradeVolume);

	// 实时的行情
	strCmdTmp = strZqdm;
	strCmdTmp += "_maxgain";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_minfall";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_TPBZ";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_CJSL";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_CJJE";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_SJW1";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_SSL1";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_BJW1";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_BSL1";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_ZJJG";
	vctCmd.push_back(strCmdTmp);

	//strCmdTmp = strZqdm;
	//strCmdTmp += "_OrigTime";
	//vctCmd.push_back(strCmdTmp);

	// 缓存历史交易圈行情
	/*strCmdTmp = strZqdm;
	strCmdTmp += "_CJSL_";
	strCmdTmp += "01";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_CJJE_";
	strCmdTmp += "01";
	vctCmd.push_back(strCmdTmp);

	strCmdTmp = strZqdm;
	strCmdTmp += "_OrigTime_";
	strCmdTmp += "01";
	vctCmd.push_back(strCmdTmp);*/

	long long llRedisRes = 0;
	std::string strRedisRes;
	std::vector<StgwRedisReply> vectArray;
	size_t stElemSize = 0;

	RedisReply emPcbCallRes = Tgw_RedisHelper::RunCmdArgv(vctCmd, &llRedisRes, &strRedisRes,
		nullptr, &vectArray, &stElemSize);
	if (RedisReply_array != emPcbCallRes)
	{
		EzLog::e("RunCmdArgv Failed !",
			"redis call error! HMGET AStockQuotTGW_TradeVolume reply not array");
	}
	else
	{
		int i = 0;
		strMaxGain = vectArray[i].str;
		ui64MaxGain = strtoull(strMaxGain.c_str(), NULL, 10);

		i = 1;
		strMinFall = vectArray[i].str;
		ui64MinFall = strtoull(strMinFall.c_str(), NULL, 10);

		i = 2;
		strTpbz = vectArray[i].str;

		i = 3;
		strLive_Cjsl = vectArray[i].str;
		ui64Cjsl = strtoull(strLive_Cjsl.c_str(), NULL, 10);

		i = 4;
		strLive_Cjje = vectArray[i].str;
		ui64Cjje = strtoull(strLive_Cjje.c_str(), NULL, 10);

		i = 5;
		strLive_SJW1 = vectArray[i].str;
		ui64SJW1 = strtoull(strLive_SJW1.c_str(), NULL, 10);

		i = 6;
		strLive_SSL1 = vectArray[i].str;
		ui64SSL1 = strtoull(strLive_SSL1.c_str(), NULL, 10);

		i = 7;
		strLive_BJW1 = vectArray[i].str;
		ui64BJW1 = strtoull(strLive_BJW1.c_str(), NULL, 10);

		i = 8;
		strLive_BSL1 = vectArray[i].str;
		ui64BSL1 = strtoull(strLive_BSL1.c_str(), NULL, 10);

		i = 9;
		strLive_Zjjg = vectArray[i].str;
		ui64Zjjg = strtoull(strLive_Zjjg.c_str(), NULL, 10);

		/*i = 10;
		strLive_Hqsj = vectArray[i].str;*/

		//i = 11;
		//strCircle_Cjsl = vectArray[i].str;
		//std::cout << setiosflags(ios::left) << setw(20) << "strCircle_Cjsl" << strCircle_Cjsl << std::endl;

		//i = 12;
		//strCircle_Cjje = vectArray[i].str;
		//std::cout << setiosflags(ios::left) << setw(20) << "strCircle_Cjje" << strCircle_Cjje << std::endl;

		//i = 13;
		//strCircle_Hqsj = vectArray[i].str;
		//std::cout << setiosflags(ios::left) << setw(20) << "strCircle_Hqsj" << strCircle_Hqsj << std::endl;
	}

	simutgw::g_redisPool.Stop();
	Tgw_RedisHelper::FreeHiredisLibrary();
	return emPcbCallRes;
}

// 删除Redis对应的行情
// 输入 ：strZqdm 待删除的行情的证券代码
// 输出 ：0 正常执行删除 操作 ， -1删除操作失败
int DelRedisKeyNum(const std::string strZqdm)
{
	int iRes = 0;
	iRes = simutgw::g_redisPool.SetConnection("127.0.0.1", 6600, 10, 0, "");
	if (0 != iRes)
	{
		EzLog::e("DelRedisKeyNum() ", "simutgw::g_redisPool.SetConnection failed!");
		return -1;
	}

	//加载Redisdll
	iRes = Tgw_RedisHelper::LoadHiredisLibrary();
	if (0 != iRes)
	{
		EzLog::e("DelRedisKeyNum( ) ", "Tgw_RedisHelper::LoadHiredisLibrary failed !");
		return -1;
	}

	//初始化redis连接池
	iRes = simutgw::g_redisPool.Init();
	if (0 != iRes)
	{
		EzLog::e("DelRedisKeyNum( ) ", "simutgw::g_redisPool.Init failed !");
		return -1;
	}

	std::string strCmd = "del tgwhqk_" + strZqdm;
	std::string strRes = "";
	long long llRes = 0;
	RedisReply emPcbCallRes = Tgw_RedisHelper::RunCmd(
		strCmd,
		&llRes,
		&strRes,
		nullptr,
		nullptr,
		nullptr);

	if (RedisReply_integer != emPcbCallRes)
	{
		EzLog::e("DelRedisKeyNum( ) ", "RunCmd Fail !");
		EzLog::e("string Cmd : ", strCmd);
		return -1;
	}

	simutgw::g_redisPool.Stop();
	Tgw_RedisHelper::FreeHiredisLibrary();
	return 0;
}

//计算区间段均价，只有一个行情时
//@parameter:
//	AStockQuot aStockQuot 输入初始行情
//	ui64OutAvePrice 传递引用要输出的int64_t价格
//@output:
//	strAvePrice  区间段均价；
std::string CalcAvePrice(const AStockQuot aStockQuot, uint64_t& ui64OutAvePrice)
{
	std::string strAvePrice;
	//aStockQuot.cjje 成交金额是以厘为单位，除以成交数量余数得到价格（单位是厘），
	ui64OutAvePrice = aStockQuot.cjje / aStockQuot.cjsl;

	Tgw_StringUtil::iLiToStr(ui64OutAvePrice, strAvePrice, 3);
	return strAvePrice;
}

//计算区间段均价，原来的行情后面推送新的行情；
//@parameter：
//	fristAStockQuot 第一个行情结构体；
//	secondAStockQuot 第二个行情结构体；
//@output:
// strAvePrice 两次行情的区间段均价
std::string CalcAvePrice(const AStockQuot firstAStockQuot, const AStockQuot secondAStockQuot, uint64_t& ui64OutAvePrice)
{
	std::string strAvePrice;
	uint64_t ui64Cjsl = secondAStockQuot.cjsl - firstAStockQuot.cjsl;
	uint64_t ui64Cjje = secondAStockQuot.cjje - firstAStockQuot.cjje;
	ui64OutAvePrice = ui64Cjje / ui64Cjsl;

	Tgw_StringUtil::iLiToStr(ui64OutAvePrice, strAvePrice, 3);
	return strAvePrice;
}