Initial Sketch Size: Binary sketch size: 43,314 bytes (of a 258,048 byte maximum)

Arduino Uno Sketch Size Maximum == 32,256 byte maximum

Code will execute upon upload, even if SD, ethernet and sensors hardware dependencies are not met

Yet To Implement

SD debugging

Native SD debug info

the SD card logging specified within evolvinator works, however it is not possible to debug the SD card via CardInfo (example sketch) when shield is connected to Mega. Presently checking successful logging requires loading SD into main computer, or loading it to an Uno board with CardInfo script (and utilising a different SD shield)

// If you are using multiple shields, make sure each has a unique mac address.

^~> Will need to liase with Erich regarding the network setup of the gallery space

Debugging Arduino IDE on Linux

Successful workarounds
-  rm .arduino/preferences.txt 
supplying Mega Board with source of voltage that isn't a USB hub. Fluctuations in some USB power outages = out of sync errors
When supplying external power heed summary section here http://www.arduino.cc/en/Main/arduinoBoardMega and ensure that USB is connected first (so that port is detected) then external power is supplied
Other Causes For Fail well explained here http://arduino.stackexchange.com/questions/739/arduino-program-only-works-when-run-as-root
