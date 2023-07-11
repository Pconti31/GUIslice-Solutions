
#include <Arduino.h>
#include "ScreenManager.h"
#include "gslc_touchAny.h"

// pointer to our screen manager
extern ScreenManager   screenMgr;

// various items for our dummy page which is used to stop our screen saver
#define E_PG_INVISIBLE  100
#define MAX_ELEM_PG_DUMMY 1
gslc_tsElem                     m_asScrMgrElem[1];
gslc_tsElemRef                  m_asScrMgrElemRef[1];
const int16_t E_ELEM_DUMMY_BOX = GSLC_ID_AUTO_BASE+1;

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
// Screen Manager callback
bool CbScrManager(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*     pGui     = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef);

  if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
      case E_ELEM_DUMMY_BOX:
        screenMgr.stop();
        break;
      default:
        break;
    }
  }
  return true;
}

// Scanner drawing callback function
// - This is called when E_ELEM_DUMMY_BOX is being rendered
bool CbDrawDummy(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw)
{
  int nInd;

  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*     pGui     = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef); 

  // Create shorthand variables for the origin
  int16_t  nX = pElem->rElem.x;
  int16_t  nY = pElem->rElem.y;

  // Draw the background
  gslc_DrawFillRect(pGui,pElem->rElem,pElem->colElemFill);

  // callback to user supplied loop function
  screenMgr.display();
  
  // Clear the redraw flag
  gslc_ElemSetRedraw(pGui,pElemRef,GSLC_REDRAW_NONE);

  return true;
}

void gslc_createInvisiblePG(gslc_tsGui* pGui) {
  
  // first create storage for all of our pages
  // increase our max_page by 1
  pGui->nPageMax++;
  gslc_tsPage* pages = (gslc_tsPage*)malloc(sizeof(gslc_tsPage)*pGui->nPageMax);
  // loop over our existing pages and copy over all values to our new storage
  for (int i=0; i<pGui->nPageCnt; i++) {
    pages[i].sCollect = pGui->asPage[i].sCollect;
    pages[i].nPageId = pGui->asPage[i].nPageId;
    pages[i].rBounds = pGui->asPage[i].rBounds;
  }
  // swap our new storage for the old page storage
  pGui->asPage = pages;

  // now add our new page
  gslc_PageAdd(pGui,E_PG_INVISIBLE,m_asScrMgrElem,MAX_ELEM_PG_DUMMY,m_asScrMgrElemRef,MAX_ELEM_PG_DUMMY);
  
  // Create E_ELEM_DUMMY_BOX box
  gslc_tsColor bgCol = screenMgr.getBgColor();
  gslc_tsElemRef* pElemRef = gslc_ElemCreateBox(pGui,E_ELEM_DUMMY_BOX,E_PG_INVISIBLE,(gslc_tsRect){0,0,pGui->nDispW,pGui->nDispH});
  gslc_ElemSetCol(pGui,pElemRef,bgCol,bgCol,bgCol);
  gslc_ElemSetClickEn(pGui, pElemRef, true);
  gslc_ElemSetTouchFunc(pGui, pElemRef, &CbScrManager);
  // Set the callback function to handle all logo drawing
  gslc_ElemSetDrawFunc(pGui,pElemRef,&CbDrawDummy);
      
}

void ScreenManager::init(gslc_tsGui* pGui, int timeout/*seconds*/, 
                         SCREENCB pausecb,
                         SCREENCB resumecb,
                         SCREENCB displaycb,
                         SCREENCB countdowncb,
                         gslc_tsColor bgCol) {
  m_bgCol = bgCol;
  m_pGui = pGui;
  m_nInactivityMax = timeout * 1000;
  m_fPausecb = pausecb;
  m_fResumecb = resumecb;
  m_fDisplaycb = displaycb;
  m_fCountdowncb = countdowncb;
  // Initialize our Timer
  m_nStartTime = millis();
  m_nInactivityCount = 0;
  m_bIsActive = false;
  // notify user of count down, if requested
  if (m_fCountdowncb != NULL) {
    (m_fCountdowncb)();
  }
  
  gslc_createInvisiblePG(m_pGui);

}

