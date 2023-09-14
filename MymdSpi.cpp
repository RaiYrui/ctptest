#include <MymdSpi.h>
#include<ThostFtdcMdApi.h>
void MymdSpi::OnFrontConnected()
{
    std::cout << "connecting" << std::endl;
    CThostFtdcReqUserLoginField ReqUserLogin;
    memset(&ReqUserLogin,0,sizeof(ReqUserLogin));
    strcpy(ReqUserLogin.BrokerID,gBrokerID);
    strcpy(ReqUserLogin.UserID,userID);
    strcpy(ReqUserLogin.Password,Password);
    int request = 0;
    if(!MyMdApi){
        std::cout<<"error:null pointer"<<std::endl;
        exit(0);
    }
    int ret = MyMdApi->ReqUserLogin(&ReqUserLogin,request);
    if(ret==0){
        std::cout<<"connect success"<<std::endl;
    }
    else if(ret == -1){
        std::cout<<"connect Failed"<<std::endl;
    }
    else if(ret == -2){
        std::cout<<"connect faild"<<std::endl;
    }
    else{
        std::cout<<" connect  faild"<<std::endl;
    }
    
}
void MymdSpi::OnFrontDisconnected(int nReason){
    std::cout<<"disconnect"<<std::endl<<" with"<<nReason<<std::endl;
}
void MymdSpi::OnHeartBeatWarning(int nTimeLapse){
            std::cout<<"heartbeatwarning"<<nTimeLapse<<std::endl;
}
void MymdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
        if(pRspInfo&&pRspInfo->ErrorID==0){
            std::cout<<"Loginsuccess"<<std::endl;
            std::cout<<
            "LoginTime:"<<pRspUserLogin->LoginTime<<std::endl<<
            "UserID:"<<pRspUserLogin->UserID<<std::endl<<
            "BrokerID:"<<pRspUserLogin->BrokerID<<std::endl<<
            "SessionID:"<<pRspUserLogin->SessionID<<std::endl<<
            "FrontID:"<<pRspUserLogin->FrontID<<std::endl<<
            "MaxOrderRef:"<<pRspUserLogin->MaxOrderRef<<std::endl;
            int ret = MyMdApi->SubscribeMarketData(ArrayofConracts,4);
        }
        else{
            std::cout<<"Login faild"<<pRspInfo->ErrorID<<":"<<pRspInfo->ErrorMsg<<std::endl;
            
        }

}
void MymdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
                    if(pRspInfo&&pRspInfo->ErrorID==0){
                        std::cout<<"successfully sub"<<std::endl;
                    }
                    else{
                        std::cout<<"sub error"<<pRspInfo->ErrorID<<":"<<pRspInfo->ErrorMsg<<std::endl;
                    }
}
void MymdSpi:: OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData){
                    if(pDepthMarketData){
                        std::cout<<"marketdata"<<std::endl;
                        std::cout<<
                        "TradingDay:"<<pDepthMarketData->TradingDay<<std::endl<<
                        "ExchangeID:"<<pDepthMarketData->ExchangeID<<std::endl<<
                        "InstrumentID:"<<pDepthMarketData->InstrumentID<<std::endl<<
                        "LastPrice:"<<pDepthMarketData->LastPrice<<std::endl<<
                        "Volume:"<<pDepthMarketData->Volume<<std::endl;
                    }else{
                        std::cout<<"marketdata error"<<std::endl;
                    }
}