#ifndef NODEIBAPI_H
#define NODEIBAPI_H

#define IB_USE_STD_STRING

#include <node.h>
#include <node_object_wrap.h>

#include "IbPosixClient.h"
#include "IbJsonParser.h"
#include "import/EWrapper.h"

using namespace v8;

class NodeIbapi : public node::ObjectWrap {
public:
    static void Init( Handle<Object> exports );

private:
    explicit NodeIbapi();
    ~NodeIbapi();

    static void New( const FunctionCallbackInfo<Value>& args );
    static Persistent<Function> constructor;
    static void ProcessMsg( const FunctionCallbackInfo<Value>& args );

    // Getters
    static void GetNextValidId( const FunctionCallbackInfo<Value>& args );

    // EClientSocket
    static void Connect( const FunctionCallbackInfo<Value>& args );
    static void Disconnect( const FunctionCallbackInfo<Value>& args );
    static void IsConnected( const FunctionCallbackInfo<Value>& args );
    static void ServerVersion( const FunctionCallbackInfo<Value>& args );
    static void TwsConnectionTime( const FunctionCallbackInfo<Value>& args );
    static void ReqMktData( const FunctionCallbackInfo<Value>& args );
    static void CancelMktData( const FunctionCallbackInfo<Value>& args );
    static void PlaceOrder( const FunctionCallbackInfo<Value>& args );
    static void CancelOrder( const FunctionCallbackInfo<Value>& args );
    static void ReqOpenOrders( const FunctionCallbackInfo<Value>& args ); 
    static void ReqAccountUpdates( const FunctionCallbackInfo<Value>& args );
    static void ReqExecutions( const FunctionCallbackInfo<Value>& args );
    static void ReqIds( const FunctionCallbackInfo<Value>& args );
    static void CheckMessages( const FunctionCallbackInfo<Value>& args ); 
    static void ReqContractDetails( const FunctionCallbackInfo<Value>& args );
    static void ReqMktDepth( const FunctionCallbackInfo<Value>& args );
    static void CancelMktDepth( const FunctionCallbackInfo<Value>& args );
    static void ReqNewsBulletins( const FunctionCallbackInfo<Value>& args );
    static void CancelNewsBulletins( const FunctionCallbackInfo<Value>& args ); 
    static void SetServerLogLevel( const FunctionCallbackInfo<Value>& args );
    static void ReqAutoOpenOrders( const FunctionCallbackInfo<Value>& args );
    static void ReqAllOpenOrders( const FunctionCallbackInfo<Value>& args ); 
    static void ReqManagedAccts( const FunctionCallbackInfo<Value>& args ); 
    static void RequestFA( const FunctionCallbackInfo<Value>& args );
    static void ReplaceFA( const FunctionCallbackInfo<Value>& args );
    static void ReqHistoricalData( const FunctionCallbackInfo<Value>& args ); 
    static void ExerciseOptions( const FunctionCallbackInfo<Value>& args );
    static void CancelHistoricalData( const FunctionCallbackInfo<Value>& args );
    static void ReqRealTimeBars( const FunctionCallbackInfo<Value>& args );
    static void CancelRealTimeBars( const FunctionCallbackInfo<Value>& args );
    static void CancelScannerSubscription( const FunctionCallbackInfo<Value>& args );
    static void ReqScannerParameters( const FunctionCallbackInfo<Value>& args ); 
    static void ReqScannerSubscription( const FunctionCallbackInfo<Value>& args );
    static void ReqCurrentTime( const FunctionCallbackInfo<Value>& args ); 
    static void ReqFundamentalData( const FunctionCallbackInfo<Value>& args );
    static void CancelFundamentalData( const FunctionCallbackInfo<Value>& args );
    static void CalculateImpliedVolatility( const FunctionCallbackInfo<Value>& args );
    static void CalculateOptionPrice( const FunctionCallbackInfo<Value>& args );
    static void CancelCalculateImpliedVolatility(
        const FunctionCallbackInfo<Value>& args );
    static void CancelCalculateOptionPrice( const FunctionCallbackInfo<Value>& args );
    static void ReqGlobalCancel( const FunctionCallbackInfo<Value>& args ); 
    static void ReqMarketDataType( const FunctionCallbackInfo<Value>& args );
    static void ReqPositions( const FunctionCallbackInfo<Value>& args ); 
    static void CancelPositions( const FunctionCallbackInfo<Value>& args ); 
    static void ReqAccountSummary( const FunctionCallbackInfo<Value>& args ); 
    static void CancelAccountSummary( const FunctionCallbackInfo<Value>& args );
    static void VerifyRequest( const FunctionCallbackInfo<Value>& args );
    static void VerifyMessage( const FunctionCallbackInfo<Value>& args );
    static void QueryDisplayGroups( const FunctionCallbackInfo<Value>& args );
    static void SubscribeToGroupEvents( const FunctionCallbackInfo<Value>& args );
    static void UpdateDisplayGroup( const FunctionCallbackInfo<Value>& args );
    static void UnsubscribeFromGroupEvents( const FunctionCallbackInfo<Value>& args );

    // events
    static void GetInboundMsg( const FunctionCallbackInfo<Value>& args );

private:
    static char *getChar( Local<Value> value, const char *fallback = "" );

    static bool isWrongArgNumber( Isolate *isolate, const FunctionCallbackInfo<Value>& args, int argNum );
    static bool isWrongType( Isolate *isolate, bool predicateRes, int argId );
    static void convertContractForIb( Handle<Object> ibContract, 
                                      Contract &contract );
    static void convertSubForIb( Handle<Object> scannerSub, 
                                 ScannerSubscription &subscription );
    static void convertOrderForIb( Handle<Object> ibOrder, Order &order );

    IbPosixClient m_client;
    IbJsonParser m_parser;

};

#endif
