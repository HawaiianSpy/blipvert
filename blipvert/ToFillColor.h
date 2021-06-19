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

namespace blipvert
{
    typedef void(__cdecl* t_fillcolorfunc) (uint8_t red_y_level, uint8_t green_u_level, uint8_t blue_v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride);

    // RGB Colorspace fill functions
    void Fill_RGB32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_RGB24(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_RGB565(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_RGB555(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);

    // YUV colorspace fill functions
    void Fill_YUY2(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_UYVY(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_YVYU(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_VYUY(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_IYUV(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_YV12(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_YVU9(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_YUV9(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_IYU1(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_IYU2(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_Y800(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_Y41P(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
    void Fill_CLJR(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride = 0);
}

