node-ibapi-addon
================

Interactive Brokers API addon for Node.js compatible with IB API 9.71

This addon uses the latest stable Interactive Brokers POSIX C++ API.

Author: Jae Yang - [dchem] (https://github.com/dchem/)

For direct JavaScript implementation of IB API for Node.js, please visit 
Pilwon Huh's [node-ib] (https://github.com/pilwon/node-ib).

[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/dchem/node-ibapi-addon?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

### Important
####Note:
The inbound messages' are in the order it was received. Also, the outbound 
messages are rate limited to 50 msg/sec as IB requires it.
Default behavior is that if your outbound rate is greater than that the 
51st message will be handled in the next second.

####WARNING:
If you have legacy code from before 0.2.0, use 0.1.23 if you don't want to 
migrate your code.

### Change Notes:

* 2015-02-15 - 0.2.3  - Code cleanup and updated examples
* 2015-01-07 - 0.2.2  - More CPU efficient
* 2014-12-29 - 0.2.1  - Uses event handlers instead of event emitters
* 2014-11-12 - 0.1.23 - Includes lib into the package itself
* 2014-09-10 - 0.1.21 - Supports API 9.71
* 2014-09-09 - 0.1.19 - Adds order.js and placeOrder can use order obj
* 2014-04-22 - 0.1.17 - Compatibility fix for API 9.70
* 2014-03-17 - 0.1.13 - Smoother installation to multiple OSes
* 2014-01-17 - 0.1.0  - all EWrapper events bound in ibapi.js
* 2013-09-02 - 0.0.1  - Initial commit

### Installation dependency
* Install Python 2.7
* Install node-gyp
```
npm install node-gyp -g
```
* Install unzip if Linux
```
sudo apt-get install unzip
```

### Additional installation dependency for Windows:
* Install MinGW
* Install msys-unzip instead of unzip
* Install msys-wget
```
mingw-get install msys-unzip
mingw-get install msys-wget
```
* Install Microsoft Visual Studio

### Installation from NPM:
```
npm install ibapi
```

For Windows with MSVS 2013:
```
npm install ibapi --msvs_version=2013
```

For Windows with MSVS 2012:
```
npm install ibapi --msvs_version=2012
```
Alternatively, include GYP_MSVS_VERSION=2012 or GYP_MSVS_VERSION=2013 in 
environment variables for Windows.

### Additional installation dependency for OS X:
* Install xcode command line tools
* Install homebrew
* Install wget through homebrew

### Installation from git repo:

1. Make sure to have node-gyp installed
2. Run the preinstall.sh which does the following:
 * Download the latest stable IB API into /import
 * Copy the contents of IB API package's 
    IBJts/source/PosixClient/Shared into /import directory
 * Copy the contents of IB API package's
    IBJts/source/PosixClient/src into /import directory
 * Add #define IB_USE_STD_STRING into the following files in /import directory:
    EClientSocketBase.cpp
    EPosixClientSocket.cpp
 * Downloads libjson 7.6.1
3. ```node-gyp rebuild```
4. If build fails because you have VS2012, use ```node-gyp --msvs_version=2012 rebuild```

### Usage
1. Require ibapi
2. Create event handlers
3. Register event handlers for messageIds
4. Invoke connect()
5. ...
6. Profit!

```js
// This file shows you one example of a barebones program that handles
//  server error messages.

// Required package name is 'ibapi' If you use your own project path,
//  just use require('ibapi') from your project root as you would 
//  normally do.
var addon = require('../ibapi'),
  messageIds = addon.messageIds,
  contract = addon.contract,
  order = addon.order;

// The api object handles the client methods. For details, refer to 
//  IB API documentation.
var api = new addon.NodeIbapi();

// Interactive Broker requires that you use orderId for every new order
//  inputted. The orderId is incremented everytime you submit an order.
//  Make sure you keep track of this.
var orderId = -1;

// Here we specify the event handlers.
//  Please follow this guideline for event handlers:
//  1. Add handlers to listen to messages
//  2. Each handler must have be a function (message) signature
var handleValidOrderId = function (message) {
  orderId = message.orderId;
  console.log('next order Id is ' + orderId);
};

var handleServerError = function (message) {
  console.log('Error: ' + message.id.toString() + '-' +
              message.errorCode.toString() + '-' +
              message.errorString.toString());
};

var handleClientError = function (message) {
  console.log('clientError');
  console.log(JSON.stringify(message));
};

var handleDisconnected = function (message) {
  console.log('disconnected');
  process.exit(1);
};

// After that, you must register the event handler with a messageId
//  For list of valid messageIds, see messageIds.js file.
api.handlers[messageIds.nextValidId] = handleValidOrderId;
api.handlers[messageIds.svrError] = handleServerError;
api.handlers[messageIds.clientError] = handleClientError;
api.handlers[messageIds.disconnected] = handleDisconnected;

// Connect to the TWS client or IB Gateway
var connected = api.connect('127.0.0.1', 7496, 0);

// Once connected, start processing incoming and outgoing messages
if (connected) {
  api.beginProcessing();
}
```
### Included libraries
These are used as state stores and abstraction for IB API. You also have to 
pass instance of the object, such as 'order' to some of the client request 
functions.

Each of theese come with a constructor:
* order
* contract
* execution
* scannerSubscription
* contractDetails

### Addon methods
* ```beginProcessing()```: Entry point - starts the message processing.
* ```doAction(action)```: An action, custom function or API requests, can be 
  bound to each events
* ```placeSimpleOrder()```: Simpler order input method.

### IB API Client Methods
```processMsg()```: Msg processor - a POSIX implementation of network message 
handler. This does not need to be called explicitly by the user

### IB API Client Requests
Following commands are used for requesting specific action through IB API:
```js
.connect(host,port,clientId)
.disconnect()
.isConnected()
.serverVersion() // returns right away
.twsConnectionTime() // returns right away
.reqMktData(reqId, contract, genericTickType, snapShot)
.cancelMktData(reqId)
// placeOrder can take either 
.placeOrder(orderId, contrct, order)
// or
.placeOrder(orderId, contract, action, quantity, orderType, price, auxPrice)
.cancelOrder(orderId)
.reqOpenOrders()
.reqAccountUpdates(subscribe, acctCode)
.reqExecutions(reqId, clientId, acctCode, time, symbol, secType, exchange, side)
.reqIds(num) // only accepts 1 for the time being (9.71)
.checkMessages()
.reqContractDetails(reqId, contract)
.reqMktDepth(tickerId, contract, numRows ) 
.cancelMktDepth(tickerId)
.reqNewsBulletins(allMsgs)
.cancelNewsBulletins()
.setServerLogLevel(level)
.reqAutoOpenOrders(bAutoBind)
.reqAllOpenOrders()
.reqManagedAccts()
.requestFA( ) // not yet implemented
.replaceFA( ) // not yet implemented
.reqHistoricalData(id, contract, endDateTime, durationStr, barSizeSetting, whatToShow, useRTH, formatDate)
.exerciseOptions(tickerId, contract, exerciseAction, exerciseQuantity, account, override )
.cancelHistoricalData(tickerId)
.reqRealtimeBars(tickerId, contract, barSize, whatToShow, useRTH)
.cancelRealTimeBars(tickerId)
.cancelScannerSubscription(tickerId)
.reqScannerParameters()
.reqScannerSubscription(tickerId, subscription)
.reqCurrentTime() // not implemented
.reqFundamentalData( reqId, contract, reportType )
.cancelFundamentalData(reqId)
.calculateImpliedVolatility( reqId, contract, optionPrice, underPrice )
.calculateOptionPrice( reqId, contract, volatility, underPrice )
.cancelCalculateImpliedVolatility(reqId)
.cancelCalculateOptionPrice(reqId)
.reqGlobalCancel()
.reqMarketDataType(marketDataType)
.reqPositions()
.cancelPositions()
.reqAccountSummary( reqId, groupName, tags )
.cancelAccountSummary(reqId)
.verifyRequest( apiName, apiVersion )
.verifyMessage( apiData )
.queryDisplayGroups( reqId )
.subscribeToGroupEvents( reqId, groupId )
.updateDisplayGroup( reqId, contractInfo )
.unsubscribeFromGroupEvents( reqId )
```

### EWrapper Events
See messageIds.js
```
tickPrice
tickSize
tickOptionComputation
tickGeneric
tickString
tickEFP
orderStatus
openOrder
openOrderEnd
clientError
connectionClosed
updateAccountValue
updatePortfolio
updateAccountTime
accountDownloadEnd
nextValidId
contractDetails
bondContractDetails
contractDetailsEnd
execDetails
execDetailsEnd
svrError
updateMktDepth
updateMktDepthL2
updateNewsBulletin
managedAccounts
receiveFA
historicalData
scannerParameters
scannerData
scannerDataEnd
realtimeBar
fundamentalData
deltaNeutralValidation
tickSnapshotEnd
marketDataType
commissionReport
position
positionEnd
accountSummary
accountSummaryEnd
verifyMessageAPI
verifyCompleted
displayGroupList
displayGroupUpdated
nextValidId
disconnected
```

### Tests:
Uses mocha, so install it.

### Issues:
* See issues list in https://github.com/dchem/node-ibapi-addon/issues?state=open

### License
Copyright (c) 2014 Jae Yang. See LICENSE file for license rights and limitations (MIT).
