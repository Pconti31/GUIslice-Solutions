// =======================================================================
// Extended Element: BusyWait 
// - XBusyWait is meant to inform user that a task is currently executing, 
//   and that will take indeterminate length of time.
// ========================================================================
// - Paul Conti
// - https://www.impulseadventure.com/elec/guislice-gui.html
// - https://github.com/ImpulseAdventure/GUIslice
// =======================================================================
//
// The MIT License
//
// Copyright (c) 2024 Paul Conti
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =======================================================================
/// \file XBusyWait.c



// GUIslice library
#include "GUIslice.h"
#include "GUIslice_drv.h"

#include <Arduino.h>

//#include "elem/XBusyWait.h"
#include "XBusyWait.h"

#include <stdio.h>

#if (GSLC_USE_PROGMEM)
  #if defined(__AVR__)
    #include <avr/pgmspace.h>
  #else
    #include <pgmspace.h>
  #endif
#endif

// ----------------------------------------------------------------------------
// Error Messages
// ----------------------------------------------------------------------------

extern const char GSLC_PMEM ERRSTR_NULL[];
extern const char GSLC_PMEM ERRSTR_PXD_NULL[];

// ----------------------------------------------------------------------------
// Extended element definitions
// ----------------------------------------------------------------------------
//
// - This file extends the core GUIslice functionality with
//   additional widget types
//
// ----------------------------------------------------------------------------

// ============================================================================
// Extended Element: BusyWait 
// - XBusyWait is meant to inform user that a task is currently executing, 
//   and that will take indeterminate length of time.
// ============================================================================

bool gslc_ElemXBusyWaitTick(void* pvGui,void* pvScope)
{
  gslc_tsGui*     pGui     = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvScope);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef);

  // Fetch the element's extended data structure
  gslc_tsXBusyWait* pXData = (gslc_tsXBusyWait*)(pElem->pXData);
  if (pXData == NULL) {
    GSLC_DEBUG_PRINT("ERROR: ElemXBusyWaitDraw pXData is NULL\n","");
    return false;
  }

  // Manually mark the element as needing redraw
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_INC);

  return true;
}

///
/// Helper function to draw a gauge with style: GSLCX_BUSYWAIT_STYLE_SPINNING_DOTS
/// - Called from gslc_ElemXBusyWaitDraw()
///
/// \param[in]  pGui:        Ptr to GUI
/// \param[in]  pElem:       Ptr to Element
/// \param[in]  pXData:      Ptr to Extended Data
///
/// \return none
///
void gslc_ElemXBusyWaitHelpSpinningDots(gslc_tsGui* pGui, gslc_tsElem* pElem, gslc_tsXBusyWait* pXData) {

  if (pXData->nSegmentsPainted >= pXData->nSegments) {
    pXData->nAngle = 0;
    pXData->nSegmentsPainted = 0;
    // swap colors to blank out dots after doing a complete circle
    if ((pXData->currentColor.r == pXData->color.r) &&
        (pXData->currentColor.g == pXData->color.g) &&
        (pXData->currentColor.b == pXData->color.b)) {
      pXData->currentColor = pElem->colElemFill;
    } else {
      pXData->currentColor = pXData->color;
    }
  }

  int32_t s64, c64;
  
  // rotate our point through a tiny rotation matrix
  s64 = gslc_sinFX(pXData->nAngle*64);
  s64 = ((int32_t)s64*10000) / 32767;           
  c64 = gslc_cosFX(pXData->nAngle*64);
  c64 = ((int32_t)c64*10000) / 32767;

  int16_t nX = -(pXData->nCircleRadius * s64) / 10000 + pXData->centerX;
  int16_t nY =  (pXData->nCircleRadius * c64) / 10000 + pXData->centerY;
  
  gslc_DrvDrawFillCircle(pGui,nX,nY,(int) pXData->nSegmentRadius,pXData->currentColor);

//    GSLC_DEBUG_PRINT("pXData->nAngle[%d]=%d\n",pXData->nSegmentsPainted, pXData->nAngle);

  pXData->nAngle += pXData->inc;
  pXData->nSegmentsPainted++;

}

