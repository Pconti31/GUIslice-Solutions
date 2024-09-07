# Example of how to bind a physical key to a specific button element

<p>
This is an example of using a physical button and a on screen button. 
When the physical key is pressed, the on screen button also has a selection effect.
The on screen button can be used independently from the physical button.
</p>

<p>
The EasyButton Arduino library is used to simplify button handling.
</p>

<p>
Before you start uploading the sketch, you’ll need to make a few changes to make sure it’ll work for you. 
Modify the following three defines at the top of btn-physical.ino, 
The example has one physical button on pin 18 and three on screen buttons. 
BUTTON_PIN must be set to pin number of your physical button.
You should uncomment which ever on screen button you want to test.
The defines are ON_SCREEN_BUTTON the enum of the on screen button and ON_SCREEN_PAGE_ID
which is the page enum where the button has been placed.
</p>

<p>
Uncommenting the various ON_SCREEN_BUTTON and ON_SCREEN_PAGE_ID will allow you to test mapping a physical button
to a base page button, main page button or a extra page 2 button.
</p>

<p>
The sample is set for Arduino Mega using a standard 320x240 pixel display with Adafruit GFX graphics
but you can use the GUIsliceBuilder to set your display size and graphics library to any 
supported platform. Simply use the PROJECT_OPTIONS tab
inside the builder and choose your IDE, screen size and graphics package.
</p> 
