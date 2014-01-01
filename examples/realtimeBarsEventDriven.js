// In this example, we will try to submit a realtimeBar request

var addon = require('../nodeibapi').addon;
var ibcontract = require('../lib/contract');

var obj = new addon.NodeIbapi();

var processIbMsg = function () {
  obj.processIbMsg();
}
var addReqId = function () {
  obj.addReqId(1);
}
var doReqFunc = function () {
  obj.doReqFunc();
}

var addEurUsd = function () {
  var eurusd = ibcontract.createContract();
  eurusd.symbol = 'EUR';
  eurusd.secType = 'CASH';
  eurusd.exchange = 'IDEALPRO';
  eurusd.primaryExchange = 'IDEALPRO';
  eurusd.currency = 'USD';
  obj.reqRealtimeBars(1,eurusd,5,"TRADES",false);
};

var addMsft = function () {
  var msftContract = ibcontract.createContract();
  msftContract.symbol = 'MSFT';
  msftContract.secType = 'STK';
  msftContract.exchange = 'SMART';
  msftContract.primaryExchange = 'NASDAQ';
  msftContract.currency = 'USD';
  obj.reqRealtimeBars(1,msftContract,5,"TRADES",false);
};

obj.on('connected', function () {
  console.log('connected');
  obj.funcQueue.push(addEurUsd);
  obj.funcQueue.push(addMsft);
})

obj.on('realtimeBar', function (data) {
  console.log( "RealtimeBar: " + data[0].toString() + " " + 
    data[1].toString() + " " + data[2].toString() + " " + 
    data[3].toString() + " " + data[4].toString() + " " +
    data[5].toString() + " " + data[6].toString() + " " + 
    data[7].toString() + " " + data[8].toString());
})

setInterval(processIbMsg,0.1);
setInterval(doReqFunc,100);
obj.connectToIb('127.0.0.1',7496,0);