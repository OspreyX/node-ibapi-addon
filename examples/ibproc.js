var addon = require('../build/Release/NodeIbapiAddon');
var obj = new addon.NodeIbapi();

obj.connect('127.0.0.1',7496,0);

var validOrderId = obj.getNextOrderId();
var orderId = -1;
var counter = 0;
var price = '';
var time = '';
var trackedOrder = [];
var once = false;

while(obj.isConnected()) {
  obj.checkMessages();
  obj.processMsg();
  // price = obj.getTickPrice();
  // time = obj.getCurrentTime();
  if (!once && counter == 500000) {
    validOrderId = obj.getNextOrderId();
    orderId = validOrderId + 1;
    console.log('Next valid order Id: %d',validOrderId);
    once = true;
  }
  if (price != '') {
    console.log('The price is: %s', price);
    price = '';
  }
  if (time != '') {
    console.log('The time is: %s', time);
    time = '';
  }
  
  if (counter == 10000000 && orderId > -1) {
    console.log('Next valid order Id: %d',validOrderId);
    console.log("Placing order for MSFT");
    obj.placeOrder(orderId,"MSFT", "STK", "SMART", "USD", 
      "BUY", 1000, "LMT", 0.11);
    orderId = orderId + 1;
  }
  if (counter == 20000000) {
    console.log('canceling order: %d', orderId-1);
    obj.cancelOrder(orderId-1);
    counter = 0;
  }
  
  counter = counter + 1;

}
obj.disconnect();