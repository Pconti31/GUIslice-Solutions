//<File !Start!>
// FILE: [simple_tabs_GSLC.h]
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
#include "elem/XCheckbox.h"
#include "elem/XProgress.h"
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
enum {E_PG_BASE,E_PG_MAIN,E_PG_CONFIG};
enum {E_DRAW_LINE1,E_ELEM_BOX3,E_ELEM_BOX4,E_ELEM_BTN_QUIT
      ,E_ELEM_CHECK1,E_ELEM_CHECK2,E_ELEM_CHECK3,E_ELEM_PROGRESS1
      ,E_ELEM_TAB_CONFIG,E_ELEM_TAB_MAIN,E_ELEM_TXT_COUNT,E_LBL_COUNT
      ,E_LBL_DATA1,E_LBL_DATA2,E_LBL_PROGRESS,E_LBL_TITLE};
// Must use separate enum for fonts with MAX_FONT at end to use gslc_FontSet.
enum {E_BUILTIN5X8,MAX_FONT};
//<Enum !End!>

// ------------------------------------------------
// Instantiate the GUI
// ------------------------------------------------

// ------------------------------------------------
// Define the maximum number of elements and pages
// ------------------------------------------------
//<ElementDefines !Start!>
#define MAX_PAGE                3

#define MAX_ELEM_PG_BASE 7 // # Elems total on page
#define MAX_ELEM_PG_BASE_RAM MAX_ELEM_PG_BASE // # Elems in RAM

#define MAX_ELEM_PG_MAIN 4 // # Elems total on page
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN // # Elems in RAM

#define MAX_ELEM_PG_CONFIG 5 // # Elems total on page
#define MAX_ELEM_PG_CONFIG_RAM MAX_ELEM_PG_CONFIG // # Elems in RAM
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
gslc_tsElem                     m_asPage2Elem[MAX_ELEM_PG_CONFIG_RAM];
gslc_tsElemRef                  m_asPage2ElemRef[MAX_ELEM_PG_CONFIG];
gslc_tsXProgress                m_sXBarGauge1;
gslc_tsXCheckbox                m_asXCheck1;
gslc_tsXCheckbox                m_asXCheck2;
gslc_tsXCheckbox                m_asXCheck3;

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Element References for direct access
//<Extern_References !Start!>
extern gslc_tsElemRef* m_pElemCnt;
extern gslc_tsElemRef* m_pElemProgress1;
extern gslc_tsElemRef* m_pElemQuit;
extern gslc_tsElemRef* m_pElemTabConfig;
extern gslc_tsElemRef* m_pElemTabMain;
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
//<Load_Fonts !End!>

