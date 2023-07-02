// =======================================================================
// GUIslice library extension: Progress Bar that reports percent completed
// NOTES: This implementation assumes the TFT_eSPI driver is used so
//        that we have access to sprites. It's a modified version
//        of the original.
// - Author: Paul Conti
// - https://www.impulseadventure.com/elec/guislice-gui.html
// - https://github.com/ImpulseAdventure/GUIslice
// =======================================================================
//
// The MIT License
//
// Copyright 2016-2021 Calvin Hass
// Copyright 2022 Paul Conti
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
/// \file XProgress.c



// GUIslice library
#include "GUIslice.h"
#include "GUIslice_drv.h"

#include "elem/XProgress.h"

#include <SPI.h>
#if defined(DRV_DISP_TFT_ESPI)
#include <TFT_eSPI.h>       // Hardware-specific library
#else
#include <Adafruit_GFX.h>
#endif

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
// Local functions
// ----------------------------------------------------------------------------


bool gslc_ElemXProgressDrawHelpNew(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,gslc_tsXProgress* pGauge,gslc_teRedrawType eRedraw);
bool gslc_ElemXProgressDrawHelpOld(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,gslc_tsXProgress* pGauge,gslc_teRedrawType eRedraw);
uint16_t colorToRaw(gslc_tsColor nCol);


// ----------------------------------------------------------------------------
// Extended element definitions
// ----------------------------------------------------------------------------
//
// - This file extends the core GUIslice functionality with
//   additional widget types
//
// ----------------------------------------------------------------------------


// ============================================================================
// Extended Element: Progress Bar
// - Basic progress bar with support for vertical / horizontal orientation and
//   fill direction. Also provides an indicator of negative regions, depending
//   on the configured range.
// ============================================================================

// Create a gauge element and add it to the GUI element list
// - Defines default styling for the element
// - Defines callback for redraw but does not track touch/click
gslc_tsElemRef* gslc_ElemXProgressCreate(gslc_tsGui* pGui,int16_t nElemId,int16_t nPage,
  gslc_tsXProgress* pXData,gslc_tsRect rElem,
  int16_t nMin,int16_t nMax,int16_t nVal,gslc_tsColor colGauge,bool bVert)
{
  (void)nMin; // unused
  (void)nMax; // unused
  
  if ((pGui == NULL) || (pXData == NULL)) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXProgressCreate";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return NULL;
  }
  gslc_tsElem     sElem;
  gslc_tsElemRef* pElemRef = NULL;
  sElem = gslc_ElemCreate(pGui,nElemId,nPage,GSLC_TYPEX_PROGRESS,rElem,NULL,0,GSLC_FONT_NONE);
  sElem.nFeatures        |= GSLC_ELEM_FEA_FRAME_EN;
  sElem.nFeatures        |= GSLC_ELEM_FEA_FILL_EN;
  sElem.nFeatures        &= ~GSLC_ELEM_FEA_CLICK_EN;  // Element is not "clickable"
  sElem.nFeatures        &= ~GSLC_ELEM_FEA_GLOW_EN;
  sElem.nGroup            = GSLC_GROUP_ID_NONE;

  pXData->bVert           = bVert;
  pXData->bFlip           = false;
  pXData->colGauge        = colGauge;
  pXData->nFontId         = -1; // no font yet assigned
  pXData->nDigetW         = 0;
  pXData->nDigetH         = 0;
  pXData->nMin            = nMin;
  pXData->nMax            = nMax;
  pXData->nVal            = nVal;
#if defined(DRV_DISP_TFT_ESPI)
    // Initialize sprite support
    TFT_eSPI* pTft = (TFT_eSPI*)gslc_GetDriverDisp(pGui);
    TFT_eSprite* pSpr = new TFT_eSprite(pTft);
    pXData->pvSpr = (void*)pSpr;
    // Set the sprite colour depth to 16
    pSpr->setColorDepth(16);
#else
    Adafruit_GFX* pTft = (Adafruit_GFX*)gslc_GetDriverDisp(pGui);
    pXData->pvTft = (void*)pTft;
