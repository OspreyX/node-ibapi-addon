#include <cstring>
#include <sstream>
#include <node.h>
#include "NodeIbapi.h"
#include "import/Contract.h"
#include "import/Order.h"
#include "import/Execution.h"
#include "import/ScannerSubscription.h"
#include "import/libjson/libjson.h"

using namespace v8;

Persistent<Function> NodeIbapi::constructor;

NodeIbapi::NodeIbapi() {
};
NodeIbapi::~NodeIbapi() {
};

void NodeIbapi::Init( Handle<Object> exports ) {
    Isolate* isolate = Isolate::GetCurrent();
    // Prep constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New( isolate, New );
    tpl->SetClassName( String::NewFromUtf8(isolate, "NodeIbapi" ) );
    tpl->InstanceTemplate()->SetInternalFieldCount( 1 );

    // prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "processMsg", ProcessMsg );
    /// getters
    NODE_SET_PROTOTYPE_METHOD(tpl, "getInboundMsg", GetInboundMsg );

    /// EClientSocket
    NODE_SET_PROTOTYPE_METHOD(tpl, "connect", Connect );
    NODE_SET_PROTOTYPE_METHOD(tpl, "disconnect", Disconnect );
    NODE_SET_PROTOTYPE_METHOD(tpl, "isConnected", IsConnected );
    NODE_SET_PROTOTYPE_METHOD(tpl, "serverVersion", ServerVersion );
    NODE_SET_PROTOTYPE_METHOD(tpl, "twsConnectionTime", TwsConnectionTime );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqMktData", ReqMktData );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelMktData", CancelMktData );
    NODE_SET_PROTOTYPE_METHOD(tpl, "placeOrder", PlaceOrder );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelOrder", CancelOrder );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqOpenOrders", ReqOpenOrders );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqAccountUpdates", ReqAccountUpdates );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqExecutions", ReqExecutions );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqIds", ReqIds );
    NODE_SET_PROTOTYPE_METHOD(tpl, "checkMessages", CheckMessages );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqContractDetails", ReqContractDetails );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqMktDepth", ReqMktDepth );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelMktDepth", CancelMktDepth );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqNewsBulletins", ReqNewsBulletins );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelNewsBulletins", CancelNewsBulletins );
    NODE_SET_PROTOTYPE_METHOD(tpl, "setServerLogLevel", SetServerLogLevel );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqAutoOpenOrders", ReqAutoOpenOrders );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqAllOpenOrders", ReqAllOpenOrders );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqManagedAccts", ReqManagedAccts );
    NODE_SET_PROTOTYPE_METHOD(tpl, "requestFA", RequestFA );
    NODE_SET_PROTOTYPE_METHOD(tpl, "replaceFA", ReplaceFA );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqHistoricalData", ReqHistoricalData );
    NODE_SET_PROTOTYPE_METHOD(tpl, "exerciseOptions", ExerciseOptions );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelHistoricalData", CancelHistoricalData );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqRealtimeBars", ReqRealTimeBars );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelRealTimeBars", CancelRealTimeBars );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelScannerSubscription", CancelScannerSubscription );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqScannerParameters", ReqScannerParameters );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqScannerSubscription", ReqScannerSubscription );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqCurrentTime", ReqCurrentTime );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqFundamentalData", ReqFundamentalData );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelFundamentalData", CancelFundamentalData );
    NODE_SET_PROTOTYPE_METHOD(tpl, "calculateImpliedVolatility", CalculateImpliedVolatility );
    NODE_SET_PROTOTYPE_METHOD(tpl, "calculateOptionPrice", CalculateOptionPrice );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelCalculateImpliedVolatility", CancelCalculateImpliedVolatility );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelCalculateOptionPrice", CancelCalculateOptionPrice );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqGlobalCancel", ReqGlobalCancel );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqMarketDataType", ReqMarketDataType );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqPositions", ReqPositions );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelPositions", CancelPositions );
    NODE_SET_PROTOTYPE_METHOD(tpl, "reqAccountSummary", ReqAccountSummary );
    NODE_SET_PROTOTYPE_METHOD(tpl, "cancelAccountSummary", CancelAccountSummary );
    NODE_SET_PROTOTYPE_METHOD(tpl, "verifyRequest", VerifyRequest );
    NODE_SET_PROTOTYPE_METHOD(tpl, "verifyMessage", VerifyMessage );
    NODE_SET_PROTOTYPE_METHOD(tpl, "queryDisplayGroups", QueryDisplayGroups );
    NODE_SET_PROTOTYPE_METHOD(tpl, "subscribeToGroupEvents", SubscribeToGroupEvents );
    NODE_SET_PROTOTYPE_METHOD(tpl, "updateDisplayGroup", UpdateDisplayGroup );
    NODE_SET_PROTOTYPE_METHOD(tpl, "unsubscribeFromGroupEvents", UnsubscribeFromGroupEvents );

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set( String::NewFromUtf8( isolate, "NodeIbapi" ), tpl->GetFunction() );
}

