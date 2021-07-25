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
    // TODO: This is a hack for unit testing purposes.

    typedef void(__cdecl* t_setpixelfunc) (uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);

    // RGB Colorspace fill functions
    void SetPixel_RGBA(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_RGB32(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_RGB24(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_RGB565(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_RGB555(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_ARGB1555(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);

    // YUV colorspace fill functions
    void SetPixel_YUY2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_UYVY(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_YVYU(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_VYUY(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_I420(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_YV12(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_YVU9(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_YUV9(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_IYU1(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_IYU2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_Y800(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_Y16(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_Y41P(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_CLJR(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_AYUV(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_IMC1(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_IMC2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_IMC3(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_IMC4(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_NV12(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_Y42T(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
    void SetPixel_Y41T(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride);
}

