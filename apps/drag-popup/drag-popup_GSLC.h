//<File !Start!>
// FILE: [drag-popup_GSLC.h]
// Created by GUIslice Builder version: [0.17.b18]
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
enum {E_PG_MAIN,E_PG_POPUP1};
enum {E_BOX_TitleBar,E_ELEM_BTN_CLOSE_WINDOW,E_ELEM_BTN_Cancel
      ,E_ELEM_BTN_No,E_ELEM_BTN_Popup,E_ELEM_BTN_Yes,E_ELEM_TEXT_L1
      ,E_ELEM_TEXT_L2,E_ELEM_TEXT_L3,E_ELEM_Title,E_ELEM_Window};
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
#define MAX_PAGE                2

#define MAX_ELEM_PG_MAIN 1 // # Elems total on page
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN // # Elems in RAM

#define MAX_ELEM_PG_POPUP1 10 // # Elems total on page
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
gslc_tsElem                     m_asPopup1Elem[MAX_ELEM_PG_POPUP1_RAM];
gslc_tsElemRef                  m_asPopup1ElemRef[MAX_ELEM_PG_POPUP1];

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Element References for direct access
//<Extern_References !Start!>
extern gslc_tsElemRef* CloseWindow1;
extern gslc_tsElemRef* Title1;
extern gslc_tsElemRef* TitleBar1;
extern gslc_tsElemRef* Window1;
extern gslc_tsElemRef* btnCancel;
extern gslc_tsElemRef* btnNo;
extern gslc_tsElemRef* btnYes;
extern gslc_tsElemRef* label1;
extern gslc_tsElemRef* label2;
extern gslc_tsElemRef* label3;
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
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPage1Elem,MAX_ELEM_PG_MAIN_RAM,m_asPage1ElemRef,MAX_ELEM_PG_MAIN);
  gslc_PageAdd(&m_gui,E_PG_POPUP1,m_asPopup1Elem,MAX_ELEM_PG_POPUP1_RAM,m_asPopup1ElemRef,MAX_ELEM_PG_POPUP1);

  // NOTE: The current page defaults to the first page added. Here we explicitly
  //       ensure that the main page is the correct page no matter the add order.
  gslc_SetPageCur(&m_gui,E_PG_MAIN);
  
  // Set Background to a flat color
  gslc_SetBkgndColor(&m_gui,((gslc_tsColor){18,18,18}));

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
  
  // create E_ELEM_BTN_Popup button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_Popup,E_PG_MAIN,
    (gslc_tsRect){31,29,80,40},(char*)"Popup",0,E_BUILTIN10X16,&CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){228,228,229}),((gslc_tsColor){55,0,179}),GSLC_COL_GRAY_DK1);

  // -----------------------------------
  // PAGE: E_PG_POPUP1
  
   
  // Create E_ELEM_Window box
  pElemRef = gslc_ElemCreateBox(&m_gui,E_ELEM_Window,E_PG_POPUP1,(gslc_tsRect){12,89,210,120});
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_WHITE,GSLC_COL_WHITE);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,false);
  Window1 = pElemRef;
   
  // Create E_BOX_TitleBar box
  pElemRef = gslc_ElemCreateBox(&m_gui,E_BOX_TitleBar,E_PG_POPUP1,(gslc_tsRect){12,89,210,20});
  gslc_ElemSetClickEn(&m_gui, pElemRef, true);
  gslc_ElemSetTouchFunc(&m_gui, pElemRef, &CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){56,60,74}),((gslc_tsColor){55,0,179}),((gslc_tsColor){55,0,179}));
  gslc_ElemSetFrameEn(&m_gui,pElemRef,false);
  TitleBar1 = pElemRef;
  
  // Create E_ELEM_Title text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_Title,E_PG_POPUP1,(gslc_tsRect){18,89,133,18},
    (char*)"Window Test",0,E_BUILTIN10X16);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){228,228,229}),((gslc_tsColor){55,0,179}),GSLC_COL_GRAY_LT2);
  Title1 = pElemRef;
  
  // create E_ELEM_BTN_CLOSE_WINDOW button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_CLOSE_WINDOW,E_PG_POPUP1,
    (gslc_tsRect){206,90,15,15},(char*)"X",0,E_BUILTIN5X8,&CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){228,228,229}),((gslc_tsColor){55,0,179}),GSLC_COL_GRAY_DK1);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,false);
  CloseWindow1 = pElemRef;
  
  // Create E_ELEM_TEXT_L1 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT_L1,E_PG_POPUP1,(gslc_tsRect){20,115,175,10},
    (char*)"This is an example for Window",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){228,228,229}),GSLC_COL_WHITE,GSLC_COL_GRAY_LT2);
  label1 = pElemRef;
  
  // Create E_ELEM_TEXT_L2 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT_L2,E_PG_POPUP1,(gslc_tsRect){20,130,193,10},
    (char*)"PopUp Movement. Touch the Title ",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){228,228,229}),GSLC_COL_WHITE,GSLC_COL_WHITE);
  label2 = pElemRef;
  
  // Create E_ELEM_TEXT_L3 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT_L3,E_PG_POPUP1,(gslc_tsRect){20,145,193,10},
    (char*)"Bar,and you can move the window.",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_BLACK);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){228,228,229}),GSLC_COL_WHITE,GSLC_COL_WHITE);
  label3 = pElemRef;
  
  // create E_ELEM_BTN_No button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_No,E_PG_POPUP1,
    (gslc_tsRect){70,170,40,30},(char*)"No",0,E_BUILTIN5X8,&CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){228,228,229}),((gslc_tsColor){55,0,179}),GSLC_COL_GRAY_DK1);
  btnNo = pElemRef;
  
  // create E_ELEM_BTN_Yes button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_Yes,E_PG_POPUP1,
    (gslc_tsRect){20,170,40,30},(char*)"Yes",0,E_BUILTIN5X8,&CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){228,228,229}),((gslc_tsColor){55,0,179}),GSLC_COL_GRAY_DK1);
  btnYes = pElemRef;
  
  // create E_ELEM_BTN_Cancel button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_Cancel,E_PG_POPUP1,
    (gslc_tsRect){160,170,45,30},(char*)"Cancel",0,E_BUILTIN5X8,&CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,((gslc_tsColor){228,228,229}),((gslc_tsColor){55,0,179}),GSLC_COL_GRAY_DK1);
  btnCancel = pElemRef;
//<InitGUI !End!>

//<Startup !Start!>
//<Startup !End!>

}

#endif // end _GUISLICE_GEN_H
