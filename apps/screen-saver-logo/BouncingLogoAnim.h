/*
 Routine for bouncing logo animation using GUIslice
 using Adafruit_GFX
 Author: Paul Conti
 Date:   2023-Jun-28
*/
#ifndef BouncingLogoAnim_h
#define BouncingLogoAnim_h

#include "GUIslice.h"
#include "GUIslice_drv.h"
#include "Adafruit_GFX.h"

class BouncingLogoAnim {

  public:
    BouncingLogoAnim()  {};
    virtual ~BouncingLogoAnim();   
    void init(gslc_tsGui* pGui, const unsigned short *logo);
    void init(gslc_tsGui* pGui, const unsigned short *logo, uint16_t bg_color, int x_delta, int y_delta, int delay_ms);
    void loop();                         //loop animation
    void pause();                        //pause animation 
    void resume();                       //resume animation
     
  private:
    void drawLogo();
    
    const int nMargin = 10;
    
    gslc_tsGui* pGui;                    // holds reference to GUIslice
    Adafruit_GFX* tft;                   // hold reference to our graphics driver
    uint16_t bgCol;                      // our back ground color
    gslc_tsImgRef sImgRef;               // holds pointer to our logo
    uint16_t* pImage;                    // pointer to raw image
    int logo_wd;                         // holds logo image width
    int logo_ht;                         // holds logo image height
    int canvas_wd;                       // our sprite width
    int canvas_ht;                       // our sprite height
    int screen_width;                    // holds screen width
    int screen_height;                   // holds screen height
    int timeFrame;                       // time frame for drawing
    uint32_t lastDrawTime;               // checking last drawing time
    bool isPlaying;                      // boolean for play or pause
    int x;                               // current x position of logo                          
    int y;                               // current y position of logo 
    int config_xd;                       // config delta to x position on each frame of animation
    int config_yd;                       // config delta to y position on each frame of animation
    int xd;                              // current delta +/- x position on each frame of animation
    int yd;                              // current delta +/- y position on each frame of animation
    int oldx;                            // previous x position of logo for erase 
    int oldy;                            // previous y position of logo for erase 

};

#endif // BouncingLogoAnim_h
