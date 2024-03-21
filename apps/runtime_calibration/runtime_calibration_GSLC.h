//<File !Start!>
// FILE: [runtime_calibration_GSLC.h]
// Created by GUIslice Builder version: [0.17.b27]
//
// GUIslice Builder Generated GUI Framework File
//
// For the latest guides, updates and support view:
// https://github.com/ImpulseAdventure/GUIslice
//
//<File !End!>

#ifndef _GUISLICE_GEN_H
#define _GUISLICE_GEN_H

// ------------------------------------------------
// Headers to include
// ------------------------------------------------
#include "GUIslice.h"
#include "GUIslice_drv.h"

// Include any extended elements
//<Includes !Start!>
// Include extended elements
#include "elem/XCheckbox.h"
//<Includes !End!>

// ------------------------------------------------
// Headers and Defines for fonts
// Note that font files are located within the Adafruit-GFX library folder:
// ------------------------------------------------
//<Fonts !Start!>
#if defined(DRV_DISP_TFT_ESPI)
  #error E_PROJECT_OPTIONS tab->Graphics Library should be TFT_eSPI
#endif
#include <Adafruit_GFX.h>
#include "NotoMono10pt7b.h"
//<Fonts !End!>

// ------------------------------------------------
// Defines for resources
// ------------------------------------------------
//<Resources !Start!>
//<Resources !End!>

// ------------------------------------------------
// Enumerations for pages, elements, fonts, images
// ------------------------------------------------
//<Enum !Start!>
enum {E_PG_MAIN,E_PG_CALIBRATION,E_PG_POPUP1};
enum {E_ELEM_BOX2,E_ELEM_BTN_CALIBRATE,E_ELEM_BTN_DONE,E_ELEM_BTN_QUIT
      ,E_ELEM_BTN_SOMETHING,E_ELEM_BTN_START,E_ELEM_EXAMPLE
      ,E_ELEM_RADIO1,E_ELEM_RADIO2,E_ELEM_STATUS,E_ELEM_TEXT10
      ,E_ELEM_TEXT11,E_ELEM_TEXT12,E_ELEM_TEXT13,E_ELEM_TEXT9
      ,E_ELEM_TITLE};
enum {E_GROUP1};
// Must use separate enum for fonts with MAX_FONT at end to use gslc_FontSet.
enum {E_BUILTIN5X8,E_NOTOMONO10,MAX_FONT};
//<Enum !End!>

// ------------------------------------------------
// Instantiate the GUI
// ------------------------------------------------

// ------------------------------------------------
// Define the maximum number of elements and pages
// ------------------------------------------------
//<ElementDefines !Start!>
#define MAX_PAGE                3

#define MAX_ELEM_PG_MAIN 7 // # Elems total on page
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN // # Elems in RAM

#define MAX_ELEM_PG_CALIBRATION 9 // # Elems total on page
#define MAX_ELEM_PG_CALIBRATION_RAM MAX_ELEM_PG_CALIBRATION // # Elems in RAM

#define MAX_ELEM_PG_POPUP1 0 // # Elems total on page
#define MAX_ELEM_PG_POPUP1_RAM MAX_ELEM_PG_POPUP1 // # Elems in RAM
//<ElementDefines !End!>

// ------------------------------------------------
// Create element storage
// ------------------------------------------------
gslc_tsGui                      m_gui;
gslc_tsDriver                   m_drv;
gslc_tsFont                     m_asFont[MAX_FONT];
gslc_tsPage                     m_asPage[MAX_PAGE];

//<GUI_Extra_Elements !Start!>
gslc_tsElem                     m_asPage1Elem[MAX_ELEM_PG_MAIN_RAM];
gslc_tsElemRef                  m_asPage1ElemRef[MAX_ELEM_PG_MAIN];
gslc_tsElem                     m_asPage2Elem[MAX_ELEM_PG_CALIBRATION_RAM];
gslc_tsElemRef                  m_asPage2ElemRef[MAX_ELEM_PG_CALIBRATION];
gslc_tsElem                     m_asPopup3Elem[MAX_ELEM_PG_POPUP1_RAM];
gslc_tsElemRef                  m_asPopup3ElemRef[MAX_ELEM_PG_POPUP1];
gslc_tsXCheckbox                m_asXRadio1;
gslc_tsXCheckbox                m_asXRadio2;

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Element References for direct access
//<Extern_References !Start!>
extern gslc_tsElemRef* m_pElemDo;
extern gslc_tsElemRef* m_pElemQuit;
extern gslc_tsElemRef* m_pElemRadioBnt1;
extern gslc_tsElemRef* m_pElemRadioBnt2;
extern gslc_tsElemRef* m_pTxtStatus;
//<Extern_References !End!>

// Define debug message function
static int16_t DebugOut(char ch);

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
bool CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY);
bool CbCheckbox(void* pvGui, void* pvElemRef, int16_t nSelId, bool bState);
bool CbDrawScanner(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw);
bool CbKeypad(void* pvGui, void *pvElemRef, int16_t nState, void* pvData);
bool CbListbox(void* pvGui, void* pvElemRef, int16_t nSelId);
bool CbSlidePos(void* pvGui,void* pvElemRef,int16_t nPos);
bool CbSpinner(void* pvGui, void *pvElemRef, int16_t nState, void* pvData);
bool CbTickScanner(void* pvGui,void* pvScope);

