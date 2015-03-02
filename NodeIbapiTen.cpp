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

NodeIbapi::NodeIbapi() {
};
NodeIbapi::~NodeIbapi() {
};

void NodeIbapi::Init( Handle<Object> exports ) {
    // Prep constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New( New );
    tpl->SetClassName( String::NewSymbol( "NodeIbapi" ) );
    tpl->InstanceTemplate()->SetInternalFieldCount( 1 );

    // prototype
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "processMsg" ),
        FunctionTemplate::New( ProcessMsg )->GetFunction() );
    /// getters
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "getInboundMsg" ),
        FunctionTemplate::New( GetInboundMsg )->GetFunction() );

    /// EClientSocket
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "connect" ),
        FunctionTemplate::New( Connect )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "disconnect" ),
        FunctionTemplate::New( Disconnect )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "isConnected" ),
        FunctionTemplate::New( IsConnected )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "serverVersion" ),
        FunctionTemplate::New( ServerVersion )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "twsConnectionTime" ),
        FunctionTemplate::New( TwsConnectionTime )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqMktData" ),
        FunctionTemplate::New( ReqMktData )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "cancelMktData" ),
        FunctionTemplate::New( CancelMktData )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "placeOrder" ),
        FunctionTemplate::New( PlaceOrder )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "cancelOrder" ),
        FunctionTemplate::New( CancelOrder )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqOpenOrders" ),
        FunctionTemplate::New( ReqOpenOrders )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqAccountUpdates" ),
        FunctionTemplate::New( ReqAccountUpdates )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqExecutions" ),
        FunctionTemplate::New( ReqExecutions )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqIds" ),
        FunctionTemplate::New( ReqIds )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "checkMessages" ),
        FunctionTemplate::New( CheckMessages )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqContractDetails" ),
        FunctionTemplate::New( ReqContractDetails )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqMktDepth" ),
        FunctionTemplate::New( ReqMktDepth )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "cancelMktDepth" ),
        FunctionTemplate::New( CancelMktDepth )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqNewsBulletins" ),
        FunctionTemplate::New( ReqNewsBulletins )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "cancelNewsBulletins" ),
        FunctionTemplate::New( CancelNewsBulletins )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "setServerLogLevel" ),
        FunctionTemplate::New( SetServerLogLevel )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqAutoOpenOrders" ),
        FunctionTemplate::New( ReqAutoOpenOrders )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqAllOpenOrders" ),
        FunctionTemplate::New( ReqAllOpenOrders )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqManagedAccts" ),
        FunctionTemplate::New( ReqManagedAccts )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "requestFA" ),
        FunctionTemplate::New( RequestFA )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "replaceFA" ),
        FunctionTemplate::New( ReplaceFA )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqHistoricalData" ),
        FunctionTemplate::New( ReqHistoricalData )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "exerciseOptions" ),
        FunctionTemplate::New( ExerciseOptions )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "cancelHistoricalData" ),
        FunctionTemplate::New( CancelHistoricalData )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqRealtimeBars" ),
        FunctionTemplate::New( ReqRealTimeBars )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "cancelRealTimeBars" ),
        FunctionTemplate::New( CancelRealTimeBars )->GetFunction() );
    tpl->PrototypeTemplate()->Set( 
        String::NewSymbol( "cancelScannerSubscription" ),
        FunctionTemplate::New( CancelScannerSubscription )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqScannerParameters" ),
        FunctionTemplate::New( ReqScannerParameters )->GetFunction() );
    tpl->PrototypeTemplate()->Set(
        String::NewSymbol( "reqScannerSubscription" ),
        FunctionTemplate::New( ReqScannerSubscription )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqCurrentTime" ),
        FunctionTemplate::New( ReqCurrentTime )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqFundamentalData" ),
        FunctionTemplate::New( ReqFundamentalData )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "cancelFundamentalData" ),
        FunctionTemplate::New( CancelFundamentalData )->GetFunction() );
    tpl->PrototypeTemplate()->Set(
        String::NewSymbol( "calculateImpliedVolatility" ),
        FunctionTemplate::New( CalculateImpliedVolatility )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "calculateOptionPrice" ),
        FunctionTemplate::New( CalculateOptionPrice )->GetFunction() );
    tpl->PrototypeTemplate()->Set(
        String::NewSymbol( "cancelCalculateImpliedVolatility" ),
        FunctionTemplate::
            New( CancelCalculateImpliedVolatility )->GetFunction() );
    tpl->PrototypeTemplate()->Set(
        String::NewSymbol( "cancelCalculateOptionPrice" ),
        FunctionTemplate::New( CancelCalculateOptionPrice )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqGlobalCancel" ),
        FunctionTemplate::New( ReqGlobalCancel )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqMarketDataType" ),
        FunctionTemplate::New( ReqMarketDataType )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqPositions" ),
        FunctionTemplate::New( ReqPositions )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "cancelPositions" ),
        FunctionTemplate::New( CancelPositions )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "reqAccountSummary" ),
        FunctionTemplate::New( ReqAccountSummary )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "cancelAccountSummary" ),
        FunctionTemplate::New( CancelAccountSummary )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "verifyRequest" ),
        FunctionTemplate::New( VerifyRequest )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "verifyMessage" ),
        FunctionTemplate::New( VerifyMessage )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "queryDisplayGroups" ),
        FunctionTemplate::New( QueryDisplayGroups )->GetFunction() );
    tpl->PrototypeTemplate()->Set(
        String::NewSymbol( "subscribeToGroupEvents" ),
        FunctionTemplate::New( SubscribeToGroupEvents )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "updateDisplayGroup" ),
        FunctionTemplate::New( UpdateDisplayGroup )->GetFunction() );
    tpl->PrototypeTemplate()->Set(
        String::NewSymbol( "unsubscribeFromGroupEvents" ),
        FunctionTemplate::New( UnsubscribeFromGroupEvents )->GetFunction() );

    Persistent<Function> constructor =
        Persistent<Function>::New( tpl->GetFunction() );
    exports->Set( String::NewSymbol( "NodeIbapi" ), constructor );
}

