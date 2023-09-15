#include<MyTrSpi.h>
void MyTrSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo && pRspInfo->ErrorID == 0) {
		std::cout << "Authenticate success" << std::endl;
		CThostFtdcReqUserLoginField ReqUserLogin;
		memset(&ReqUserLogin, 0, sizeof(ReqUserLogin));
		strcpy(ReqUserLogin.BrokerID, gBrokerID);
		strcpy(ReqUserLogin.UserID, userID);
		strcpy(ReqUserLogin.Password, Password);
		static int requestID = 0;
		int rt = MyTrApi->ReqUserLogin(&ReqUserLogin, requestID);
		if (!rt)
			std::cout << "send login request" << std::endl;
		else
			std::cerr << "login request failed" << std::endl;
	}
	else
		std::cout << "Authenticate failed" << " error:" << (pRspInfo ? pRspInfo->ErrorMsg : "null pointer") << std::endl;
}
void MyTrSpi::Auth() {
	static int requestID = 0;
	CThostFtdcReqAuthenticateField ReqAuthenticate;
	memset(&ReqAuthenticate, 0, sizeof(ReqAuthenticate));
	strcpy(ReqAuthenticate.AppID, appid);
	strcpy(ReqAuthenticate.AuthCode, authcode);
	strcpy(ReqAuthenticate.BrokerID, gBrokerID);
	strcpy(ReqAuthenticate.UserID, userID);
	int rtau = MyTrApi->ReqAuthenticate(&ReqAuthenticate, requestID);
	if (!rtau)
		std::cout << "authenticate send" << std::endl;
	else
		std::cout << "authenticate request failed" << std::endl;
}
void MyTrSpi::OnFrontConnected() {
	std::cout << "Connect success" << std::endl;
	Auth();
	//std::thread auththread(Auth);
	//auththread.join();
	//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}
void MyTrSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo && pRspInfo->ErrorID == 0) {
		std::cout << "Login success" << std::endl;
		std::cout << "TradingDay： " << pRspUserLogin->TradingDay << std::endl;
		std::cout << "LoginTime： " << pRspUserLogin->LoginTime << std::endl;
		std::cout << "BrokerID： " << pRspUserLogin->BrokerID << std::endl;
		std::cout << "UserID： " << pRspUserLogin->UserID << std::endl;
		trade_front_id = pRspUserLogin->FrontID;
		session_id = pRspUserLogin->SessionID;
		strcpy(order_ref, pRspUserLogin->MaxOrderRef);
		islogin = true;
		//confirm the result
		CThostFtdcSettlementInfoConfirmField settlementConfirmReq;
		memset(&settlementConfirmReq, 0, sizeof(settlementConfirmReq));
		strcpy(settlementConfirmReq.BrokerID, gBrokerID);
		strcpy(settlementConfirmReq.InvestorID, userID);
		static int requestID = 0;
		int rt = MyTrApi->ReqSettlementInfoConfirm(&settlementConfirmReq, requestID);
		if (!rt)
			std::cout << "confirm result send" << std::endl;
		else
			std::cerr << "confirm result failed" << std::endl;
	}
	else
		std::cout << "Login failed" << " error" << (pRspInfo ? pRspInfo->ErrorMsg : "null pointer") << std::endl;
}
void MyTrSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo && pRspInfo->ErrorID == 0) {
		std::cout << "SettlementInfoConfirm success" << std::endl;
		std::cout << "ConfirmDate： " << pSettlementInfoConfirm->ConfirmDate << std::endl;
		std::cout << "ConfirmTime： " << pSettlementInfoConfirm->ConfirmTime << std::endl;
		CThostFtdcQryInstrumentField instrumentReq;
		memset(&instrumentReq, 0, sizeof(instrumentReq));
		strcpy(instrumentReq.InstrumentID, g_pTradeInstrumentID);
		static int requestID = 0;
		std::cout << "Strat Trading" << std::endl;
		if (islogin)
			Trade(g_pTradeInstrumentID);
		//int rt = MyTrApi->ReqQryInstrument(&instrumentReq, requestID);
		//if (!rt)
		//	std::cout << "query intrument send" << std::endl;
		//else
		//	std::cerr << "query for intrument failed" << std::endl;
	}
	else
		std::cout << "SettlementInfoConfirm failed" << " error:" << (pRspInfo ? pRspInfo->ErrorMsg : "null pointer") << std::endl;
}
void MyTrSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo && pRspInfo->ErrorID == 0) {
		std::cout << "Instrument query success" << std::endl;
		std::cout << "ExchangeID： " << pInstrument->ExchangeID << std::endl;
		std::cout << "InstrumentID： " << pInstrument->InstrumentID << std::endl;
		std::cout << "ExchangeInstID： " << pInstrument->ExchangeInstID << std::endl;
		std::cout << "StrikePrice： " << pInstrument->StrikePrice << std::endl;
		std::cout << "EndDelivDate： " << pInstrument->EndDelivDate << std::endl;
		std::cout << "IsTrading： " << pInstrument->IsTrading << std::endl;
		// query for trading account
		CThostFtdcQryTradingAccountField tradingAccountReq;
		memset(&tradingAccountReq, 0, sizeof(tradingAccountReq));
		strcpy(tradingAccountReq.BrokerID, gBrokerID);
		strcpy(tradingAccountReq.InvestorID, userID);
		static int requestID = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(700));
		int rt = MyTrApi->ReqQryTradingAccount(&tradingAccountReq, requestID);
		if (!rt)
			std::cout << "trading account query send" << std::endl;
		else
			std::cerr << "trading account query failed" << std::endl;
	}
	else
		std::cout << "Instrument query failed" << " error:" << (pRspInfo ? pRspInfo->ErrorMsg : "null pointer") << std::endl;
}
void MyTrSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo && pRspInfo->ErrorID == 0) {
		std::cout << "TradingAccout Qry success" << std::endl;
		std::cout << "AccountID： " << pTradingAccount->AccountID << std::endl;
		std::cout << "Available： " << pTradingAccount->Available << std::endl;
		std::cout << "WithdrawQuota： " << pTradingAccount->WithdrawQuota << std::endl;
		std::cout << "CurrMargin: " << pTradingAccount->CurrMargin << std::endl;
		std::cout << "CloseProfit： " << pTradingAccount->CloseProfit << std::endl;
		// query for InvestorPosition
		CThostFtdcQryInvestorPositionField postionReq;
		memset(&postionReq, 0, sizeof(postionReq));
		strcpy(postionReq.BrokerID, gBrokerID);
		strcpy(postionReq.InvestorID, userID);
		strcpy(postionReq.InstrumentID, g_pTradeInstrumentID);
		static int requestID = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(700)); // it takes few seconds
		int rt = MyTrApi->ReqQryInvestorPosition(&postionReq, requestID);
		if (!rt)
			std::cout << "InvestorPosition query success" << std::endl;
		else
			std::cerr << "InvestorPosition query failed" << std::endl;
	}
	else
		std::cout << "Trading query failed" << " error:" << (pRspInfo ? pRspInfo->ErrorMsg : "null pointer") << std::endl;
}
void MyTrSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo && pRspInfo->ErrorID == 0) {
		std::cout << "InvestorPosition query success" << std::endl;
		if (pInvestorPosition)
		{
			std::cout << "InstrumentID： " << pInvestorPosition->InstrumentID << std::endl;
			std::cout << "OpenAmount： " << pInvestorPosition->OpenAmount << std::endl;
			std::cout << "OpenVolume： " << pInvestorPosition->OpenVolume << std::endl;
			std::cout << "PosiDirection： " << pInvestorPosition->PosiDirection << std::endl;
			std::cout << "UseMargin：" << pInvestorPosition->UseMargin << std::endl;
		}
		else
			std::cout << "----->this InvestorPosition empty" << std::endl;

		std::cout << "=====start Trading=====" << std::endl;
		// 报单请求
		//if (islogin)
		//	Trade(g_pTradeInstrumentID);

	}
	else {
		std::cout << "InvestorPosition query failed" << " error:" << (pRspInfo ? pRspInfo->ErrorMsg : "null pointer") << std::endl;
	}
}
void MyTrSpi::Trade(TThostFtdcInstrumentIDType instrumentid) {
	CThostFtdcInputOrderField order;
	memset(&order, 0, sizeof(order));
	strcpy(order.BrokerID, gBrokerID);
	strcpy(order.InvestorID, userID);
	strcpy(order.InstrumentID, instrumentid);
	strcpy(order.OrderRef, order_ref);
	order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	order.LimitPrice = 1900.00;
	order.TimeCondition = THOST_FTDC_TC_GFD;
	order.Direction = THOST_FTDC_D_Buy;
	order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	order.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	order.VolumeTotalOriginal = 1;
	order.ContingentCondition = THOST_FTDC_CC_Immediately;
	order.VolumeCondition = THOST_FTDC_VC_AV;
	order.MinVolume = 1;
	order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	order.IsAutoSuspend = 0;
	order.UserForceClose = 0;
	static int requestid = 0;
	int rt = MyTrApi->ReqOrderInsert(&order, ++requestid);
	if (!rt)
		std::cout << "Order send success" << std::endl;
	else
		std::cerr << "Order send failed" << std::endl;
}
void MyTrSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo && pRspInfo->ErrorID == 0) {
		std::cout << "Order Insert success" << std::endl;
		std::cout << "InstrumentID： " << pInputOrder->InstrumentID << std::endl;
		std::cout << "LimitPrice： " << pInputOrder->LimitPrice << std::endl;
		std::cout << "VolumeTotalOriginal： " << pInputOrder->VolumeTotalOriginal << std::endl;
		std::cout << "Direction： " << pInputOrder->Direction << std::endl;

	}
	else
		std::cout << "Order Insert failed" << " error:" << (pRspInfo ? pRspInfo->ErrorMsg : "null pointer") << std::endl;
}
void MyTrSpi::OnRtnOrder(CThostFtdcOrderField* pOrder) {
	char str[10];
	sprintf(str, "%d", pOrder->OrderSubmitStatus);
	int orderState = atoi(str) - '0';
	std::cout << "Recive Order return" << std::endl;
	if ((pOrder->FrontID == trade_front_id) &&
		(pOrder->SessionID == session_id) &&
		(strcmp(pOrder->OrderRef, order_ref) == 0)) {
		if ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded)) {
			std::cout << "Order already committed" << std::endl;
		}
	}
}
void MyTrSpi::OnRtnTrade(CThostFtdcTradeField* pTrade) {
	std::cout << "TradeOrder Done" << std::endl;
	std::cout << "TradeTime： " << pTrade->TradeTime << std::endl;
	std::cout << "InstrumentID： " << pTrade->InstrumentID << std::endl;
	std::cout << "Price： " << pTrade->Price << std::endl;
	std::cout << "Volume： " << pTrade->Volume << std::endl;
	std::cout << "Direction： " << pTrade->Direction << std::endl;
}