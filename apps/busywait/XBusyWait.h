#ifndef _GUISLICE_EX_XBUSYWAIT_H_
#define _GUISLICE_EX_XBUSYWAIT_H_

#include "GUIslice.h"


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
/// \file XBusyWait.h

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


// ======================================================================================
// Extended Element: Example busywait
// ======================================================================================

// Define unique identifier for extended element type
// - Select any number above GSLC_TYPE_BASE_EXTEND
#define  GSLC_TYPEX_BUSYWAIT GSLC_TYPE_BASE_EXTEND + 52

/// Gauge drawing style
typedef enum {
  GSLCX_BUSYWAIT_STYLE_UNDEFINED,       ///< No assigned style (error condition)
  GSLCX_BUSYWAIT_STYLE_SPINNING_DOTS,   ///< Circle with spinning dots around circumference
  GSLCX_BUSYWAIT_STYLE__SPINNING_WHEEL, ///< Circle with spinning wheel
  GSLCX_BUSYWAIT_STYLE_THREE_DOTS,      ///< Three dots flickering
} gslc_teXBusyWaitStyle;

#define BUSYWAIT_DELAY    100  // Number milliseconds between drawing each piece

/// Callback function for ScreenManager
typedef bool (*BUSYWAITCB)();

// Extended element data structures
// - These data structures are maintained in the gslc_tsElem
//   structure via the pXData pointer

/// Extended data for Slider element
typedef struct {
  // Common variables
  int16_t               centerX;      // center position of our circle display
  int16_t               centerY;
  gslc_tsColor          color;        // Color of busywait
  gslc_tsColor          currentColor; // Color varies from busywait color to background color
  BUSYWAITCB            pfuncYield;   // pointer to yield callback

  // GSLCX_BUSYWAIT_STYLE_SPINNING_DOTS variables
  int16_t               nSegments;         // number of balls to place around the circle
  int16_t               nSegmentRadius; // size of an individual segment
  int16_t               nCircleRadius;  // size of our main circle
  int16_t               inc;            // amount to increment angles when placing them on the circle
  int16_t               nSegmentsPainted;  // number balls displayed
  int16_t               nAngle;         // angle of spoke
  
  // State variables
  int16_t               nDelay;       // how long to delay between drawing
  bool                  bClearScreen; // true if we need to clear display
  bool                  bFirstTime;   // true if first time for animation
  gslc_teXBusyWaitStyle eStyle;       // Style of busywait 
  bool                  bActive;      // boolean indicates start of animation
  uint32_t              nTime;        // Length of time between drawing spokes
} gslc_tsXBusyWait;

///
/// Create a BusyWait Element from a Box Element
/// This avoids the GUIsliceBuilder having to support this element.
///
/// NOTE: Box size is ignored
///
/// \param[in]  pGui:       Pointer to GUI
/// \param[in]  pElemRef    Pointer to a BOX element reference
/// \param[in]  eStyle:     Drawing style for the busywait
/// \param[in]  nCol:       Color for the balls
/// \param[in]  pfYield;    Callback for yield
///
/// \return none
///
void redefineBoxAsBusyWait(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef, 
  gslc_teXBusyWaitStyle eStyle, gslc_tsColor nCol, BUSYWAITCB pfYield);

///
/// Position the busywait animation on the screen
/// This routine is optional, The default is to blank display then center animation
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElemRef:   Void ptr to Element (typecast to gslc_tsElemRef*)
/// \param[in]  nX:          Center animation X coordinate
/// \param[in]  nY:          Center animation Y coordinate
/// \param[in]  bClearScr:   true if you want the display cleared
///
/// \return none
///
void gslc_ElemXBusyWaitPositionAnimation(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef,
  int16_t nX, int16_t nY, bool bClearScr);

///
/// Start the busywait animation on the screen
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElemRef:   Void ptr to Element (typecast to gslc_tsElemRef*)
///
/// \return none
///
void gslc_ElemXBusyWaitStartAnimation(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef);

///
/// Start the busywait animation on the screen
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElemRef:   Void ptr to Element (typecast to gslc_tsElemRef*)
///
/// \return none
///
void gslc_ElemXBusyWaitStopAnimation(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef);

///
/// Draw the busywait element on the screen 
/// - Called from gslc_ElemDraw()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElemRef:   Void ptr to Element (typecast to gslc_tsElemRef*)
/// \param[in]  eRedraw:     Redraw mode
///
/// \return true if success, false otherwise
///
bool gslc_ElemXBusyWaitDraw(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw);


// ============================================================================


#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _GUISLICE_EX_XBUSYWAIT_H_
