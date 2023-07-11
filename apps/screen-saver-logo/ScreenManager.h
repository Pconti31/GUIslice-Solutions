// =======================================================================
// ScreenManager 
// Implements a framework or wrapper for integrating a screen 
// saver for use with applications written using GUIslice API.
//
// This routine will work with any display or touch interface
// supported by the GUIslice library.
// 
// ScreenManager will callback to supplied routines to trigger
// resuming, pausing and displaying a screen saver and will handle
// all details concerning interfacing with GUIslice.
//
// The resume and pause callback routines can be as simple as 
// turning the backlight on/off or as complex as a screen animation.
// If you are dealing with backlight simply don't add display and countdown 
// callbacks, ie.   
// screenMgr.init(&m_gui, 1800 /*30 minutes*/, &my_pause, &my_resume);
//
// This routine makes use of another piece of code called
// gslc_touchAny() which is a cut down version of GUIslice's gslc_update().
// If you use ScreenManager you must not call gslc_update() yourself.
// Instead you must call ScreenManager update in its place.
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
// Copyright (c) 2022-2023 Paul Conti
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
/// \file ScreenManager.h 


#ifndef _SCREENMANAGER_H
#define _SCREENMANAGER_H

#include "GUIslice.h"
#include "GUIslice_drv.h"

/// Callback function for ScreenManager
typedef void (*SCREENCB)();

void gslc_createInvisiblePG(gslc_tsGui* pGui);

class ScreenManager {
  public:
    ScreenManager() {};
    virtual ~ScreenManager() {};   
    void init(gslc_tsGui* pGui, int timeout/*seconds*/, 
                                SCREENCB      pausecb,
                                SCREENCB      resumecb,
                                SCREENCB      displaycb,
                                SCREENCB      countdowncb,
                                gslc_tsColor  bgCol); 
                                
    void update();              // update display
    void disable();             // disable screen manager timeouts
    void enable();              // enable screen manager after a disable call
    
    bool isActive() { return m_bIsActive; };
    bool isDisabled() { return m_bIsDisabled; };

    int  getCountDown();        // return count down in seconds
    void setTimeout(int timeout/*seconds*/); // change current time out value
    
    // The following would be better private but needs to be from C so...
    void stop();                // stop screen saver
    void display();             // called by our hidden callback to display screen saver
    gslc_tsColor getBgColor() { return m_bgCol; }; // get background color 
    
  protected:
    void pause();                // call user supplied pause
    void resume();               // call user supplied resume
    void closePopup();           // if current page is a popup close it
    
    SCREENCB m_fPausecb;         // routine to call when screen is paused
    SCREENCB m_fResumecb;        // routine to call when screen is resumed
    SCREENCB m_fDisplaycb;       // routine to call for display of screen
    SCREENCB m_fCountdowncb;// routine to call each second during inactivity 

    gslc_tsGui* m_pGui;           // pointer to all of GUIslice's state and content
    gslc_tsPage* m_curPage;       // page to restore
    bool     m_bIsActive;         // boolean for screen saver on/off
    bool     m_bIsDisabled;       // boolean indicates screen manager disabled
    uint32_t m_nStartTime;        // timer start time in millseconds
    uint32_t m_nInactivityMax;    // maximum time to have no activity in milliseconds
    uint32_t m_nInactivityCount;  // current inactivity time in seconds
    gslc_tsColor m_bgCol;         // background color
};

#endif // _SCREENMANAGER_H
