#include <Time.h>

// Time Keeping
unsigned long currentMs;

// Lights
/* simulated day lengths required */

unsigned long dayLengthStep = 125; // the value which when elapsed in microseconds will dim the lights of the evolvinator
unsigned long oldDayLengthRead = 0; // this value is used with currentMs for the arduinos internal timekeeping
int lightArc = 0; // this value is written to the analog pulse width modulation, range: 0 - 255
int lightStep = 1; // amount to increment/decremenet lightArc value to simulate light dimming 

void setup() {

  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  setTime(15,43,0,20,5,15); // set any time you like here in format (hr,min,sec,day,month,yr)

}

void loop() {
  currentMs = millis();
  analogWrite(11, lightArc);
  analogWrite(10, lightArc);
  analogWrite(9, lightArc);
  if (currentMs - oldDayLengthRead > dayLengthStep){
    // this loop checks to see if the dayLengthStep value has elapsed in microseconds, 
    // if it has the light intensity of the LEDs will be adjusted
    lightArc = lightArc+lightStep;
    oldDayLengthRead = currentMs;
    // transition from day to night
    if (lightArc == 0 || lightArc == 255) {
      // this conditional triggers the LEDs to decrement once value 255 (max PWM intensity is reached)
      lightStep = -lightStep;
    }
//  delay(dayLengthStep);
  }
}
