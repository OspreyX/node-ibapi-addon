var contract = require('./contract').createContract();

function contractDetails() {
}

contractDetails.prototype.summary = contract;
contractDetails.prototype.marketName = '';
contractDetails.prototype.minTick = 0.0;
contractDetails.prototype.orderTypes = '';
contractDetails.prototype.validExchanges = '';
contractDetails.prototype.priceMagnifier = 0;
contractDetails.prototype.underConId = 0;
contractDetails.prototype.longName = '';
contractDetails.prototype.contractMonth = '';
contractDetails.prototype.industry = '';
contractDetails.prototype.category = '';
contractDetails.prototype.subcategory = '';
contractDetails.prototype.timeZoneId = '';
contractDetails.prototype.tradingHours = '';
contractDetails.prototype.liquidHours = '';
contractDetails.prototype.evRule = '';
contractDetails.prototype.evMultiplier = 0.0;

// Bond values   
contractDetails.prototype.cusip = '';
contractDetails.prototype.ratings = '';
contractDetails.prototype.descAppend = '';
contractDetails.prototype.bondType = '';
contractDetails.prototype.couponType = '';
contractDetails.prototype.callable = false;
contractDetails.prototype.putable = false;
contractDetails.prototype.coupon = 0.0;
contractDetails.prototype.convertible = false;
contractDetails.prototype.maturity = '';
contractDetails.prototype.issueDate = '';
contractDetails.prototype.nextOptionDate = '';
contractDetails.prototype.nextOptionType = '';
contractDetails.prototype.nextOptionPartial = false;
contractDetails.prototype.notes = '';

exports.createContractDetails = function () {
  var newContractDetails = new contractDetails();
  return newContractDetails;
};
