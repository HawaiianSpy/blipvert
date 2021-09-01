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

    void I420_to_YVU9(Stage* in, Stage* out);
    void I420_to_YUV9(Stage* in, Stage* out);
    void I420_to_YV12(Stage* in, Stage* out);
    void I420_to_YUY2(Stage* in, Stage* out);
    void I420_to_UYVY(Stage* in, Stage* out);
    void I420_to_YVYU(Stage* in, Stage* out);
    void I420_to_VYUY(Stage* in, Stage* out);
    void I420_to_IYU1(Stage* in, Stage* out);
    void I420_to_IYU2(Stage* in, Stage* out);
    void I420_to_Y800(Stage* in, Stage* out);
    void I420_to_Y16(Stage* in, Stage* out);
    void I420_to_CLJR(Stage* in, Stage* out);
    void I420_to_Y41P(Stage* in, Stage* out);
    void I420_to_AYUV(Stage* in, Stage* out);
    void I420_to_IMC1(Stage* in, Stage* out);
    void I420_to_IMC2(Stage* in, Stage* out);
    void I420_to_IMC3(Stage* in, Stage* out);
    void I420_to_IMC4(Stage* in, Stage* out);
    void I420_to_NV12(Stage* in, Stage* out);
    void I420_to_NV21(Stage* in, Stage* out);
    void I420_to_Y42T(Stage* in, Stage* out);
    void I420_to_Y41T(Stage* in, Stage* out);
    void I420_to_YV16(Stage* in, Stage* out);

    void YV12_to_YVU9(Stage* in, Stage* out);
    void YV12_to_YUV9(Stage* in, Stage* out);
    void YV12_to_I420(Stage* in, Stage* out);
    void YV12_to_YUY2(Stage* in, Stage* out);
    void YV12_to_UYVY(Stage* in, Stage* out);
    void YV12_to_YVYU(Stage* in, Stage* out);
    void YV12_to_VYUY(Stage* in, Stage* out);
    void YV12_to_IYU1(Stage* in, Stage* out);
    void YV12_to_IYU2(Stage* in, Stage* out);
    void YV12_to_Y800(Stage* in, Stage* out);
    void YV12_to_Y16(Stage* in, Stage* out);
    void YV12_to_CLJR(Stage* in, Stage* out);
    void YV12_to_Y41P(Stage* in, Stage* out);
    void YV12_to_AYUV(Stage* in, Stage* out);
    void YV12_to_IMC1(Stage* in, Stage* out);
    void YV12_to_IMC2(Stage* in, Stage* out);
    void YV12_to_IMC3(Stage* in, Stage* out);
    void YV12_to_IMC4(Stage* in, Stage* out);
    void YV12_to_NV12(Stage* in, Stage* out);
    void YV12_to_NV21(Stage* in, Stage* out);
    void YV12_to_Y42T(Stage* in, Stage* out);
    void YV12_to_Y41T(Stage* in, Stage* out);
    void YV12_to_YV16(Stage* in, Stage* out);

    void YVU9_to_I420(Stage* in, Stage* out);
    void YVU9_to_YV12(Stage* in, Stage* out);
    void YVU9_to_YUV9(Stage* in, Stage* out);
    void YVU9_to_YUY2(Stage* in, Stage* out);
    void YVU9_to_UYVY(Stage* in, Stage* out);
    void YVU9_to_YVYU(Stage* in, Stage* out);
    void YVU9_to_VYUY(Stage* in, Stage* out);
    void YVU9_to_IYU1(Stage* in, Stage* out);
    void YVU9_to_IYU2(Stage* in, Stage* out);
    void YVU9_to_Y800(Stage* in, Stage* out);
    void YVU9_to_Y16(Stage* in, Stage* out);
    void YVU9_to_CLJR(Stage* in, Stage* out);
    void YVU9_to_Y41P(Stage* in, Stage* out);
    void YVU9_to_AYUV(Stage* in, Stage* out);
    void YVU9_to_IMC1(Stage* in, Stage* out);
    void YVU9_to_IMC2(Stage* in, Stage* out);
    void YVU9_to_IMC3(Stage* in, Stage* out);
    void YVU9_to_IMC4(Stage* in, Stage* out);
    void YVU9_to_NV12(Stage* in, Stage* out);
    void YVU9_to_NV21(Stage* in, Stage* out);
    void YVU9_to_Y42T(Stage* in, Stage* out);
    void YVU9_to_Y41T(Stage* in, Stage* out);
    void YVU9_to_YV16(Stage* in, Stage* out);

    void YUV9_to_I420(Stage* in, Stage* out);
    void YUV9_to_YV12(Stage* in, Stage* out);
    void YUV9_to_YVU9(Stage* in, Stage* out);
    void YUV9_to_YUY2(Stage* in, Stage* out);
    void YUV9_to_UYVY(Stage* in, Stage* out);
    void YUV9_to_YVYU(Stage* in, Stage* out);
    void YUV9_to_VYUY(Stage* in, Stage* out);
    void YUV9_to_IYU1(Stage* in, Stage* out);
    void YUV9_to_IYU2(Stage* in, Stage* out);
    void YUV9_to_Y800(Stage* in, Stage* out);
    void YUV9_to_Y16(Stage* in, Stage* out);
    void YUV9_to_CLJR(Stage* in, Stage* out);
    void YUV9_to_Y41P(Stage* in, Stage* out);
    void YUV9_to_AYUV(Stage* in, Stage* out);
    void YUV9_to_IMC1(Stage* in, Stage* out);
    void YUV9_to_IMC2(Stage* in, Stage* out);
    void YUV9_to_IMC3(Stage* in, Stage* out);
    void YUV9_to_IMC4(Stage* in, Stage* out);
    void YUV9_to_NV12(Stage* in, Stage* out);
    void YUV9_to_NV21(Stage* in, Stage* out);
    void YUV9_to_Y42T(Stage* in, Stage* out);
    void YUV9_to_Y41T(Stage* in, Stage* out);
    void YUV9_to_YV16(Stage* in, Stage* out);

    void IYU1_to_YUY2(Stage* in, Stage* out);
    void IYU1_to_UYVY(Stage* in, Stage* out);
    void IYU1_to_YVYU(Stage* in, Stage* out);
    void IYU1_to_VYUY(Stage* in, Stage* out);
    void IYU1_to_I420(Stage* in, Stage* out);
    void IYU1_to_YV12(Stage* in, Stage* out);
    void IYU1_to_YVU9(Stage* in, Stage* out);
    void IYU1_to_YUV9(Stage* in, Stage* out);
    void IYU1_to_IYU2(Stage* in, Stage* out);
    void IYU1_to_Y800(Stage* in, Stage* out);
    void IYU1_to_Y16(Stage* in, Stage* out);
    void IYU1_to_CLJR(Stage* in, Stage* out);
    void IYU1_to_Y41P(Stage* in, Stage* out);
    void IYU1_to_AYUV(Stage* in, Stage* out);
    void IYU1_to_IMC1(Stage* in, Stage* out);
    void IYU1_to_IMC2(Stage* in, Stage* out);
    void IYU1_to_IMC3(Stage* in, Stage* out);
    void IYU1_to_IMC4(Stage* in, Stage* out);
    void IYU1_to_NV12(Stage* in, Stage* out);
    void IYU1_to_NV21(Stage* in, Stage* out);
    void IYU1_to_Y42T(Stage* in, Stage* out);
    void IYU1_to_Y41T(Stage* in, Stage* out);
    void IYU1_to_YV16(Stage* in, Stage* out);

    void IYU2_to_YUY2(Stage* in, Stage* out);
    void IYU2_to_UYVY(Stage* in, Stage* out);
    void IYU2_to_YVYU(Stage* in, Stage* out);
    void IYU2_to_VYUY(Stage* in, Stage* out);
    void IYU2_to_I420(Stage* in, Stage* out);
    void IYU2_to_YV12(Stage* in, Stage* out);
    void IYU2_to_YVU9(Stage* in, Stage* out);
    void IYU2_to_YUV9(Stage* in, Stage* out);
    void IYU2_to_IYU1(Stage* in, Stage* out);
    void IYU2_to_Y800(Stage* in, Stage* out);
    void IYU2_to_Y16(Stage* in, Stage* out);
    void IYU2_to_CLJR(Stage* in, Stage* out);
    void IYU2_to_Y41P(Stage* in, Stage* out);
    void IYU2_to_AYUV(Stage* in, Stage* out);
    void IYU2_to_IMC1(Stage* in, Stage* out);
    void IYU2_to_IMC2(Stage* in, Stage* out);
    void IYU2_to_IMC3(Stage* in, Stage* out);
    void IYU2_to_IMC4(Stage* in, Stage* out);
    void IYU2_to_NV12(Stage* in, Stage* out);
    void IYU2_to_NV21(Stage* in, Stage* out);
    void IYU2_to_Y42T(Stage* in, Stage* out);
    void IYU2_to_Y41T(Stage* in, Stage* out);
    void IYU2_to_YV16(Stage* in, Stage* out);

    void Y800_to_YUY2(Stage* in, Stage* out);
    void Y800_to_UYVY(Stage* in, Stage* out);
    void Y800_to_YVYU(Stage* in, Stage* out);
    void Y800_to_VYUY(Stage* in, Stage* out);
    void Y800_to_I420(Stage* in, Stage* out);
    void Y800_to_YV12(Stage* in, Stage* out);
    void Y800_to_YVU9(Stage* in, Stage* out);
    void Y800_to_YUV9(Stage* in, Stage* out);
    void Y800_to_IYU1(Stage* in, Stage* out);
    void Y800_to_IYU2(Stage* in, Stage* out);
    void Y800_to_CLJR(Stage* in, Stage* out);
    void Y800_to_Y41P(Stage* in, Stage* out);
    void Y800_to_AYUV(Stage* in, Stage* out);
    void Y800_to_Y16(Stage* in, Stage* out);
    void Y800_to_IMC1(Stage* in, Stage* out);
    void Y800_to_IMC2(Stage* in, Stage* out);
    void Y800_to_IMC3(Stage* in, Stage* out);
    void Y800_to_IMC4(Stage* in, Stage* out);
    void Y800_to_NV12(Stage* in, Stage* out);
    void Y800_to_Y42T(Stage* in, Stage* out);
    void Y800_to_Y41T(Stage* in, Stage* out);
    void Y800_to_YV16(Stage* in, Stage* out);

    void Y16_to_YUY2(Stage* in, Stage* out);
    void Y16_to_UYVY(Stage* in, Stage* out);
    void Y16_to_YVYU(Stage* in, Stage* out);
    void Y16_to_VYUY(Stage* in, Stage* out);
    void Y16_to_I420(Stage* in, Stage* out);
    void Y16_to_YV12(Stage* in, Stage* out);
    void Y16_to_YVU9(Stage* in, Stage* out);
    void Y16_to_YUV9(Stage* in, Stage* out);
    void Y16_to_IYU1(Stage* in, Stage* out);
    void Y16_to_IYU2(Stage* in, Stage* out);
    void Y16_to_CLJR(Stage* in, Stage* out);
    void Y16_to_Y41P(Stage* in, Stage* out);
    void Y16_to_AYUV(Stage* in, Stage* out);
    void Y16_to_Y800(Stage* in, Stage* out);
    void Y16_to_IMC1(Stage* in, Stage* out);
    void Y16_to_IMC2(Stage* in, Stage* out);
    void Y16_to_IMC3(Stage* in, Stage* out);
    void Y16_to_IMC4(Stage* in, Stage* out);
    void Y16_to_NV12(Stage* in, Stage* out);
    void Y16_to_Y42T(Stage* in, Stage* out);
    void Y16_to_Y41T(Stage* in, Stage* out);
    void Y16_to_YV16(Stage* in, Stage* out);

    void CLJR_to_YUY2(Stage* in, Stage* out);
    void CLJR_to_UYVY(Stage* in, Stage* out);
    void CLJR_to_YVYU(Stage* in, Stage* out);
    void CLJR_to_VYUY(Stage* in, Stage* out);
    void CLJR_to_I420(Stage* in, Stage* out);
    void CLJR_to_YV12(Stage* in, Stage* out);
    void CLJR_to_YVU9(Stage* in, Stage* out);
    void CLJR_to_YUV9(Stage* in, Stage* out);
    void CLJR_to_IYU1(Stage* in, Stage* out);
    void CLJR_to_IYU2(Stage* in, Stage* out);
    void CLJR_to_Y800(Stage* in, Stage* out);
    void CLJR_to_Y16(Stage* in, Stage* out);
    void CLJR_to_Y41P(Stage* in, Stage* out);
    void CLJR_to_AYUV(Stage* in, Stage* out);
    void CLJR_to_IMC1(Stage* in, Stage* out);
    void CLJR_to_IMC2(Stage* in, Stage* out);
    void CLJR_to_IMC3(Stage* in, Stage* out);
    void CLJR_to_IMC4(Stage* in, Stage* out);
    void CLJR_to_NV12(Stage* in, Stage* out);
    void CLJR_to_NV21(Stage* in, Stage* out);
    void CLJR_to_Y42T(Stage* in, Stage* out);
    void CLJR_to_Y41T(Stage* in, Stage* out);
    void CLJR_to_YV16(Stage* in, Stage* out);

    void Y41P_to_YUY2(Stage* in, Stage* out);
    void Y41P_to_UYVY(Stage* in, Stage* out);
    void Y41P_to_YVYU(Stage* in, Stage* out);
    void Y41P_to_VYUY(Stage* in, Stage* out);
    void Y41P_to_I420(Stage* in, Stage* out);
    void Y41P_to_YV12(Stage* in, Stage* out);
    void Y41P_to_YVU9(Stage* in, Stage* out);
    void Y41P_to_YUV9(Stage* in, Stage* out);
    void Y41P_to_IYU1(Stage* in, Stage* out);
    void Y41P_to_IYU2(Stage* in, Stage* out);
    void Y41P_to_Y800(Stage* in, Stage* out);
    void Y41P_to_Y16(Stage* in, Stage* out);
    void Y41P_to_CLJR(Stage* in, Stage* out);
    void Y41P_to_AYUV(Stage* in, Stage* out);
    void Y41P_to_IMC1(Stage* in, Stage* out);
    void Y41P_to_IMC2(Stage* in, Stage* out);
    void Y41P_to_IMC3(Stage* in, Stage* out);
    void Y41P_to_IMC4(Stage* in, Stage* out);
    void Y41P_to_NV12(Stage* in, Stage* out);
    void Y41P_to_NV21(Stage* in, Stage* out);
    void Y41P_to_Y42T(Stage* in, Stage* out);
    void Y41P_to_Y41T(Stage* in, Stage* out);
    void Y41P_to_YV16(Stage* in, Stage* out);

    void IMC1_to_AYUV(Stage* in, Stage* out);
    void IMC1_to_YUY2(Stage* in, Stage* out);
    void IMC1_to_UYVY(Stage* in, Stage* out);
    void IMC1_to_YVYU(Stage* in, Stage* out);
    void IMC1_to_VYUY(Stage* in, Stage* out);
    void IMC1_to_I420(Stage* in, Stage* out);
    void IMC1_to_YV12(Stage* in, Stage* out);
    void IMC1_to_YUV9(Stage* in, Stage* out);
    void IMC1_to_YVU9(Stage* in, Stage* out);
    void IMC1_to_IYU1(Stage* in, Stage* out);
    void IMC1_to_IYU2(Stage* in, Stage* out);
    void IMC1_to_Y800(Stage* in, Stage* out);
    void IMC1_to_Y16(Stage* in, Stage* out);
    void IMC1_to_CLJR(Stage* in, Stage* out);
    void IMC1_to_Y41P(Stage* in, Stage* out);
    void IMC1_to_IMC2(Stage* in, Stage* out);
    void IMC1_to_IMC3(Stage* in, Stage* out);
    void IMC1_to_IMC4(Stage* in, Stage* out);
    void IMC1_to_NV12(Stage* in, Stage* out);
    void IMC1_to_NV21(Stage* in, Stage* out);
    void IMC1_to_Y42T(Stage* in, Stage* out);
    void IMC1_to_Y41T(Stage* in, Stage* out);
    void IMC1_to_YV16(Stage* in, Stage* out);

    void IMC2_to_AYUV(Stage* in, Stage* out);
    void IMC2_to_YUY2(Stage* in, Stage* out);
    void IMC2_to_UYVY(Stage* in, Stage* out);
    void IMC2_to_YVYU(Stage* in, Stage* out);
    void IMC2_to_VYUY(Stage* in, Stage* out);
    void IMC2_to_I420(Stage* in, Stage* out);
    void IMC2_to_YV12(Stage* in, Stage* out);
    void IMC2_to_YUV9(Stage* in, Stage* out);
    void IMC2_to_YVU9(Stage* in, Stage* out);
    void IMC2_to_IYU1(Stage* in, Stage* out);
    void IMC2_to_IYU2(Stage* in, Stage* out);
    void IMC2_to_Y800(Stage* in, Stage* out);
    void IMC2_to_Y16(Stage* in, Stage* out);
    void IMC2_to_CLJR(Stage* in, Stage* out);
    void IMC2_to_Y41P(Stage* in, Stage* out);
    void IMC2_to_IMC1(Stage* in, Stage* out);
    void IMC2_to_IMC3(Stage* in, Stage* out);
    void IMC2_to_IMC4(Stage* in, Stage* out);
    void IMC2_to_NV12(Stage* in, Stage* out);
    void IMC2_to_NV21(Stage* in, Stage* out);
    void IMC2_to_Y42T(Stage* in, Stage* out);
    void IMC2_to_Y41T(Stage* in, Stage* out);
    void IMC2_to_YV16(Stage* in, Stage* out);

    void IMC3_to_AYUV(Stage* in, Stage* out);
    void IMC3_to_YUY2(Stage* in, Stage* out);
    void IMC3_to_UYVY(Stage* in, Stage* out);
    void IMC3_to_YVYU(Stage* in, Stage* out);
    void IMC3_to_VYUY(Stage* in, Stage* out);
    void IMC3_to_I420(Stage* in, Stage* out);
    void IMC3_to_YV12(Stage* in, Stage* out);
    void IMC3_to_YUV9(Stage* in, Stage* out);
    void IMC3_to_YVU9(Stage* in, Stage* out);
    void IMC3_to_IYU1(Stage* in, Stage* out);
    void IMC3_to_IYU2(Stage* in, Stage* out);
    void IMC3_to_Y800(Stage* in, Stage* out);
    void IMC3_to_Y16(Stage* in, Stage* out);
    void IMC3_to_CLJR(Stage* in, Stage* out);
    void IMC3_to_Y41P(Stage* in, Stage* out);
    void IMC3_to_IMC1(Stage* in, Stage* out);
    void IMC3_to_IMC2(Stage* in, Stage* out);
    void IMC3_to_IMC4(Stage* in, Stage* out);
    void IMC3_to_NV12(Stage* in, Stage* out);
    void IMC3_to_NV21(Stage* in, Stage* out);
    void IMC3_to_Y42T(Stage* in, Stage* out);
    void IMC3_to_Y41T(Stage* in, Stage* out);
    void IMC3_to_YV16(Stage* in, Stage* out);

    void IMC4_to_AYUV(Stage* in, Stage* out);
    void IMC4_to_YUY2(Stage* in, Stage* out);
    void IMC4_to_UYVY(Stage* in, Stage* out);
    void IMC4_to_YVYU(Stage* in, Stage* out);
    void IMC4_to_VYUY(Stage* in, Stage* out);
    void IMC4_to_I420(Stage* in, Stage* out);
    void IMC4_to_YV12(Stage* in, Stage* out);
    void IMC4_to_YUV9(Stage* in, Stage* out);
    void IMC4_to_YVU9(Stage* in, Stage* out);
    void IMC4_to_IYU1(Stage* in, Stage* out);
    void IMC4_to_IYU2(Stage* in, Stage* out);
    void IMC4_to_Y800(Stage* in, Stage* out);
    void IMC4_to_Y16(Stage* in, Stage* out);
    void IMC4_to_CLJR(Stage* in, Stage* out);
    void IMC4_to_Y41P(Stage* in, Stage* out);
    void IMC4_to_IMC1(Stage* in, Stage* out);
    void IMC4_to_IMC2(Stage* in, Stage* out);
    void IMC4_to_IMC3(Stage* in, Stage* out);
    void IMC4_to_NV12(Stage* in, Stage* out);
    void IMC4_to_NV21(Stage* in, Stage* out);
    void IMC4_to_Y42T(Stage* in, Stage* out);
    void IMC4_to_Y41T(Stage* in, Stage* out);
    void IMC4_to_YV16(Stage* in, Stage* out);

    void NV12_to_AYUV(Stage* in, Stage* out);
    void NV12_to_YUY2(Stage* in, Stage* out);
    void NV12_to_UYVY(Stage* in, Stage* out);
    void NV12_to_YVYU(Stage* in, Stage* out);
    void NV12_to_VYUY(Stage* in, Stage* out);
    void NV12_to_I420(Stage* in, Stage* out);
    void NV12_to_YV12(Stage* in, Stage* out);
    void NV12_to_YUV9(Stage* in, Stage* out);
    void NV12_to_YVU9(Stage* in, Stage* out);
    void NV12_to_IYU1(Stage* in, Stage* out);
    void NV12_to_IYU2(Stage* in, Stage* out);
    void NV12_to_Y800(Stage* in, Stage* out);
    void NV12_to_Y16(Stage* in, Stage* out);
    void NV12_to_CLJR(Stage* in, Stage* out);
    void NV12_to_Y41P(Stage* in, Stage* out);
    void NV12_to_IMC1(Stage* in, Stage* out);
    void NV12_to_IMC2(Stage* in, Stage* out);
    void NV12_to_IMC3(Stage* in, Stage* out);
    void NV12_to_IMC4(Stage* in, Stage* out);
    void NV12_to_NV21(Stage* in, Stage* out);
    void NV12_to_Y42T(Stage* in, Stage* out);
    void NV12_to_Y41T(Stage* in, Stage* out);
    void NV12_to_YV16(Stage* in, Stage* out);

    void NV21_to_AYUV(Stage* in, Stage* out);
    void NV21_to_YUY2(Stage* in, Stage* out);
    void NV21_to_UYVY(Stage* in, Stage* out);
    void NV21_to_YVYU(Stage* in, Stage* out);
    void NV21_to_VYUY(Stage* in, Stage* out);
    void NV21_to_I420(Stage* in, Stage* out);
    void NV21_to_YV12(Stage* in, Stage* out);
    void NV21_to_YUV9(Stage* in, Stage* out);
    void NV21_to_YVU9(Stage* in, Stage* out);
    void NV21_to_IYU1(Stage* in, Stage* out);
    void NV21_to_IYU2(Stage* in, Stage* out);
    void NV21_to_Y800(Stage* in, Stage* out);
    void NV21_to_Y16(Stage* in, Stage* out);
    void NV21_to_CLJR(Stage* in, Stage* out);
    void NV21_to_Y41P(Stage* in, Stage* out);
    void NV21_to_IMC1(Stage* in, Stage* out);
    void NV21_to_IMC2(Stage* in, Stage* out);
    void NV21_to_IMC3(Stage* in, Stage* out);
    void NV21_to_IMC4(Stage* in, Stage* out);
    void NV21_to_NV12(Stage* in, Stage* out);
    void NV21_to_Y42T(Stage* in, Stage* out);
    void NV21_to_Y41T(Stage* in, Stage* out);
    void NV21_to_YV16(Stage* in, Stage* out);

    void Y42T_to_YUY2(Stage* in, Stage* out);
    void Y42T_to_UYVY(Stage* in, Stage* out);
    void Y42T_to_YVYU(Stage* in, Stage* out);
    void Y42T_to_VYUY(Stage* in, Stage* out);
    void Y42T_to_I420(Stage* in, Stage* out);
    void Y42T_to_YV12(Stage* in, Stage* out);
    void Y42T_to_YUV9(Stage* in, Stage* out);
    void Y42T_to_YVU9(Stage* in, Stage* out);
    void Y42T_to_IYU1(Stage* in, Stage* out);
    void Y42T_to_IYU2(Stage* in, Stage* out);
    void Y42T_to_Y800(Stage* in, Stage* out);
    void Y42T_to_Y16(Stage* in, Stage* out);
    void Y42T_to_CLJR(Stage* in, Stage* out);
    void Y42T_to_Y41P(Stage* in, Stage* out);
    void Y42T_to_AYUV(Stage* in, Stage* out);
    void Y42T_to_IMC1(Stage* in, Stage* out);
    void Y42T_to_IMC2(Stage* in, Stage* out);
    void Y42T_to_IMC3(Stage* in, Stage* out);
    void Y42T_to_IMC4(Stage* in, Stage* out);
    void Y42T_to_NV12(Stage* in, Stage* out);
    void Y42T_to_NV21(Stage* in, Stage* out);
    void Y42T_to_Y41T(Stage* in, Stage* out);
    void Y42T_to_YV16(Stage* in, Stage* out);

    void Y41T_to_YUY2(Stage* in, Stage* out);
    void Y41T_to_UYVY(Stage* in, Stage* out);
    void Y41T_to_YVYU(Stage* in, Stage* out);
    void Y41T_to_VYUY(Stage* in, Stage* out);
    void Y41T_to_I420(Stage* in, Stage* out);
    void Y41T_to_YV12(Stage* in, Stage* out);
    void Y41T_to_YUV9(Stage* in, Stage* out);
    void Y41T_to_YVU9(Stage* in, Stage* out);
    void Y41T_to_IYU1(Stage* in, Stage* out);
    void Y41T_to_IYU2(Stage* in, Stage* out);
    void Y41T_to_Y800(Stage* in, Stage* out);
    void Y41T_to_Y16(Stage* in, Stage* out);
    void Y41T_to_CLJR(Stage* in, Stage* out);
    void Y41T_to_Y41P(Stage* in, Stage* out);
    void Y41T_to_AYUV(Stage* in, Stage* out);
    void Y41T_to_IMC1(Stage* in, Stage* out);
    void Y41T_to_IMC2(Stage* in, Stage* out);
    void Y41T_to_IMC3(Stage* in, Stage* out);
    void Y41T_to_IMC4(Stage* in, Stage* out);
    void Y41T_to_NV12(Stage* in, Stage* out);
    void Y41T_to_NV21(Stage* in, Stage* out);
    void Y41T_to_Y42T(Stage* in, Stage* out);
    void Y41T_to_YV16(Stage* in, Stage* out);

    void YV16_to_YUY2(Stage* in, Stage* out);
    void YV16_to_UYVY(Stage* in, Stage* out);
    void YV16_to_YVYU(Stage* in, Stage* out);
    void YV16_to_VYUY(Stage* in, Stage* out);
    void YV16_to_I420(Stage* in, Stage* out);
    void YV16_to_YV12(Stage* in, Stage* out);
    void YV16_to_YUV9(Stage* in, Stage* out);
    void YV16_to_YVU9(Stage* in, Stage* out);
    void YV16_to_IYU1(Stage* in, Stage* out);
    void YV16_to_IYU2(Stage* in, Stage* out);
    void YV16_to_Y800(Stage* in, Stage* out);
    void YV16_to_Y16(Stage* in, Stage* out);
    void YV16_to_CLJR(Stage* in, Stage* out);
    void YV16_to_Y41P(Stage* in, Stage* out);
    void YV16_to_AYUV(Stage* in, Stage* out);
    void YV16_to_IMC1(Stage* in, Stage* out);
    void YV16_to_IMC2(Stage* in, Stage* out);
    void YV16_to_IMC3(Stage* in, Stage* out);
    void YV16_to_IMC4(Stage* in, Stage* out);
    void YV16_to_NV12(Stage* in, Stage* out);
    void YV16_to_NV21(Stage* in, Stage* out);
    void YV16_to_Y42T(Stage* in, Stage* out);
    void YV16_to_Y41T(Stage* in, Stage* out);

    // Interlaced versions of common YUV formats for what?
    void UYVY_to_IUYV(Stage* in, Stage* out);
    void IUYV_to_UYVY(Stage* in, Stage* out);
    void IY41_to_Y41P(Stage* in, Stage* out);
    void Y41P_to_IY41(Stage* in, Stage* out);
}

