
#include "gslc_touchAny.h"

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
extern const char GSLC_PMEM ERRSTR_PXD_NULL[];// Polling loop for GUIslice events

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
  // If event found, return true
  // --------------------------------------------------------------
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
        // Track touch event
        gslc_TrackTouch(pGui,NULL,nTouchX,nTouchY,nTouchPress);
        return true;
      case GSLC_INPUT_NONE:
      default:
        break;
    }

  }

  #endif // !DRV_TOUCH_NONE
  return false;
}

