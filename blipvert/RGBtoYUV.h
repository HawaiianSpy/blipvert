#pragma once

//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021-2025 Don Jordan
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
#include "Staging.h"

namespace blipvert
{
    // RGB to YUV (YCbCr) transforms

    void RGBA_to_AYUV(Stage* in, Stage* out);
    void RGBA_to_Y42T(Stage* in, Stage* out);
    void RGBA_to_Y41T(Stage* in, Stage* out);

    void ARGB1555_to_AYUV(Stage* in, Stage* out);
    void ARGB1555_to_Y42T(Stage* in, Stage* out);
    void ARGB1555_to_Y41T(Stage* in, Stage* out);

    void RGB32_to_PackedY422(Stage* in, Stage* out);
    void RGB32_to_PlanarYUV(Stage* in, Stage* out);
    void RGB32_to_IYU1(Stage* in, Stage* out);
    void RGB32_to_IYU2(Stage* in, Stage* out);
    void RGB32_to_Y41P(Stage* in, Stage* out);
    void RGB32_to_CLJR(Stage* in, Stage* out);
    void RGB32_to_Y800(Stage* in, Stage* out);
    void RGB32_to_Y16(Stage* in, Stage* out);
    void RGB32_to_AYUV(Stage* in, Stage* out);
    void RGB32_to_IMCx(Stage* in, Stage* out);
    void RGB32_to_NVx(Stage* in, Stage* out);
    void RGB32_to_Y42T(Stage* in, Stage* out);
    void RGB32_to_Y41T(Stage* in, Stage* out);
    void RGB32_to_YV16(Stage* in, Stage* out);

    void RGB24_to_PackedY422(Stage* in, Stage* out);
    void RGB24_to_PlanarYUV(Stage* in, Stage* out);
    void RGB24_to_IYU1(Stage* in, Stage* out);
    void RGB24_to_IYU2(Stage* in, Stage* out);
    void RGB24_to_Y41P(Stage* in, Stage* out);
    void RGB24_to_CLJR(Stage* in, Stage* out);
    void RGB24_to_Y800(Stage* in, Stage* out);
    void RGB24_to_Y16(Stage* in, Stage* out);
    void RGB24_to_AYUV(Stage* in, Stage* out);
    void RGB24_to_IMCx(Stage* in, Stage* out);
    void RGB24_to_NVx(Stage* in, Stage* out);
    void RGB24_to_Y42T(Stage* in, Stage* out);
    void RGB24_to_Y41T(Stage* in, Stage* out);
    void RGB24_to_YV16(Stage* in, Stage* out);

    void RGB565_to_PackedY422(Stage* in, Stage* out);
    void RGB565_to_PlanarYUV(Stage* in, Stage* out);
    void RGB565_to_IYU1(Stage* in, Stage* out);
    void RGB565_to_IYU2(Stage* in, Stage* out);
    void RGB565_to_Y41P(Stage* in, Stage* out);
    void RGB565_to_CLJR(Stage* in, Stage* out);
    void RGB565_to_Y800(Stage* in, Stage* out);
    void RGB565_to_Y16(Stage* in, Stage* out);
    void RGB565_to_AYUV(Stage* in, Stage* out);
    void RGB565_to_IMCx(Stage* in, Stage* out);
    void RGB565_to_NVx(Stage* in, Stage* out);
    void RGB565_to_Y42T(Stage* in, Stage* out);
    void RGB565_to_Y41T(Stage* in, Stage* out);
    void RGB565_to_YV16(Stage* in, Stage* out);

    void RGB555_to_PackedY422(Stage* in, Stage* out);
    void RGB555_to_PlanarYUV(Stage* in, Stage* out);
    void RGB555_to_IYU1(Stage* in, Stage* out);
    void RGB555_to_IYU2(Stage* in, Stage* out);
    void RGB555_to_Y41P(Stage* in, Stage* out);
    void RGB555_to_CLJR(Stage* in, Stage* out);
    void RGB555_to_Y800(Stage* in, Stage* out);
    void RGB555_to_Y16(Stage* in, Stage* out);
    void RGB555_to_AYUV(Stage* in, Stage* out);
    void RGB555_to_IMCx(Stage* in, Stage* out);
    void RGB555_to_NVx(Stage* in, Stage* out);
    void RGB555_to_Y42T(Stage* in, Stage* out);
    void RGB555_to_Y41T(Stage* in, Stage* out);
    void RGB555_to_YV16(Stage* in, Stage* out);

    void RGB8_to_PackedY422(Stage* in, Stage* out);
    void RGB8_to_PlanarYUV(Stage* in, Stage* out);
    void RGB8_to_IYU1(Stage* in, Stage* out);
    void RGB8_to_IYU2(Stage* in, Stage* out);
    void RGB8_to_Y41P(Stage* in, Stage* out);
    void RGB8_to_CLJR(Stage* in, Stage* out);
    void RGB8_to_Y800(Stage* in, Stage* out);
    void RGB8_to_Y16(Stage* in, Stage* out);
    void RGB8_to_AYUV(Stage* in, Stage* out);
    void RGB8_to_IMCx(Stage* in, Stage* out);
    void RGB8_to_NVx(Stage* in, Stage* out);
    void RGB8_to_Y42T(Stage* in, Stage* out);
    void RGB8_to_Y41T(Stage* in, Stage* out);
    void RGB8_to_YV16(Stage* in, Stage* out);
}

