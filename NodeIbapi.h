#ifndef NODEIBAPI_H
#define NODEIBAPI_H

#define IB_USE_STD_STRING

#include <node.h>
#include "IbPosixClient.h"
#include "IbJsonParser.h"
#include "import/EWrapper.h"

using namespace v8;

class NodeIbapi : public node::ObjectWrap{
public:
    static void Init( Handle<Object> exports );

private:
    NodeIbapi();
    ~NodeIbapi();

    static Handle<Value> New( const Arguments& args );
    static Handle<Value> ProcessMsg( const Arguments& args );

    // Getters
    static Handle<Value> GetNextValidId( const Arguments& args );

    // EClientSocket
    static Handle<Value> Connect( const Arguments& args );
    static Handle<Value> Disconnect( const Arguments& args );
    static Handle<Value> IsConnected( const Arguments& args );
    static Handle<Value> ServerVersion( const Arguments& args );
    static Handle<Value> TwsConnectionTime( const Arguments& args );
    static Handle<Value> ReqMktData( const Arguments& args );
    static Handle<Value> CancelMktData( const Arguments& args );
    static Handle<Value> PlaceOrder( const Arguments& args );
    static Handle<Value> CancelOrder( const Arguments& args );
    static Handle<Value> ReqOpenOrders( const Arguments& args ); 
    static Handle<Value> ReqAccountUpdates( const Arguments& args );
    static Handle<Value> ReqExecutions( const Arguments& args );
    static Handle<Value> ReqIds( const Arguments& args );
    static Handle<Value> CheckMessages( const Arguments& args ); 
    static Handle<Value> ReqContractDetails( const Arguments& args );
    static Handle<Value> ReqMktDepth( const Arguments& args );
    static Handle<Value> CancelMktDepth( const Arguments& args );
    static Handle<Value> ReqNewsBulletins( const Arguments& args );
    static Handle<Value> CancelNewsBulletins( const Arguments& args ); 
    static Handle<Value> SetServerLogLevel( const Arguments& args );
    static Handle<Value> ReqAutoOpenOrders( const Arguments& args );
    static Handle<Value> ReqAllOpenOrders( const Arguments& args ); 
    static Handle<Value> ReqManagedAccts( const Arguments& args ); 
    static Handle<Value> RequestFA( const Arguments& args );
    static Handle<Value> ReplaceFA( const Arguments& args );
    static Handle<Value> ReqHistoricalData( const Arguments& args ); 
    static Handle<Value> ExerciseOptions( const Arguments& args );
    static Handle<Value> CancelHistoricalData( const Arguments& args );
    static Handle<Value> ReqRealTimeBars( const Arguments& args );
    static Handle<Value> CancelRealTimeBars( const Arguments& args );
    static Handle<Value> CancelScannerSubscription( const Arguments& args );
    static Handle<Value> ReqScannerParameters( const Arguments& args ); 
    static Handle<Value> ReqScannerSubscription( const Arguments& args );
    static Handle<Value> ReqCurrentTime( const Arguments& args ); 
    static Handle<Value> ReqFundamentalData( const Arguments& args );
    static Handle<Value> CancelFundamentalData( const Arguments& args );
    static Handle<Value> CalculateImpliedVolatility( const Arguments& args );
    static Handle<Value> CalculateOptionPrice( const Arguments& args );
    static Handle<Value> CancelCalculateImpliedVolatility(
        const Arguments& args );
    static Handle<Value> CancelCalculateOptionPrice( const Arguments& args );
    static Handle<Value> ReqGlobalCancel( const Arguments& args ); 
    static Handle<Value> ReqMarketDataType( const Arguments& args );
    static Handle<Value> ReqPositions( const Arguments& args ); 
    static Handle<Value> CancelPositions( const Arguments& args ); 
    static Handle<Value> ReqAccountSummary( const Arguments& args ); 
    static Handle<Value> CancelAccountSummary( const Arguments& args );
    static Handle<Value> VerifyRequest( const Arguments& args );
    static Handle<Value> VerifyMessage( const Arguments& args );
    static Handle<Value> QueryDisplayGroups( const Arguments& args );
    static Handle<Value> SubscribeToGroupEvents( const Arguments& args );
    static Handle<Value> UpdateDisplayGroup( const Arguments& args );
    static Handle<Value> UnsubscribeFromGroupEvents( const Arguments& args );

    // events
    static Handle<Value> GetInboundMsg( const Arguments& args );

private:
    static char *getChar( Local<Value> value, const char *fallback = "" );

    static bool isWrongArgNumber( const Arguments& args, int argNum );
    static bool isWrongType( bool predicateRes, int argId );
    static void convertContractForIb( Handle<Object> ibContract, 
                                      Contract &contract );
    static void convertSubForIb( Handle<Object> scannerSub, 
                                 ScannerSubscription &subscription );
    static void convertOrderForIb( Handle<Object> ibOrder, Order &order );

    IbPosixClient m_client;
    IbJsonParser m_parser;

};

#endif