#endif
  
  sElem.pXData            = (void*)(pXData);
  sElem.pfuncXDraw        = &gslc_ElemXProgressDraw;
  sElem.pfuncXTouch       = NULL;           // No need to track touches
  sElem.colElemFill       = GSLC_COL_BLACK;
  sElem.colElemFillGlow   = GSLC_COL_BLACK;
  sElem.colElemFrame      = GSLC_COL_GRAY;
  sElem.colElemFrameGlow  = GSLC_COL_GRAY;
  sElem.colElemText       = GSLC_COL_WHITE;

  
  if (nPage != GSLC_PAGE_NONE) {
    pElemRef = gslc_ElemAdd(pGui,nPage,&sElem,(gslc_teElemRefFlags)GSLC_ELEMREF_DEFAULT);
    return pElemRef;
  }
  return NULL;
}

// Update the gauge control's current position
void gslc_ElemXProgressSetVal(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,int16_t nVal)
{
  if (pElemRef == NULL) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXProgressSetVal";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }
  gslc_tsXProgress*  pGauge  = (gslc_tsXProgress*)gslc_GetXDataFromRef(pGui,pElemRef,GSLC_TYPEX_PROGRESS,__LINE__);

  if (pGauge->nFontId >= 0) {
  /* When a user has set a font we use the new behavior
   * to show a percent text inside the bar graph.
   * Unlike the original progress bar this only takes
   * a percent from 0 to 100.
   */
    if (nVal < 0) 
      nVal = 0;
    else if (nVal > 100)
      nVal = 100;
  }
  
 // Update the data element
  int16_t nValOld = pGauge->nVal;
  pGauge->nVal = nVal;

  // Element needs redraw
  if (nVal != nValOld) {
    // We only need an incremental redraw
    // NOTE: If the user configures the indicator to be
    //       long enough that it overlaps some of the gauge indicators
    //       then a full redraw should be done instead.
    gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_INC);
  }

}

// Add a Font to Gauge element so it can display percent completed
void gslc_ElemXProgressSetFont(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,int8_t nFontId)
{
  if (pElemRef == NULL) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXProgressSetVal";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }
  gslc_tsXProgress* pGauge = (gslc_tsXProgress*)gslc_GetXDataFromRef(pGui,pElemRef,GSLC_TYPEX_PROGRESS,__LINE__);

  // make sure font is valid
  gslc_tsFont* pFont =  gslc_FontGet(pGui,nFontId);
  if (pFont == NULL) {
    GSLC_DEBUG_PRINT("gslc_ElemXProgressSetFont: gslc_FontGet=NULL\n","");
    return;
  }

  int16_t       nOffsetX, nOffsetY; // not used
  uint16_t      nTxtW, nTxtH; 
  char acText[2] = "0"; 
  gslc_DrvGetTxtSize(pGui, pFont, (char*)&acText, GSLC_TXT_DEFAULT, &nOffsetX, &nOffsetY, &nTxtW, &nTxtH);
  pGauge->nDigetW = (int16_t)nTxtW;
  pGauge->nDigetH = (int16_t)nTxtH;
//  GSLC_DEBUG_PRINT("getCharSize: %c nDigetW=%d nDigetH=%d\n",acText,nTxtW,nTxtH);

  // Add font
  pGauge->nFontId = nFontId;
}


// Update the gauge's fill direction
// - Setting bFlip causes the gauge to be filled in the reverse direction
//   to the default
// - Default fill direction for horizontal gauges: left-to-right
// - Default fill direction for vertical gauges: bottom-to-top
void gslc_ElemXProgressSetFlip(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,bool bFlip)
{
  if (pElemRef == NULL) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXProgressSetFlip";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }

  // Fetch the element's extended data structure
  gslc_tsXProgress*  pGauge  = (gslc_tsXProgress*)gslc_GetXDataFromRef(pGui,pElemRef,GSLC_TYPEX_PROGRESS,__LINE__);

  pGauge->bFlip = bFlip;

  // Mark for redraw
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_FULL);

}

// Update the gauge's color
void gslc_ElemXProgressSetGaugeCol(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,gslc_tsColor colGauge)
{
  if (pElemRef == NULL) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXProgressSetGaugeCol";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }

  // Fetch the element's extended data structure
  gslc_tsXProgress*  pGauge  = (gslc_tsXProgress*)gslc_GetXDataFromRef(pGui,pElemRef,GSLC_TYPEX_PROGRESS,__LINE__);

  pGauge->colGauge = colGauge;

  // Mark for redraw
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_INC);

}



