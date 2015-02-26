#include "IbJsonParser.h"
#include "import/libjson/libjson.h"

// value can be string or number -> in IbPosixClient, they are disambibuated as
//    valueDouble: from tickGeneric
//  valueString: from tickString and others
// time can be string or integer -> in IbPosixClient, they are disambiguated as
//    timeLong: from realtimeBar and currentTime
//    timeString: from execution

IbJsonParser::IbJsonParser()
{
}
IbJsonParser::~IbJsonParser()
{
}

Handle<Object> IbJsonParser::parse( const JSONNode& n ) {
    Isolate* isolate = Isolate::GetCurrent();
    Handle<Object> retData = Object::New(isolate);
    JSONNode::const_iterator i = n.begin();
    while ( i != n.end() ) {
        std::string node_name = i->name();
        // Let if hell begin:
        if ( node_name == "orderState" || node_name == "underComp" ||
             node_name == "commissionReport" || node_name == "contract" ||
             node_name == "order" || node_name == "execution" ||
             node_name == "summary" || node_name == "contractDetails" ) {
            retData->Set( String::NewFromUtf8( isolate, node_name.c_str() ), parse( *i ) );
        }
        else if ( filterAsString( node_name ) ) {
            retData->Set( String::NewFromUtf8( isolate, node_name.c_str() ),
                          String::NewFromUtf8( isolate, i->as_string().c_str() ) );
        }
        else if ( filterAsNumber( node_name ) ) {
            retData->Set( String::NewFromUtf8( isolate, node_name.c_str() ),
                          Number::New( isolate, i->as_float() ) );
        }
        else if ( filterAsInteger( node_name ) ) {
            retData->Set( String::NewFromUtf8( isolate, node_name.c_str() ),
                          Integer::New( isolate, i->as_int() ) );
        }
        else if ( filterAsBoolean( node_name ) ) {
            retData->Set( String::NewFromUtf8( isolate, node_name.c_str() ),
                          Boolean::New( isolate, i->as_bool() ) );
        }
        ++i;
    }
    return retData;
}

