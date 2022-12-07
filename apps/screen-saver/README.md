
# Screen Saver Application

<p>
Demonstrates how to make screen saver that appears after 10 seconds of 
no activity by a user. Of course, this timeout can be set to any number 
seconds desired. This screen saver will display an animation of Matrix 
movie like rain and requires using TFT_eSPI driver with either a ESP32 
or ESP8266. 
</p>

<p>
However, ScreenManager can be used by any GUIslice supported 
MCU, Graphics library and display driver. 
</p>

<p>
The ScreenManager avoids you having to create a screen-sized button or any
extra elements or pages to provide for a screensaver wake-up. It handles all of
implementation details for timing, screen blackout and restore itself.  
You just need to provide callback routines for pause(), resume(), and display(). 
</p>

<p>
ACKNOWLEDGEMENT: Matrix Rain animation **DigitalRainAnim** is from the project 
hosted on github: 
[MatrixRain](https://github.com/delhoume/MatrixRain)
</p>

<p>
This sample is set for ESP32 using TFT_eSPI library and a standard 
480x320 pixel display. If you avoid using the library DigitalRainAnim 
you can use any graphics library and MCU. For example, you could simply 
turn your displays backlight on/off. Just use the PROJECT_OPTIONS tab 
inside the GUIsliceBuilder and choose your IDE, screen size and graphics 
package. 
</p> 
