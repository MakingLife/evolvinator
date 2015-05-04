A trimmed down version of the evolvinator

The ethernet functionality is removed, timekeeping by the arduino is done by manually entering the time via a setTime(hr,min,sec,day,month,yr) function call in void setup()

Several hardcoded calibration values are set within the script as follows

Calibration Parameters

	# 1
	# globals

	feedFrequency // from evolvinator.ino

	# privates

	float flowMax = 40; // Max flow rate in ml/hr
	float volume = 30; // The chamber volume

	int pulseFreq; // hz to set to pump, set through function 1a
	float pulseVol; // volume of pulse, set through function 1a

	def flowSet():

	pulseFreq = int((flowMax * (3600 / 200) + 1.1148) / 0.6757) + 1
	pulseFreq = constrain(pulseFreq, 31, 200);

	defaults:
	// pulse duration of 10 seconds and 3 minute freq
	    pulseDuration = 10000;
	    feedFrequency = 180000;
	    pulseVol = flowMax / 20;

	# 2

	ODMin[0] = ODMin[0] * .9861 + 0.0058;


	#4

	timeCheck() function relies on hardcoded year (like 2015)
