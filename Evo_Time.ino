// 4 Time -----------------------------------------------------------

void timeInit(){
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
      Serial.println(pctime);
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
      return;
    } // end if 
  } // end while

}

// timeCheck
void timeCheck() {
  t = now();
  if (year(t) == 2015 && currentMs - msBackup > 5 * 60000) {
    tBackup = now();
    msBackup = millis();
    if (debugMode) {
      Serial.print("Back up time: ");
      Serial.println(tBackup);
    }
  }
  if (year(t) != 2015) {
    if (debugMode) {
      Serial.println("Year is wrong: ");
      Serial.println(year(t));
    }
    setTime(hour(tBackup), minute(tBackup), second(tBackup), day(tBackup), month(tBackup), year(tBackup));
    int adjustTime = (currentMs - msBackup) / 1000;
    setTime(hour(tBackup), minute(tBackup) + adjustTime / 60, second(tBackup) + adjustTime % 60, day(tBackup), month(tBackup), year(tBackup));
    t = now();
    if (debugMode) {
      Serial.println("Time failed to reset correctly, backup used");
      Serial.print("Is year right? ");
      Serial.println(year(t));
    }
  }
  if (tStart) {
    tElapsed = t - tStart;
    tUnix = tUnixStart + tElapsed;
  }
}
