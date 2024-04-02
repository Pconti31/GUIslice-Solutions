# busywait animation

Animation that indicates task will run indeterminate length of time.
 - Animation example
 - Demonstrates using multiple cores on ESP32

<p>
Often you have a need to run sensors or move motors
in the background. There are times when you can't predict 
how long such a task will take to execute. This prevents you
from using something like a progress bar to notify users of
the required time duration. 
</p>

<p>
This is when an animation like a spinning wheel can be used
to at least indicate to your user that they need to wait for
something to complete before moving forward.
</p>

XBusyWait extended element supports three different animations
 - Balls that spin around in a circle.
 - A spinning wheel
 - Three balls in a horizontal row blinking on and off.

<p>
The XBusyWait.h file has complete information on usage and warnings. 
While the sample application has various options you can set by 
modifying various #defines. 
</p>

<p>
Basically you just need to provide a popup page with a box defined anywhere
on this page of any size witha fill color of your background and yield 
callback function as in the example to use this extended UI element.
</p>

<p>
The sample is set for ESP32 using TFT_eSPI graphics library and a standard
320x240 pixel display but you can use the GUIsliceBuilder to set these
to any supported platform. Simply use the PROJECT_OPTIONS tab inside 
the builder and choose your IDE, screen size and graphics package.
</p> 

<p>
The sample uses ESP32's core 1 for GUIslice's UI and core 0 for background task
to calculate PI to 74 digits when you touch the calculate PI button.
</p>
