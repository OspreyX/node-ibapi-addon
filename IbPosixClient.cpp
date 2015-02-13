
#define IB_USE_STD_STRING

#include <iostream>

#include "IbPosixClient.h"

#include "import/EPosixClientSocket.h"
#include "import/EPosixClientSocketPlatform.h"
#include "import/libjson/libjson.h"

#include "import/Contract.h"
#include "import/Order.h"

IbPosixClient::IbPosixClient()
    : m_pClient( new EPosixClientSocket( this ) )
{
}

IbPosixClient::~IbPosixClient() 
{
}

/////////////////// Custom functions for node access //////////////////////////
bool IbPosixClient::connect( const char *host, unsigned int port,
                             int clientId ) {
    bool isConnected = m_pClient->eConnect( host, port, clientId );
    return isConnected;
}

void IbPosixClient::disconnect() const {
    m_pClient->eDisconnect();
}

bool IbPosixClient::isConnected() const {
    return m_pClient->isConnected();
}

void IbPosixClient::processMessages() {
    fd_set readfds, writefds, errorfds;

    struct timeval tval;
    tval.tv_usec = 0;
    tval.tv_sec = 0;
    time_t now = time( NULL );

    if( m_pClient->fd() >= 0 ) {
        FD_ZERO( &readfds );
        errorfds = writefds = readfds;
        FD_SET( m_pClient->fd(), &readfds );

        if( !m_pClient->isOutBufferEmpty() )
            FD_SET( m_pClient->fd(), &writefds );

        FD_CLR( m_pClient->fd(), &errorfds );
        int ret = select( m_pClient->fd() + 1, &readfds, &writefds, &errorfds,
                          &tval );

        if( ret == 0 ) // timeout
            return;
        if( ret < 0 ) // error
            disconnect();
            return;
        if( m_pClient->fd() < 0 )
            return;
        if ( FD_ISSET( m_pClient->fd(), &errorfds ) ) 
            m_pClient->onError();
        if( m_pClient->fd() < 0 )
            return;
        if ( FD_ISSET( m_pClient->fd(), &writefds ) ) 
            m_pClient->onSend();
        if( m_pClient->fd() < 0 )
            return;
        if ( FD_ISSET( m_pClient->fd(), &readfds ) ) 
            m_pClient->onReceive();
    }
}


/////////////////// API EPosixClientSocket method for node access /////////////
int IbPosixClient::serverVersion() {
    return m_pClient->serverVersion();
}
IBString IbPosixClient::TwsConnectionTime() {
    return m_pClient->TwsConnectionTime();
}
void IbPosixClient::reqMktData( TickerId tickerId, const Contract &contract,
                                const IBString &genericTick, bool snapShot,
                                const TagValueListSPtr& mktDataOptions ) {
    m_pClient->reqMktData( tickerId, contract, genericTick, snapShot,
                            mktDataOptions );
}
void IbPosixClient::cancelMktData( TickerId tickerId ) {
    m_pClient->cancelMktData(tickerId);
}
void IbPosixClient::placeOrder( 
    OrderId id, const Contract &contract, const Order &order ) {
    m_pClient->placeOrder( id, contract, order );
}
void IbPosixClient::cancelOrder( OrderId orderId ) {
    m_pClient->cancelOrder( orderId );
}
void IbPosixClient::reqOpenOrders() {
    m_pClient->reqOpenOrders();
}
void IbPosixClient::reqAccountUpdates( bool subscribe, 
                                       const IBString& acctCode ) {
    m_pClient->reqAccountUpdates( subscribe, acctCode );
}
void IbPosixClient::reqExecutions( int reqId, const ExecutionFilter& filter ) {
    m_pClient->reqExecutions( reqId, filter );
}
void IbPosixClient::reqIds( int numIds ) {
    m_pClient->reqIds( numIds );
}
bool IbPosixClient::checkMessages(){
    return m_pClient->checkMessages();
}
void IbPosixClient::reqContractDetails( int reqId, const Contract &contract ) {
    m_pClient->reqContractDetails( reqId, contract );
}
void IbPosixClient::reqMktDepth( TickerId tickerId, const Contract &contract,
                                 int numRows,
                                 const TagValueListSPtr& mktDepthOptions ) {
    m_pClient->reqMktDepth( tickerId, contract, numRows, mktDepthOptions );
}
void IbPosixClient::cancelMktDepth( TickerId tickerId ) {
    m_pClient->cancelMktDepth( tickerId );
}
void IbPosixClient::reqNewsBulletins( bool allMsgs ) {
    m_pClient->reqNewsBulletins( allMsgs );
}
void IbPosixClient::cancelNewsBulletins() {
    m_pClient->cancelNewsBulletins();
}
void IbPosixClient::setServerLogLevel( int level ) {
    m_pClient->setServerLogLevel( level );
}