//<InitGUI !Start!>
  gslc_PageAdd(&m_gui,E_PG_BASE,m_asBasePage1Elem,MAX_ELEM_PG_BASE_RAM,m_asBasePage1ElemRef,MAX_ELEM_PG_BASE);
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPage1Elem,MAX_ELEM_PG_MAIN_RAM,m_asPage1ElemRef,MAX_ELEM_PG_MAIN);
  gslc_PageAdd(&m_gui,E_PG_CONFIG,m_asPage2Elem,MAX_ELEM_PG_CONFIG_RAM,m_asPage2ElemRef,MAX_ELEM_PG_CONFIG);

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
  
  
  // Create E_LBL_TITLE text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_TITLE,E_PG_BASE,(gslc_tsRect){10,10,80,12},
    (char*)"GUIslice Demo",0,E_BUILTIN5X8);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  
  // Create E_ELEM_BTN_QUIT button with modifiable text label
  static char m_strbtn1[7] = "Quit";
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_BTN_QUIT,E_PG_BASE,
    (gslc_tsRect){240,5,50,25},
    (char*)m_strbtn1,7,E_BUILTIN5X8,&CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_RED_DK2,GSLC_COL_RED_DK4,GSLC_COL_RED_DK1);
  gslc_ElemSetFrameEn(&m_gui,pElemRef,false);
  m_pElemQuit = pElemRef;
  
  // Create E_LBL_COUNT text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_COUNT,E_PG_BASE,(gslc_tsRect){110,10,38,12},
    (char*)"Count:",0,E_BUILTIN5X8);
  
  // Create E_ELEM_TXT_COUNT runtime modifiable text
  static char m_sDisplayText5[8] = "";
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TXT_COUNT,E_PG_BASE,(gslc_tsRect){170,10,44,12},
    (char*)m_sDisplayText5,8,E_BUILTIN5X8);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_PURPLE);
  m_pElemCnt = pElemRef;
  
  // create E_ELEM_TAB_MAIN button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_TAB_MAIN,E_PG_BASE,
    (gslc_tsRect){20,50,50,20},(char*)"Main",0,E_BUILTIN5X8,&CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLUE_DK4,GSLC_COL_BLUE_LT3);
  gslc_ElemSetRoundEn(&m_gui, pElemRef, true);
  m_pElemTabMain = pElemRef;
  
  // create E_ELEM_TAB_CONFIG button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_ELEM_TAB_CONFIG,E_PG_BASE,
    (gslc_tsRect){75,50,50,20},(char*)"Extra",0,E_BUILTIN5X8,&CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLUE_DK4,GSLC_COL_BLUE_LT3);
  gslc_ElemSetRoundEn(&m_gui, pElemRef, true);
  m_pElemTabConfig = pElemRef;

  // Create E_DRAW_LINE1 line 
  pElemRef = gslc_ElemCreateLine(&m_gui,E_DRAW_LINE1,E_PG_BASE,0,35,320,35);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLACK,GSLC_COL_GRAY_LT2,GSLC_COL_GRAY_LT2);

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
   
  // Create E_ELEM_BOX3 box
  pElemRef = gslc_ElemCreateBox(&m_gui,E_ELEM_BOX3,E_PG_MAIN,(gslc_tsRect){20,70,200,150});
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLACK,GSLC_COL_BLACK);
  
  // Create E_LBL_PROGRESS text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_PROGRESS,E_PG_MAIN,(gslc_tsRect){40,120,56,12},
    (char*)"Progress:",0,E_BUILTIN5X8);

  // Create progress bar E_ELEM_PROGRESS1 
  pElemRef = gslc_ElemXProgressCreate(&m_gui,E_ELEM_PROGRESS1,E_PG_MAIN,&m_sXBarGauge1,
    (gslc_tsRect){100,120,50,12},0,100,0,GSLC_COL_GREEN,false);
  m_pElemProgress1 = pElemRef;
   
  // create checkbox E_ELEM_CHECK1
  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_ELEM_CHECK1,E_PG_MAIN,&m_asXCheck1,
    (gslc_tsRect){100,160,30,30},false,GSLCX_CHECKBOX_STYLE_X,GSLC_COL_BLUE_LT2,false);

  // -----------------------------------
  // PAGE: E_PG_CONFIG
  
   
  // Create E_ELEM_BOX4 box
  pElemRef = gslc_ElemCreateBox(&m_gui,E_ELEM_BOX4,E_PG_CONFIG,(gslc_tsRect){20,70,200,150});
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLACK,GSLC_COL_BLACK);
   
  // create checkbox E_ELEM_CHECK2
  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_ELEM_CHECK2,E_PG_CONFIG,&m_asXCheck2,
    (gslc_tsRect){75,117,20,20},false,GSLCX_CHECKBOX_STYLE_X,GSLC_COL_RED_LT2,false);
  
  // Create E_LBL_DATA1 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_DATA1,E_PG_CONFIG,(gslc_tsRect){115,117,38,12},
    (char*)"Data 1",0,E_BUILTIN5X8);
   
  // create checkbox E_ELEM_CHECK3
  pElemRef = gslc_ElemXCheckboxCreate(&m_gui,E_ELEM_CHECK3,E_PG_CONFIG,&m_asXCheck3,
    (gslc_tsRect){75,147,20,20},false,GSLCX_CHECKBOX_STYLE_X,GSLC_COL_RED_LT2,false);
  
  // Create E_LBL_DATA2 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_DATA2,E_PG_CONFIG,(gslc_tsRect){115,147,38,12},
    (char*)"Data 2",0,E_BUILTIN5X8);
//<InitGUI !End!>

//<Startup !Start!>
//<Startup !End!>

}

#endif // end _GUISLICE_GEN_H
