#include <Time.h>

// time constants

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

// bare communication constants
#define LED 13
#define DEBUG false

String time="12:00";

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
 
  // Turn Led off
  digitalWrite(LED, LOW);

  // Print the led Status to the serial in JSON format
  printLEDStatus();
  
  if(DEBUG){
    // make a call to the serial for a time value
    setSyncProvider( requestSync);
  }
  
}

int incomingByte = 0;
void loop() {

  
  // Check if there's a serial message waiting.
  if (Serial.available() > 0) {
    // If there is, read the incoming byte.
    incomingByte = Serial.read();
    
    switch(incomingByte){
      case 'y':
        // do
        digitalWrite(LED, HIGH);
        printLEDStatus();
        break;
      case 'n':
        //do
        digitalWrite(LED, LOW);
        printLEDStatus();
        break;
    }
    // if "y" turn on the led, otherwise turn off 
    /*if (incomingByte == 'y') {
      digitalWrite(LED, HIGH);
    } else if (incomingByte == 'n') {
      digitalWrite(LED, LOW);
    }*/

    // printLEDStatus();
  }
  
  delay(1000);

}

// Send the Led status via serial in JSON format
void printLEDStatus () {
   // read current LED status
    int ledStatus = digitalRead(LED);
    
    if(DEBUG){
      time = String(day())+" "+String(month());
      // time = dayStr()+" "+monthStr();
    }
    
    // Create the JSON to send
    String json = "{\"ledStatus\": " + String(ledStatus)+","
                   + "\"time\" :" + "12:00" 
                  + "}";
    
    // Print JSON via serial
    Serial.println(json);
  
}

// Time


void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
    }  
  }
}

time_t requestSync()
{
  Serial.print(TIME_REQUEST);
  return 0; // the time will be sent later in response to serial mesg
}