// Redraw the gauge
// - Note that this redraw is for the entire element rect region
// - The Draw function parameters use void pointers to allow for
//   simpler callback function definition & scalability.
bool gslc_ElemXProgressDraw(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw)
{
  if ((pvGui == NULL) || (pvElemRef == NULL)) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXProgressDraw";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return false;
  }

  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*       pGui      = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef*   pElemRef  = (gslc_tsElemRef*)(pvElemRef);

  // Fetch the element's extended data structure
  gslc_tsXProgress*  pGauge  = (gslc_tsXProgress*)gslc_GetXDataFromRef(pGui,pElemRef,GSLC_TYPEX_PROGRESS,__LINE__);

  // does our caller want new or old behavior?
#if defined(DRV_DISP_TFT_ESPI)
  if (pGauge->nFontId >= 0) {
    gslc_ElemXProgressDrawHelpNew(pGui,pElemRef,pGauge,eRedraw);
  } else {
    gslc_ElemXProgressDrawHelpOld(pGui,pElemRef,pGauge,eRedraw);
  }
#else
  if (pGauge->nFontId >= 0) {
    gslc_ElemXProgressDrawHelpNew(pGui,pElemRef,pGauge,eRedraw);
  } else {
    gslc_ElemXProgressDrawHelpOld(pGui,pElemRef,pGauge,eRedraw);
  }
#endif
  // Save as "last state" to support incremental erase/redraw
  pGauge->nValLast      = pGauge->nVal;
  pGauge->bValLastValid = true;

  // Clear the redraw flag
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_NONE);

  return true;
}

