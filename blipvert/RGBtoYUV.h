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

#include "blipverttypes.h"

namespace blipvert
{
    // RGB to YUV (YCbCr) transforms

    void RGBA_to_AYUV(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGBA_to_Y42T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGBA_to_Y41T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);

    void ARGB1555_to_AYUV(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void ARGB1555_to_Y42T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void ARGB1555_to_Y41T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);

    void RGB32_to_YUY2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_UYVY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_YVYU(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_VYUY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_I420(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_YV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_YVU9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_YUV9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_IYU1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_IYU2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_Y41P(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_CLJR(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_Y800(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_Y16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_AYUV(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_IMC1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_IMC2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_IMC3(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_IMC4(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_NV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_NV21(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_Y42T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_Y41T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB32_to_YV16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);

    void RGB24_to_YUY2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_UYVY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_YVYU(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_VYUY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_I420(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_YV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_YVU9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_YUV9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_IYU1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_IYU2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_Y41P(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_CLJR(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_Y800(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_Y16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_AYUV(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_IMC1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_IMC2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_IMC3(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_IMC4(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_NV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_NV21(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_Y42T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_Y41T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB24_to_YV16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);

    void RGB565_to_YUY2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_UYVY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_YVYU(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_VYUY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_I420(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_YV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_YVU9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_YUV9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_IYU1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_IYU2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_Y41P(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_CLJR(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_Y800(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_Y16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_AYUV(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_IMC1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_IMC2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_IMC3(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_IMC4(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_NV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_NV21(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_Y42T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_Y41T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB565_to_YV16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);

    void RGB555_to_YUY2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_UYVY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_YVYU(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_VYUY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_I420(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_YV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_YVU9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_YUV9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_IYU1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_IYU2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_Y41P(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_CLJR(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_Y800(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_Y16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_AYUV(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_IMC1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_IMC2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_IMC3(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_IMC4(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_NV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_NV21(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_Y42T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_Y41T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB555_to_YV16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);

    void RGB8_to_YUY2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_UYVY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_YVYU(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_VYUY(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_I420(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_YV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_YVU9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_YUV9(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_IYU1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_IYU2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_Y41P(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_CLJR(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_Y800(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_Y16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_AYUV(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_IMC1(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_IMC2(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_IMC3(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_IMC4(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_NV12(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_NV21(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_Y42T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_Y41T(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
    void RGB8_to_YV16(int32_t  width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, xRGBQUAD* in_palette = nullptr);
}

