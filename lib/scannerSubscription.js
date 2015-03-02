function scannerSubscription() {
}

scannerSubscription.prototype.numberOfRows = -1;
scannerSubscription.prototype.instrument = '';
scannerSubscription.prototype.locationCode = '';
scannerSubscription.prototype.scanCode = '';
scannerSubscription.prototype.abovePrice = Number.MAX_VALUE;
scannerSubscription.prototype.belowPrice = Number.MAX_VALUE;
scannerSubscription.prototype.aboveVolume = Number.MAX_VALUE;
scannerSubscription.prototype.marketCapAbove = Number.MAX_VALUE;
scannerSubscription.prototype.marketCapBelow = Number.MAX_VALUE;
scannerSubscription.prototype.moodyRatingAbove = '';
scannerSubscription.prototype.moodyRatingBelow = '';
scannerSubscription.prototype.spRatingAbove = '';
scannerSubscription.prototype.spRatingBelow = '';
scannerSubscription.prototype.maturityDateAbove = '';
scannerSubscription.prototype.maturityDateBelow = '';
scannerSubscription.prototype.couponRateAbove = Number.MAX_VALUE;
scannerSubscription.prototype.couponRateBelow = Number.MAX_VALUE;
scannerSubscription.prototype.excludeConvertible = 0;
scannerSubscription.prototype.averageOptionVolumeAbove = 0;
scannerSubscription.prototype.scannerSettingPairs = '';
scannerSubscription.prototype.stockTypeFilter = '';

exports.createScannerSub = function() {
  var newScanSub = new scannerSubscription();
  return newScanSub;
};
