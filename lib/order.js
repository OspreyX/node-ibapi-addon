function order() {
}

//-- order identifier
order.prototype.orderId = 0;
order.prototype.clientId = 0;
order.prototype.permId = 0;

//-- main order fields
order.prototype.action = '';
order.prototype.totalQuantity = 0;
order.prototype.orderType = '';
order.prototype.lmtPrice = Number.MAX_VALUE;
order.prototype.auxPrice = Number.MAX_VALUE;

//-- extended order fields
// "Time in Force" - DAY, GTC, etc.
order.prototype.tif = '';

// one cancels all group name
order.prototype.ocaGroup = '';

// 1 = CANCEL_WITH_BLOCK, 2 = REDUCE_WITH_BLOCK, 3 = REDUCE_NON_BLOCK
order.prototype.ocaType = 0;

order.prototype.orderRef = '';

// if false, order will be created but not transmited
order.prototype.transmit = true;

// Parent order Id, to associate Auto STP or TRAIL orders with the original
//  order.
order.prototype.parentId = 0;

order.prototype.blockOrder = false;
order.prototype.sweepToFill = false;
order.prototype.displaySize = 0;

// 0=Default, 1=Double_Bid_Ask, 2=Last, 3=Double_Last, 4=Bid_Ask,
//  7=Last_or_Bid_Ask, 8=Mid-point
order.prototype.triggerMethod = 0;

order.prototype.outsideRth = false;
order.prototype.hidden = false;

// Format: 20060505 08:00:00 {time zone}
order.prototype.goodAfterTime = '';

// Format: 20060505 08:00:00 {time zone}
order.prototype.goodTillDate = '';

// Individual = 'I', Agency = 'A', AgentOtherMember = 'W',
//  IndividualPTIA = 'J', AgencyPTIA = 'U', AgentOtherMemberPTIA = 'M',
//  IndividualPT = 'K', AgencyPT = 'Y', AgentOtherMemberPT = 'N'
order.prototype.rule80A = '';

order.prototype.allOrNone = false;
order.prototype.minQty = Number.MAX_VALUE;

//-- REL orders only
order.prototype.percentOffset = Number.MAX_VALUE;
order.prototype.overridePercentageConstraints = false;

//-- TRAILLIMIT orders only
order.prototype.trailStopPrice = Number.MAX_VALUE;
order.prototype.trailingPercent = Number.MAX_VALUE;

//-- financial advisors only

//-- institutional (ie non-cleared) only

//-- SMART routing only
order.prototype.discretionaryAmt = 0;
order.prototype.eTradeOnly = true;
order.prototype.firmQuoteOnly = true;
order.prototype.nbboPriceCap = Number.MAX_VALUE;
order.prototype.optOutSmartRouting = false;

//-- BOX exchange orders only
// AUCTION_MATCH, AUCTION_IMPROVEMENT, AUCTION_TRANSPARENT
order.prototype.auctionStrategy = 0;

order.prototype.startingPrice = Number.MAX_VALUE;
order.prototype.stockRefPrice = Number.MAX_VALUE;
order.prototype.delta = Number.MAX_VALUE;

//-- pegged to stock and VOL orders only
order.prototype.stockRangeLower = Number.MAX_VALUE;
order.prototype.stockRangeUpper = Number.MAX_VALUE;

//-- VOLATILITY ORDERS ONLY
order.prototype.volatility = Number.MAX_VALUE;

// 1=daily, 2=annual
order.prototype.volatilityType = Number.MAX_VALUE;

order.prototype.deltaNeutralOrderType = '';
order.prototype.deltaNeutralAuxPrice = Number.MAX_VALUE;
order.prototype.deltaNeutralConId = 0;
order.prototype.deltaNeutralSettlingFirm = '';
order.prototype.deltaNeutralClearingAccount = '';
order.prototype.deltaNeutralClearingIntent = '';
order.prototype.deltaNeutralOpenClose = '';
order.prototype.deltaNeutralShortSale = false;
order.prototype.deltaNeutralShortSaleSlot = 0;
order.prototype.deltaNeutralDesignatedLocation = '';
order.prototype.continuousUpdate = false;

// 1=Average, 2 = BidOrAsk
order.prototype.referencePriceType = Number.MAX_VALUE;

//-- COMBO ORDERS ONLY

//-- SCALE ORDERS ONLY
order.prototype.scaleInitLevelSize = Number.MAX_VALUE;
order.prototype.scaleSubsLevelSize = Number.MAX_VALUE;
order.prototype.scalePriceIncrement = Number.MAX_VALUE;
order.prototype.scalePriceAdjustValue = Number.MAX_VALUE;
order.prototype.scalePriceAdjustInterval = Number.MAX_VALUE;
order.prototype.scaleProfitOffset = Number.MAX_VALUE;
order.prototype.scaleAutoReset = false;
order.prototype.scaleInitPosition = Number.MAX_VALUE;
order.prototype.scaleInitFillQty = Number.MAX_VALUE;
order.prototype.scaleRandomPercent = false;

//-- HEDGE ORDERS

// 'D' - delta, 'B' - beta, 'F' - FX, 'P' - pair
order.prototype.hedgeType = '';

// 'beta=X' value for beta hedge, 'ratio=Y' for pair hedge
order.prototype.hedgeParam = '';

//-- Clearing info
order.prototype.account = '';
order.prototype.settlingFirm = '';

// True beneficiary of the order
order.prototype.clearingAccount = '';

// "" (Default), "IB", "Away", "PTA" (PostTrade)
order.prototype.clearingIntent = '';

//-- ALGO ORDERS ONLY

//-- What-if

//-- Not Held

exports.createOrder = function() {
  var newOrder = new order();
  return newOrder;
};
