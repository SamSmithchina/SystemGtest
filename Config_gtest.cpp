#include "SystemGtestConfigs/configs.h"
#include "util/EzLog.h"
#include "gtest\gtest.h"

//���Զ���󶩵��ŵĺ���ReadMaxRecNumFromDB
//   Config.ReadMaxRecNumFromDB
TEST(Config, ReadMaxRecNumFromDB)
{
	int iRes = ReadMaxRecNumFromDB();
	EXPECT_LT(-1, iRes);
}