//<App !Start!>
// FILE: [drag-popup.ino]
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
#include "drag-popup_GSLC.h"

//
// - Example of moving elements on screen
// Based upon code written by Lucy2003, South China University of Technology
// 

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Macro moveElem to calculate new coordinates and assign them back to element
#define moveElem(obj) rElemCur = gslc_ElemGetRect(&m_gui,obj);\
  rElemCur.x -= lastX-nX;rElemCur.y -= lastY-nY;gslc_ElemSetRect(&m_gui,obj,rElemCur);

int Touching=0;
int lastX=0;   //Position offset
int lastY=0;

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* CloseWindow1      = NULL;
gslc_tsElemRef* Title1            = NULL;
gslc_tsElemRef* TitleBar1         = NULL;
gslc_tsElemRef* Window1           = NULL;
gslc_tsElemRef* btnCancel         = NULL;
gslc_tsElemRef* btnNo             = NULL;
gslc_tsElemRef* btnYes            = NULL;
gslc_tsElemRef* label1            = NULL;
gslc_tsElemRef* label2            = NULL;
gslc_tsElemRef* label3            = NULL;
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
  gslc_tsRect rElemBox;
  if (eTouch == GSLC_TOUCH_DOWN_IN ) {
      if(pElem->nId == E_BOX_TitleBar){
        Touching=1;
        lastX=nX;
        lastY=nY;
        return true;
      }
  }
  if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_ELEM_BTN_Popup:
        gslc_PopupShow(&m_gui,E_PG_POPUP1,true);
        break;
      case E_BOX_TitleBar:
        Touching=0;
        break;
      case E_ELEM_BTN_CLOSE_WINDOW:
        gslc_PopupHide(&m_gui);
        break;
      case E_ELEM_BTN_No:
        gslc_PopupHide(&m_gui);
        break;
      case E_ELEM_BTN_Yes:
        gslc_PopupHide(&m_gui);
        break;
      case E_ELEM_BTN_Cancel:
        gslc_PopupHide(&m_gui);
        break;
//<Button Enums !End!>
      default:
        break;
    }
    return true;
  }
  //Move Window
  switch(pElem->nId){
      case E_BOX_TitleBar:
        /* WARNING! This code requires each element
				 * to have been assigned a ElementRef name
				 * otherwise the element will be passed over.
				 */
        if (Touching==1){
					/* this code allows you to add and remove elements
					 * from the page without needing to make any
					 * code modifications.
					 */
					gslc_tsPage* myPage = &m_asPage[E_PG_POPUP1]; // find our page reference
					// avoid window moving off screen
          if (lastX-nX==0) return true;
          if (lastY-nY==0) return true;
          gslc_tsRect rElemCur;
          gslc_tsElemRef* pElemRef;
          gslc_tsCollect* pCollect = &myPage->sCollect;
					// for each element on this page change its coordinates
          for (int i=0; i<pCollect->nElemCnt; i++) {
            pElemRef = &pCollect->asElemRef[i];
            moveElem(pElemRef);
          }
					// now we need to change the this page's bounds
          myPage->rBounds = gslc_ElemGetRect(&m_gui, Window1);
					// inform GUIslice API the page element has moved
          gslc_InvalidateRgnPage(&m_gui,myPage);
          return true;
        }
/* 
        Alternative way to move elements
        if (Touching==1){
          if (lastX-nX==0) return true;
          if (lastY-nY==0) return true;
          gslc_tsRect rElemCur;
          moveElem(TitleBar1);
          moveElem(Window1);
          moveElem(Title1);
          moveElem(CloseWindow1);
          moveElem(label1);
          moveElem(label2);
          moveElem(label3);
          moveElem(btnNo);
          moveElem(btnYes);
          moveElem(btnCancel);
          m_asPage[E_PG_POPUP1].rBounds = gslc_ElemGetRect(&m_gui, Window1);
          gslc_InvalidateRgnPage(&m_gui,&m_asPage[E_PG_POPUP1]);
          return true;
        }
*/
        break;
      default:
        break;
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

  gslc_ElemSetClickEn(&m_gui, gslc_PageFindElemById(&m_gui, E_PG_POPUP1, E_BOX_TitleBar), true);

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
