#ifndef __QUOTATIONBUILD_H__
#define __QUOTATIONBUILD_H__

#include "SHShare/SHShare.h"
#include "MatchMode/MatchMode.h"
#include "tool_string/TimeStringUtil.h"
#include "tool_redis/Tgw_RedisHelper.h"
#include "util/EzLog.h"
#include "quotation/AStockQuotationHelper.h"
#include "simutgw/stgw_config/g_values_inner.h"
//#include "simutgw/stgw_config/g_values_net.h"

#include <stdlib.h>
#include <string>


//����ض�����������Example
//���룺����������AStockQuot &inout_aStockQuot
//�������ֵ������鲢���أ�
void CreateQuotation(AStockQuot &inout_aStockQuot);

//����ض���������NewExample
//���룺����ṹ��AStockQuot & inout_aStockQuot
//�������ֵ�ڸ����鲢����
void CreateNewQuotation(AStockQuot &inout_aStockQuot);

//����������redis
//���룺�����͵�����ṹ�� in_aStockQuot;
//�����0����ʾ������ȷִ�У�  1����ʾ���ִ��� 
int SendQuotToRedis(AStockQuot& in_aStockQuot);


// ɾ��Redis��Ӧ������
// ���� ��strZqdm ��ɾ���������֤ȯ����
// ��� ��0 ����ִ��ɾ�� ���� �� -1ɾ������ʧ��
int DelRedisKeyNum(const std::string& strZqdm);

//��tgwhqk_tv��ȡ��������
//���룺 
//	strZqdm ֤ȯ����	
//����� 
//	uint64_t ui64MaxGain �Ƿ�
//  uint64_t ui64MinFall ����
//  uint64_t ui64Cjje �ɽ����
//  uint64_t ui64Cjsl �ɽ�����
//  uint64_t ui64SJW1 ��1�۸�
//  uint64_t ui64SSL1 ��1����
//  uint64_t ui64BJW1 ��1�۸�
//  uint64_t ui64BSL1 ��һ����
//  uint64_t ui64Zjjg ����ɽ���
//  RedisReply emPcbCallRes ��ѯredis����ķ���״̬
RedisReply GetQuotFromTgwHqkTV(
	const std::string& strZqdm, std::string& strTpbz,
	uint64_t& ui64MaxGain, uint64_t& ui64MinFall, uint64_t& ui64Cjje,
	uint64_t& ui64Cjsl, uint64_t& ui64SJW1, uint64_t& ui64SSL1,
	uint64_t& ui64BJW1, uint64_t& ui64BSL1, uint64_t& ui64Zjjg);

//��������ξ��ۣ�ֻ��һ������ʱ
//@parameter:
//	AStockQuot aStockQuot �����ʼ����
//	ui64OutAvePrice ��������Ҫ�����int64_t�۸�
//@output:
//	strAvePrice  ����ξ��ۣ�
std::string CalcAvePrice(const AStockQuot aStockQuot, uint64_t& ui64OutAvePrice);

//��������ξ��ۣ�ԭ����������������µ����飻
//@parameter��
//	fristAStockQuot ��һ������ṹ�壻
//	secondAStockQuot �ڶ�������ṹ�壻
//@output:
// strAvePrice �������������ξ���
std::string CalcAvePrice(const AStockQuot firstAStockQuot, const AStockQuot secondAStockQuot, uint64_t& ui64OutAvePrice);

#endif