void NodeIbapi::New( const FunctionCallbackInfo<Value> &args ) {

    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        NodeIbapi* obj = new NodeIbapi();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

void NodeIbapi::Connect( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 3 ) || 
        isWrongType( !args[0]->IsString(), 0 ) ||
        isWrongType( !args[1]->IsUint32(), 1 ) || 
        isWrongType( !args[2]->IsInt32(), 2 ) ) {
        return;
    }

    char * host = getChar( args[0], "" );
    bool conn = 
        obj->m_client.connect(
            host, args[1]->Uint32Value(), args[2]->Int32Value() );
    args.GetReturnValue().Set( Boolean::New( isolate, conn ) );
}

void NodeIbapi::Disconnect( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.disconnect();
}

void NodeIbapi::IsConnected( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    args.GetReturnValue().Set( Boolean::New( isolate, obj->m_client.isConnected() ) );
}

void NodeIbapi::ProcessMsg( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.processMessages();
}


///////////////////////////////////////////////////////////////////////////////
// node implementation for EClientSocket methods
///////////////////////////////////////////////////////////////////////////////
void NodeIbapi::ServerVersion( const FunctionCallbackInfo<Value>& args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    args.GetReturnValue().Set( Integer::New( isolate, obj->m_client.serverVersion() ) );
}
void NodeIbapi::TwsConnectionTime( const FunctionCallbackInfo<Value>& args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    args.GetReturnValue().Set(
        String::NewFromUtf8(isolate, obj->m_client.TwsConnectionTime().c_str() ) );
}
void NodeIbapi::ReqMktData( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 4 ) ) {
        return;
    }

    TickerId tickerId = args[0]->Int32Value();
    Contract contract;

    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract ); 

    IBString genericTick = getChar( args[2] );

    bool snapShot = args[3]->BooleanValue();

    TagValueListSPtr mktDataOptions;

    obj->m_client.reqMktData( tickerId, contract, genericTick, snapShot,
                              mktDataOptions );
}
void NodeIbapi::CancelMktData( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 1 ) ||
         isWrongType( !args[0]->IsUint32(), 0 ) ) {
        return;
    }

    TickerId tickerId = args[0]->Int32Value();

    obj->m_client.cancelMktData( tickerId );
}
void NodeIbapi::PlaceOrder( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    // TODO: Better way to handle arg num issue
    if ( args.Length() != 7 && args.Length() != 3 ) {
        isolate->ThrowException(
            Exception::TypeError(
                String::NewFromUtf8(isolate, "Wrong number of arguments, must be 3 or 7" ) ) );
        return;
    }

    OrderId orderId;
    Contract contract;
    Order order;

    orderId = args[0]->Int32Value();

    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );

    if ( args.Length() == 7 ) {
        if ( isWrongType( !args[2]->IsString(), 2 ) )
            return;

        order.action = getChar( args[2] );
        order.totalQuantity = args[3]->Int32Value();
        order.orderType = getChar( args[4] );
        order.lmtPrice = args[5]->NumberValue();
        order.auxPrice = args[6]->NumberValue();
    }
    else if ( args.Length() == 3 ) {
        if ( isWrongType( !args[2]->IsObject(), 2 ) )
            return;

        Handle<Object> ibOrder = Handle<Object>::Cast( args[2] );
        convertOrderForIb( ibOrder, order );
    }

    obj->m_client.placeOrder( orderId, contract, order );
}
void NodeIbapi::CancelOrder( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ||
         isWrongType( !args[0]->IsUint32(), 0 ) ) {
        return;
    }
    obj->m_client.cancelOrder( args[0]->Int32Value() );
}
void NodeIbapi::ReqOpenOrders( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.reqOpenOrders();
}
void NodeIbapi::ReqAccountUpdates( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 2 ) ||
         isWrongType( !args[0]->IsBoolean(), 0 ) ||
         isWrongType( !args[1]->IsString(), 1 ) ) {
        return;
    }
    bool subscribe = args[0]->BooleanValue();;
    IBString acctCode = getChar( args[1] );

    obj->m_client.reqAccountUpdates( subscribe, acctCode );
}
void NodeIbapi::ReqExecutions( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 2 ) ) {
        return;
    }
    int reqId = args[0]->Int32Value();
    ExecutionFilter filter;
    Handle<Object> ibExecutionFilter = Handle<Object>::Cast( args[1] );
    convertExecutionFilterForIb(ibExecutionFilter, filter);

    obj->m_client.reqExecutions( reqId, filter );
}
void NodeIbapi::ReqIds( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }
    int numIds = args[0]->Int32Value();
    obj->m_client.reqIds( numIds );
}
void NodeIbapi::CheckMessages( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.checkMessages();
}
void NodeIbapi::ReqContractDetails( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    int reqId = args[0]->Int32Value();

    if ( isWrongArgNumber( args.Length(), 2 ) ) {
        return;
    }

    Contract contract;

    reqId = args[0]->Int32Value();
    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );

    obj->m_client.reqContractDetails( reqId, contract );
}
void NodeIbapi::ReqMktDepth( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 3 ) ) {
        return;
    }

    TickerId tickerId = args[0]->Int32Value();
    Contract contract;

    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );

    int numRows = args[2]->Int32Value();

    TagValueListSPtr mktDepthOptions;

    obj->m_client.reqMktDepth( tickerId, contract, numRows, mktDepthOptions );
}
void NodeIbapi::CancelMktDepth( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }
    TickerId tickerId = args[0]->Int32Value();
    obj->m_client.cancelMktDepth( tickerId );
}
void NodeIbapi::ReqNewsBulletins( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }
    bool allMsgs = args[0]->BooleanValue();
    obj->m_client.reqNewsBulletins( allMsgs );
}
void NodeIbapi::CancelNewsBulletins( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.cancelNewsBulletins();
}
void NodeIbapi::SetServerLogLevel( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }
    int level = args[0]->Int32Value();
    obj->m_client.setServerLogLevel( level );
}
void NodeIbapi::ReqAutoOpenOrders( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }
    bool bAutoBind = args[0]->BooleanValue();
    obj->m_client.reqAutoOpenOrders( bAutoBind );
}
void NodeIbapi::ReqAllOpenOrders( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.reqAllOpenOrders();
}
void NodeIbapi::ReqManagedAccts( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.reqManagedAccts();
}
void NodeIbapi::RequestFA( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    // TODO: placeholder

    return;
}
void NodeIbapi::ReplaceFA( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    // TODO: placeholder
}
void NodeIbapi::ReqHistoricalData( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 8 ) ) {
        return;
    }
    TickerId id;
    Contract contract;
    IBString endDateTime;
    IBString durationStr;
    IBString barSizeSetting;
    IBString whatToShow;
    int useRTH;
    int formatDate;

    id = args[0]->Int32Value();

    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );

    endDateTime = getChar( args[2] );
    durationStr = getChar( args[3] );
    barSizeSetting = getChar( args[4] );
    whatToShow = getChar( args[5] );
    useRTH = args[6]->Int32Value();
    formatDate = args[7]->Int32Value();

    TagValueListSPtr chartOptions;

    obj->m_client.reqHistoricalData( id, contract, endDateTime, durationStr,
                                     barSizeSetting, whatToShow, useRTH,
                                     formatDate, chartOptions );
}
void NodeIbapi::ExerciseOptions( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 6 ) ) {
        return;
    }
    TickerId tickerId;
    Contract contract;
    int exerciseAction;
    int exerciseQuantity;
    IBString account;
    int override;

    tickerId = args[0]->Int32Value();
    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );

    exerciseAction = args[2]->Int32Value();
    exerciseQuantity = args[3]->Int32Value();
    account = getChar( args[4] );
    override = args[5]->Int32Value();

    obj->m_client.exerciseOptions( tickerId, contract, exerciseAction,
                                   exerciseQuantity, account, override );

}
void NodeIbapi::CancelHistoricalData( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }
    TickerId tickerId = args[0]->Int32Value();
    obj->m_client.cancelHistoricalData( tickerId );
}
void NodeIbapi::ReqRealTimeBars( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 5 ) ) {
        return;
    }

    TickerId tickerId = args[0]->Int32Value();
    Contract contract;

    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );

    int barSize = args[2]->Int32Value();
    IBString whatToShow = getChar( args[3] );
    bool useRTH = args[4]->BooleanValue();

    TagValueListSPtr realTimeBarsOptions;
    obj->m_client.reqRealTimeBars( tickerId, contract, barSize, whatToShow,
                                   useRTH, realTimeBarsOptions );
}
void NodeIbapi::CancelRealTimeBars( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    TickerId tickerId = args[0]->Int32Value();
    obj->m_client.cancelRealTimeBars( tickerId );
}
void NodeIbapi::CancelScannerSubscription( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    TickerId tickerId = args[0]->Int32Value();
    obj->m_client.cancelScannerSubscription( tickerId );
}
void NodeIbapi::ReqScannerParameters( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.reqScannerParameters();
}
void NodeIbapi::ReqScannerSubscription( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 2 ) ) {
        return;
    }

    TickerId tickerId = args[0]->Int32Value();
    ScannerSubscription subscription;
    Handle<Object> ibsub = Handle<Object>::Cast( args[1] );
    convertSubForIb( ibsub, subscription );

    TagValueListSPtr scannerSubscriptionOptions;

    obj->m_client.reqScannerSubscription( tickerId, subscription,
                                        scannerSubscriptionOptions );
}
void NodeIbapi::ReqCurrentTime( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.reqCurrentTime();
}
void NodeIbapi::ReqFundamentalData( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 3 ) ) {
        return;
    }

    TickerId reqId = args[0]->Int32Value();
    Contract contract;
 
    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );
    IBString reportType = getChar( args[2] );

    obj->m_client.reqFundamentalData( reqId, contract, reportType );
}
void NodeIbapi::CancelFundamentalData( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    TickerId reqId = args[0]->Int32Value();
    obj->m_client.cancelFundamentalData( reqId );
}
void NodeIbapi::CalculateImpliedVolatility( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 4 ) ) {
        return;
    }

    TickerId reqId = args[0]->Int32Value();
    Contract contract;
 
    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );
    double optionPrice = args[2]->NumberValue();
    double underPrice = args[3]->NumberValue();

    obj->m_client.calculateImpliedVolatility( reqId, contract, optionPrice,
                                              underPrice );
}
void NodeIbapi::CalculateOptionPrice( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 4 ) ) {
        return;
    }

    TickerId reqId = args[0]->Int32Value();
    Contract contract;
 
    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );
    double volatility = args[2]->NumberValue();
    double underPrice = args[3]->NumberValue();

    obj->m_client.calculateOptionPrice( reqId, contract, volatility,
                                        underPrice );
}
void NodeIbapi::CancelCalculateImpliedVolatility(
    const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    TickerId reqId = args[0]->Int32Value();
    obj->m_client.cancelCalculateImpliedVolatility( reqId );
}
void NodeIbapi::CancelCalculateOptionPrice( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    TickerId reqId = args[0]->Int32Value();
    obj->m_client.cancelCalculateOptionPrice( reqId );
}
void NodeIbapi::ReqGlobalCancel( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.reqGlobalCancel();
}
void NodeIbapi::ReqMarketDataType( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    int marketDataType = args[0]->Int32Value();
    obj->m_client.reqMarketDataType( marketDataType );
}
void NodeIbapi::ReqPositions( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.reqPositions();
}
void NodeIbapi::CancelPositions( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    obj->m_client.cancelPositions();
}
void NodeIbapi::ReqAccountSummary( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 3 ) ) {
        return;
    }

    int reqId = args[0]->Int32Value();
    IBString groupName = getChar( args[1] );
    IBString tags = getChar( args[2] );
    obj->m_client.reqAccountSummary( reqId, groupName, tags );
}
void NodeIbapi::CancelAccountSummary( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    int reqId = args[0]->Int32Value();
    obj->m_client.cancelAccountSummary( reqId );
}
void NodeIbapi::VerifyRequest( const FunctionCallbackInfo<Value>& args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 2 ) ) {
        return;
    }

    if ( isWrongType( !args[0]->IsString(), 0) ||
         isWrongType( !args[1]->IsString(), 1) ) {
        return;
    }

    IBString apiName = getChar( args[0] );
    IBString apiVersion = getChar( args[1] );

    obj->m_client.verifyRequest( apiName, apiVersion );
}
void NodeIbapi::VerifyMessage( const FunctionCallbackInfo<Value>& args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );
    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    if ( isWrongType( !args[0]->IsString(), 0 ) ) {
        return;
    }

    IBString apiData = getChar( args[0] );

    obj->m_client.verifyMessage( apiData );
}
void NodeIbapi::QueryDisplayGroups( const FunctionCallbackInfo<Value>& args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    if ( isWrongType( !args[0]->IsInt32(), 0 ) ) {
        return;
    }

    int reqId = args[0]->Int32Value();

    obj->m_client.queryDisplayGroups( reqId );
}
void NodeIbapi::SubscribeToGroupEvents( const FunctionCallbackInfo<Value>& args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 2 ) ) {
        return;
    }

    if ( isWrongType( !args[0]->IsInt32(), 0 ) ||
         isWrongType( !args[1]->IsInt32(), 1 ) ) {
        return;
    }

    int reqId = args[0]->Int32Value();
    int groupId = args[1]->Int32Value();

    obj->m_client.subscribeToGroupEvents( reqId, groupId );
}
void NodeIbapi::UpdateDisplayGroup( const FunctionCallbackInfo<Value>& args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 2 ) ) {
        return;
    }

    if ( isWrongType( !args[0]->IsInt32(), 0 ) ||
         isWrongType( !args[1]->IsString(), 1 ) ) {
        return;
    }

    int reqId = args[0]->Int32Value();
    IBString contractInfo = getChar( args[1] );

    obj->m_client.updateDisplayGroup( reqId, contractInfo );
}
void NodeIbapi::UnsubscribeFromGroupEvents( const FunctionCallbackInfo<Value>& args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    if ( isWrongArgNumber( args.Length(), 1 ) ) {
        return;
    }

    if ( isWrongType( !args[0]->IsInt32(), 0 ) ) {
        return;
    }

    int reqId = args[0]->Int32Value();

    obj->m_client.unsubscribeFromGroupEvents( reqId );
}


