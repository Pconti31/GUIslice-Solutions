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
// Copyright (c) 2016-2019 Calvin Hass  
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
/// \file XTabBtn.c



// GUIslice library
#include "GUIslice.h"
#include "GUIslice_drv.h"

//#include "elem/XTabBtn.h"
#include "XTabBtn.h"

#include <stdio.h>

#if (GSLC_USE_PROGMEM)
    #include <avr/pgmspace.h>
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
// Extended Element: TabBtn
// - TabBtn with custom handler for touch which
//   sets glow once a button pressed on and remains glowing
//   until the button is pressed again thus the toggle effect.
//   Once selected a jump to nJumpPage will occur automatically.
//   Just like radio buttons tab buttons can be grouped
//   making them an ideal choice for tabs on a base page.
// ============================================================================

// Create a pTabBtn element from an existing Text Button
void redefineButtonAsTab(gslc_tsGui* pGui, gslc_tsElemRef* pElemRef, int16_t nPage,
  int16_t nJumpPage, bool bState) 
{
  if (pGui == NULL) {
    static const char GSLC_PMEM FUNCSTR[] = "redefineButtonAsTab";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }
  gslc_tsElem* pElem = gslc_GetElemFromRef(pGui,pElemRef);

	// change element type from text button to Tab Button
  pElem->nType            = GSLC_TYPEX_TABBTN; 

  // Default group assignment. Can override later with ElemSetGroup()
  pElem->nGroup            = 999;

  // Define other extended data
  pElem->pXData           = (void*)(malloc(sizeof(gslc_tsXTabBtn)));
  gslc_tsXTabBtn* pTabBtn = (gslc_tsXTabBtn*)pElem->pXData;
  pTabBtn->bOn            = bState;  // save on/off status
  pTabBtn->nMyPageId      = nPage;   // save our page id for group by access later
  pTabBtn->nJumpPageId    = nJumpPage; // where to jump when selected
  
  // Specify the custom drawing callback
  pElem->pfuncXDraw       = &gslc_ElemXTabBtnDraw;
  
  // Specify the custom touch handler
  pTabBtn->pfunctUser     = pElem->pfuncXTouch; // save user's callback in our extra data
  pElem->pfuncXTouch      = &gslc_ElemXTabBtnTouch;

}

bool gslc_ElemXTabBtnGetState(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef)
{
  gslc_tsElem*    pElem     = gslc_GetElemFromRef(pGui,pElemRef);
  gslc_tsXTabBtn* pTabBtn = (gslc_tsXTabBtn*) gslc_GetXDataFromRef(pGui, pElemRef, GSLC_TYPEX_TABBTN, __LINE__);
  if (!pTabBtn) {
    GSLC_DEBUG_PRINT("ERROR: gslc_ElemXTabBtnGetState() pXData is NULL\n","");
    return false;
  }

  return pTabBtn->bOn;
}


// Determine which tab buttun in the group is selected "on"
gslc_tsElemRef* gslc_ElemXTabBtnFindSelected(gslc_tsGui* pGui,int16_t nGroupId)
{
  int16_t             nCurInd;
  gslc_tsElemRef*     pCurElemRef = NULL;
  gslc_tsElem*        pCurElem = NULL;
  int16_t             nCurType;
  int16_t             nCurGroup;
  bool                bCurSelected;
  gslc_tsElemRef*     pFoundElemRef = NULL;

  // Operate on current page
  // TODO: Support other page layers
  gslc_tsPage* pPage = pGui->apPageStack[GSLC_STACK_CUR];
  if (pPage == NULL) {
    return NULL; // No page added yet
  }
  
  if (pGui == NULL) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXTabBtnFindChecked";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return NULL;
  }

  gslc_tsCollect* pCollect = &pPage->sCollect;
  for (nCurInd=0;nCurInd<pCollect->nElemCnt;nCurInd++) {
    // Fetch extended data
    pCurElemRef   = &(pCollect->asElemRef[nCurInd]);
    pCurElem      = gslc_GetElemFromRef(pGui,pCurElemRef);
    nCurType      = pCurElem->nType;
    // Only want to proceed if it is a tab buttun
    if (nCurType != GSLC_TYPEX_TABBTN) {
      continue;
    }

    nCurGroup     = pCurElem->nGroup;
    bCurSelected   = gslc_ElemXTabBtnGetState(pGui,pCurElemRef);

    // If this is in a different group, ignore it
    if (nCurGroup != nGroupId) {
      continue;
    }

    // Did we find an element in the group that was checked?
    if (bCurSelected) {
      pFoundElemRef = pCurElemRef;
      break;
    }
  } // nCurInd
  return pFoundElemRef;
}

