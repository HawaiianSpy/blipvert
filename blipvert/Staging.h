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
#include "blipvert.h"
#include <memory>
#include <vector>

namespace blipvert
{
    typedef struct Stage {
        const MediaFormatID* format;
        int32_t width;
        int32_t height;
        uint8_t* buf;
        int32_t stride;
        int32_t stride_x_2;
        int32_t stride_x_3;
        int32_t stride_x_4;
        bool flipped;
        xRGBQUAD* palette;
        bool has_odd;
        uint16_t remainder;
        int16_t Y0_index;
        int16_t Y1_index;
        int16_t U_index;
        int16_t V_index;
        int32_t uv_width;
        int32_t uv_height;
        int32_t y_stride;
        int32_t uv_stride;
        uint8_t* vplane;
        uint8_t* uplane;
        uint8_t* uvplane;
    } Stage;

    typedef struct TransformStage {
        std::shared_ptr<Stage> in;
        std::shared_ptr<Stage> out;
    } TransformStage;

    std::shared_ptr<Stage> Stage_RGBA(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_RGB32(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_RGB24(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_RGB565(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_RGB555(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_ARGB1555(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_RGB8(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_RGB4(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_RGB1(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);

    std::shared_ptr<Stage> Stage_YUY2(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_UYVY(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_YVYU(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_VYUY(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_I420(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_YV12(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_YVU9(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_YUV9(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_IYU1(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_IYU2(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_Y41P(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_CLJR(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_Y800(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_Y16(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_AYUV(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_IMC1(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_IMC2(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_IMC3(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_IMC4(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_NV12(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_NV21(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_Y42T(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_Y41T(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
    std::shared_ptr<Stage> Stage_YV16(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped = false, xRGBQUAD* palette = nullptr);
}
