/* 1 Flow Control -----------------------------------------------------------
 1a flowSet
 1b pulseFeed
 1c addMedia 
 */

/* >>>>>>>>>>>>>>>>>>>>> Constants and Variables <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

float flowMax = 40;                       // Max flow rate in ml/hr
float volume = 30;                        // The chamber volume

int pulseFreq;                            // hz to set to pump
unsigned long pulseDuration = 10000;      // default is 10 second pulse (do not change)
float pulseVol;                           // volume of pulse
float totalVol = 0;                       // total volume added
int pulseCount = 0;


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Functions<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
// 1a flowSet -- changes the frequency and pulse duration settings
void flowSet() {
  pulseFreq = int((flowMax * (3600 / 200) + 1.1148) / 0.6757) + 1; // convert max flow to Hz (10 second pulse 20 times an hour) **FROM CALIBRATION**
  pulseFreq = constrain(pulseFreq, 31, 200);
  if (pulseFreq == 200) {                                          // if flow Hz is maxed, change pulse duration
    pulseDuration = long(flowMax / 134.025 * 3600 / 20 * 1000);    // 134 is ml/hr at 200 hz **FROM CALIBRATION**
    feedFrequency = 180000;
    pulseVol = flowMax / 20;
  }
  else if (pulseFreq == 31) {                                      // if the flow rate is slow decrease the feed frequency 
    pulseDuration = 10000;
    feedFrequency = long(1000 / (flowMax / 19.8319 / 10));         // 19.8319 is ml/hr at 31 Hz **FROM CALIBRATION**
    pulseVol = 19.8319 / 360;
  }
  else {                                                           // else pulse duration should be 10 seconds and 3 minute freq
    pulseDuration = 10000;
    feedFrequency = 180000;
    pulseVol = flowMax / 20;
  }

  // Debug
  if (debugMode) {
    Serial.print("Flow Hz = ");  
    Serial.println(pulseFreq);
    Serial.print("Pulse Duration = ");
    Serial.println(pulseDuration);
  }
}

// 1b pulseFeed 
void pulseFeed() {
  float flowrateInstant = pulseVol * 3600 / (now() - tPulse);
  pulseCount += 1;  
  tone(pinP1FlowWrite, pulseFreq, pulseDuration);
  totalVol += pulseVol;
  tPulse = now();
  // SDDataLog('f', pulseCount, flowrateInstant);
}

// 1c addMedia - adds x ml of media
void addMedia(float addMediaMl) {
  long addDuration = long(addMediaMl / 0.03723 * 1000); // 0.03723 is the ml/s flow rate at 200 Hz
  tone(pinP1FlowWrite, 200, addDuration);
  if (tStart) {
    totalVol += addMediaMl;
    pulseCount += 1;
    float flowrateInstant = pulseVol * 3600 / (now() - tPulse);
    // SDDataLog('f', pulseCount, flowrateInstant);
  }
  return;
}



