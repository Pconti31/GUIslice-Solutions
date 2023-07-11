//<App !Start!>
// FILE: [screen-saver-logo.ino]
// Created by GUIslice Builder version: [0.17.b24]
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
#include "screen-saver-logo_GSLC.h"

#include "ScreenManager.h"


ScreenManager   screenMgr = ScreenManager();

#include "BouncingLogoAnim.h"
BouncingLogoAnim myAnimation = BouncingLogoAnim();
extern "C" const unsigned short dvd_logo[] PROGMEM;
 
// ------------------------------------------------
// Program Globals
// ------------------------------------------------
bool           m_bScreenSaverOn = false;
uint32_t       m_nStartTime;
const uint32_t m_nInactivityMax = 10000; // ten seconds
int            m_nInactivityCount = 0;
char           m_acTxt[MAX_STR];

boolean        m_bBusyStatus = false; // this is just to show on/off text string

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemCBCheckBox = NULL;
gslc_tsElemRef* m_pTimeout        = NULL;
gslc_tsElemRef* m_pTxtCountDown   = NULL;
gslc_tsElemRef* m_pTxtStatus      = NULL;
gslc_tsElemRef* m_pElemKeyPadNum  = NULL;
//<Save_References !End!>

// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }

// ------------------------------------------------
// Screen Saver Callbacks
// ------------------------------------------------
void my_pause() {
  myAnimation.pause();
}

void my_resume() {
  myAnimation.resume();
}

void my_display() {
  // myAnimation doesn't really loop
  // but just paints one frame then returns
  if (screenMgr.isActive())
    myAnimation.loop();
}

void my_counter() {
  int count = screenMgr.getCountDown();
  snprintf(m_acTxt,MAX_STR,"%02d",count);
  gslc_ElemSetTxtStr(&m_gui,m_pTxtCountDown,m_acTxt);
//  Serial.print("*** Timer! "); Serial.println(m_acTxt);
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
      case E_ELEM_BTN_BUSY:
        gslc_SetPageCur(&m_gui, E_PG2);
        break;
      case E_NUMINPUT_TIMEOUT:
        // Clicked on edit field, so show popup box and associate with this text field
        gslc_ElemXKeyPadInputAsk(&m_gui, m_pElemKeyPadNum, E_POP_KEYPAD_NUM, m_pTimeout);
        break;
      case E_BTN_1:
        Serial.println("BTN 1");
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
        break;
      case E_BTN_2:
        Serial.println("BTN 2");
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
        break;
      case E_BTN_3:
        Serial.println("BTN 3");
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
        break;
      case E_BTN_4:
        Serial.println("BTN 4");
        break;
      case E_BTN_5:
        Serial.println("BTN 5");
        break;
      case E_BTN_6:
        Serial.println("BTN 6");
        break;
      case E_BTN_7:
        Serial.println("BTN 7");
        break;
      case E_BTN_8:
        Serial.println("BTN 8");
        break;
      case E_BTN_9:
        Serial.println("BTN 9");
        break;
//<Button Enums !End!>
      default:
        break;
    }
  }
  return true;
}
// Checkbox / radio callbacks
// - Creating a callback function is optional, but doing so enables you to
//   detect changes in the state of the elements.
bool CbCheckbox(void* pvGui, void* pvElemRef, int16_t nSelId, bool bState)
{
  gslc_tsGui*     pGui      = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef  = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem     = gslc_GetElemFromRef(pGui,pElemRef);
  if (pElemRef == NULL) {
    return false;
  }
  
  boolean bChecked = gslc_ElemXCheckboxGetState(pGui,pElemRef);

  // Determine which element issued the callback
  switch (pElem->nId) {
//<Checkbox Enums !Start!>
    case E_ELEM_CHECK1:
      break;

//<Checkbox Enums !End!>
    default:
      break;
  } // switch
  return true;
}
// KeyPad Input Ready callback
bool CbKeypad(void* pvGui, void *pvElemRef, int16_t nState, void* pvData)
{
  gslc_tsGui*     pGui     = (gslc_tsGui*)pvGui;
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef);
  int32_t         nTimeout;
  
  // From the pvData we can get the ID element that is ready.
  int16_t nTargetElemId = gslc_ElemXKeyPadDataTargetIdGet(pGui, pvData);
  if (nState == XKEYPAD_CB_STATE_DONE) {
    // User clicked on Enter to leave popup
    // - If we have a popup active, pass the return value directly to
    //   the corresponding value field
    switch (nTargetElemId) {
//<Keypad Enums !Start!>
      case E_NUMINPUT_TIMEOUT:
        gslc_ElemXKeyPadInputGet(pGui, m_pTimeout, pvData);
        // convert string to int fot timeout
        nTimeout = atol(gslc_ElemGetTxtStr(&m_gui, m_pTimeout));
        // reset the default timeout value for the screen manager
        screenMgr.setTimeout(nTimeout);
        gslc_PopupHide(&m_gui);
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
  Serial.begin(115200);
  // Wait for USB Serial 
  //delay(1000);  // NOTE: Some devices require a delay after Serial.begin() before serial port can be used

  gslc_InitDebug(&DebugOut);

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUIslice_gen();
  
  // must be Adafruit_GFX
  myAnimation.init(&m_gui, dvd_logo);
  
// use this init if you need to change defaults
// myAnimation.init(&m_gui, dvd_logo, 0x0000/*BLACK*/, 5 /*x delta*/, 5 /*y delta*/, 5 /*time delay ms*/);

  myAnimation.pause();

  screenMgr.init(&m_gui, 10 /*timeout seconds*/, &my_pause, &my_resume, &my_display, &my_counter, GSLC_COL_BLACK);
  
}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{
  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------
  
  // Put your application code here
  
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  
  // Our ScreenMgr calls gslc_Update when required
  // DO NOT call it directly or your screen will get messed up!
  //gslc_Update(&m_gui);
  screenMgr.update();
    
}
