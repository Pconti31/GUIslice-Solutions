//<App !Start!>
// FILE: [led-panel.ino]
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
#include "led-panel_GSLC.h"


// ------------------------------------------------
// Program Globals
// ------------------------------------------------

bool m_bRedLED    = false;
bool m_bGreenLED  = false;
bool m_bBlueLED   = false;
bool m_bYellowLED = false;

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pLED            = NULL;
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
      case E_RED_BTN:
        m_bRedLED = m_bRedLED ? false : true; // toggle led
        break;
      case E_BTN_GREEN:
        m_bGreenLED = m_bGreenLED ? false : true; // toggle led
        break;
      case E_BTN_BLUE:
        m_bBlueLED = m_bBlueLED ? false : true; // toggle led
        break;
      case E_BTN_YELLOW:
        m_bYellowLED = m_bYellowLED ? false : true; // toggle led
        break;
//<Button Enums !End!>
      default:
        break;
    }
    // Manually mark the elements affected as needing redraw
    gslc_ElemSetRedraw(&m_gui,m_pLED,GSLC_REDRAW_FULL);
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
 
// Scanner drawing callback function
// - This is called when E_ELEM_SCAN is being rendered
bool CbDrawScanner(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw)
{
  int nInd;

  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*     pGui     = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef); 

  // Create shorthand variables for the origin
  int16_t  nX = pElem->rElem.x;
  int16_t  nY = pElem->rElem.y;

  // Draw the background
  gslc_tsRect rInside = pElem->rElem;
  rInside = gslc_ExpandRect(rInside,-1,-1);
  gslc_DrawFillRect(pGui,rInside,pElem->colElemFill);

  // Enable localized clipping
  gslc_SetClipRect(pGui,&rInside);

  //TODO - Add your drawing graphic primitives
  int x = 20;
  int y = 225;
  if (m_bRedLED) {
    gslc_DrawFillCircle(pGui, x,y,5,GSLC_COL_RED);
  } else {
    gslc_DrawFillCircle(pGui, x,y,5,GSLC_COL_WHITE);
  }
  x += 20;
  if (m_bGreenLED) {
    gslc_DrawFillCircle(pGui, x,y,5,GSLC_COL_GREEN);
  } else {
    gslc_DrawFillCircle(pGui, x,y,5,GSLC_COL_WHITE);
  }
  x += 20;
  if (m_bBlueLED) {
    gslc_DrawFillCircle(pGui, x,y,5,GSLC_COL_BLUE);
  } else {
    gslc_DrawFillCircle(pGui, x,y,5,GSLC_COL_WHITE);
  }
  x += 20;
  if (m_bYellowLED) {
    gslc_DrawFillCircle(pGui, x,y,5,GSLC_COL_YELLOW);
  } else {
    gslc_DrawFillCircle(pGui, x,y,5,GSLC_COL_WHITE);
  }
  // Disable clipping region
  gslc_SetClipRect(pGui,NULL);

  // Draw the frame
  gslc_DrawFrameRect(pGui,pElem->rElem,pElem->colElemFrame);

  // Clear the redraw flag
  gslc_ElemSetRedraw(&m_gui,pElemRef,GSLC_REDRAW_NONE);

  return true;
}
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
    
}

