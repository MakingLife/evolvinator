Significant amendments made to initial evolvinator code for purposes of exhibition.

Additionally several more compact arduino sketches are included here (within /companion_sketches). These scripts were loaded to separate arduino boards to provide evocative illumination of the Star Beasts object, respectively they are EarthDayLength and SevSegClock.

A dramatic amount of functionality has been deprecated in order to have the unit functioning without ethernet connectivity and debug ability, as was the case in the gallery

Optical Density sensing is deprecated due to time constraints foreclosing accurate correlation of cyanobacteria absorbtion rates to the visual density desired for exhibition purposes

Temperature loop is deprecated as cyanobacteria are robust at room temperature

The flow is to be recalibrated based on driving the motor via relay or on board voltage

Ethernet functionality remains intact, but gallery installation did not avail of this function

###Installation

The .ino scripts in the root directory are the files required to run the evolvinator apparatus

The folders companion_sketches/EarthDayLength and companion_sketches/SevSegClock are self contained arduino scripts designed to be used with a board of LEDs and a LEDs plus Seven Segment Display respectively. To install them they must be extracted from the root folder of the evolvinator

Your local install of arduino needs to use the following libraries 

~~evolvinator
SPI.h  
Ethernet.h  
EthernetUdp.h  
Time.h  
SD.h  

~~companion scripts

SevenSeg.h # an absolutely EXCELLENT and well documented library for controlling seven segment displays  
avr/interrupt.h  
