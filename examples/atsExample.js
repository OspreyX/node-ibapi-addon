// In this example, we will implement a simple currency trading application
//  with node-ibapi-addon and kefir, a functional reactive programming module.
// Let's say that you wanted to make an ATS that buys eur/usd on three 
//  consecutive down ticks, and sell it on three consecutive up ticks.

var addon = require('../ibapi'),
  messageIds = addon.messageIds,
  contract = addon.contract,
  order = addon.order;
var Kefir = require('kefir').Kefir;
var api = new addon.NodeIbapi();
var orderId = -1;

var eurusd = contract.createContract();
eurusd.symbol = 'EUR';
eurusd.secType = 'CASH';
eurusd.exchange = 'IDEALPRO';
eurusd.primaryExchange = 'IDEALPRO';
eurusd.currency = 'USD';

var buyOrder = function (data) {
  console.log("Buying EUR");
  api.placeOrder(orderId, eurusd, "BUY", 100, "MKT", 0.0, 0.0);
  orderId = orderId + 1;
};
var sellOrder = function (data) {
  console.log("Selling EUR");
  api.placeOrder(orderId, eurusd, "SELL", 100, "MKT", 1000.0, 0.0);
  orderId = orderId + 1;
};
var handleValidOrderId = function (data) {
  orderId = data.orderId;
  console.log('nextValidId: ' + orderId);
  api.reqMktData(1,eurusd,"165",false);
};
var printTickPrice = function (tickPrice) {
  console.log( "TickPrice: " + tickPrice.tickerId.toString() + " " + 
    tickPrice.field.toString() + " " + tickPrice.price.toString() + " " +
    tickPrice.canAutoExecute.toString());
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

// Let's use kefir to compose the event handler functions
var kemitter = Kefir.emitter();

// In order to do that, register all incoming messages to use kefir's emitter
var ibapiCallback = function (data) {
  kemitter.emit(data);
};
Object.keys(messageIds).forEach(function (messageId) {
  api.handlers[messageId] = ibapiCallback;
});

//  Let's now think about how will we handle the data stream from IB:
//
//  [IB Data Stream]
//       |
//       |-->[nextValidId]-->(subscribe to eurusd)
//       |
//       |-->[svrError]-->(print it)
//       |           
//       |-->[tickPrices]-->(print it)
//                 |
//                 |-->[last three prices]
//                              |
//                              |-->(isBuySignal?)-->(buy)
//                              |
//                              |-->(isSellSignal?)-->(sell)
//

// Now, we should write some predicates for data filtering:
var isNextValidId = function (data) {
  return data.messageId === messageIds.nextValidId;
};
var isServerError = function (data) {
  return data.messageId === messageIds.svrError;
};
var isTickPrice = function (data) {
  return data.messageId === messageIds.tickPrice;
};
var isBuySignal = function (buf) {
  return (buf[2] < buf[1] && buf[1] < buf[0]);
};
var isSellSignal = function (buf) {
  return (buf[2] > buf[1] && buf[1] > buf[0]);
};

// Observable for handling nextValidId - should be done only once
var nextValidIdObs = kemitter.filter(isNextValidId);
nextValidIdObs.onValue(handleValidOrderId);

// Observable for reading tickPrices - we can further manipulate this stream 
var tickObs = kemitter.filter(isTickPrice);
tickObs.onValue(printTickPrice);

// Observable for handling server error
var svrErrorObs = kemitter.filter(isServerError);
svrErrorObs.onValue(handleServerError);

// Finally, we use kefir's slidingWindow to create a length 3 buffer to trade
var tradeSignal = tickObs.slidingWindow(3);
tradeSignal.filter(isBuySignal).onValue(buyOrder);
tradeSignal.filter(isSellSignal).onValue(sellOrder);

var connected = api.connect('127.0.0.1',7496,0);

if (connected) {
  api.beginProcessing();
};
