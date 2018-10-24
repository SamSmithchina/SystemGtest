#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest\gtest.h"

//测试读最大订单号的函数ReadMaxRecNumFromDB
//   Config.ReadMaxRecNumFromDB
TEST(Config, ReadMaxRecNumFromDB)
{
	int iRes = ReadMaxRecNumFromDB();
	EXPECT_LT(-1, iRes);
}