Initial Sketch Size: Binary sketch size: 43,314 bytes (of a 258,048 byte maximum)

Arduino Uno Sketch Size Maximum == 32,256 byte maximum

Code will execute upon upload, even if SD, ethernet and sensors hardware dependencies are not met

Debugging Arduino IDE on Linux

Successful workarounds
-  rm .arduino/preferences.txt 
supplying Mega Board with source of voltage that isn't a USB hub. Fluctuations in some USB power outages = out of sync errors
When supplying external power heed summary section here http://www.arduino.cc/en/Main/arduinoBoardMega and ensure that USB is connected first (so that port is detected) then external power is supplied
Other Causes For Fail well explained here http://arduino.stackexchange.com/questions/739/arduino-program-only-works-when-run-as-root

Potential Blockers
Can chrome serial reliably run for the days at a time durations required by the directed evolution set up.
Can the script be calibrated in such a way as to know that the arduino is in the middle of a run?