Handle<Value> NodeIbapi::New( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = new NodeIbapi();
    obj->Wrap( args.This() );

    return args.This();
}

Handle<Value> NodeIbapi::Connect( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 3 ) || 
        isWrongType( !args[0]->IsString(), 0 ) ||
        isWrongType( !args[1]->IsUint32(), 1 ) || 
        isWrongType( !args[2]->IsInt32(), 2 ) ) {
        return scope.Close( Undefined() );
    }

    char * host = getChar( args[0], "" );
    bool conn = 
        obj->m_client.connect(
            host, args[1]->Uint32Value(), args[2]->Int32Value() );
    return scope.Close( Boolean::New( conn ) );
}

Handle<Value> NodeIbapi::Disconnect( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.disconnect();
    return scope.Close( Undefined() );
}

Handle<Value> NodeIbapi::IsConnected( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    return scope.Close( Boolean::New( obj->m_client.isConnected() ) );
}

Handle<Value> NodeIbapi::ProcessMsg( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.processMessages();
    return scope.Close( Undefined() );
}


///////////////////////////////////////////////////////////////////////////////
// node implementation for EClientSocket methods
///////////////////////////////////////////////////////////////////////////////
Handle<Value> NodeIbapi::ServerVersion( const Arguments& args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    return scope.Close( Integer::New( obj->m_client.serverVersion() ) );
}
Handle<Value> NodeIbapi::TwsConnectionTime( const Arguments& args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    return scope.Close(
        String::New( obj->m_client.TwsConnectionTime().c_str() ) );
}
Handle<Value> NodeIbapi::ReqMktData( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 4 ) ) {
        return scope.Close( Undefined() );
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
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelMktData( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 1 ) ||
         isWrongType( !args[0]->IsUint32(), 0 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId tickerId = args[0]->Int32Value();

    obj->m_client.cancelMktData( tickerId );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::PlaceOrder( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    // TODO: Better way to handle arg num issue
    if ( args.Length() != 7 && args.Length() != 3 ) {
        ThrowException(
            Exception::TypeError(
                String::New( "Wrong number of arguments, must be 3 or 7" ) ) );
        return scope.Close( Undefined() );
    }

    OrderId orderId;
    Contract contract;
    Order order;

    orderId = args[0]->Int32Value();

    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );

    if ( args.Length() == 7 ) {
        if ( isWrongType( !args[2]->IsString(), 2 ) )
            return scope.Close( Undefined() );

        order.action = getChar( args[2] );
        order.totalQuantity = args[3]->Int32Value();
        order.orderType = getChar( args[4] );
        order.lmtPrice = args[5]->NumberValue();
        order.auxPrice = args[6]->NumberValue();
    }
    else if ( args.Length() == 3 ) {
        if ( isWrongType( !args[2]->IsObject(), 2 ) )
            return scope.Close( Undefined() );

        Handle<Object> ibOrder = Handle<Object>::Cast( args[2] );
        convertOrderForIb( ibOrder, order );
    }

    obj->m_client.placeOrder( orderId, contract, order );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelOrder( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ||
         isWrongType( !args[0]->IsUint32(), 0 ) ) {
        return scope.Close( Undefined() );
    }
    obj->m_client.cancelOrder( args[0]->Int32Value() );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqOpenOrders( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.reqOpenOrders();
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqAccountUpdates( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 2 ) ||
         isWrongType( !args[0]->IsBoolean(), 0 ) ||
         isWrongType( !args[1]->IsString(), 1 ) ) {
        return scope.Close( Undefined() );
    }
    bool subscribe = args[0]->BooleanValue();;
    IBString acctCode = getChar( args[1] );

    obj->m_client.reqAccountUpdates( subscribe, acctCode );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqExecutions( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 8 ) ) {
        return scope.Close( Undefined() );
    }
    int reqId = args[0]->Int32Value();
    ExecutionFilter filter;
    Handle<Object> ibExecutionFilter = Handle<Object>::Cast( args[1] );
    convertExecutionFilterForIb(ibExecutionFilter, filter);

    obj->m_client.reqExecutions( reqId, filter );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqIds( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }
    int numIds = args[0]->Int32Value();
    obj->m_client.reqIds( numIds );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CheckMessages( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.checkMessages();
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqContractDetails( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    int reqId = args[0]->Int32Value();

    if ( isWrongArgNumber( args, 2 ) ) {
        return scope.Close( Undefined() );
    }

    Contract contract;

    reqId = args[0]->Int32Value();
    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );

    obj->m_client.reqContractDetails( reqId, contract );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqMktDepth( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 3 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId tickerId = args[0]->Int32Value();
    Contract contract;

    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );

    int numRows = args[2]->Int32Value();

    TagValueListSPtr mktDepthOptions;

    obj->m_client.reqMktDepth( tickerId, contract, numRows, mktDepthOptions );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelMktDepth( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }
    TickerId tickerId = args[0]->Int32Value();
    obj->m_client.cancelMktDepth( tickerId );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqNewsBulletins( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }
    bool allMsgs = args[0]->BooleanValue();
    obj->m_client.reqNewsBulletins( allMsgs );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelNewsBulletins( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.cancelNewsBulletins();
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::SetServerLogLevel( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }
    int level = args[0]->Int32Value();
    obj->m_client.setServerLogLevel( level );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqAutoOpenOrders( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }
    bool bAutoBind = args[0]->BooleanValue();
    obj->m_client.reqAutoOpenOrders( bAutoBind );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqAllOpenOrders( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.reqAllOpenOrders();
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqManagedAccts( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.reqManagedAccts();
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::RequestFA( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    // TODO: placeholder

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReplaceFA( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    // TODO: placeholder

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqHistoricalData( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 8 ) ) {
        return scope.Close( Undefined() );
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
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ExerciseOptions( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 6 ) ) {
        return scope.Close( Undefined() );
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

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelHistoricalData( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }
    TickerId tickerId = args[0]->Int32Value();
    obj->m_client.cancelHistoricalData( tickerId );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqRealTimeBars( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 5 ) ) {
        return scope.Close( Undefined() );
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
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelRealTimeBars( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId tickerId = args[0]->Int32Value();
    obj->m_client.cancelRealTimeBars( tickerId );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelScannerSubscription( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId tickerId = args[0]->Int32Value();
    obj->m_client.cancelScannerSubscription( tickerId );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqScannerParameters( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.reqScannerParameters();
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqScannerSubscription( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 2 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId tickerId = args[0]->Int32Value();
    ScannerSubscription subscription;
    Handle<Object> ibsub = Handle<Object>::Cast( args[1] );
    convertSubForIb( ibsub, subscription );

    TagValueListSPtr scannerSubscriptionOptions;

    obj->m_client.reqScannerSubscription( tickerId, subscription,
                                        scannerSubscriptionOptions );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqCurrentTime( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.reqCurrentTime();
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqFundamentalData( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 3 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId reqId = args[0]->Int32Value();
    Contract contract;
 
    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );
    IBString reportType = getChar( args[2] );

    obj->m_client.reqFundamentalData( reqId, contract, reportType );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelFundamentalData( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId reqId = args[0]->Int32Value();
    obj->m_client.cancelFundamentalData( reqId );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CalculateImpliedVolatility( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 4 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId reqId = args[0]->Int32Value();
    Contract contract;
 
    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );
    double optionPrice = args[2]->NumberValue();
    double underPrice = args[3]->NumberValue();

    obj->m_client.calculateImpliedVolatility( reqId, contract, optionPrice,
                                              underPrice );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CalculateOptionPrice( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 4 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId reqId = args[0]->Int32Value();
    Contract contract;
 
    Handle<Object> ibContract = Handle<Object>::Cast( args[1] );
    convertContractForIb( ibContract, contract );
    double volatility = args[2]->NumberValue();
    double underPrice = args[3]->NumberValue();

    obj->m_client.calculateOptionPrice( reqId, contract, volatility,
                                        underPrice );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelCalculateImpliedVolatility(
    const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId reqId = args[0]->Int32Value();
    obj->m_client.cancelCalculateImpliedVolatility( reqId );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelCalculateOptionPrice( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    TickerId reqId = args[0]->Int32Value();
    obj->m_client.cancelCalculateOptionPrice( reqId );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqGlobalCancel( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.reqGlobalCancel();
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqMarketDataType( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    int marketDataType = args[0]->Int32Value();
    obj->m_client.reqMarketDataType( marketDataType );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqPositions( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.reqPositions();

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelPositions( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    obj->m_client.cancelPositions();

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::ReqAccountSummary( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 3 ) ) {
        return scope.Close( Undefined() );
    }

    int reqId = args[0]->Int32Value();
    IBString groupName = getChar( args[1] );
    IBString tags = getChar( args[2] );
    obj->m_client.reqAccountSummary( reqId, groupName, tags );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::CancelAccountSummary( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    int reqId = args[0]->Int32Value();
    obj->m_client.cancelAccountSummary( reqId );

    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::VerifyRequest( const Arguments& args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 2 ) ) {
        return scope.Close( Undefined() );
    }

    if ( isWrongType( !args[0]->IsString(), 0) ||
         isWrongType( !args[1]->IsString(), 1) ) {
        return scope.Close( Undefined() );
    }

    IBString apiName = getChar( args[0] );
    IBString apiVersion = getChar( args[1] );

    obj->m_client.verifyRequest( apiName, apiVersion );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::VerifyMessage( const Arguments& args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );
    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    if ( isWrongType( !args[0]->IsString(), 0 ) ) {
        return scope.Close( Undefined() );
    }

    IBString apiData = getChar( args[0] );

    obj->m_client.verifyMessage( apiData );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::QueryDisplayGroups( const Arguments& args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    if ( isWrongType( !args[0]->IsInt32(), 0 ) ) {
        return scope.Close( Undefined() );
    }

    int reqId = args[0]->Int32Value();

    obj->m_client.queryDisplayGroups( reqId );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::SubscribeToGroupEvents( const Arguments& args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 2 ) ) {
        return scope.Close( Undefined() );
    }

    if ( isWrongType( !args[0]->IsInt32(), 0 ) ||
         isWrongType( !args[1]->IsInt32(), 1 ) ) {
        return scope.Close( Undefined() );
    }

    int reqId = args[0]->Int32Value();
    int groupId = args[1]->Int32Value();

    obj->m_client.subscribeToGroupEvents( reqId, groupId );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::UpdateDisplayGroup( const Arguments& args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 2 ) ) {
        return scope.Close( Undefined() );
    }

    if ( isWrongType( !args[0]->IsInt32(), 0 ) ||
         isWrongType( !args[1]->IsString(), 1 ) ) {
        return scope.Close( Undefined() );
    }

    int reqId = args[0]->Int32Value();
    IBString contractInfo = getChar( args[1] );

    obj->m_client.updateDisplayGroup( reqId, contractInfo );
    return scope.Close( Undefined() );
}
Handle<Value> NodeIbapi::UnsubscribeFromGroupEvents( const Arguments& args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    if ( isWrongArgNumber( args, 1 ) ) {
        return scope.Close( Undefined() );
    }

    if ( isWrongType( !args[0]->IsInt32(), 0 ) ) {
        return scope.Close( Undefined() );
    }

    int reqId = args[0]->Int32Value();

    obj->m_client.unsubscribeFromGroupEvents( reqId );
    return scope.Close( Undefined() );
}


///////////////////////////////////////////////////////////////////////////////
//  Getters
///////////////////////////////////////////////////////////////////////////////

Handle<Value> NodeIbapi::GetInboundMsg( const Arguments &args ) {
    HandleScope scope;
    NodeIbapi* obj = ObjectWrap::Unwrap<NodeIbapi>( args.This() );

    JSONNode newMsg;
    newMsg = obj->m_client.getInboundMsg();

    Handle<Object> retData = Object::New();
    retData = obj->m_parser.parse( newMsg );

    return scope.Close( retData );
}

///////////////////////////////////////////////////////////////////////////////
//  Helper methods
///////////////////////////////////////////////////////////////////////////////
// see http://stackoverflow.com/questions/10507323/
//  shortest-way-one-liner-to-get-a-default-argument-out-of-a-v8-function
char *NodeIbapi::getChar( v8::Local<v8::Value> value, const char *fallback ) {
    if ( value->IsString() ) {
        v8::String::AsciiValue string( value );
        char *str  = ( char * ) malloc( string.length() + 1 );
        std::strcpy( str, *string );
        return str;
    }
    char *str = ( char * ) malloc( std::strlen( fallback ) + 1 );
    std::strcpy( str, fallback );
    return str;
}

bool NodeIbapi::isWrongArgNumber( const Arguments &args, int argNum ) {
    if ( args.Length() != argNum ) {
        ThrowException(
            Exception::TypeError(
                String::New( "Wrong number of arguments" ) ) );
        return true;
    }
    return false;
}

bool NodeIbapi::isWrongType( bool predicateRes, int argId ) {
    if ( predicateRes ) {
        std::ostringstream ss;
        ss << "Argument " << argId << " is of wrong type.";
        ThrowException(
            Exception::TypeError(
                String::New( ss.str().c_str() ) ) );
        return true;
    } 
    return false;
}

void NodeIbapi::convertExecutionFilterForIb( Handle<Object> ibExecutionFilter,
                                             ExecutionFilter &filter) {

    filter.m_clientId = ibExecutionFilter->Get(
            String::New( "clientId") )->Int32Value();
    filter.m_acctCode = getChar(
            ibExecutionFilter->Get( String::New( "acctCode") ) );
    filter.m_time = getChar(
            ibExecutionFilter->Get( String::New( "time") ) );
    filter.m_symbol = getChar(
            ibExecutionFilter->Get( String::New( "symbol") ) );
    filter.m_secType = getChar(
            ibExecutionFilter->Get( String::New( "secType") ) );
    filter.m_exchange = getChar(
            ibExecutionFilter->Get( String::New( "exchange") ) );
    filter.m_side = getChar(
            ibExecutionFilter->Get( String::New( "side") ) );
}

void NodeIbapi::convertContractForIb( Handle<Object> ibContract,
                                      Contract &contract ) {
    // checks if order is being submitted through Conract ID from
    //  contract specification
    contract.conId = ibContract->Get( String::New( "conId" ) )->Int32Value();
    contract.exchange =
        getChar( ibContract->Get( String::New( "exchange" ) ) );

    contract.symbol = getChar( ibContract->Get( String::New( "symbol" ) ) );
    contract.secType = getChar( ibContract->Get( String::New( "secType" ) ) );
    contract.expiry = getChar( ibContract->Get( String::New( "expiry" ) ) );
    contract.strike = ibContract->Get( String::New( "strike" ) )->NumberValue();
    contract.right = getChar( ibContract->Get( String::New( "right" ) ) );
    contract.multiplier =
        getChar( ibContract->Get( String::New( "multiplier" ) ) );
    contract.primaryExchange =
        getChar( ibContract->Get( String::New( "primaryExchange" ) ) );
    contract.currency = getChar( ibContract->Get( String::New( "currency" ) ) );
    contract.localSymbol =
        getChar( ibContract->Get( String::New( "localSymbol" ) ) );
    contract.tradingClass =
        getChar( ibContract->Get( String::New( "tradingClass" ) ) );
    contract.includeExpired =
        ibContract->Get( String::New( "includeExpired" ) )->BooleanValue();
    contract.secIdType =
        getChar( ibContract->Get( String::New( "secIdType" ) ) );
    contract.secId = getChar( ibContract->Get( String::New( "secId" ) ) );
}

void NodeIbapi::convertSubForIb( Handle<Object> scannerSub,
                                 ScannerSubscription &subscription ) {

    subscription.numberOfRows =
        scannerSub->Get( String::New( "numberOfRows" ) )->Int32Value();
    subscription.abovePrice =
        scannerSub->Get( String::New( "abovePrice" ) )->NumberValue();
    subscription.belowPrice =
        scannerSub->Get( String::New( "belowPrice" ) )->NumberValue();
    subscription.aboveVolume =
        scannerSub->Get( String::New( "aboveVolume" ) )->Int32Value();
    subscription.marketCapAbove =
        scannerSub->Get( String::New( "marketCapAbove" ) )->NumberValue();
    subscription.marketCapBelow =
        scannerSub->Get( String::New( "marketCapBelow" ) )->NumberValue();
    subscription.couponRateAbove =
        scannerSub->Get( String::New( "couponRateAbove" ) )->NumberValue();
    subscription.couponRateBelow =
        scannerSub->Get( String::New( "couponRateBelow" ) )->NumberValue();

    subscription.instrument =
        getChar( scannerSub->Get( String::New( "instrument" ) ) );
    subscription.locationCode =
        getChar( scannerSub->Get( String::New( "locationCode" ) ) );
    subscription.scanCode =
        getChar( scannerSub->Get( String::New( "scanCode" ) ) );
    subscription.moodyRatingAbove =
        getChar( scannerSub->Get( String::New( "moodyRatingAbove" ) ) );
    subscription.moodyRatingBelow =
        getChar( scannerSub->Get( String::New( "moodyRatingBelow" ) ) );
    subscription.spRatingAbove =
        getChar( scannerSub->Get( String::New( "spRatingAbove" ) ) );
    subscription.spRatingBelow =
        getChar( scannerSub->Get( String::New( "spRatingBelow" ) ) );
    subscription.maturityDateAbove =
        getChar( scannerSub->Get( String::New( "maturityDateAbove" ) ) );
    subscription.maturityDateBelow =
        getChar( scannerSub->Get( String::New( "maturityDateBelow" ) ) );
    subscription.excludeConvertible =
        scannerSub->Get( String::New( "excludeConvertible" ) )->Int32Value();
    subscription.averageOptionVolumeAbove =
        scannerSub->Get(
            String::New( "averageOptionVolumeAbove" ) )->Int32Value();
    subscription.scannerSettingPairs =
        getChar( scannerSub->Get( String::New( "scannerSettingPairs" ) ) );
    subscription.stockTypeFilter =
        getChar( scannerSub->Get( String::New( "stockTypeFilter" ) ) );
}

void NodeIbapi::convertOrderForIb( Handle<Object> ibOrder, Order &order ) {
    // order identifier
    order.orderId = ibOrder->Get( String::New( "orderId" ) )->Int32Value();
    order.clientId = ibOrder->Get( String::New( "clientId" ) )->Int32Value();
    order.permId = ibOrder->Get( String::New( "permId" ) )->Int32Value();

    // main order fields
    order.action = getChar( ibOrder->Get( String::New( "action" ) ) );
    order.totalQuantity =
        ibOrder->Get( String::New( "totalQuantity" ) )->Int32Value();
    order.orderType = getChar( ibOrder->Get( String::New( "orderType" ) ) );
    order.lmtPrice = ibOrder->Get( String::New( "lmtPrice" ) )->NumberValue();
    order.auxPrice = ibOrder->Get( String::New( "auxPrice" ) )->NumberValue();

    // extended order fields
    order.tif = getChar( ibOrder->Get( String::New( "tif" ) ) );
    order.ocaGroup = getChar( ibOrder->Get( String::New( "ocaGroup" ) ) );
    order.ocaType = ibOrder->Get( String::New( "ocaType" ) )->Int32Value();
    order.orderRef = getChar( ibOrder->Get( String::New( "orderRef" ) ) );
    order.transmit = ibOrder->Get( String::New( "transmit" ) )->BooleanValue();
    order.parentId = ibOrder->Get( String::New( "parentId" ) )->Int32Value();
    order.blockOrder =
        ibOrder->Get( String::New( "blockOrder" ) )->BooleanValue();
    order.sweepToFill =
        ibOrder->Get( String::New( "sweepToFill" ) )->BooleanValue();
    order.displaySize =
        ibOrder->Get( String::New( "displaySize" ) )->Int32Value();
    order.triggerMethod =
        ibOrder->Get( String::New( "triggerMethod" ) )->Int32Value();
    order.outsideRth =
        ibOrder->Get( String::New( "outsideRth" ) )->BooleanValue();
    order.hidden = ibOrder->Get( String::New( "hidden" ) )->BooleanValue();
    order.goodAfterTime =
        getChar( ibOrder->Get( String::New( "goodAfterTime" ) ) );
    order.goodTillDate =
        getChar( ibOrder->Get( String::New( "goodTillDate" ) ) );
    order.rule80A = getChar( ibOrder->Get( String::New( "rule80A" ) ) );
    order.allOrNone =
        ibOrder->Get( String::New( "allOrNone" ) )->BooleanValue();
    order.minQty = ibOrder->Get( String::New( "minQty" ) )->Int32Value();
    order.percentOffset =
        ibOrder->Get( String::New( "percentOffset" ) )->NumberValue();
    order.overridePercentageConstraints =
        ibOrder->Get(
            String::New( "overridePercentageConstraints" ) )->BooleanValue();
    order.trailStopPrice =
        ibOrder->Get( String::New( "trailStopPrice" ) )->NumberValue();
    order.trailingPercent =
        ibOrder->Get( String::New( "trailingPercent" ) )->NumberValue();

    // financial advisors only

    // institutional (ie non-cleared) only

    // SMART routing only
    order.discretionaryAmt =
        ibOrder->Get( String::New( "discretionaryAmt" ) )->NumberValue();
    order.eTradeOnly =
        ibOrder->Get( String::New( "eTradeOnly" ) )->BooleanValue();
    order.firmQuoteOnly =
        ibOrder->Get( String::New( "firmQuoteOnly" ) )->BooleanValue();
    order.nbboPriceCap =
        ibOrder->Get( String::New( "nbboPriceCap" ) )->NumberValue();
    order.optOutSmartRouting =
        ibOrder->Get( String::New( "optOutSmartRouting" ) )->BooleanValue();

    // BOX exchange orders only
    order.auctionStrategy =
        ibOrder->Get( String::New( "auctionStrategy" ) )->Int32Value();
    order.startingPrice =
        ibOrder->Get( String::New( "startingPrice" ) )->NumberValue();
    order.stockRefPrice =
        ibOrder->Get( String::New( "stockRefPrice" ) )->NumberValue();
    order.delta =
        ibOrder->Get( String::New( "delta" ) )->NumberValue();

    // pegged to stock and VOL orders only
    order.stockRangeLower =
        ibOrder->Get( String::New( "stockRangeLower" ) )->NumberValue();
    order.stockRangeUpper =
        ibOrder->Get( String::New( "stockRangeUpper" ) )->NumberValue();

    // VOLATILITY ORDERS ONLY
    order.volatility =
        ibOrder->Get( String::New( "volatility" ) )->NumberValue();
    order.volatilityType =
        ibOrder->Get( String::New( "volatilityType" ) )->Int32Value();
    order.deltaNeutralOrderType =
        getChar( ibOrder->Get( String::New( "deltaNeutralOrderType" ) ) );
    order.deltaNeutralAuxPrice =
        ibOrder->Get( String::New( "deltaNeutralAuxPrice" ) )->NumberValue();
    order.deltaNeutralConId =
        ibOrder->Get( String::New( "deltaNeutralConId" ) )->Int32Value();
    order.deltaNeutralSettlingFirm =
        getChar( ibOrder->Get( String::New( "deltaNeutralSettlingFirm" ) ) );
    order.deltaNeutralClearingAccount =
        getChar( ibOrder->Get( String::New( "deltaNeutralClearingAccount" ) ) );
    order.deltaNeutralClearingIntent =
        getChar( ibOrder->Get( String::New( "deltaNeutralClearingIntent" ) ) );
    order.deltaNeutralOpenClose =
        getChar( ibOrder->Get( String::New( "deltaNeutralOpenClose" ) ) );
    order.deltaNeutralShortSale =
        ibOrder->Get( String::New( "deltaNeutralShortSale" ) )->BooleanValue();
    order.deltaNeutralShortSaleSlot =
        ibOrder->Get( String::New( "deltaNeutralShortSaleSlot" ) )->Int32Value();
    order.deltaNeutralDesignatedLocation =
        getChar( ibOrder->Get(
            String::New( "deltaNeutralDesignatedLocation" ) ) );
    order.continuousUpdate =
        ibOrder->Get( String::New( "continuousUpdate" ) )->BooleanValue();
    order.referencePriceType =
        ibOrder->Get( String::New( "referencePriceType" ) )->Int32Value();

    // COMBO ORDERS ONLY

    // SCALE ORDERS ONLY
    order.scaleInitLevelSize =
        ibOrder->Get( String::New( "scaleInitLevelSize" ) )->Int32Value();
    order.scaleSubsLevelSize =
        ibOrder->Get( String::New( "scaleSubsLevelSize" ) )->Int32Value();
    order.scalePriceIncrement =
        ibOrder->Get( String::New( "scalePriceIncrement" ) )->NumberValue();
    order.scalePriceAdjustValue =
        ibOrder->Get( String::New( "scalePriceAdjustValue" ) )->NumberValue();
    order.scalePriceAdjustInterval =
        ibOrder->Get( String::New( "scalePriceAdjustInterval" ) )->Int32Value();
    order.scaleProfitOffset =
        ibOrder->Get( String::New( "scaleProfitOffset" ) )->NumberValue();
    order.scaleAutoReset =
        ibOrder->Get( String::New( "scaleAutoReset" ) )->BooleanValue();
    order.scaleInitPosition =
        ibOrder->Get( String::New( "scaleInitPosition" ) )->Int32Value();
    order.scaleInitFillQty =
        ibOrder->Get( String::New( "scaleInitFillQty" ) )->Int32Value();
    order.scaleRandomPercent =
        ibOrder->Get( String::New( "scaleRandomPercent" ) )->BooleanValue();

    // HEDGE ORDERS
    order.hedgeType = getChar( ibOrder->Get( String::New( "hedgeType" ) ) );
    order.hedgeParam = getChar( ibOrder->Get( String::New( "hedgeParam" ) ) );

    // Clearing info
    order.account = getChar( ibOrder->Get( String::New( "account" ) ) );
    order.settlingFirm =
        getChar( ibOrder->Get( String::New( "settlingFirm" ) ) );
    order.clearingAccount =
        getChar( ibOrder->Get( String::New( "clearingAccount" ) ) );
    order.clearingIntent =
        getChar( ibOrder->Get( String::New( "clearingIntent" ) ) );

    // ALGO ORDERS ONLY

    // What-if

    // Not Held
}
