#include <SevenSeg.h>
#include <avr/interrupt.h>
// aim for 6 minutes

// Time Keeping
unsigned long currentMs;

// Lights
/* simulated day lengths required */

unsigned long dayLengthStep = 65; // the value which when elapsed in microseconds will dim the lights of the evolvinator
unsigned long oldDayLengthRead = 0; // this value is used with currentMs for the arduinos internal timekeeping
//int lightArc = 0; // this value is written to the analog pulse width modulation, range: 0 - 255
//int lightStep = 1; // amount to increment/decremenet lightArc value to simulate light dimming 
float lightStep = 0.34997998; // 0.35625
float lightArc = 0;
float top = 255.00;
float bottom = 0.00;
// seg display

/*
int digit1 = 11; //PWM Display pin 12
int digit2 = 10; //PWM Display pin 9
int digit3 = 9; //PWM Display pin 8
int digit4 = 6; //PWM Display pin 6

//Pin mapping from Arduino to the ATmega DIP28 if you need it
//http://www.arduino.cc/en/Hacking/PinMapping
int segA = A1; //Display pin 11
int segB = 2; //Display pin 7
int segC = 4; //Display pin 4
int segD = 12; //Display pin 2
int segE = A0; //Display pin 1
int segF = 7; //Display pin 10
int segG = 8; //Display pin 5

*/

SevenSeg disp(A1, 2, 4, 12, A0, 7, 8);
const int numOfDigits=4;
int digitPins[numOfDigits]={11,10,9,6};

void setup() {
  // put your setup code here, to run once:
  disp.setDigitPins(numOfDigits,digitPins);
  //disp.setRefreshRate(60);
  disp.setTimer(2);
  disp.startTimer();
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  //disp.setDigitDelay(2000); // loops through 1000 every second
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //disp.write(1358);
//  disp.writeClock(00,00);
//  writeFloat();

  for(int i = 00; i < 24; i++){
    for(int j = 00; j < 60; j++){
      disp.writeClock(i,j);
      delay(250); // 250 = 6 minute 24 hour period
      lightTransition();
    }
  }

}

void lightTransition(){
//  currentMs = millis();
  analogWrite(3, lightArc);
  analogWrite(5, lightArc);
//  if (currentMs - oldDayLengthRead > dayLengthStep){
    // this loop checks to see if the dayLengthStep value has elapsed in microseconds, 
    // if it has the light intensity of the LEDs will be adjusted
    lightArc = lightArc+lightStep;
//    delay(250);
//    oldDayLengthRead = currentMs;
    // transition from day to night
    if (lightArc <= bottom || lightArc >= top) {
      // this conditional triggers the LEDs to decrement once value 255 (max PWM intensity is reached)
      lightStep = -lightStep;
    }
//  delay(dayLengthStep);
//  }
}

ISR(TIMER2_COMPA_vect){
  disp.interruptAction();
}
