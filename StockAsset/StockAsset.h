#ifndef __SOCKETASSET_H__
#define __SOCKETASSET_H__

#include "tool_mysql/MySqlCnnC602.h"
#include <iostream>
#include <string>

struct StockAsset
{
	//
	// Member
	//
	//用string 类型代替4、8、12、15、20 bigint 类型
									//数据库原始定义
	std::string id;					// bigint(20) unsigned NOT NULL AUTO_INCREMENT,
	std::string account_id;			// varchar(12) COLLATE gbk_bin NOT NULL COMMENT '证券账户',
	std::string stock_id;			// varchar(8) COLLATE gbk_bin NOT NULL COMMENT '证券代码',
	std::string trade_market;		// tinyint(4) DEFAULT NULL COMMENT '0--深圳A股，1--上海A股',
	std::string stock_balance;		// bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '证券持有数量，股份余额',
	std::string stock_auction_purchase_balance;	// bigint(20) NOT NULL DEFAULT '0' COMMENT '竞价买入量',
	std::string stock_staple_purchase_balance;	// bigint(20) NOT NULL DEFAULT '0' COMMENT '大宗买入量',
	std::string stock_etf_redemption_balance;	// bigint(20) NOT NULL DEFAULT '0' COMMENT 'etf赎回量，可竞价卖出',
	std::string stock_available;	// bigint(20) NOT NULL DEFAULT '0' COMMENT '证券持有可用余额，可用于申购etf份额和可竞价卖出',
	std::string stock_last_balance;	//bigint(20) NOT NULL DEFAULT '0' COMMENT '上次余额',
	std::string	is_close;			//tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否销户，1--已销户',
	//	std::string oper_time;		//timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT '发生时间',
	//	std::string operator;		// varchar(15) COLLATE gbk_bin DEFAULT NULL COMMENT '操作人',
	//UNIQUE KEY `acc_stock` (`account_id`,`stock_id`)

	//
	// Function
	//
	

	//初始化函数
	int Init(string strAcc, string strStock);

};


#endif