// =======================================================================
// TFT_Calibration 
//
// Author: Paul Conti 2024-03-19
//
// Implements a framework or wrapper for integrating  
// runtime TFT display calibration with values stored
// on an SD card thus overriding values stored in
// compiled code taken from GUIslice/config/*.h files.
// This is for use with applications written using GUIslice API.
//
// TFT_Calibration.init() will determine if calibration
// values are presently stored either on the mounted SD card,
// or in EEPROM depending upon which #define 
// USE_SD_INTERFACE or USE_EEPROM_INTERFACE is selected.
// 
// During init() phase it will read in the calibration values and call 
// GUIslice API's that will then override the default values taken 
// from the config file.
//
// All details concerning interfacing with GUIslice will be handled 
// by this routine. In particular it will use these GUIslice API calls:
// -  gslc_SetTouchRemapCal(&m_gui, nXMin, nXMax, nYMin, nYMax);
// -  gslc_SetTouchRemapYX(&m_gui, nRemapYX);
// -  gslc_SetTouchRemapEn(&m_gui, true); 
//
// USAGE:
//   In your App's global space create a new object for calibation like so:
//   TFT_Calibration calibration = TFT_Calibration();
//
//   Then your App should call calibration.init() in your setup() function
//   after doing InitGUIslice_gen();
//
//   To use this object you must add a popup page to your App. 
//   No Need to add any elements. Whatever enum name assigned to this page
//   it's name must be passed into TFT_Calibration.init() call.
// 
//   This is how this class can restore your App back to where it came
//   from simply by doing a gslc_PopupHide(pGui),
//
//   All on screen text display has been removed to allow developers to localize
//   instructions to their language. This means you must provide a page
//   informing a user on how to run calibration.
//
// NOTES: 
//   This code is completely and shamelessly ripped off from Calvin Hass's 
//   "diag_and_touch_calib.ino".
//
//   This routine will work with any touch interface supported by the GUIslice library.
//   By default, a TFT display may not require calibration (eg. capacitive touch),
//
//   MANY CAPACITIVE DISPLAYS DO NOT REQUIRE ANY CALIBRATION AT ALL,
//   However some may have flipped X/Y axes. In this case, running the calibration
//   sketch can enable one to ensure the orientation is correct.
//   All of which means you should simply use the standalone "diag_and_touch_calib.ino".
//   and avoid using this class for capacitive displays. When adding the calibration
//   settings to such configurations, one will also need to enable DRV_TOUCH_CALIB inside
//   your config file to indicate that the calibration settings are to be expected. 
//   Otherwise they will be ignored by GUIslice API. 
//
//   If this header file has DEBUG_CALIB set to 1 the calibration values will
//   be output to your serial monitor.  
//
//   setting DEBUG_CALIB 2 will output a more detailed reporting for deeper
//   debugging of this routine.
//
//   The Test features of "diag_and_touch_calib.ino" where a red dot can be placed on
//   screen has not be ported to this module since it's not expected that users would be
//   forced to do any debugging.
//
// WARNINGS:
//   If you use TFT_Calibration you must not call gslc_update() yourself.
//   Instead you must call TFT_Calibration.update in its place.
//   gslc_update() must be avoided since your App's UI Elements and touch
//   handling along with callbacks would interfere with the calibration UI.
//
// =======================================================================
// GUIslice library
// - Calvin Hass
// - https://www.impulseadventure.com/elec/guislice-gui.html
// - https://github.com/ImpulseAdventure/GUIslice
// =======================================================================// =======================================================================
//
// The MIT License
//
// Copyright (c) 2016-2021 Calvin Hass
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
/// \file TFT_Calibration.h 


#ifndef _TFT_CALIBRATION_H
#define _TFT_CALIBRATION_H

#include "GUIslice.h"
#include "GUIslice_drv.h"

// Ensure touch support has been enabled
#if defined(DRV_TOUCH_NONE)
  #error "Calibration testing requires a touch driver but DRV_TOUCH_NONE was set"
#elif defined(DRV_TOUCH_INPUT)
  #error "Calibration testing requires a touch driver but DRV_TOUCH_INPUT was set"
#elif defined(DRV_TOUCH_M5STACK)
  #error "Calibration testing requires a touch driver but DRV_TOUCH_M5STACK was set"
#endif

// ------------------------------------------------------------
// Check configuration settings
// ------------------------------------------------------------
#if !defined(DRV_TOUCH_CALIB)
  #warning "Calibration only works for resistive displays"
#endif // DRV_TOUCH_CALIB
#define DO_CALIB

// Import optional libraries
#if defined(DRV_DISP_ADAGFX_MCUFRIEND)
  // Import MCUFRIEND to support ID reporting
  #include <MCUFRIEND_kbv.h>
  extern MCUFRIEND_kbv m_disp;
  int16_t m_nMcuFriendId = -1;
#endif

