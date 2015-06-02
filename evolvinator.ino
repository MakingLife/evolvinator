/*
Evolvinator
 
 REV 1
 
 The Evolvinator uses an Arduino MEGA for its expanded memory
 
 It uses an ethernet shield to connect to the NTP servers (timekeeping) and to provide a debug interface
 
 The following PINS are reserved
 reserves the pins for 50, 51, 52 SPI communication (the same as in the ICSP connector) and pin 53
 reserves pin 10 Ethernet circuit choice
 reserves pin 4 for SD memory card
 
 pins 13, 12, 11, 9, 6 available for pulse width modulation
 
 This program controls the Evolvinator. There are 3 main components:
 Pump Control - dictates the speed of the p-1 perstaltic pump
 OD Sensor - makes OD measurements
 - also controls the valve 
 Temp Control - monitors the temperature and feeds back to the heat resistors
 
 Search ENTER to see where you need to input parameters
 Search CALIBRATION to see where you need to input calibration data
 */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>> Constants and Variables <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
// Libraries
#include <SPI.h>
// a synchronous serial data protocol used by microcontrollers for communicating with one or more peripheral devices quickly over short distances
// not obvious where this is being utilised, would conflict with python controller I imagine
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Time.h>
#include <SD.h>

// Ethernet  
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0x4F, 0x74 };   // ENTER MAC address of ethernet shield
IPAddress ip(192, 168, 1, 88);          // ENTER IP address 

// static ip address to connect to
EthernetServer server(80);                // default web request server
EthernetUDP Udp; // UDP is used as the protocol and buffer to best retrieve the time from one of the NTP server

// Relay Modules
// currently employing an active low relay module, ergo different constants required for triggering the relay driven components

const boolean RELAYHIGH = !HIGH;
const boolean RELAYLOW = !LOW;

// Lights
/* simulated day lengths required */

unsigned long dayLengthStep = 5806; // the value which when elapsed in microseconds will dim the lights of the evolvinator
unsigned long oldDayLengthRead = 0;
int lightArc = 0;
int lightStep = 1;

// Time
unsigned long currentMs;
unsigned long oldMsODRead = 0;
unsigned long oldMsPulseFed = 0;         

const int localPort = 8888;               // local port to listen for UDP packets
byte timeServer[] = { 
  192, 43, 244, 18};                      // time.nist.gov NTP server
const int NTP_PACKET_SIZE= 48;            // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE];      // buffer to hold incoming and outgoing packets from NTP

time_t epoch; // only other reference to epoch is within Evo_Time and it is retrieved via the server
time_t tStart;                            // starting time
time_t t;                                 // current time
time_t tElapsed;                          // elapsed time (s)
time_t tPulse;                            // time of the last pulse
time_t tBackup;                           // reference time for if time sync with NTP server fails
unsigned long msBackup;                   // associated arduino clock time
unsigned long tUnixStart;                 // unix time at run start
unsigned long tUnix;                      // unix time
unsigned long msElapsedPrestart;          // ms elapsed run start.

// Flow
const byte pinP1FlowWrite = 8;            // which pin tells p1 (through pin 14) what speed (0-200 Hz)
unsigned long feedFrequency = 18000000;     // frequency of the pulses given (default 1 ever 3 minutes), 300000 = once every 5 minutes

// OD
const byte pinODLED = 7;                  // pin that powers the OD LED - move away from using the TX
const byte pinODRead = A1;                // pin that reads the OD sensor
const byte pinValve = 3;                  // pin that controls the valve
float ODDesired = 0.2;                    // Set desired OD
float ODMin[10];                          // stores recent OD measurements (current = ODMin[0]                         
float OD3MinAvg;
float ODZero = 0;                         // photodiode blank reading 

// Modes
boolean debugMode = true;
boolean calibrationMode = false;
boolean exhibitionMode = true;

