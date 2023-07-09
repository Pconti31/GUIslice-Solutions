
#include <Arduino.h>
#include "ScreenManager.h"
#include "gslc_touchAny.h"

extern gslc_tsPage m_asPage[];

void ScreenManager::init(gslc_tsGui* pGui, int timeout/*seconds*/, 
                         SCREENCB pausecb,
                         SCREENCB resumecb) {
  init(pGui, timeout, pausecb, resumecb, NULL, NULL);
}
                         
void ScreenManager::init(gslc_tsGui* pGui, int timeout/*seconds*/, 
                         SCREENCB pausecb,
                         SCREENCB resumecb,
                         SCREENCB displaycb) {
  init(pGui, timeout, pausecb, resumecb, displaycb, NULL);
}
                         
void ScreenManager::init(gslc_tsGui* pGui, int timeout/*seconds*/, 
                         SCREENCB pausecb,
                         SCREENCB resumecb,
                         SCREENCB displaycb,
                         SCREENCB countdowncb) {
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

}

void ScreenManager::update() {
  // ----------------------------------------------------------
  // Check for Inactivity
  // ----------------------------------------------------------
  // Test for any touch events 
  // A touch event will trigger a reset of our inactivity timer
  // NOTE: We can't place reset code inside CbBtnCommon() since
  //       checkboxes and sliders don't use that callback.
  // -----------------------------------------------------------
  bool bTouchEvent = gslc_touchAny(m_pGui);
  uint32_t currentTime = millis();
  if (m_bIsActive) {
    if (bTouchEvent) {
      // notify user of count down, if requested
      if (m_fCountdowncb != NULL) {
        int count= (int)(m_nInactivityMax/1000);
        (m_fCountdowncb)();
      } 
      // turn off screen saver
      m_bIsActive = false;
      pause();
      // refresh current page 
      gslc_DrvDrawBkgnd(m_pGui);
      gslc_tsPage* curPage = &m_asPage[gslc_GetPageCur(m_pGui)];
      gslc_SetPageCur(m_pGui, curPage->nPageId);
      
      gslc_InvalidateRgnPage(m_pGui, curPage);
      gslc_tsCollect* pCollect = &curPage->sCollect;
      gslc_tsElemRef*   pElemRef = NULL;
      uint16_t          nInd;
      for (nInd=0;nInd<pCollect->nElemRefCnt;nInd++) {
        // Fetch the element pointer from the reference array
        pElemRef = &(pCollect->asElemRef[nInd]);
        gslc_ElemSetRedraw(m_pGui,pElemRef,GSLC_REDRAW_FULL);
      }
//      Serial.println("ScreenManager EXIT UPDATE");
      m_nStartTime = millis();
      /* If user supplied a countdown callback
       * notify user that touch occurred and our inactiviry countdown
       * has been reset. 
       */
      if (m_fCountdowncb != NULL) {
        m_nInactivityCount = 0;
       (m_fCountdowncb)();
      }
    }
  } else {
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
        closePopup();  // active popup will screwup return from screen saver
//        gslc_Update(m_pGui);
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
    if (m_bIsActive) {
      // callback to user supplied display function
      if (m_fDisplaycb != NULL)
        (m_fDisplaycb)();
      else
        yield(); // no routine supplied so just yield control.
    } else {
      // not active so call GUIslice's update so GUI can run
      gslc_Update(m_pGui);
    }
  }
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // Only if screen saver is off
  // ------------------------------------------------
  if (m_bIsActive) {
    // callback to user supplied loop function
    if (m_fDisplaycb != NULL)
      (m_fDisplaycb)();
  } else {
    gslc_Update(m_pGui);
  }
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

void ScreenManager::setTimeout(int timeout/*seconds*/) {
  m_nInactivityMax = timeout * 1000;
  m_nInactivityCount = 0;
}

void ScreenManager::closePopup() {
  gslc_tsPage* pPage = m_pGui->apPageStack[GSLC_STACK_OVERLAY];
  if (pPage != NULL) {
//    Serial.print("Closed Popup page: "); Serial.println(pPage->nPageId);
    gslc_PopupHide(m_pGui);
  }
}
