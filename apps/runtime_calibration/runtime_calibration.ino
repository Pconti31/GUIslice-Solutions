//<App !Start!>
// FILE: [runtime_calibration.ino]
// Created by GUIslice Builder version: [0.17.b27]
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
#include "runtime_calibration_GSLC.h"

#include "TFT_Calibration.h"

// - Runtime TFT Display Calibration Example:
//   - Example of Application that can make use of touch screen
//     calibrations for XMIN, XMAX, YMIN, YMAX values stored
//     either on SD Card or in EEPROM thus overriding values stored in
//     compiled code taken from GUIslice/config/*.h files.
//   - Expected behavior: Clicking on CALIBRATE button outputs a message
//     along with instructions on how to calibrate your screen.
//
//     After completing all steps the values are saved to SD Card or EEPROM.
//     Optionally if your TFT_Calibration.h has DEBUG_CALIB = 1 output 
//     to serial monitor.
//
//   NOTE:
//     Don't forget to read the NOTES section inside TFT_Calibration.h
//     for instructions and warnings on TFT_Calibration class usage.
//

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

TFT_Calibration calibration = TFT_Calibration();
bool        m_bQuit = false;
bool        m_bDoSomething = false;

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemDo         = NULL;
gslc_tsElemRef* m_pElemQuit       = NULL;
gslc_tsElemRef* m_pElemRadioBnt1  = NULL;
gslc_tsElemRef* m_pElemRadioBnt2  = NULL;
gslc_tsElemRef* m_pTxtStatus      = NULL;
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
      case E_ELEM_BTN_QUIT:
        m_bQuit = true;
        gslc_ElemSetTxtStr(&m_gui,m_pElemQuit,"DONE");
        gslc_ElemSetCol(&m_gui,m_pElemQuit,GSLC_COL_RED,GSLC_COL_BLACK,GSLC_COL_BLACK);
        Serial.println("QUIT");
        break;
      case E_ELEM_BTN_SOMETHING:
        m_bDoSomething = !m_bDoSomething;
        if (m_bDoSomething) {
          Serial.println("SOME THING DONE");
          gslc_ElemSetTxtStr(&m_gui,m_pElemDo,"SOMETHING");
          gslc_ElemSetCol(&m_gui,m_pElemDo,GSLC_COL_BLACK,GSLC_COL_BLACK,GSLC_COL_BLACK);
        } else {
          Serial.println("WAITING TO DO SOME THING");
          gslc_ElemSetTxtStr(&m_gui,m_pElemDo,"DO SOMETHING");
          gslc_ElemSetCol(&m_gui,m_pElemDo,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_BLUE_DK2);
        }
        break;
      case E_ELEM_BTN_CALIBRATE:
        calibration.setStatus(CALIBR_UNDEFINED);
        gslc_ElemSetTxtStr(&m_gui,m_pTxtStatus," ");
        gslc_SetPageCur(&m_gui, E_PG_CALIBRATION);
        break;
      case E_ELEM_BTN_START:
        calibration.run();
        break;
      case E_ELEM_BTN_DONE:
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
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

    case E_ELEM_RADIO2:
      break;
//<Checkbox Enums !End!>
    default:
      break;
  } // switch
  return true;
}
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
  
  // ------------------------------------------------
  // Initialize our calibration object
  // ------------------------------------------------
#if defined(USE_SD_INTERFACE)
  calibration.init(&m_gui, E_PG_POPUP1. "mydata.bin");
#else
  // store calibration data at beginning of our EEPROM. 
  calibration.init(&m_gui, E_PG_POPUP1, 0);
#endif  

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
  // DO NOT call GUIslice update function!
  // We must use calibration.update() instead.
  // ------------------------------------------------
  //gslc_Update(&m_gui);
  calibration.update();
  
  if (calibration.getStatus() != CALIBR_UNDEFINED) {
    if (calibration.getStatus() == CALIBR_SUCCESS) {
      gslc_ElemSetTxtStr(&m_gui,m_pTxtStatus,"Calibration Success");
    } else {
      gslc_ElemSetTxtStr(&m_gui,m_pTxtStatus,"Calibration Failure");
    }
  }
  
  // In a real program, we would detect the button press and take an action.
  // For this Arduino demo, we will pretend to exit by emulating it with an
  // infinite loop. Note that interrupts are not disabled so that any debug
  // messages via Serial have an opportunity to be transmitted.
  if (m_bQuit) {
    gslc_Quit(&m_gui);
    while (1) { }
  }
    
}