// SD
const int pinSD = 4;


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Setup - runs once <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void setup() {
  // General
  analogReference(DEFAULT);                // analog input reference is 5 V

  // Serial, Ethernet
  Serial.begin(9600);
  pinMode(53, OUTPUT);                      // SS pin on Mega
  Ethernet.begin(mac, ip);
  server.begin();
  delay(1);                                 // give it a msecond to connect

  // Pump Control
  pinMode(pinP1FlowWrite, OUTPUT);          // output to pump what speed it should go (tone between 0-200 Hz)
  // flowSet(); // call to Evo_Flow.ino

  // OD Sensor
  pinMode(pinODLED, OUTPUT);                // pin that controls LED is output
  digitalWrite(pinODLED, LOW);              // light off by default
  pinMode(pinODRead, INPUT);                // pin that reads photodiode is input
  digitalWrite(pinODRead, HIGH);            // enable 20k pullup resistor
  pinMode(pinValve, OUTPUT);                // pin that controls valve is output
  digitalWrite(pinValve, RELAYLOW);              // valve open at start - humidifier should be normally closed

  // Lights
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  // Timer
  
  Udp.begin(localPort);
  
  // hardcoded time
  if (debugMode) {
    setTime(19,40,0,22,5,15); // (hr,min,sec,day,month,yr) CALIBRATION
    setSyncInterval(60 * 5);
  } else {
    // network synced time
    setSyncProvider(getTime);                 // sync interval default is 5 mins, uses NTP which in accessible via current simple LAN 
    setSyncInterval(60 * 5);
  }
  
  tUnixStart = tUnix; 
  tBackup = now();                          // set back up time
  msBackup = millis();                      // set assocated backup time on Arduino's internal clock
  msElapsedPrestart = millis();             // ms passed since power on to calculate unix time.
  if (debugMode) {
    Serial.print("Main Script ~ Back up time: ");
    Serial.println(tBackup);
  }
  
    // SD
  SD.begin(pinSD);
  
  digitalWrite(pinP1FlowWrite, RELAYLOW);
  digitalWrite(pinValve, RELAYLOW);

}

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Loop - is program <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void loop() {

    // Take OD measurement ever minute
    currentMs = millis();
    analogWrite(11, lightArc);
    analogWrite(12, lightArc);

    // Feed pulse if threshold is reached and it's been long enough
    currentMs = millis();
    
    if(exhibitionMode) {
      // if its exhibition mode we want the motor to pulse for a minute every 10 minutes? = 90ml an hour, = the entire bacteria is diluted
      
      if (currentMs - oldDayLengthRead > dayLengthStep){
        // change the lightVal
        lightArc = lightArc+lightStep;
        oldDayLengthRead = currentMs;
        // transition from day to night
        if (lightArc == 0 || lightArc == 255) {
          lightStep = -lightStep;
        }
      }

      
      // the LEDs are also designed to be mapped to a cyclic duration - day length abstracted to ms, using currentMs as the base 
      if(currentMs - oldMsPulseFed > feedFrequency){
        Serial.println(currentMs - oldMsPulseFed);
        // pulse for 30 seconds every 5 minutes, diluting culure in 100ml chamber by 5.5ml
          digitalWrite(pinValve, RELAYHIGH);                   // close valve ???
          delay(2000); 
          digitalWrite(pinP1FlowWrite, RELAYHIGH);
          delay(30000);
          digitalWrite(pinP1FlowWrite, RELAYLOW);
          delay(30);
          digitalWrite(pinValve, RELAYLOW);                    // turn air back on ???
        oldMsPulseFed = currentMs;
      }
       
    } 
}

boolean exhibitionPulse(){
  digitalWrite(pinValve, RELAYHIGH);                   // close valve ???
  delay(2000); 
  digitalWrite(pinP1FlowWrite, RELAYHIGH);
  delay(30000);
  digitalWrite(pinP1FlowWrite, RELAYLOW);
  delay(30);
  digitalWrite(pinValve, RELAYLOW);                    // turn air back on ???
  return true;
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Functions - List function calls below <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
 0 Start Run
 1 Flow Control
 1a flowSet
 1b pulseFeed
 1c addMedia 
 2 OD Sensor
 2a ODRead
 2b ODCalibrate
 3 Temperature Control
 3a tempSet
 3b tempRead
 3c tempWrite
 4 Time
 5 WebLoop
 6 SD Card
 6a SDInitialize
 6b SDDataLog
 6c SDWebLoad
 */

// 0 startRun  ----------------------------------
void startRun() {
  tStart = now();
  tElapsed = 0;
  tUnixStart += (millis() - msElapsedPrestart) / 1000;    // to adjust unix time
  tUnix = tUnixStart + tElapsed;
  SDInitialize();
  digitalWrite(pinValve, RELAYLOW);          // open air valve ????
}


