//<App !Start!>
// FILE: [ntp-test.ino]
// Created by GUIslice Builder version: [0.17.b18]
//
// GUIslice Builder Generated File
//
// For the latest guides, updates and support view:
// https://github.com/ImpulseAdventure/GUIslice
//
//<App !End!>

// ------------------------------------------------
// Headers to include
// ------------------------------------------------
#include "ntp-test_GSLC.h"

#include <WiFi.h> // WiFi.h is a library containing the ESP32-specific WiFi methods
#include "time.h" // time.h is the ESP32 native time library that handles 
                  // NTP server synchronization gracefully.

// ------------------------------------------------
// Program Globals
// ------------------------------------------------
const char* ssid       = "YOUR_SSID";
const char* password   = "YOUR_PASS";

const char* ntpServer = "pool.ntp.org";  // find closest time server
// 
const long  gmtOffset_sec = -18000;    // Eastern Standard Time
const int   daylightOffset_sec = 3600; // Yes, we are on Daylight saving time

uint32_t m_nStartTime;
int16_t  m_nAskTimeRate = 1000; // update every 1 second 

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pCurrentTime    = NULL;
//<Save_References !End!>

// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }

/*
 * displayLocalTime() 
 * custom function to print the current date and time.
 *
 * In the table below, you can see how each member of this 
 * time structure relates to a certain piece of information. 
 *
 *  %A  returns day of week
 *  %B  returns month of year
 *  %d  returns day of month
 *  %Y  returns year
 *  %H  returns hour
 *  %M  returns minutes
 *  %S  returns seconds
 *
 */
void displayLocalTime()
{
	char acTxt[MAX_STR];
  struct tm timeinfo;
	
  if (millis() - m_nStartTime >= m_nAskTimeRate) {
		if(!getLocalTime(&timeinfo)){
			Serial.println("Failed to obtain time");
			return;
		}
		strftime(acTxt, MAX_STR, "%A, %B %d %Y %H:%M:%S", &timeinfo);
		gslc_ElemSetTxtStr(&m_gui, m_pCurrentTime, acTxt);
    m_nStartTime = millis();
    Serial.println(acTxt);
	}
 
}

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
// Common Button callback
bool CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*     pGui     = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef);

  if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_BTN_NEXT_PG:
        gslc_SetPageCur(&m_gui, E_PG2);
        break;
      case E_ELEM_BTN2:
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
        break;
//<Button Enums !End!>
      default:
        break;
    }
  }
  return true;
}
//<Checkbox Callback !Start!>
//<Checkbox Callback !End!>
//<Keypad Callback !Start!>
//<Keypad Callback !End!>
//<Spinner Callback !Start!>
//<Spinner Callback !End!>
//<Listbox Callback !Start!>
//<Listbox Callback !End!>
//<Draw Callback !Start!>
//<Draw Callback !End!>
//<Slider Callback !Start!>
//<Slider Callback !End!>
//<Tick Callback !Start!>
//<Tick Callback !End!>

void setup()
{
  // ------------------------------------------------
  // Initialize
  // ------------------------------------------------
  Serial.begin(115200);
  // Wait for USB Serial 
  //delay(1000);  // NOTE: Some devices require a delay after Serial.begin() before serial port can be used

  gslc_InitDebug(&DebugOut);

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUIslice_gen();

  //connect to WiFi
  Serial.print("Connecting to )"); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Initialize our Timer to prime the pump
  m_nStartTime = millis() + m_nAskTimeRate + 1;

  displayLocalTime(); // send our first time request so we can shutoff wifi

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{

  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------
  
  displayLocalTime();
  
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  gslc_Update(&m_gui);
    
}