///////////////////////////////////////////////////////////////////////////////
//  Getters
///////////////////////////////////////////////////////////////////////////////

void NodeIbapi::GetInboundMsg( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.Holder() );

    JSONNode newMsg;
    newMsg = obj->m_client.getInboundMsg();

    Handle<Object> retData = Object::New( isolate );
    retData = obj->m_parser.parse( newMsg );

    args.GetReturnValue().Set( retData );
}

///////////////////////////////////////////////////////////////////////////////
//  Helper methods
///////////////////////////////////////////////////////////////////////////////
// see http://stackoverflow.com/questions/10507323/
//  shortest-way-one-liner-to-get-a-default-argument-out-of-a-v8-function
char *NodeIbapi::getChar( v8::Local<v8::Value> value, const char *fallback ) {
    if ( value->IsString() ) {
        v8::String::Utf8Value string( value );
        char *str  = ( char * ) malloc( string.length() + 1 );
        std::strcpy( str, *string );
        return str;
    }
    char *str = ( char * ) malloc( std::strlen( fallback ) + 1 );
    std::strcpy( str, fallback );
    return str;
}

bool NodeIbapi::isWrongArgNumber( int argNumInput, int argNumExpected ) {
    Isolate* isolate = Isolate::GetCurrent();
    if ( argNumInput != argNumExpected ) {
        isolate->ThrowException(
            Exception::TypeError(
                String::NewFromUtf8(isolate, "Wrong number of arguments" ) ) );
        return true;
    }
    return false;
}

