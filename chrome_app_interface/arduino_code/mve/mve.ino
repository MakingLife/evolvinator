// the minimum viable evolvinator

// git commit 184f922bb0e4cc597c0a4f63db7d0265ef9d75b7 contains a version where time sync occurs and the interface command will reach the controller
// consult git commit 9338f23f393e3355f71d7419548a1e894f954e41 for a main.js and mve.ino that play well together

  #include <Time.h>
  #define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
  #define TIME_HEADER  'T'   // Header tag for serial time sync message
  #define TERM_FOOTER 'Q'
  
  unsigned long currentMs;
  unsigned long msElapsedPrestart;
  unsigned long tUnixStart;                 // unix time at run start
  unsigned long tUnix;                      // unix time
  unsigned long countdown;
  time_t tStart;                            // starting time
  time_t t;                                 // current time
  time_t tElapsed;                          // elapsed time (s)  
  
  boolean userInput = false;
  
  // OD aka generic sensor
  unsigned long oldMsODRead = 0;
  const byte pinODLED = 1;                  // pin that powers the OD LED
  const byte pinODRead = A1;                // pin that reads the OD sensor
  const byte pinValve = 13;                  // pin that controls the valve
  
  void setup(){
    Serial.begin(9600);
    setTime(8,29,0,2,5,15); 
    // (hr,min,sec,day,month,yr)
    // set time to some generic vals
    pinMode(pinValve, OUTPUT);  
  }
  
  // this code will be run data, main void loop()
  void loop() {
    Serial.println("loop() begin");
    // If run has started
    digitalWrite(pinValve, LOW);
    if (tStart) {
      Serial.println("measure me some sensors");
      // take a sensor measurement
      currentMs = millis();
      if (currentMs - oldMsODRead > 60000) {
        SensorRead(); 
        // call to a sensorRead function which should mimic the data logging call
        oldMsODRead = currentMs; 
        // handles doing this once a minute without cause for external time reference
      }
    }
    // in the other code there are measurements that will happen irrespective of a run having been started
    digitalClockDisplay();
      // Check and adjust time if neccessary
    currentMs = millis();
    timeCheck();  // only time that a call to check external time is necessary
    // delay(1000);// this superfluous if using delay within timecheck executions itself
    // interface();
  }
  
    void timeCheck(){
      // this code also has to call to the Serial
      // so what would be ideal here is some way of it knowing that a serial connection has been made to the chrome app - like a handshake
      Serial.println(0);
      delay(1000);
      timeSync();

    } // end function
  
  
void timeSync() {
  // if time sync available from serial port, update time and return true

  // while(Serial.available()){
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits  
    // this while is not executing
    // time message consists of a header and ten ascii digits
    char c = Serial.read() ; 
    // Serial.print(c);  
    if( c == TIME_HEADER ) { // this never executes       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   // end for
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
      return;
    } // end if 
  } // end while

}
  
  // void interface(){
  void serialEvent() {
    Serial.println(2);
    //delay(1000);
  // this code handles interactivity from the interface, ergo Serial
    char s;

    while(Serial.available()) { 
      // basically pauses the program until input received,returns 0 when no serial has been sent, not 0 = 1
    
      s = Serial.read();
      
      switch (s) {

          case 'y':
            // do
            Serial.print("you have entered ");
            Serial.print(s);
            Serial.println(" this means you wish to start a run");
            startRun();
            break;
          case 'n':
            //do
            break;
          /* case 'd':
            dataRead();
            break; */
          
        }
    } // end while
//    else {
//      return;
//    }
  }
  

  
  void dataRead(){
    // a function to handle storing data
  } 
   
  void startRun() {
    tStart = now();
    Serial.print("run commenced at: ");
    digitalClockDisplay();
    tElapsed = 0;
    tUnixStart += (millis() - msElapsedPrestart) / 1000;    // to adjust unix time
    tUnix = tUnixStart + tElapsed;
    // SDInitialize();
    digitalWrite(pinValve, HIGH);          // open air valve ~ subsitute with a LED with attendant change on the logic
    delay(10000);
    Serial.print("run concluded at: ");
    digitalClockDisplay();
  }
  
  void SensorRead(){
  
    // do the analog read stuff
    
    SDDataLog(pinODRead);
    
  }
  
  // 6b
  void SDDataLog(float value){
    // Find time as above functions have delay 
    t = now();
    tElapsed = t - tStart;
    tUnix = tUnixStart + tElapsed;
    
      // Time
    Serial.print(tUnix);
    Serial.print(",");
    Serial.print(month(t));
    Serial.print("/");
    Serial.print(day(t));
    Serial.print("/");
    Serial.print(year(t));
    Serial.print(" ");
    Serial.print(hour(t));
    Serial.print(":");
    if (minute(t) < 10) {
      Serial.print("0");
    }
    Serial.print(minute(t));
    Serial.print(":");
    if (second(t) < 10) {
      Serial.print("0");
    }
    Serial.print(second(t));
    Serial.print(",");
    Serial.print(tElapsed);
    Serial.print(",");
    Serial.println(value);
  
  }
  
  // human readable formatting
  
  void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}
void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
