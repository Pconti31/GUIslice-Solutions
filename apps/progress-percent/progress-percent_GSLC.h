//<File !Start!>
// FILE: [progress-percent_GSLC.h]
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
// Include extended elements
#include "elem/XProgress.h"
#include "elem/XTogglebtn.h"
//<Includes !End!>

// ------------------------------------------------
// Headers and Defines for fonts
// Note that font files are located within the Adafruit-GFX library folder:
// ------------------------------------------------
//<Fonts !Start!>
#if !defined(DRV_DISP_TFT_ESPI)
  #error E_PROJECT_OPTIONS tab->Graphics Library should be Adafruit_GFX
#endif
#include <TFT_eSPI.h>
#include "NotoMono8pt7b.h"
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
enum {E_PG_MAIN,E_PG2};
enum {E_BTN_BACK,E_BTN_NEXTPG,E_ELEM_PROGRESS2,E_ELEM_PROGRESS3
      ,E_ELEM_PROGRESS4,E_ELEM_TEXT2,E_ELEM_TEXT3,E_ELEM_TEXT4
      ,E_ELEM_TEXT5,E_LBL_TITLE,E_PROGRESS,E_TOGGLE_FLIP1
      ,E_TOGGLE_FLIP2};
// Must use separate enum for fonts with MAX_FONT at end to use gslc_FontSet.
enum {E_BUILTIN10X16,E_BUILTIN5X8,E_NOTOMONO8,MAX_FONT};
//<Enum !End!>

// ------------------------------------------------
// Instantiate the GUI
// ------------------------------------------------

// ------------------------------------------------
// Define the maximum number of elements and pages
// ------------------------------------------------
//<ElementDefines !Start!>
#define MAX_PAGE                2

#define MAX_ELEM_PG_MAIN 7 // # Elems total on page
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN // # Elems in RAM

#define MAX_ELEM_PG2 6 // # Elems total on page
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
gslc_tsElem                     m_asPage1Elem[MAX_ELEM_PG_MAIN_RAM];
gslc_tsElemRef                  m_asPage1ElemRef[MAX_ELEM_PG_MAIN];
gslc_tsElem                     m_asPage2Elem[MAX_ELEM_PG2_RAM];
gslc_tsElemRef                  m_asPage2ElemRef[MAX_ELEM_PG2];
gslc_tsXProgress                m_sXBarGauge1;
gslc_tsXProgress                m_sXBarGauge2;
gslc_tsXTogglebtn               m_asXToggle2;
gslc_tsXProgress                m_sXBarGauge3;
gslc_tsXProgress                m_sXBarGauge4;
gslc_tsXTogglebtn               m_asXToggle1;

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>


// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Element References for direct access
//<Extern_References !Start!>
extern gslc_tsElemRef* m_pElemProgress1;
extern gslc_tsElemRef* m_pElemProgress2;
extern gslc_tsElemRef* m_pElemProgress3;
extern gslc_tsElemRef* m_pElemProgress4;
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
    if (!gslc_FontSet(&m_gui,E_NOTOMONO8,GSLC_FONTREF_PTR,&NotoMono8pt7b,1)) { return; }
//<Load_Fonts !End!>

