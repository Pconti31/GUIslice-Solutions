# Example App that integrates runtime TFT Screen Calibration 

<p>
Example of Application that can make use of touch screen
calibrations for XMIN, XMAX, YMIN, YMAX values stored
either on SD Card, EEPROM or Flash thus overriding values stored in
compiled code taken from GUIslice/config/*.h files.
Flash Storage requires the library:
https://github.com/cmaglie/FlashStorage and has been tested with
ARM Cortex M0+ based board 
</p>

<p>
The provided class TFT_Calibration can also do new runtime 
calibration to allow a user to fine tune an existing hard-coded
configuration.
</p>

<p>
This should be a major help to those supplying resistive touch displays
to users of their applications.
</p>

<p>
Expected behavior: Clicking on CALIBRATE button outputs a message
along with instructions on how to calibrate your screen.
</p>

<p>
After completing all steps the values are saved to SD Card or EEPROM.
Optionally if your TFT_Calibration.h has DEBUG_CALIB = 1 output 
to serial monitor will display the new values.
</p>

<p>
The sample is set for Arduino IDE using Adafruit_GFX library and a standard
320x240 pixel display but you can use the GUIsliceBuilder to set the
your display to another size or to use the TFT_eSPI library. 
Simply open the use the "runtime_calibration.prj" file within the GUIsliceBuilder, 
select the PROJECT_OPTIONS tab and make your changes before doing asave and 
new code generation. 
</p> 

## NOTES:
<p>
Don't forget to read the NOTES and WARNINGs sections inside "TFT_Calibration.h"
for TFT_Calibration class usage.
</p>

<p>
One thing that may not be obvious is how to use this new class with my 
screen saver class since they both require calling their own update 
function. The answer is to ignore the Calibration update() function and 
and instead do this... 
</p>


```
loop() {
 .
 .
 .
  
  if (calibration.isActive()) {
    calibration.loop();
  } else {
    // Our ScreenMgr calls gslc_Update when required
    // DO NOT call it directly or your screen will get messed up!
    //gslc_Update(&m_gui);
    screenMgr.update();
  }

```
    

