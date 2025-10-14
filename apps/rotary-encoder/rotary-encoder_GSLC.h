//<File !Start!>
// FILE: [rotary-encoder_GSLC.h]
// Created by GUIslice Builder version: [0.17.b41]
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
#include "elem/XTogglebtn.h"
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
enum {E_PG_MAIN,E_PG_MENU1,E_PG_MENU2,E_PG_MENU3,E_PG_POPUP1
      ,E_PG_POPUP2};
enum {E_ELEM_BOX1,E_ELEM_BOX2,E_ELEM_BTN1,E_ELEM_BTN2,E_ELEM_BTN4
      ,E_ELEM_BTN5,E_ELEM_BTN6,E_ELEM_BTN9,E_ELEM_BTN_BACK1
      ,E_ELEM_BTN_BACK2,E_ELEM_BTN_ITEM1,E_ELEM_BTN_ITEM2,E_ELEM_CHECK1
      ,E_ELEM_CHECK2,E_ELEM_TEXT1,E_ELEM_TEXT10,E_ELEM_TEXT2
      ,E_ELEM_TEXT3,E_ELEM_TEXT4,E_ELEM_TEXT5,E_ELEM_TEXT6,E_ELEM_TEXT7
      ,E_ELEM_TEXT8,E_ELEM_TEXT9,E_ELEM_TOGGLE1,E_ELEM_TOGGLE2};
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
#define MAX_PAGE                6

#define MAX_ELEM_PG_MAIN 4 // # Elems total on page
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN // # Elems in RAM

#define MAX_ELEM_PG_MENU1 6 // # Elems total on page
#define MAX_ELEM_PG_MENU1_RAM MAX_ELEM_PG_MENU1 // # Elems in RAM

#define MAX_ELEM_PG_MENU2 6 // # Elems total on page
#define MAX_ELEM_PG_MENU2_RAM MAX_ELEM_PG_MENU2 // # Elems in RAM

#define MAX_ELEM_PG_MENU3 4 // # Elems total on page
#define MAX_ELEM_PG_MENU3_RAM MAX_ELEM_PG_MENU3 // # Elems in RAM

#define MAX_ELEM_PG_POPUP1 3 // # Elems total on page
#define MAX_ELEM_PG_POPUP1_RAM MAX_ELEM_PG_POPUP1 // # Elems in RAM

