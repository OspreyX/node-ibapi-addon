// This is a bare-bones setting for the scripts using node-ibapi-addon
var addon = require('../build/Release/NodeIbapiAddon');
var obj = new addon.NodeIbapi();

obj.connect('127.0.0.1',7496,0);

var validOrderId = -1;
var orderId = -1;
var counter = 0;
var ready = false;

var clientError;
var srvError;

while(obj.isConnected()) {
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

  if (counter == 20000 && !ready) {
    validOrderId = obj.getNextOrderId();
    if (validOrderId > -1) {
      ready = true;
      orderId = validOrderId;
      counter = 0;
    }
    obj.reqIds(1);
  }
  
  // implement your functionalities here
  if (counter == 50000 && ready) {
    console.log('Next valid order Id: %d',validOrderId);
    obj.disconnect();
  }
  counter = counter + 1;
}
