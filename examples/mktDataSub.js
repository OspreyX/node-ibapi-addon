// In this example, we will try to submit a market data subscription
//  to EUR/USD and get the TickPrice and TickSize events from the
//  IB Cash Data Server
// Also, in this example we will submit a market data subscription 
//  to MSFT.
// 

var addon = require('../ibapi');
var obj = new addon.NodeIbapi();

var ibcontract = require('../lib/contract');

var processIbMsg = function () {
  obj.processIbMsg();
}
var doReqFunc = function () {
  obj.doReqFunc();
}

var eurusd = ibcontract.createContract();
eurusd.symbol = 'EUR';
eurusd.secType = 'CASH';
eurusd.exchange = 'IDEALPRO';
eurusd.primaryExchange = 'IDEALPRO';
eurusd.currency = 'USD';

var msftContract = ibcontract.createContract();
msftContract.symbol = 'MSFT';
msftContract.secType = 'STK';
msftContract.exchange = 'SMART';
msftContract.primaryExchange = 'NASDAQ';
msftContract.currency = 'USD';

var subscribeEurUsd = function () {
  obj.reqMktData(1,eurusd,"165",false);
}
var subscribeMsft = function () {
  obj.reqMktData(3,msftContract,"165",false);
}

obj.on('connected', function () {
  console.log('connected');
  setInterval(processIbMsg,0.1);
  obj.funcQueue.push(subscribeEurUsd);
  obj.funcQueue.push(subscribeMsft);
})
.once('nextValidId', function (data) {
  orderId = data;
  setInterval(doReqFunc,100);
})
.on('tickPrice', function (data) {
  console.log( "TickPrice: " + data[0].toString() + " " + 
    data[1].toString() + " " + data[2].toString());
})
.on('tickSize', function (data) {
  console.log( "TickSize: " + data[0].toString() + " " + 
    data[1].toString() + " " + data[2].toString());
})
.on('clientError', function (data) {
  console.log('Client error' + data[1].toString());
})
.on('srvError', function (data) {
  console.log('Error: ' + data[0].toString() + ' - ' + 
    data[1].toString() + ' - ' + data[2].toString());
})
.on('disconnected', function () {
  console.log('disconnected');
  process.exit(1);
})

obj.connectToIb('127.0.0.1',7496,0);
