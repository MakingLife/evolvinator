// the minimum viable evolvinator

  #include <Time.h>
  #define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
  #define TIME_HEADER  'T'   // Header tag for serial time sync message
  #define TERM_FOOTER 'Q'
  
  unsigned long currentMs;
  unsigned long msElapsedPrestart;
  unsigned long tUnixStart;                 // unix time at run start
  unsigned long tUnix;                      // unix time
  time_t tStart;                            // starting time
  time_t t;                                 // current time
  time_t tElapsed;                          // elapsed time (s)  
  
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
    // If run has started
    digitalWrite(pinValve, LOW);
    if (tStart) {
      
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
    
      // Check and adjust time if neccessary
    currentMs = millis();
    timeCheck();  // only time that a call to check external time is necessary
    interface();
  }
  
  void interface(){
  // this code handles interactivity from the interface, ergo Serial
    char c;
    if (Serial.available()) {
      c = Serial.read();
        switch (c) {
        /* the two cases we are interested in presently are 
        startRun();
        dataRead(); // download data
        */
          case 'y':
            // do
            startRun();
            break;
          case 'n':
            //do
            break;    
        }
    }
    else {
      return;
    }
    
  }
  void timeCheck(){
    // this code also has to call to the Serial
    // so what would be ideal here is some way of it knowing that a serial connection has been made to the chrome app - like a handshake
    Serial.println(0);
    //if (Serial.available()) {
        //Serial.println("arduino attempt to set time"); // definitely detecting the transmission of serial
        timeSync();
    //} // end first  if
    delay(1000);
  } // end function
  
  
  void timeSync(){
        //while(Serial.available() >=  TIME_MSG_LEN ){
         while(Serial.available()){  // time message consists of a header and ten ascii digits
          char c = Serial.read() ; 
          Serial.print(c);  
          //if( c == TIME_HEADER ) {
            // Serial.println("time header logic functional as expected");       
            time_t pctime = 0;
            for(int i=0; i < TIME_MSG_LEN -3; i++){   
              c = Serial.read();
              Serial.println(c);          
              if( c >= '0' && c <= '9'){   
                pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
              } else if (c == TERM_FOOTER) {
                return;
              } 
            }   // end for
            setTime(pctime);   // Sync Arduino clock to the time received on the serial port
            return;
          // } // end if header 
        } // end while
  }
  
  void dataRead(){
    // a function to handle storing data
  } 
   
  void startRun() {
    tStart = now();
    Serial.print("tStart time = ");
    Serial.println(tStart);
    tElapsed = 0;
    tUnixStart += (millis() - msElapsedPrestart) / 1000;    // to adjust unix time
    tUnix = tUnixStart + tElapsed;
    // SDInitialize();
    digitalWrite(pinValve, HIGH);          // open air valve ~ subsitute with a LED with attendant change on the logic
    delay(10000);
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
