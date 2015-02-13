function contract() {
}

contract.prototype.conId = 0;
contract.prototype.symbol = '';
contract.prototype.secType = '';
contract.prototype.expiry = '';
contract.prototype.strike = 0.0;
contract.prototype.right = '';
contract.prototype.multiplier = '';
contract.prototype.exchange = '';

// pick an actual (ie non-aggregate) exchange that the contract trades on.
//  DO NOT SET TO SMART.
contract.prototype.primaryExchange = '';
contract.prototype.currency = '';
contract.prototype.localSymbol = '';
contract.prototype.tradingClass = '';
contract.prototype.includeExpired = false;

// CUSIP;SEDOL;ISIN;RIC
contract.prototype.secIdType = '';
contract.prototype.secId = '';

function underComp() {
}
underComp.prototype.conId = 0;
underComp.prototype.delta = 0;
underComp.prototype.price = 0;

exports.createContract = function() {
  var newContract = new contract();
  return newContract;
};

exports.createUnderComp = function() {
  var newUnderComp = new underComp();
  return newUnderComp;
};
