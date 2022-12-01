//<App !Start!>
// FILE: [ContinuePress-Btn.ino]
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
#include "ContinuePress-Btn_GSLC.h"


// ------------------------------------------------
// Program Globals
// ------------------------------------------------
uint32_t m_nCount = 0;
uint32_t m_nTimePressStart = 0; 
bool m_bRunMotor = false;

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemProgress1  = NULL;
gslc_tsElemRef* m_pElemStatus     = NULL;
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
      case E_ELEM_BTN_RUN:
				gslc_ElemSetTxtStr(pGui,m_pElemStatus,"STOPPED");
  			m_bRunMotor = false;
        break;
//<Button Enums !End!>
      default:
        break;
    }
  } else if ((eTouch == GSLC_TOUCH_DOWN_IN)) {
		if (pElem->nId == E_ELEM_BTN_RUN) {
		  gslc_ElemSetTxtStr(pGui,m_pElemStatus,"RUNNING");
			m_nCount = 0;
      gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress1,m_nCount);
			m_nTimePressStart = millis();
			m_bRunMotor = true;
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

bool CbTickScanner(void* pvGui,void* pvScope)
{
  gslc_tsGui*     pGui      = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef  = (gslc_tsElemRef*)(pvScope);
  gslc_tsElem*    pElem     = gslc_GetElemFromRef(pGui,pElemRef);

	if (m_bRunMotor) {
  // NOTE: The timer is simply to create a increment for the
	//       progress bar.
		if (millis() - m_nTimePressStart > 100 /* increment every tenth of a second */) {
      m_nTimePressStart = millis();
			m_nCount++;
      if (m_nCount > 100)
        m_nCount = 1;
			gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress1,m_nCount);
		}
	}

  return true;
}

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

}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{

  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------
  
  //TODO - Add update code for any text, gauges, or sliders
  
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  gslc_Update(&m_gui);
    
}
