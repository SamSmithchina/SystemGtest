﻿#include "SystemGtestConfigs/configs.h"
#include "quotation/AStockQuotationHelper.h"
#include "gtest/gtest.h"
#include "AShareCheckorder/AShareCheckOrder.h"
#include "AShareCheckOrder/QuotationBuild.h"
#include "quotation/MarketInfoHelper.h"
#include <vector>

//		AStockQuot.Example
TEST(AStockQuot, Example)
{
	AStockQuotationHelper myQuotHelper;
	AStockQuot aStockQuot;
	int iRes = 0;

	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600317";
	aStockQuot.zqmc = "营口港";
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));
	iRes = myQuotHelper.Validate(aStockQuot);
	EXPECT_EQ(0, iRes) << iRes;	std::string  strTemp = "";

	EzLog::i("CreateQuotationExample();\n", aStockQuot.OriginStr);
	EzLog::i(__FUNCTION__, "\n\n");
	if (0 != iRes)
	{
		EzLog::e(__FUNCTION__, "\n");
	}
}

//	AStockQuot.NewExample
TEST(AStockQuot, NewExample)
{
	AStockQuot aStockQuot;
	AStockQuotationHelper myQuotHelper;
	int iRes = 0;

	CreateQuotationNewExample(aStockQuot);
	aStockQuot.zqdm = "600317";
	aStockQuot.zqmc = "营口港";
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));
	iRes = myQuotHelper.Validate(aStockQuot);
	EXPECT_EQ(0, iRes);

	EzLog::i("NewExample;\n", aStockQuot.OriginStr);
	if(iRes != 0)
	{
		EzLog::e(__FUNCTION__, "\n");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n");
	}
}

// AStockQuot.HMGET		
TEST(AStockQuot, HMGET)
{
	//推送行情
	AStockQuot aStockQuot;				//行情CJSL = 100000
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600323";
	aStockQuot.zqmc = "瀚蓝环境";
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot));

	//股票信息
	std::string strZqdm = aStockQuot.zqdm;
	uint64_t ui64MaxGain = 0;
	uint64_t ui64MinFall = 0;
	string strTpbz;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Cjsl = 0;
	uint64_t ui64SJW1 = 0;
	uint64_t ui64SSL1 = 0;
	uint64_t ui64BJW1 = 0;
	uint64_t ui64BSL1 = 0;
	uint64_t ui64Zjjg = 0;

	int n = 0;

	Sleep(g_iTimeOut * 30);		//前面推送到了redis，必须等待stgw_quotation处理行情，之后在读取行情tgwhqk_tv；
	RedisReply enumRedisReplyRes = GetQuotFromTgwHqkTV(
		strZqdm, strTpbz,
		ui64MaxGain, ui64MinFall, ui64Cjje,
		ui64Cjsl, ui64SJW1, ui64SSL1,
		ui64BJW1, ui64BSL1, ui64Zjjg);

	EXPECT_EQ(RedisReply_array, enumRedisReplyRes) << n++;

	//输出行情结果；
	if (RedisReply_array == enumRedisReplyRes)		// 值存在
	{

		EzLog::i("股票代码 ", strZqdm);
		EzLog::i("交易圈 ", "01");
		EzLog::Out("strMaxGain ", (trivial::severity_level)2, (long)ui64MaxGain); //EzLog::Out() 没有重载输出uint64_t的函数，
		EzLog::Out("strMinFall ", (trivial::severity_level)2, (long)ui64MinFall);
		EzLog::Out("strTpbz ", (trivial::severity_level)2, strTpbz);
		EzLog::Out("strLive_Cjsl ", (trivial::severity_level)2, (long)ui64Cjsl);
		EzLog::Out("strLive_Cjje ", (trivial::severity_level)2, (long)ui64Cjje);
		EzLog::Out("strLive_SJW1 ", (trivial::severity_level)2, (long)ui64SJW1);
		EzLog::Out("strLive_SSL1 ", (trivial::severity_level)2, (long)ui64SSL1);
		EzLog::Out("strLive_BJW1 ", (trivial::severity_level)2, (long)ui64BJW1);
		EzLog::Out("strLive_BSL1 ", (trivial::severity_level)2, (long)ui64BSL1);
		EzLog::Out("strLive_Zjjg ", (trivial::severity_level)2, (long)ui64Zjjg);

		//EzLog::Out( "strLive_Hqsj" , (trivial::severity_level)2 ,strLive_Hqsj );
		//EzLog::Out("strCircle_Cjsl" , (trivial::severity_level)2 ,strCircle_Cjsl );
		//EzLog::Out( "strCircle_Cjje" , (trivial::severity_level)2 ,strCircle_Cjje );
		//EzLog::Out( "strCircle_Hqsj" , (trivial::severity_level)2 ,strCircle_Hqsj );

		EXPECT_EQ(aStockQuot.maxgain, ui64MaxGain) << n++;
		EXPECT_EQ(aStockQuot.minfall, ui64MinFall) << n++;
		EXPECT_EQ(aStockQuot.TPBZ, strTpbz) << n++;
		EXPECT_EQ(aStockQuot.cjsl, ui64Cjsl) << n++;
		EXPECT_EQ(aStockQuot.cjje, ui64Cjje) << n++;
		EXPECT_EQ(aStockQuot.SJW1, ui64SJW1) << n++;
		EXPECT_EQ(aStockQuot.SSL1, ui64SSL1 * 1000) << n++;
		EXPECT_EQ(aStockQuot.BJW1, ui64BJW1) << n++;
		EXPECT_EQ(aStockQuot.BSL1, ui64BSL1 * 1000) << n++;
		EXPECT_EQ(aStockQuot.zjjg, ui64Zjjg) << n++;
	}
	if (0 != n)
	{
		EzLog::e(__FUNCTION__, "\n");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n");
	}
}