bool NodeIbapi::isWrongType( bool predicateRes, int argId ) {
    Isolate* isolate = Isolate::GetCurrent();
    if ( predicateRes ) {
        std::ostringstream ss;
        ss << "Argument " << argId << " is of wrong type.";
        isolate->ThrowException(
            Exception::TypeError(
                String::NewFromUtf8(isolate, ss.str().c_str() ) ) );
        return true;
    } 
    return false;
}

void NodeIbapi::convertExecutionFilterForIb( Handle<Object> ibExecutionFilter,
                                             ExecutionFilter &filter) {
    Isolate* isolate = Isolate::GetCurrent();

    filter.m_clientId = ibExecutionFilter->Get(
            String::NewFromUtf8(isolate, "clientId") )->Int32Value();
    filter.m_acctCode = getChar(
            ibExecutionFilter->Get( String::NewFromUtf8(isolate, "acctCode") ));
    filter.m_time = getChar(
            ibExecutionFilter->Get( String::NewFromUtf8(isolate, "time") ) );
    filter.m_symbol = getChar(
            ibExecutionFilter->Get( String::NewFromUtf8(isolate, "symbol") ) );
    filter.m_secType = getChar(
            ibExecutionFilter->Get( String::NewFromUtf8(isolate, "secType") ) );
    filter.m_exchange = getChar(
            ibExecutionFilter->Get( String::NewFromUtf8(isolate, "exchange") ));
    filter.m_side = getChar(
            ibExecutionFilter->Get( String::NewFromUtf8(isolate, "side") ) );
}

