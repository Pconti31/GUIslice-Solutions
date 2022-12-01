//<App !Start!>
// FILE: [ex_boot_screen.ino]
// Created by GUIslice Builder version: [0.17.b17]
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
#include "ex_boot_screen_GSLC.h"


// ------------------------------------------------
// Program Globals
// ------------------------------------------------
//const uint32_t maxtime_to_show = 10000; // max time to display splash screen in milliseconds
uint32_t dt; // timer for our splash screen
bool     m_bShowSplash = true; // flag that indicates we need to show our splash screen
bool     m_bQuit = false;

// simulate hardware loading using a counter
// Free-running counter for display
unsigned    m_nCount = 0;
const unsigned maxcount = 100;

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemProgress1  = NULL;
//<Save_References !End!>

// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }

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
      case E_BTN_QUIT:
        // Output a message when the button is pressed
        Serial.println("Quit button pressed");
        // Set a variable flag that we can use elsewhere
        m_bQuit = true;
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
  Serial.begin(9600);
  // Wait for USB Serial 
  //delay(1000);  // NOTE: Some devices require a delay after Serial.begin() before serial port can be used

  gslc_InitDebug(&DebugOut);

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUIslice_gen();

  // Initialize our Splash Screen Timer
	dt = millis();
	
	// show our popup splash screen
	gslc_PopupShow(&m_gui, E_PG_POPUP1, true);
}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{

  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------
	// turn off splash screen?
//  if ((millis()-dt) > maxtime_to_show && m_bShowSplash) {
  if (m_nCount>maxcount && m_bShowSplash) {
		m_bShowSplash = false;
		gslc_PopupHide(&m_gui);
	} 
  if (m_bShowSplash) {
	  gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress1,(m_nCount%100));
	}
  m_nCount++; // hardware simulator

  // Slow down updates
  delay(100);
	
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  gslc_Update(&m_gui);

  // In a real program, we would detect the button press and take an action.
  // For this Arduino demo, we will pretend to exit by emulating it with an
  // infinite loop. Note that interrupts are not disabled so that any debug
  // messages via Serial have an opportunity to be transmitted.
  if (m_bQuit) {
    gslc_Quit(&m_gui);
    while (1) { }
  }
       
}
