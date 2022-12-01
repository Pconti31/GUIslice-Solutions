// =======================================================================
// GUIslice library (extensions)
// - Calvin Hass
// - https://www.impulseadventure.com/elec/guislice-gui.html
// - https://github.com/ImpulseAdventure/GUIslice
//
// Modified by Paul Conti to erase previous data before displaying new data.
// This results a dramatic reduction in screen flicker. // =======================================================================
//
// =======================================================================
// The MIT License
//
// Copyright 2016-2020 Calvin Hass
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
/// \file XGraph.c



// GUIslice library
#include "GUIslice.h"
#include "GUIslice_drv.h"

#include "elem/XGraph.h"

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

gslc_tsElemRef* gslc_ElemXGraphCreate(gslc_tsGui* pGui,int16_t nElemId,int16_t nPage,
  gslc_tsXGraph* pXData,gslc_tsRect rElem,int16_t nFontId,int16_t* pBuf,
  uint16_t nBufMax,gslc_tsColor colGraph)
{
  if ((pGui == NULL) || (pXData == NULL)) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXGraphCreate";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return NULL;
  }
  gslc_tsElem     sElem;
  gslc_tsElemRef* pElemRef = NULL;
  sElem = gslc_ElemCreate(pGui,nElemId,nPage,GSLC_TYPEX_GRAPH,rElem,NULL,0,nFontId);
  sElem.nFeatures        |= GSLC_ELEM_FEA_FRAME_EN;
  sElem.nFeatures        |= GSLC_ELEM_FEA_FILL_EN;
  sElem.nFeatures        &= ~GSLC_ELEM_FEA_CLICK_EN;
  sElem.nFeatures        &= ~GSLC_ELEM_FEA_GLOW_EN;

  // Default group assignment. Can override later with ElemSetGroup()
  sElem.nGroup            = GSLC_GROUP_ID_NONE;

  // Define other extended data
  pXData->pBuf            = pBuf;
  pXData->nMargin         = 5;

  pXData->nBufMax         = nBufMax;
  pXData->nBufHead        = 0;
  pXData->nBufTail        = 0;
  pXData->nBufCnt         = 0;
  pXData->nOverriddenVal  = 0; 
  pXData->bValidOverflow  = false;

  pXData->colGraph        = colGraph;
  pXData->eStyle          = GSLCX_GRAPH_STYLE_DOT;

  // Define the visible region of the window
  // - The range in value can be overridden by the user
  pXData->nWndHeight = rElem.h - (2*pXData->nMargin);
  pXData->nWndWidth  = rElem.w - (2*pXData->nMargin);

  pXData->nBufPhys = nBufMax;
  // - Each data point (buffer row) gets 1 pixel in X direction
  //   at most, can be reduce with settings
  if (nBufMax > pXData->nWndWidth) {
    pXData->nBufMax         = pXData->nWndWidth;
  } else {
    pXData->nBufMax         = nBufMax;
  }

  // Default scale is
  // - Data value is directly mapped to height in Y direction
  pXData->nPlotValMin   = 0;
  pXData->nPlotValMax   = pXData->nWndHeight;

  // Clear the buffer
  memset(pBuf,0,nBufMax*sizeof(int16_t));

  sElem.pXData            = (void*)(pXData);

  // Specify the custom drawing callback
  sElem.pfuncXDraw        = &gslc_ElemXGraphDraw;
  sElem.pfuncXTouch       = NULL;
  sElem.colElemFill       = GSLC_COL_BLACK;
  sElem.colElemFillGlow   = GSLC_COL_BLACK;
  sElem.colElemFrame      = GSLC_COL_GRAY;
  sElem.colElemFrameGlow  = GSLC_COL_WHITE;
  if (nPage != GSLC_PAGE_NONE) {
    pElemRef = gslc_ElemAdd(pGui,nPage,&sElem,GSLC_ELEMREF_DEFAULT);
    return pElemRef;
#if (GSLC_FEATURE_COMPOUND)
  } else {
    // Save as temporary element
    pGui->sElemTmp = sElem;
    pGui->sElemRefTmp.pElem = &(pGui->sElemTmp);
    pGui->sElemRefTmp.eElemFlags = GSLC_ELEMREF_DEFAULT | GSLC_ELEMREF_REDRAW_FULL;
    return &(pGui->sElemRefTmp);
#endif
  }
  return NULL;
}


