var addon = require('../ibapi');
var obj = new addon.NodeIbapi();

var orderId = -1;
var processIbMsg = function () {
  obj.processIbMsg();
}
var addReqId = function () {
  obj.addReqId(1);
}
var doReqFunc = function () {
  obj.doReqFunc();
}
obj.on('connected', function () {
  console.log('connected');
  setInterval(processIbMsg,0.1);
  obj.funcQueue.push(addReqId);
})
.once('nextValidId', function (data) {
  orderId = data;
  console.log('nextValidId: ' + orderId);
  setInterval(doReqFunc,100);
})
.on('disconnected', function () {
  console.log('disconnected');
  process.exit(1);
})

obj.connectToIb('127.0.0.1',7496,0);
