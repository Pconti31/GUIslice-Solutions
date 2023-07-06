//<File !Start!>
// FILE: [screen-saver-logo_GSLC.h]
// Created by GUIslice Builder version: [0.17.b24]
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
enum {E_PG_BASE,E_PG_MAIN,E_PG2,E_POP_KEYPAD_NUM};
enum {E_BTN_1,E_BTN_2,E_BTN_3,E_BTN_4,E_BTN_5,E_BTN_6,E_BTN_7,E_BTN_8
      ,E_BTN_9,E_ELEM_BTN_BUSY,E_ELEM_CHECK1,E_ELEM_TEXT1,E_ELEM_TEXT10
      ,E_ELEM_TEXT2,E_ELEM_TEXT4,E_ELEM_TEXT5,E_ELEM_TEXT8
      ,E_ELEM_TEXT_STATUS,E_NUMINPUT_TIMEOUT,E_TXT_COUNTDOWN
      ,E_ELEM_KEYPAD_NUM};
// Must use separate enum for fonts with MAX_FONT at end to use gslc_FontSet.
enum {E_BUILTIN10X16,E_BUILTIN5X8,MAX_FONT};
//<Enum !End!>

// ------------------------------------------------
// Instantiate the GUI
// ------------------------------------------------

// ------------------------------------------------
// Define the maximum number of elements and pages
// ------------------------------------------------
//<ElementDefines !Start!>
#define MAX_PAGE                4

#define MAX_ELEM_PG_BASE 3 // # Elems total on page
#define MAX_ELEM_PG_BASE_RAM MAX_ELEM_PG_BASE // # Elems in RAM

#define MAX_ELEM_PG_MAIN 8 // # Elems total on page
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN // # Elems in RAM

#define MAX_ELEM_PG2 9 // # Elems total on page
#define MAX_ELEM_PG2_RAM MAX_ELEM_PG2 // # Elems in RAM
//<ElementDefines !End!>

// ------------------------------------------------
// Create element storage
// ------------------------------------------------
gslc_tsGui                      m_gui;
gslc_tsDriver                   m_drv;
gslc_tsFont                     m_asFont[MAX_FONT];
gslc_tsPage                     m_asPage[MAX_PAGE];

//<GUI_Extra_Elements !Start!>
gslc_tsElem                     m_asBasePage1Elem[MAX_ELEM_PG_BASE_RAM];
gslc_tsElemRef                  m_asBasePage1ElemRef[MAX_ELEM_PG_BASE];
gslc_tsElem                     m_asPage1Elem[MAX_ELEM_PG_MAIN_RAM];
gslc_tsElemRef                  m_asPage1ElemRef[MAX_ELEM_PG_MAIN];
gslc_tsElem                     m_asPage2Elem[MAX_ELEM_PG2_RAM];
gslc_tsElemRef                  m_asPage2ElemRef[MAX_ELEM_PG2];
gslc_tsElem                     m_asKeypadNumElem[1];
gslc_tsElemRef                  m_asKeypadNumElemRef[1];
gslc_tsXKeyPad                  m_sKeyPadNum;
gslc_tsXCheckbox                m_asXCheck1;

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Element References for direct access
//<Extern_References !Start!>
extern gslc_tsElemRef* m_pElemCBCheckBox;
extern gslc_tsElemRef* m_pTimeout;
extern gslc_tsElemRef* m_pTxtCountDown;
extern gslc_tsElemRef* m_pTxtStatus;
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
//<Load_Fonts !End!>