bool gslc_ElemXProgressDrawHelpNew(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,
  gslc_tsXProgress* pGauge,gslc_teRedrawType eRedraw)
{
  gslc_tsElem*  pElem   = gslc_GetElemFromRef(pGui,pElemRef);

  gslc_tsRect   rGauge;              // Filled portion of gauge
  gslc_tsRect   rElem = pElem->rElem;// our bounding box

  uint16_t      nBarMaxW,nBarMaxH;
  uint16_t      nBarMaxX0,nBarMaxY0,nBarMaxX1,nBarMaxY1;
  int16_t       nGaugeX0,nGaugeY0,nGaugeW,nGaugeH;
 
  bool    bVert    = pGauge->bVert;
  bool    bFlip    = pGauge->bFlip;

  // Create a sprite of defined size
  gslc_tsRect rInner = gslc_ExpandRect(rElem,-2,-2); 

  /* unlike the original progress bar this only takes
   * a percent from 0 to 100.
   */
   int16_t nPercent = pGauge->nVal;
  if (nPercent < 0) 
    nPercent = 0;
  else if (nPercent > 100)
    nPercent = 100;
  
  // determine sizeof our textbox
  uint16_t nTextboxHeight = pGauge->nDigetH;
  uint16_t nTextboxWidth  = pGauge->nDigetW;

  if (nPercent < 10)
    nTextboxWidth = nTextboxWidth*2;
  else if (nPercent < 100)
    nTextboxWidth = nTextboxWidth*3;
  else 
    nTextboxWidth = nTextboxWidth*4;
//  GSLC_DEBUG_PRINT("Textbox: w=%d h=%d\n",nTextboxWidth,nTextboxHeight);

  // set the gauge's starting x and y position
  nBarMaxX0 = 0;
  nBarMaxY0 = 0;

  /* calculate our bar's max height and width
   * leaving room for our text box.
   */
  if (bVert) {
    nBarMaxW  = rInner.w;
    nBarMaxH  = rInner.h;
    nBarMaxX1 = nBarMaxW;
    nBarMaxY1 = nBarMaxH - 1;
  } else {
    nBarMaxW  = rInner.w;
    nBarMaxH  = rInner.h;
    nBarMaxX1 = nBarMaxW-1;
    nBarMaxY1 = nBarMaxH;
  }
//  GSLC_DEBUG_PRINT("rInner x=%d y=%d w=%d h=%d\n",rInner.x,rInner.y,rInner.w,rInner.h);
//  GSLC_DEBUG_PRINT("nBarMaxX0=%d nBarMaxY0=%d\n",nBarMaxX0,nBarMaxY0);
//  GSLC_DEBUG_PRINT("nBarMaxX1=%d nBarMaxY1=%d\n",nBarMaxX1,nBarMaxY1);
//  GSLC_DEBUG_PRINT("nBarMaxW=%d nBarMaxH=%d\n",nBarMaxW,nBarMaxH);

  // Define the gauge's fill rectangle region and text position
  // Support flipping of gauge directionality
  // - The bFlip flag reverses the fill direction
  // - Vertical gauges are flipped by default
  gslc_tsRect   rText;
  int16_t nLen = 0;
//  GSLC_DEBUG_PRINT("VERT=%d FLIP=%d\n",bVert,bFlip);
  if (bVert) {
    // Calculate the length of the bar
    if (nPercent > 0)
      nLen = ((nPercent * nBarMaxH) + 50 /* round up */) / 100;
    nGaugeX0 = 0;
    nGaugeY0 = rInner.h - nLen;;
    nGaugeW = nBarMaxX1;
    nGaugeH = nLen;
    rText.x  = (rInner.w/2) - (nTextboxWidth/2); // center text
    rText.y  = min((int)nGaugeY0+nTextboxHeight+1,(int)rInner.h-5);
    rText.w  = nTextboxWidth;
    rText.h  = nTextboxHeight;
    if (bFlip) {
      nGaugeY0 = 0;
//      rText.x  = (rInner.w/2) - (nTextboxWidth/2); // center text
      rText.y  = max((nGaugeH-5),(int)nTextboxHeight+5);
    }
  } else {
    if (nPercent > 0)
      nLen = ((nPercent * nBarMaxW) + 50 /* round up */) / 100;
    nGaugeX0 = 0;
    nGaugeY0 = 0;
    nGaugeW = nLen;
    nGaugeH = nBarMaxY1;
    rText.x  = max((nGaugeW-nTextboxWidth-5),5);
    rText.y  = (rInner.h/2) + (nTextboxHeight/2);
    rText.w  = nTextboxWidth;
    rText.h  = nTextboxHeight;
    if (bFlip) {
      nGaugeX0 = rInner.w - nLen;
      rText.x  = min((rInner.w-nTextboxWidth-5),rInner.w-nLen);
    }
  }

//  GSLC_DEBUG_PRINT("Gauge: x=%d y=%d w=%d h=%d\n",nGaugeX0,nGaugeY0,nGaugeW,nGaugeH);
//  GSLC_DEBUG_PRINT("rText x=%d y=%d w=%d h=%d\n",rText.x,rText.y,rText.w,rText.h);
  
#if defined(DRV_DISP_TFT_ESPI)
  // access our sprite
  TFT_eSprite* pSpr = (TFT_eSprite*)pGauge->pvSpr;

  pSpr->createSprite(rInner.w, rInner.h);
  
  pSpr->fillSprite(colorToRaw(pElem->colElemFill));

  // Draw the gauge fill region
  if (nPercent > 0) {
    if (pElem->nFeatures & GSLC_ELEM_FEA_ROUND_EN) {
      pSpr->fillRoundRect(nGaugeX0,nGaugeY0,nGaugeW,nGaugeH,
        (int32_t)pGui->nRoundRadius,colorToRaw(pGauge->colGauge));
    } else {
      pSpr->fillRect(nGaugeX0,nGaugeY0,nGaugeW,nGaugeH,colorToRaw(pGauge->colGauge));
    }
  }

  // Draw our Text - Best to use mono font
  gslc_tsFont* pFont =  gslc_FontGet(pGui,pGauge->nFontId);
  if (pFont == NULL) {
    pSpr->setTextFont(pFont->nSize);
  } else {
    pSpr->setFreeFont((const GFXfont *)pFont->pvFont);
  }
  
  // Set the font colour and the background colour
  pSpr->setTextColor(colorToRaw(pElem->colElemText), colorToRaw(pGauge->colGauge));
  char acPercent[6];
  sprintf(acPercent,"%d%s",nPercent,"%");
  pSpr->setCursor(rText.x,rText.y);
//  GSLC_DEBUG_PRINT("setCursor acPercent=%s x=%d y=%d\n",acPercent,rText.x,rText.y);

  // Set text coordinate datum to middle centre
  //pSpr->setTextDatum(MC_DATUM);
  // write percent to our sprite
  pSpr->print(acPercent);

  // flush our sprite to the display
  pSpr->pushSprite(rInner.x, rInner.y);

  pSpr->unloadFont(); // Remove the font to recover memory used
  
  pSpr->deleteSprite(); // Recover memory

#else
  Adafruit_GFX* pTft = (Adafruit_GFX*)pGauge->pvTft;
 
  GFXcanvas16 *canvas = new GFXcanvas16 (rInner.w, rInner.h);
  canvas->fillRect (0, 0, rInner.w, rInner.h, colorToRaw(pElem->colElemFill));

  // Draw the gauge fill region
  if (nPercent > 0) {
    if (pElem->nFeatures & GSLC_ELEM_FEA_ROUND_EN) {
      canvas->fillRoundRect(nGaugeX0,nGaugeY0,nGaugeW,nGaugeH,
        (int32_t)pGui->nRoundRadius,colorToRaw(pGauge->colGauge));
    } else {
      canvas->fillRect(nGaugeX0,nGaugeY0,nGaugeW,nGaugeH,colorToRaw(pGauge->colGauge));
    }
  }

  // Draw our Text - Best to use mono font
  gslc_tsFont* pFont =  gslc_FontGet(pGui,pGauge->nFontId);
  if (pFont == NULL) {
    canvas->setTextSize(pFont->nSize);
  } else {
    canvas->setFont((const GFXfont *)pFont->pvFont);
  }
  
  // Set the font colour and the background colour
  canvas->setTextColor(colorToRaw(pElem->colElemText), colorToRaw(pGauge->colGauge));
  char acPercent[6];
  sprintf(acPercent,"%d%s",nPercent,"%");
  canvas->setCursor(rText.x,rText.y);
//  GSLC_DEBUG_PRINT("setCursor acPercent=%s x=%d y=%d\n",acPercent,rText.x,rText.y);

  // write percent to our sprite
  canvas->print(acPercent);

  // flush our canvas to the display
  pTft->drawRGBBitmap (rInner.x, rInner.y, canvas -> getBuffer (), rInner.w, rInner.h);

  delete canvas; // Recover memory

 #endif


  // Draw a frame around the gauge
  // - Only draw this during full redraw
  if (eRedraw == GSLC_REDRAW_FULL && (pElem->nFeatures & GSLC_ELEM_FEA_FRAME_EN)) {
    if (pElem->nFeatures & GSLC_ELEM_FEA_ROUND_EN) {
      gslc_DrawFrameRoundRect(pGui, rElem, pGui->nRoundRadius, pElem->colElemFrame);
    } else {
      gslc_DrawFrameRect(pGui, rElem, pElem->colElemFrame);
    }
  }

  return true;
}

