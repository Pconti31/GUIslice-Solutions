
# Sample Long Press of a Button Application

<p>
Demonstrates how to make a Text Button detect a long press.
Hold a increase or decrease for a second and counter will be modified.
Hold reset button for 3 or more seconds and counter will be reset to 0.
The button callbacks could of course be changed from modifying a counter to a jump 
to another page using the gslc_SetPageCur(&m_gui,E_PG_????); call.
</p>

<p>
The sample is set for Arduino using AdaFruit_GFX library and a standard
320x240 pixel display but you can use the GUIsliceBuilder to set these
to any supported platform. Simply use the PROJECT_OPTIONS tab inside 
the builder and choose your IDE, screen size and graphics package.
</p> 
