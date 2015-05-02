// Serial used from your Arduino board
// const DEVICE_PATH = 'COM11'; // PC
const DEVICE_PATH = '/dev/ttyACM1'; //MAC
const serial = chrome.serial;

/* Interprets an ArrayBuffer as UTF-8 encoded string data. */
var ab2str = function(buf) {
  var bufView = new Uint8Array(buf);
  var encodedString = String.fromCharCode.apply(null, bufView);
  return decodeURIComponent(escape(encodedString));
};

/* Converts a string to UTF-8 encoding in a Uint8Array; returns the array buffer. */
var str2ab = function(str) {
  var encodedString = unescape(encodeURIComponent(str));
  console.log("str2ab string = "+encodedString);
  var bytes = new Uint8Array(encodedString.length);
  for (var i = 0; i < encodedString.length; ++i) {
    bytes[i] = encodedString.charCodeAt(i);
  }
  return bytes.buffer;
};

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

var SerialConnection = function() {
  this.connectionId = -1;
  this.lineBuffer = "";
  this.boundOnReceive = this.onReceive.bind(this);
  this.boundOnReceiveError = this.onReceiveError.bind(this);
  this.onConnect = new chrome.Event();
  this.onReadLine = new chrome.Event();
  this.onError = new chrome.Event();
};

SerialConnection.prototype.onConnectComplete = function(connectionInfo) {
  if (!connectionInfo) {
    log("Connection failed.");
    return;
  }
  this.connectionId = connectionInfo.connectionId;
  chrome.serial.onReceive.addListener(this.boundOnReceive);
  chrome.serial.onReceiveError.addListener(this.boundOnReceiveError);
  this.onConnect.dispatch();
};

SerialConnection.prototype.onReceive = function(receiveInfo) {
  if (receiveInfo.connectionId !== this.connectionId) {
    return;
  }

  this.lineBuffer += ab2str(receiveInfo.data);

  var index;
  while ((index = this.lineBuffer.indexOf('\n')) >= 0) {
    var line = this.lineBuffer.substr(0, index + 1);
    this.onReadLine.dispatch(line);
    this.lineBuffer = this.lineBuffer.substr(index + 1);
  }

};

SerialConnection.prototype.onReceiveError = function(errorInfo) {

  if (errorInfo.connectionId === this.connectionId) {
    console.log("connection error detected");
    this.onError.dispatch(errorInfo.error);
  }
};

SerialConnection.prototype.connect = function(path) {
  serial.connect(path, this.onConnectComplete.bind(this))
};

SerialConnection.prototype.send = function(msg) {
  console.log('passing the message "'+msg+'" to the array buffer conversion function');
  if (this.connectionId < 0) {
    throw 'Invalid connection';
  }
  serial.send(this.connectionId, str2ab(msg), function() {});
  //chrome.serial.send(integer connectionId, ArrayBuffer data, function callback)
};

SerialConnection.prototype.disconnect = function() {
  if (this.connectionId < 0) {
    throw 'Invalid connection';
  }
  serial.disconnect(this.connectionId, function() {});
};

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

var connection = new SerialConnection();

connection.onConnect.addListener(function() {

  log('connected to: ' + DEVICE_PATH);
  //console.log('connected to: ' + DEVICE_PATH);
  //connection.send("hello arduino");
  connection.send('T'+new Date().getTime()); // these are never received by the arduino
  //connection.send('T1430342577');
});

connection.onReadLine.addListener(function(line) {
  console.log(typeof(line)); // confirm that arduino is sending the expected string
  // above will always be a string because of function ab2str
  //if(jQuery.parseJSON(line)===0) {
  //  console.log("time being sought");
  //  connection.send('T1430342577');
  //}
  console.log(line);
  logJSON(line);
});
connection.onReadLine.addListener(function(bell) {
  if(jQuery.parseJSON(bell)===0) {
    console.log("time being sought");
    connection.send('T'+new Date().getTime());
  }
});

connection.connect(DEVICE_PATH);
//connection.send('T'+new Date().getTime());

function logJSON(status) {
  // console.log("led status function being called");

  // Get the LED status from the Json returned by the Serial
  // 0 = off | 1 = on
  // console.log((jQuery.parseJSON(status)));
  ledstatus = jQuery.parseJSON( status ).ledStatus ;
  // console.log(status);
  // Set the circle color according with the LED status
  if (ledstatus == 0)
     $('#statusCircle').css('fill','red');
  else
    $('#statusCircle').css('fill','green');

  // Print led Status to HTML buffer area
  log(ledstatus);
  console.log(ledStatus);
}


function log(msg) {
  $('#buffer').append(msg + '<br/>');
}

var is_on = false;
$('#toggle').click(function() {

  is_on = !is_on;
  connection.send(is_on ? 'y' : 'n');

  // 'T1430342577'
});
$('#time').click(function() {
  connection.send('T1430342577');
  // when received back the byte count behaves erratically
});

$('#numbers').click(function() {
  connection.send('1234567891011'); // above behaviour appears to be a flaw of sending numbers via a UTF8 buffer
});

$('#verbose').click(function() {
  connection.send('a reallly really long set of chars');
});