//插入行情后清空行情在插入相同股份的行情（成交数量减小），测试此时的订单是否正常交易
//  AStockQuot.DelKeyNum
TEST(AStockQuot, DelKeyNum)
{
	int iRes = 0;
	iRes = simutgw::g_redisPool.SetConnection("127.0.0.1", 6600, 10, 0, "");
	ASSERT_EQ(0, iRes);

	int n = 0;
	//新建一条行情
	AStockQuot aStockQuot;
	CreateQuotationExample(aStockQuot);
	aStockQuot.zqdm = "600000";
	aStockQuot.zqmc = "测试Redis行情";
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot)) << n++;
	EzLog::i("插入一条行情，股票代码", aStockQuot.zqdm);

	//删除该行情，
	Sleep(g_iTimeOut * 20);
	iRes = DelRedisKeyNum(aStockQuot.zqdm);
	ASSERT_EQ(0, iRes);
	if (0 != iRes)
	{
		EzLog::e("删除该行情", "失败 ！");
	}
	else
	{
		EzLog::e("删除该行情", "成功 ！");
	}

	Sleep(g_iTimeOut * 10);
	//插入新的行情，新行情成交数量cjsl比之前小
	aStockQuot.cjsl -= 50000;
	aStockQuot.cjje -= 50000000;
	aStockQuot.SSL1 -= 50000000;		//SSL BSL 都放大1000；方便后面用Litostr（）转换
	aStockQuot.BSL1 -= 50000000;
	ASSERT_EQ(0, SendQuotToRedis(aStockQuot)) << n++;
	EzLog::i("插入第二条行情，股票代码", aStockQuot.zqdm);

	//获取最新行情容量
	std::string strZqdm = aStockQuot.zqdm;
	uint64_t ui64MaxGain = 0;
	uint64_t ui64MinFall = 0;
	string strTpbz;
	uint64_t ui64Cjje = 0;
	uint64_t ui64Cjsl = 0;
	uint64_t ui64SJW1 = 0;
	uint64_t ui64SSL1 = 0;
	uint64_t ui64BJW1 = 0;
	uint64_t ui64BSL1 = 0;
	uint64_t ui64Zjjg = 0;

	Sleep(g_iTimeOut * 40);		//前面推送到了redis，必须等待stgw_quotation处理行情，之后在读取行情tgwhqk_tv；
	RedisReply enumRedisReplyRes = GetQuotFromTgwHqkTV(
		strZqdm, strTpbz,
		ui64MaxGain, ui64MinFall, ui64Cjje,
		ui64Cjsl, ui64SJW1, ui64SSL1,
		ui64BJW1, ui64BSL1, ui64Zjjg);

	EXPECT_EQ(RedisReply_array, enumRedisReplyRes) << n++;
	//输出行情结果；
	if (RedisReply_array == enumRedisReplyRes)		// 值存在
	{
		EXPECT_EQ(aStockQuot.maxgain, ui64MaxGain) << n++;
		EXPECT_EQ(aStockQuot.minfall, ui64MinFall) << n++;
		EXPECT_EQ(aStockQuot.TPBZ, strTpbz) << n++;
		EXPECT_EQ(aStockQuot.cjsl, ui64Cjsl) << n++;
		EXPECT_EQ(aStockQuot.cjje, ui64Cjje) << n++;
		EXPECT_EQ(aStockQuot.SJW1, ui64SJW1) << n++;
		EXPECT_EQ(aStockQuot.SSL1, ui64SSL1 * 1000) << n++;
		EXPECT_EQ(aStockQuot.BJW1, ui64BJW1) << n++;
		EXPECT_EQ(aStockQuot.BSL1, ui64BSL1 * 1000) << n++;
		EXPECT_EQ(aStockQuot.zjjg, ui64Zjjg) << n++;
	}
	if (0 != n)
	{
		EzLog::e(__FUNCTION__, "未正常获取行情容量！");
	}
	else
	{
		EzLog::i(__FUNCTION__, "\n");
	}
}