void gslc_ElemXGraphSetStyle(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,
        gslc_teXGraphStyle eStyle,uint8_t nMargin)
{
  gslc_tsXGraph*  pBox;
  gslc_tsElem*    pElem = gslc_GetElemFromRef(pGui,pElemRef);
  pBox = (gslc_tsXGraph*)(pElem->pXData);

  pBox->eStyle  = eStyle;
  pBox->nMargin = nMargin;

  // TODO: Recalculate the window extents and defaults
  // - Note that ElemXGraphSetStyle() was not intended to be
  //   called after the control is already in use/displayed

  // Set the redraw flag
  // - Force full redraw
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_FULL);

}

// TODO: Support scaling in X direction
void gslc_ElemXGraphSetRange(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,
        int16_t nYMin,int16_t nYMax)
{
  gslc_tsXGraph*    pBox;
  gslc_tsElem*      pElem = gslc_GetElemFromRef(pGui,pElemRef);
  pBox = (gslc_tsXGraph*)(pElem->pXData);

  pBox->nPlotValMax = nYMax;
  pBox->nPlotValMin = nYMin;

  // Set the redraw flag
  // - As we are changing the scale, force a full redraw
  //   since incremental redraws may make assumption about
  //   prior coordinate scaling
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_FULL);

}

void gslc_ElemXGraphClear(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef)
{

  gslc_tsXGraph*  pBox;
  gslc_tsElem*    pElem = gslc_GetElemFromRef(pGui,pElemRef);
  pBox = (gslc_tsXGraph*)(pElem->pXData);

  pBox->nBufHead = 0;
  pBox->nBufTail = 0;
  pBox->nBufCnt  = 0;

  // Set the redraw flag
  // - As we are clearing the buffer, force a full redraw
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_FULL);

}

// Write a data value to the buffer
// - Advance the write ptr, wrap if needed
// - If encroach upon buffer read ptr, then drop the oldest line from the buffer
void gslc_ElemXGraphAdd(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,int16_t nVal)
{
  gslc_tsXGraph*  pBox = NULL;
  gslc_tsElem*    pElem = gslc_GetElemFromRef(pGui,pElemRef);
  pBox = (gslc_tsXGraph*)(pElem->pXData);

  int next;
  
/*
  The big pain point in circular buffers is that there is no clean way to 
  differentiate the buffer full vs empty cases. This is because in both 
  the cases, head is equal to tail. That is, initially head and tail 
  pointed point to the same location; when data is filled into the buffer, 
  head is incremented and eventually it wraps around and catches tail. 
  At this point, head will point to the same location as tail but now the 
  buffer is actually full, not empty. 
  
  Some implemenations keep a boolean flag to indicate a full buffer
  I chose to just keep a counter nBufCnt from 0 to nBufMax.
*/

  // If we have a full buffer before we add data we need to pop data off our buffer
  if (pBox->nBufCnt >= pBox->nBufMax) {
//    if (pBox->nBufHead != pBox->nBufTail) { // if the head == tail, we don't have any to data to pop
      // save the overflow value so we can erase it on incremental redraws
      pBox->nOverriddenVal = pBox->pBuf[pBox->nBufTail];
      pBox->bValidOverflow = true;      
      // next is where tail will point to after this read.
      next = pBox->nBufTail + 1;            
      if(next >= pBox->nBufMax)
          next = 0;
      pBox->nBufTail = next; // tail to next offset.
//    }
  } else {
    pBox->nBufCnt++;
  }
  
  // Now push our data on the buffer
  next = pBox->nBufHead + 1;  // next is where head will point to after this write.
  if (next >= pBox->nBufMax) 
      next = 0;

  pBox->pBuf[pBox->nBufHead] = nVal; // Load data and then move
  
//  GSLC_DEBUG_PRINT("Added pBox->pBuf[%d]=%d pBox->nBufTail=%d pBox->nBufCnt=%d pBox->nOverriddenVal=%d\n",
//  pBox->nBufHead,nVal,pBox->nBufTail,pBox->nBufCnt,pBox->nOverriddenVal);
  
  pBox->nBufHead = next;             // head to next data offset.

//  for (int i=0; i<pBox->nBufMax; i++) {
//    GSLC_DEBUG_PRINT("pBox->pBuf[%d]=%d\n",i,pBox->pBuf[i]);
//  }

  // Set the redraw flag
  // - Only need incremental redraw
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_INC);
}

