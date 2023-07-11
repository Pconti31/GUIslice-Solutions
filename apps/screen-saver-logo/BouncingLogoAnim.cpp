/*
 Routine for bouncing logo animation using GUIslice
 using Adafruit_GFX
 Author: Paul Conti
 Date:   2023-Jun-28
*/

#include "BouncingLogoAnim.h"

#if (GSLC_USE_PROGMEM)
  #if defined(__AVR__)
    #include <avr/pgmspace.h>
  #else
    #include <pgmspace.h>
  #endif
#endif

BouncingLogoAnim::~BouncingLogoAnim() {
}

void BouncingLogoAnim::init(gslc_tsGui* pGui, const unsigned short *logo) {
  oldx = -1;
  oldy = -1;
  config_xd = 5;
  config_yd = 5;
  x = nMargin+1;
  y = nMargin+1;
  xd = config_xd;
  yd = config_yd;
  timeFrame = 5;
  bgCol = 0x0000; // black
  pGui = pGui;
  screen_width = pGui->nDispW;
  screen_height = pGui->nDispH;
  sImgRef = gslc_GetImageFromProg((const unsigned char*)logo, GSLC_IMGREF_FMT_BMP24);
  pImage = (uint16_t*)sImgRef.pImgBuf;
  logo_ht = pgm_read_word(pImage++);
  logo_wd = pgm_read_word(pImage++);
  canvas_wd = logo_wd + (nMargin);
  canvas_ht = logo_ht + (nMargin);
  // find our display driver
  tft = (Adafruit_GFX*)gslc_GetDriverDisp(pGui);
  tft->fillScreen(bgCol); // clear our screen
}

void BouncingLogoAnim::init(gslc_tsGui* pGui, const unsigned short *logo, gslc_tsColor bg_color, int x_delta, int y_delta, int delay_ms) {
  pGui = pGui;
  config_xd = x_delta;
  config_yd = y_delta;
  x = nMargin+1;
  y = nMargin+1;
  oldx = -1;
  oldy = -1;
  bgCol = colorTo16Bit(bg_color);
  xd = config_xd;
  yd = config_yd;
  timeFrame = delay_ms;
  screen_width = pGui->nDispW;
  screen_height = pGui->nDispH;
  sImgRef = gslc_GetImageFromProg((const unsigned char*)logo, GSLC_IMGREF_FMT_BMP24);
  pImage = (uint16_t*)sImgRef.pImgBuf;

  logo_ht = pgm_read_word(pImage++);
  logo_wd = pgm_read_word(pImage++);

  canvas_wd = logo_wd + (nMargin);
  canvas_ht = logo_ht + (nMargin);
  // find our display driver
  tft = (Adafruit_GFX*)gslc_GetDriverDisp(pGui);
  tft->fillScreen(bgCol); // clear our screen
}

//a function where animation continues to run.
void BouncingLogoAnim::loop(){
  yield();
  uint32_t currentTime = millis();
 
  if (((currentTime - lastDrawTime) < timeFrame)) {
    return;
  }

  if (isPlaying) {
    drawLogo();
    x += xd;
    y += yd;
    if (x < nMargin) {
        x = oldx;
        xd = -xd;
    } else if (x+canvas_wd+nMargin > screen_width) {
        x = oldx;
        xd = -xd;
    }
    if (y < nMargin) {
        y = oldy;
        yd = -yd;
    } else if (y+canvas_ht+nMargin > screen_height) {
        y = oldy;
        yd = -yd;
    }
  }
  lastDrawTime = currentTime;
  yield();
}

// actual drawing takes place here
void BouncingLogoAnim::drawLogo() {
  if (oldx > -1) {
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega328P__)
    tft->fillRect(0, 0, screen_width, screen_height, bgCol);
    tft->drawRGBBitmap (x, y, (const uint16_t[])pImage, logo_wd, logo_ht);
#else 
    GFXcanvas16 *canvas = new GFXcanvas16 (canvas_wd, canvas_ht);
    canvas -> fillRect (0, 0, canvas_wd, canvas_ht, bgCol);
    int x1 = config_xd + (x - oldx);
    int y1 = config_yd + (y - oldy);
    canvas->drawRGBBitmap(x1, y1, pImage, logo_wd, logo_ht);
    tft->drawRGBBitmap (x, y, canvas -> getBuffer (), canvas_wd, canvas_ht);
    delete canvas;
#endif
  }
  oldx = x;
  oldy = y;
}

//a function to stop animation.
void BouncingLogoAnim::pause() {
  isPlaying = false;
}

//a function to resume animation.
void BouncingLogoAnim::resume() {
  isPlaying = true;
  x = config_xd;
  y = config_yd;
  oldx = -1;
  oldy = -1;
  xd = config_xd;
  yd = config_yd;
  tft->fillScreen(bgCol);
}

uint16_t BouncingLogoAnim::colorTo16Bit(gslc_tsColor col888) {
  uint16_t nCol = 0;
  nCol  = (uint16_t)(col888.r  & 0b11111000) <<8;
  nCol |= (uint16_t)(col888.g & 0b11111100 ) <<3;
  nCol |= (uint16_t)(col888.b >> 3);
  return nCol;
}