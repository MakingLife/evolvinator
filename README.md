A trimmed down version of the evolvinator, currently sized at 22184 btyes so will fit on an Uno

The ethernet functionality is removed, timekeeping by the arduino is done by manually entering the time via a 

	setTime(hr,min,sec,day,month,yr) 

function call in void setup()

Deprecating the ethernet functionality means that interactivity is removed

This means that the only way to log and observe values measured by arduino is via Serial Monitor

the following calibration is no longer possible

*ODCalibrate(); // zero the OD sensor | photodiode

*addMedia(); // manually add media outside of the usual pulses

which means the operation of the script is as follows

*ODRead();

*pulseFeed();

*tempRead(); // Check temp every 5 seconds

*tempWrite(); // PID adjust every 10 seconds

*timeCheck();

*startRun(); // conditionally execute once

Several hardcoded calibration values are set within the script as follows

There are also several functions which may require calibration based on the hardware operation

====Calibration Parameters====

	# 1 feed pulsing
	# globals

	feedFrequency // inherit from evolvinator.ino

	# privates

	float flowMax = 40; // Max flow rate in ml/hr
	float volume = 30; // The chamber volume

	int pulseFreq; // hz to set to pump, set through function 1a
	float pulseVol; // volume of pulse, set through function 1a

	# getters & setters

	def flowSet():

	pulseFreq = int((flowMax * (3600 / 200) + 1.1148) / 0.6757) + 1
	pulseFreq = constrain(pulseFreq, 31, 200);

	defaults:
	// pulse duration of 10 seconds and 3 minute freq
	    pulseDuration = 10000;
	    feedFrequency = 180000;
	    pulseVol = flowMax / 20;

	# 2 Optical Density Sensing
	# globals // inherit from evolinator.ino

	ODMin[0] = ODMin[0] * .9861 + 0.0058;

	# getters & setters

	digitalWrite(pinValve, HIGH);                   // close valve
  	delay(2000);                                    // delay to let bubbles dissapate


  	# 3 Temperature Control

  	# globals - inherits many parameters from evolvinator.ino

  	# privates

  	tempDesiredPID;

  	# getters and setters
  	tempDesiredPID = double(tempDesired * 0.9569 - 6.7693);
  	tempPrintAvg = temp1MinAvg * 1.0401 + 7.2117;                  // convert to temp of water for print

	# 4 Time

	timeCheck() function relies on hardcoded year (like 2015)
