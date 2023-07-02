
# Screen Saver Application

<p>
Demonstrates how to make screen saver that appears after 10 seconds of 
no activity by a user. Of course, this timeout can be set to any number 
seconds desired. This screen saver will display an animation of Bitmap 
like a company logo.  It requires using Adafruit_GFX graphics driver.
</p>

<p>
However, ScreenManager can be used by any GUIslice supported 
MCU, Graphics library and display driver. 
</p>

<P>
The Bouncing Logo animation has a few configable parmeters that may be required
depending upon the speed of your micro-controller and size of your logo.
x_delta is the x distance to move your logo bitmap on each frame, y_delta is
y distance. delete_ms is the number of milliseconds between frames, default is 5.
</p>

<p>
Your logo needs to be in flash or RAM and follows GUIslice convensions of having the first
two words of your image being height and width.  You may use my GUIsliceImage2C utility
to convert png or jpg files to the requided format.
</p>

<p>
The ScreenManager avoids you having to create a screen-sized button or any
extra elements or pages to provide for a screensaver wake-up. It handles all of
implementation details for timing, screen blackout and restore itself.  
You just need to provide callback routines for pause(), resume(), and display(). 
</p>

<p>
This sample is set for Adafruit_GFX library and a standard 
320x240 pixel display. If you avoid using the library BouncingLogoAnim 
you can use any graphics library and MCU. For example, you could simply 
turn your display's backlight on/off. Just use the PROJECT_OPTIONS tab 
inside the GUIsliceBuilder and choose your IDE, screen size and graphics 
package. 
</p> 
