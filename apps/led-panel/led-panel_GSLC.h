//<File !Start!>
// FILE: [led-panel_GSLC.h]
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
enum {E_PG_BASE,E_PG_MAIN};
enum {E_BTN_BLUE,E_BTN_GREEN,E_BTN_YELLOW,E_LED_PANEL,E_RED_BTN};
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
#define MAX_PAGE                2

#define MAX_ELEM_PG_BASE 1 // # Elems total on page
#define MAX_ELEM_PG_BASE_RAM MAX_ELEM_PG_BASE // # Elems in RAM
// Define the maximum number of elements per page
// - To enable the same code to run on devices that support storing
//   data into Flash (PROGMEM) and those that don't, we can make the
//   number of elements in Flash dependent upon GSLC_USE_PROGMEM
// - This should allow both Arduino and ARM Cortex to use the same code.

#define MAX_ELEM_PG_MAIN 4 // # Elems total on page
#if (GSLC_USE_PROGMEM)
  #define MAX_ELEM_PG_MAIN_PROG 4 // # Elems in Flash
#else
  #define MAX_ELEM_PG_MAIN_PROG 0 // No Elems in Flash
#endif
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN - MAX_ELEM_PG_MAIN_PROG 
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

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Element References for direct access
//<Extern_References !Start!>
extern gslc_tsElemRef* m_pLED;
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
  
   
  // Create E_LED_PANEL box
  pElemRef = gslc_ElemCreateBox(&m_gui,E_LED_PANEL,E_PG_BASE,(gslc_tsRect){10,210,80,30});
  // Set the callback function to handle all drawing for the element
  gslc_ElemSetDrawFunc(&m_gui,pElemRef,&CbDrawScanner);
  m_pLED = pElemRef;

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
  
  // create E_RED_BTN button with text label
  gslc_ElemCreateBtnTxt_P(&m_gui,E_RED_BTN,E_PG_MAIN,40,120,40,40,
    "RED",&m_asFont[E_BUILTIN5X8],
    GSLC_COL_BLACK,GSLC_COL_BLUE_DK2,GSLC_COL_RED,GSLC_COL_BLUE_DK2,
    GSLC_COL_GRAY,GSLC_ALIGN_MID_MID,true,true,&CbBtnCommon,NULL);
  // gslc_ElemSetFrameEn(); currently not supported by the FLASH _P calls.
  
  // create E_BTN_GREEN button with text label
  gslc_ElemCreateBtnTxt_P(&m_gui,E_BTN_GREEN,E_PG_MAIN,100,120,40,40,
    "GREEN",&m_asFont[E_BUILTIN5X8],
    GSLC_COL_BLACK,GSLC_COL_BLUE_DK2,GSLC_COL_GREEN,GSLC_COL_BLUE_DK2,
    GSLC_COL_GRAY,GSLC_ALIGN_MID_MID,true,true,&CbBtnCommon,NULL);
  // gslc_ElemSetFrameEn(); currently not supported by the FLASH _P calls.
  
  // create E_BTN_BLUE button with text label
  gslc_ElemCreateBtnTxt_P(&m_gui,E_BTN_BLUE,E_PG_MAIN,160,120,40,40,
    "BLUE",&m_asFont[E_BUILTIN5X8],
    GSLC_COL_BLACK,GSLC_COL_BLUE_DK2,GSLC_COL_BLUE,GSLC_COL_BLUE_DK2,
    GSLC_COL_GRAY,GSLC_ALIGN_MID_MID,true,true,&CbBtnCommon,NULL);
  // gslc_ElemSetFrameEn(); currently not supported by the FLASH _P calls.
  
  // create E_BTN_YELLOW button with text label
  gslc_ElemCreateBtnTxt_P(&m_gui,E_BTN_YELLOW,E_PG_MAIN,230,120,40,40,
    "YELLOW",&m_asFont[E_BUILTIN5X8],
    GSLC_COL_BLACK,GSLC_COL_BLUE_DK2,GSLC_COL_YELLOW,GSLC_COL_BLUE_DK2,
    GSLC_COL_GRAY,GSLC_ALIGN_MID_MID,true,true,&CbBtnCommon,NULL);
  // gslc_ElemSetFrameEn(); currently not supported by the FLASH _P calls.
//<InitGUI !End!>

//<Startup !Start!>
//<Startup !End!>

}

#endif // end _GUISLICE_GEN_H
