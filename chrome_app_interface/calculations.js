/**
 * Created by stephen on 18/05/15.
 */

var diodeReading = 633.00;
var ODZero = 663.67; // ODZero value should be close to the diodereading
var ODMin = diodeReading;
ODMin = (Math.log(1 / (ODMin / ODZero)) / Math.log(10));
ODMin = ODMin * .9861 + 0.0058;
console.log(ODMin);

// spreadsheet calculations

// Math.LOG10(1/($avg/ODzero));