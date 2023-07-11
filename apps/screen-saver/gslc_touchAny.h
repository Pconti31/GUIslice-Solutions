#ifndef _GSLC_TOUCHANY_H_
#define _GSLC_TOUCHANY_H_

// =======================================================================
// GUIslice library
// - Calvin Hass
// - https://www.impulseadventure.com/elec/guislice-gui.html
// - https://github.com/ImpulseAdventure/GUIslice
//
// - Paul Conti - modified gslc_update() to create this routine
//                which simply tracks and notifies caller
//                upon detecting any touch events
// =======================================================================
//
// The MIT License
//
// Copyright 2016-2021 Calvin Hass
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
/// \file gslc_touchAny.h


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "GUIslice.h"
#include "GUIslice_drv.h"

///
/// Perform main GUIslice touch event tracking
/// - Tracks any touch events and return status
///
/// \param[in]  pGui:    Pointer to GUI
///
/// \return true if touch event. esle false
///
bool gslc_touchAny(gslc_tsGui* pGui);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _GSLC_TOUCHANY_H_
