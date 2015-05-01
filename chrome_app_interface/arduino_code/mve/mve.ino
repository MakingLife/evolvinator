// the minimum viable evolvinator

#include <Time.h>

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
const byte pinValve = 3;                  // pin that controls the valve

void setup(){
  Serial.begin(9600);
  setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011 
  // pass in some generic vals
}

// this code will be run data, main void loop()
void loop() {
  // If run has started
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
// this code handles interactivity from the interface
  char c;
  if (Serial.available()) {
    c = Serial.read();
  }
  else {
    return;
  }
  
  switch (c) {
  // the two cases we are interested in presently are 
  startRun();
 
  dataRead(); // download data
  
  }
 }

void dataRead(){
  // a function to handle storing data
} 
 
void startRun() {
  tStart = now();
  tElapsed = 0;
  tUnixStart += (millis() - msElapsedPrestart) / 1000;    // to adjust unix time
  tUnix = tUnixStart + tElapsed;
  // SDInitialize();
  digitalWrite(pinValve, LOW);          // open air valve ~ subsitute with a LED
}

void timeCheck(){

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