///
/// Helper function to draw a gauge with style: GSLCX_BUSYWAIT_STYLE__SPINNING_WHEEL
/// - Called from gslc_ElemXBusyWaitDraw()
///
/// \param[in]  pGui:        Ptr to GUI
/// \param[in]  pElem:       Ptr to Element
/// \param[in]  pXData:      Ptr to Extended Data
///
/// \return none
///
void gslc_ElemXBusyWaitHelpSpinningWheel(gslc_tsGui* pGui, gslc_tsElem* pElem, gslc_tsXBusyWait* pXData) {

  if (pXData->nSegmentsPainted >= pXData->nSegments) {
    pXData->nAngle = 0;
    pXData->nSegmentsPainted = 0;
    // swap colors to blank out dots after doing a complete circle
    if ((pXData->currentColor.r == pXData->color.r) &&
        (pXData->currentColor.g == pXData->color.g) &&
        (pXData->currentColor.b == pXData->color.b)) {
      pXData->currentColor = pElem->colElemFill;
    } else {
      pXData->currentColor = pXData->color;
    }
  }

  int16_t nDrawStart = 0;
  int16_t nDrawVal = pXData->nAngle+pXData->inc;

  // draw wheel
  GSLC_DEBUG_PRINT("RingDraw: start=%d end=%d\n", nDrawStart, nDrawVal);
  gslc_DrawFillSector(pGui, 72, pXData->centerX, pXData->centerY, 
    pXData->nCircleRadius-pXData->nSegmentRadius, pXData->nCircleRadius, 
    pXData->currentColor, nDrawStart, nDrawVal);

  GSLC_DEBUG_PRINT("pXData->nAngle[%d]=%d\n",pXData->nSegmentsPainted, pXData->nAngle);

  pXData->nAngle += pXData->inc;
  pXData->nSegmentsPainted++;
}