// ------------------------------------------------------------
// Defines
// ------------------------------------------------------------
// Uncomment one of the following lines to use either SD card or EEPROM
//#define USE_SD_INTERFACE
#define USE_EEPROM_INTERFACE

// - Set DEBUG_CALIB to >0 to enable detailed tracing via the Serial connection
//   - DEBUG_CALIB 0 = Disable all messaging to serial monitor
//   - DEBUG_CALIB 1 = Enable summary messaging
//   - DEBUG_CALIB 2 = Enable detailed tracing for debugging
#define DEBUG_CALIB  1

#define CALIB_DEBUG_PRINT(sFmt, ...)                    \
          do {                                                  \
            if (DEBUG_CALIB) {                                    \
              gslc_DebugPrintf(PSTR(sFmt),__VA_ARGS__);         \
            }                                                   \
          } while (0) 
 
  #define CALIB_DEBUG2_PRINT(sFmt, ...)                          \
          do {                                                  \
            if (DEBUG_ERR >= 2) {                               \
              gslc_DebugPrintf(sFmt,__VA_ARGS__);               \
            }                                                   \
          } while (0)
           
// Select method used for averaging
// - Uncomment one of the following lines#define CALC_BASIC_AVG  // Simple average
#define CALC_BASIC_AVG  // Simple average
//#define CALC_EXP_AVG  // Exponential moving average [not yet supported]

// Define calibration target display
#define HOLD_DELAY    3000 // 3 seconds
#define TARGET_MARGIN 10
#define TARGET_SIZE   10
#define SEG_MIN_RANGE 200
#define MY_STRING_SZ  60

// Define the tolerance in detecting the rotated display
// - Setting a value of 10 means that it will attempt to
//   locate a calibration point within 10% of the rotated click
#define DET_ROT_TOLER_PCT 10

// Define the minimum threshold in Min-Max raw range below
// which we report that the calibration has failed. In most
// displays, a typical range might be 200...3700, implying
// a full-scale range of 3500. One would expect to obtain
// calibration ranges in excess of 10% of the full-scale range.
// For resistive displays, a default of 350 is reasonable
// For capacitive displays, a default of 50 is reasonable
#define DET_RAW_RANGE_MIN 50

// Define the maximum variance between readings that are
// expected to be on the same side of the display (eg. X:TL/BL)
// Exceeding this value will cause the sketch to report
// that the calibration has failed, likely due to a pin
// wiring issue.
// For resistive displays, a default of 350 is reasonable
// For capacitive displays, a default of 50 is reasonable
#define DET_RAW_VAR_MAX 350

// ------------------------------------------------------------
// TYPEDEFs
// ------------------------------------------------------------

// State machine settings
typedef enum {
  STATE_CAL_UNDEF = 0,
  // CALIB:
  STATE_CAL_START_MSG,
  STATE_CAPT_TL_MSG,
  STATE_CAPT_TL_DO,
  STATE_CAPT_TR_MSG,
  STATE_CAPT_TR_DO,
  STATE_CAPT_BL_MSG,
  STATE_CAPT_BL_DO,
  STATE_CAPT_BR_MSG,
  STATE_CAPT_BR_DO,
  STATE_CAPT_ROT,
  STATE_CAPT_ROT_TL_MSG,
  STATE_CAPT_ROT_TL_DO,
  STATE_CAL_REPORT,
  STATE_CAL_DONE_MSG,
  STATE_CAL_DONE_END,
} teState;

typedef enum {
  DEBOUNCE_UNDEF = 0,
  DEBOUNCE_UP,
  DEBOUNCE_UP2DOWN,
  DEBOUNCE_DOWN,
  DEBOUNCE_DOWN2UP,
} teDebounce;

#define DEBOUNCE_DELAY 50 // Delay (in ms) for debouncing touch events

// Definitions for calibration points
typedef enum {
  POINT_TL = 0,
  POINT_TR,
  POINT_BL,
  POINT_BR,
  POINT_TL_ROT,
  POINT_TEST,
  POINT__NUM,
} teCalibPt;

typedef enum {
  E_SEG_LL = 0,
  E_SEG_ML,
  E_SEG_HL,
  E_SEG_LM,
  E_SEG_MM,
  E_SEG_HM,
  E_SEG_LH,
  E_SEG_MH,
  E_SEG_HH
} teCalibSeg;

typedef enum {
  E_ROT_NORMAL = 0,
  E_ROT_REVERSE,
  E_ROT_SWAP,
  E_ROT_NONE,
  E_ROT_UNDEF
} teRotSpecial;

typedef struct {
  bool          bXLeft;
  bool          bYTop;
  const char*   acStr;
  teState       eStateDo;
  teState       eStateNxt;
} tsCalibStage;

enum eCalibrateStatus { CALIBR_UNDEFINED, CALIBR_SUCCESS, CALIBR_FAILURE };

// ------------------------------------------------------------

