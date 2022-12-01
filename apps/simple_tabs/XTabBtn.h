#ifndef _GUISLICE_EX_XTABBTN_H_
#define _GUISLICE_EX_XTABBTN_H_

#include "GUIslice.h"


// =======================================================================
// GUIslice library extension: Tab button control
// - Paul Conti
// - Based on Calvin Hass' Text Button and Checkbox controls
// - https://www.impulseadventure.com/elec/guislice-gui.html
// - https://github.com/ImpulseAdventure/GUIslice
// =======================================================================
//
// The MIT License
//
// Copyright 2016-2019 Calvin Hass and Paul Conti
// Copyright (c) 2022 Paul Conti
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
/// \file XTabBtn.h

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// ============================================================================
// Extended Element: Pushbtn
// - Pushbtn with custom handler for touch which
//   sets glow once a button pressed on and remains glowing
//   until the button is pressed again thus the toggle effect.
//   Just like radio buttons toggle buttons can be grouped
//   making them an ideal choice for tabs on a base page.
// ============================================================================

// Define unique identifier for extended element type
// - Select any number above GSLC_TYPE_BASE_EXTEND
#define  GSLC_TYPEX_TABBTN GSLC_TYPE_BASE_EXTEND + 400

// Extended element data structures
// - These data structures are maintained in the gslc_tsElem
//   structure via the pXData pointer

/// Extended data for Tab button element
typedef struct {
  bool                        bOn;          ///< Indicates if it is selected (checked)
  int16_t                     nMyPageId;    ///< We need to track our page in case of grouping elements 
                                            ///< on a non current layer
  int16_t                     nJumpPageId;  ///< Jump to this Page when selected
  GSLC_CB_TOUCH               pfunctUser;   ///< User's Callback event to say element has changed
} gslc_tsXTabBtn;


///
/// Redefine a TabBtn Element from a Text Button Element
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElemRef:    Pointer to a BOX element reference with click and draw callbacks
/// \param[in]  nPage:       Page ID where this element is attached
/// \param[in]  nJumpPage:   Jump to this Page ID when selected
/// \param[in]  bState:      Starting state true=selected, false=not selected
///
/// \return none
///
void redefineButtonAsTab(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef, int16_t nPage, 
  int16_t nJumpPage, bool bState); 

///
/// Get a TabBtn element's current state
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElemRef:    Pointer to Element reference
///
/// \return Current state
///
bool gslc_ElemXTabBtnGetState(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef);

///
/// Set a TabBtn element's current state
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElemRef:    Pointer to Element reference
/// \param[in]  bOn:    New state
///
/// \return none
///
void gslc_ElemXTabBtnSetState(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,bool bOn);

///
/// Find the tabbtn within a group that has been selected
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  nGroupId:    Group ID to search
///
/// \return Element Ptr or NULL if none selected
///
gslc_tsElemRef* gslc_ElemXTabBtnFindSelected(gslc_tsGui* pGui,int16_t nGroupId);

///
/// Push a TabBtn element's current state
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElemRef:    Pointer to Element reference
///
/// \return none
///
void gslc_ElemXTabBtnPushState(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef);

///
/// Draw a TabBtn element on the screen
/// - Called from gslc_ElemDraw()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElemRef:   Void ptr to Element reference (typecast to gslc_tsElemRef*)
/// \param[in]  eRedraw:     Redraw mode
///
/// \return true if success, false otherwise
///
bool gslc_ElemXTabBtnDraw(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw);

///
/// Handle touch events to TabBtn element
/// - Called from gslc_ElemSendEventTouch()
///
/// \param[in]  pvGui:       Void ptr to GUI (typecast to gslc_tsGui*)
/// \param[in]  pvElemRef:   Void ptr to Element reference (typecast to gslc_tsElemRef*)
/// \param[in]  eTouch:      Touch event type
/// \param[in]  nRelX:       Touch X coord relative to element
/// \param[in]  nRelY:       Touch Y coord relative to element
///
/// \return true if success, false otherwise
///
bool gslc_ElemXTabBtnTouch(void* pvGui,void* pvElemRef,gslc_teTouch eTouch,int16_t nRelX,int16_t nRelY);

// ============================================================================


#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _GUISLICE_EX_XTABBTN_H_
