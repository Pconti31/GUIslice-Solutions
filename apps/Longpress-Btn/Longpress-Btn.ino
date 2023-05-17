//<App !Start!>
// FILE: [Longpress-Btn.ino]
// Created by GUIslice Builder version: [0.17.b22]
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
#include "Longpress-Btn_GSLC.h"


// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemCounter    = NULL;
//<Save_References !End!>

// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }
int32_t m_nTimePressStart = 0;
int16_t count = 0;



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
  char acTxt[MAX_STR];
  
  if ( eTouch == GSLC_TOUCH_DOWN_IN ) {
    m_nTimePressStart = millis();
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_ELEM_BTN_INCREASE:
        break;
      case E_ELEM_BTN_DECREASE:
        break;
      case E_ELEM_BTN_RESET:
        break;
//<Button Enums !End!>
      default:
        break;
    }
  }
  
  else if (eTouch == GSLC_TOUCH_MOVE_IN) {
    // this code will run as long as a button is held down
    if (millis() - m_nTimePressStart > 3000){

      switch (pElem->nId) {

        case E_ELEM_BTN_RESET:
          count = 0;
          snprintf(acTxt, MAX_STR, "%d", count);
          gslc_ElemSetTxtStr(&m_gui, m_pElemCounter, acTxt); 
          m_nTimePressStart = millis();
          break;
      }
    }
    if (millis() - m_nTimePressStart > 1000) {
      switch (pElem->nId) {

        case E_ELEM_BTN_INCREASE:
          count += 1;
          snprintf(acTxt, MAX_STR, "%d", count);
          gslc_ElemSetTxtStr(&m_gui, m_pElemCounter, acTxt);
          delay (200); 
          break;
        case E_ELEM_BTN_DECREASE:
          count -= 1;
          snprintf(acTxt, MAX_STR, "%d", count);
          gslc_ElemSetTxtStr(&m_gui, m_pElemCounter, acTxt);
          delay (200);
          break;
      }
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

  
 if (count > 100){count = 0;}


 if (count < -99){count = 0;}
 

}