bool gslc_ElemXGraphDraw(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw)
{
  if ((pvGui == NULL) || (pvElemRef == NULL)) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXGraphDraw";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return false;
  }
  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*     pGui      = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef  = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem     = gslc_GetElemFromRef(pGui,pElemRef);

  // Fetch the element's extended data structure
  gslc_tsXGraph* pBox;
  pBox = (gslc_tsXGraph*)(pElem->pXData);
  if (pBox == NULL) {
    GSLC_DEBUG2_PRINT("ERROR: ElemXGraphDraw(%s) pXData is NULL\n","");
    return false;
  }

  bool     bGlow     = (pElem->nFeatures & GSLC_ELEM_FEA_GLOW_EN) && gslc_ElemGetGlow(pGui,pElemRef);
  bool     bFrameEn  = (pElem->nFeatures & GSLC_ELEM_FEA_FRAME_EN);

  // Draw the frame
  if (eRedraw == GSLC_REDRAW_FULL) {
    if (bFrameEn) {
      gslc_DrawFrameRect(pGui,pElem->rElem,pElem->colElemFrame);
    }
    gslc_tsRect rInner = gslc_ExpandRect(pElem->rElem,-1,-1);
    gslc_DrawFillRect(pGui,rInner,(bGlow)?pElem->colElemFillGlow:pElem->colElemFill);
  }

  int16_t           nDataVal;
  uint16_t          nCurX = 0;
  uint16_t          nPixX,nPixY,nPixYOffset;
  gslc_tsColor      colGraph;

  uint16_t          nScrollMax;

  // Initialize color state
  colGraph  = pBox->colGraph;

  uint16_t nPixYBase   = pElem->rElem.y - pBox->nMargin + pElem->rElem.h-1;

  bool bEraseOld = false;
  if (pBox->bValidOverflow && pBox->nBufCnt >= pBox->nBufMax && eRedraw == GSLC_REDRAW_INC) 
    bEraseOld = true;

