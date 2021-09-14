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
#include "Staging.h"

namespace blipvert
{
    // YUV (YCbCr) to RGB transforms

    void PackedY422_to_RGB32(Stage* in, Stage* out);
    void PackedY422_to_RGB24(Stage* in, Stage* out);
    void PackedY422_to_RGB565(Stage* in, Stage* out);
    void PackedY422_to_RGB555(Stage* in, Stage* out);

    void CLJR_to_RGB32(Stage* in, Stage* out);
    void CLJR_to_RGB24(Stage* in, Stage* out);
    void CLJR_to_RGB565(Stage* in, Stage* out);
    void CLJR_to_RGB555(Stage* in, Stage* out);

    void Y41P_to_RGB32(Stage* in, Stage* out);
    void Y41P_to_RGB24(Stage* in, Stage* out);
    void Y41P_to_RGB565(Stage* in, Stage* out);
    void Y41P_to_RGB555(Stage* in, Stage* out);

    void IYU1_to_RGB32(Stage* in, Stage* out);
    void IYU1_to_RGB24(Stage* in, Stage* out);
    void IYU1_to_RGB565(Stage* in, Stage* out);
    void IYU1_to_RGB555(Stage* in, Stage* out);

    void IYU2_to_RGB32(Stage* in, Stage* out);
    void IYU2_to_RGB24(Stage* in, Stage* out);
    void IYU2_to_RGB565(Stage* in, Stage* out);
    void IYU2_to_RGB555(Stage* in, Stage* out);

    void PlanarYUV_to_RGB32(Stage* in, Stage* out);
    void PlanarYUV_to_RGB24(Stage* in, Stage* out);
    void PlanarYUV_to_RGB565(Stage* in, Stage* out);
    void PlanarYUV_to_RGB555(Stage* in, Stage* out);

    void IMCx_to_RGB32(Stage* in, Stage* out);
    void IMCx_to_RGB24(Stage* in, Stage* out);
    void IMCx_to_RGB565(Stage* in, Stage* out);
    void IMCx_to_RGB555(Stage* in, Stage* out);

    void Y800_to_RGB32(Stage* in, Stage* out);
    void Y800_to_RGB24(Stage* in, Stage* out);
    void Y800_to_RGB565(Stage* in, Stage* out);
    void Y800_to_RGB555(Stage* in, Stage* out);

    void Y16_to_RGB32(Stage* in, Stage* out);
    void Y16_to_RGB24(Stage* in, Stage* out);
    void Y16_to_RGB565(Stage* in, Stage* out);
    void Y16_to_RGB555(Stage* in, Stage* out);

    void AYUV_to_RGBA(Stage* in, Stage* out);
    void AYUV_to_RGB32(Stage* in, Stage* out);
    void AYUV_to_RGB24(Stage* in, Stage* out);
    void AYUV_to_RGB565(Stage* in, Stage* out);
    void AYUV_to_RGB555(Stage* in, Stage* out);
    void AYUV_to_ARGB1555(Stage* in, Stage* out);

    void NVx_to_RGB32(Stage* in, Stage* out);
    void NVx_to_RGB24(Stage* in, Stage* out);
    void NVx_to_RGB565(Stage* in, Stage* out);
    void NVx_to_RGB555(Stage* in, Stage* out);

    void Y42T_to_RGBA(Stage* in, Stage* out);
    void Y42T_to_RGB32(Stage* in, Stage* out);
    void Y42T_to_RGB24(Stage* in, Stage* out);
    void Y42T_to_RGB565(Stage* in, Stage* out);
    void Y42T_to_RGB555(Stage* in, Stage* out);
    void Y42T_to_ARGB1555(Stage* in, Stage* out);

    void Y41T_to_RGBA(Stage* in, Stage* out);
    void Y41T_to_RGB32(Stage* in, Stage* out);
    void Y41T_to_RGB24(Stage* in, Stage* out);
    void Y41T_to_RGB565(Stage* in, Stage* out);
    void Y41T_to_RGB555(Stage* in, Stage* out);
    void Y41T_to_ARGB1555(Stage* in, Stage* out);

    void YV16_to_RGB32(Stage* in, Stage* out);
    void YV16_to_RGB24(Stage* in, Stage* out);
    void YV16_to_RGB565(Stage* in, Stage* out);
    void YV16_to_RGB555(Stage* in, Stage* out);
}

