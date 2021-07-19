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
    // In-place transform of bitmaps to greyscale.
    typedef void(__cdecl* t_greyscalefunc) (int32_t width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette);

    void RGBA_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void RGB4_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void RGB1_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);

    void AYUV_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void UYVY_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void YVYU_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void VYUY_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void YUY2_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void I420_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void YV12_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void YVU9_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void YUV9_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void IYU1_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void IYU2_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void Y41P_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void CLJR_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void IMC1_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void IMC2_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void IMC3_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void IMC4_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void NV12_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
    void Y42T_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0, xRGBQUAD* in_palette = nullptr);
};

