/*
  DigitalRainAnim.cpp - Library for Digital Rain Animation for TFT_eSPI.
  Created by Eric Nam, December 30, 2020.
  Released into the public domain.
*/

#include "DigitalRainAnim.h"
#include "YuGothicUI-Regular14.h"

DigitalRainAnim::DigitalRainAnim() 
: spr(0), line_length(0), line_pos(0), line_speed(0){
}

DigitalRainAnim::~DigitalRainAnim() {
  delete [] line_length;
  delete [] line_pos;
  delete [] line_speed;
  if (spr) 
    delete spr;
}

void DigitalRainAnim::init(TFT_eSPI* tft){
  init(tft, 5, 18, 3, 15, 25);
}

void DigitalRainAnim::init(TFT_eSPI* tft, int new_line_len_min, int new_line_len_max, 
                           int new_line_speed_min, int new_line_speed_max, int new_timeFrame) {
  tft_DigitalRainAnim = tft; 
  line_len_min = new_line_len_min;
  line_len_max = new_line_len_max;
  line_speed_min = new_line_speed_min;
  line_speed_max = new_line_speed_max;
  timeFrame = new_timeFrame;

  width = tft_DigitalRainAnim->width();
  height = tft_DigitalRainAnim->height();

  // use same sprite for all lines  
  spr = new TFT_eSprite(tft);
  spr->createSprite(LINE_WIDTH, height);
  // from SPIFFS
//  spr->loadFont("YuGothicUI-Regular-14");
// from memory
  spr->loadFont(YuGothicUIRegular14);
  prepareAnim();
}

//checking how many lines it can draw from the width of the screen.
//the size of the array is determined by the number of lines.
void DigitalRainAnim::prepareAnim() {
  lastDrawTime = millis() - timeFrame;
  numOfline = width / LINE_WIDTH;
  line_length = new int[numOfline];
  line_pos = new int[numOfline];
  line_speed = new int[numOfline];
  for(int i = 0; i < numOfline; i++) {
    line_length[i] = getRandomNum(line_len_min, line_len_max);
    line_pos[i]    = setYPos(line_length[i]);
    line_speed[i]  = getRandomNum(line_speed_min, line_speed_max);
  }
  isPlaying = true;
 }

//updating each line with a new length, Y position, and speed.
void DigitalRainAnim::lineUpdate(int lineNum) {
    line_length[lineNum] = getRandomNum(line_len_min, line_len_max);
    line_pos[lineNum]    = setYPos(line_length[lineNum]);
    line_speed[lineNum]  = getRandomNum(line_speed_min, line_speed_max);
}

//while moving vertically, the color value changes and the character changes as well.
void DigitalRainAnim::lineAnimation(int lineNum) {
  int startX = lineNum * LINE_WIDTH;
  int currentY = 0;

  spr->fillSprite(TFT_BLACK);

  for(int i = 0; i < line_length[lineNum]; i++) {
    int colorVal = map(i, 0, line_length[lineNum], 25, 255);    
    spr->setCursor(0, line_pos[lineNum] + currentY);
    spr->setTextColor(tft_DigitalRainAnim->color565(0, colorVal, 0), TFT_BLACK); 
    spr->drawGlyph(getUnicodeChar());   
    currentY = (i * LETTER_HEIGHT);
  }  

  // last one (white)
     spr->setCursor(0, line_pos[lineNum] + currentY);
    spr->setTextColor(TFT_WHITE, TFT_BLACK);
    spr->drawGlyph(getUnicodeChar()); 
    
    spr->pushSprite(startX, 0);

  line_pos[lineNum] += line_speed[lineNum];
  if (line_pos[lineNum] >= height){
    lineUpdate(lineNum);
  }
}
 int DigitalRainAnim::getUnicodeChar() {
  // some ascii most hiragana
  return random(0, 10) == 0 ? random(65, 91) : random(12353, 12438);
 }

//a function where animation continues to run.
void DigitalRainAnim::loop(){
  yield();
  uint32_t currentTime = millis();
 
  if (((currentTime - lastDrawTime) < timeFrame)) {
    return;
  }

  if (isPlaying) {
    for (int i = 0; i < numOfline; i++) {
      lineAnimation(i);
    }
  }
  lastDrawTime = currentTime;
}

//a function to stop animation.
void DigitalRainAnim::pause() {
  isPlaying = false;
}

//a function to resume animation.
void DigitalRainAnim::resume() {
  isPlaying = true;
}

//a function that gets randomly from ASCII code 33 to 126.
String DigitalRainAnim::getASCIIChar() {
  return String((char)(random(33, 127)));
}

//move the position to start from out of the screen.
int DigitalRainAnim::setYPos(int lineLen) {
  return lineLen * -20;
}

//the function is to get the random number (including max)
int DigitalRainAnim::getRandomNum(int min, int max) {
  return random(min, max+1);
}
