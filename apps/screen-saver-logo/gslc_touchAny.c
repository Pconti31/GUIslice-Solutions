
#include "gslc_touchAny.h"

// uncomment to debug my touch events
//#define MY_TOUCH

bool my_TrackTouch(gslc_tsGui* pGui,int16_t nX,int16_t nY,uint16_t nPress)
{
  // Determine the transitions in the touch events based
  // on the previous touch pressure state
  gslc_teTouch  eTouch = GSLC_TOUCH_NONE;
  if ((pGui->nTouchLastPress == 0) && (nPress > 0)) {
    eTouch = GSLC_TOUCH_DOWN;
    #ifdef MY_TOUCH
    GSLC_DEBUG_PRINT("My Trk: (%3d,%3d) P=%3u : TouchDown\n",nX,nY,nPress);
    #endif
    return true;
  } else if ((pGui->nTouchLastPress > 0) && (nPress == 0)) {
    eTouch = GSLC_TOUCH_UP;
    #ifdef MY_TOUCH
    GSLC_DEBUG_PRINT("My Trk: (%3d,%3d) P=%3u : TouchUp\n",nX,nY,nPress);
    #endif
    return true;
  } else if ((pGui->nTouchLastX != nX) || (pGui->nTouchLastY != nY)) {
    // We only track movement if touch is "down"
    if (nPress > 0) {
      eTouch = GSLC_TOUCH_MOVE;
      #ifdef MY_TOUCH
      GSLC_DEBUG_PRINT("My Trk: (%3d,%3d) P=%3u : TouchMove\n",nX,nY,nPress);
      #endif
      return true;
    }
  }

  // Save raw touch status so that we can detect transitions
  pGui->nTouchLastX      = nX;
  pGui->nTouchLastY      = nY;
  pGui->nTouchLastPress  = nPress;

  #ifdef MY_TOUCH
  GSLC_DEBUG_PRINT("My Trk: No More Touch Events\n","");
  #endif
  return false;
}

bool my_touch_loop(gslc_tsGui* pGui) {
  uint16_t              nTouchPress = 0;
  bool                  bEvent = false;
  int16_t               nTouchX = 0;
  int16_t               nTouchY = 0;
  gslc_teInputRawEvent  eInputEvent = GSLC_INPUT_NONE;
  int16_t               nInputVal = 0;

  // Fetch input event, which could include touch / mouse / keyboard / pin
  bEvent = gslc_GetTouch(pGui, &nTouchX, &nTouchY, &nTouchPress, &eInputEvent, &nInputVal);
  if (bEvent) {
    // Track and handle the input events
    // - Handle the events on the current page
    switch (eInputEvent) {
      case GSLC_INPUT_KEY_DOWN:
      case GSLC_INPUT_PIN_ASSERT:
      case GSLC_INPUT_KEY_UP:
      case GSLC_INPUT_PIN_DEASSERT:
        break;
      case GSLC_INPUT_TOUCH:
        // Track touch event
        return my_TrackTouch(pGui,nTouchX,nTouchY,nTouchPress);
      case GSLC_INPUT_NONE:
      default:
        break;
    }
  }
  return false;
}

// Polling loop for GUIslice events
// does not affect event queue and does not replace calling gslc_update() 
bool gslc_touchAny(gslc_tsGui* pGui)
{
  // The touch handling logic is used by both the touchscreen
  // handler as well as the GPIO/pin/keyboard input controller
  #if !defined(DRV_TOUCH_NONE)

  // Check to see if we had a touch initialization error
  // if so, mark this on the display.
  if (pGui->eInitStatTouch == GSLC_INITSTAT_FAIL) {
    return false;
  }

  // ---------------------------------------------
  // Touch handling
  // ---------------------------------------------

  int16_t               nTouchX = 0;
  int16_t               nTouchY = 0;
  uint16_t              nTouchPress = 0;
  bool                  bEvent = false;
  gslc_teInputRawEvent  eInputEvent = GSLC_INPUT_NONE;
  int16_t               nInputVal = 0;

  // Track touchscreen presses
  bool      bDoneEvts = false;
  uint16_t  nNumEvts  = 0;

  bEvent = false;

  // --------------------------------------------------------------
  // First check physical pin inputs
  // --------------------------------------------------------------

  #if (GSLC_FEATURE_INPUT)
  int16_t  nPinNum = -1;
  int16_t  nPinState = 0;
  GSLC_CB_PIN_POLL  pfuncPinPoll = pGui->pfuncPinPoll;

  if (pfuncPinPoll != NULL) {
    bEvent = (*pfuncPinPoll)(pGui,&nPinNum,&nPinState);
    if (bEvent) {
      return true;
    }
  } 
  #endif // GSLC_FEATURE_INPUT

  // Fetch input event, which could include touch / mouse / keyboard / pin
  bEvent = gslc_GetTouch(pGui, &nTouchX, &nTouchY, &nTouchPress, &eInputEvent, &nInputVal);

  // --------------------------------------------------------------
  // If event found, we need to loop until 
  // --------------------------------------------------------------
  uint16_t nReturn = 0;
  if (bEvent) {

    // Track and handle the input events
    // - Handle the events on the current page
    switch (eInputEvent) {
      case GSLC_INPUT_KEY_DOWN:
        gslc_TrackInput(pGui,eInputEvent,nInputVal);
        return true;
      case GSLC_INPUT_KEY_UP:
        // NOTE: For now, only handling key-down events
        break;
      case GSLC_INPUT_PIN_ASSERT:
        gslc_TrackInput(pGui,eInputEvent,nInputVal);
        return true;
      case GSLC_INPUT_PIN_DEASSERT:
        return true;
      case GSLC_INPUT_TOUCH:
        // eat all touch events to avoid triggering hidden UI elements
        while(my_touch_loop(pGui)) { };
        GSLC_DEBUG_PRINT("My Trk: EXIT ****\n","");
        return true;
      case GSLC_INPUT_NONE:
      default:
        break;
    }

  }

  #endif // !DRV_TOUCH_NONE
  return false;
}
