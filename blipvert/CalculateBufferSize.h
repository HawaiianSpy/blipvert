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
    typedef uint32_t(__cdecl* t_calcbuffsizefunc) (int32_t width, int32_t height, int32_t& stride);

    uint32_t CalcBufferSize_RGBA(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_RGB32(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_RGB24(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_RGB565(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_RGB555(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_ARGB1555(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_RGB8(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_RGB4(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_RGB1(int32_t width, int32_t height, int32_t& stride);

    uint32_t CalcBufferSize_AYUV(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_UYVY(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_YVYU(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_VYUY(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_YUY2(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_I420(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_YV12(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_YVU9(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_YUV9(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_IYU1(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_IYU2(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_Y800(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_Y16(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_Y41P(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_CLJR(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_IMC1(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_IMC2(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_IMC3(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_IMC4(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_NV12(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_NV21(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_Y42T(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_Y41T(int32_t width, int32_t height, int32_t& stride);
    uint32_t CalcBufferSize_YV16(int32_t width, int32_t height, int32_t& stride);
};

