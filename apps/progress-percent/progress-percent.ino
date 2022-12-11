//<App !Start!>
// FILE: [progress-percent.ino]
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
#include "progress-percent_GSLC.h"

// ------------------------------------------------
// Program Globals
// ------------------------------------------------
int       m_nCount = 0;

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemProgress1  = NULL;
gslc_tsElemRef* m_pElemProgress2  = NULL;
gslc_tsElemRef* m_pElemProgress3  = NULL;
gslc_tsElemRef* m_pElemProgress4  = NULL;
gslc_tsElemRef* m_pElemToggle1    = NULL;
gslc_tsElemRef* m_pElemToggle2    = NULL;
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
      case E_BTN_NEXTPG:
        m_nCount = 0;
        gslc_SetPageCur(&m_gui, E_PG2);
        break;
      case E_TOGGLE_FLIP1:
        m_nCount = 0;
        if (gslc_ElemXTogglebtnGetState(&m_gui, m_pElemToggle1)) {
          gslc_ElemXProgressSetFlip(&m_gui,m_pElemProgress1,true);
          gslc_ElemXProgressSetFlip(&m_gui,m_pElemProgress2,true);
        } else {
          gslc_ElemXProgressSetFlip(&m_gui,m_pElemProgress1,false);
          gslc_ElemXProgressSetFlip(&m_gui,m_pElemProgress2,false);
        }
        break;
      case E_BTN_BACK:
        m_nCount = 0;
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
        break;
      case E_TOGGLE_FLIP2:
        m_nCount = 0;
        if (gslc_ElemXTogglebtnGetState(&m_gui, m_pElemToggle2)) {
          gslc_ElemXProgressSetFlip(&m_gui,m_pElemProgress3,true);
          gslc_ElemXProgressSetFlip(&m_gui,m_pElemProgress4,true);
        } else {
          gslc_ElemXProgressSetFlip(&m_gui,m_pElemProgress3,false);
          gslc_ElemXProgressSetFlip(&m_gui,m_pElemProgress4,false);
        }
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
  m_nCount=1;
  
  gslc_ElemXProgressSetFont(&m_gui, m_pElemProgress1, E_NOTOMONO8);
  // Set the rounded frame/fill state for this control
  gslc_ElemSetRoundEn(&m_gui, m_pElemProgress1, true);

  gslc_ElemXProgressSetFont(&m_gui, m_pElemProgress2, E_NOTOMONO8);

//  gslc_SetPageCur(&m_gui,E_PG2);
}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{

  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  gslc_Update(&m_gui);
  
  if (m_nCount <= 100 && gslc_GetPageCur(&m_gui) == E_PG_MAIN) {
    gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress1,m_nCount);
    gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress2,m_nCount);
    m_nCount++;
  }

  if (m_nCount <= 100 && gslc_GetPageCur(&m_gui) == E_PG2) {
    gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress3,m_nCount);
    gslc_ElemXProgressSetVal(&m_gui,m_pElemProgress4,m_nCount);
    m_nCount++;
  }

  delay(50);
}