// Helper routine for gslc_ElemXTabBtnSetState()
// - Updates the tab buttun control's state but does
//   not touch any other controls in the group
void gslc_ElemXTabBtnSetStateHelp(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,bool bOn)
{
  gslc_tsElem*    pElem     = gslc_GetElemFromRef(pGui,pElemRef);
  gslc_tsXTabBtn* pTabBtn = (gslc_tsXTabBtn*) gslc_GetXDataFromRef(pGui, pElemRef, GSLC_TYPEX_TABBTN, __LINE__);
  if (!pTabBtn) {
    GSLC_DEBUG_PRINT("ERROR: gslc_ElemXTabBtnSetStateHelp() pXData is NULL\n","");
    return;
  }

  // Update our data element
  bool  bStateOld = pTabBtn->bOn;
  pTabBtn->bOn = bOn;

  // Element needs redraw
  if (bOn != bStateOld) {
    // Only need an incremental redraw
    gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_INC);
  }

}

// Update the pTabBtn control's state. If it's part of a group
// then also update the state of all other buttons in the group.
void gslc_ElemXTabBtnSetState(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef,bool bOn)
{
  gslc_tsElem* pElem = gslc_GetElemFromRef(pGui,pElemRef);
  gslc_tsXTabBtn* pTabBtn = (gslc_tsXTabBtn*) gslc_GetXDataFromRef(pGui, pElemRef, GSLC_TYPEX_TABBTN, __LINE__);
  if (!pTabBtn) {
    GSLC_DEBUG_PRINT("ERROR: gslc_ElemXTabBtnSetState() pXData is NULL\n","");
    return;
  }

  int16_t             nGroup    = pElem->nGroup;
  int16_t             nElemId   = pElem->nId;

  // if state hasn't changed just return
	if (pTabBtn->bOn == bOn) 
		return;
	
  if (bOn && pElem->nGroup != GSLC_GROUP_ID_NONE) {

    // If we are selecting a button that is already
    // selected, then skip further update events.
    // NOTE: This check is not very efficient, but it avoids
    // the creation of extra events.
    gslc_tsElemRef* pTmpRef = gslc_ElemXTabBtnFindSelected(pGui, nGroup);
    if (pTmpRef == pElemRef) {
      // Same element, so skip
      return;
    }

    // Proceed to deselect any other selected items in the group.
    // Note that SetState calls itself to deselect other items so it
    // is important to qualify this logic with bOn=true
    int16_t           nCurInd;
    int16_t           nCurId;
    gslc_tsElem*      pCurElem = NULL;
    gslc_tsElemRef*   pCurElemRef = NULL;
    int16_t           nCurType;
    int16_t           nCurGroup;

    /* 
     * The elements must be grouped on the same layer but do not need to be on the current
     * page.  This allows us to place grouped elements on the base page.
     * p conti.
     */
    // Find our page layer
    gslc_tsPage* pPage = gslc_PageFindById(pGui, pTabBtn->nMyPageId);
    if (pPage == NULL) {
      GSLC_DEBUG2_PRINT("ERROR: gslc_ElemXTabBtnSetState() can't find page (ID=%d)\n", 
         pTabBtn->nMyPageId);
      return;
    }

    gslc_tsCollect* pCollect = &pPage->sCollect;
    for (nCurInd=0;nCurInd<pCollect->nElemRefCnt;nCurInd++) {
      // Fetch extended data
      pCurElemRef   = &pCollect->asElemRef[nCurInd];
      pCurElem      = gslc_GetElemFromRef(pGui,pCurElemRef);
      nCurId        = pCurElem->nId;
      nCurType      = pCurElem->nType;

      nCurGroup     = pCurElem->nGroup;

      // If this is in a different group, ignore it
      if (nCurGroup != nGroup) {
        continue;
      }

      // Is this our element? If so, ignore the deselect operation
      if (nCurId == nElemId) {
        continue;
      }

      // Deselect all other elements
      gslc_ElemXTabBtnSetStateHelp(pGui,pCurElemRef,false);

    } // nInd

  } // bOn

  // Set the state of the current element
  gslc_ElemXTabBtnSetStateHelp(pGui,pElemRef,bOn);
}

