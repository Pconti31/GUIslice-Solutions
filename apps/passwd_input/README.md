
# Keyboard Application that can hide a password

<p>
Demonstrates how to make a Input Text field hide or show a password
using GUIslice's AlphaNumeric keyboard. This text field option requires the
replacement of various routines inside GUIslice. 
</p>

<p>
Before running this sample you must move the modified GUIslice sources contained
inside the sub-folder guislice-src. GUIslice.h, GUIslice.c, and 
GUIslice_drv_tft_espi.cpp need to replace files in Arduino/libraries/GUIslice/src
while the files inside guislice-src/elem need to replace files inside Arduino/libraries/GUIslice/src/elem.
</p>

<p>
Since only GUIslice's TFT_eSPI driver GUIslice_drv_tft_espi.cpp has been modified no other library will support passwords.
Modifications to other libraries like GUIslice_drv_adagfx.cpp etc... is left as a student exercise.
</p>

<p>
The GUIslice standard keyboard may appear too small for your application but you can change button size and font automatically by using the Builder
edit->options->KeyPadAlpha or KeyPadNum tab change Font and "Use Default Button Sizes?" from true to false 
and then the Button Height and Width even the gap between the buttons.
</p>

<p>
The sample is set for Arduino using TFT_eSPI library and a standard
320x240 pixel display but you can use the GUIsliceBuilder to set the
display to any size. Simply use the PROJECT_OPTIONS tab inside 
the builder and choose your IDE and screen size.
</p> 