bool gslc_ElemXProgressDrawHelpOld(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,
  gslc_tsXProgress* pGauge,gslc_teRedrawType eRedraw)
{
  gslc_tsElem*       pElem   = gslc_GetElemFromRef(pGui,pElemRef);

  gslc_tsRect   rTmp;           // Temporary rect for drawing
  gslc_tsRect   rGauge;         // Filled portion of gauge
  gslc_tsRect   rEmpty;         // Empty portion of gauge
  uint16_t      nElemW,nElemH;
  int16_t       nElemX0,nElemY0,nElemX1,nElemY1;
  int16_t       nGaugeX0,nGaugeY0,nGaugeX1,nGaugeY1;

  nElemX0 = pElem->rElem.x;
  nElemY0 = pElem->rElem.y;
  nElemX1 = pElem->rElem.x + pElem->rElem.w - 1;
  nElemY1 = pElem->rElem.y + pElem->rElem.h - 1;
  nElemW  = pElem->rElem.w;
  nElemH  = pElem->rElem.h;

  bool    bVert = pGauge->bVert;
  bool    bFlip = pGauge->bFlip;
  int16_t nMax  = pGauge->nMax;
  int16_t nMin  = pGauge->nMin;
  int16_t nRng  = pGauge->nMax - pGauge->nMin;

  uint32_t nScl = 1;
  int16_t nGaugeMid = 0;
  int16_t nLen = 0;
  int16_t nTmp = 0;
  int32_t nTmpL = 0;

  if (nRng == 0) {
    GSLC_DEBUG2_PRINT("ERROR: ElemXProgressDraw() Zero gauge range [%d,%d]\n",nMin,nMax);
    return false;
  }

  if (bVert) {
    nScl = nElemH*32768/nRng;
  } else {
    nScl = nElemW*32768/nRng;
  }

  // Calculate the control midpoint/zeropoint (for display purposes)
  nTmpL = -((int32_t)nMin * (int32_t)nScl / 32768);
  nGaugeMid = (int16_t)nTmpL;


  // Calculate the length of the bar
  // - Use long mult/divide to avoid need for floating point
  nTmpL = (int32_t)(pGauge->nVal) * (int32_t)(nScl) / 32768;
  nLen  = (int16_t)(nTmpL);

  // Define the gauge's fill rectangle region
  // depending on the orientation (bVert) and whether
  // the current position is negative or positive.
  if (nLen >= 0) {
    if (bVert) {
      nGaugeY0 = nElemY0 + nGaugeMid;
      nGaugeY1 = nElemY0 + nGaugeMid + nLen;
    } else {
      nGaugeX0 = nElemX0 + nGaugeMid;
      nGaugeX1 = nElemX0 + nGaugeMid + nLen;
    }
  } else {
    if (bVert) {
      nGaugeY0 = nElemY0 + nGaugeMid + nLen;
      nGaugeY1 = nElemY0 + nGaugeMid;
    } else {
      nGaugeX0 = nElemX0 + nGaugeMid + nLen;
      nGaugeX1 = nElemX0 + nGaugeMid;
    }
  }
  if (bVert) {
    nGaugeX0 = nElemX0;
    nGaugeX1 = nElemX1;
  } else {
    nGaugeY0 = nElemY0;
    nGaugeY1 = nElemY1;
  }


  // Clip the region
  nGaugeX0 = (nGaugeX0 < nElemX0)? nElemX0 : nGaugeX0;
  nGaugeY0 = (nGaugeY0 < nElemY0)? nElemY0 : nGaugeY0;
  nGaugeX1 = (nGaugeX1 > nElemX1)? nElemX1 : nGaugeX1;
  nGaugeY1 = (nGaugeY1 > nElemY1)? nElemY1 : nGaugeY1;

  // Support flipping of gauge directionality
  // - The bFlip flag reverses the fill direction
  // - Vertical gauges are flipped by default

  if (bVert && !bFlip) {
    nTmp      = nElemY0+(nElemY1-nGaugeY1);  // nTmp will be swapped into nGaugeY0
    nGaugeY1  = nElemY1-(nGaugeY0-nElemY0);
    nGaugeY0  = nTmp;
    nGaugeMid = nElemH-nGaugeMid-1;
  } else if (!bVert && bFlip) {
    nTmp      = nElemX0+(nElemX1-nGaugeX1);  // nTmp will be swapped into nGaugeX0
    nGaugeX1  = nElemX1-(nGaugeX0-nElemX0);
    nGaugeX0  = nTmp;
    nGaugeMid = nElemW-nGaugeMid-1;
  }

//  GSLC_DEBUG_PRINT("** VERT=%d FLIP=%d\n",bVert,bFlip);
//  GSLC_DEBUG_PRINT("Gauge: x0=%d y0=%d x1=%d y1=%d\n",nGaugeX0,nGaugeY0,nGaugeX1,nGaugeY1);

  #ifdef DBG_LOG
  //printf("Gauge: nMin=%4d nMax=%4d nRng=%d nVal=%4d fScl=%6.3f nGaugeMid=%4d RectX=%4d RectW=%4d\n",
  //  nMin,nMax,nRng,pGauge->nGaugeVal,fScl,nGaugeMid,rGauge.x,rGauge.w);
  #endif

  // Draw a frame around the gauge
  // - Only draw this during full redraw
  if (eRedraw == GSLC_REDRAW_FULL) {
    gslc_DrawFrameRect(pGui, pElem->rElem, pElem->colElemFrame);
  }

  // To avoid flicker, we only erase the portion of the gauge
  // that isn't "filled". Determine the gauge empty region and erase it
  // There are two empty regions (one in negative and one in positive)
  int16_t nEmptyPos;
  if (bVert) {
    // Empty Region #1 (negative)
    nEmptyPos = (nGaugeY0 > nElemY1) ? nElemY1 : nGaugeY0;
    rEmpty = (gslc_tsRect){nElemX0,
                           nElemY0,
                           (uint16_t)(nElemX1-nElemX0+1),
                           (uint16_t)(nEmptyPos-nElemY0+1)};
    rTmp = gslc_ExpandRect(rEmpty,-1,-1);
    gslc_DrawFillRect(pGui,rTmp,pElem->colElemFill);
    // Empty Region #2 (positive)
    nEmptyPos = (nGaugeY1 < nElemY0) ? nElemY0 : nGaugeY1;
    rEmpty = (gslc_tsRect){nElemX0,
                           nEmptyPos,
                           (uint16_t)(nElemX1-nElemX0+1),
                           (uint16_t)(nElemY1-nEmptyPos+1)};
    rTmp = gslc_ExpandRect(rEmpty,-1,-1);
    gslc_DrawFillRect(pGui,rTmp,pElem->colElemFill);
  } else {
    // Empty Region #1 (negative)
    nEmptyPos = (nGaugeX0 > nElemX1) ? nElemX1 : nGaugeX0;
    rEmpty = (gslc_tsRect){nElemX0,
                           nElemY0,
                           (uint16_t)(nEmptyPos-nElemX0+1),
                           (uint16_t)(nElemY1-nElemY0+1)};
    rTmp = gslc_ExpandRect(rEmpty,-1,-1);
    gslc_DrawFillRect(pGui, rTmp, pElem->colElemFill);
    // Empty Region #2 (positive)
    nEmptyPos = (nGaugeX1 < nElemX0) ? nElemX0 : nGaugeX1;
    rEmpty = (gslc_tsRect){nEmptyPos,
                           nElemY0,
                           (uint16_t)(nElemX1-nEmptyPos+1),
                           (uint16_t)(nElemY1-nElemY0+1)};
    rTmp = gslc_ExpandRect(rEmpty,-1,-1);
    gslc_DrawFillRect(pGui, rTmp, pElem->colElemFill);
  }

  // Draw the gauge fill region
  rGauge = (gslc_tsRect){nGaugeX0,
                         nGaugeY0,
                         (uint16_t)(nGaugeX1-nGaugeX0+1),
                         (uint16_t)(nGaugeY1-nGaugeY0+1)};
//  GSLC_DEBUG_PRINT("rGauge: x0=%d y0=%d x1=%d y1=%d\n",rGauge.x,rGauge.y,rGauge.w,rGauge.h);
  rTmp = gslc_ExpandRect(rGauge,-1,-1);
  gslc_DrawFillRect(pGui,rTmp,pGauge->colGauge);


  // Draw the midpoint line
  if (bVert) {
    if (nElemY0 + nGaugeMid < nElemY1) {
      gslc_DrawLine(pGui, nElemX0, nElemY0 + nGaugeMid, nElemX1, nElemY0 + nGaugeMid, pElem->colElemFrame);
    }
  } else {
    if (nElemX0 + nGaugeMid < nElemX1) {
      gslc_DrawLine(pGui, nElemX0 + nGaugeMid, nElemY0, nElemX0 + nGaugeMid, nElemY1, pElem->colElemFrame);
    }
  }

  return true;
}