#define MAX_ELEM_PG_POPUP2 3 // # Elems total on page
#define MAX_ELEM_PG_POPUP2_RAM MAX_ELEM_PG_POPUP2 // # Elems in RAM
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
gslc_tsElem                     m_asPage2Elem[MAX_ELEM_PG_MENU1_RAM];
gslc_tsElemRef                  m_asPage2ElemRef[MAX_ELEM_PG_MENU1];
gslc_tsElem                     m_asPage3Elem[MAX_ELEM_PG_MENU2_RAM];
gslc_tsElemRef                  m_asPage3ElemRef[MAX_ELEM_PG_MENU2];
gslc_tsElem                     m_asPage4Elem[MAX_ELEM_PG_MENU3_RAM];
gslc_tsElemRef                  m_asPage4ElemRef[MAX_ELEM_PG_MENU3];
gslc_tsElem                     m_asPopup1Elem[MAX_ELEM_PG_POPUP1_RAM];
gslc_tsElemRef                  m_asPopup1ElemRef[MAX_ELEM_PG_POPUP1];
gslc_tsElem                     m_asPopup2Elem[MAX_ELEM_PG_POPUP2_RAM];
gslc_tsElemRef                  m_asPopup2ElemRef[MAX_ELEM_PG_POPUP2];
gslc_tsXCheckbox                m_asXCheck1;
gslc_tsXCheckbox                m_asXCheck2;
gslc_tsXTogglebtn               m_asXToggle1;
gslc_tsXTogglebtn               m_asXToggle2;

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Element References for direct access
//<Extern_References !Start!>
extern gslc_tsElemRef* m_Btn_Item1;
extern gslc_tsElemRef* m_Btn_Item2;
extern gslc_tsElemRef* m_Elem_BTN1;
extern gslc_tsElemRef* m_pCB_Option1;
extern gslc_tsElemRef* m_pCB_Option2;
extern gslc_tsElemRef* m_pElemToggle1;
extern gslc_tsElemRef* m_pElemToggle2;
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
  gslc_PageAdd(&m_gui,E_PG_MENU1,m_asPage2Elem,MAX_ELEM_PG_MENU1_RAM,m_asPage2ElemRef,MAX_ELEM_PG_MENU1);
  gslc_PageAdd(&m_gui,E_PG_MENU2,m_asPage3Elem,MAX_ELEM_PG_MENU2_RAM,m_asPage3ElemRef,MAX_ELEM_PG_MENU2);
  gslc_PageAdd(&m_gui,E_PG_MENU3,m_asPage4Elem,MAX_ELEM_PG_MENU3_RAM,m_asPage4ElemRef,MAX_ELEM_PG_MENU3);
  gslc_PageAdd(&m_gui,E_PG_POPUP1,m_asPopup1Elem,MAX_ELEM_PG_POPUP1_RAM,m_asPopup1ElemRef,MAX_ELEM_PG_POPUP1);
  gslc_PageAdd(&m_gui,E_PG_POPUP2,m_asPopup2Elem,MAX_ELEM_PG_POPUP2_RAM,m_asPopup2ElemRef,MAX_ELEM_PG_POPUP2);

  // NOTE: The current page defaults to the first page added. Here we explicitly
  //       ensure that the main page is the correct page no matter the add order.
  gslc_SetPageCur(&m_gui,E_PG_MAIN);
  
  // Set Background to a flat color
  gslc_SetBkgndColor(&m_gui,GSLC_COL_BLACK);

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
  
  // Create E_ELEM_TEXT1 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT1,E_PG_MAIN,(gslc_tsRect){34,10,252,20},
    (char*)"Rotary Encoder Sample",0,E_BUILTIN10X16);
  
  // create E_ELEM_BTN1 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN1,E_PG_MAIN,
    (gslc_tsRect){120,72,80,30},(char*)"Menu 1",0,E_BUILTIN5X8,&CbBtnCommon);
  m_Elem_BTN1 = pElemRef;
  
  // create E_ELEM_BTN2 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN2,E_PG_MAIN,
    (gslc_tsRect){120,117,80,30},(char*)"Menu 2",0,E_BUILTIN5X8,&CbBtnCommon);
  
  // create E_ELEM_BTN6 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN6,E_PG_MAIN,
    (gslc_tsRect){120,162,80,30},(char*)"Menu 3",0,E_BUILTIN5X8,&CbBtnCommon);

  // -----------------------------------
  // PAGE: E_PG_MENU1
  
  
  // Create E_ELEM_TEXT2 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT2,E_PG_MENU1,(gslc_tsRect){110,86,48,10},
    (char*)"Option 1",0,E_BUILTIN5X8);
  
  // Create E_ELEM_TEXT3 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT3,E_PG_MENU1,(gslc_tsRect){110,122,48,10},
    (char*)"Option 2",0,E_BUILTIN5X8);
  
  // Create E_ELEM_TEXT4 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT4,E_PG_MENU1,(gslc_tsRect){124,10,72,16},
    (char*)"Menu 1",0,E_BUILTIN10X16);
   
  // create checkbox E_ELEM_CHECK1
  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_ELEM_CHECK1,E_PG_MENU1,&m_asXCheck1,
    (gslc_tsRect){180,79,20,20},false,GSLCX_CHECKBOX_STYLE_X,GSLC_COL_ORANGE,false);
  gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
  m_pCB_Option1 = pElemRef;
   
  // create checkbox E_ELEM_CHECK2
  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_ELEM_CHECK2,E_PG_MENU1,&m_asXCheck2,
    (gslc_tsRect){180,115,20,20},false,GSLCX_CHECKBOX_STYLE_X,GSLC_COL_ORANGE,false);
  gslc_ElemXCheckboxSetStateFunc(&m_gui, pElemRef, &CbCheckbox);
  m_pCB_Option2 = pElemRef;
  
  // create E_ELEM_BTN4 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN4,E_PG_MENU1,
    (gslc_tsRect){260,195,40,30},(char*)"Main",0,E_BUILTIN5X8,&CbBtnCommon);

  // -----------------------------------
  // PAGE: E_PG_MENU2
  
  
  // Create E_ELEM_TEXT5 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT5,E_PG_MENU2,(gslc_tsRect){124,15,72,16},
    (char*)"Menu 2",0,E_BUILTIN10X16);
  
  // Create E_ELEM_TEXT6 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT6,E_PG_MENU2,(gslc_tsRect){110,86,42,10},
    (char*)"Style 1",0,E_BUILTIN5X8);
  
  // Create E_ELEM_TEXT7 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT7,E_PG_MENU2,(gslc_tsRect){110,123,42,8},
    (char*)"Style 2",0,E_BUILTIN5X8);
  
  // Create toggle button E_ELEM_TOGGLE1
  pElemRef = gslc_ElemXTogglebtnCreate(&m_gui,E_ELEM_TOGGLE1,E_PG_MENU2,&m_asXToggle1,
    (gslc_tsRect){180,79,35,20},GSLC_COL_GRAY,GSLC_COL_BLUE_DK2,GSLC_COL_GRAY_LT3,
    true,false,&CbBtnCommon);
  m_pElemToggle1 = pElemRef;
  
  // Create toggle button E_ELEM_TOGGLE2
  pElemRef = gslc_ElemXTogglebtnCreate(&m_gui,E_ELEM_TOGGLE2,E_PG_MENU2,&m_asXToggle2,
    (gslc_tsRect){179,115,35,20},GSLC_COL_GRAY,GSLC_COL_BLUE_DK2,GSLC_COL_GRAY_LT3,
    true,false,&CbBtnCommon);
  m_pElemToggle2 = pElemRef;
  
  // create E_ELEM_BTN5 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN5,E_PG_MENU2,
    (gslc_tsRect){260,195,40,30},(char*)"Main",0,E_BUILTIN5X8,&CbBtnCommon);

  // -----------------------------------
  // PAGE: E_PG_MENU3
  
  
  // Create E_ELEM_TEXT10 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT10,E_PG_MENU3,(gslc_tsRect){114,5,72,16},
    (char*)"Menu 3",0,E_BUILTIN10X16);
  
  // create E_ELEM_BTN_ITEM1 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_ITEM1,E_PG_MENU3,
    (gslc_tsRect){120,82,80,30},(char*)"Item 1",0,E_BUILTIN5X8,&CbBtnCommon);
  m_Btn_Item1 = pElemRef;
  
  // create E_ELEM_BTN_ITEM2 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_ITEM2,E_PG_MENU3,
    (gslc_tsRect){120,127,80,30},(char*)"Item 2",0,E_BUILTIN5X8,&CbBtnCommon);
  m_Btn_Item2 = pElemRef;
  
  // create E_ELEM_BTN9 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN9,E_PG_MENU3,
    (gslc_tsRect){260,195,40,30},(char*)"Main",0,E_BUILTIN5X8,&CbBtnCommon);

  // -----------------------------------
  // PAGE: E_PG_POPUP1
  
   
  // Create E_ELEM_BOX1 box
  pElemRef = gslc_ElemCreateBox(&m_gui,E_ELEM_BOX1,E_PG_POPUP1,(gslc_tsRect){8,3,305,236});
  
  // Create E_ELEM_TEXT8 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT8,E_PG_POPUP1,(gslc_tsRect){115,86,90,16},
    (char*)"Popup 1",0,E_BUILTIN10X16);
  
  // create E_ELEM_BTN_BACK1 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_BACK1,E_PG_POPUP1,
    (gslc_tsRect){260,195,40,30},(char*)"Back",0,E_BUILTIN5X8,&CbBtnCommon);

  // -----------------------------------
  // PAGE: E_PG_POPUP2
  
   
  // Create E_ELEM_BOX2 box
  pElemRef = gslc_ElemCreateBox(&m_gui,E_ELEM_BOX2,E_PG_POPUP2,(gslc_tsRect){8,3,305,236});
  
  // Create E_ELEM_TEXT9 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT9,E_PG_POPUP2,(gslc_tsRect){115,86,84,16},
    (char*)"Popup 2",0,E_BUILTIN10X16);
  
  // create E_ELEM_BTN_BACK2 button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_BACK2,E_PG_POPUP2,
    (gslc_tsRect){260,195,40,30},(char*)"Back",0,E_BUILTIN5X8,&CbBtnCommon);
//<InitGUI !End!>

//<Startup !Start!>
//<Startup !End!>

}

#endif // end _GUISLICE_GEN_H
