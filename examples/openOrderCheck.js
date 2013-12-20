// In this example, we will try to place a limit order and to 
//  receive the orderStatus, as well as to poll for openOrders.

var addon = require('../build/Release/NodeIbapiAddon');
var obj = new addon.NodeIbapi();

obj.connect('127.0.0.1',7496,0);

var orderStatus;
var openOrder;
var orderId = -1;
var counter = 0;
var validOrderId;
var ready = false;
var clientError;
var srvError;

var time = process.hrtime();
var diff = process.hrtime(time);
var funqueue = []; // function queue

var addReqId = function () {
  obj.reqIds(1);
}

while (obj.isConnected()) {
  diff = process.hrtime(time);
  
  // timed function queue
  if (diff[1] > 21000000) {
    time = process.hrtime();
    if (funqueue[0] != null) {
      (funqueue.shift())();
    }
  }

  obj.checkMessages();
  obj.processMsg();
  clientError = obj.getWinError();
  srvError = obj.getError();

  if (clientError[0] != "") {
    console.log('Client error');
  }
  if (srvError[0] != -1) {
    console.log('Error: ' + srvError[0].toString() + ' - ' + 
      srvError[1].toString() + ' - ' + srvError[2].toString());
  }

 // retrieve validOrderId
  validOrderId = obj.getNextOrderId();
  if (validOrderId < 0 && !once) {
    once = true;
    funqueue.push(addReqId);
  }
  if (validOrderId > 0 && !ready) {
      ready = true;
      orderId = validOrderId;
  }
  
  orderStatus = obj.getOrderStatus();
  openOrder = obj.getOpenOrder();

  // order status right after submittal
  if (orderStatus[0] > -1) {
    console.log("OrderID, status, filled, remaining, avgFillPrice, permId, parentId, lastFillPrice, clientId, whyHeld");
    console.log( 
      orderStatus[0].toString() + " " + orderStatus[1].toString() + " " +
      orderStatus[2].toString() + " " + orderStatus[3].toString() + " " +
      orderStatus[4].toString() + " " + orderStatus[5].toString() + " " +
      orderStatus[6].toString() + " " + orderStatus[7].toString() + " " +
      orderStatus[8].toString() + " " + orderStatus[9].toString()
      );
  }

  // check open order
  if (openOrder[0] > -1) {
    console.log("OrderId, status, initMargin, maintMargin, equityWithLoan, Commission, minCommission, maxCommission, commissionCurrency, warningText");
    console.log(
      openOrder[0].toString() + " " + openOrder[1].toString() + " " + 
      openOrder[2].toString() + " " + openOrder[3].toString() + " " + 
      openOrder[4].toString() + " " + openOrder[5].toString() + " " + 
      openOrder[6].toString() + " " + openOrder[7].toString() + " " + 
      openOrder[8].toString() + " " + openOrder[9].toString()
      );
  }

  if (counter == 50000) {
    obj.reqOpenOrders();
  }
  
  if (counter == 700000 && orderId > -1) {
    console.log('Next valid order Id: %d',orderId);
    console.log("Placing order for MSFT");
    obj.placeSimpleOrder(orderId,"MSFT", "STK", "SMART", "NASDAQ", "USD", 
      "BUY", 1000, "LMT", 0.11);
    orderId = orderId + 1;
  } 
  if (counter == 1000000) {
    console.log('canceling order: %d', orderId-1);
    obj.cancelOrder(orderId-1);
    counter = 0;
  }
  counter = counter + 1;

}
