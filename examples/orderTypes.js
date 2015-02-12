// In this example, we will submit orders with different methods
var addon = require('../ibapi'),
  messageIds = addon.messageIds,
  contract = addon.contract,
  order = addon.order;

var api = new addon.NodeIbapi();
var orderId = -1;

function createMarketOrder() {
  var msftContract1 = contract.createContract();
  msftContract1.secIdType = 'ISIN';
  msftContract1.secId = 'US92343V3024';
  msftContract1.secType = 'STK';
  msftContract1.exchange = 'NYSE';
  msftContract1.currency = 'USD';

  var newOrder = order.createOrder();
  newOrder.action = "BUY";
  newOrder.totalQuantity = 100;
  newOrder.orderType = "MKT";

  api.placeOrder(orderId, msftContract1, newOrder);
}

function createReplaceOrder() {
  createLimitOrder();

  setTimeout(function () {
    var msftContract1 = contract.createContract();
    msftContract1.symbol = 'AAPL';
    msftContract1.secType = 'STK';
    msftContract1.exchange = 'SMART';
    msftContract1.primaryExchange = 'NASDAQ';
    msftContract1.currency = 'USD';

    var newOrder = order.createOrder();
    newOrder.action = "BUY";
    newOrder.totalQuantity = 100;
    newOrder.orderType = "LMT";
    newOrder.lmtPrice = 109;

    api.placeOrder(orderId, msftContract1, newOrder);
  }, 3000);
}

function createIocOrder() {
  var msftContract1 = contract.createContract();
  msftContract1.symbol = 'AAPL';
  msftContract1.secType = 'STK';
  msftContract1.exchange = 'SMART';
  msftContract1.primaryExchange = 'NASDAQ';
  msftContract1.currency = 'USD';

  var newOrder = order.createOrder();
  newOrder.action = "BUY";
  newOrder.totalQuantity = 100;
  newOrder.orderType = "LMT";
  newOrder.tif = "IOC";
  newOrder.lmtPrice = 111.65;

  api.placeOrder(orderId, msftContract1, newOrder);
}

function createFokOrder() {
  var msftContract1 = contract.createContract();
  msftContract1.symbol = 'AAPL';
  msftContract1.secType = 'STK';
  msftContract1.exchange = 'SMART';
  msftContract1.primaryExchange = 'NASDAQ';
  msftContract1.currency = 'USD';

  var newOrder = order.createOrder();
  newOrder.action = "BUY";
  newOrder.totalQuantity = 1000;
  newOrder.orderType = "MKT";
  newOrder.allOrNone = true;

  api.placeOrder(orderId, msftContract1, newOrder);
}

function createLimitOrder() {
  var msftContract1 = contract.createContract();
  msftContract1.symbol = 'AAPL';
  msftContract1.secType = 'STK';
  msftContract1.exchange = 'SMART';
  msftContract1.primaryExchange = 'NASDAQ';
  msftContract1.currency = 'USD';

  var newOrder = order.createOrder();
  newOrder.action = "BUY";
  newOrder.totalQuantity = 100;
  newOrder.orderType = "LMT";
  newOrder.lmtPrice = 110.54;

  api.placeOrder(orderId, msftContract1, newOrder);
}

var handleValidOrderId = function (message) {
  orderId = message.orderId;

  // Try out the different order types shown above. For more details,
  //  see IB API documentation
  createMarketOrder();
  console.log('next order Id is ' + orderId);
};

var handleServerError = function (message) {
  console.log('Error: ' + message.id.toString() + '-' +
              message.errorCode.toString() + '-' +
              message.errorString.toString());
};

var handleOrderStatus = function (message) {
  console.log(JSON.stringify(message));
};

var handleOpenOrder = function (message) {
  console.log(JSON.stringify(message));
};

var handleExecutions = function (message) {
  console.log('execDetails');
  console.log(JSON.stringify(message));
};

api.handlers[messageIds.nextValidId] = handleValidOrderId;
api.handlers[messageIds.svrError] = handleServerError;
api.handlers[messageIds.orderStatus] = handleOrderStatus;
api.handlers[messageIds.openOrder] = handleOpenOrder;
api.handlers[messageIds.execDetails] = handleExecutions;

var connected = api.connect('127.0.0.1', 7496, 0);

if (connected) {
  api.beginProcessing();
}
