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
    // YUV (YCbCr) to YUV (YCbCr) transforms
    void AYUV_to_PackedY422(Stage* in, Stage* out);
    void AYUV_to_PlanarYUV(Stage* in, Stage* out);
    void AYUV_to_IMCx(Stage* in, Stage* out);
    void AYUV_to_NVx(Stage* in, Stage* out);
    void AYUV_to_IYU1(Stage* in, Stage* out);
    void AYUV_to_IYU2(Stage* in, Stage* out);
    void AYUV_to_Y800(Stage* in, Stage* out);
    void AYUV_to_Y16(Stage* in, Stage* out);
    void AYUV_to_CLJR(Stage* in, Stage* out);
    void AYUV_to_Y41P(Stage* in, Stage* out);
    void AYUV_to_Y42T(Stage* in, Stage* out);
    void AYUV_to_Y41T(Stage* in, Stage* out);
    void AYUV_to_YV16(Stage* in, Stage* out);

    void PackedY422_to_PackedY422(Stage* in, Stage* out);
    void PackedY422_to_PlanarYUV(Stage* in, Stage* out);
    void PackedY422_to_IYU1(Stage* in, Stage* out);
    void PackedY422_to_IYU2(Stage* in, Stage* out);
    void PackedY422_to_Y800(Stage* in, Stage* out);
    void PackedY422_to_Y16(Stage* in, Stage* out);
    void PackedY422_to_CLJR(Stage* in, Stage* out);
    void PackedY422_to_Y41P(Stage* in, Stage* out);
    void PackedY422_to_AYUV(Stage* in, Stage* out);
    void PackedY422_to_IMCx(Stage* in, Stage* out);
    void PackedY422_to_NVx(Stage* in, Stage* out);
    void PackedY422_to_Y42T(Stage* in, Stage* out);
    void PackedY422_to_Y41T(Stage* in, Stage* out);
    void PackedY422_to_YV16(Stage* in, Stage* out);

    void PlanarYUV_to_PlanarYUV(Stage* in, Stage* out);
    void PlanarYUV_to_PackedY422(Stage* in, Stage* out);
    void PlanarYUV_to_IYU1(Stage* in, Stage* out);
    void PlanarYUV_to_IYU2(Stage* in, Stage* out);
    void PlanarYUV_to_Y800(Stage* in, Stage* out);
    void PlanarYUV_to_Y16(Stage* in, Stage* out);
    void PlanarYUV_to_CLJR(Stage* in, Stage* out);
    void PlanarYUV_to_Y41P(Stage* in, Stage* out);
    void PlanarYUV_to_AYUV(Stage* in, Stage* out);
    void PlanarYUV_to_IMCx(Stage* in, Stage* out);
    void PlanarYUV_to_NVx(Stage* in, Stage* out);
    void PlanarYUV_to_Y42T(Stage* in, Stage* out);
    void PlanarYUV_to_Y41T(Stage* in, Stage* out);
    void PlanarYUV_to_YV16(Stage* in, Stage* out);

    void IYU1_to_PackedY422(Stage* in, Stage* out);
    void IYU1_to_PlanarYUV(Stage* in, Stage* out);
    void IYU1_to_IYU2(Stage* in, Stage* out);
    void IYU1_to_Y800(Stage* in, Stage* out);
    void IYU1_to_Y16(Stage* in, Stage* out);
    void IYU1_to_CLJR(Stage* in, Stage* out);
    void IYU1_to_Y41P(Stage* in, Stage* out);
    void IYU1_to_AYUV(Stage* in, Stage* out);
    void IYU1_to_IMCx(Stage* in, Stage* out);
    void IYU1_to_NVx(Stage* in, Stage* out);
    void IYU1_to_Y42T(Stage* in, Stage* out);
    void IYU1_to_Y41T(Stage* in, Stage* out);
    void IYU1_to_YV16(Stage* in, Stage* out);

    void IYU2_to_PackedY422(Stage* in, Stage* out);
    void IYU2_to_PlanarYUV(Stage* in, Stage* out);
    void IYU2_to_IYU1(Stage* in, Stage* out);
    void IYU2_to_Y800(Stage* in, Stage* out);
    void IYU2_to_Y16(Stage* in, Stage* out);
    void IYU2_to_CLJR(Stage* in, Stage* out);
    void IYU2_to_Y41P(Stage* in, Stage* out);
    void IYU2_to_AYUV(Stage* in, Stage* out);
    void IYU2_to_IMCx(Stage* in, Stage* out);
    void IYU2_to_NVx(Stage* in, Stage* out);
    void IYU2_to_Y42T(Stage* in, Stage* out);
    void IYU2_to_Y41T(Stage* in, Stage* out);
    void IYU2_to_YV16(Stage* in, Stage* out);

    void Y800_to_PackedY422(Stage* in, Stage* out);
    void Y800_to_PlanarYUV(Stage* in, Stage* out);
    void Y800_to_IYU1(Stage* in, Stage* out);
    void Y800_to_IYU2(Stage* in, Stage* out);
    void Y800_to_CLJR(Stage* in, Stage* out);
    void Y800_to_Y41P(Stage* in, Stage* out);
    void Y800_to_AYUV(Stage* in, Stage* out);
    void Y800_to_Y16(Stage* in, Stage* out);
    void Y800_to_IMCx(Stage* in, Stage* out);
    void Y800_to_NVx(Stage* in, Stage* out);
    void Y800_to_Y42T(Stage* in, Stage* out);
    void Y800_to_Y41T(Stage* in, Stage* out);
    void Y800_to_YV16(Stage* in, Stage* out);

    void Y16_to_PackedY422(Stage* in, Stage* out);
    void Y16_to_PlanarYUV(Stage* in, Stage* out);
    void Y16_to_IYU1(Stage* in, Stage* out);
    void Y16_to_IYU2(Stage* in, Stage* out);
    void Y16_to_CLJR(Stage* in, Stage* out);
    void Y16_to_Y41P(Stage* in, Stage* out);
    void Y16_to_AYUV(Stage* in, Stage* out);
    void Y16_to_Y800(Stage* in, Stage* out);
    void Y16_to_IMCx(Stage* in, Stage* out);
    void Y16_to_NVx(Stage* in, Stage* out);
    void Y16_to_Y42T(Stage* in, Stage* out);
    void Y16_to_Y41T(Stage* in, Stage* out);
    void Y16_to_YV16(Stage* in, Stage* out);

    void CLJR_to_PackedY422(Stage* in, Stage* out);
    void CLJR_to_PlanarYUV(Stage* in, Stage* out);
    void CLJR_to_IYU1(Stage* in, Stage* out);
    void CLJR_to_IYU2(Stage* in, Stage* out);
    void CLJR_to_Y800(Stage* in, Stage* out);
    void CLJR_to_Y16(Stage* in, Stage* out);
    void CLJR_to_Y41P(Stage* in, Stage* out);
    void CLJR_to_AYUV(Stage* in, Stage* out);
    void CLJR_to_IMCx(Stage* in, Stage* out);
    void CLJR_to_NVx(Stage* in, Stage* out);
    void CLJR_to_Y42T(Stage* in, Stage* out);
    void CLJR_to_Y41T(Stage* in, Stage* out);
    void CLJR_to_YV16(Stage* in, Stage* out);

    void Y41P_to_PackedY422(Stage* in, Stage* out);
    void Y41P_to_PlanarYUV(Stage* in, Stage* out);
    void Y41P_to_IYU1(Stage* in, Stage* out);
    void Y41P_to_IYU2(Stage* in, Stage* out);
    void Y41P_to_Y800(Stage* in, Stage* out);
    void Y41P_to_Y16(Stage* in, Stage* out);
    void Y41P_to_CLJR(Stage* in, Stage* out);
    void Y41P_to_AYUV(Stage* in, Stage* out);
    void Y41P_to_IMCx(Stage* in, Stage* out);
    void Y41P_to_NVx(Stage* in, Stage* out);
    void Y41P_to_Y42T(Stage* in, Stage* out);
    void Y41P_to_Y41T(Stage* in, Stage* out);
    void Y41P_to_YV16(Stage* in, Stage* out);

    void IMCx_to_AYUV(Stage* in, Stage* out);
    void IMCx_to_PackedY422(Stage* in, Stage* out);
    void IMCx_to_PlanarYUV(Stage* in, Stage* out);
    void IMCx_to_IYU1(Stage* in, Stage* out);
    void IMCx_to_IYU2(Stage* in, Stage* out);
    void IMCx_to_Y800(Stage* in, Stage* out);
    void IMCx_to_Y16(Stage* in, Stage* out);
    void IMCx_to_CLJR(Stage* in, Stage* out);
    void IMCx_to_Y41P(Stage* in, Stage* out);
    void IMCx_to_IMCx(Stage* in, Stage* out);
    void IMCx_to_NVx(Stage* in, Stage* out);
    void IMCx_to_Y42T(Stage* in, Stage* out);
    void IMCx_to_Y41T(Stage* in, Stage* out);
    void IMCx_to_YV16(Stage* in, Stage* out);

    void NVx_to_AYUV(Stage* in, Stage* out);
    void NVx_to_PackedY422(Stage* in, Stage* out);
    void NVx_to_PlanarYUV(Stage* in, Stage* out);
    void NVx_to_IYU1(Stage* in, Stage* out);
    void NVx_to_IYU2(Stage* in, Stage* out);
    void NVx_to_Y800(Stage* in, Stage* out);
    void NVx_to_Y16(Stage* in, Stage* out);
    void NVx_to_CLJR(Stage* in, Stage* out);
    void NVx_to_Y41P(Stage* in, Stage* out);
    void NVx_to_IMCx(Stage* in, Stage* out);
    void NVx_to_NVx(Stage* in, Stage* out);
    void NVx_to_Y42T(Stage* in, Stage* out);
    void NVx_to_Y41T(Stage* in, Stage* out);
    void NVx_to_YV16(Stage* in, Stage* out);

    void Y42T_to_PackedY422(Stage* in, Stage* out);
    void Y42T_to_PlanarYUV(Stage* in, Stage* out);
    void Y42T_to_IYU1(Stage* in, Stage* out);
    void Y42T_to_IYU2(Stage* in, Stage* out);
    void Y42T_to_Y800(Stage* in, Stage* out);
    void Y42T_to_Y16(Stage* in, Stage* out);
    void Y42T_to_CLJR(Stage* in, Stage* out);
    void Y42T_to_Y41P(Stage* in, Stage* out);
    void Y42T_to_AYUV(Stage* in, Stage* out);
    void Y42T_to_IMCx(Stage* in, Stage* out);
    void Y42T_to_NVx(Stage* in, Stage* out);
    void Y42T_to_Y41T(Stage* in, Stage* out);
    void Y42T_to_YV16(Stage* in, Stage* out);

    void Y41T_to_PackedY422(Stage* in, Stage* out);
    void Y41T_to_PlanarYUV(Stage* in, Stage* out);
    void Y41T_to_IYU1(Stage* in, Stage* out);
    void Y41T_to_IYU2(Stage* in, Stage* out);
    void Y41T_to_Y800(Stage* in, Stage* out);
    void Y41T_to_Y16(Stage* in, Stage* out);
    void Y41T_to_CLJR(Stage* in, Stage* out);
    void Y41T_to_Y41P(Stage* in, Stage* out);
    void Y41T_to_AYUV(Stage* in, Stage* out);
    void Y41T_to_IMCx(Stage* in, Stage* out);
    void Y41T_to_NVx(Stage* in, Stage* out);
    void Y41T_to_Y42T(Stage* in, Stage* out);
    void Y41T_to_YV16(Stage* in, Stage* out);

    void YV16_to_PackedY422(Stage* in, Stage* out);
    void YV16_to_PlanarYUV(Stage* in, Stage* out);
    void YV16_to_IYU1(Stage* in, Stage* out);
    void YV16_to_IYU2(Stage* in, Stage* out);
    void YV16_to_CLJR(Stage* in, Stage* out);
    void YV16_to_Y41P(Stage* in, Stage* out);
    void YV16_to_AYUV(Stage* in, Stage* out);
    void YV16_to_IMCx(Stage* in, Stage* out);
    void YV16_to_NVx(Stage* in, Stage* out);
    void YV16_to_Y42T(Stage* in, Stage* out);
    void YV16_to_Y41T(Stage* in, Stage* out);

    // Interlaced versions of common YUV formats for what?
    void UYVY_to_IUYV(Stage* in, Stage* out);
    void IUYV_to_UYVY(Stage* in, Stage* out);
    void IY41_to_Y41P(Stage* in, Stage* out);
    void Y41P_to_IY41(Stage* in, Stage* out);
}

