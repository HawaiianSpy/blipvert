#pragma once

//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021 Don Jordan
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this softwareand associated documentation files(the "Software"), to deal
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
    // YUV colorspace fill functions
    void Fill_YUY2(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_UYVY(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_YVYU(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_VYUY(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_IYUV(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_YV12(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_YVU9(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_YUV9(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_IYU1(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_IYU2(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_Y800(uint8_t y_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_Y41P(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);
    void Fill_CLJR(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride);

    // YUV (YCbCr) to YUV (YCbCr) conversions
    void YUY2_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUY2_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void UYVY_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void UYVY_to_IUYV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void YVYU_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVYU_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void VYUY_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void VYUY_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void IYUV_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYUV_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void YV12_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YV12_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void YVU9_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YVU9_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void YUV9_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void YUV9_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void IYU1_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU1_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void IYU2_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void IYU2_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void Y800_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y800_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void CLJR_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void CLJR_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void Y41P_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void Y41P_to_IY41(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void IUYV_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void IY41_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

}

