/* 2 OD --------------------------------------------------------------------
 2a ODRead
 2b ODCalibrate
 */

/* >>>>>>>>>>>>>>>>>>>>> Constants and Variables <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

int diodeReading;                         // raw diode reading [0-1023]                      
float ODZeroPrint = 0;                    // convert to 1 cm 
int ODCount = 0;

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Functions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
// 2 ODRead 
float ODRead() {
  // Local Variables
  int photoRead;                                  // photodetector reading
  int diodeLightOnAvg = 0;
  int diodeLightOffAvg = 0;
  static unsigned long oldMsOD10MinAvg = 0;       // static variables are only declared the first time the function runs

  digitalWrite(pinValve, HIGH);                   // close valve
  delay(2000);                                    // delay to let bubbles dissapate

  for (int x = 9; x > 0; x--) {                   // move data down for 10 min avg
    ODMin[x] = ODMin[x - 1];
  }

  // Determine OD
  diodeReading = 0;
  for (int y = 0; y < 5; y++) {                   // average over 5 readings
    photoRead = 0;
    for (int x = 0; x < 30; x++) {                // consisiting of 30 readings each
      photoRead += analogRead(pinODRead);         // read data
      delay(10);
    }
    diodeLightOffAvg = photoRead / 30;            // average
    digitalWrite(pinODLED, HIGH);                 // turn light on
    delayMicroseconds(1);                         // give the photodiode a us to adjust
    photoRead = 0;
    for (int x = 0; x < 30; x++) {
      photoRead += analogRead(pinODRead);
      delay(10);
    }
    diodeLightOnAvg = photoRead / 30;
    diodeReading += diodeLightOnAvg - diodeLightOffAvg;
    digitalWrite(pinODLED, LOW);                  // turn light off
    delayMicroseconds(1);
  }

  digitalWrite(pinValve, LOW);                    // turn air back on

  diodeReading /= 5;
  if (diodeReading < 1) {                         // correct for super high OD
    diodeReading = 1;
  }
  ODMin[0] = float(diodeReading);
  ODMin[0] = (log(1 / (ODMin[0] / ODZero)) / log(10));
  ODMin[0] = ODMin[0] * .9861 + 0.0058;           // **TRANSFER FUNCTION FROM CALIBRATION**
  if (ODMin[0] < 0) {                             // set it to 0 if negative value is read
    ODMin[0] = 0;
  }
  OD3MinAvg = (ODMin[0] + ODMin[1] + ODMin[2]) / 3;

  ODCount += 1; 
  // SDDataLog('o', ODCount, 0);                     // save data point to SD card

  if (debugMode) Serial.println(ODMin[0]);

  return ODMin[0], OD3MinAvg, diodeReading;
}

// 2b ODCalibrate
float ODCalibrate() {
  int photoRead;                                 
  int diodeLightOnAvg = 0;
  int diodeLightOffAvg = 0;

  digitalWrite(pinValve, HIGH);                     
  delay(2000);                                     

  ODZero = 0;                                       
  for (int y = 0; y < 10; y++) {
    photoRead = 0; 
    for (int x = 0; x < 30; x++) {
      photoRead += analogRead(pinODRead);
      delay(10);
    }
    diodeLightOffAvg = photoRead / 30;
    digitalWrite(pinODLED, HIGH);                    
    delayMicroseconds(1);                            
    photoRead = 0;
    for (int x = 0; x < 30; x++) {
      photoRead += analogRead(pinODRead);
      delay(10);
    }
    diodeLightOnAvg = photoRead / 30; 
    ODZero += float(diodeLightOnAvg - diodeLightOffAvg);
    digitalWrite(pinODLED, LOW);
    delayMicroseconds(1);
  }
  ODZero /= 10;                                     

  if (tStart) {
    digitalWrite(pinValve, LOW);
  }

  if (debugMode) Serial.println(ODZero);

  return ODZero;
}