// ------------------------------------------------
// Create page elements
// ------------------------------------------------
void InitGUIslice_gen()
{
  gslc_tsElemRef* pElemRef = NULL;

  if (!gslc_Init(&m_gui,&m_drv,m_asPage,MAX_PAGE,m_asFont,MAX_FONT)) { return; }

  // ------------------------------------------------
  // Load Fonts
  // ------------------------------------------------
//<Load_Fonts !Start!>
    if (!gslc_FontSet(&m_gui,E_BUILTIN5X8,GSLC_FONTREF_PTR,NULL,1)) { return; }
    if (!gslc_FontSet(&m_gui,E_NOTOMONO10,GSLC_FONTREF_PTR,&NotoMono10pt7b,1)) { return; }
//<Load_Fonts !End!>

//<InitGUI !Start!>
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPage1Elem,MAX_ELEM_PG_MAIN_RAM,m_asPage1ElemRef,MAX_ELEM_PG_MAIN);
  gslc_PageAdd(&m_gui,E_PG_CALIBRATION,m_asPage2Elem,MAX_ELEM_PG_CALIBRATION_RAM,m_asPage2ElemRef,MAX_ELEM_PG_CALIBRATION);
  gslc_PageAdd(&m_gui,E_PG_POPUP1,m_asPopup3Elem,MAX_ELEM_PG_POPUP1_RAM,m_asPopup3ElemRef,MAX_ELEM_PG_POPUP1);

  // NOTE: The current page defaults to the first page added. Here we explicitly
  //       ensure that the main page is the correct page no matter the add order.
  gslc_SetPageCur(&m_gui,E_PG_MAIN);
  
  // Set Background to a flat color
  gslc_SetBkgndColor(&m_gui,GSLC_COL_GRAY);

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
  
  // Create E_ELEM_TITLE text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TITLE,E_PG_MAIN,(gslc_tsRect){83,10,154,20},
    (char*)"My Sample App",0,E_NOTOMONO10);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
  
  // Create E_ELEM_BTN_QUIT button with modifiable text label
  static char m_strbtn1[11] = "QUIT";
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_QUIT,E_PG_MAIN,
    (gslc_tsRect){210,160,80,40},
    (char*)m_strbtn1,11,E_BUILTIN5X8,&CbBtnCommon);
  m_pElemQuit = pElemRef;
  
  // Create E_ELEM_BTN_SOMETHING button with modifiable text label
  static char m_strbtn2[16] = "DO SOMETHING";
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_SOMETHING,E_PG_MAIN,
    (gslc_tsRect){30,90,80,40},
    (char*)m_strbtn2,16,E_BUILTIN5X8,&CbBtnCommon);
  m_pElemDo = pElemRef;
  
  // create E_ELEM_BTN_CALIBRATE button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_CALIBRATE,E_PG_MAIN,
    (gslc_tsRect){30,160,80,40},(char*)"CALIBRATE",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // Create radio button E_ELEM_RADIO1
  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_ELEM_RADIO1,E_PG_MAIN,&m_asXRadio1,
    (gslc_tsRect){214,101,20,20},true,GSLCX_CHECKBOX_STYLE_ROUND,GSLC_COL_RED_DK2,true);
  gslc_ElemSetGroup(&m_gui,pElemRef,E_GROUP1);
  m_pElemRadioBnt1 = pElemRef;
  
  // Create radio button E_ELEM_RADIO2
  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_ELEM_RADIO2,E_PG_MAIN,&m_asXRadio2,
    (gslc_tsRect){264,101,20,20},true,GSLCX_CHECKBOX_STYLE_ROUND,GSLC_COL_GREEN_DK3,false);
  gslc_ElemSetGroup(&m_gui,pElemRef,E_GROUP1);
  gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
  m_pElemRadioBnt2 = pElemRef;
  
  // Create E_ELEM_EXAMPLE text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_EXAMPLE,E_PG_MAIN,(gslc_tsRect){67,50,186,8},
    (char*)"Runtime TFT Display Calibration",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLACK,GSLC_COL_BLACK,GSLC_COL_BLACK);

  // -----------------------------------
  // PAGE: E_PG_CALIBRATION
  
   
  // Create E_ELEM_BOX2 box
  pElemRef = gslc_ElemCreateBox(&m_gui,E_ELEM_BOX2,E_PG_CALIBRATION,(gslc_tsRect){10,20,300,200});
  
  // Create E_ELEM_TEXT9 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT9,E_PG_CALIBRATION,(gslc_tsRect){40,50,240,10},
    (char*)"In the following pages, use point/stylus",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_ELEM_TEXT10 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT10,E_PG_CALIBRATION,(gslc_tsRect){40,70,234,10},
    (char*)"to touch the crosshairs in each corner.",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_ELEM_TEXT11 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT11,E_PG_CALIBRATION,(gslc_tsRect){40,90,216,10},
    (char*)"Continue to hold until the crosshair",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_ELEM_TEXT12 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT12,E_PG_CALIBRATION,(gslc_tsRect){40,110,252,8},
    (char*)"disapprears, it will take about 3 seconds.",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_ELEM_TEXT13 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT13,E_PG_CALIBRATION,(gslc_tsRect){54,25,212,15},
    (char*)"Screen Calibration",0,E_NOTOMONO10);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // create E_ELEM_BTN_START button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_START,E_PG_CALIBRATION,
    (gslc_tsRect){100,130,100,20},(char*)"Click to Start",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // Create E_ELEM_STATUS runtime modifiable text
  static char m_sDisplayText14[21] = "";
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_STATUS,E_PG_CALIBRATION,(gslc_tsRect){90,160,120,8},
    (char*)m_sDisplayText14,21,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  m_pTxtStatus = pElemRef;
  
  // create E_ELEM_BTN_DONE button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_DONE,E_PG_CALIBRATION,
    (gslc_tsRect){100,180,100,20},(char*)"Done",0,E_BUILTIN5X8,&CbBtnCommon);

  // -----------------------------------
  // PAGE: E_PG_POPUP1
  
//<InitGUI !End!>

//<Startup !Start!>
//<Startup !End!>

}

#endif // end _GUISLICE_GEN_H
