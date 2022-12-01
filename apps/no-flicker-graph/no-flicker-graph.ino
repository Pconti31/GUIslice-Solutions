//<App !Start!>
// FILE: [no-flicker-graph.ino]
// Created by GUIslice Builder version: [0.17.b18]
//
// GUIslice Builder Generated File
//
// For the latest guides, updates and support view:
// https://github.com/ImpulseAdventure/GUIslice
//
//<App !End!>

//
// ARDUINO NOTES:
// - GUIslice_config.h must be edited to match the pinout connections
//   between the Arduino CPU and the display controller (see ADAGFX_PIN_*).
//

// ------------------------------------------------
// Headers to include
// ------------------------------------------------
#include "no-flicker-graph_GSLC.h"



// ------------------------------------------------
// Program Globals
// ------------------------------------------------
bool      m_bPause = false;

// Free-running counter for display
unsigned  m_nCount = 0;

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemBtnState   = NULL;
gslc_tsElemRef* m_pElemGraph1     = NULL;
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
      case E_ELEM_BTN_STATE:
        if (m_bPause) {
          m_bPause = false;
          gslc_ElemSetTxtStr(&m_gui,m_pElemBtnState,"PAUSE");
          gslc_ElemSetCol(&m_gui,m_pElemBtnState,GSLC_COL_BLUE_DK2,GSLC_COL_RED,GSLC_COL_GRAY);
        } else {
          m_bPause = true;
          gslc_ElemSetTxtStr(&m_gui,m_pElemBtnState,"GO");
          gslc_ElemSetCol(&m_gui,m_pElemBtnState,GSLC_COL_BLUE_DK2,GSLC_COL_GREEN_DK3,GSLC_COL_GRAY);
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
  // Load Fonts
  // ------------------------------------------------

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUIslice_gen();


  gslc_GuiRotate(&m_gui,3);
}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{
  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------

  // compute a new sin value
  if (!m_bPause) {
    graph_sin();
  }
  
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  gslc_Update(&m_gui);

  delay(100);
}

void graph_sin() 
{
  static double x=0.0;
  double y = sin(x);
  /* scale (map) data for graph
   * The defined rectangle's width=302 so accounting for the 1 pixel frame
	 * that leaves us 300 for the number of points available on the y axis
	 * While the defined rectangle's height=202 giving x axis of 200 points (202 - frame size)
	 * Therefore the map function is setup to map y +- over 150 (the 300 y axis)
	 * and the x is set to map between 30 and 170 so we have some space left 
	 * over on the top and bottom of our graph.
	 *
   * Since map only handles integers we multiply y's floating point numbers
	 * by 100 to get 2 diget decimal number as an integer. 
	 * So for example a y of 0.25 becomes 25.
   */
  int16_t nY =  map((int16_t)(y*100), -150, 150, 30, 170);
  // add to graph
  gslc_ElemXGraphAdd(&m_gui,m_pElemGraph1,nY);
  // increment x to next time period
  if (x <= 25.2) {
    x += 0.025;
  } else {
    x = 0.0;
 }
}
