#pragma once

//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021 Don Jordan
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files(the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions :
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

#include "legacydefs.h"

namespace blipvert
{
    // TODO: This is a hack for unit testing purposes.

    // In-place transform of bitmaps to greyscale.
    typedef void(__cdecl* t_flipverticalfunc) (int32_t width, int32_t height, uint8_t* buf, int32_t stride);

    void FlipVertical_RGBA(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_RGB32(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_RGB24(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_RGB565(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_RGB555(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_ARGB1555(int32_t width, int32_t height, uint8_t* buf, int32_t stride);

    void FlipVertical_AYUV(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_UYVY(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_YVYU(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_VYUY(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_YUY2(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_I420(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_YV12(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_YVU9(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_YUV9(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_IYU1(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_IYU2(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_Y800(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_Y16(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_Y41P(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_CLJR(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_IMC1(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_IMC2(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_IMC3(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_IMC4(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_NV12(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_Y42T(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void FlipVertical_Y41T(int32_t width, int32_t height, uint8_t* buf, int32_t stride);
};