void ScreenManager::update() {
  // just jump to guislice update if we are disabled
  if (isDisabled()) {
    gslc_Update(m_pGui);
    return;
  }
  // ----------------------------------------------------------
  // Check for Inactivity
  // ----------------------------------------------------------
  // Test for any touch events 
  // A touch event will trigger a reset of our inactivity timer
  // NOTE: We can't place reset code inside CbBtnCommon() since
  //       checkboxes and sliders don't use that callback.
  // -----------------------------------------------------------
  uint32_t currentTime = millis();
  bool bTouchEvent = false;
  if (!m_bIsActive) {
    bTouchEvent = gslc_touchAny(m_pGui);
    if (bTouchEvent) {
      // screen saver is not on but activity occurred so reset timer
      m_nStartTime = currentTime;
      if (m_fCountdowncb != NULL) {
        m_nInactivityCount = 0;
       (m_fCountdowncb)();
      }
    } else {
      // screen saver is not on but no activity occurred
      // so test if timer has expired?
      if ((currentTime-m_nStartTime) >= m_nInactivityMax) {
        // timer has expired so trigger screen saver code
        // but first blank screen
        gslc_DrvDrawBkgnd(m_pGui);
//        gslc_InvalidateRgnPage(m_pGui,&m_asPage[gslc_GetPageCur(m_pGui)]);
        closePopup();  // active popup will screw-up return from screen saver
        // now save our current page so we can jump back
//        m_curPage = (gslc_tsPage*)&m_pGui->asPage[gslc_GetPageCur(m_pGui)];
        // set new current page to our invisible page holding giant invisible button
        gslc_PopupShow(m_pGui, E_PG_INVISIBLE, true);

        // turn on our screen saver
        m_nInactivityCount = 0;
        m_bIsActive = true;
        
        resume();
      } // timer not expired so update count down display, if on second boundary
        else if (m_fCountdowncb != NULL &&
                 ((currentTime-m_nStartTime)/1000) > m_nInactivityCount) {
          // count down only shows seconds remaining
          m_nInactivityCount = (currentTime-m_nStartTime)/1000;
          int count= (int)((m_nInactivityMax/1000)-m_nInactivityCount);
          (m_fCountdowncb)();
      }
    }
  } else {
    display();
  }
  // ------------------------------------------------
  // call GUIslice update function
  // ------------------------------------------------
  gslc_Update(m_pGui);
 }

//call a user supplied function to pause.
void ScreenManager::pause() {
  m_bIsActive = false;
  (m_fPausecb)();
}

//call a user supplied function to pause.
void ScreenManager::resume() {
  m_bIsActive = true;
  (m_fResumecb)();
}

int ScreenManager::getCountDown() {
  int count= (int)((m_nInactivityMax/1000)-m_nInactivityCount);
  return count;
}

void ScreenManager::display() {
  if (m_fDisplaycb != NULL) {
    (m_fDisplaycb)();
  }

}

void ScreenManager::stop() {
  m_bIsActive = false;
//  Serial.println("ScreenManager stop");
  
  // refresh current page 
  gslc_DrvDrawBkgnd(m_pGui);
  gslc_PopupHide(m_pGui);
  /* If user supplied a countdown callback
   * notify user that touch occurred and our inactiviry countdown
   * has been reset. 
   */
  m_nInactivityCount = 0;
  if (m_fCountdowncb != NULL) {
   (m_fCountdowncb)();
  }
  
  m_nStartTime = millis();
  
  pause();

}

void ScreenManager::setTimeout(int timeout/*seconds*/) {
  m_nInactivityMax = timeout * 1000;
  m_nInactivityCount = 0;
}

void ScreenManager::disable() {
  m_bIsDisabled = true;
  m_nInactivityCount = 0;
  if (isActive()) {
    stop();
  }
}

void ScreenManager::enable() {
  m_bIsDisabled = false;
  m_nInactivityCount = 0;
  m_nStartTime = millis();
}

void ScreenManager::closePopup() {
  gslc_tsPage* pPage = m_pGui->apPageStack[GSLC_STACK_OVERLAY];
  if (pPage != NULL) {
    gslc_PopupHide(m_pGui);
  }
}
