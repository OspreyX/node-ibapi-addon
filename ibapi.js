// module for event driven methods
var addon = require('./build/Release/NodeIbapiAddon'),
  contract = require('./lib/contract'),
  order = require('./lib/order'),
  execution = require('./lib/execution'),
  contractDetails = require('./lib/contractDetails'),
  scannerSubscription = require('./lib/scannerSubscription'),
  messageIds = require('./messageIds'),
  async = require("async"),
  RateLimiter = require("limiter").RateLimiter;

function NodeIbapi() {
  this.client = new addon.NodeIbapi();
  this.limiter = new RateLimiter(50, 'second');
  this.handlers = {};
  this.isProcessing=false;
}

NodeIbapi.prototype = {

  _consumeMessages: function () {
    async.forever( this.processMessage.bind(this),
      function(err) {
        console.error(err);
        throw err;
      }
    );
  },

  processMessage: function (next) {
    this.client.checkMessages();
    this.client.processMsg();
    var message = this.client.getInboundMsg();
    if (message.messageId in this.handlers) {
      var handler = this.handlers[message.messageId];
      if (handler.length === 2)
        return handler(message, next);
      else
        handler(message);
    };
    if (!this.client.isConnected()) {
      message = {};
      message.messageId = 'disconnected';
      if (message.messageId in this.handlers) {
        var handler = this.handlers[message.messageId];
        if (handler.length === 2)
          return handler(message, next);
        else
          handler(message);
      };
    };
    setTimeout(next,0);
  },

  connect: function (host, port, clientId) {
    return this.client.connect(host, port, clientId)
  },

  beginProcessing: function () {
    if (!this.isProcessing) {
      this._consumeMessages();
      this.isProcessing = true;
    }
  },

  disconnect: function () {
    this.doAction( function () {
      this.client.disconnect();
    });
  },

  isConnected: function () {
    return this.client.isConnected();
  },

  serverVersion: function () {
    return this.client.serverVersion();
  },

  twsConnectionTime: function () {
    return this.client.twsConnectionTime();
  },

  reqMktData: function (reqId, contract, genericTickType, snapShot) {
    this.doAction( function () {
      this.client.reqMktData(reqId, contract, genericTickType, snapShot);
    });
  },

  cancelMktData: function (reqId) {
    this.doAction( function () {
      this.client.cancelMktData(reqId);
    });
  },

  placeOrder: function (orderId, contract, order) {
    this.doAction( function () {
      this.client.placeOrder(orderId, contract, order);
    });
  },

  placeSimpleOrder: function (orderId, contract, action, quantity,
                                 orderType, price, auxPrice) {
    this.doAction( function () {
      this.client.placeOrder( orderId, contract, action, quantity, orderType,
                              price, auxPrice);
    });
  },

  cancelOrder: function (orderId) {
    this.doAction( function () {
      this.client.cancelOrder(orderId);
    });
  },

  reqOpenOrders: function () {
    this.doAction( function () {
      this.client.reqOpenOrders();
    });
  },

  reqAccountUpdates: function (subscribe, acctCode) {
    this.doAction( function () {
      this.client.reqAccountUpdates(subscribe, acctCode);
    });
  },

  reqExecutions: function (reqId, clientId, acctCode, time,
                           symbol, secType, exchange, side) {
    this.doAction( function () {
      this.client.reqExecutions(reqId, clientId, acctCode, time,
                                symbol, secType, exchange, side);
    });
  },

  checkMessages: function () {
    this.doAction( function () { this.client.checkMessages(); });
  },

  reqContractDetails: function (reqId, contract) {
    this.doAction( function () {
      this.client.reqContractDetails(reqId, contract);
    });
  },

  reqMktDepth: function (tickerId, contract, numRows) {
    this.doAction( function () {
      this.client.reqMktDepth(tickerId, contract, numRows);
    });
  },

  cancelMktDepth: function (tickerId) {
    this.doAction( function () {
      this.client.cancelMktDepth(tickerId);
    });
  },

  reqNewsBulletins: function (allMsgs) {
    this.doAction( function () {
      this.client.reqNewsBulletins(allMsgs);
    });
  },

  cancelNewsBulletins: function () {
    this.doAction( function () {
      this.client.cancelNewsBulletins();
    });
  },

  setServerLogLevel: function (level) {
    this.doAction( function () {
      this.client.setServerLogLevel(level);
    });
  },

  reqAutoOpenOrders: function (bAutoBind) {
    this.doAction( function () {
      this.client.reqAutoOpenOrders(bAutoBind);
    });
  },

  reqAllOpenOrders: function () {
    this.doAction( function () {
      this.client.reqAllOpenOrders();
    });
  },

  reqManagedAccts: function () {
    this.doAction( function () {
      this.client.reqManagedAccts();
    });
  },

  reqHistoricalData: function (id, contract, endDateTime,
                               durationStr, barSizeSetting,
                               whatToShow, useRTH, formatDate) {
    this.doAction( function () {
      this.client.reqHistoricalData(id, contract, endDateTime,
                                    durationStr, barSizeSetting,
                                    whatToShow, useRTH, formatDate);
    });
  },

  exerciseOptions: function (tickerId, contract, exerciseAction,
                             exerciseQuantity, account, override) {
    this.doAction( function () {
      this.client.exerciseOptions(tickerId, contract, exerciseAction,
                                  exerciseQuantity, account, override);
    });
  },

  cancelHistoricalData: function (tickerId) {
    this.doAction( function () {
      this.client.cancelHistoricalData(tickerId);
    });
  },

  reqRealtimeBars: function (tickerId, contract, barSize,
                             whatToShow, useRTH) {
    this.doAction( function () {
      this.client.reqRealtimeBars(tickerId, contract, barSize,
                                  whatToShow, useRTH);
    });
  },

  cancelRealTimeBars: function (tickerId) {
    this.doAction( function () {
      this.client.cancelRealTimeBars(tickerId);
    });
  },

  cancelScannerSubscription: function (tickerId) {
    this.doAction( function () {
      this.client.cancelScannerSubscription(tickerId);
    });
  },

  reqScannerParameters: function () {
    this.doAction( function () {
      this.client.reqScannerParameters();
    });
  },

  reqScannerSubscription: function (tickerId, subscription) {
    this.doAction( function () {
      this.client.reqScannerSubscription(tickerId, subscription);
    });
  },

  reqFundamentalData: function (reqId, contract, reportType) {
    this.doAction( function () {
      this.client.reqFundamentalData(reqId, contract, reportType);
    });
  },

  cancelFundamentalData: function (reqId) {
    this.doAction( function () {
      this.client.cancelFundamentalData(reqId);
    });
  },

  calculateImpliedVolatility: function (reqId, contract, optionPrice,
                                        underPrice) {
    this.doAction( function () {
      this.client.calculateImpliedVolatility(reqId, contract, optionPrice, 
                                             underPrice);
    });
  },

  calculateOptionPrice: function (reqId, contract, volatility, underPrice) {
    this.doAction( function () {
      this.client.calculateOptionPrice(reqId, contract,
                                       volatility, underPrice);
    });
  },

  cancelCalculateImpliedVolatility: function (reqId) {
    this.doAction( function () {
      this.client.cancelCalculateImpliedVolatility(reqId);
    });
  },

  cancelCalculateOptionPrice: function (reqId) {
    this.doAction( function () {
      this.client.cancelCalculateOptionPrice(reqId);
    });
  },

  reqGlobalCancel: function () {
    this.doAction( function () {
      this.client.reqGlobalCancel();
    });
  },

  reqMarketDataType: function (marketDataType) {
    this.doAction( function () {
      this.client.reqMarketDataType(marketDataType);
    });
  },

  reqPositions: function () {
    this.doAction( function () {
      this.client.reqPositions();
    });
  },

  cancelPositions: function () {
    this.doAction( function () {
      this.client.cancelPositions();
    });
  },

  reqAccountSummary: function (reqId, groupName, tags) {
    this.doAction( function () {
      this.client.reqAccountSummary(reqId, groupName, tags);
    });
  },

  cancelAccountSummary: function (reqId) {
    this.doAction( function () {
      this.client.cancelAccountSummary(reqId);
    });
  },

  verifyRequest: function (apiName, apiVersion) {
    this.doAction( function () {
      this.client.verifyRequest(apiName, apiVersion);
    });
  },

  verifyMessage: function (apiData) {
    this.doAction( function () {
      this.client.verifyMessage(apiData);
    });
  },

  queryDisplayGroups: function (reqId) {
    this.doAction( function () {
      this.client.queryDisplayGroups(reqId);
    });
  },

  subscribeToGroupEvents: function (reqId, groupId) {
    this.doAction( function () {
      this.client.subscribeToGroupEvents(reqId, groupId);
    });
  },

  updateDisplayGroup: function (reqId, contractInfo) {
    this.doAction( function () {
      this.client.updateDisplayGroup(reqId, contractInfo);
    });
  },

  unsubscribeFromGroupEvents: function (reqId) {
    this.doAction( function () {
      this.client.unsubscribeFromGroupEvents(reqId);
    });
  },

  reqIds: function (numIds) {
    this.doAction( function () {
      this.client.reqIds(numIds);
    });
  },

  reqCurrentTime: function () {
    this.doAction(function () { this.client.reqCurrentTime(); });
  },

  doAction: function (action) {
    this.limiter.removeTokens(1, function (err, remainingRequests) {
      action.bind(this)();
    }.bind(this));
  }
};

exports = module.exports = {
  "NodeIbapi": NodeIbapi,
  "messageIds": messageIds,
  "contract": contract,
  "execution": execution,
  "scannerSubscription": scannerSubscription,
  "contractDetails": contractDetails,
  "order": order
};
