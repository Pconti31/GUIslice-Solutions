//<App !Start!>
// FILE: [simple_tabs.ino]
// Created by GUIslice Builder version: [0.17.b17]
//
// GUIslice Builder Generated File
//
// For the latest guides, updates and support view:
// https://github.com/ImpulseAdventure/GUIslice
//
//<App !End!>

//
// - Modified Example 24 using XTabBtn control (Arduino):
//   - Multiple page handling, tab dialog, base page elements
//     

// ------------------------------------------------
// Headers to include
// ------------------------------------------------
#include "simple_tabs_GSLC.h"

#include "XTabBtn.h"


// ------------------------------------------------
// Program Globals
// ------------------------------------------------
bool      m_bQuit = false;

// Free-running counter for display
unsigned  m_nCount = 0;

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemCnt        = NULL;
gslc_tsElemRef* m_pElemProgress1  = NULL;
gslc_tsElemRef* m_pElemQuit       = NULL;
gslc_tsElemRef* m_pElemTabConfig  = NULL;
gslc_tsElemRef* m_pElemTabMain    = NULL;
//<Save_References !End!>

// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
// Common Button callback
bool CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem* pElem = gslc_GetElemFromRef(&m_gui,pElemRef);

  if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_ELEM_BTN_QUIT:
        m_bQuit = true;
        gslc_ElemSetTxtStr(&m_gui,m_pElemQuit,"DONE");
        gslc_ElemSetCol(&m_gui,m_pElemQuit,GSLC_COL_RED,GSLC_COL_BLACK,GSLC_COL_BLACK);
        break;
      case E_ELEM_TAB_MAIN:
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
        break;
      case E_ELEM_TAB_CONFIG:
        gslc_SetPageCur(&m_gui, E_PG_CONFIG);
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
  // Load Fonts
  // ------------------------------------------------

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUIslice_gen();

  // Install our tabs
	redefineButtonAsTab(&m_gui, m_pElemTabMain, E_PG_BASE, E_PG_MAIN, true /* On */);
	redefineButtonAsTab(&m_gui, m_pElemTabConfig, E_PG_BASE, E_PG_CONFIG, false /* Off */);

}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{
  char              acTxt[MAX_STR];

  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------
  m_nCount++;

  // Perform drawing updates
  // - Note: we can make the updates conditional on the active
  //   page by checking gslc_GetPageCur() first.

  if ((m_nCount % 50) == 0) {
    snprintf(acTxt, MAX_STR, "%u", m_nCount);
    gslc_ElemSetTxtStr(&m_gui, m_pElemCnt, acTxt);
  }

  gslc_ElemXProgressSetVal(&m_gui, m_pElemProgress1, ((m_nCount / 2) % 100));

  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  gslc_Update(&m_gui);
    
  // Slow down updates
  delay(10);

  // In a real program, we would detect the button press and take an action.
  // For this Arduino demo, we will pretend to exit by emulating it with an
  // infinite loop. Note that interrupts are not disabled so that any debug
  // messages via Serial have an opportunity to be transmitted.
  if (m_bQuit) {
    gslc_Quit(&m_gui);
    while (1) {}
  }
}
