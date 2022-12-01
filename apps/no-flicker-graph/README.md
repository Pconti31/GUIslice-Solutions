
# No Flicker Graphs Application

Demonstrates how to display your graphs without any major flicker.

<p>
This was oroginally a simple demonstration of how to use the extended element
XGraph since people kept asking how to use it.  Hpwever, I found the screen flicker
so annoying that I finally modified the original XGraph code to erase old data 
before display new data. If you run this on a ESP32 no flicker will be noticeable. 
On Arduino Mega some flicker will remain but its greatly reduced. 
</p>

<p>
I also removed the flat line at the bottom of the graph that appears 
upon starting your graph that represents empty data cells.
</p>

<p>
You should run this sample first using the orignal XGraph element that ships
with GUIslice API so you can notice the difference. 
</p>

<p>
To install my version simply copy XGraph.<c h> inside the sub-folder "elem" 
to your GUIslice/src/elem folder overwriting the original versions. 
</p>

<p>
Maybe a better implementation for people that use TFT_eSPI drivers 
would be to use sprites to draw the graph but that's left for the future.
</p>

<p>
The sample is set for Arduino using AdaFruit_GFX library and a standard
320x240 pixel display but you can use the GUIsliceBuilder to set these
to any supported platform. Simply use the PROJECT_OPTIONS tab inside 
the builder and choose your IDE, screen size and graphics package.
</p> 
