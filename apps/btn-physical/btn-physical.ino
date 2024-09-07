//<App !Start!>
// FILE: [btn-physical.ino]
// Created by GUIslice Builder version: [0.17.b34]
//
// GUIslice Builder Generated File
//
// For the latest guides, updates and support view:
// https://github.com/ImpulseAdventure/GUIslice
//
//<App !End!>

//
// - Example 02 (Arduino):
//   - Accept touch input, text button
//   - Expected behavior: Clicking on button outputs a message
//     to the serial monitor
//   - NOTE: This is the simple version of the example without
//     optimizing for memory consumption. 
//
// ARDUINO NOTES:
// - GUIslice_config.h must be edited to match the pinout connections
//   between the Arduino CPU and the display controller (see ADAGFX_PIN_*).
//

// ------------------------------------------------
// Headers to include
// ------------------------------------------------
#include "btn-physical_GSLC.h"

#include "EasyButton.h"

// ------------------------------------------------
// EasyButton variables and defines
// - NOTE: The EasyButton constructor defaults to pull-up enabled with inverted
//         polarity, meaning that the switches will connect the pin to ground
//         when closed, and pull-up (via ATmega internal pullup resistors) when
//         open. If your device does not have internal pullup resistors, then
//         you must provide them externally. If you don't understand this google it!
//         EasyButton(uint8_t pin, uint32_t dbTime = 35, bool puEnable = true, bool invert = true);
// ------------------------------------------------
#define BUTTON_PIN 18  // pin number assigned to physical button
EasyButton btn_physical(BUTTON_PIN, 35, true, true);
// create mapping of physical button to on screen button and the page it sits on
#define ON_SCREEN_BUTTON    E_ELEM_BTN_QUIT 
#define ON_SCREEN_PAGE_ID   E_PG_BASE
//#define ON_SCREEN_BUTTON    E_ELEM_BTN_JUMP 
//#define ON_SCREEN_PAGE_ID   E_PAGE_MAIN
//#define ON_SCREEN_BUTTON    E_ELEM_BTN_BACK 
//#define ON_SCREEN_PAGE_ID   E_PG2

// ------------------------------------------------
// Program Globals
// ------------------------------------------------


bool    m_bQuit = false;

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemQuit       = NULL;
//<Save_References !End!>

// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }

/// - Create a clickable event
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  nElemId:     Element ID to assign (0..16383 or GSLC_ID_AUTO to autogen)
/// \param[in]  nPageId:     Page ID to attach element to
/// \param[in]  cbTouch:     Global Callback for button touch events
void triggerEvent(gslc_tsGui* pGui,int16_t nElemId,int16_t nPageId,GSLC_CB_TOUCH cbTouch)
{
   // first determine if the page that our button is placed on is active or not
   // ignore button if page isn't active
  for (unsigned nStack = 0; nStack < GSLC_STACK__MAX; nStack++) {
    gslc_tsPage* pStackPage = pGui->apPageStack[nStack];
    if (pStackPage->nPageId == nPageId) {
      // Ensure the page layer is active (receiving touch events)
      if (pGui->abPageStackActive[nStack]) {
        // found page is active, we ignore focus 
        // now find ElementRef for our button
        gslc_tsElemRef* pElemRef = gslc_PageFindElemById(pGui, nPageId, nElemId);
        if (pElemRef != NULL) {
          // Ok, now jump to global button callback
          (cbTouch)((void*)(pGui), (void*)(pElemRef), GSLC_TOUCH_UP_IN, 0, 0);
          // Incremental redraw
          gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_INC);
        }
        break;
      }
    }
  }
}

// EasyButton Callback.
void onPressed() {
  triggerEvent(&m_gui, ON_SCREEN_BUTTON, ON_SCREEN_PAGE_ID, &CbBtnCommon);
}

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
// Common Button callback
bool CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem* pElem = gslc_GetElemFromRef(&m_gui,pElemRef);

  // Determine what type of event occurred on the button
  // - In this case we're just looking for the user releasing
  //   a touch over the button.  if ( eTouch == GSLC_TOUCH_UP_IN ) {
  if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_ELEM_BTN_QUIT:
        // Set a variable flag that we can use elsewhere        
        m_bQuit = !m_bQuit;
        if (m_bQuit) {
          gslc_ElemSetTxtStr(&m_gui,m_pElemQuit,"DONE");
          gslc_ElemSetCol(&m_gui,m_pElemQuit,GSLC_COL_RED,GSLC_COL_BLACK,GSLC_COL_RED);
        } else {
          gslc_ElemSetTxtStr(&m_gui,m_pElemQuit,"Quit");
          gslc_ElemSetCol(&m_gui,m_pElemQuit,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE_DK4,GSLC_COL_BLUE_DK1);
        }
        break;
      case E_ELEM_BTN_JUMP:
        gslc_SetPageCur(&m_gui, E_PG2);
        break;
      case E_ELEM_BTN_BACK:
        gslc_SetPageCur(&m_gui, E_PAGE_MAIN);
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

  // ------------------------------------------------
  // Setup physical buttons
  // ------------------------------------------------
  btn_physical.begin(); // Init button
  btn_physical.onPressed(onPressed);  // Attach callback.

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

  // ------------------------------------------------
  // Periodically call EasyButton read for dispatch
  // ------------------------------------------------
  btn_physical.read(); 

  // In most programs, we would detect the button press and take an
  // action. In this simplest of examples, the "Quit" button callback
  // just outputs a message to the serial monitor when pressed and
  // sets the variable m_bQuit to true but the main loop continues to run.
  if (m_bQuit) {
    // Output a message when the button is pressed
    Serial.println("Quit button pressed");
  }

}
