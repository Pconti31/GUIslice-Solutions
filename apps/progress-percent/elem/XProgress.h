#ifndef _GUISLICE_EX_XPROGRESS_H_
#define _GUISLICE_EX_XPROGRESS_H_

#include "GUIslice.h"

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
/// \file XProgress.h

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


// ============================================================================
// Extended Element: Progress Bar
// - Basic progress bar with support for vertical / horizontal orientation and
//   fill direction. Also provides an indicator of negative regions, depending
//   on the configured range.
// ============================================================================

// Define unique identifier for extended element type
// - Select any number above GSLC_TYPE_BASE_EXTEND
#define  GSLC_TYPEX_PROGRESS GSLC_TYPE_BASE_EXTEND + 60



// Extended element data structures
// - These data structures are maintained in the gslc_tsElem
//   structure via the pXData pointer

/// Extended data for Gauge element
typedef struct {

  // Range config
  int16_t             nMin;           ///< Minimum control value
  int16_t             nMax;           ///< Maximum control value

  // Current value
  int16_t             nVal;           ///< Current control value
  // Previous value
  int16_t             nValLast;       ///< Last value
  bool                bValLastValid;  ///< Last value valid?

  // Appearance config
  int16_t             nFontId;        ///< Configured Mono font for Percent Number
  int16_t             nDigetW;        ///< Width of our Mono font single numeric diget
  int16_t             nDigetH;        ///< Height of our Mono font single numeric diget
  gslc_tsColor        colGauge;       ///< Color of gauge fill bar
  bool                bVert;          ///< Vertical if true, else Horizontal
  bool                bFlip;          ///< Reverse direction of gauge

#if defined(DRV_DISP_TFT_ESPI)
  void* pvSpr;
#endif

} gslc_tsXProgress;


///
/// Create a Progress Bar Element
/// - Draws a gauge element that represents a proportion (nVal)
///   between nMin and nMax.
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  nElemId:     Element ID to assign (0..16383 or GSLC_ID_AUTO to autogen)
/// \param[in]  nPage:       Page ID to attach element to
/// \param[in]  pXData:      Ptr to extended element data structure
/// \param[in]  rElem:       Rectangle coordinates defining gauge size
/// \param[in]  nMin:        not used
/// \param[in]  nMax:        not used
/// \param[in]  nVal:        Starting value of gauge - only 0 to 100 accepted
/// \param[in]  colGauge:    Color for the gauge indicator
/// \param[in]  bVert:       Flag to indicate vertical vs horizontal action
///                          (true = vertical, false = horizontal)
///
/// \return Pointer to Element reference or NULL if failure
///
gslc_tsElemRef* gslc_ElemXProgressCreate(gslc_tsGui* pGui,int16_t nElemId,int16_t nPage,
  gslc_tsXProgress* pXData,gslc_tsRect rElem,int16_t nMin,int16_t nMax,int16_t nVal,
  gslc_tsColor colGauge,bool bVert);


///
/// Update a Gauge element's current value
/// - Note that min & max values are not used and
///   nVal is a percent from 0 to 100%
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElemRef:    Pointer to Element reference
/// \param[in]  nVal:        New value to show in gauge
///
/// \return none
///
void gslc_ElemXProgressSetVal(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,int16_t nVal);

///
/// Add a Font to Gauge element so it can display percent completed or remaining
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElemRef:    Pointer to Element reference
/// \param[in]  nFontId:     Font ID to use for drawing text
///
/// \return none
///
void gslc_ElemXProgressSetFont(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,int8_t nFontId);

///
/// Set a Gauge element's to show percent remaining instead of percent completed
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElemRef:    Pointer to Element reference
/// \param[in]  bFlip:       If set, show percent remaining
///
/// \return none
///
void gslc_ElemXProgressSetFlip(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,bool bFlip);

///
/// Set the gauge color
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElemRef:    Pointer to Element reference
/// \param[in]  colGauge:    Color for the gauge's fill
///
/// \return none
///
void gslc_ElemXProgressSetGaugeCol(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,gslc_tsColor colGauge);


///
/// Draw a gauge element on the screen
/// - Called from gslc_ElemDraw()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElemRef:   Void ptr to Element reference (typecast to gslc_tsElemRef*)
/// \param[in]  eRedraw:     Redraw mode
///
/// \return true if success, false otherwise
///
bool gslc_ElemXProgressDraw(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw);



// ============================================================================

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _GUISLICE_EX_XPROGRESS_H_

