//<App !Start!>
// FILE: [rotary-encoder.ino]
// Created by GUIslice Builder version: [0.17.b41]
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
#include "rotary-encoder_GSLC.h"

#include "EasyButton.h"

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Define the pins connected to the rotary encoder
const int A_PIN =  18;  // A (Clock)
const int B_PIN =  19;  // B (DT)
const int SWITCH_PIN = 17;  // Select Button 

EasyButton btn_sel( SWITCH_PIN, 35, true, true);

// define PIN GUI States
const int STATE_PREVIOUS = 1;
const int STATE_SELECT   = 2;
const int STATE_NEXT     = 3;

#define MAX_INPUT_MAP       4
gslc_tsInputMap             m_asInputMap[MAX_INPUT_MAP];

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_Btn_Item1       = NULL;
gslc_tsElemRef* m_Btn_Item2       = NULL;
gslc_tsElemRef* m_Elem_BTN1       = NULL;
gslc_tsElemRef* m_pCB_Option1     = NULL;
gslc_tsElemRef* m_pCB_Option2     = NULL;
gslc_tsElemRef* m_pElemToggle1    = NULL;
gslc_tsElemRef* m_pElemToggle2    = NULL;
//<Save_References !End!>

// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }

// ------------------------------------------------
//  Polling routine to Read Rotary Encoder
//  NOTE: I use polling instead of interrupts to avoid
//        getting into deep trouble as the random bouncing
//        of the input will trigger interrupts all the
//        time impacting performance of the app while
//        also giving false data.
// ------------------------------------------------
// A vald CW returns 1 or CCW move returns -1, invalid returns 0.
int8_t read_rotary() {
  // checkout: https://www.best-microcontroller-projects.com/rotary-encoder.html
  // for details on rot_enc_table
  // It provides a software debounce ability.
  static const int8_t rot_enc_table[]= {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};
  static uint8_t prevNextCode = 0;
  static uint16_t store=0;

  prevNextCode <<= 2;
  if (digitalRead(A_PIN)) prevNextCode |= 0x02;
  if (digitalRead(B_PIN)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

   // If valid then store as 16 bit data.
   if  (rot_enc_table[prevNextCode] ) {
      store <<= 4;
      store |= prevNextCode;
      if ((store&0xff)==0x2b) return -1;
      if ((store&0xff)==0x17) return 1;
   }
   return 0;
}

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
bool CbPinPoll(void* pvGui, int16_t* pnPinInd, int16_t* pnPinVal)
{
  // Read the pushbutton
  btn_sel.read();
  if (btn_sel.wasPressed()) { 
    *pnPinInd = 2;  
    *pnPinVal = 1; 
    Serial.println("btn_sel.wasPressed()");
    return true;
  } else if (btn_sel.wasReleased()) { 
    Serial.println("btn_sel.wasReleased()");
    *pnPinInd = 2;  
    *pnPinVal = 0; 
    return true;
  }

  // Sample the rotary encoder.
  int8_t encoderPos = read_rotary();
  // Here we are only concerned with direction 
  // of rotation Clockwise or Counter Clockwise
  if (encoderPos != 0) {
    // Advance the GUI navigation from the rotary encoder
    // The action to take is returned in pnPinInd 
    // assigned in gslc_InputMapAdd)
    if (encoderPos == 1) { 
      Serial.println("Clockwise");
      *pnPinInd = STATE_NEXT; *pnPinVal = 1; // Rotary Increment -> GUI next
    } else {
      Serial.println("Counter Clockwise");
      *pnPinInd = STATE_PREVIOUS; *pnPinVal = 1; // Rotary Decrement -> GUI previous
    }
    return true;
  }
  
  // If we reach here, then no input event was detected
  return false;
}

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
      case E_ELEM_BTN1:
        Serial.println("Jump to Menu 1");
        gslc_SetPageCur(&m_gui, E_PG_MENU1);
        break;
      case E_ELEM_BTN2:
        Serial.println("Jump to Menu 2");
        gslc_SetPageCur(&m_gui, E_PG_MENU2);
        break;
      case E_ELEM_BTN6:
        gslc_SetPageCur(&m_gui, E_PG_MENU3);
        break;
      case E_ELEM_BTN4:
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
        break;
      case E_ELEM_TOGGLE1:
        if (gslc_ElemXTogglebtnGetState(&m_gui, m_pElemToggle1)) {
          Serial.println("Style 1 Selected");
        } else {
          Serial.println("Style 1 UnSelected");
        }
        break;
      case E_ELEM_TOGGLE2:
        if (gslc_ElemXTogglebtnGetState(&m_gui, m_pElemToggle2)) {
          Serial.println("Style 2 Selected");
        } else {
          Serial.println("Style 2 UnSelected");
        }
         break;
      case E_ELEM_BTN5:
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
        break;
      case E_ELEM_BTN_ITEM1:
        gslc_PopupShow(&m_gui, E_PG_POPUP1, true);
        break;
      case E_ELEM_BTN_ITEM2:
        gslc_PopupShow(&m_gui, E_PG_POPUP2, true);
        break;
      case E_ELEM_BTN9:
        gslc_SetPageCur(&m_gui, E_PG_MAIN);
        break;
      case E_ELEM_BTN_BACK1:
        gslc_PopupHide(&m_gui);
        break;
      case E_ELEM_BTN_BACK2:
        gslc_PopupHide(&m_gui);
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
      if (bChecked) {
        Serial.println("Option 1 Checked");
      } else {
        Serial.println("Option 1 UnChecked");
      }
      break;
    case E_ELEM_CHECK2:
      if (bChecked) {
        Serial.println("Option 2 Checked");
      } else {
        Serial.println("Option 2 UnChecked");
      }
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
  Serial.begin(115200);
  // Wait for USB Serial 
  delay(1000);  // NOTE: Some devices require a delay after Serial.begin() before serial port can be used

  gslc_InitDebug(&DebugOut);

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUIslice_gen();

  // Configure encoder pins as inputs with internal pull-ups
  pinMode(A_PIN, INPUT_PULLUP);
  pinMode(B_PIN, INPUT_PULLUP);
  pinMode(SWITCH_PIN, INPUT_PULLUP); 
  
  gslc_tsGui* pGui = (gslc_tsGui*)&m_gui;
  
  // Set the pin poll callback function
  gslc_SetPinPollFunc(pGui, CbPinPoll);

  // Create the GUI input mapping (pin event to GUI action)
  gslc_InitInputMap(pGui,m_asInputMap,MAX_INPUT_MAP);
  gslc_InputMapAdd(pGui, GSLC_INPUT_PIN_ASSERT,   STATE_PREVIOUS, GSLC_ACTION_FOCUS_PREV, 0);
  gslc_InputMapAdd(pGui, GSLC_INPUT_PIN_ASSERT,   STATE_SELECT, GSLC_ACTION_PRESELECT, 0);
  gslc_InputMapAdd(pGui, GSLC_INPUT_PIN_DEASSERT, STATE_SELECT, GSLC_ACTION_SELECT, 0);
  gslc_InputMapAdd(pGui, GSLC_INPUT_PIN_ASSERT,   STATE_NEXT, GSLC_ACTION_FOCUS_NEXT, 0);

  // Set focus on the first menu element on the main page.
  gslc_TrackInput(pGui,(gslc_teInputRawEvent)GSLC_INPUT_PIN_ASSERT,3 /*next*/);

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
