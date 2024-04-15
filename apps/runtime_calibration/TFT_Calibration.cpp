
#include <Arduino.h>
#include "TFT_Calibration.h"


#if defined(USE_SD_INTERFACE)
  #include <SD.h>   // Include support for SD card access
  #include <SPI.h>
#elif defined(USE_EEPROM_INTERFACE)
  #include <EEPROM.h>
  // MAGIC_VALUE allows us to test if eeprom values are set or not
  const uint16_t MAGIC_VALUE = 0xBCA5;  
#else
  #include <FlashStorage.h> //include support for writing to Flash Storage
  FlashStorage(flsh_xMin, int16_t); //create space in flash to save xMin value
  FlashStorage(flsh_xMax, int16_t); //create space in flash to save xMax value
  FlashStorage(flsh_yMin, int16_t); //create space in flash to save yMin value
  FlashStorage(flsh_yMax, int16_t); //create space in flash to save yMax value
  FlashStorage(flsh_remapYX, int16_t); //create space in flash to save remapXY
  FlashStorage(flsh_initial_Calib, bool); //create space in flash to save initial Calibration flag, defaults to false upon creation
#endif

#if defined(USE_SD_INTERFACE)
  void TFT_Calibration::init(gslc_tsGui* pGui, int16_t popupPage, char* fileName) {
    m_fileName = fileName;
#elif defined(USE_EEPROM_INTERFACE)
  void TFT_Calibration::init(gslc_tsGui* pGui, int16_t popupPage, int eeAddr) {
    m_eeAddr = eeAddr;
#else //using Flash Storage
  void TFT_Calibration::init(gslc_tsGui* pGui, int16_t popupPage) {
#endif    
  m_pGui  = pGui;
  
  // save rotation so we can restore after doing a calibration
  m_nRotation = m_pGui->nRotation;
  
  // save these values in case of failure later
  m_saved_bTouchRemapEn = m_pGui->bTouchRemapEn;
  m_saved_bTouchRemapYX = m_pGui->bTouchRemapYX;
  
  // set our flags
  m_bTouchCalFail = false;
  m_bIsActive = false;
  
  // The trick to save and restore our current page is for
  // the App developer to create a popup page that we can 
  // invoke before doing our calibration.  
  // Then when completed we just need to repaint our App's background 
  // and do a gslc_PopupHide(m_pGui);
  m_popupPage = popupPage;

#if defined(USE_SD_INTERFACE)
  if (SD.exists(m_fileName)) {
    File values = SD.open(m_fileName);
    values.read((byte*)&m_xMin, sizeof(int16_t));
    values.read((byte*)&m_xMax, sizeof(int16_t));
    values.read((byte*)&m_yMin, sizeof(int16_t));
    values.read((byte*)&m_yMax, sizeof(int16_t));
    values.read((byte*)&m_remapYX, sizeof(int16_t));
    values.close();
    
    CALIB_DEBUG2_PRINT("Touch Config values from SD file %s\n", m_fileName);
    printStoredValues();
    inform(); // let GUIslice know about our touch values
  } else {
    printDefinedValues();
  }
#elif defined(USE_EEPROM_INTERFACE)
  uint16_t test_magic;
  
  EEPROM.get(eeAddr, test_magic);
  if (test_magic == MAGIC_VALUE) {
    eeAddr+=sizeof(uint16_t);
    EEPROM.get(eeAddr, m_xMin);
    eeAddr+=sizeof(int16_t);
    EEPROM.get(eeAddr, m_xMax);
    eeAddr+=sizeof(int16_t);
    EEPROM.get(eeAddr, m_yMin);
    eeAddr+=sizeof(int16_t);
    EEPROM.get(eeAddr, m_yMax);
    eeAddr+=sizeof(int16_t);
    EEPROM.get(eeAddr, m_remapYX);
    CALIB_DEBUG2_PRINT("Touch Config values from EEPROM\n", "");
    printStoredValues();
    inform(); // let GUIslice know about our touch values
  } else {
    printDefinedValues();
  }
#else //using Flash Storage
  m_initial_Calib = flsh_initial_Calib.read(); //save calibration flag to variable
  if (m_initial_Calib == true) //check if initial calibration during runtime has been performed
  {
    m_xMin = flsh_xMin.read(); //read xMin flash value from storage
    m_xMax = flsh_xMax.read(); //read xMax flash value from storage
    m_yMin = flsh_yMin.read(); //read yMin flash value from storage
    m_yMax = flsh_yMax.read(); //read yMax flash value from storage
    m_remapYX = flsh_remapYX.read(); //read remapXy value from storage
    CALIB_DEBUG2_PRINT("Touch Config values from Flash\n", "");
    printStoredValues();
    inform();
  }
  else
  {
    printDefinedValues();
  }
#endif
}

void TFT_Calibration::update() {
  // just jump to guislice update if we are not running calibration
  if (isActive()) {
    loop();
  } else {
    gslc_Update(m_pGui);
  }
  
}

void TFT_Calibration::run() {
  // begin by initializing our control variables
  m_nPointFinalX = -1;
  m_nPointFinalY = -1;
  m_bTouchCalFail = false;
  m_bRemapYX = false;
  m_bTouchCoordValid = false;
  m_nPointBufX = 0;
  m_nPointBufY = 0;
  m_nPointBufCnt = 0;
  
  // state machine initializtion
  m_eState = STATE_CAL_START_MSG;
  m_eStateOld = STATE_CAL_UNDEF;
  m_eDebounce = DEBOUNCE_UP;
  m_eCalibPt = POINT_TL;
  m_eRotSpecial = E_ROT_UNDEF;
  m_bStateBegin = true;
  
  // Define calibration sequence
  m_asCalibStage[0].bXLeft = true;
  m_asCalibStage[0].bYTop = true;
  m_asCalibStage[0].eStateDo = STATE_CAPT_TL_DO;
  m_asCalibStage[0].eStateNxt = STATE_CAPT_TR_MSG;
  m_asCalibStage[1].bXLeft = false;
  m_asCalibStage[1].bYTop = true;
  m_asCalibStage[1].eStateDo = STATE_CAPT_TR_DO;
  m_asCalibStage[1].eStateNxt = STATE_CAPT_BL_MSG;
  m_asCalibStage[2].bXLeft = true;
  m_asCalibStage[2].bYTop = false;
  m_asCalibStage[2].eStateDo = STATE_CAPT_BL_DO;
  m_asCalibStage[2].eStateNxt = STATE_CAPT_BR_MSG;
  m_asCalibStage[3].bXLeft = false;
  m_asCalibStage[3].bYTop = false;
  m_asCalibStage[3].eStateDo = STATE_CAPT_BR_DO;
  m_asCalibStage[3].eStateNxt = STATE_CAPT_ROT;
  m_asCalibStage[4].bXLeft = true;
  m_asCalibStage[4].bYTop = true;
  m_asCalibStage[4].eStateDo = STATE_CAPT_ROT_TL_DO;
  m_asCalibStage[4].eStateNxt = STATE_CAL_REPORT;
   
  // Initialize settings and report
  #if defined(DO_CALIB)
    CALIB_DEBUG_PRINT("\n=== Touch Calibration ===\n\n", "");
  #else
    CALIB_DEBUG_PRINT("\n=== Touch Testing ===\n\n", "");
  #endif
  // Reset to calibration defaults from configuration
  m_nTouchCalXMin = ADATOUCH_X_MIN;
  m_nTouchCalXMax = ADATOUCH_X_MAX;
  m_nTouchCalYMin = ADATOUCH_Y_MIN;
  m_nTouchCalYMax = ADATOUCH_Y_MAX;
  #if defined(ADATOUCH_REMAP_YX)
    m_bRemapYX = ADATOUCH_REMAP_YX;
  #else
    m_bRemapYX = false;
  #endif

  // ---------------------------------------------------------
  // set new current page to our non-functional popup page
  gslc_PopupShow(m_pGui, m_popupPage, true);
  
  // Rotate the display to ROTATE=0 native orientation
  // can't use gslc_GuiRotate(m_pGui, 0) since it causes
  // a screen refresh
  gslc_DrvRotate(m_pGui,0);

  m_bStartTimer = true;
  m_bIsActive = true;
  loop();
}

void TFT_Calibration::restore() {
  m_xMin = ADATOUCH_X_MIN;
  m_xMax = ADATOUCH_X_MAX;
  m_yMin = ADATOUCH_Y_MIN;
  m_yMax = ADATOUCH_Y_MAX;
  #if defined(ADATOUCH_REMAP_YX)
    m_remapYX = ADATOUCH_REMAP_YX;
  #else
    m_remapYX = 0;
  #endif
  inform(); // let GUIslice know about our touch values

}

void TFT_Calibration::loop() {

  bool bOk;
  int16_t               nTouchX = 0;
  int16_t               nTouchY = 0;
  uint16_t              nTouchZ = 0;
  bool                  bTouchEvent = false;
  gslc_teInputRawEvent  eInputEvent = GSLC_INPUT_NONE;
  int16_t               nInputVal = 0;

  // Perform touch tracking
  bTouchEvent = gslc_GetTouch(m_pGui, &nTouchX, &nTouchY, &nTouchZ, &eInputEvent, &nInputVal);

  if (eInputEvent != GSLC_INPUT_TOUCH) {
    // Just in case we received another type of event
    bTouchEvent = false;
  }
  else {
    // Set the flag indicating that we have at least one
    // valid reading. This just affects our display.
    m_bTouchCoordValid = true;
  }

  // Detect transition events
  bool bTouchDown = (bTouchEvent && (nTouchZ > 0));
  bool bTouchUp = (bTouchEvent && (nTouchZ == 0));
  bool bFiltDown = false;
  bool bFiltUp = false;

  // Perform debouncing
  doDebounce(bTouchDown, bTouchUp, bFiltDown, bFiltUp);

  // Step through the FSM
  doFsm(bFiltDown, bFiltUp, nTouchX, nTouchY, nTouchZ);

}

// Take raw input down / up events and update with debounced versions
void TFT_Calibration::doDebounce(bool bRawDown, bool bRawUp, bool &bFiltDown, bool &bFiltUp)
{
  static uint32_t nTmStart = 0; // Timestamp of last transition event
  bool bHeld = (millis() - nTmStart) > DEBOUNCE_DELAY;

  switch (m_eDebounce) {
  case DEBOUNCE_UP:
    if (bRawDown) {
      nTmStart = millis(); // Latch timestamp for start of transition
      m_eDebounce = DEBOUNCE_UP2DOWN;
    } else if (bRawUp) {
      // Don't provide any touch events while continuing to be up
    }
    break;
  case DEBOUNCE_UP2DOWN:
    if (bRawUp) {
      m_eDebounce = DEBOUNCE_UP;
      CALIB_DEBUG2_PRINT("DBG: Touch bounce (up-down-up) detected\n", "");
    } else if (bHeld) {
      m_eDebounce = DEBOUNCE_DOWN; // Transition confirmed
      bFiltDown = true;
    }
    break;
  case DEBOUNCE_DOWN:
    if (bRawUp) {
      m_eDebounce = DEBOUNCE_DOWN2UP;
      nTmStart = millis(); // Latch timestamp for start of transition
    } else if (bRawDown) {
      // Provide continuous touch events while continuing to be down
      bFiltDown = true;
    }
    break;
  case DEBOUNCE_DOWN2UP:
    if (bRawDown) {
      m_eDebounce = DEBOUNCE_DOWN;
      CALIB_DEBUG2_PRINT("DBG: Touch bounce (down-up-down) detected\n", "");
    } else if (bHeld) {
      m_eDebounce = DEBOUNCE_UP; // Transition confirmed
      bFiltUp = true;
      nTmStart = millis(); // Latch timestamp for start of transition
    }
    break;
  default:
    break;
  }

}

// Implement State Machine
void TFT_Calibration::doFsm(bool bTouchDown, bool bTouchUp, int16_t nTouchX, int16_t nTouchY, uint16_t nTouchZ)
{
  // Save state
  m_eStateOld = m_eState;

  // ----------------------------------------------------------------------------
  // CALIB:
  // ----------------------------------------------------------------------------
  tsCalibStage sCalibStage;
  bool bSegsOk = false;

  // Determine which calibration point we are handling
  switch (m_eState) {
  case STATE_CAPT_TL_MSG:
  case STATE_CAPT_TL_DO:
    m_eCalibPt = POINT_TL;
    break;
  case STATE_CAPT_TR_MSG:
  case STATE_CAPT_TR_DO:
    m_eCalibPt = POINT_TR;
    break;
  case STATE_CAPT_BL_MSG:
  case STATE_CAPT_BL_DO:
    m_eCalibPt = POINT_BL;
    break;
  case STATE_CAPT_BR_MSG:
  case STATE_CAPT_BR_DO:
    m_eCalibPt = POINT_BR;
    break;
  case STATE_CAPT_ROT_TL_MSG:
  case STATE_CAPT_ROT_TL_DO:
    m_eCalibPt = POINT_TL_ROT;
    break;

  default:
    m_eCalibPt = POINT_TEST;
    break;
  } // m_eState
  // ----------------------------------------------------------------------------

  // Handle state machine
  switch (m_eState) {

  // ----------------------------------------------------------------------------
  // CALIB:
  // ----------------------------------------------------------------------------

  case STATE_CAL_START_MSG:
    if (m_bStateBegin) {
      drawBackground();

      CALIB_DEBUG2_PRINT("doFsm: m_bStateBegin\n","");
      resetDatapoints();

      // Initialize the calibration to arbitrary defaults
      resetCalib();
      resetMax();

      #if defined(CALC_EXP_AVG)
      CALIB_DEBUG_PRINT("CALIB: Averaging mode: EXP\n", "");
      #elif defined(CALC_BASIC_AVG)
      CALIB_DEBUG_PRINT("CALIB: Averaging mode: BASIC\n", "");
      #endif      
      CALIB_DEBUG_PRINT("\n", "");

      // Need to disable any touch remapping during calibration
      gslc_SetTouchRemapEn(m_pGui, false);
      gslc_SetTouchRemapYX(m_pGui, false);
    }
//    if (bTouchUp) { m_eState = STATE_CAPT_TL_MSG; }
     m_eState = STATE_CAPT_TL_MSG;
    break;

  case STATE_CAPT_TL_MSG:
  case STATE_CAPT_TR_MSG:
  case STATE_CAPT_BL_MSG:
  case STATE_CAPT_BR_MSG:
  case STATE_CAPT_ROT_TL_MSG:
    sCalibStage = m_asCalibStage[m_eCalibPt];
    if (m_bStateBegin) {
      drawBackground();
//      redrawStatus(g_CalibStageStatus[m_eCalibPt]);
      resetMax();
      resetDatapoints();
      m_bTouchCoordValid = false;
//      redrawCoords(0, 0, 0);
    }
    drawTarget(sCalibStage.bXLeft ? TARGET_MARGIN : (m_pGui->nDispW - TARGET_MARGIN),
      sCalibStage.bYTop ? TARGET_MARGIN : (m_pGui->nDispH - TARGET_MARGIN),
      TARGET_SIZE, GSLC_COL_GREEN, GSLC_COL_GREEN_LT2);
    if (bTouchDown) { m_eState = sCalibStage.eStateDo; }
    break;

  case STATE_CAPT_TL_DO:
  case STATE_CAPT_TR_DO:
  case STATE_CAPT_BL_DO:
  case STATE_CAPT_BR_DO:
  case STATE_CAPT_ROT_TL_DO:
    sCalibStage = m_asCalibStage[m_eCalibPt];
    if (bTouchDown) {
      if (m_bStartTimer) {
        m_nTmHold = millis();
        m_bStartTimer = false;
      }
      if ((millis() - m_nTmHold) >= HOLD_DELAY) {
        eraseTarget(sCalibStage.bXLeft ? TARGET_MARGIN : (m_pGui->nDispW - TARGET_MARGIN),
          sCalibStage.bYTop ? TARGET_MARGIN : (m_pGui->nDispH - TARGET_MARGIN),
          TARGET_SIZE);
      } else {
        calcMaxCoords(nTouchX, nTouchY, nTouchZ);
//        redrawCoords(nTouchX, nTouchY, nTouchZ);
        addDatapoint(nTouchX, nTouchY);
      }
    }
    if (bTouchUp) {
      m_bStartTimer = true;
      calcDatapoint(m_eCalibPt);
      CALIB_DEBUG2_PRINT("%s: X=%d Y=%d Cnt=%u, Z=(%d..%d)\n", sCalibStage.acStr, m_nPointFinalX, m_nPointFinalY, m_nPointBufCnt,
        m_nTouchZMin, m_nTouchZMax);
      m_eState = sCalibStage.eStateNxt;
    }
    break;

  case STATE_CAPT_ROT:
    // Calculate calibration
    bSegsOk = calcSegments();
	(void)bSegsOk; // Unused
    calcCalib();


    // Reset the background
    drawBackground();

    // Rotate the display to ROTATE=1
    //
    // This step detects how the display rotation behaves (ie. rotates clockwise or
    // counter-clockwise) by asking for an additional calibration point
    // after rotating the display between orientation 0 to orientation 1.
    // Depending on the quadrant that the user selects in this final step,
    // a determination can be made as to the rotation direction and
    // an additional rotation config setting can be reported.
    gslc_GuiRotate(m_pGui, 1);

    m_eState = STATE_CAPT_ROT_TL_MSG;
    break;

  case STATE_CAL_REPORT:
    // Redraw to cleanup markers
    drawBackground();

    m_bTouchCoordValid = false;

    // Calculate the calibration settings based on the
    // rotated orientation (ROTATE=1).
    calcCalibRotate();

    reportCalibResult();
    drawCalibResult();

    m_eState = STATE_CAL_DONE_MSG;
    // restore rotation to starting value
    gslc_GuiRotate(m_pGui, m_nRotation);
    gslc_PopupHide(m_pGui);
    m_bIsActive = false;
    break;

  case STATE_CAL_DONE_MSG:
    if (bTouchDown) { m_eState = STATE_CAL_DONE_END; }
    break;
  case STATE_CAL_DONE_END:
    break;

  // ----------------------------------------------------------------------------
  default:
    break;
  } // case(m_eState)

  // Are we going to enter a new state?
  // - This is used to limit certain behavior to state entry
  m_bStateBegin = (m_eState != m_eStateOld);
}

void TFT_Calibration::storeValues() {
#if defined(USE_SD_INTERFACE)
  File values = SD.open(m_fileName, FILE_WRITE);
  values.write((byte*)&m_xMin, sizeof(int16_t));
  values.write((byte*)&m_xMax, sizeof(int16_t));
  values.write((byte*)&m_yMin, sizeof(int16_t));
  values.write((byte*)&m_yMax, sizeof(int16_t));
  values.write((byte*)&m_remapYX, sizeof(int16_t));
  values.close();
  CALIB_DEBUG_PRINT("Touch Calibration values written to SD file %s\n", m_fileName);
#elif defined(USE_EEPROM_INTERFACE)
  int eeAddr = m_eeAddr;
  EEPROM.put(eeAddr, MAGIC_VALUE);
  eeAddr += sizeof(uint16_t);
  EEPROM.put(eeAddr, m_xMin);
  eeAddr += sizeof(int16_t);
  EEPROM.put(eeAddr, m_xMax);
  eeAddr += sizeof(int16_t);
  EEPROM.put(eeAddr, m_yMin);
  eeAddr += sizeof(int16_t);
  EEPROM.put(eeAddr, m_yMax);
  eeAddr += sizeof(int16_t);
  EEPROM.put(eeAddr, m_remapYX);
  
  CALIB_DEBUG_PRINT("Touch Calibration values written EEPROM address %d\n", m_eeAddr);

#else ///using Flash Storage
  m_initial_Calib = true; //initial runtime calibration was succesful, write flag to true
  flsh_xMin.write(m_xMin); //write Xmin value to Flash
  flsh_xMax.write(m_xMax); //write Xmax value to Flash
  flsh_yMin.write(m_yMin); //write Ymin value to Flash
  flsh_yMax.write(m_yMax); //write Ymax value to Flash
  flsh_remapYX.write(m_remapYX); //write remapYX value to Flash
  flsh_initial_Calib.write(m_initial_Calib); //write initial calibration flag to flash

  CALIB_DEBUG_PRINT("Touch Calibration values written to FLASH STORAGE\n", "");
#endif
}

void TFT_Calibration::inform() {
  gslc_SetTouchRemapCal(m_pGui, m_xMin, m_xMax, m_yMin, m_yMax);
  gslc_SetTouchRemapYX(m_pGui, m_remapYX);
  gslc_SetTouchRemapEn(m_pGui, true);
  CALIB_DEBUG2_PRINT("GUIslice has been informed of our Touch Calibration values\n", "");
}

void TFT_Calibration::printStoredValues() {
  CALIB_DEBUG2_PRINT("xMin=%d\n", m_xMin);
  CALIB_DEBUG2_PRINT("xMax=%d\n", m_xMax);
  CALIB_DEBUG2_PRINT("yMin=%d\n", m_yMin);
  CALIB_DEBUG2_PRINT("yMax=%d\n", m_yMax);
  CALIB_DEBUG2_PRINT("remapYX=%d\n",m_remapYX);
}

void TFT_Calibration::serialPrintStoredValues(){
  Serial.print("xMin = ");
  Serial.println(m_xMin);
  Serial.print("xMax = ");
  Serial.println(m_xMax);
  Serial.print("yMin = ");
  Serial.println(m_yMin);
  Serial.print("yMax = ");
  Serial.println(m_yMax);
}

void TFT_Calibration::serialPrintDefinedValues(){
  Serial.print("ADATOUCH xMin = ");
  Serial.println(ADATOUCH_X_MIN);
  Serial.print("ADATOUCH xMax = ");
  Serial.println(ADATOUCH_X_MAX);
  Serial.print("ADATOUCH yMin = ");
  Serial.println(ADATOUCH_Y_MIN);
  Serial.print("ADATOUCH yMax = ");
  Serial.println(ADATOUCH_Y_MAX);
}

void TFT_Calibration::printDefinedValues() {
  CALIB_DEBUG2_PRINT("FYI The #Defines for Touch in Config file\n","");
  CALIB_DEBUG2_PRINT("ADATOUCH_X_MIN=%d\n", ADATOUCH_X_MIN);
  CALIB_DEBUG2_PRINT("ADATOUCH_X_MAX=%d\n", ADATOUCH_X_MAX);
  CALIB_DEBUG2_PRINT("ADATOUCH_Y_MIN=%d\n", ADATOUCH_Y_MIN);
  CALIB_DEBUG2_PRINT("ADATOUCH_Y_MAX=%d\n", ADATOUCH_Y_MAX);
  #if defined(ADATOUCH_REMAP_YX)
    CALIB_DEBUG2_PRINT("ADATOUCH_REMAP_YX=%d\n",ADATOUCH_REMAP_YX);
  #else
    CALIB_DEBUG2_PRINT("ADATOUCH_REMAP_YX=0\n","");
  #endif
}

void TFT_Calibration::drawBackground() {
  gslc_tsRect rBack = (gslc_tsRect) { 0, 0, 0, 0 };
  rBack.w = m_pGui->nDispW;
  rBack.h = m_pGui->nDispH;
  gslc_DrawFillRect(m_pGui, rBack, GSLC_COL_BLACK);
}

// ----------------------------------------------------------------------------
// CALIB:
// ----------------------------------------------------------------------------
void TFT_Calibration::resetCalib() {
  // Reset the calibration ranges to enable min/max determination
  m_nTouchCalXMin = 9999;
  m_nTouchCalXMax = 0;
  m_nTouchCalYMin = 9999;
  m_nTouchCalYMax = 0;

  for (int nInd = 0; nInd < POINT__NUM; nInd++) {
    m_anTouchXPeak[nInd] = 0;
    m_anTouchYPeak[nInd] = 0;
  }

  m_eRotSpecial = E_ROT_UNDEF;
}

void TFT_Calibration::resetDatapoints()
{
  m_nPointBufX = 0;
  m_nPointBufY = 0;
  m_nPointBufCnt = 0;
}

void TFT_Calibration::addDatapoint(int16_t nTouchX, int16_t nTouchY) {
  #if defined(CALC_EXP_AVG)
  // Exponential moving average
  m_nPointBufX = ((EXP_AVG_NUMER)* m_nPointBufX / EXP_AVG_DENOM) + (EXP_AVG_FIXPT * (EXP_AVG_DENOM - EXP_AVG_NUMER) * nTouchX / EXP_AVG_DENOM);
  m_nPointBufY = ((EXP_AVG_NUMER)* m_nPointBufY / EXP_AVG_DENOM) + (EXP_AVG_FIXPT * (EXP_AVG_DENOM - EXP_AVG_NUMER) * nTouchY / EXP_AVG_DENOM);
  m_nPointBufCnt++;
  #elif defined(CALC_BASIC_AVG)
  // Simple averaging
  // - Avoid integer overflow
  if (m_nPointBufCnt < 10000) {
    m_nPointBufX += nTouchX;
    m_nPointBufY += nTouchY;
    m_nPointBufCnt++;
  }
  #endif
}

void TFT_Calibration::calcDatapoint(teCalibPt ePoint) {
  #if defined(CALC_EXP_AVG)
  m_nPointFinalX = m_nPointBufX / EXP_AVG_FIXPT;
  m_nPointFinalY = m_nPointBufY / EXP_AVG_FIXPT;
  #elif defined(CALC_BASIC_AVG)
  if (m_nPointBufCnt > 0) {
    m_nPointFinalX = m_nPointBufX / m_nPointBufCnt;
    m_nPointFinalY = m_nPointBufY / m_nPointBufCnt;
  }
  #endif  

  // Record the final value in the corresponding touch point
  m_anTouchXPeak[ePoint] = m_nPointFinalX;
  m_anTouchYPeak[ePoint] = m_nPointFinalY;

}

bool TFT_Calibration::calcSegments() {
  // Algorithm:
  // - 1) Calculate min & max of X & Y coordinates
  // - 2) Segment X & Y ranges into thirds (Low,Mid,High), making
  //      a total of 9 segments when using both X & Y together
  // - 3) Classify each touch press into one of the 9 segments
  // - 4) Determine if all corner segments (LL, HL, LH, HH) have
  //      been detected in the 4 points. If not, error.
  // - 5) Identify the mapping type based on the segment assignments.

  int16_t nXMin = 9999;
  int16_t nXMax = 0;
  int16_t nYMin = 9999;
  int16_t nYMax = 0;

  int16_t nXRng = 1;
  int16_t nYRng = 1;
  int16_t nXEdgeLM = 0;
  int16_t nXEdgeMH = 0;
  int16_t nYEdgeLM = 0;
  int16_t nYEdgeMH = 0;
  teCalibSeg aePointSeg[4];

  // Step 1:
  for (unsigned nInd = POINT_TL; nInd <= POINT_BR; nInd++) {
    nXMin = (m_anTouchXPeak[nInd] < nXMin) ? m_anTouchXPeak[nInd] : nXMin;
    nXMax = (m_anTouchXPeak[nInd] > nXMax) ? m_anTouchXPeak[nInd] : nXMax;
    nYMin = (m_anTouchYPeak[nInd] < nYMin) ? m_anTouchYPeak[nInd] : nYMin;
    nYMax = (m_anTouchYPeak[nInd] > nYMax) ? m_anTouchYPeak[nInd] : nYMax;
  }

  // Step 2:
  nXRng = nXMax - nXMin;
  nYRng = nYMax - nYMin;
  nXEdgeLM = 1*(nXRng / 3);
  nXEdgeMH = 2*(nXRng / 3);
  nYEdgeLM = 1*(nYRng / 3);
  nYEdgeMH = 2*(nYRng / 3);
  // Check for very small ranges
  if ((nXRng < SEG_MIN_RANGE) || (nYRng < SEG_MIN_RANGE)) {
    CALIB_DEBUG_PRINT("ERROR: Small range detected (XRng=%d YRng=%d)\n", nXRng, nYRng);
    return false;
  }

  // Step 3:
  int16_t nX, nY, nSeg;
  for (unsigned nInd = POINT_TL; nInd <= POINT_BR; nInd++) {
    nX = m_anTouchXPeak[nInd];
    nY = m_anTouchYPeak[nInd];
    nSeg = 0;
    // Shortcut math to identify all 9 segs:
    if (nY < nYEdgeLM) {
      nSeg = 0;
    } else if (nY < nYEdgeMH) {
      nSeg = 3;
    } else {
      nSeg = 6;
    }
    if (nX < nXEdgeLM) {
      nSeg += 0;
    } else if (nX < nXEdgeMH) {
      nSeg += 1;
    } else {
      nSeg += 2;
    }
    aePointSeg[nInd] = static_cast<teCalibSeg>(nSeg);
  }

  // Step 4:
  uint8_t nSegsFound = 0;
  for (unsigned nInd = POINT_TL; nInd <= POINT_BR; nInd++) {
    if (aePointSeg[nInd] == E_SEG_LL) {
      nSegsFound |= 1;
    } else if (aePointSeg[nInd] == E_SEG_HL) {
      nSegsFound |= 2;
    } else if (aePointSeg[nInd] == E_SEG_LH) {
      nSegsFound |= 4;
    } else if (aePointSeg[nInd] == E_SEG_HH) {
      nSegsFound |= 8;
    }
  }
  if (nSegsFound != 15) {
    CALIB_DEBUG_PRINT("ERROR: Missing corner segment (status=%u)\n", nSegsFound);
    return false;
  }

  // Step 5:
  teCalibSeg segTL, segTR, segBL, segBR;
  m_bRemapYX = false;
  segTL = aePointSeg[POINT_TL];
  segTR = aePointSeg[POINT_TR];
  segBL = aePointSeg[POINT_BL];
  segBR = aePointSeg[POINT_BR];
  if ((segTL == E_SEG_LL) && (segTR == E_SEG_HL) && (segBL == E_SEG_LH) && (segBR == E_SEG_HH)) {
    CALIB_DEBUG_PRINT("SegDetect: Normal display\n", "");
  } else if ((segTL == E_SEG_HL) && (segTR == E_SEG_LL) && (segBL == E_SEG_HH) && (segBR == E_SEG_LH)) {
    CALIB_DEBUG_PRINT("SegDetect: Normal display (FlipX)\n", "");
  } else if ((segTL == E_SEG_LH) && (segTR == E_SEG_HH) && (segBL == E_SEG_LL) && (segBR == E_SEG_HL)) {
    CALIB_DEBUG_PRINT("SegDetect: Normal display (FlipY)\n", "");
  } else if ((segTL == E_SEG_HH) && (segTR == E_SEG_LH) && (segBL == E_SEG_HL) && (segBR == E_SEG_LL)) {
    CALIB_DEBUG_PRINT("SegDetect: Normal display (FlipX,FlipY)\n", "");
  } else if ((segTL == E_SEG_LL) && (segTR == E_SEG_LH) && (segBL == E_SEG_HL) && (segBR == E_SEG_HH)) {
    CALIB_DEBUG_PRINT("SegDetect: RemapYX display\n", "");
    m_bRemapYX = true;
  } else if ((segTL == E_SEG_LH) && (segTR == E_SEG_LL) && (segBL == E_SEG_HH) && (segBR == E_SEG_HL)) {
    CALIB_DEBUG_PRINT("SegDetect: RemapYX display (FlipX)\n", "");
    m_bRemapYX = true;
  } else if ((segTL == E_SEG_HL) && (segTR == E_SEG_HH) && (segBL == E_SEG_LL) && (segBR == E_SEG_LH)) {
    CALIB_DEBUG_PRINT("SegDetect: RemapYX display (FlipY)\n", "");
    m_bRemapYX = true;
  } else if ((segTL == E_SEG_HH) && (segTR == E_SEG_HL) && (segBL == E_SEG_LH) && (segBR == E_SEG_LL)) {
    CALIB_DEBUG_PRINT("SegDetect: RemapYX display (FlipX,FlipY)\n", "");
    m_bRemapYX = true;
  } else {
    CALIB_DEBUG_PRINT("SegDetect: Unknown display orientation (%d,%d,%d,%d)\n",
      segTL, segTR, segBL, segBR);
    return false;
  }

  // If m_bRemapYX is set, then we will request the following line be added
  // to the user configuration. This is displayed in the final report.
  //   #define ADATOUCH_REMAP_YX 1

  // If we determined that the X & Y were swapped (even in the native orientation)
  // then we must update the calibration points we have collected so far to adjust
  // for this so that all further calculations account for this remapping.
  if (m_bRemapYX) {
    for (unsigned nInd = POINT_TL; nInd <= POINT_BR; nInd++) {
      nX = m_anTouchXPeak[nInd];
      nY = m_anTouchYPeak[nInd];
      m_anTouchXPeak[nInd] = nY;
      m_anTouchYPeak[nInd] = nX;
    }
  }

  // From this point onwards, we want to use the XY coordinates taking
  // into account any swapping detected above. This way, when we
  // capture coordinates from additional calibration points (eg. rotated view),
  // the coordinates will be remapped as needed.
  gslc_SetTouchRemapYX(m_pGui, m_bRemapYX);

  return true;
}

void TFT_Calibration::calcCalib() {
  float   fSx, fSy;
  int16_t nXMin, nXMax, nYMin, nYMax;
  int16_t nXd, nYd;
  int16_t nPixXd, nPixYd;

  // Calculate calibration ranges based on averaging of raw target values
  // - Assume calibration was done in native orientation
  // - Assume pins XP/XM/YP/YM have been assigned correctly
  m_nTouchCalXMinRaw = (m_anTouchXPeak[POINT_TL] + m_anTouchXPeak[POINT_BL]) / 2;
  m_nTouchCalXMaxRaw = (m_anTouchXPeak[POINT_TR] + m_anTouchXPeak[POINT_BR]) / 2;
  m_nTouchCalYMinRaw = (m_anTouchYPeak[POINT_TL] + m_anTouchYPeak[POINT_TR]) / 2;
  m_nTouchCalYMaxRaw = (m_anTouchYPeak[POINT_BL] + m_anTouchYPeak[POINT_BR]) / 2;

  m_nTouchCalXRngRaw = m_nTouchCalXMaxRaw - m_nTouchCalXMinRaw;
  m_nTouchCalYRngRaw = m_nTouchCalYMaxRaw - m_nTouchCalYMinRaw;
  m_nTouchCalXRngRaw = abs(m_nTouchCalXRngRaw);
  m_nTouchCalYRngRaw = abs(m_nTouchCalYRngRaw);

  // If the pin configuration was incorrect, then the Left-Right or Top-Bottom
  // ranges could be close to zero. Detect this condition and report it
  if ((m_nTouchCalXRngRaw < DET_RAW_RANGE_MIN) || (m_nTouchCalYRngRaw < DET_RAW_RANGE_MIN)) {
    m_bTouchCalFail = true;
  }
  // Similarly, if there was a large variance between the points being averaged
  // (X:TL/BL, X:TR/BR, Y:TL/TR, Y:BL/BR), then it suggests we might also have
  // a bad pin configuration (eg. perhaps pins were swapped).
  int16_t nVarXMinRaw = (m_anTouchXPeak[POINT_TL] - m_anTouchXPeak[POINT_BL]);
  int16_t nVarXMaxRaw = (m_anTouchXPeak[POINT_TR] - m_anTouchXPeak[POINT_BR]);
  int16_t nVarYMinRaw = (m_anTouchYPeak[POINT_TL] - m_anTouchYPeak[POINT_TR]);
  int16_t nVarYMaxRaw = (m_anTouchYPeak[POINT_BL] - m_anTouchYPeak[POINT_BR]);
  nVarXMinRaw = abs(nVarXMinRaw);
  nVarXMaxRaw = abs(nVarXMaxRaw);
  nVarYMinRaw = abs(nVarYMinRaw);
  nVarYMaxRaw = abs(nVarYMaxRaw);
  if ((nVarXMinRaw > DET_RAW_VAR_MAX) || (nVarXMaxRaw > DET_RAW_VAR_MAX) || \
      (nVarYMinRaw > DET_RAW_VAR_MAX) || (nVarYMaxRaw > DET_RAW_VAR_MAX)) {
    m_bTouchCalFail = true;
  }

  if (m_bTouchCalFail) {
    CALIB_DEBUG_PRINT("\nERROR: Calibration failed. Please check touch pin config ADATOUCH_PIN_*\n\n", "");
  }

  // Apply correction factor for targets not at corners

  // Report raw calibration range prior to adjustment
  CALIB_DEBUG2_PRINT("CALIB: Result(raw) XMin=%d XMax=%d Ymin=%d YMax=%d\n",
    m_nTouchCalXMinRaw, m_nTouchCalXMaxRaw, m_nTouchCalYMinRaw, m_nTouchCalYMaxRaw);
  CALIB_DEBUG2_PRINT("CALIB: Range X=%d Y=%d. Expect > %d\n",
    m_nTouchCalXRngRaw, m_nTouchCalYRngRaw, DET_RAW_RANGE_MIN);
  CALIB_DEBUG2_PRINT("CALIB: Var XMin=%d XMax=%d YMin=%d YMax=%d. Expect < %d\n",
    nVarXMinRaw, nVarXMaxRaw, nVarYMinRaw, nVarYMaxRaw, DET_RAW_VAR_MAX);

  // Calculate the range in touch readings & range in pixel coordinates
  nXd = (m_nTouchCalXMaxRaw - m_nTouchCalXMinRaw);
  nYd = (m_nTouchCalYMaxRaw - m_nTouchCalYMinRaw);
  nPixXd = (m_pGui->nDisp0W - 2 * TARGET_MARGIN);
  nPixYd = (m_pGui->nDisp0H - 2 * TARGET_MARGIN);

  // Determine mapping slope
  fSx = (float)nXd / (float)nPixXd;
  fSy = (float)nYd / (float)nPixYd;

  // Extrapolate range in touch readings (at corners)
  nXMin = m_nTouchCalXMinRaw - (fSx * TARGET_MARGIN);
  nYMin = m_nTouchCalYMinRaw - (fSy * TARGET_MARGIN);
  nXMax = m_nTouchCalXMaxRaw + (fSx * TARGET_MARGIN);
  nYMax = m_nTouchCalYMaxRaw + (fSy * TARGET_MARGIN);

  // Assign the adjusted calibration ranges
  m_nTouchCalXMin = nXMin;
  m_nTouchCalYMin = nYMin;
  m_nTouchCalXMax = nXMax;
  m_nTouchCalYMax = nYMax;

  CALIB_DEBUG2_PRINT("CALIB: Result(adj) XMin=%d XMax=%d YMin=%d YMax=%d\n",
    m_nTouchCalXMin, m_nTouchCalXMax, m_nTouchCalYMin, m_nTouchCalYMax);

}

teCalibPt TFT_Calibration::findClosestPt(int16_t nX, int16_t nY) {
  teCalibPt ePoint = POINT_TEST;
  int16_t nDeltaMinX, nDeltaMaxX, nDeltaMinY, nDeltaMaxY;
  /*
    nDeltaMinX = abs(m_nTouchCalXMin - nX);
    nDeltaMaxX = abs(m_nTouchCalXMax - nX);
    nDeltaMinY = abs(m_nTouchCalYMin - nY);
    nDeltaMaxY = abs(m_nTouchCalYMax - nY);
  */
  nDeltaMinX = m_nTouchCalXMinRaw - nX;
  nDeltaMaxX = m_nTouchCalXMaxRaw - nX;
  nDeltaMinY = m_nTouchCalYMinRaw - nY;
  nDeltaMaxY = m_nTouchCalYMaxRaw - nY;
  nDeltaMinX = abs(nDeltaMinX);
  nDeltaMaxX = abs(nDeltaMaxX);
  nDeltaMinY = abs(nDeltaMinY);
  nDeltaMaxY = abs(nDeltaMaxY);

  // Calculate tolerance for corner detection
  int16_t nTolerX;
  int16_t nTolerY;
  nTolerX = m_nTouchCalXRngRaw * DET_ROT_TOLER_PCT / 100;
  nTolerY = m_nTouchCalYRngRaw * DET_ROT_TOLER_PCT / 100;

    CALIB_DEBUG2_PRINT("ROT_FIND: calib_raw MinX=%d MaxX=%d MinY=%d MaxY=%d\n",
      m_nTouchCalXMinRaw, m_nTouchCalXMaxRaw, m_nTouchCalYMinRaw, m_nTouchCalYMaxRaw);
    CALIB_DEBUG2_PRINT("ROT_FIND: rotate    X=%d Y=%d\n", nX, nY);
    CALIB_DEBUG2_PRINT("ROT_FIND: tolerance X=%d Y=%d\n", nTolerX, nTolerY);
    CALIB_DEBUG2_PRINT("ROT_FIND: delta     MinX= %d MaxX= %d MinY= %d MaxY= %d\n",
      nDeltaMinX, nDeltaMaxX, nDeltaMinY, nDeltaMaxY);

  // Locate nearest calibration point that is within the tolerance zone
  if ((nDeltaMinX < nDeltaMaxX) && (nDeltaMinX < nTolerX)) {
    if ((nDeltaMinY < nDeltaMaxY) && (nDeltaMinY < nTolerY)) {
      ePoint = POINT_TL;
    }
    else if ((nDeltaMaxY < nDeltaMinY) && (nDeltaMaxY < nTolerY)) {
      ePoint = POINT_BL;
    }
    else {
        CALIB_DEBUG2_PRINT("ROT_FIND: Closest not found: X close, Y far\n", "");
    }
  }
  else if ((nDeltaMaxX < nDeltaMinX) && (nDeltaMaxX < nTolerX)) {
    if ((nDeltaMinY < nDeltaMaxY) && (nDeltaMinY < nTolerY)) {
      ePoint = POINT_TR;
    }
    else if ((nDeltaMaxY < nDeltaMinY) && (nDeltaMaxY < nTolerY)) {
      ePoint = POINT_BR;
    }
    else {
        CALIB_DEBUG2_PRINT("ROT_FIND: Closest not found: X far, Y close\n", "");
    }
  }
  else {
    CALIB_DEBUG2_PRINT("ROT_FIND: Closest not found: X far, Y far\n", "");
  }

  #if DEBUG_CALIB == 2
    if (ePoint != POINT_TEST) {
      CALIB_DEBUG2_PRINT("ROT_FIND: Closest found: [%d]\n", ePoint);
    }
  #endif // DEBUG_CALIB

  return ePoint;
}

// Perform additional check to infer screen behavior as a
// result of screen rotation. The additional calibrattion
// point in ROTATE=1 mode enables us to see whether the
// display rotates as expected or whether the rotation direction
// is reversed and requires additional configuration.
void TFT_Calibration::calcCalibRotate() {
  m_eRotSpecial = E_ROT_UNDEF;

  // Is the remapped POINT_ROT_TL in the top-right corner? (near POINT_TR?)
  // Determine which of the four raw calibration points were closest to
  // rotated point. Depending on which point was detected, we can infer
  // how the display must have been rotated. This enables us to determine
  // if we need additional configuration to handle the various rotation modes.
  teCalibPt ePtClosest = findClosestPt(m_anTouchXPeak[POINT_TL_ROT], m_anTouchYPeak[POINT_TL_ROT]);

  switch (ePtClosest) {
  case POINT_TL:
    // No change! Display doesn't support rotation
    // NOTE: This case is expected on certain displays, but GUIslice
    //       does not currently disable rotation in this mode. For now
    //       users should not call GuiRotate() on these displays.
    m_eRotSpecial = E_ROT_NONE;
    break;
  case POINT_TR:
    // Normal expected rotation
    CALIB_DEBUG_PRINT("CALIB: Detected normal rotation\n", "");
    m_eRotSpecial = E_ROT_NORMAL;
    break;
  case POINT_BL:
    // Reversed rotation
    // NOTE: This case is not expected to be encountered and is not
    //       currently supported by GUIslice. However, if users report
    //       encountering displays with this case, support will be added.
    CALIB_DEBUG_PRINT("WARN: Detected reversed rotation. Please file GitHub issue.\n", "");
    m_eRotSpecial = E_ROT_REVERSE;
    break;
  case POINT_BR:
    // Unusual (swapped) rotation
    // NOTE: This case is not expected to be encountered and will not
    //       be handled by GUIslice
    CALIB_DEBUG_PRINT("ERROR: Detected unusual swapped rotation. Please file GitHub issue.\n", "");
    m_eRotSpecial = E_ROT_SWAP;
    break;
  case POINT_TEST:
  default:
    // No match found?
    CALIB_DEBUG_PRINT("ERROR: Could not detect rotation. Please retry calibration.\n", "");
    m_eRotSpecial = E_ROT_UNDEF;
    break;
  }

}

void TFT_Calibration::reportCalibResult() {
  CALIB_DEBUG_PRINT("\n\n", "");
  CALIB_DEBUG_PRINT("Recommended calibration settings are shown below.\n", "");

  CALIB_DEBUG_PRINT("---\n", "");

  // Report the display's native dimensions
  char acDim[12] = "";
  snprintf(acDim, 12, "[%ux%u]", m_pGui->nDisp0W, m_pGui->nDisp0H);

  // NOTE: Only need to handle resistive touch display modes
  #if defined(DRV_TOUCH_ADA_SIMPLE)
    CALIB_DEBUG_PRINT("  // DRV_TOUCH_ADA_SIMPLE %s: ", acDim);
    #if defined(DRV_DISP_ADAGFX_MCUFRIEND)
      // For MCUFRIEND displays, report the ID
      snprintf(m_acTxt, MY_STRING_SZ, "(MCUFRIEND ID=0x%04X)", m_nMcuFriendId);
      CALIB_DEBUG_PRINT("%s ", m_acTxt);
    #endif
    // Report the pin wiring to the 4-wire resistive interface
    #if !defined(A0)
      // Support MCUs that don't use analog pin "A0" notation (eg. STM32)
      CALIB_DEBUG_PRINT("(XP=%d,XM=%d,YP=%d,YM=%d) ", ADATOUCH_PIN_XP, ADATOUCH_PIN_XM, ADATOUCH_PIN_YP, ADATOUCH_PIN_YM);
    #else
      CALIB_DEBUG_PRINT("(XP=%u,XM=A%d,YP=A%d,YM=%u) ", ADATOUCH_PIN_XP, (ADATOUCH_PIN_XM - A0), (ADATOUCH_PIN_YP - A0), ADATOUCH_PIN_YM);
    #endif

    CALIB_DEBUG_PRINT("\n", "");
  #elif defined(DRV_TOUCH_ADA_STMPE610)
    CALIB_DEBUG_PRINT("  // DRV_TOUCH_ADA_STMPE610 %s:\n", acDim);
  #elif defined(DRV_TOUCH_XPT2046_PS)
    CALIB_DEBUG_PRINT("  // DRV_TOUCH_XPT2046_PS %s:\n", acDim);
  #elif defined(DRV_TOUCH_XPT2046_STM)
    CALIB_DEBUG_PRINT("  // DRV_TOUCH_XPT2046_STM %s:\n", acDim);
  #endif // DRV_TOUCH_ADA_*

  if (m_bTouchCalFail) {
    CALIB_DEBUG_PRINT("*** ERROR: Calibration failed. Please check serial monitor log.\n", "");
    return;
  }

  CALIB_DEBUG_PRINT("  X_MIN    %d\n", m_nTouchCalXMin);
  CALIB_DEBUG_PRINT("  X_MAX    %d\n", m_nTouchCalXMax);
  CALIB_DEBUG_PRINT("  Y_MIN    %d\n", m_nTouchCalYMin);
  CALIB_DEBUG_PRINT("  Y_MAX    %d\n", m_nTouchCalYMax);

  CALIB_DEBUG_PRINT("  REMAP_YX %u\n", m_bRemapYX);
  
  m_xMin = m_nTouchCalXMin;
  m_xMax = m_nTouchCalXMax;
  m_yMin = m_nTouchCalYMin;
  m_yMax = m_nTouchCalYMax;
  m_remapYX = m_bRemapYX ? 1 : 0;

  #if !defined(DRV_TOUCH_CALIB)
    // By default, this display may not require calibration (eg. capacitive touch),
    // but since the calibration sketch is being run, force the calibration
    // settings by enabling DRV_TOUCH_CALIB.
    //
    // Many capacitive displays do not require any calibration at all,
    // however some may have flipped X/Y axes. In this case, running the calibration
    // sketch can enable one to ensure the orientation is correct.
    // When adding the calibration settings to such configurations, one will
    // also need to enable DRV_TOUCH_CALIB to indicate that the
    // calibration settings are to be expected.
    CALIB_DEBUG_PRINT("Crap... You have run this code on a display\n","");
    CALIB_DEBUG_PRINT("that will ignore our new settings unless you modify\n","");
    CALIB_DEBUG_PRINT("your config/*.h files and add:\n","");
    CALIB_DEBUG_PRINT("  #define DRV_TOUCH_CALIB\n", "");
  #endif // DRV_TOUCH_CALIB
/*
  // This code for GSLC_ROTATE_SPECIAL and the various E_ROT_***
  // does not appear inside any GUIslice routines
  // so I (Paul Conti Mar 18 2024) have commented it out even though it seems like
  // it would be very useful.
  
  // Check for unexpected rotation behavior
  switch (m_eRotSpecial) {
  case E_ROT_NORMAL:
    // No need to add special config
    break;
  case E_ROT_REVERSE:
    CALIB_DEBUG_PRINT("  #define GSLC_ROTATE_SPECIAL %d // Reversed Rotation\n", m_eRotSpecial);
    break;
  case E_ROT_SWAP:
    CALIB_DEBUG_PRINT("  #define GSLC_ROTATE_SPECIAL %d // Swapped Rotation\n", m_eRotSpecial);
    break;
  case E_ROT_NONE:
    CALIB_DEBUG_PRINT("  #define GSLC_ROTATE_SPECIAL %d // No Rotation supported\n", m_eRotSpecial);
    break;
  case E_ROT_UNDEF:
  default:
    CALIB_DEBUG_PRINT("  // NOTE: Rotation type not detected properly\n", "");
    break;
  }
  CALIB_DEBUG_PRINT("---\n", "");

  if ((m_eRotSpecial == E_ROT_REVERSE) || (m_eRotSpecial == E_ROT_SWAP)) {
    CALIB_DEBUG_PRINT("*** Please file a GitHub issue to add support for this mode\n", "");
  }
*/
  CALIB_DEBUG_PRINT("\n", "");

}

void TFT_Calibration::drawCalibResult() {
  
  setStatus(CALIBR_SUCCESS);
  if (m_bTouchCalFail) {
    CALIB_DEBUG_PRINT("ERROR: See serial log/n", "");
    // restore the original values saved during init()
    gslc_SetTouchRemapEn(m_pGui, m_saved_bTouchRemapEn);
    gslc_SetTouchRemapYX(m_pGui, m_saved_bTouchRemapYX);
    setStatus(CALIBR_FAILURE);
    return;
  } else if (m_eRotSpecial == E_ROT_REVERSE) {
    CALIB_DEBUG_PRINT("ERROR: E_ROT_REVERSE See serial log/n","");
  } else if (m_eRotSpecial == E_ROT_SWAP) {
    CALIB_DEBUG_PRINT("ERROR: E_ROT_SWAP See serial log/n","");
  } else if (m_eRotSpecial == E_ROT_NONE) {
    CALIB_DEBUG_PRINT("No rotation supported\n", "");
  } else if (m_eRotSpecial == E_ROT_UNDEF) {
    CALIB_DEBUG_PRINT("ERROR: Could not detect rotation\n","");
  } else if (m_eRotSpecial == E_ROT_NORMAL) {
    CALIB_DEBUG_PRINT("Rotation detected OK\n", "");
  }
  storeValues(); // success
  inform();
}

// ----------------------------------------------------------------------------

void TFT_Calibration::resetMax() {
  m_nTouchXMin = 9999;
  m_nTouchXMax = 0;
  m_nTouchYMin = 9999;
  m_nTouchYMax = 0;
  m_nTouchZMin = 9999;
  m_nTouchZMax = 0;

  m_bTouchCoordValid = false;
}

void TFT_Calibration::drawTarget(int16_t nX, int16_t nY, uint16_t nSize, gslc_tsColor nColOut, gslc_tsColor nColIn) {
  gslc_tsRect rRect;
  rRect.x = nX - nSize;
  rRect.y = nY - nSize;
  rRect.w = nSize * 2;
  rRect.h = nSize * 2;
  gslc_DrawFrameRect(m_pGui, rRect, nColOut);
  gslc_DrawLine(m_pGui, nX - (nSize / 2), nY, nX + (nSize / 2) - 1, nY, nColIn);
  gslc_DrawLine(m_pGui, nX, nY - (nSize / 2), nX, nY + (nSize / 2) - 1, nColIn);
}

void TFT_Calibration::eraseTarget(int16_t nX, int16_t nY, uint16_t nSize) {
  gslc_tsRect rRect;
  rRect.x = nX - nSize;
  rRect.y = nY - nSize;
  rRect.w = nSize * 2;
  rRect.h = nSize * 2;
  gslc_DrawFillRect(m_pGui, rRect, GSLC_COL_BLACK);
}

void TFT_Calibration::calcMaxCoords(int16_t nTouchX, int16_t nTouchY, uint16_t nTouchZ) {
  // Capture maximums (for current step)
  m_nTouchXMin = (nTouchX < m_nTouchXMin) ? nTouchX : m_nTouchXMin;
  m_nTouchXMax = (nTouchX > m_nTouchXMax) ? nTouchX : m_nTouchXMax;
  m_nTouchYMin = (nTouchY < m_nTouchYMin) ? nTouchY : m_nTouchYMin;
  m_nTouchYMax = (nTouchY > m_nTouchYMax) ? nTouchY : m_nTouchYMax;
  m_nTouchZMin = ((int16_t)nTouchZ < m_nTouchZMin) ? (int16_t)nTouchZ : m_nTouchZMin;
  m_nTouchZMax = ((int16_t)nTouchZ > m_nTouchZMax) ? (int16_t)nTouchZ : m_nTouchZMax;
}
