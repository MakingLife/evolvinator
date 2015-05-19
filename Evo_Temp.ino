/* 3 Temperature Control -----------------------------------------------------------
 3a tempSet
 3b tempRead
 3c tempWrite
 */

/* >>>>>>>>>>>>>>>>>>>>> Constants and Variables <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Functions<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
// 3a tempSet
int tempSet() {
  tempDesiredPID = double(tempDesired * 0.9569 - 6.7693);   // ** FUNC FROM CALIBRATION**  [C]
  return tempDesiredPID;
}

// 3b tempRead
int tempRead() {
  // Local Variables
  static float temp1Min[12];         // readings from last 12 measurements
  float temp1MinAvg;
  int Vsupply = 4700;                // supply voltage in 10^-4 V (silly units needed for integer math) should be 5 V
  int ADCValue;                      // value read from temp sensor (0-1023) converted to 0-5V (4.9mV resoltuion)

  // Find temperatue 
  ADCValue = analogRead(pinTempRead);                            // read voltage from temp sensor into int 0-1023
  ADCValue = map(ADCValue, 0, 1023, 0, 5000);                    // convert to V * 10^-4
  temp = (((ADCValue * (5000 / Vsupply)) - 1375)) / 22.5 ;       // convert to C. **TRANSFER FUNCTION FROM Analog Devices AD22100 DATASHEET**

  // Calculate 1 min average and convert for printing
  for (int x = 11; x > 0; x--) {                                 // move data down 
    temp1Min[x] = temp1Min[x - 1];
  }
  temp1Min[0] = temp;  
  temp1MinAvg = 0;                                       
  for (int x = 0; x < 12; x++) {
    temp1MinAvg += temp1Min[x];
  }
  temp1MinAvg = temp1MinAvg / 12;
  tempPrintAvg = temp1MinAvg * 1.0401 + 7.2117;                  // convert to temp of water for print **FUNC FROM CALIBRATION**

  if (debugMode) {
    Serial.print(temp);
    Serial.print(",tempPrintAvg,");
    Serial.print(tempPrintAvg);
    Serial.print(",tempPWM,");
    Serial.print(tempPWM);
    Serial.print(",tempDesiredPID,");
    Serial.println(tempDesiredPID);
  }

  return tempPrintAvg;
}  

// 3c tempWrite
void tempWrite() {

  // check to see how far temp is off set point, and adjust aggressiveness of PID
  double gap = abs(tempDesired - temp);
  if(gap < 2) tempPID.SetTunings(consKp, consKi, consKd);
  else tempPID.SetTunings(aggKp, aggKi, aggKd);

  tempPID.Compute();
  // analogWrite(pinTempWrite, tempPWM);
}


