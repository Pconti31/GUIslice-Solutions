//<App !Start!>
// FILE: [passwd_input.ino]
// Created by GUIslice Builder version: [0.17.b38]
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
#include "passwd_input_GSLC.h"



// ------------------------------------------------
// Program Globals
// ------------------------------------------------
gslc_tsXKeyPadCfg* pKeyboardConfig = NULL;  // used to turn on/off password mode
char m_acPasswd[33]; // must be same size as holds password mostly for debugging

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemClearTxt   = NULL;
gslc_tsElemRef* m_pElemPasswd     = NULL;
gslc_tsElemRef* m_pElemToggle1    = NULL;
gslc_tsElemRef* m_pElemKeyPadAlpha= NULL;
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
      case E_TXT_PASSWD:
        // Clicked on edit field, so show popup box and associate with this text field
        gslc_ElemXKeyPadInputAsk(&m_gui, m_pElemKeyPadAlpha, E_POP_KEYPAD_ALPHA, m_pElemPasswd);
        break;
      case E_ELEM_TOGGLE1:
        // code for Toggle button ON/OFF state
        if (gslc_ElemXToggleImgbtnGetState(&m_gui, m_pElemToggle1)) {
          gslc_ElemSetPasswdEn(&m_gui, m_pElemPasswd, true);
        } else {
          gslc_ElemSetPasswdEn(&m_gui, m_pElemPasswd, false);
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
// KeyPad Input Ready callback
bool CbKeypad(void* pvGui, void *pvElemRef, int16_t nState, void* pvData)
{
  gslc_tsGui*     pGui     = (gslc_tsGui*)pvGui;
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef);

  // From the pvData we can get the ID element that is ready.
  int16_t nTargetElemId = gslc_ElemXKeyPadDataTargetIdGet(pGui, pvData);
  if (nState == XKEYPAD_CB_STATE_DONE) {
    // User clicked on Enter to leave popup
    // - If we have a popup active, pass the return value directly to
    //   the corresponding value field
    switch (nTargetElemId) {
//<Keypad Enums !Start!>

      case E_TXT_PASSWD:
        gslc_ElemXKeyPadInputGet(pGui, m_pElemPasswd, pvData);
        strcpy(m_acPasswd, gslc_ElemGetTxtStr(&m_gui, m_pElemPasswd));
        gslc_ElemSetTxtStr(&m_gui, m_pElemClearTxt, m_acPasswd); // set cleartext version of password
        Serial.print("Password entered: ");
        Serial.println(m_acPasswd);
        break;
//<Keypad Enums !End!>
      default:
        break;
    }
  } else if (nState == XKEYPAD_CB_STATE_CANCEL) {
    // User escaped from popup, so don't update values
    gslc_PopupHide(&m_gui);
  }
  return true;
}
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

  // Turn on password mode
  gslc_ElemSetPasswdEn(&m_gui, m_pElemPasswd, true);
  gslc_ElemXToggleImgbtnSetState(&m_gui, m_pElemToggle1, true);
  
  m_acPasswd[0] = '\0'; // this global is just for debugging

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