//<InitGUI !Start!>
  gslc_PageAdd(&m_gui,E_PG_BASE,m_asBasePage1Elem,MAX_ELEM_PG_BASE_RAM,m_asBasePage1ElemRef,MAX_ELEM_PG_BASE);
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPage1Elem,MAX_ELEM_PG_MAIN_RAM,m_asPage1ElemRef,MAX_ELEM_PG_MAIN);
  gslc_PageAdd(&m_gui,E_PG2,m_asPage2Elem,MAX_ELEM_PG2_RAM,m_asPage2ElemRef,MAX_ELEM_PG2);
  gslc_PageAdd(&m_gui,E_POP_KEYPAD_NUM,m_asKeypadNumElem,1,m_asKeypadNumElemRef,1);  // KeyPad

  // Now mark E_PG_BASE as a "base" page which means that it's elements
  // are always visible. This is useful for common page elements.
  gslc_SetPageBase(&m_gui, E_PG_BASE);


  // NOTE: The current page defaults to the first page added. Here we explicitly
  //       ensure that the main page is the correct page no matter the add order.
  gslc_SetPageCur(&m_gui,E_PG_MAIN);
  
  // Set Background to a flat color
  gslc_SetBkgndColor(&m_gui,GSLC_COL_BLACK);

  // -----------------------------------
  // PAGE: E_PG_BASE
  
  
  // Create E_ELEM_TEXT8 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT8,E_PG_BASE,(gslc_tsRect){10,210,130,12},
    (char*)"Inactivity Count Down:",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_TXT_COUNTDOWN runtime modifiable text
  static char m_sDisplayText9[3] = "10";
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_TXT_COUNTDOWN,E_PG_BASE,(gslc_tsRect){150,205,30,20},
    (char*)m_sDisplayText9,3,E_BUILTIN10X16);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  m_pTxtCountDown = pElemRef;
  
  // Create E_ELEM_TEXT10 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT10,E_PG_BASE,(gslc_tsRect){185,210,30,12},
    (char*)"Secs",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
  
  // Create E_ELEM_TEXT1 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT1,E_PG_MAIN,(gslc_tsRect){47,15,225,14},
    (char*)"Sample Screen Saver",0,E_BUILTIN10X16);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // create E_ELEM_BTN_BUSY button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_BUSY,E_PG_MAIN,
    (gslc_tsRect){10,160,80,40},(char*)"Next Page",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // Create E_ELEM_TEXT2 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT2,E_PG_MAIN,(gslc_tsRect){20,55,144,8},
    (char*)"That only appears after:",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_ELEM_TEXT4 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT4,E_PG_MAIN,(gslc_tsRect){20,71,126,8},
    (char*)"seconds of inactivity",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
   
  // create checkbox E_ELEM_CHECK1
  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_ELEM_CHECK1,E_PG_MAIN,&m_asXCheck1,
    (gslc_tsRect){190,120,20,20},false,GSLCX_CHECKBOX_STYLE_X,GSLC_COL_ORANGE,false);
  gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
  m_pElemCBCheckBox = pElemRef;
  
  // Create E_ELEM_TEXT5 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT5,E_PG_MAIN,(gslc_tsRect){10,125,170,12},
    (char*)"Check & Uncheck to keep busy",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  
  // Create E_ELEM_TEXT_STATUS runtime modifiable text
  static char m_sDisplayText11[4] = "OFF";
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT_STATUS,E_PG_MAIN,(gslc_tsRect){120,170,36,16},
    (char*)m_sDisplayText11,4,E_BUILTIN10X16);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  m_pTxtStatus = pElemRef;
  
  // Create E_NUMINPUT_TIMEOUT numeric input field
  static char m_sInputNumber2[6] = "10";
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_NUMINPUT_TIMEOUT,E_PG_MAIN,(gslc_tsRect){180,50,70,20},
    (char*)m_sInputNumber2,6,E_BUILTIN10X16);
  gslc_ElemSetTxtMargin(&m_gui,pElemRef,5);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,true);
  gslc_ElemSetClickEn(&m_gui, pElemRef, true);
  gslc_ElemSetTouchFunc(&m_gui, pElemRef, &CbBtnCommon);
  m_pTimeout = pElemRef;

  // -----------------------------------
  // PAGE: E_PG2
  
  
  // create E_BTN_1 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_1,E_PG2,
    (gslc_tsRect){20,30,80,40},(char*)"BTN 1",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // create E_BTN_2 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_2,E_PG2,
    (gslc_tsRect){120,30,80,40},(char*)"BTN 2",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // create E_BTN_3 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_3,E_PG2,
    (gslc_tsRect){220,30,80,40},(char*)"BTN 3",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // create E_BTN_4 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_4,E_PG2,
    (gslc_tsRect){20,90,80,40},(char*)"BTN 4",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // create E_BTN_5 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_5,E_PG2,
    (gslc_tsRect){120,90,80,40},(char*)"BTN 5",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // create E_BTN_6 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_6,E_PG2,
    (gslc_tsRect){220,90,80,40},(char*)"BTN 6",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // create E_BTN_7 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_7,E_PG2,
    (gslc_tsRect){20,150,80,40},(char*)"BTN 7",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // create E_BTN_8 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_8,E_PG2,
    (gslc_tsRect){120,150,80,40},(char*)"BTN 8",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // create E_BTN_9 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_9,E_PG2,
    (gslc_tsRect){220,150,80,40},(char*)"BTN 9",0,E_BUILTIN5X8,&CbBtnCommon);

  // -----------------------------------
  // PAGE: E_POP_KEYPAD_NUM
  
  static gslc_tsXKeyPadCfg_Num sCfg;
  sCfg = gslc_ElemXKeyPadCfgInit_Num();
  gslc_ElemXKeyPadCfgSetFloatEn_Num(&sCfg, true);
  gslc_ElemXKeyPadCfgSetSignEn_Num(&sCfg, true);
  m_pElemKeyPadNum = gslc_ElemXKeyPadCreate_Num(&m_gui, E_ELEM_KEYPAD_NUM, E_POP_KEYPAD_NUM,
    &m_sKeyPadNum, 65, 80, E_BUILTIN5X8, &sCfg);
  gslc_ElemXKeyPadValSetCb(&m_gui, m_pElemKeyPadNum, &CbKeypad);
//<InitGUI !End!>

//<Startup !Start!>
  gslc_GuiRotate(&m_gui, 1);
//<Startup !End!>

}

#endif // end _GUISLICE_GEN_H