// Push the pTabBtn control's state
void gslc_ElemXTabBtnPushState(gslc_tsGui* pGui,gslc_tsElemRef* pElemRef)
{
  if (pElemRef == NULL) {
    static const char GSLC_PMEM FUNCSTR[] = "ElemXTabBtnPushState";
    GSLC_DEBUG2_PRINT_CONST(ERRSTR_NULL,FUNCSTR);
    return;
  }
  // Update the data element
  bool bStateNew = (gslc_ElemXTabBtnGetState(pGui,pElemRef))? false : true;
  gslc_ElemXTabBtnSetState(pGui,pElemRef,bStateNew);

  // Element needs redraw
  // - Only incremental is needed
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_INC);
}

// Redraw the tab buttun
// - Note that this redraw is for the entire element rect region
// - The Draw function parameters use void pointers to allow for
//   simpler callback function definition & scalability.

bool gslc_ElemXTabBtnDraw(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw)
{
  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*       pGui  = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef*   pElemRef = (gslc_tsElemRef*)(pvElemRef);

  gslc_tsElem*    pElem     = gslc_GetElemFromRef(pGui,pElemRef);
  gslc_tsXTabBtn* pTabBtn = (gslc_tsXTabBtn*) gslc_GetXDataFromRef(pGui, pElemRef, GSLC_TYPEX_TABBTN, __LINE__);
  if (!pTabBtn) {
    GSLC_DEBUG_PRINT("ERROR: gslc_ElemXTabBtnDraw() pXData is NULL\n","");
    return false;
  }
  gslc_tsRect     rInner, rInnerGlow;

  bool bOn    = pTabBtn->bOn;
  bool bGlow  = bOn;
//  bool bGlow  = (pElem->nFeatures & GSLC_ELEM_FEA_GLOW_EN) && gslc_ElemGetGlow(pGui,pElemRef);

  // Work out the size of the inner rects, one for normal state, another smaller one 
  // for selected state.  This best demonstrates a toggled button visually.
  rInner = gslc_ExpandRect(pElem->rElem,-1,-1);
  //rInnerGlow = gslc_ExpandRect(pElem->rElem,-5,-5);
  if (bOn) {
      // If selected, glow the inner region
    if (pElem->nFeatures & GSLC_ELEM_FEA_ROUND_EN) {
//      gslc_DrawFillRoundRect(pGui,rInner,pGui->nRoundRadius,pElem->colElemFill);
//      gslc_DrawFillRoundRect(pGui,rInnerGlow,pGui->nRoundRadius,pElem->colElemFillGlow);
      gslc_DrawFillRoundRect(pGui,rInner,pGui->nRoundRadius,pElem->colElemFillGlow);
    } else {
//      gslc_DrawFillRect(pGui,rInner,pElem->colElemFill);
//      gslc_DrawFillRect(pGui,rInnerGlow, pElem->colElemFillGlow);
      gslc_DrawFillRect(pGui,rInner, pElem->colElemFillGlow);
    }
    gslc_DrawTxtBase(pGui, pElem->pStrBuf, rInner, pElem->pTxtFont, pElem->eTxtFlags,
      pElem->eTxtAlign, pElem->colElemTextGlow, pElem->colElemFillGlow, pElem->nTxtMarginX, pElem->nTxtMarginY);

  } else {
    // Not selected so just standard background
    if (pElem->nFeatures & GSLC_ELEM_FEA_ROUND_EN) {
      gslc_DrawFillRoundRect(pGui,rInner,pGui->nRoundRadius,pElem->colElemFill);
    } else {
      gslc_DrawFillRect(pGui,rInner,pElem->colElemFill);
    }
    gslc_DrawTxtBase(pGui, pElem->pStrBuf, rInner, pElem->pTxtFont, pElem->eTxtFlags,
      pElem->eTxtAlign, pElem->colElemText, pElem->colElemFill, pElem->nTxtMarginX, pElem->nTxtMarginY);
  }
  // Draw a frame around the tab buttun
	if (pElem->nFeatures & GSLC_ELEM_FEA_FRAME_EN) {
		if (pElem->nFeatures & GSLC_ELEM_FEA_ROUND_EN) {
			gslc_DrawFrameRoundRect(pGui,pElem->rElem,pGui->nRoundRadius,pElem->colElemFrame);
		} else {
			gslc_DrawFrameRect(pGui,pElem->rElem,pElem->colElemFrame);
		}
	}
  // Clear the redraw flag
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_NONE);

  // Mark page as needing flip
  gslc_PageFlipSet(pGui,true);

  return true;
}