void IbPosixClient::reqAutoOpenOrders( bool bAutoBind ) {
    m_pClient->reqAutoOpenOrders( bAutoBind );
}
void IbPosixClient::reqAllOpenOrders() {
    m_pClient->reqAllOpenOrders();
}
void IbPosixClient::reqManagedAccts() {
    m_pClient->reqManagedAccts();
}
void IbPosixClient::requestFA( faDataType pFaDataType ) {
    m_pClient->requestFA( pFaDataType );
}
void IbPosixClient::replaceFA( faDataType pFaDataType, const IBString& cxml ) {
    m_pClient->replaceFA( pFaDataType, cxml );
}
void IbPosixClient::reqHistoricalData( TickerId id, const Contract &contract,
                                       const IBString &endDateTime,
                                       const IBString &durationStr,
                                       const IBString &barSizeSetting,
                                       const IBString &whatToShow,
                                       int useRTH, int formatDate,
                                       const TagValueListSPtr& chartOptions ) {
    m_pClient->reqHistoricalData( id, contract, endDateTime, durationStr,
                                  barSizeSetting, whatToShow, useRTH,
                                  formatDate, chartOptions );
}
void IbPosixClient::exerciseOptions( TickerId tickerId,
                                     const Contract &contract,
                                     int exerciseAction, int exerciseQuantity,
                                     const IBString &account, int override ) {
    m_pClient->exerciseOptions( tickerId, contract, exerciseAction,
                                exerciseQuantity, account, override );
}
void IbPosixClient::cancelHistoricalData( TickerId tickerId ) {
    m_pClient->cancelHistoricalData( tickerId );
}
void IbPosixClient::reqRealTimeBars( TickerId id, const Contract &contract,
                                     int barSize, const IBString &whatToShow,
                                     bool useRTH,
                                     const TagValueListSPtr& realTimeBarsOptions
                                      ) {
    m_pClient->reqRealTimeBars( id, contract, barSize, whatToShow, useRTH,
                                realTimeBarsOptions );
}
void IbPosixClient::cancelRealTimeBars( TickerId tickerId ) {
    m_pClient->cancelRealTimeBars( tickerId );
}
void IbPosixClient::cancelScannerSubscription( int tickerId ) {
    m_pClient->cancelScannerSubscription( tickerId );
}
void IbPosixClient::reqScannerParameters() {
    m_pClient->reqScannerParameters();
}
void IbPosixClient::reqScannerSubscription( int tickerId,
                        const ScannerSubscription &subscription,
                        const TagValueListSPtr& scannerSubscriptionOptions ) {
    m_pClient->reqScannerSubscription( tickerId, subscription,
                                        scannerSubscriptionOptions );
}
void IbPosixClient::reqCurrentTime() {
    m_pClient->reqCurrentTime();
}
void IbPosixClient::reqFundamentalData( TickerId reqId,
                                        const Contract &contract,
                                        const IBString &reportType ) {
    m_pClient->reqFundamentalData( reqId, contract, reportType );
}
void IbPosixClient::cancelFundamentalData( TickerId reqId ) {
    m_pClient->cancelFundamentalData( reqId );
}
void IbPosixClient::calculateImpliedVolatility( TickerId reqId,
                                                const Contract &contract,
                                                double optionPrice,
                                                double underPrice ) {
    m_pClient->calculateImpliedVolatility( reqId, contract, optionPrice,
                                           underPrice );
}
void IbPosixClient::calculateOptionPrice( TickerId reqId,
                                          const Contract &contract,
                                          double volatility,
                                          double underPrice ) {
    m_pClient->calculateOptionPrice( reqId, contract, volatility, underPrice );
}
void IbPosixClient::cancelCalculateImpliedVolatility( TickerId reqId ) {
    m_pClient->cancelCalculateImpliedVolatility( reqId );
}
void IbPosixClient::cancelCalculateOptionPrice( TickerId reqId ) {
    m_pClient->cancelCalculateOptionPrice( reqId );
}
void IbPosixClient::reqGlobalCancel() {
    m_pClient->reqGlobalCancel();
}
void IbPosixClient::reqMarketDataType( int marketDataType ) {
    m_pClient->reqMarketDataType( marketDataType );
}
void IbPosixClient::reqPositions() {
    m_pClient->reqPositions();
}
void IbPosixClient::cancelPositions() {
    m_pClient->cancelPositions();
}
void IbPosixClient::reqAccountSummary( int reqId, const IBString& groupName,
                                       const IBString& tags ) {
    m_pClient->reqAccountSummary( reqId, groupName, tags );
}
void IbPosixClient::cancelAccountSummary( int reqId ) {
    m_pClient->cancelAccountSummary( reqId );
}
void IbPosixClient::verifyRequest( const IBString& apiName,
                                   const IBString& apiVersion ) {
    m_pClient->verifyRequest( apiName, apiVersion );
}
void IbPosixClient::verifyMessage( const IBString& apiData) {
    m_pClient->verifyMessage( apiData );
}
void IbPosixClient::queryDisplayGroups( int reqId ) {
    m_pClient->queryDisplayGroups( reqId );
}
void IbPosixClient::subscribeToGroupEvents( int reqId, int groupId ) {
    m_pClient->subscribeToGroupEvents( reqId, groupId );
}
void IbPosixClient::updateDisplayGroup( int reqId,
                                        const IBString& contractInfo ) {
    m_pClient->updateDisplayGroup( reqId, contractInfo );
}
void IbPosixClient::unsubscribeFromGroupEvents( int reqId ) {
    m_pClient->unsubscribeFromGroupEvents( reqId );
}


