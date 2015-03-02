'use strict'
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

/**
  * Constructor. Default rate limited to 50 output requests per second
  * @constructor
  */
function NodeIbapi() {
  this.client = new addon.NodeIbapi();
  this.limiter = new RateLimiter(50, 'second');
  this.handlers = {};
  this.isProcessing=false;
}

NodeIbapi.prototype = {

  _consumeMessages: function () {
    async.forever( this._processMessage.bind(this),
      function(err) {
        console.error(err);
        throw err;
      }
    );
  },

  _processMessage: function (next) {
    this.client.checkMessages();
    this.client.processMsg();
    var message = this.client.getInboundMsg();
    if (message.messageId in this.handlers) {
      var handler = this.handlers[message.messageId];
      handler(message);
    }
    if (!this.client.isConnected()) {
      message = {};
      message.messageId = 'disconnected';
      if (message.messageId in this.handlers) {
        var dcHandler = this.handlers[message.messageId];
        dcHandler(message);
      }
    }
    setTimeout(next,0);
  },

  /**
    * This function must be called before any other. There is no feedback for a
    * successful connection, but a subsequent attempt to connect will return the
    * message "Already connected." See
    * interactivebrokers.com/en/software/api/apiguide/c/econnect.htm
    * @memberof NodeIbapi
    * @param {string} host -
    * @param {uint} port -
    * @param {int} clientId -
    */
  connect: function (params) {
    return this.client.connect(params);
  },

  /**
    * Call this function to start processing incoming and outgoing messages
    * @memberof NodeIbapi
    */
  beginProcessing: function () {
    if (!this.isProcessing) {
      this._consumeMessages();
      this.isProcessing = true;
    }
  },

  /**
    * Call this function to terminate the connections with TWS. Calling this
    * function does not cancel orders that have already been sent. See
    * interactivebrokers.com/en/software/api/apiguide/c/edisconnect.htm
    * @memberof NodeIbapi
    */
  disconnect: function () {
    this.doAction( function () {
      this.client.disconnect();
    });
  },

  /**
    * Call this function to check if there is a connection with TWS. See
    * interactivebrokers.com/en/software/api/apiguide/c/isconnected.htm
    * @memberof NodeIbapi
    */
  isConnected: function () {
    return this.client.isConnected();
  },

  /**
    * Returns the version of the TWS instance to which the API application is
    * connected. See
    * interactivebrokers.com/en/software/api/apiguide/c/serverversion.htm
    * @memberof NodeIbapi
    */
  serverVersion: function () {
    return this.client.serverVersion();
  },

  /**
    * Returns the time the API application made a connection to TWS. See
    * interactivebrokers.com/en/software/api/apiguide/c/twsconnectiontime.htm
    * @memberof NodeIbapi
    */
  twsConnectionTime: function () {
    return this.client.twsConnectionTime();
  },

  /**
    * Call this function to request market data. The market data will be 
    * returned by the tickPrice and tickSize events. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqmktdata.htm
    * @memberof NodeIbapi
    * @param {int} reqId - 
    * @param {contract} contract - see contract.js
    * @param {string} genericTickType -
    * @param {bool} snapShot - 
    */
  reqMktData: function (params) {
    this.doAction( function () {
      this.client.reqMktData(params);
    });
  },

  /**
    * After calling this function, market data for the specified id will stop
    * flowing. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelmktdata.htm
    * @memberof NodeIbapi
    * @param {uint} reqId -
    */
  cancelMktData: function (params) {
    this.doAction( function () {
      this.client.cancelMktData(params);
    });
  },

  /**
    * Call this function to place an order. The order status will be returned 
    * by the orderStatus event. See
    * interactivebrokers.com/en/software/api/apiguide/c/placeorder.htm
    * @memberof NodeIbapi
    * @param {int} orderId -
    * @param {contract} contract - 
    * @param {order} order - 
    */
  placeOrder: function (params) {
    this.doAction( function () {
      this.client.placeOrder(params);
    });
  },

  /**
    * Call this function to place an order. Short form. The order status will 
    * be returned by the orderStatus event. See
    * interactivebrokers.com/en/software/api/apiguide/c/placeorder.htm
    * @memberof NodeIbapi
    * @param {int} orderId -
    * @param {contract} contract - 
    * @param {string} action - "BUY", "SELL"
    * @param {int} quantity -
    * @param {string} orderType - "LMT", "MKT", etc
    * @param {double} price -
    * @param {double} auxPrice -
    */
  placeSimpleOrder: function (params) {
    this.doAction( function () {
      this.client.placeOrder(params);
    });
  },

  /**
    * Call this function to cancel an order.
    * interactivebrokers.com/en/software/api/apiguide/c/cancelorder.htm
    * @memberof NodeIbapi
    * @param {uint} orderId - orderId
    */
  cancelOrder: function (orderId) {
    this.doAction( function () {
      this.client.cancelOrder(orderId);
    });
  },

  /**
    * Call this function to request the open orders that were placed from this
    * client. Each open order will be fed back through the openOrder() and
    * orderStatus() functions on the EWrapper.
    * See interactivebrokers.com/en/software/api/apiguide/c/reqopenorders.htm
    * @memberof NodeIbapi
    */
  reqOpenOrders: function () {
    this.doAction( function () {
      this.client.reqOpenOrders();
    });
  },

  /**
    * Call this function to start getting account values, portfolio, and last
    * update time information. See 
    * interactivebrokers.com/en/software/api/apiguide/c/reqaccountupdates.htm
    * @memberof NodeIbapi
    * @param {bool} subscribe -
    * @param {string} acctCode -
    */
  reqAccountUpdates: function (params) {
    this.doAction( function () {
      this.client.reqAccountUpdates(params);
    });
  },

  /**
    * When this function is called, the execution reports that meet the filter
    * criteria are downloaded to the client via the execDetails() function. To
    * view executions beyond the past 24 hours, open the Trade Log in TWS and,
    * while the Trade Log is displayed, request the executions again from the
    * API. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqexecutions.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    * @param {executionFilter} filter - see execution.js
    */
  reqExecutions: function (params) {
    this.doAction( function () {
      this.client.reqExecutions(params);
    });
  },

  /**
    * This function should be called frequently (every 1 second) to check for
    * messages received from TWS. In NodeIbapi, this is automatically called See
    * interactivebrokers.com/en/software/api/apiguide/c/checkmessages.htm
    * @memberof NodeIbapi
    */
  checkMessages: function () {
    this.doAction( function () { this.client.checkMessages(); });
  },

  /**
    * Call this function to download all details for a particular underlying.
    * The contract details will be received via the contractDetails() function
    * on the EWrapper. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqcontractdetails.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    * @param {contract} contract -
    */
  reqContractDetails: function (params) {
    this.doAction( function () {
      this.client.reqContractDetails(params);
    });
  },

  /**
    * Call this function to request market depth for a specific contract. The
    * market depth will be returned by the updateMktDepth() and
    * updateMktDepthL2() events. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqmktdepth.htm
    * @memberof NodeIbapi
    * @param {int} tickerId -
    * @param {contract} contract -
    * @param {int} numRows -
    */
  reqMktDepth: function (params) {
    this.doAction( function () {
      this.client.reqMktDepth(params);
    });
  },

  /**
    * After calling this function, market depth data for the specified id will
    * stop flowing. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelmktdepth.htm
    * @memberof NodeIbapi
    * @param {int} tickerId -
    */
  cancelMktDepth: function (tickerId) {
    this.doAction( function () {
      this.client.cancelMktDepth(tickerId);
    });
  },

  /**
    * Call this function to start receiving news bulletins. Each bulletin will
    * be returned by the updatedNewsBulletin() event. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqnewsbulletins.htm
    * @memberof NodeIbapi
    * @param {bool} allMsgs -
    */
  reqNewsBulletins: function (allMsgs) {
    this.doAction( function () {
      this.client.reqNewsBulletins(allMsgs);
    });
  },

  /**
    * Call this function to stop receiving news bulletins. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelnewsbulletins.htm
    * @memberof NodeIbapi
    */
  cancelNewsBulletins: function () {
    this.doAction( function () {
      this.client.cancelNewsBulletins();
    });
  },

  /**
    * The default detail level is ERROR. See
    * interactivebrokers.com/en/software/api/apiguide/c/setloglevel.htm
    * @memberof NodeIbapi
    * @param {int} level -
    */
  setServerLogLevel: function (level) {
    this.doAction( function () {
      this.client.setServerLogLevel(level);
    });
  },

  /**
    * Call this function to request that newly created TWS orders be implicitly
    * associated with the client. When a new TWS order is created, the order
    * will be associated with the client, and fed back through the openOrder()
    * and orderStatus() functions on the EWrapper. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqautoopenorders.htm
    * @memberof NodeIbapi
    * @param {bool} bAutoBind -
    */
  reqAutoOpenOrders: function (bAutoBind) {
    this.doAction( function () {
      this.client.reqAutoOpenOrders(bAutoBind);
    });
  },

  /**
    * Call this function to request the open orders placed from all clients and
    * also from TWS. Each open order will be fed back through the openOrder()
    * and orderStatus() functions on the EWrapper. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqallopenorders.htm
    * @memberof NodeIbapi
    */
  reqAllOpenOrders: function () {
    this.doAction( function () {
      this.client.reqAllOpenOrders();
    });
  },

  /**
    * Call this function to request the list of managed accounts. The list will
    * be returned by the managedAccounts() function on the EWrapper. Note: This
    * request can only be made when connected to a FA managed account. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqmanagedaccts.htm
    * @memberof NodeIbapi
    */
  reqManagedAccts: function () {
    this.doAction( function () {
      this.client.reqManagedAccts();
    });
  },

  /**
    * Request historical data. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqhistoricaldata.htm
    * @memberof NodeIbapi
    * @param {int} id -
    * @param {contract} contract -
    * @param {string} endDateTime -
    * @param {string} durationStr -
    * @param {string} barSizeSetting -
    * @param {string} whatToShow -
    * @param {int} useRTH -
    * @param {int} formatDate -
    */
  reqHistoricalData: function (params) {
    this.doAction( function () {
      this.client.reqHistoricalData(params);
    });
  },

  /**
    * See interactivebrokers.com/en/software/api/apiguide/c/exerciseoptions.htm
    * @memberof NodeIbapi
    * @param {int} id -
    * @param {contract} contract -
    * @param {int} exerciseAction -
    * @param {int} exerciseQuantity -
    * @param {string} account -
    * @param {int} override -
    */
  exerciseOptions: function (params) {
    this.doAction( function () {
      this.client.exerciseOptions(params);
    });
  },

  /**
    * Used if an internet disconnect has occurred or the results of a query are
    * otherwise delayed and the application is no longer interested in receiving
    * the data. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelhistoricaldata.htm
    * @memberof NodeIbapi
    * @param {int} tickerId -
    */
  cancelHistoricalData: function (tickerId) {
    this.doAction( function () {
      this.client.cancelHistoricalData(tickerId);
    });
  },

  /**
    * Call the reqRealTimeBars() function to start receiving real time bar
    * results through the realtimeBar() EWrapper function. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqrealtimebars.htm
    * @memberof NodeIbapi
    * @param {int} id -
    * @param {contract} contract -
    * @param {int} barSize -
    * @param {string} whatToShow -
    * @param {bool} useRTH -
    */
  reqRealtimeBars: function (params) {
    this.doAction( function () {
      this.client.reqRealtimeBars(params);
    });
  },

  /**
    * Call the cancelRealTimeBars() function to stop receiving real time bar
    * results. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelrealtimebars.htm
    * @memberof NodeIbapi
    * @param {int} tickerId - 
    */
  cancelRealTimeBars: function (tickerId) {
    this.doAction( function () {
      this.client.cancelRealTimeBars(tickerId);
    });
  },

  /**
    * See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelscannersubscription.htm
    * @memberof NodeIbapi
    * @param {int} tickerId -
    */
  cancelScannerSubscription: function (tickerId) {
    this.doAction( function () {
      this.client.cancelScannerSubscription(tickerId);
    });
  },

  /**
    * Requests an XML string that describes all possible scanner queries. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqscannerparameters.htm
    * @memberof NodeIbapi
    */
  reqScannerParameters: function () {
    this.doAction( function () {
      this.client.reqScannerParameters();
    });
  },

  /**
    * See
    * interactivebrokers.com/en/software/api/apiguide/c/reqscannersubscription.htm
    * @memberof NodeIbapi
    * @param {int} tickerId -
    * @param {scannerSubscription} subscription - see scannerSubscription.js
    */
  reqScannerSubscription: function (params) {
    this.doAction( function () {
      this.client.reqScannerSubscription(params);
    });
  },

  /**
    * Call this function to receive Reuters global fundamental data for stocks.
    * There must be a subscription to Reuters Fundamental set up in Account
    * Management before you can receive this data. reqFundamentalData() can
    * handle conid specified in the Contract object, but not tradingClass or
    * multiplier. This is because reqFundamentalData() is used only for stocks
    * and stocks do not have a multiplier and trading class. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqfundamentaldata.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    * @param {contract} contract -
    * @param {string} reportType -
    */
  reqFundamentalData: function (params) {
    this.doAction( function () {
      this.client.reqFundamentalData(params);
    });
  },

  /**
    * Call this function to stop receiving Reuters global fundamental data. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelfundamentaldata.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    */
  cancelFundamentalData: function (reqId) {
    this.doAction( function () {
      this.client.cancelFundamentalData(reqId);
    });
  },

  /**
    * Call this function to calculate volatility for a supplied option price and
    * underlying price. See
    * interactivebrokers.com/en/software/api/apiguide/c/calculateimpliedvolatility.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    * @param {contract} contract -
    * @param {double} optionPrice -
    * @param {double} underPrice -
    */
  calculateImpliedVolatility: function (params) {
    this.doAction( function () {
      this.client.calculateImpliedVolatility(params);
    });
  },

  /**
    * Call this function to calculate option price and greek values for a
    * supplied volatility and underlying price. See
    * interactivebrokers.com/en/software/api/apiguide/c/calculateoptionprice.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    * @param {contract} contract -
    * @param {double} volatility -
    * @param {double} underPrice -
    */
  calculateOptionPrice: function (params) {
    this.doAction( function () {
      this.client.calculateOptionPrice(params);
    });
  },

  /**
    * Call this function to cancel a request to calculate volatility for a
    * supplied option price and underlying price. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelcalculateimpliedvolatility.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    */
  cancelCalculateImpliedVolatility: function (reqId) {
    this.doAction( function () {
      this.client.cancelCalculateImpliedVolatility(reqId);
    });
  },

  /**
    * Call this function to cancel a request to calculate the option price and
    * greek values for a supplied volatility and underlying price. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelcalculateoptionprice.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    */
  cancelCalculateOptionPrice: function (reqId) {
    this.doAction( function () {
      this.client.cancelCalculateOptionPrice(reqId);
    });
  },

  /**
    * Use this function to cancel all open orders globally. It cancels both API
    * and TWS open orders. If the order was created in TWS, it also gets
    * canceled. If the order was initiated in the API, it also gets canceled.
    * See
    * interactivebrokers.com/en/software/api/apiguide/c/reqglobalcancel.htm
    * @memberof NodeIbapi
    */
  reqGlobalCancel: function () {
    this.doAction( function () {
      this.client.reqGlobalCancel();
    });
  },

  /**
    * The API can receive frozen market data from Trader Workstation. Frozen
    * market data is the last data recorded in our system. During normal trading
    * hours, the API receives real-time market data. If you use this function,
    * you are telling TWS to automatically switch to frozen market data after
    * the close. Then, before the opening of the next trading day, market data
    * will automatically switch back to real-time market data. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqmarketdatatype.htm
    * @memberof NodeIbapi
    * @param {int} marketDataType -
    */
  reqMarketDataType: function (marketDataType) {
    this.doAction( function () {
      this.client.reqMarketDataType(marketDataType);
    });
  },

  /**
    * Requests real-time position data for all accounts. Note: This request can
    * only be made when connected to an FA managed account. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqpositions.htm
    * @memberof NodeIbapi
    */
  reqPositions: function () {
    this.doAction( function () {
      this.client.reqPositions();
    });
  },

  /**
    * Cancels real-time position updates. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelpositions.htm
    * @memberof NodeIbapi
    */
  cancelPositions: function () {
    this.doAction( function () {
      this.client.cancelPositions();
    });
  },

  /**
    * Call this method to request and keep up to date the data that appears on
    * the TWS Account Window Summary tab. The data is returned by
    * accountSummary(). See
    * interactivebrokers.com/en/software/api/apiguide/c/reqaccountsummary.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    * @param {string} groupName -
    * @param {string} tags -
    */
  reqAccountSummary: function (params) {
    this.doAction( function () {
      this.client.reqAccountSummary(params);
    });
  },

  /**
    * Cancels the request for Account Window Summary tab data. See
    * interactivebrokers.com/en/software/api/apiguide/c/cancelaccountsummary.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    */
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

  /**
    * See
    * interactivebrokers.com/en/software/api/apiguide/c/querydisplaygroups.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    */
  queryDisplayGroups: function (reqId) {
    this.doAction( function () {
      this.client.queryDisplayGroups(reqId);
    });
  },

  /**
    * See
    * interactivebrokers.com/en/software/api/apiguide/c/subscribetogroupevents.htm
    * @memberof NodeIbapi
    * @param {int} reqId - 
    * @param {int} groupId -
    */
  subscribeToGroupEvents: function (params) {
    this.doAction( function () {
      this.client.subscribeToGroupEvents(params);
    });
  },

  /**
    * See
    * interactivebrokers.com/en/software/api/apiguide/c/updatedisplaygroup.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    * @param {string} contractInfo -
    */
  updateDisplayGroup: function (params) {
    this.doAction( function () {
      this.client.updateDisplayGroup(params);
    });
  },

  /**
    * See
    * interactivebrokers.com/en/software/api/apiguide/c/unsubscribefromgroupevents.htm
    * @memberof NodeIbapi
    * @param {int} reqId -
    */
  unsubscribeFromGroupEvents: function (reqId) {
    this.doAction( function () {
      this.client.unsubscribeFromGroupEvents(reqId);
    });
  },

  /**
    * Call this function to request from TWS the next valid ID that can be used
    * when placing an order.  After calling this function, the nextValidId()
    * event will be triggered, and the id returned is that next valid ID. That
    * ID will reflect any autobinding that has occurred (which generates new IDs
    * and increments the next valid ID therein). See
    * interactivebrokers.com/en/software/api/apiguide/c/reqids.htm
    * @memberof NodeIbapi
    * @param {int} numIds -
    */
  reqIds: function (numIds) {
    this.doAction( function () {
      this.client.reqIds(numIds);
    });
  },

  /**
    * Returns the current system time on the server side. See
    * interactivebrokers.com/en/software/api/apiguide/c/reqcurrenttime.htm
    * @memberof NodeIbapi
    */
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