void NodeIbapi::convertContractForIb( Handle<Object> ibContract,
                                      Contract &contract ) {
    Isolate* isolate = Isolate::GetCurrent();
    // checks if order is being submitted through Conract ID from
    //  contract specification
    contract.conId = ibContract->Get( String::NewFromUtf8(isolate, "conId" ) )->Int32Value();
    contract.exchange =
        getChar( ibContract->Get( String::NewFromUtf8(isolate, "exchange" ) ) );

    contract.symbol = getChar( ibContract->Get( String::NewFromUtf8(isolate, "symbol" ) ) );
    contract.secType = getChar( ibContract->Get( String::NewFromUtf8(isolate, "secType" ) ) );
    contract.expiry = getChar( ibContract->Get( String::NewFromUtf8(isolate, "expiry" ) ) );
    contract.strike = ibContract->Get( String::NewFromUtf8(isolate, "strike" ) )->NumberValue();
    contract.right = getChar( ibContract->Get( String::NewFromUtf8(isolate, "right" ) ) );
    contract.multiplier =
        getChar( ibContract->Get( String::NewFromUtf8(isolate, "multiplier" ) ) );
    contract.primaryExchange =
        getChar( ibContract->Get( String::NewFromUtf8(isolate, "primaryExchange" ) ) );
    contract.currency = getChar( ibContract->Get( String::NewFromUtf8(isolate, "currency" ) ) );
    contract.localSymbol =
        getChar( ibContract->Get( String::NewFromUtf8(isolate, "localSymbol" ) ) );
    contract.tradingClass =
        getChar( ibContract->Get( String::NewFromUtf8(isolate, "tradingClass" ) ) );
    contract.includeExpired =
        ibContract->Get( String::NewFromUtf8(isolate, "includeExpired" ) )->BooleanValue();
    contract.secIdType =
        getChar( ibContract->Get( String::NewFromUtf8(isolate, "secIdType" ) ) );
    contract.secId = getChar( ibContract->Get( String::NewFromUtf8(isolate, "secId" ) ) );
}