//  GSLC_DEBUG_PRINT("***ENTER gslc_ElemXGraphDraw pBox->nBufCnt=%d bEraseOld=%d\n",pBox->nBufCnt,(int)bEraseOld);

  if (bEraseOld) {
    // erase the overflow value first!
    nDataVal = pBox->nOverriddenVal;
    if      (nDataVal > pBox->nPlotValMax) { nDataVal = pBox->nPlotValMax; }
    else if (nDataVal < pBox->nPlotValMin) { nDataVal = pBox->nPlotValMin; }
    nPixX       = pElem->rElem.x + pBox->nMargin;
    nPixYOffset = (nDataVal >= 0)? nDataVal : 0;
    if (nPixYOffset > pBox->nWndHeight) { nPixYOffset = pBox->nWndHeight; }
    nPixY = nPixYBase - nPixYOffset;
    if (pBox->eStyle == GSLCX_GRAPH_STYLE_DOT) {
//      GSLC_DEBUG_PRINT("***Erase nDataVal=%d nPixX=%d nPixY=%d nPixYBase=%d nPixYOffset=%d\n",
//      nDataVal,nPixX,nPixY,nPixYBase,nPixYOffset);
      gslc_DrawSetPixel(pGui,nPixX,nPixY,pElem->colElemFill);
    } else if (pBox->eStyle == GSLCX_GRAPH_STYLE_LINE) {
    } else if (pBox->eStyle == GSLCX_GRAPH_STYLE_FILL) {
      gslc_DrawLine(pGui,nPixX,nPixYBase,nPixX,nPixY,pElem->colElemFill);
    }
  }

  uint16_t nDataInd = 0;
  if (pBox->nBufCnt >= pBox->nBufMax) {
    nDataInd = pBox->nBufHead;
  }
  if (pBox->nBufCnt == 0) {
    // Clear the redraw flag
    gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_NONE);

    // Mark page as needing flip
    gslc_PageFlipSet(pGui,true);
    return true;
  }
  for(int nPlotInd=0; nPlotInd<pBox->nWndWidth; nPlotInd++) {

    nDataVal = pBox->pBuf[nDataInd];
    //      if (nDataInd + 5 > pBox->nWndWidth)
    //GSLC_DEBUG_PRINT("Plot=%d pBox->pBuf[%d] nPlotInd=%d\n",nDataVal,nDataInd,nPlotInd);

    // Clip the value to the plot range
    if      (nDataVal > pBox->nPlotValMax) { nDataVal = pBox->nPlotValMax; }
    else if (nDataVal < pBox->nPlotValMin) { nDataVal = pBox->nPlotValMin; }

    // TODO: Make nCurX a scaled version of nPlotInd
    // - Support different modes for mapping multiple data points
    //   a single X coordinate and mapping a single data point
    //   to multiple X coordinates
    // - For now, just have a 1:1 correspondence between data
    //   points and the X coordinate
    nCurX = nPlotInd;

    // TODO: Scale data value

    // TODO: Consider supporting various color mapping modes
    //colGraph = gslc_ColorBlend2(GSLC_COL_BLACK,GSLC_COL_WHITE,500,nDataVal*500/200);

    // Determine the drawing coordinates
    nPixX       = pElem->rElem.x + pBox->nMargin + nCurX;

    // Calculate Y value
    nPixYOffset = (nDataVal >= 0)? nDataVal : 0;
    // Clip plot Y value
    if (nPixYOffset > pBox->nWndHeight) { nPixYOffset = pBox->nWndHeight; }

    // Calculate final Y coordinate
    nPixY       = nPixYBase - nPixYOffset;

    // Render the datapoints
    if (pBox->eStyle == GSLCX_GRAPH_STYLE_DOT) {
     if (bEraseOld && nPlotInd+1 < nPlotInd<pBox->nWndWidth) {
//      GSLC_DEBUG_PRINT("Erase nDataVal=%d pBox->pBuf[%d] nPixX+1=%d nPixY=%d nPixYBase=%d nPixYOffset=%d\n",
//      nDataVal,nDataInd,nPixX+1,nPixY,nPixYBase,nPixYOffset);
        gslc_DrawSetPixel(pGui,nPixX+1,nPixY,pElem->colElemFill);
     } 
//      GSLC_DEBUG_PRINT("Plot nDataVal=%d pBox->pBuf[%d] nPixX=%d nPixY=%d nPixYBase=%d nPixYOffset=%d\n",
//      nDataVal,nDataInd,nPixX,nPixY,nPixYBase,nPixYOffset);
      gslc_DrawSetPixel(pGui,nPixX,nPixY,colGraph);
    } else if (pBox->eStyle == GSLCX_GRAPH_STYLE_LINE) {
    } else if (pBox->eStyle == GSLCX_GRAPH_STYLE_FILL) {
      gslc_DrawLine(pGui,nPixX,nPixYBase,nPixX,nPixY,colGraph);
    }
    // advance our data index to next element
    nDataInd++;
    if (nDataInd >= pBox->nBufMax) {
      nDataInd = 0; // circle back to beginning of buffer
    } 
    // Test for done when we do not yet have a full buffer    
    if ((pBox->nBufCnt < pBox->nBufMax) && (nPlotInd+1 >= pBox->nBufCnt)) {
//      GSLC_DEBUG_PRINT(">>>(pBox->nBufCnt < pBox->nBufMax) && (nDataInd=%d >= pBox->nBufCnt=%d\n",nDataInd,pBox->nBufCnt);
      break;
    }
  } // end for (nPlotInd<pBox->nWndWidth)

//  GSLC_DEBUG_PRINT("=== EXIT gslc_ElemXGraphDraw\n","");

  // Clear the redraw flag
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_NONE);

  // Mark page as needing flip
  gslc_PageFlipSet(pGui,true);

  return true;
}

// ============================================================================
