// In this example, we will request and receive a historical market data
// This file shows you one example of a barebones program that handles
//  server error messages.
var addon = require('../ibapi'),
  messageIds = addon.messageIds,
  contract = addon.contract,
  order = addon.order;

var api = new addon.NodeIbapi();
var orderId = -1;

// Let's create a IB complient contract using the library function
//  See contract.js in the /lib directory for details
var msftContract = contract.createContract();
msftContract.symbol = 'MSFT';
msftContract.secType = 'STK';
msftContract.exchange = 'SMART';
msftContract.primaryExchange = 'NASDAQ';
msftContract.currency = 'USD';

var subscribeMsft = function () {
  // Here we bind the request function to API so that it can take advantage of 
  //  the async facility and the rateLimiter. However, since we are making only
  //  one request in this example, you could easily just instead call:
  //  api.reqHistoricalData(<params>)
  setImmediate(
    api.reqHistoricalData.bind(api, 1, msftContract, "20131001 00:00:00",
      "10 D", "1 hour", "MIDPOINT", "1", "1"));
};

var handleValidOrderId = function (message) {
  orderId = message.orderId;
  console.log('next order Id is ' + orderId);
  subscribeMsft();
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

var handleHistData = function (data) {
  if (data.date.toString().indexOf("finished") < 0) {
    console.log( data.date + ' - ' + data.open + ' - ' + data.high + ' - ' +
                 data.low + ' - ' + data.close + ' - ' + data.volume + ' - ' +
                 data.barCount + ' - ' + data.WAP + ' - ' + data.hasGaps
    );
  }
  else {
    console.log('End of Historical Data');
  }
  // Or you can just stringify it:
  //  console.log(JSON.stringify(histData));
};

api.handlers[messageIds.nextValidId] = handleValidOrderId;
api.handlers[messageIds.svrError] = handleServerError;
api.handlers[messageIds.clientError] = handleClientError;
api.handlers[messageIds.historicalData] = handleHistData;

var connected = api.connect('127.0.0.1', 7496, 0);

if (connected) {
  api.beginProcessing();
}
