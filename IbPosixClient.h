#ifndef ibposixclient_h__INCLUDED
#define ibposixclient_h__INCLUDED

#include "import/EWrapper.h"
#include <cstring>
#include <memory>

#include <queue>
#include <utility>
#include "EWrapperStore.h"

class EPosixClientSocket;
struct ExecutionFilter;
struct ScannerSubscription;

class IbPosixClient : public EWrapper {
public:
    IbPosixClient();
    ~IbPosixClient();

    void processMessages();

// accessors
    std::string getCurrentTime();
    TickPriceData getTickPrice();
    TickSizeData getTickSize();
    TickOptionComputationData getTickOptionComputation();
    TickGenericData getTickGeneric();
    TickStringData getTickString();
    TickEFPData getTickEFP();
    TickSnapshotEndData getTickSnapshotEnd();
    MarketDataTypeData getMarketDataType();

    OrderStatusData getOrderStatus();
    OpenOrderData getOpenOrder();
    RealtimeBarData getRealtimeBar();

    WinErrorData getWinError();
    ErrorData getError();

    UpdateAccountValueData getUpdateAccountValue();
    UpdatePortfolioData getUpdatePortfolio();
    UpdateAccountTimeData getUpdateAccountTime();

    OrderId getNextValidId();

// EClientSocket

    bool connect( const char * host, unsigned int port, int clientId = 0 );
    void disconnect() const;
    bool isConnected() const;