//<InitGUI !Start!>
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPage1Elem,MAX_ELEM_PG_MAIN_RAM,m_asPage1ElemRef,MAX_ELEM_PG_MAIN);
  gslc_PageAdd(&m_gui,E_PG2,m_asPage2Elem,MAX_ELEM_PG2_RAM,m_asPage2ElemRef,MAX_ELEM_PG2);

  // NOTE: The current page defaults to the first page added. Here we explicitly
  //       ensure that the main page is the correct page no matter the add order.
  gslc_SetPageCur(&m_gui,E_PG_MAIN);
  
  // Set Background to a flat color
  gslc_SetBkgndColor(&m_gui,((gslc_tsColor){28,47,84}));

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
  
  // Create E_LBL_TITLE text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_LBL_TITLE,E_PG_MAIN,(gslc_tsRect){51,5,217,18},
    (char*)"Test Progress Bars",0,E_BUILTIN10X16);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,((gslc_tsColor){28,47,84}),GSLC_COL_BLACK);

  // Create progress bar E_PROGRESS 
  pElemRef = gslc_ElemXProgressCreate(&m_gui,E_PROGRESS,E_PG_MAIN,&m_sXBarGauge1,
    (gslc_tsRect){20,85,200,30},0,100,0,GSLC_COL_GREEN_DK4,false);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,((gslc_tsColor){28,47,84}),((gslc_tsColor){28,47,84}));
  m_pElemProgress1 = pElemRef;
  
  // create E_BTN_NEXTPG button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_NEXTPG,E_PG_MAIN,
    (gslc_tsRect){260,190,50,40},(char*)"Next",0,E_BUILTIN5X8,&CbBtnCommon);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,GSLC_COL_BLUE_DK4,GSLC_COL_BLUE_DK1);

  // Create progress bar E_ELEM_PROGRESS2 
  pElemRef = gslc_ElemXProgressCreate(&m_gui,E_ELEM_PROGRESS2,E_PG_MAIN,&m_sXBarGauge2,
    (gslc_tsRect){260,30,50,150},0,100,0,GSLC_COL_RED_DK2,true);
  m_pElemProgress2 = pElemRef;
  
  // Create E_ELEM_TEXT2 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT2,E_PG_MAIN,(gslc_tsRect){20,55,103,10},
    (char*)"Show new Features",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,((gslc_tsColor){28,47,84}),GSLC_COL_BLACK);
  
  // Create toggle button E_TOGGLE_FLIP1
  pElemRef = gslc_ElemXTogglebtnCreate(&m_gui,E_TOGGLE_FLIP1,E_PG_MAIN,&m_asXToggle2,
    (gslc_tsRect){55,200,35,20},GSLC_COL_GRAY,GSLC_COL_BLUE_DK2,GSLC_COL_GRAY_LT3,
    true,false,&CbBtnCommon);
  m_pElemToggle1 = pElemRef;
  
  // Create E_ELEM_TEXT5 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT5,E_PG_MAIN,(gslc_tsRect){20,205,25,10},
    (char*)"Flip",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,((gslc_tsColor){28,47,84}),GSLC_COL_BLACK);

  // -----------------------------------
  // PAGE: E_PG2
  
  
  // create E_BTN_BACK button with text label
  pElemRef = gslc_ElemCreateBtnTxt(&m_gui,E_BTN_BACK,E_PG2,
    (gslc_tsRect){260,190,50,40},(char*)"Back",0,E_BUILTIN5X8,&CbBtnCommon);

  // Create progress bar E_ELEM_PROGRESS3 
  pElemRef = gslc_ElemXProgressCreate(&m_gui,E_ELEM_PROGRESS3,E_PG2,&m_sXBarGauge3,
    (gslc_tsRect){20,85,200,30},0,100,0,GSLC_COL_GREEN_DK4,false);
  m_pElemProgress3 = pElemRef;

  // Create progress bar E_ELEM_PROGRESS4 
  pElemRef = gslc_ElemXProgressCreate(&m_gui,E_ELEM_PROGRESS4,E_PG2,&m_sXBarGauge4,
    (gslc_tsRect){260,30,50,150},0,100,0,GSLC_COL_RED_DK2,true);
  m_pElemProgress4 = pElemRef;
  
  // Create E_ELEM_TEXT3 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT3,E_PG2,(gslc_tsRect){20,55,139,10},
    (char*)"Demostrate Old Behavior",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,((gslc_tsColor){28,47,84}),GSLC_COL_BLACK);
  
  // Create toggle button E_TOGGLE_FLIP2
  pElemRef = gslc_ElemXTogglebtnCreate(&m_gui,E_TOGGLE_FLIP2,E_PG2,&m_asXToggle1,
    (gslc_tsRect){55,200,35,20},GSLC_COL_GRAY,GSLC_COL_BLUE_DK2,GSLC_COL_GRAY_LT3,
    true,false,&CbBtnCommon);
  m_pElemToggle2 = pElemRef;
  
  // Create E_ELEM_TEXT4 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT4,E_PG2,(gslc_tsRect){20,205,25,10},
    (char*)"Flip",0,E_BUILTIN5X8);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_GRAY,((gslc_tsColor){28,47,84}),GSLC_COL_BLACK);
//<InitGUI !End!>

//<Startup !Start!>
//<Startup !End!>

}

#endif // end _GUISLICE_GEN_H