/////////////////// API EWrapper event methods ////////////////////////////////

void IbPosixClient::tickPrice( TickerId tickerId, TickType field, double price,
                               int canAutoExecute ) {
    JSONNode n(JSON_NODE);
    n.push_back( JSONNode( "messageId", "tickPrice" ) );
    n.push_back( JSONNode( "tickerId", tickerId ) );
    n.push_back( JSONNode( "field", field ) );
    n.push_back( JSONNode( "price", price ) );
    n.push_back( JSONNode( "canAutoExecute", canAutoExecute ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::tickSize( TickerId tickerId, TickType field, int size ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "tickSize" ) );
    n.push_back( JSONNode( "tickerId", tickerId ) );
    n.push_back( JSONNode( "field", field ) );
    n.push_back( JSONNode( "size", size ) );
    this->m_inboundMsgs.push( n );
}
// TODO NOT TESTED
void IbPosixClient::tickOptionComputation( TickerId tickerId,
                                           TickType tickType,
                                           double impliedVol, double delta,
                                           double optPrice, double pvDividend,
                                           double gamma, double vega,
                                           double theta, double undPrice ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "tickOptionComputation" ) );
    n.push_back( JSONNode( "tickerId", tickerId ) );
    n.push_back( JSONNode( "tickType", tickType ) );
    n.push_back( JSONNode( "impliedVol", impliedVol ) );
    n.push_back( JSONNode( "delta", delta ) );
    n.push_back( JSONNode( "optPrice", optPrice ) );
    n.push_back( JSONNode( "pvDividend", pvDividend ) );
    n.push_back( JSONNode( "gamma", gamma ) );
    n.push_back( JSONNode( "vega", vega ) );
    n.push_back( JSONNode( "theta", theta ) );
    n.push_back( JSONNode( "undPrice", undPrice ) );
    this->m_inboundMsgs.push( n );
}
// TODO NOT TESTED
void IbPosixClient::tickGeneric( TickerId tickerId, TickType tickType,
                                 double value ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "tickGeneric" ) );
    n.push_back( JSONNode( "tickerId", tickerId ) );
    n.push_back( JSONNode( "tickType", tickType ) );
    n.push_back( JSONNode( "valueDouble", value ) );
    this->m_inboundMsgs.push( n );
}
// TODO NOT TESTED
void IbPosixClient::tickString( TickerId tickerId, TickType tickType,
                                const IBString& value ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "tickString" ) );
    n.push_back( JSONNode( "tickerId", tickerId ) );
    n.push_back( JSONNode( "tickType", tickType ) );
    n.push_back( JSONNode( "valueString", value ) );
    this->m_inboundMsgs.push( n );
}
// TODO NOT TESTED
void IbPosixClient::tickEFP( TickerId tickerId, TickType tickType,
                             double basisPoints,
                             const IBString& formattedBasisPoints,
                             double totalDividends, int holdDays,
                             const IBString& futureExpiry,
                             double dividendImpact,
                             double dividendsToExpiry ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "tickEFP" ) );
    n.push_back( JSONNode( "tickerId", tickerId ) );
    n.push_back( JSONNode( "tickType", tickType ) );
    n.push_back( JSONNode( "basisPoints", basisPoints ) );
    n.push_back( JSONNode( "formattedBasisPoints", formattedBasisPoints ) );
    n.push_back( JSONNode( "totalDividends", totalDividends ) );
    n.push_back( JSONNode( "holdDays", holdDays ) );
    n.push_back( JSONNode( "futureExpiry", futureExpiry ) );
    n.push_back( JSONNode( "dividendImpact", dividendImpact ) );
    n.push_back( JSONNode( "dividendsToExpiry", dividendsToExpiry ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::orderStatus( OrderId orderId, const IBString &status,
                                 int filled, int remaining,
                                 double avgFillPrice, int permId, int parentId,
                                 double lastFillPrice, int clientId,
                                 const IBString& whyHeld ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "orderStatus" ) );
    n.push_back( JSONNode( "orderId", orderId ) );
    n.push_back( JSONNode( "status", status ) );
    n.push_back( JSONNode( "filled", filled ) );
    n.push_back( JSONNode( "remaining", remaining ) );
    n.push_back( JSONNode( "avgFillPrice", avgFillPrice ) );
    n.push_back( JSONNode( "permId", permId ) );
    n.push_back( JSONNode( "parentId", parentId ) );
    n.push_back( JSONNode( "lastFillPrice", lastFillPrice ) );
    n.push_back( JSONNode( "clientId", clientId ) );
    n.push_back( JSONNode( "whyHeld", whyHeld ) );
    this->m_inboundMsgs.push( n );
}
// No idea how to handle contract and order
void IbPosixClient::openOrder( OrderId orderId, const Contract& contract,
                               const Order& order,
                               const OrderState& ostate ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "openOrder" ) );
    n.push_back( JSONNode( "orderId", orderId ) );
    n.push_back( jsonifyContract( "contract", contract ) );
    n.push_back( jsonifyOrder( order ) );
    n.push_back( jsonifyOrderState( ostate ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::openOrderEnd() {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "openOrderEnd" ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::winError( const IBString &str, int lastError ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "winError" ) );
    n.push_back( JSONNode( "str", str ) );
    n.push_back( JSONNode( "lastError", lastError ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::connectionClosed() {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "connectionClosed" ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::updateAccountValue( const IBString& key,
                                        const IBString& val,
                                        const IBString& currency,
                                        const IBString& accountName ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "updateAccountValue" ) );
    n.push_back( JSONNode( "key", key ) );
    n.push_back( JSONNode( "val", val ) );
    n.push_back( JSONNode( "currency", currency ) );
    n.push_back( JSONNode( "accountName", accountName ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::updatePortfolio( const Contract& contract, int position,
                                     double marketPrice, double marketValue,
                                     double averageCost, double unrealizedPNL,
                                     double realizedPNL,
                                     const IBString& accountName ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "updatePortfolio" ) );
    n.push_back( jsonifyContract( "contract", contract ) );
    n.push_back( JSONNode( "position", position ) );
    n.push_back( JSONNode( "marketPrice", marketPrice ) );
    n.push_back( JSONNode( "marketValue", marketValue ) );
    n.push_back( JSONNode( "averageCost", averageCost ) );
    n.push_back( JSONNode( "unrealizedPNL", unrealizedPNL ) );
    n.push_back( JSONNode( "realizedPNL", realizedPNL ) );
    n.push_back( JSONNode( "accountName", accountName ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::updateAccountTime( const IBString& timeStamp ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "updateAccountTime" ) );
    n.push_back( JSONNode( "timeStamp", timeStamp ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::accountDownloadEnd( const IBString& accountName ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "accountDownloadEnd" ) );
    n.push_back( JSONNode( "accountName", accountName ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::nextValidId( OrderId orderId ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "nextValidId" ) );
    n.push_back( JSONNode( "orderId", orderId ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::contractDetails( int reqId,
                                     const ContractDetails& contractDetails ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "contractDetails" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( jsonifyContractDetails( contractDetails ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::bondContractDetails( int reqId,
                                      const ContractDetails& contractDetails ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "bondContractDetails" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( jsonifyContractDetails( contractDetails ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::contractDetailsEnd( int reqId ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "contractDetailsEnd" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::execDetails( int reqId, const Contract& contract,
                                 const Execution& execution ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "execDetails" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( jsonifyContract( "contract", contract ) );
    n.push_back( jsonifyExecution( execution ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::execDetailsEnd( int reqId ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "execDetailsEnd" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::error( const int id, const int errorCode,
                           const IBString errorString ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "error" ) );
    n.push_back( JSONNode( "id", id ) );
    n.push_back( JSONNode( "errorCode", errorCode ) );
    n.push_back( JSONNode( "errorString", errorString ) );
    this->m_inboundMsgs.push( n );
    if ( id == -1 && errorCode == 1100 )
        disconnect();
}
void IbPosixClient::updateMktDepth( TickerId id, int position, int operation,
                                    int side, double price, int size ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "updateMktDepth" ) );
    n.push_back( JSONNode( "id", id ) );
    n.push_back( JSONNode( "position", position ) );
    n.push_back( JSONNode( "operation", operation ) );
    n.push_back( JSONNode( "side", side ) );
    n.push_back( JSONNode( "price", price ) );
    n.push_back( JSONNode( "size", size ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::updateMktDepthL2( TickerId id, int position,
                                      IBString marketMaker, int operation,
                                      int side, double price, int size ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "updateMktDepthL2" ) );
    n.push_back( JSONNode( "id", id ) );
    n.push_back( JSONNode( "position", position ) );
    n.push_back( JSONNode( "marketMaker", marketMaker ) );
    n.push_back( JSONNode( "operation", operation ) );
    n.push_back( JSONNode( "side", side ) );
    n.push_back( JSONNode( "price", price ) );
    n.push_back( JSONNode( "size", size ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::updateNewsBulletin( int msgId, int msgType,
                                        const IBString& newsMessage,
                                        const IBString& originExch ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "updateNewsBulletin" ) );
    n.push_back( JSONNode( "msgId", msgId ) );
    n.push_back( JSONNode( "msgType", msgType ) );
    n.push_back( JSONNode( "newsMessage", newsMessage ) );
    n.push_back( JSONNode( "originExch", originExch ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::managedAccounts( const IBString& accountsList ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "managedAccounts" ) );
    n.push_back( JSONNode( "accountsList", accountsList ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::receiveFA( faDataType pFaDataType, const IBString& cxml ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "receiveFA" ) );
    n.push_back( JSONNode( "pFaDataType", pFaDataType ) );
    n.push_back( JSONNode( "cxml", cxml ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::historicalData( TickerId reqId, const IBString& date,
                                    double open, double high, double low,
                                    double close, int volume, int barCount,
                                    double WAP, int hasGaps ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "historicalData" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( JSONNode( "date", date ) );
    n.push_back( JSONNode( "open", open ) );
    n.push_back( JSONNode( "high", high ) );
    n.push_back( JSONNode( "low", low ) );
    n.push_back( JSONNode( "close", close ) );
    n.push_back( JSONNode( "volume", volume ) );
    n.push_back( JSONNode( "barCount", barCount ) );
    n.push_back( JSONNode( "WAP", WAP ) );
    n.push_back( JSONNode( "hasGaps", hasGaps ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::scannerParameters( const IBString &xml ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "scannerParameters" ) );
    n.push_back( JSONNode( "xml", xml ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::scannerData( int reqId, int rank,
                                 const ContractDetails &contractDetails,
                                 const IBString &distance,
                                 const IBString &benchmark,
                                 const IBString &projection,
                                 const IBString &legsStr ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "scannerData" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( JSONNode( "rank", rank ) );
    n.push_back( jsonifyContractDetails( contractDetails ) );
    n.push_back( JSONNode( "distance", distance ) );
    n.push_back( JSONNode( "benchmark", benchmark ) );
    n.push_back( JSONNode( "projection", projection ) );
    n.push_back( JSONNode( "legsStr", legsStr ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::scannerDataEnd( int reqId ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "scannerDataEnd" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::realtimeBar( TickerId reqId, long itime, double open,
                                 double high, double low, double close,
                                 long volume, double wap, int count ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "realtimeBar" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( JSONNode( "timeLong", itime ) );
    n.push_back( JSONNode( "open", open ) );
    n.push_back( JSONNode( "high", high ) );
    n.push_back( JSONNode( "low", low ) );
    n.push_back( JSONNode( "close", close ) );
    n.push_back( JSONNode( "volume", volume ) );
    n.push_back( JSONNode( "wap", wap ) );
    n.push_back( JSONNode( "count", count ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::currentTime( long time ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "currentTime" ) );
    n.push_back( JSONNode( "timeLong", time ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::fundamentalData( TickerId reqId, const IBString& data ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "fundamentalData" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( JSONNode( "data", data ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::deltaNeutralValidation( int reqId,
                                            const UnderComp& underComp ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "deltaNeutralValidation" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( jsonifyUnderComp( underComp ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::tickSnapshotEnd( int reqId ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "tickSnapshotEnd" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::marketDataType( TickerId reqId, int marketDataType ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "marketDataType" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( JSONNode( "marketDataType", marketDataType ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::commissionReport( 
        const CommissionReport& commissionReport ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "commissionReport" ) );
    n.push_back( jsonifyCommissionReport( commissionReport ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::position( const IBString& account,
                              const Contract& contract, int position,
                              double avgCost ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "position" ) );
    n.push_back( JSONNode( "account", account ) );
    n.push_back( jsonifyContract( "contract", contract ) );
    n.push_back( JSONNode( "position", position ) );
    n.push_back( JSONNode( "avgCost", avgCost ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::positionEnd() {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "positionEnd" ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::accountSummary( int reqId, const IBString& account,
                                    const IBString& tag, const IBString& value,
                                    const IBString& curency ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "accountSummary" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( JSONNode( "account", account ) );
    n.push_back( JSONNode( "tag", tag ) );
    n.push_back( JSONNode( "valueString", value ) );
    n.push_back( JSONNode( "curency", curency ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::accountSummaryEnd( int reqId ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "accountSummaryEnd" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::verifyMessageAPI( const IBString& apiData ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "verifyMessageAPI" ) );
    n.push_back( JSONNode( "apiData", apiData ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::verifyCompleted( bool isSuccessful,
                                     const IBString& errorText ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "verifyCompleted" ) );
    n.push_back( JSONNode( "isSuccessful", isSuccessful ) );
    n.push_back( JSONNode( "errorText", errorText ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::displayGroupList( int reqId, const IBString& groups ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "displayGroupList" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( JSONNode( "groups", groups ) );
    this->m_inboundMsgs.push( n );
}
void IbPosixClient::displayGroupUpdated( int reqId,
                                         const IBString& contractInfo ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "displayGroupUpdated" ) );
    n.push_back( JSONNode( "reqId", reqId ) );
    n.push_back( JSONNode( "contractInfo", contractInfo ) );
    this->m_inboundMsgs.push( n );
}

JSONNode IbPosixClient::getInboundMsg() {
    JSONNode popped;
    if ( !this->m_inboundMsgs.empty() ) {
        popped = this->m_inboundMsgs.front();
        this->m_inboundMsgs.pop();
        return popped;
    }
    return popped;
}


JSONNode IbPosixClient::jsonifyContract( std::string name,
                                         const Contract& contract ) {
    JSONNode c( JSON_NODE );
    c.set_name( name );
    c.push_back( JSONNode( "conId", contract.conId ) );
    c.push_back( JSONNode( "symbol", contract.symbol ) );
    c.push_back( JSONNode( "secType", contract.secType ) );
    c.push_back( JSONNode( "expiry", contract.expiry ) );
    c.push_back( JSONNode( "strike", contract.strike ) );
    c.push_back( JSONNode( "right", contract.right ) );
    c.push_back( JSONNode( "multiplier", contract.multiplier ) );
    c.push_back( JSONNode( "exchange", contract.exchange ) );
    c.push_back( JSONNode( "primaryExchange", contract.primaryExchange ) );
    c.push_back( JSONNode( "currency", contract.currency ) );
    c.push_back( JSONNode( "localSymbol", contract.localSymbol ) );
    c.push_back( JSONNode( "tradingClass", contract.tradingClass ) );
    c.push_back( JSONNode( "includeExpired", contract.includeExpired ) );
    c.push_back( JSONNode( "secIdType", contract.secIdType ) );
    c.push_back( JSONNode( "secId", contract.secId ) );
    return c;
}
JSONNode IbPosixClient::jsonifyUnderComp( const UnderComp& underComp ) {
    JSONNode u( JSON_NODE );
    u.set_name( "underComp" );
    u.push_back( JSONNode( "conId", underComp.conId ) );
    u.push_back( JSONNode( "delta", underComp.delta ) );
    u.push_back( JSONNode( "price", underComp.price ) );
    return u;
}
JSONNode IbPosixClient::jsonifyContractDetails( const ContractDetails& cd ) {
    JSONNode c( JSON_NODE );
    c.set_name( "contractDetails" );

    c.push_back( jsonifyContract( "summary", cd.summary ) );
    c.push_back( JSONNode( "marketName", cd.marketName ) );
    c.push_back( JSONNode( "minTick", cd.minTick ) );
    c.push_back( JSONNode( "orderTypes", cd.orderTypes ) );
    c.push_back( JSONNode( "validExchanges", cd.validExchanges ) );
    c.push_back( JSONNode( "priceMagnifier", cd.priceMagnifier ) );
    c.push_back( JSONNode( "underConId", cd.underConId ) );
    c.push_back( JSONNode( "longName", cd.longName ) );
    c.push_back( JSONNode( "contractMonth", cd.contractMonth ) );
    c.push_back( JSONNode( "industry", cd.industry ) );
    c.push_back( JSONNode( "category", cd.category ) );
    c.push_back( JSONNode( "subcategory", cd.subcategory ) );
    c.push_back( JSONNode( "timeZoneId", cd.timeZoneId ) );
    c.push_back( JSONNode( "tradingHours", cd.tradingHours ) );
    c.push_back( JSONNode( "liquidHours", cd.liquidHours ) );
    c.push_back( JSONNode( "evRule", cd.evRule ) );
    c.push_back( JSONNode( "evMultiplier", cd.evMultiplier ) );

    // BOND values
    c.push_back( JSONNode( "cusip", cd.cusip ) );
    c.push_back( JSONNode( "ratings", cd.ratings ) );
    c.push_back( JSONNode( "descAppend", cd.descAppend ) );
    c.push_back( JSONNode( "bondType", cd.bondType ) );
    c.push_back( JSONNode( "couponType", cd.couponType ) );
    c.push_back( JSONNode( "callable", cd.callable ) );
    c.push_back( JSONNode( "putable", cd.putable ) );
    c.push_back( JSONNode( "coupon", cd.coupon ) );
    c.push_back( JSONNode( "convertible", cd.convertible ) );
    c.push_back( JSONNode( "maturity", cd.maturity ) );
    c.push_back( JSONNode( "issueDate", cd.issueDate ) );
    c.push_back( JSONNode( "nextOptionDate", cd.nextOptionDate ) );
    c.push_back( JSONNode( "nextOptionType", cd.nextOptionType ) );
    c.push_back( JSONNode( "nextOptionPartial", cd.nextOptionPartial ) );
    c.push_back( JSONNode( "notes", cd.notes ) );

    return c;
}
JSONNode IbPosixClient::jsonifyOrder( const Order& order ) {
    JSONNode o( JSON_NODE );
    o.set_name( "order" );

    // order identifier
    o.push_back( JSONNode( "orderId", order.orderId ) );
    o.push_back( JSONNode( "clientId", order.clientId ) );
    o.push_back( JSONNode( "permId", order.permId ) );

    // main order fields
    o.push_back( JSONNode( "action", order.action ) );
    o.push_back( JSONNode( "totalQuantity", order.totalQuantity ) );
    o.push_back( JSONNode( "orderType", order.orderType ) );
    o.push_back( JSONNode( "lmtPrice", order.lmtPrice ) );
    o.push_back( JSONNode( "auxPrice", order.auxPrice ) );

    // extended order fields
    o.push_back( JSONNode( "tif", order.tif ) );
    o.push_back( JSONNode( "activeStartTime", order.activeStartTime ) );
    o.push_back( JSONNode( "activeStopTime", order.activeStopTime ) );
    o.push_back( JSONNode( "ocaGroup", order.ocaGroup ) );
    o.push_back( JSONNode( "ocaType", order.ocaType ) );
    o.push_back( JSONNode( "orderRef", order.orderRef ) );
    o.push_back( JSONNode( "transmit", order.transmit ) );
    o.push_back( JSONNode( "parentId", order.parentId ) );
    o.push_back( JSONNode( "blockOrder", order.blockOrder ) );
    o.push_back( JSONNode( "sweepToFill", order.sweepToFill ) );
    o.push_back( JSONNode( "displaySize", order.displaySize ) );
    o.push_back( JSONNode( "triggerMethod", order.triggerMethod ) );
    o.push_back( JSONNode( "outsideRth", order.outsideRth ) );
    o.push_back( JSONNode( "hidden", order.hidden ) );
    o.push_back( JSONNode( "goodAfterTime", order.goodAfterTime ) );
    o.push_back( JSONNode( "goodTillDate", order.goodTillDate ) );
    o.push_back( JSONNode( "rule80A", order.rule80A ) );
    o.push_back( JSONNode( "allOrNone", order.allOrNone ) );
    o.push_back( JSONNode( "minQty", order.minQty ) );
    o.push_back( JSONNode( "percentOffset", order.percentOffset ) );
    o.push_back( JSONNode( "overridePercentageConstraints",
                           order.overridePercentageConstraints ) );
    o.push_back( JSONNode( "trailStopPrice", order.trailStopPrice ) );
    o.push_back( JSONNode( "trailingPercent", order.trailingPercent ) );

    // financial advisors only
    o.push_back( JSONNode( "faGroup", order.faGroup ) );
    o.push_back( JSONNode( "faProfile", order.faProfile ) );
    o.push_back( JSONNode( "faMethod", order.faMethod ) );
    o.push_back( JSONNode( "faPercentage", order.faPercentage ) );

    // institutional (ie non-cleared) only
    o.push_back( JSONNode( "openClose", order.openClose ) );
    o.push_back( JSONNode( "origin", order.origin ) );
    o.push_back( JSONNode( "shortSaleSlot", order.shortSaleSlot ) );
    o.push_back( JSONNode( "designatedLocation", order.designatedLocation ) );
    o.push_back( JSONNode( "exemptCode", order.exemptCode ) );

    // SMART routing only
    o.push_back( JSONNode( "discretionaryAmt", order.discretionaryAmt ) );
    o.push_back( JSONNode( "eTradeOnly", order.eTradeOnly ) );
    o.push_back( JSONNode( "firmQuoteOnly", order.firmQuoteOnly ) );
    o.push_back( JSONNode( "nbboPriceCap", order.nbboPriceCap ) );
    o.push_back( JSONNode( "optOutSmartRouting", order.optOutSmartRouting ) );

    // BOX exchange orders only
    o.push_back( JSONNode( "auctionStrategy", order.auctionStrategy ) );
    o.push_back( JSONNode( "startingPrice", order.startingPrice ) );
    o.push_back( JSONNode( "stockRefPrice", order.stockRefPrice ) );
    o.push_back( JSONNode( "delta", order.delta ) );

    // pegged to stock and VOL orders only
    o.push_back( JSONNode( "stockRangeLower", order.stockRangeLower ) );
    o.push_back( JSONNode( "stockRangeUpper", order.stockRangeUpper ) );

    // VOLATILITY ORDERS ONLY
    o.push_back( JSONNode( "volatility", order.volatility ) );
    o.push_back( JSONNode( "volatilityType", order.volatilityType ) );
    o.push_back( JSONNode( "deltaNeutralOrderType",
                           order.deltaNeutralOrderType ) );
    o.push_back( JSONNode( "deltaNeutralAuxPrice",
                           order.deltaNeutralAuxPrice ) );
    o.push_back( JSONNode( "deltaNeutralConId", order.deltaNeutralConId ) );
    o.push_back( JSONNode( "deltaNeutralSettlingFirm",
                           order.deltaNeutralSettlingFirm ) );
    o.push_back( JSONNode( "deltaNeutralClearingAccount",
                           order.deltaNeutralClearingAccount ) );
    o.push_back( JSONNode( "deltaNeutralClearingIntent",
                           order.deltaNeutralClearingIntent ) );
    o.push_back( JSONNode( "deltaNeutralOpenClose",
                           order.deltaNeutralOpenClose ) );
    o.push_back( JSONNode( "deltaNeutralShortSale",
                           order.deltaNeutralShortSale ) );
    o.push_back( JSONNode( "deltaNeutralShortSaleSlot",
                           order.deltaNeutralShortSaleSlot ) );
    o.push_back( JSONNode( "deltaNeutralDesignatedLocation",
                           order.deltaNeutralDesignatedLocation ) );
    o.push_back( JSONNode( "continuousUpdate", order.continuousUpdate ) );
    o.push_back( JSONNode( "referencePriceType", order.referencePriceType ) );

    // COMBO ORDERS ONLY
    o.push_back( JSONNode( "basisPoints", order.basisPoints ) );
    o.push_back( JSONNode( "basisPointsType", order.basisPointsType ) );

    // SCALE ORDERS ONLY
    o.push_back( JSONNode( "scaleInitLevelSize", order.scaleInitLevelSize ) );
    o.push_back( JSONNode( "scaleSubsLevelSize", order.scaleSubsLevelSize ) );
    o.push_back( JSONNode( "scalePriceIncrement", order.scalePriceIncrement ) );
    o.push_back( JSONNode( "scalePriceAdjustValue",
                           order.scalePriceAdjustValue ) );
    o.push_back( JSONNode( "scalePriceAdjustInterval",
                           order.scalePriceAdjustInterval ) );
    o.push_back( JSONNode( "scaleProfitOffset", order.scaleProfitOffset ) );
    o.push_back( JSONNode( "scaleAutoReset", order.scaleAutoReset ) );
    o.push_back( JSONNode( "scaleInitPosition", order.scaleInitPosition ) );
    o.push_back( JSONNode( "scaleInitFillQty", order.scaleInitFillQty ) );
    o.push_back( JSONNode( "scaleRandomPercent", order.scaleRandomPercent ) );
    o.push_back( JSONNode( "scaleTable", order.scaleTable ) );

    // HEDGE ORDERS
    o.push_back( JSONNode( "hedgeType", order.hedgeType ) );
    o.push_back( JSONNode( "hedgeParam", order.hedgeParam ) );

    // Clearing info
    o.push_back( JSONNode( "account", order.account ) );
    o.push_back( JSONNode( "settlingFirm", order.settlingFirm ) );
    o.push_back( JSONNode( "clearingAccount", order.clearingAccount ) );
    o.push_back( JSONNode( "clearingIntent", order.clearingIntent ) );

    return o;
}
JSONNode IbPosixClient::jsonifyOrderState(const OrderState& ostate) {
    JSONNode o( JSON_NODE );
    o.set_name( "orderState" );
    o.push_back( JSONNode( "status", ostate.status ) );
    o.push_back( JSONNode( "initMargin", ostate.initMargin ) );
    o.push_back( JSONNode( "maintMargin", ostate.maintMargin ) );
    o.push_back( JSONNode( "equityWithLoan", ostate.equityWithLoan ) );
    o.push_back( JSONNode( "commission", ostate.commission ) );
    o.push_back( JSONNode( "minCommission", ostate.minCommission ) );
    o.push_back( JSONNode( "maxCommission", ostate.maxCommission ) );
    o.push_back( JSONNode( "commissionCurrency", ostate.commissionCurrency ) );
    o.push_back( JSONNode( "warningText", ostate.warningText ) );
    return o;
}
JSONNode IbPosixClient::jsonifyExecution(const Execution& execution) {
    JSONNode e( JSON_NODE );
    e.set_name( "execution" );
    e.push_back( JSONNode( "execId", execution.execId ) );
    e.push_back( JSONNode( "timeString", execution.time ) );
    e.push_back( JSONNode( "acctNumber", execution.acctNumber ) );
    e.push_back( JSONNode( "exchange", execution.exchange ) );
    e.push_back( JSONNode( "side", execution.side ) );
    e.push_back( JSONNode( "shares", execution.shares ) );
    e.push_back( JSONNode( "price", execution.price ) );
    e.push_back( JSONNode( "permId", execution.permId ) );
    e.push_back( JSONNode( "clientId", execution.clientId ) );
    e.push_back( JSONNode( "orderId", execution.orderId ) );
    e.push_back( JSONNode( "liquidation", execution.liquidation ) );
    e.push_back( JSONNode( "cumQty", execution.cumQty ) );
    e.push_back( JSONNode( "avgPrice", execution.avgPrice ) );
    e.push_back( JSONNode( "orderRef", execution.orderRef ) );
    e.push_back( JSONNode( "evRule", execution.evRule ) );
    e.push_back( JSONNode( "evMultiplier", execution.evMultiplier ) );
    return e;
}
JSONNode IbPosixClient::jsonifyCommissionReport(const CommissionReport& cr) {
    JSONNode c( JSON_NODE );
    c.set_name( "commissionReport" );
    c.push_back( JSONNode( "execId", cr.execId ) );
    c.push_back( JSONNode( "commission", cr.commission ) );
    c.push_back( JSONNode( "currency", cr.currency ) );
    c.push_back( JSONNode( "realizedPNL", cr.realizedPNL ) );
    c.push_back( JSONNode( "yield", cr.yield ) );
    c.push_back( JSONNode( "yieldRedemptionDate", cr.yieldRedemptionDate ) );
    return c;
}