void NodeIbapi::convertSubForIb( Handle<Object> scannerSub,
                                 ScannerSubscription &subscription ) {
    Isolate* isolate = Isolate::GetCurrent();

    subscription.numberOfRows =
        scannerSub->Get( String::NewFromUtf8(isolate, "numberOfRows" ) )->Int32Value();
    subscription.abovePrice =
        scannerSub->Get( String::NewFromUtf8(isolate, "abovePrice" ) )->NumberValue();
    subscription.belowPrice =
        scannerSub->Get( String::NewFromUtf8(isolate, "belowPrice" ) )->NumberValue();
    subscription.aboveVolume =
        scannerSub->Get( String::NewFromUtf8(isolate, "aboveVolume" ) )->Int32Value();
    subscription.marketCapAbove =
        scannerSub->Get( String::NewFromUtf8(isolate, "marketCapAbove" ) )->NumberValue();
    subscription.marketCapBelow =
        scannerSub->Get( String::NewFromUtf8(isolate, "marketCapBelow" ) )->NumberValue();
    subscription.couponRateAbove =
        scannerSub->Get( String::NewFromUtf8(isolate, "couponRateAbove" ) )->NumberValue();
    subscription.couponRateBelow =
        scannerSub->Get( String::NewFromUtf8(isolate, "couponRateBelow" ) )->NumberValue();

    subscription.instrument =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "instrument" ) ) );
    subscription.locationCode =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "locationCode" ) ) );
    subscription.scanCode =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "scanCode" ) ) );
    subscription.moodyRatingAbove =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "moodyRatingAbove" ) ) );
    subscription.moodyRatingBelow =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "moodyRatingBelow" ) ) );
    subscription.spRatingAbove =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "spRatingAbove" ) ) );
    subscription.spRatingBelow =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "spRatingBelow" ) ) );
    subscription.maturityDateAbove =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "maturityDateAbove" ) ) );
    subscription.maturityDateBelow =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "maturityDateBelow" ) ) );
    subscription.excludeConvertible =
        scannerSub->Get( String::NewFromUtf8(isolate, "excludeConvertible" ) )->Int32Value();
    subscription.averageOptionVolumeAbove =
        scannerSub->Get(
            String::NewFromUtf8(isolate, "averageOptionVolumeAbove" ) )->Int32Value();
    subscription.scannerSettingPairs =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "scannerSettingPairs" ) ) );
    subscription.stockTypeFilter =
        getChar( scannerSub->Get( String::NewFromUtf8(isolate, "stockTypeFilter" ) ) );
}