class TFT_Calibration {
  public:
    TFT_Calibration() {};
    virtual ~TFT_Calibration() {};   
#if defined(USE_SD_INTERFACE)
    void init(gslc_tsGui* pGui, int16_t popupPage, char* fileName); 
#elif defined(USE_EEPROM_INTERFACE)
    void init(gslc_tsGui* pGui, int16_t popupPage, int eeAddr /* EEPROM Address */); 
#else
  #error MUST use either SD card or EEPROM to store calibration values
#endif    
    void update();              // update display
    void run();                 // run calibratio
    void restore();             // restore values from config/*.h file in case of a screwup
    
    bool isActive() { return m_bIsActive; }; // currently running calibration? yes/no
    
    void setStatus(eCalibrateStatus eStatus) { m_eStatus = eStatus; };
    eCalibrateStatus getStatus() { return m_eStatus; };  

  protected:
    void loop();                 // Calibration loop()
    void storeValues();          // Place our calibration values into storage
    void inform();               // inform GUIslice of our calibration values 
    
    void printStoredValues();
    void printDefinedValues();
    
    void doDebounce(bool bRawDown, bool bRawUp, bool &bFiltDown, bool &bFiltUp);
    void doFsm(bool bTouchDown, bool bTouchUp, int16_t nTouchX, int16_t nTouchY, uint16_t nTouchZ);
    
  private:
    void drawBackground();
    void addDatapoint(int16_t nTouchX, int16_t nTouchY);
    void calcCalib();
    void calcCalibRotate();
    void calcDatapoint(teCalibPt ePoint);
    void calcMaxCoords(int16_t nTouchX, int16_t nTouchY, uint16_t nTouchZ);
    bool calcSegments();
    void drawCalibResult();
    void drawTarget(int16_t nX, int16_t nY, uint16_t nSize, gslc_tsColor nColOut, gslc_tsColor nColIn);
    void eraseTarget(int16_t nX, int16_t nY, uint16_t nSize);
    void reportCalibResult();
    void resetCalib();
    void resetMax();
    void redrawStatus(const char* pStatus);
    void resetDatapoints();
    
    teCalibPt findClosestPt(int16_t nX, int16_t nY); 
    
#if defined(USE_SD_INTERFACE)
    char*        m_fileName;      // file that has calibration values
#elif defined(USE_EEPROM_INTERFACE)
    int          m_eeAddr;        // EEPROM Address that holds calibration values
#endif     
    gslc_tsGui*  m_pGui;          // pointer to all of GUIslice's state and content
    bool         m_bIsActive;     // boolean for is calibration running 
    gslc_tsColor m_bgCol;         // background color
    int16_t      m_popupPage;     // popup page to overwrite
    uint8_t      m_nRotation;     // Adafruit GFX Rotation of display
    int16_t      m_xMin;
    int16_t      m_xMax;
    int16_t      m_yMin;
    int16_t      m_yMax;
    int16_t      m_remapYX;
    
    // save some original values in case of failure
    bool         m_saved_bTouchRemapEn;
    bool         m_saved_bTouchRemapYX;
    
    // Calibation state machine
    uint32_t     m_nTmHold;
    bool         m_bStartTimer;
    tsCalibStage m_asCalibStage[5];   
    teState      m_eState;
    teDebounce   m_eDebounce;
    teCalibPt    m_eCalibPt;
    teRotSpecial m_eRotSpecial;
    teState      m_eStateOld;
    bool         m_bStateBegin ;
    eCalibrateStatus m_eStatus;

    // Calibration data  
    int16_t      m_nTouchXMin;
    int16_t      m_nTouchXMax;
    int16_t      m_nTouchYMin;
    int16_t      m_nTouchYMax;
    int16_t      m_nTouchZMin;
    int16_t      m_nTouchZMax;

    int16_t      m_nTouchCalXMin;
    int16_t      m_nTouchCalXMax;
    int16_t      m_nTouchCalYMin;
    int16_t      m_nTouchCalYMax;

    int16_t      m_nPointFinalX;
    int16_t      m_nPointFinalY;

    // Corner readings before adjusting for offsets
    int16_t      m_nTouchCalXMinRaw;
    int16_t      m_nTouchCalXMaxRaw;
    int16_t      m_nTouchCalYMinRaw;
    int16_t      m_nTouchCalYMaxRaw;
    // Preserve reading ranges
    int16_t      m_nTouchCalXRngRaw;
    int16_t      m_nTouchCalYRngRaw;

    bool         m_bTouchCalFail;

    int          m_anTouchXPeak[POINT__NUM];
    int          m_anTouchYPeak[POINT__NUM];

    bool         m_bRemapYX;

    bool         m_bTouchCoordValid;
    
    // Storage for touch averaging
    int32_t      m_nPointBufX;
    int32_t      m_nPointBufY;
    uint16_t     m_nPointBufCnt;

    char         m_acTxt[MY_STRING_SZ];    
};

#endif // _TFT_CALIBRATION_H