// This callback function is called by gslc_ElemSendEventTouch()
// after any touch event
// - NOTE: Adding this touch callback is optional. Without it, we
//   can still have a functional pTabBtn, but doing the touch
//   tracking allows us to change the glow state of the element
//   dynamically, as well as updating the pTabBtn state if the
//   user releases over it (ie. a click event).
//
bool gslc_ElemXTabBtnTouch(void* pvGui,void* pvElemRef,gslc_teTouch eTouch,int16_t nRelX,int16_t nRelY)
{
#if defined(DRV_TOUCH_NONE)
  return false;
#else

  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*       pGui  = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef*   pElemRef = (gslc_tsElemRef*)(pvElemRef);

  gslc_tsElem* pElem = gslc_GetElemFromRef(pGui,pElemRef);
  gslc_tsXTabBtn* pTabBtn = (gslc_tsXTabBtn*) gslc_GetXDataFromRef(pGui, pElemRef, GSLC_TYPEX_TABBTN, __LINE__);
  if (!pTabBtn) {
    GSLC_DEBUG_PRINT("ERROR: gslc_ElemXTabBtnTouch() pXData is NULL\n","");
    return false;
  }

  // if we are already on stay on
	if (pTabBtn->bOn)
		return false;

  // If the pTabBtn changed state, redraw
  bool  bStateNew;
  bool  bStateOld = pTabBtn->bOn;
  bool  bChanged = false;

  switch(eTouch) {

    case GSLC_TOUCH_UP_IN:
      // Now that we released on element, update the state
      // TabBtn button action: toggle
      bStateNew = (pTabBtn->bOn)?false:true;
			if (bStateNew != bStateOld) {
				bChanged = true; 
        gslc_ElemSetGlow(pGui,pElemRef,bStateNew);
        gslc_ElemXTabBtnSetState(pGui,pElemRef,bStateNew);
			}
      break;
    default:
      return false;
      break;
  }

  if (bChanged) {
    // Now send the callback notification
    (*pTabBtn->pfunctUser)((void*)(pGui), (void*)(pElemRef), eTouch, nRelX, nRelY);
    if (pTabBtn->bOn) // jump to new page
      gslc_SetPageCur(pGui, pTabBtn->nJumpPageId);
    // Incremental redraw
    gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_INC);
  }

  return true;
  #endif // !DRV_TOUCH_NONE
}

// ============================================================================
