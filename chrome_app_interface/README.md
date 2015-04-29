This is a fork of <a target="_blank" href="https://chrome.google.com/webstore/detail/bdiclhdalonemjdeeaglackjgdboboem">Fabio Biondi's instructive tutorial</a> on doing basic two way communication between a Chrome App and an arduino

# Arduino LED toggle

Note: on Mac OS X Lion, it's necessary to update USB Serial drivers:
http://geekscape.posterous.com/mac-os-x-17-lion-upgrading-ftdi-usb-serial-dr

This sample shows a big button that lets you toggle between the on/off
state of an LED connected to an Arduino.

1. Install the LED sketch on your Duo.
2. Attach a LED to pin 2 (with a resistor to not burn it out).
3. Install and launch this packaged app.
4. Press the button to toggle the LED.

Future version: use the standard Firmata sketch and build a JS firmata driver
for Chrome packaged apps.

## APIs

* [Serial API](http://developer.chrome.com/apps/app.hardware.html#serial)
* [Runtime](http://developer.chrome.com/apps/app.runtime.html)
* [Window](http://developer.chrome.com/apps/app.window.html)
     
## Screenshot
![screenshot](https://raw.github.com/GoogleChrome/chrome-app-samples/master/serial/ledtoggle/assets/screenshot_1280_800.png)