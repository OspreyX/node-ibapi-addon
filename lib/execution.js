function execution() {
}

execution.prototype.execId = '';
execution.prototype.time = '';
execution.prototype.acctNumber = '';
execution.prototype.exchange = '';
execution.prototype.side = '';
execution.prototype.shares = 0;
execution.prototype.price = 0.0;
execution.prototype.permId = 0;
execution.prototype.clientId = 0;
execution.prototype.orderId = 0;
execution.prototype.liquidation = 0;
execution.prototype.cumQty = 0;
execution.prototype.avgPrice = 0.0;
execution.prototype.orderRef = '';
execution.prototype.evRule = '';
execution.prototype.evMultiplier = 0.0;

exports.createExecution = function() {
  var newExecution = new execution();
  return newExecution;
};

function executionFilter() {
}

executionFilter.prototype.clientId = 0;
executionFilter.prototype.acctCode = '';
executionFilter.prototype.time = '';
executionFilter.prototype.symbol = '';
executionFilter.prototype.secType = '';
executionFilter.prototype.exchange = '';
executionFilter.prototype.side = '';

exports.createExecutionFilter = function() {
  var newExecutionFilter = new executionFilter();
  return newExecutionFilter;
}
