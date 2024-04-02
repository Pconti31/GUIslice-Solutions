//<App !Start!>
// FILE: [busywait.ino]
// Created by GUIslice Builder version: [0.17.b27]
//
// GUIslice Builder Generated File
//
// For the latest guides, updates and support view:
// https://github.com/ImpulseAdventure/GUIslice
//
//<App !End!>
//
// - busywait   (ESP32):
// Animation that indicates task will run indeterminate length of time.
//   - Animation example
//   - Demonstrates using multiple cores on ESP32
//
// Often you have a need to run sensors or move motors
// in the background. There are times when you can't predict 
// how long such a task will take to execute. This prevents you
// from using something like a progress bar to notify users of
// the required time duration. 
//
// This is when an animation like a spinning wheel can be used
// to at least indicate to your user that they need to wait for
// something to complete before moving forward.
//
// ------------------------------------------------
// Headers to include
// ------------------------------------------------
#include "busywait_GSLC.h"

#include "Calc_PI.h"
#include "XBusyWait.h"

// ------------------------------------------------
// Defines
// ------------------------------------------------
// uncomment below whichever version of 
// Busywait animation you want on screen.
//#define STYLE_SPINNING_DOTS
//#define STYLE__SPINNING_WHEEL
#define STYLE_THREE_DOTS

#define POSITION_ANIMATION
//#define CLEAR_SCREEN

// ------------------------------------------------
// Program Globals
// ------------------------------------------------
TaskHandle_t PI_Task;

#if defined(POSITION_ANIMATION)
  #if defined(CLEAR_SCREEN)
  bool m_bClearScreen = true;
  #else
  bool m_bClearScreen = false;
  #endif
#endif

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemBusywait   = NULL;
gslc_tsElemRef* m_pElemTxtPI      = NULL;
//<Save_References !End!>

// --------------------------------------------------------
// Define our Busywait yield function
//
// Our background task is running in core 0
// so we need to check that task to determine
// if it's finished or not.
//
// Our busywait extended element has registered
// a tick callback that occurs every pass
// through loop()->gslc_Update(&m_gui);
// which in turns calls this cbYield() function.
//
// Returns true if the animation is to stop, false otherwise
// ---------------------------------------------------------
bool cbYield() {
  char *pi;

  if (Calc_PI::isFinish()) {
    pi = Calc_PI::getPI();
    Serial.println(pi);
    Serial.print("pi digits="); Serial.println(strlen(pi));
    // NOTE: gslc_ElemSetTxtStr will only copyto max size of UI field
    gslc_ElemSetTxtStr(&m_gui,m_pElemTxtPI,pi);
    free(pi);
    return true;
  }
  return false;
}

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

  int nMax = 115;  // max iterations

  if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_ELEM_BTN1:
        gslc_ElemSetTxtStr(&m_gui,m_pElemTxtPI," ");
        gslc_ElemXBusyWaitStartAnimation(&m_gui, m_pElemBusywait);
        gslc_PopupShow(&m_gui, E_PG_POPUP_BUSY, true);
        // ------------------------------------------------
        // Create a task that will be executed in core 0 
        // Calc_PI::pi_task() function 
        // ------------------------------------------------
        xTaskCreatePinnedToCore(
                        Calc_PI::pi_task,  /* Task function. */
                        "CalcPI",          /* name of task. */
                        10000,             /* Stack size of task */
                        (void*)nMax,       /* parameter of the task */
                        1,                 /* priority of the task */
                        &PI_Task,          /* Task handle to keep track of created task */
                        0);                /* pin task to core 0 */                  
        delay(100);
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
  delay(2000);  // NOTE: Some devices require a delay after Serial.begin() before serial port can be used

  gslc_InitDebug(&DebugOut);

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUIslice_gen();

  gslc_ElemSetTxtStr(&m_gui,m_pElemTxtPI," ");

  // ------------------------------------------------
  // Create E_ELEM_BUSYWAIT 
  // ------------------------------------------------

#if defined(STYLE_SPINNING_DOTS)
  redefineBoxAsBusyWait(&m_gui, m_pElemBusywait, GSLCX_BUSYWAIT_STYLE_SPINNING_DOTS, GSLC_COL_BLUE, &cbYield);
#elif defined(STYLE__SPINNING_WHEEL)
  redefineBoxAsBusyWait(&m_gui, m_pElemBusywait, GSLCX_BUSYWAIT_STYLE__SPINNING_WHEEL, GSLC_COL_BLUE, &cbYield);
#elif defined(STYLE_THREE_DOTS)
  redefineBoxAsBusyWait(&m_gui, m_pElemBusywait, GSLCX_BUSYWAIT_STYLE_THREE_DOTS, GSLC_COL_BLUE, &cbYield);
#endif

#if defined(POSITION_ANIMATION)
  // center animation on bottom of screen
  int16_t nX = m_gui.nDispW/2;
  #if defined(STYLE_THREE_DOTS)
    int16_t nY = m_gui.nDispH-20;
  #else
    int16_t nY = m_gui.nDispH-50; 
  #endif
  gslc_ElemXBusyWaitPositionAnimation(&m_gui, m_pElemBusywait, nX, nY, m_bClearScreen);
#endif

  BigNumber::begin (70);    // Length of the BigNumber
  
  Serial.print("GUIslice UI running on core ");
  Serial.println(xPortGetCoreID());

}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{

  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------
  
  
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  gslc_Update(&m_gui);
    
}
