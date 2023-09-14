#ifndef __MYSPI__
#define __MYSPI__
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>
#include <ThostFtdcMdApi.h>
extern TThostFtdcBrokerIDType gBrokerID;                       // 模拟经纪商代码
extern TThostFtdcInvestorIDType userID ;                         // 投资者账户名
extern TThostFtdcPasswordType Password;                     // 投资者密码
extern CThostFtdcMdApi *MyMdApi;                           // 行情指针
extern char MdFrontAddr[] ;               // 模拟行情前置地址
extern char *ArrayofConracts[];// 行情合约代码列表                                            // 行情合约订阅数量
//unordered_map<string, TickToKlineHelper> g_KlineHash;              // 不同合约的k线存储表
class MymdSpi : public CThostFtdcMdSpi
{
public:
    void OnFrontConnected();
    void OnFrontDisconnected(int nReason);
    void OnHeartBeatWarning(int nTimeLapse);
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    // void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    // void OnRspQryMulticastInstrument(CThostFtdcMulticastInstrumentField *pMulticastInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    // void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    // void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    // void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    // void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    // void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);
};
#endif