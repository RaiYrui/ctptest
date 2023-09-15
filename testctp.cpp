#include <iostream>
#include <unordered_map>
#include <vector>
#include <MymdSpi.h>
#include<MyTrSpi.h>
TThostFtdcBrokerIDType gBrokerID = "0001"; // 模拟经纪商代码
TThostFtdcInvestorIDType userID = "888601";// 账户名
TThostFtdcPasswordType Password = "Zerg2017";// 密码
TThostFtdcAppIDType appid = "JY_ZERG_V1.1.0";
TThostFtdcAuthCodeType authcode = "1FUVUZPQ02WYBVN6";

CThostFtdcMdApi* MyMdApi = nullptr;                           // 行情指针
//char MdFrontAddr[] = "tcp://180.168.146.187:10010";// 模拟行情前置地址
char MdFrontAddr[] = "tcp://115.238.45.57:51213";
char TrFrontAddr[] = "tcp://115.238.45.57:51205";
CThostFtdcTraderApi* MyTrApi = nullptr;
char* ArrayofConracts[] = { (char*)"a2311",(char*)"cu2310",(char*)"AP310",(char*)"IC2309" };  // 行情合约代码列表
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "AP310";
//unordered_map<string, TickToKlineHelper> g_KlineHash;// 不同合约的k线存储表
int main()
{
	std::cout << "Hello World!!" << std::endl;
	std::cout << "账号" << std::endl;
	std::cout << ArrayofConracts[0] << std::endl;
	printf("%s", userID);
	std::cout << "密码" << std::endl;
	printf("%s", Password);
	std::cout << "初始化行情" << std::endl;
	MyMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
	CThostFtdcMdSpi* mdspi = new MymdSpi();
	MyMdApi->RegisterSpi(mdspi);
	MyMdApi->RegisterFront(MdFrontAddr);
	MyMdApi->Init();
	std::cout << "init trading" << std::endl;
	MyTrApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	CThostFtdcTraderSpi* trspi = new MyTrSpi();
	MyTrApi->RegisterSpi(trspi);
	MyTrApi->SubscribePublicTopic(THOST_TERT_RESTART);
	MyTrApi->SubscribePrivateTopic(THOST_TERT_RESTART);
	MyTrApi->RegisterFront(TrFrontAddr);
	MyTrApi->Init();
	MyMdApi->Join();
	delete mdspi;
	MyMdApi->Release();

	MyTrApi->Join();
	delete trspi;
	MyTrApi->Release();
	std::cout << "end" << std::endl;
	system("pause");
	return 0;
}