//<File !Start!>
// FILE: [popup_input_GSLC.h]
// Created by GUIslice Builder version: [0.17.b17]
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
#include "elem/XKeyPad_Num.h"

// Ensure optional features are enabled in the configuration
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
#include "FreeSans8pt7b.h"
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
enum {E_PG_MAIN,E_PG_APP,E_PG_POPUP_CODE,E_POP_KEYPAD_NUM};
enum {E_BTN_START,E_ELEM_BOX1,E_ELEM_TEXT1,E_ELEM_TEXT4,E_LBL_ANSWER
      ,E_LBL_CODE,E_LBL_SECRET,E_LBL_STARTED,E_NUMINPUT_CODE
      ,E_NUMINPUT_CODE2,E_TXT_ANSWER,E_ELEM_KEYPAD_NUM};
// Must use separate enum for fonts with MAX_FONT at end to use gslc_FontSet.
enum {E_BUILTIN10X16,E_BUILTIN5X8,E_FREESANS8,MAX_FONT};
//<Enum !End!>

// ------------------------------------------------
// Instantiate the GUI
// ------------------------------------------------

// ------------------------------------------------
// Define the maximum number of elements and pages
// ------------------------------------------------
//<ElementDefines !Start!>
#define MAX_PAGE                4

#define MAX_ELEM_PG_MAIN 4 // # Elems total on page
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN // # Elems in RAM

#define MAX_ELEM_PG_APP 3 // # Elems total on page
#define MAX_ELEM_PG_APP_RAM MAX_ELEM_PG_APP // # Elems in RAM

#define MAX_ELEM_PG_POPUP_CODE 4 // # Elems total on page
#define MAX_ELEM_PG_POPUP_CODE_RAM MAX_ELEM_PG_POPUP_CODE // # Elems in RAM
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
gslc_tsElem                     m_asPage2Elem[MAX_ELEM_PG_APP_RAM];
gslc_tsElemRef                  m_asPage2ElemRef[MAX_ELEM_PG_APP];
gslc_tsElem                     m_asPopup1Elem[MAX_ELEM_PG_POPUP_CODE_RAM];
gslc_tsElemRef                  m_asPopup1ElemRef[MAX_ELEM_PG_POPUP_CODE];
gslc_tsElem                     m_asKeypadNumElem[1];
gslc_tsElemRef                  m_asKeypadNumElemRef[1];
gslc_tsXKeyPad                  m_sKeyPadNum;

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Element References for direct access
//<Extern_References !Start!>
extern gslc_tsElemRef* m_pAnswer;
extern gslc_tsElemRef* m_pNumCode;
extern gslc_tsElemRef* m_pNumCode2;
extern gslc_tsElemRef* m_pElemKeyPadNum;
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
    if (!gslc_FontSet(&m_gui,E_BUILTIN10X16,GSLC_FONTREF_PTR,NULL,2)) { return; }
    if (!gslc_FontSet(&m_gui,E_BUILTIN5X8,GSLC_FONTREF_PTR,NULL,1)) { return; }
    if (!gslc_FontSet(&m_gui,E_FREESANS8,GSLC_FONTREF_PTR,&FreeSans8pt7b,1)) { return; }
//<Load_Fonts !End!>