void NodeIbapi::convertOrderForIb( Handle<Object> ibOrder, Order &order ) {
    Isolate* isolate = Isolate::GetCurrent();
    // order identifier
    order.orderId = ibOrder->Get( String::NewFromUtf8(isolate, "orderId" ) )->Int32Value();
    order.clientId = ibOrder->Get( String::NewFromUtf8(isolate, "clientId" ) )->Int32Value();
    order.permId = ibOrder->Get( String::NewFromUtf8(isolate, "permId" ) )->Int32Value();

    // main order fields
    order.action = getChar( ibOrder->Get( String::NewFromUtf8(isolate, "action" ) ) );
    order.totalQuantity =
        ibOrder->Get( String::NewFromUtf8(isolate, "totalQuantity" ) )->Int32Value();
    order.orderType = getChar( ibOrder->Get( String::NewFromUtf8(isolate, "orderType" ) ) );
    order.lmtPrice = ibOrder->Get( String::NewFromUtf8(isolate, "lmtPrice" ) )->NumberValue();
    order.auxPrice = ibOrder->Get( String::NewFromUtf8(isolate, "auxPrice" ) )->NumberValue();

    // extended order fields
    order.tif = getChar( ibOrder->Get( String::NewFromUtf8(isolate, "tif" ) ) );
    order.ocaGroup = getChar( ibOrder->Get( String::NewFromUtf8(isolate, "ocaGroup" ) ) );
    order.ocaType = ibOrder->Get( String::NewFromUtf8(isolate, "ocaType" ) )->Int32Value();
    order.orderRef = getChar( ibOrder->Get( String::NewFromUtf8(isolate, "orderRef" ) ) );
    order.transmit = ibOrder->Get( String::NewFromUtf8(isolate, "transmit" ) )->BooleanValue();
    order.parentId = ibOrder->Get( String::NewFromUtf8(isolate, "parentId" ) )->Int32Value();
    order.blockOrder =
        ibOrder->Get( String::NewFromUtf8(isolate, "blockOrder" ) )->BooleanValue();
    order.sweepToFill =
        ibOrder->Get( String::NewFromUtf8(isolate, "sweepToFill" ) )->BooleanValue();
    order.displaySize =
        ibOrder->Get( String::NewFromUtf8(isolate, "displaySize" ) )->Int32Value();
    order.triggerMethod =
        ibOrder->Get( String::NewFromUtf8(isolate, "triggerMethod" ) )->Int32Value();
    order.outsideRth =
        ibOrder->Get( String::NewFromUtf8(isolate, "outsideRth" ) )->BooleanValue();
    order.hidden = ibOrder->Get( String::NewFromUtf8(isolate, "hidden" ) )->BooleanValue();
    order.goodAfterTime =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "goodAfterTime" ) ) );
    order.goodTillDate =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "goodTillDate" ) ) );
    order.rule80A = getChar( ibOrder->Get( String::NewFromUtf8(isolate, "rule80A" ) ) );
    order.allOrNone =
        ibOrder->Get( String::NewFromUtf8(isolate, "allOrNone" ) )->BooleanValue();
    order.minQty = ibOrder->Get( String::NewFromUtf8(isolate, "minQty" ) )->Int32Value();
    order.percentOffset =
        ibOrder->Get( String::NewFromUtf8(isolate, "percentOffset" ) )->NumberValue();
    order.overridePercentageConstraints =
        ibOrder->Get(
            String::NewFromUtf8(isolate, "overridePercentageConstraints" ) )->BooleanValue();
    order.trailStopPrice =
        ibOrder->Get( String::NewFromUtf8(isolate, "trailStopPrice" ) )->NumberValue();
    order.trailingPercent =
        ibOrder->Get( String::NewFromUtf8(isolate, "trailingPercent" ) )->NumberValue();

    // financial advisors only

    // institutional (ie non-cleared) only

    // SMART routing only
    order.discretionaryAmt =
        ibOrder->Get( String::NewFromUtf8(isolate, "discretionaryAmt" ) )->NumberValue();
    order.eTradeOnly =
        ibOrder->Get( String::NewFromUtf8(isolate, "eTradeOnly" ) )->BooleanValue();
    order.firmQuoteOnly =
        ibOrder->Get( String::NewFromUtf8(isolate, "firmQuoteOnly" ) )->BooleanValue();
    order.nbboPriceCap =
        ibOrder->Get( String::NewFromUtf8(isolate, "nbboPriceCap" ) )->NumberValue();
    order.optOutSmartRouting =
        ibOrder->Get( String::NewFromUtf8(isolate, "optOutSmartRouting" ) )->BooleanValue();

    // BOX exchange orders only
    order.auctionStrategy =
        ibOrder->Get( String::NewFromUtf8(isolate, "auctionStrategy" ) )->Int32Value();
    order.startingPrice =
        ibOrder->Get( String::NewFromUtf8(isolate, "startingPrice" ) )->NumberValue();
    order.stockRefPrice =
        ibOrder->Get( String::NewFromUtf8(isolate, "stockRefPrice" ) )->NumberValue();
    order.delta =
        ibOrder->Get( String::NewFromUtf8(isolate, "delta" ) )->NumberValue();

    // pegged to stock and VOL orders only
    order.stockRangeLower =
        ibOrder->Get( String::NewFromUtf8(isolate, "stockRangeLower" ) )->NumberValue();
    order.stockRangeUpper =
        ibOrder->Get( String::NewFromUtf8(isolate, "stockRangeUpper" ) )->NumberValue();

    // VOLATILITY ORDERS ONLY
    order.volatility =
        ibOrder->Get( String::NewFromUtf8(isolate, "volatility" ) )->NumberValue();
    order.volatilityType =
        ibOrder->Get( String::NewFromUtf8(isolate, "volatilityType" ) )->Int32Value();
    order.deltaNeutralOrderType =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "deltaNeutralOrderType" ) ) );
    order.deltaNeutralAuxPrice =
        ibOrder->Get( String::NewFromUtf8(isolate, "deltaNeutralAuxPrice" ) )->NumberValue();
    order.deltaNeutralConId =
        ibOrder->Get( String::NewFromUtf8(isolate, "deltaNeutralConId" ) )->Int32Value();
    order.deltaNeutralSettlingFirm =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "deltaNeutralSettlingFirm" ) ) );
    order.deltaNeutralClearingAccount =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "deltaNeutralClearingAccount" ) ) );
    order.deltaNeutralClearingIntent =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "deltaNeutralClearingIntent" ) ) );
    order.deltaNeutralOpenClose =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "deltaNeutralOpenClose" ) ) );
    order.deltaNeutralShortSale =
        ibOrder->Get( String::NewFromUtf8(isolate, "deltaNeutralShortSale" ) )->BooleanValue();
    order.deltaNeutralShortSaleSlot =
        ibOrder->Get( String::NewFromUtf8(isolate, "deltaNeutralShortSaleSlot" ) )->Int32Value();
    order.deltaNeutralDesignatedLocation =
        getChar( ibOrder->Get(
            String::NewFromUtf8(isolate, "deltaNeutralDesignatedLocation" ) ) );
    order.continuousUpdate =
        ibOrder->Get( String::NewFromUtf8(isolate, "continuousUpdate" ) )->BooleanValue();
    order.referencePriceType =
        ibOrder->Get( String::NewFromUtf8(isolate, "referencePriceType" ) )->Int32Value();

    // COMBO ORDERS ONLY

    // SCALE ORDERS ONLY
    order.scaleInitLevelSize =
        ibOrder->Get( String::NewFromUtf8(isolate, "scaleInitLevelSize" ) )->Int32Value();
    order.scaleSubsLevelSize =
        ibOrder->Get( String::NewFromUtf8(isolate, "scaleSubsLevelSize" ) )->Int32Value();
    order.scalePriceIncrement =
        ibOrder->Get( String::NewFromUtf8(isolate, "scalePriceIncrement" ) )->NumberValue();
    order.scalePriceAdjustValue =
        ibOrder->Get( String::NewFromUtf8(isolate, "scalePriceAdjustValue" ) )->NumberValue();
    order.scalePriceAdjustInterval =
        ibOrder->Get( String::NewFromUtf8(isolate, "scalePriceAdjustInterval" ) )->Int32Value();
    order.scaleProfitOffset =
        ibOrder->Get( String::NewFromUtf8(isolate, "scaleProfitOffset" ) )->NumberValue();
    order.scaleAutoReset =
        ibOrder->Get( String::NewFromUtf8(isolate, "scaleAutoReset" ) )->BooleanValue();
    order.scaleInitPosition =
        ibOrder->Get( String::NewFromUtf8(isolate, "scaleInitPosition" ) )->Int32Value();
    order.scaleInitFillQty =
        ibOrder->Get( String::NewFromUtf8(isolate, "scaleInitFillQty" ) )->Int32Value();
    order.scaleRandomPercent =
        ibOrder->Get( String::NewFromUtf8(isolate, "scaleRandomPercent" ) )->BooleanValue();

    // HEDGE ORDERS
    order.hedgeType = getChar( ibOrder->Get( String::NewFromUtf8(isolate, "hedgeType" ) ) );
    order.hedgeParam = getChar( ibOrder->Get( String::NewFromUtf8(isolate, "hedgeParam" ) ) );

    // Clearing info
    order.account = getChar( ibOrder->Get( String::NewFromUtf8(isolate, "account" ) ) );
    order.settlingFirm =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "settlingFirm" ) ) );
    order.clearingAccount =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "clearingAccount" ) ) );
    order.clearingIntent =
        getChar( ibOrder->Get( String::NewFromUtf8(isolate, "clearingIntent" ) ) );

    // ALGO ORDERS ONLY

    // What-if

    // Not Held
}