///
/// Helper function to draw a gauge with style: GSLCX_BUSYWAIT_STYLE_THREE_DOTS
/// - Called from gslc_ElemXBusyWaitDraw()
///
/// \param[in]  pGui:        Ptr to GUI
/// \param[in]  pElem:       Ptr to Element
/// \param[in]  pXData:      Ptr to Extended Data
///
/// \return none
///
void gslc_ElemXBusyWaitHelpThreeDots(gslc_tsGui* pGui, gslc_tsElem* pElem, gslc_tsXBusyWait* pXData) {
  if (pXData->nSegmentsPainted >= pXData->nSegments) {
    pXData->nAngle = 0;
    pXData->nSegmentsPainted = 0;
  }
  
  if (pXData->bFirstTime) {
      gslc_DrawFillCircle(pGui,pXData->centerX-40,pXData->centerY, pXData->nSegmentRadius,GSLC_COL_GRAY); // left
      gslc_DrawFillCircle(pGui,pXData->centerX,   pXData->centerY, pXData->nSegmentRadius,GSLC_COL_GRAY); // center
      gslc_DrawFillCircle(pGui,pXData->centerX+40,pXData->centerY, pXData->nSegmentRadius,GSLC_COL_GRAY); // right
    return;
  }
  switch(pXData->nSegmentsPainted) {
    case 0:
      gslc_DrawFillCircle(pGui,pXData->centerX+40,pXData->centerY, pXData->nCircleRadius,pElem->colElemFill);  // right
      gslc_DrawFillCircle(pGui,pXData->centerX+40,pXData->centerY, pXData->nSegmentRadius,GSLC_COL_GRAY); // right
      gslc_DrawFillCircle(pGui,pXData->centerX-40,pXData->centerY, pXData->nCircleRadius,pXData->color);       // left
      break;
    case 1:
      gslc_DrawFillCircle(pGui,pXData->centerX-40,pXData->centerY, pXData->nCircleRadius,pElem->colElemFill);  // left
      gslc_DrawFillCircle(pGui,pXData->centerX-40,pXData->centerY, pXData->nSegmentRadius,GSLC_COL_GRAY); // left
      gslc_DrawFillCircle(pGui,pXData->centerX,   pXData->centerY, pXData->nCircleRadius,pXData->color);       // center
      break;
    case 2:
      gslc_DrawFillCircle(pGui,pXData->centerX   ,pXData->centerY, pXData->nCircleRadius,pElem->colElemFill);  // center
      gslc_DrawFillCircle(pGui,pXData->centerX   ,pXData->centerY, pXData->nSegmentRadius,GSLC_COL_GRAY); // center
      gslc_DrawFillCircle(pGui,pXData->centerX+40,pXData->centerY, pXData->nCircleRadius,pXData->color);       // right
      break;
    default:
      break;
   }
  pXData->nSegmentsPainted++;
  if (pXData->nSegmentsPainted >= pXData->nSegments) {
    pXData->nSegmentsPainted = 0;
  }
}
// Create a XBusyWait element from an existing Box element
void redefineBoxAsBusyWait(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef, 
  gslc_teXBusyWaitStyle eStyle, gslc_tsColor nCol, BUSYWAITCB pfYield) {
 
  if (pGui == NULL) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXBusyWait";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }
  
  gslc_tsElem* pElem = gslc_GetElemFromRef(pGui,pElemRef);
  
  // create our extended data
  gslc_tsXBusyWait* pXData = (gslc_tsXBusyWait*)malloc(sizeof(gslc_tsXBusyWait));
  if (pXData == NULL) {
    GSLC_DEBUG_PRINT("ERROR: ElemXBusyWait malloc of pXData is NULL\n","");
    return;
  }

  pXData->centerX      = pGui->nDispW/2; // center busywait
  pXData->centerY      = pGui->nDispH/2; 
  
  switch (eStyle) {
    case GSLCX_BUSYWAIT_STYLE_SPINNING_DOTS:
      pXData->nSegmentsPainted = 0;
      pXData->nSegments        = 10;
      pXData->nSegmentRadius   = 5;
      pXData->nCircleRadius    = 44;
      pXData->inc              = 360 / pXData->nSegments; 
      pXData->nDelay           = BUSYWAIT_DELAY;
      break;
    case GSLCX_BUSYWAIT_STYLE__SPINNING_WHEEL:
      pXData->nSegmentsPainted = 0;
      pXData->nSegments        = 9;
      pXData->nCircleRadius    = 44;
      pXData->nSegmentRadius   = 10;
      pXData->inc              = 360 / pXData->nSegments; 
      pXData->nDelay           = BUSYWAIT_DELAY;
      break;
    case GSLCX_BUSYWAIT_STYLE_THREE_DOTS:
      pXData->nSegmentsPainted = 0;
      pXData->nSegments        = 3;
      pXData->nSegmentRadius   = 8;
      pXData->nCircleRadius    = 10;
      pXData->nDelay           = BUSYWAIT_DELAY*4;
      break;
    default:
      GSLC_DEBUG_PRINT("ERROR: ElemXBusyWaitDraw undefined\n","");
      pXData->eStyle = GSLCX_BUSYWAIT_STYLE_UNDEFINED;
      pXData->nDelay       = BUSYWAIT_DELAY*2;
      return;
  }
  pXData->eStyle = eStyle;
  
  pXData->color        = nCol;
  pXData->nAngle       = 0;
  pXData->nTime        = 0;
  pXData->bActive      = false;
  pXData->bClearScreen = true;
  pXData->pfuncYield   = pfYield;

  pElem->pXData        = (void*)(pXData);
  
  // Specify the custom drawing callback
  pElem->pfuncXDraw    = &gslc_ElemXBusyWaitDraw;
  
  // Specify the custom touch tracking callback
  pElem->pfuncXTouch   = NULL;

  // Specify the custom tick callback
  pElem->pfuncXTick    = &gslc_ElemXBusyWaitTick;

}

void gslc_ElemXBusyWaitPositionAnimation(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef,
  int16_t nX, int16_t nY, bool bClearScr) {
  if ((pGui == NULL) || (pElemRef == NULL)) {
    static const char GSLC_PMEM FUNCSTR[] = "gslc_ElemXBusyWaitPositionAnimation";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }
  // Typecast the parameters to match the GUI and element types
  gslc_tsElem*      pElem = gslc_GetElemFromRef(pGui,pElemRef);

  // Fetch the element's extended data structure
  gslc_tsXBusyWait* pXData = (gslc_tsXBusyWait*)(pElem->pXData);
  if (pXData == NULL) {
    GSLC_DEBUG_PRINT("ERROR: gslc_ElemXBusyWaitPositionAnimation pXData is NULL\n","");
    return;
  }
  
  pXData->centerX      = nX;
  pXData->centerY      = nY;
  pXData->bClearScreen = bClearScr;
  
}

