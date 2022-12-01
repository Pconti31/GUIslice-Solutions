
# LED Panel Application

<p>
Demonstrates how to use a Box Element with a Draw Callback to implement
a panel of LEDs. In this case 4 Leds, controlled by booleans stored in global space. 
You could use int8_t instead and use values from 0 to 2 to set three colors or more for each Led.
I also placed the Led Panel on a base page so all Leds would show up on each page of the UI. 
</p>

<p>
The sample is set for Arduino using AdaFruit_GFX library and a standard
320x240 pixel display but you can use the GUIsliceBuilder to set these
to any supported platform. Simply use the PROJECT_OPTIONS tab inside 
the builder and choose your IDE, screen size and graphics package.
</p> 