// Convert from RGB struct to native screen format
// TODO: Use 32bit return type?
uint16_t colorToRaw(gslc_tsColor nCol)
{
  uint16_t nColRaw = 0;

  #if defined(DRV_COLORMODE_MONO)
    // Monochrome
    if ((nCol.r == 0) && (nCol.g == 0) && (nCol.b == 0)) { // GSLC_COL_BLACK
      nColRaw = 0;  // BLACK
    } else {
      nColRaw = 1;  // WHITE
    }

  #elif defined(DRV_COLORMODE_BGR565)
    nColRaw |= (((nCol.b & 0xF8) >> 3) << 11); // Mask: 1111 1000 0000 0000
    nColRaw |= (((nCol.g & 0xFC) >> 2) <<  5); // Mask: 0000 0111 1110 0000
    nColRaw |= (((nCol.r & 0xF8) >> 3) <<  0); // Mask: 0000 0000 0001 1111

  #else
    // Default to DRV_COLORMODE_RGB565
    nColRaw |= (((nCol.r & 0xF8) >> 3) << 11); // Mask: 1111 1000 0000 0000
    nColRaw |= (((nCol.g & 0xFC) >> 2) <<  5); // Mask: 0000 0111 1110 0000
    nColRaw |= (((nCol.b & 0xF8) >> 3) <<  0); // Mask: 0000 0000 0001 1111

  #endif // DRV_COLORMODE_*

  return nColRaw;
}

// ============================================================================