    void reqMktData( TickerId id, const Contract &contract, 
                     const IBString &genericTicks, bool snapshot );
    void cancelMktData( TickerId id );
    void placeOrder( OrderId id, const Contract &contract, 
                     const Order &order );
    void cancelOrder( OrderId id ) ;
    void reqOpenOrders();
    void reqAccountUpdates( bool subscribe, const IBString& acctCode );
    void reqExecutions( int reqId, const ExecutionFilter& filter );
    void reqIds( int numIds );
    bool checkMessages();
    void reqContractDetails( int reqId, const Contract &contract );
    void reqMktDepth( TickerId tickerId, const Contract &contract, 
                      int numRows );
    void cancelMktDepth( TickerId tickerId );
    void reqNewsBulletins( bool allMsgs );
    void cancelNewsBulletins();
    void setServerLogLevel( int level );
    void reqAutoOpenOrders( bool bAutoBind );
    void reqAllOpenOrders();
    void reqManagedAccts();
    void requestFA( faDataType pFaDataType );
    void replaceFA( faDataType pFaDataType, const IBString& cxml );
    void reqHistoricalData( TickerId id, const Contract &contract,
                            const IBString &endDateTime, 
                            const IBString &durationStr, 
                            const IBString & barSizeSetting, 
                            const IBString &whatToShow, int useRTH, 
                            int formatDate );
    void exerciseOptions( TickerId tickerId, const Contract &contract, 
                          int exerciseAction, int exerciseQuantity, 
                          const IBString &account, int override );
    void cancelHistoricalData( TickerId tickerId );
    void reqRealTimeBars( TickerId id, const Contract &contract, int barSize, 
                          const IBString &whatToShow, bool useRTH );
    void cancelRealTimeBars( TickerId tickerId );
    void cancelScannerSubscription( int tickerId );
    void reqScannerParameters();
    void reqScannerSubscription( int tickerId, 
                                 const ScannerSubscription &subscription );
    void reqCurrentTime();
    void reqFundamentalData( TickerId reqId, const Contract&, 
                             const IBString &reportType );
    void cancelFundamentalData( TickerId reqId );
    void calculateImpliedVolatility( TickerId reqId, const Contract &contract, 
                                     double optionPrice, double underPrice );
    void calculateOptionPrice( TickerId reqId, const Contract &contract, 
                               double volatility, double underPrice );
    void cancelCalculateImpliedVolatility( TickerId reqId );
    void cancelCalculateOptionPrice( TickerId reqId );
    void reqGlobalCancel();
    void reqMarketDataType( int marketDataType );
    void reqPositions();
    void cancelPositions();
    void reqAccountSummary( int reqId, const IBString& groupName, 
                            const IBString& tags );
    void cancelAccountSummary( int reqId );


// EWrapper
public:
    void tickPrice( TickerId tickerId, TickType field, double price, 
                    int canAutoExecute );
    void tickSize( TickerId tickerId, TickType field, int size );
    void tickOptionComputation( TickerId tickerId, TickType tickType, 
                                double impliedVol, double delta, 
                                double optPrice, double pvDividend, 
                                double gamma, double vega, double theta, 
                                double undPrice );
    void tickGeneric( TickerId tickerId, TickType tickType, double value );
    void tickString( TickerId tickerId, TickType tickType, 
                     const IBString& value );
    void tickEFP( TickerId tickerId, TickType tickType, double basisPoints, 
                  const IBString& formattedBasisPoints, double totalDividends, 
                  int holdDays, const IBString& futureExpiry, 
                  double dividendImpact, double dividendsToExpiry );
    void orderStatus( OrderId orderId, const IBString &status, int filled,
        int remaining, double avgFillPrice, int permId, int parentId,
        double lastFillPrice, int clientId, const IBString& whyHeld );
    void openOrder( OrderId orderId, const Contract&, const Order&, 
                    const OrderState& );
    void openOrderEnd();
    void winError( const IBString &str, int lastError );
    void connectionClosed();
    void updateAccountValue( const IBString& key, const IBString& val,
                             const IBString& currency, 
                             const IBString& accountName );
    void updatePortfolio( const Contract& contract, int position,
                          double marketPrice, double marketValue, 
                          double averageCost, double unrealizedPNL, 
                          double realizedPNL, const IBString& accountName );
    void updateAccountTime( const IBString& timeStamp );
    void accountDownloadEnd( const IBString& accountName );
    void nextValidId( OrderId orderId );
    void contractDetails( int reqId, const ContractDetails& contractDetails );
    void bondContractDetails( int reqId, 
                              const ContractDetails& contractDetails );
    void contractDetailsEnd( int reqId );
    void execDetails( int reqId, const Contract& contract, 
                      const Execution& execution );
    void execDetailsEnd( int reqId );
    void error( const int id, const int errorCode, const IBString errorString );
    void updateMktDepth( TickerId id, int position, int operation, int side,
                         double price, int size );
    void updateMktDepthL2( TickerId id, int position, IBString marketMaker, 
                           int operation, int side, double price, int size );
    void updateNewsBulletin( int msgId, int msgType, 
                             const IBString& newsMessage, 
                             const IBString& originExch );
    void managedAccounts( const IBString& accountsList );
    void receiveFA( faDataType pFaDataType, const IBString& cxml );
    void historicalData( TickerId reqId, const IBString& date, double open, 
                         double high, double low, double close, int volume, 
                         int barCount, double WAP, int hasGaps );
    void scannerParameters( const IBString &xml );
    void scannerData( int reqId, int rank, 
                      const ContractDetails &contractDetails, 
                      const IBString &distance, const IBString &benchmark, 
                      const IBString &projection, const IBString &legsStr );
    void scannerDataEnd( int reqId );
    void realtimeBar( TickerId reqId, long time, double open, double high, 
                      double low, double close, long volume, double wap, 
                      int count );
    void currentTime( long time );
    void fundamentalData( TickerId reqId, const IBString& data );
    void deltaNeutralValidation( int reqId, const UnderComp& underComp );
    void tickSnapshotEnd( int reqId );
    void marketDataType( TickerId reqId, int marketDataType );
    void commissionReport( const CommissionReport& commissionReport );
    void position( const IBString& account, const Contract& contract, 
                   int position, double avgCost );
    void positionEnd();
    void accountSummary( int reqId, const IBString& account, 
                         const IBString& tag, const IBString& value, 
                         const IBString& curency );
    void accountSummaryEnd( int reqId );

private:
    std::auto_ptr<EPosixClientSocket> m_pClient;

///// node.js accessible
    std::string m_currentTime;
    std::queue< TickPriceData > m_tickPrices;
    std::queue< TickSizeData > m_tickSizes;
    std::queue< TickOptionComputationData > m_tickOptionComps;
    std::queue< TickGenericData > m_tickGenerics;
    std::queue< TickStringData > m_tickStrings;
    std::queue< TickEFPData > m_tickEFPs;
    std::queue< TickSnapshotEndData > m_tickSnapshotEnds;
    std::queue< MarketDataTypeData > m_marketDataTypes;

    std::queue< OrderStatusData > m_orderStatuses;
    std::queue< OpenOrderData > m_openOrders;
    std::queue< RealtimeBarData > m_realtimeBars;

    std::queue< WinErrorData > m_winErrors;
    std::queue< ErrorData > m_errors;

    std::queue< UpdateAccountValueData > m_updateAccountValues;
    std::queue< UpdatePortfolioData > m_updatePortfolios;
    std::queue< UpdateAccountTimeData > m_updateAccountTimes;
    std::queue< OrderId > m_validId;

};

#endif
