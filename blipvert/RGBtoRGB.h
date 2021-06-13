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

    // RGB Colorspace fill functions
    void Fill_RGB32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride = 0);
    void Fill_RGB24(uint8_t red, uint8_t green, uint8_t blue, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride = 0);
    void Fill_RGB565(uint8_t red, uint8_t green, uint8_t blue, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride = 0);
    void Fill_RGB555(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride = 0);

    // RGB Colorspace check functions
    
    bool Check_RGB32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride = 0);
    bool Check_RGB24(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride = 0);
    bool Check_RGB565(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride = 0);
    bool Check_RGB555(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride = 0);

    // RGB to RGB conversion functions
    void RGB32_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB32_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB32_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void RGB24_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB24_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB24_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void RGB555_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB555_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB555_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void RGB565_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB565_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB565_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);

    void RGB8_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB8_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB8_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
    void RGB8_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBQUAD* in_palette = nullptr);
}