//<InitGUI !Start!>
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPage1Elem,MAX_ELEM_PG_MAIN_RAM,m_asPage1ElemRef,MAX_ELEM_PG_MAIN);
  gslc_PageAdd(&m_gui,E_PG_APP,m_asPage2Elem,MAX_ELEM_PG_APP_RAM,m_asPage2ElemRef,MAX_ELEM_PG_APP);
  gslc_PageAdd(&m_gui,E_PG_POPUP_CODE,m_asPopup1Elem,MAX_ELEM_PG_POPUP_CODE_RAM,m_asPopup1ElemRef,MAX_ELEM_PG_POPUP_CODE);
  gslc_PageAdd(&m_gui,E_POP_KEYPAD_NUM,m_asKeypadNumElem,1,m_asKeypadNumElemRef,1);  // KeyPad

  // NOTE: The current page defaults to the first page added. Here we explicitly
  //       ensure that the main page is the correct page no matter the add order.
  gslc_SetPageCur(&m_gui,E_PG_MAIN);
  
  // Set Background to a flat color
  gslc_SetBkgndColor(&m_gui,GSLC_COL_BLACK);

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
  
  // Create E_ELEM_TEXT1 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT1,E_PG_MAIN,(gslc_tsRect){33,20,253,20},
    (char*)"Demo Popup with Input",0,E_BUILTIN10X16);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_LBL_CODE text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_CODE,E_PG_MAIN,(gslc_tsRect){35,75,31,12},
    (char*)"Code:",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_NUMINPUT_CODE numeric input field
  static char m_sInputNumber1[6] = "";
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_NUMINPUT_CODE,E_PG_MAIN,(gslc_tsRect){75,75,40,10},
    (char*)m_sInputNumber1,6,E_BUILTIN5X8);
  gslc_ElemSetTxtMargin(&m_gui,pElemRef,5);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
  gslc_ElemSetClickEn(&m_gui, pElemRef, true);
  gslc_ElemSetTouchFunc(&m_gui, pElemRef, &CbBtnCommon);
  m_pNumCode = pElemRef;
  
  // create E_BTN_START button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_START,E_PG_MAIN,
    (gslc_tsRect){35,130,80,30},(char*)"Start App",0,E_BUILTIN5X8,&CbBtnCommon);

  // -----------------------------------
  // PAGE: E_PG_APP
  
  
  // Create E_LBL_STARTED text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_STARTED,E_PG_APP,(gslc_tsRect){96,105,127,10},
    (char*)"Your App has started!",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_LBL_ANSWER text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_ANSWER,E_PG_APP,(gslc_tsRect){25,195,31,12},
    (char*)"Code:",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_TXT_ANSWER runtime modifiable text
  static char m_sDisplayText6[6] = "";
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_TXT_ANSWER,E_PG_APP,(gslc_tsRect){65,195,31,10},
    (char*)m_sDisplayText6,6,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  m_pAnswer = pElemRef;

  // -----------------------------------
  // PAGE: E_PG_POPUP_CODE
  
   
  // Create E_ELEM_BOX1 box
  pElemRef = gslc_ElemCreateBox(&m_gui,E_ELEM_BOX1,E_PG_POPUP_CODE,(gslc_tsRect){85,55,150,75});
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,GSLC_COL_GRAY_LT1,GSLC_COL_GRAY_LT1);
  
  // Create E_ELEM_TEXT4 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT4,E_PG_POPUP_CODE,(gslc_tsRect){102,70,115,10},
    (char*)"You must enter your",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
  
  // Create E_NUMINPUT_CODE2 numeric input field
  static char m_sInputNumber2[6] = "";
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_NUMINPUT_CODE2,E_PG_POPUP_CODE,(gslc_tsRect){140,110,40,12},
    (char*)m_sInputNumber2,6,E_BUILTIN5X8);
  gslc_ElemSetTxtMargin(&m_gui,pElemRef,5);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
  gslc_ElemSetClickEn(&m_gui, pElemRef, true);
  gslc_ElemSetTouchFunc(&m_gui, pElemRef, &CbBtnCommon);
  m_pNumCode2 = pElemRef;
  
  // Create E_LBL_SECRET text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_SECRET,E_PG_POPUP_CODE,(gslc_tsRect){123,85,73,10},
    (char*)"secret code!",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);

  // -----------------------------------
  // PAGE: E_POP_KEYPAD_NUM
  
  static gslc_tsXKeyPadCfg_Num sCfg;
  sCfg = gslc_ElemXKeyPadCfgInit_Num();
  gslc_ElemXKeyPadCfgSetFloatEn_Num(&sCfg, false);
  gslc_ElemXKeyPadCfgSetSignEn_Num(&sCfg, false);
  gslc_ElemXKeyPadCfgSetButtonSz((gslc_tsXKeyPadCfg*)&sCfg, 30, 30);
  m_pElemKeyPadNum = gslc_ElemXKeyPadCreate_Num(&m_gui, E_ELEM_KEYPAD_NUM, E_POP_KEYPAD_NUM,
    &m_sKeyPadNum, 40, 60, E_FREESANS8, &sCfg);
  gslc_ElemXKeyPadValSetCb(&m_gui, m_pElemKeyPadNum, &CbKeypad);
//<InitGUI !End!>

//<Startup !Start!>
//<Startup !End!>

}

#endif // end _GUISLICE_GEN_H
