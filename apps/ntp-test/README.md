# Example of a modifiable Text Field on Base page

<p>
This is an example of using a Base page to create a status line
that is updated every second and shows on each page. The Status will
be the current time and date taken from the nearest NTP time server.
</p>

<p>
Before you start uploading the sketch, you’ll need to make a few 
changes to make sure it’ll work for you. 
</p>

<p>
Modify the following two variables with your network credentials so that 
the ESP32 can connect to an existing network. 
</p>
```
const char* ssid       = "YOUR_SSID";
const char* password   = "YOUR_PASS";
```
<p>
Adjust the UTC offset for your timezone (in seconds). Refer to the list 
of UTC time offsets. Here are some examples for various time zones: 
</p>
```
For UTC -5.00 : -5 * 60 * 60 : -18000 // Eastern Standard Time
For UTC +1.00 : 1 * 60 * 60 : 3600    // One hour difference
For UTC +0.00 : 0 * 60 * 60 : 0       // Greenwich Mean Time

const long  gmtOffset_sec = 3600;
```
<p>
Change the Daylight offset (in seconds). Set it to 3600 if your time 
zone observes Daylight saving time; otherwise, set it to 0. 
</p>
```
const int   daylightOffset_sec = 3600;
```
<p>
After uploading the sketch, you may need to press your reset button on your ESP32. The 
serial monitor should display the date and time every second. The speed is set to 115200 BAUD.
</p>

<p>
The sample is set for a ESP32 or ESP8366 with the TFT_eSPI graphics library using 
a standard 320x240 pixel display but you can use the GUIsliceBuilder to set your display
size and graphics library to any supported platform. Simply use the PROJECT_OPTIONS tab
inside the builder and choose your IDE, screen size and graphics package.
</p> 