bool IbJsonParser::filterAsString( const std::string& node_name ) {
    if ( node_name == "messageId" ||
         node_name == "formattedBasisPoints" ||
         node_name == "futureExpiry" ||
         node_name == "status" ||
         node_name == "whyHeld" ||
         node_name == "initMargin" ||
         node_name == "maintMargin" ||
         node_name == "equityWithLoan" ||
         node_name == "commissionCurrency" ||
         node_name == "warningText" ||
         node_name == "str" ||
         node_name == "key" ||
         node_name == "val" ||
         node_name == "currency" ||
         node_name == "accountName" ||
         node_name == "timeStamp" ||
         node_name == "errorString" ||
         node_name == "marketMaker" ||
         node_name == "newsMessage" ||
         node_name == "originExch" ||
         node_name == "accountsList" ||
         node_name == "date" ||
         node_name == "xml" ||
         node_name == "distance" ||
         node_name == "benchmark" ||
         node_name == "projection" ||
         node_name == "legsStr" ||
         node_name == "data" ||
         node_name == "execId" ||
         node_name == "account" ||
         node_name == "tag" ||
         node_name == "curency" ||
         node_name == "apiData" ||
         node_name == "errorText" ||
         node_name == "groups" ||
         node_name == "contractInfo" ||
         node_name == "valueString" ||
         node_name == "symbol" ||
         node_name == "secType" ||
         node_name == "expiry" ||
         node_name == "right" ||
         node_name == "multiplier" ||
         node_name == "exchange" ||
         node_name == "primaryExchange" ||
         node_name == "localSymbol" ||
         node_name == "tradingClass" ||
         node_name == "secIdType" ||
         node_name == "secId" ||
         node_name == "marketName" ||
         node_name == "orderTypes" ||
         node_name == "validExchanges" ||
         node_name == "longName" ||
         node_name == "contractMonth" ||
         node_name == "industry" ||
         node_name == "category" ||
         node_name == "subcategory" ||
         node_name == "timeZoneId" ||
         node_name == "tradingHours" ||
         node_name == "liquidHours" ||
         node_name == "evRule" ||
         node_name == "cusip" ||
         node_name == "ratings" ||
         node_name == "descAppend" ||
         node_name == "bondType" ||
         node_name == "couponType" ||
         node_name == "maturity" ||
         node_name == "issueDate" ||
         node_name == "nextOptionDate" ||
         node_name == "nextOptionType" ||
         node_name == "notes" ||
         node_name == "execId" ||
         node_name == "timeString" ||
         node_name == "acctNumber" ||
         node_name == "side" ||
         node_name == "orderRef" ||
         node_name == "evRule" ||
         node_name == "action" ||
         node_name == "orderType" ||
         node_name == "tif" ||
         node_name == "ocaGroup" ||
         node_name == "orderRef" ||
         node_name == "goodAfterTime" ||
         node_name == "rule80A" ||
         node_name == "deltaNeutralOrderType" ||
         node_name == "deltaNeutralSettlingFirm" ||
         node_name == "deltaNeutralClearingAccount" ||
         node_name == "deltaNeutralClearingIntent" ||
         node_name == "deltaNeutralOpenClose" ||
         node_name == "deltaNeutralDesignatedLocation" ||
         node_name == "hedgeType" ||
         node_name == "hedgeParam" ||
         node_name == "account" ||
         node_name == "settlingFirm" ||
         node_name == "clearingAccount" ||
         node_name == "clearingIntent" )
        return true;
    return false;
}
bool IbJsonParser::filterAsNumber( const std::string& node_name ) {
    if ( node_name == "price" ||
         node_name == "impliedVol" ||
         node_name == "delta" ||
         node_name == "optPrice" ||
         node_name == "pvDividend" ||
         node_name == "gamma" ||
         node_name == "vega" ||
         node_name == "theta" ||
         node_name == "undPrice" ||
         node_name == "basisPoints" ||
         node_name == "totalDividends" ||
         node_name == "dividendImpact" ||
         node_name == "dividendsToExpiry" ||
         node_name == "avgFillPrice" ||
         node_name == "lastFillPrice" ||
         node_name == "commission" ||
         node_name == "minCommission" ||
         node_name == "maxCommission" ||
         node_name == "marketPrice" ||
         node_name == "marketValue" ||
         node_name == "averageCost" ||
         node_name == "unrealizedPNL" ||
         node_name == "realizedPNL" ||
         node_name == "open" ||
         node_name == "high" ||
         node_name == "low" ||
         node_name == "close" ||
         node_name == "WAP" ||
         node_name == "wap" ||
         node_name == "yield" ||
         node_name == "avgCost" ||
         node_name == "strike" ||
         node_name == "minTick" ||
         node_name == "evMultiplier" ||
         node_name == "coupon" ||
         node_name == "avgPrice" ||
         node_name == "lmtPrice" ||
         node_name == "auxPrice" ||
         node_name == "percentOffset" ||
         node_name == "trailStopPrice" ||
         node_name == "trailingPercent" ||
         node_name == "discretionaryAmt" ||
         node_name == "nbboPriceCap" ||
         node_name == "startingPrice" ||
         node_name == "stockRefPrice" ||
         node_name == "stockRangeLower" ||
         node_name == "stockRangeUpper" ||
         node_name == "volatility" ||
         node_name == "deltaNeutralAuxPrice" ||
         node_name == "scalePriceIncrement" ||
         node_name == "scalePriceAdjustValue" ||
         node_name == "scaleProfitOffset" || 
         node_name == "valueDouble" )
        return true;
    return false;
}
bool IbJsonParser::filterAsInteger( const std::string& node_name ) {
    if ( node_name == "timeLong" ||
         node_name == "tickerId" ||
         node_name == "field" ||
         node_name == "canAutoExecute" ||
         node_name == "size" ||
         node_name == "tickType" ||
         node_name == "holdDays" ||
         node_name == "orderId" ||
         node_name == "filled" ||
         node_name == "remaining" ||
         node_name == "permId" ||
         node_name == "parentId" ||
         node_name == "clientId" ||
         node_name == "lastError" ||
         node_name == "position" ||
         node_name == "reqId" ||
         node_name == "id" ||
         node_name == "errorCode" ||
         node_name == "operation" ||
         node_name == "side" ||
         node_name == "msgId" ||
         node_name == "msgType" ||
         node_name == "volume" ||
         node_name == "barCount" ||
         node_name == "hasGaps" ||
         node_name == "rank" ||
         node_name == "count" ||
         node_name == "conId" ||
         node_name == "marketDataType" ||
         node_name == "yieldRedemptionDate"  ||
         node_name == "priceMagnifier" ||
         node_name == "underConId" ||
         node_name == "shares" ||
         node_name == "liquidation" ||
         node_name == "cumQty" ||
         node_name == "totalQuantity" ||
         node_name == "ocaType" ||
         node_name == "displaySize" ||
         node_name == "triggerMethod" ||
         node_name == "minQty" ||
         node_name == "auctionStrategy" ||
         node_name == "volatilityType" ||
         node_name == "deltaNeutralConId" ||
         node_name == "deltaNeutralShortSaleSlot" ||
         node_name == "referencePriceType" ||
         node_name == "scaleInitLevelSize" ||
         node_name == "scaleSubsLevelSize" ||
         node_name == "scalePriceAdjustInterval" ||
         node_name == "scaleInitPosition" ||
         node_name == "scaleInitFillQty" )
        return true;
    return false;
}
bool IbJsonParser::filterAsBoolean( const std::string& node_name ) {
    if ( node_name == "isSuccessful" ||
         node_name == "includeExpired" ||
         node_name == "callable" ||
         node_name == "putable" ||
         node_name == "convertible" ||
         node_name == "nextOptionPartial" ||
         node_name == "transmit" ||
         node_name == "blockOrder" ||
         node_name == "sweepToFill" ||
         node_name == "outsideRth" ||
         node_name == "hidden" ||
         node_name == "allOrNone" ||
         node_name == "overridePercentageConstraints" ||
         node_name == "eTradeOnly" ||
         node_name == "firmQuoteOnly" ||
         node_name == "optOutSmartRouting" ||
         node_name == "deltaNeutralShortSale" ||
         node_name == "continuousUpdate" ||
         node_name == "scaleAutoReset" ||
         node_name == "scaleRandomPercent"
        )
        return true;
    return false;
}