// start our Animation
void gslc_ElemXBusyWaitStartAnimation(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef) {
  if ((pGui == NULL) || (pElemRef == NULL)) {
    static const char GSLC_PMEM FUNCSTR[] = "gslc_ElemXBusyWaitStartAnimation";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }
  // Typecast the parameters to match the GUI and element types
  gslc_tsElem*      pElem = gslc_GetElemFromRef(pGui,pElemRef);

  // Fetch the element's extended data structure
  gslc_tsXBusyWait* pXData = (gslc_tsXBusyWait*)(pElem->pXData);
  if (pXData == NULL) {
    GSLC_DEBUG_PRINT("ERROR: gslc_ElemXBusyWaitStartAnimation pXData is NULL\n","");
    return;
  }
  
  pXData->currentColor  = pXData->color;  // start with busywait color
  pXData->nSegmentsPainted = 0;
  pXData->nAngle        = 0;
  pXData->nTime         = millis()+pXData->nDelay; // for first time
  pXData->bFirstTime    = true;
  pXData->bActive       = true;

  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_FULL);
  
}

// stop our Animation
void gslc_ElemXBusyWaitStopAnimation(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef) {
  if ((pGui == NULL) || (pElemRef == NULL)) {
    static const char GSLC_PMEM FUNCSTR[] = "gslc_ElemXBusyWaitStopAnimation";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }
  // Typecast the parameters to match the GUI and element types
  gslc_tsElem*      pElem = gslc_GetElemFromRef(pGui,pElemRef);

  // Fetch the element's extended data structure
  gslc_tsXBusyWait* pXData = (gslc_tsXBusyWait*)(pElem->pXData);
  if (pXData == NULL) {
    GSLC_DEBUG_PRINT("ERROR: gslc_ElemXBusyWaitStopAnimation pXData is NULL\n","");
    return;
  }
  pXData->bActive = false;

  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_INC);
  
}

// Redraw the element
bool gslc_ElemXBusyWaitDraw(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw) {
  if ((pvGui == NULL) || (pvElemRef == NULL)) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXBusyWaitDraw";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return false;
  }
  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*       pGui  = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef*   pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*      pElem = gslc_GetElemFromRef(pGui,pElemRef);

  // Fetch the element's extended data structure
  gslc_tsXBusyWait* pXData = (gslc_tsXBusyWait*)(pElem->pXData);
  if (pXData == NULL) {
    GSLC_DEBUG_PRINT("ERROR: ElemXBusyWaitDraw pXData is NULL\n","");
    return false;
  }
  
//  GSLC_DEBUG_PRINT("ENTER gslc_ElemXBusyWaitDraw  pXData->nTime=%d\n",  pXData->nTime);
  
  if (!pXData->bActive) {
    gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_NONE);
    return true;
  }

  // Disable clipping region
  gslc_SetClipRect(pGui,NULL);

  if (pXData->bFirstTime && pXData->bClearScreen && eRedraw == GSLC_REDRAW_FULL) {
    gslc_tsRect rDisplay = { 0, 0, pGui->nDispW, pGui->nDispH };
    gslc_DrawFillRect(pGui,rDisplay,pElem->colElemFill);
    pXData->bClearScreen = false;
  }

  if ((millis() - pXData->nTime) >= pXData->nDelay) {
 
    switch (pXData->eStyle) {
      case GSLCX_BUSYWAIT_STYLE_SPINNING_DOTS:
        gslc_ElemXBusyWaitHelpSpinningDots(pGui, pElem, pXData);
        break;
      case GSLCX_BUSYWAIT_STYLE__SPINNING_WHEEL:
        gslc_ElemXBusyWaitHelpSpinningWheel(pGui, pElem, pXData);
        break;
      case GSLCX_BUSYWAIT_STYLE_THREE_DOTS:
        gslc_ElemXBusyWaitHelpThreeDots(pGui, pElem, pXData);
        break;
      default:
        gslc_ElemXBusyWaitHelpSpinningDots(pGui, pElem, pXData);
        gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_NONE);
        return true;
    }

    pXData->nTime = millis();
//    GSLC_DEBUG_PRINT("Ball %d drawn\n", pXData->nSegmentsPainted);
  } // end if millis()

  pXData->bFirstTime = false;

  if ((*pXData->pfuncYield)()) {
    gslc_ElemXBusyWaitStopAnimation(pGui, pElemRef);
    gslc_PopupHide(pGui);
  }

  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_NONE);
//  GSLC_DEBUG_PRINT("EXIT gslc_ElemXBusyWaitDraw  pXData->nTime=%d\n",  pXData->nTime);
  
  return true;

}

// ============================================================================
