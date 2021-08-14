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

#include "pch.h"
#include "Staging.h"
#include "LookupTables.h"

#include <cstring>
#include <map>
#include <memory>

using namespace blipvert;
using namespace std;

map<MediaFormatID, t_stagetransformfunc> StagingMap = {
    { MVFMT_RGBA, Stage_RGBA },
    { MVFMT_RGB32, Stage_RGB32 },
    { MVFMT_RGB24, Stage_RGB24 },
    { MVFMT_RGB565, Stage_RGB565 },
    { MVFMT_RGB555, Stage_RGB555 },
    { MVFMT_ARGB1555, Stage_ARGB1555 },
    { MVFMT_RGB8, Stage_RGB8 },
    { MVFMT_RGB4, Stage_RGB4 },
    { MVFMT_RGB1, Stage_RGB1 },
    { MVFMT_YUY2, Stage_YUY2 },
    { MVFMT_UYVY, Stage_UYVY },
    { MVFMT_YVYU, Stage_YVYU },
    { MVFMT_VYUY, Stage_VYUY },
    { MVFMT_I420, Stage_I420 },
    { MVFMT_YV12, Stage_YV12 },
    { MVFMT_YVU9, Stage_YVU9 },
    { MVFMT_YUV9, Stage_YUV9 },
    { MVFMT_IYU1, Stage_IYU1 },
    { MVFMT_IYU2, Stage_IYU2 },
    { MVFMT_Y41P, Stage_Y41P },
    { MVFMT_CLJR, Stage_CLJR },
    { MVFMT_Y800, Stage_Y800 },
    { MVFMT_Y16, Stage_Y16 },
    { MVFMT_AYUV, Stage_AYUV },
    { MVFMT_IMC1, Stage_IMC1 },
    { MVFMT_IMC2, Stage_IMC2 },
    { MVFMT_IMC3, Stage_IMC3 },
    { MVFMT_IMC4, Stage_IMC4 },
    { MVFMT_NV12, Stage_NV12 },
    { MVFMT_NV21, Stage_NV21 },
    { MVFMT_Y42T, Stage_Y42T },
    { MVFMT_Y41T, Stage_Y41T },
    { MVFMT_YV16, Stage_YV16 }
};

t_stagetransformfunc blipvert::FindTransformStage(const MediaFormatID& inFormat)
{
    map<MediaFormatID, t_stagetransformfunc>::iterator it = StagingMap.find(inFormat);
    if (it != StagingMap.end())
    {
        return *(it->second);
    }

    // Not found, so try cross-referenced formats in case there's a known duplicate definition.

    VideoFormatInfo inInfo;
    if (GetVideoFormatInfo(inFormat, inInfo))
    {
        MediaFormatID inid;
        if (GetVideoFormatID(inInfo.xRefFourcc, inid))
        {
            map<MediaFormatID, t_stagetransformfunc>::iterator it = StagingMap.find(inid);
            if (it != StagingMap.end())
            {
                return *(it->second);
            }
        }
    }

    return nullptr;
}

void blipvert::Stage_RGBA(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_RGBA;
    result->thread_index = thread_index;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 4)
        result->stride = width * 4;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_RGB32(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
     memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_RGB32;
    result->thread_index = thread_index;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 4)
        result->stride = width * 4;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_RGB24(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_RGB24;
    result->thread_index = thread_index;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 3)
        result->stride = width * 3;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_RGB565(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_RGB565;
    result->thread_index = thread_index;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 2)
        result->stride = width * 2;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_RGB555(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_RGB555;
    result->thread_index = thread_index;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 2)
        result->stride = width * 2;
    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_ARGB1555(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_ARGB1555;
    result->thread_index = thread_index;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 2)
        result->stride = width * 2;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_RGB8(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_RGB8;
    result->thread_index = thread_index;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width)
        result->stride = width;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_RGB4(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_RGB4;
    result->thread_index = thread_index;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    result->has_odd = width % 2 != 0;
    if (result->stride < width / 2)
    {
        result->stride = width / 2;
        if (result->has_odd)
        {
            result->stride++;
        }
    }

    if (result->palette == nullptr)
        result->palette = rgb4_greyscale_palette;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_RGB1(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_RGB1;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;
    result->palette = palette;

    result->remainder = width % 8;
    if (result->stride < width / 8)
    {
        result->stride = width / 8;
        if (result->remainder)
        {
            result->stride++;
        }
    }

    if (result->palette == nullptr)
        result->palette = rgb1_greyscale_palette;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

shared_ptr<Stage> Stage_PackedY422(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 2)
        result->stride = width * 2;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_YUY2(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_PackedY422(result, thread_index, thread_count, width, height, buf, stride, flipped);

    result->format = &MVFMT_YUY2;
    result->Y0_index = 0;
    result->Y1_index = 2;
    result->U_index = 1;
    result->V_index = 2;
}

void blipvert::Stage_UYVY(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_PackedY422(result, thread_index, thread_count, width, height, buf, stride, flipped);

    result->format = &MVFMT_UYVY;
    result->Y0_index = 1;
    result->Y1_index = 3;
    result->U_index = 0;
    result->V_index = 2;
}

void blipvert::Stage_YVYU(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_PackedY422(result, thread_index, thread_count, width, height, buf, stride, flipped);

    result->format = &MVFMT_YVYU;
    result->Y0_index = 0;
    result->Y1_index = 2;
    result->U_index = 3;
    result->V_index = 1;
}

void blipvert::Stage_VYUY(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_PackedY422(result, thread_index, thread_count, width, height, buf, stride, flipped);

    result->format = &MVFMT_VYUY;
    result->Y0_index = 1;
    result->Y1_index = 3;
    result->U_index = 2;
    result->V_index = 0;
}


shared_ptr<Stage> Stage_PlanarYUV(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, bool ufirst, int32_t decimation)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->width = width;
    result->height = slice_height;
    result->flipped = flipped;

    result->uv_width = width / decimation;
    result->uv_height = height / decimation;
    result->uv_slice_height = result->uv_height / thread_count;

    if (stride <= width)
    {
        result->y_stride = width;
        result->uv_stride = result->uv_width;
    }
    else
    {
        result->y_stride = stride;
        result->uv_stride = stride;
    }

    if (flipped)
    {
        result->buf = buf + (result->y_stride * ((height - 1) - thread_index * slice_height));

        int32_t offset_from_bottom = (result->uv_stride * ((result->uv_height - 1) - thread_index * result->uv_slice_height));

        if (ufirst)
        {
            uint8_t* ubuf = buf + (result->y_stride * height);
            result->uplane = ubuf + offset_from_bottom;
            uint8_t* vbuf = ubuf + (result->uv_stride * result->uv_height);
            result->vplane = vbuf + offset_from_bottom;
        }
        else
        {
            uint8_t* vbuf = buf + (result->y_stride * height);
            result->vplane = vbuf + offset_from_bottom;
            uint8_t* ubuf = vbuf + (result->uv_stride * result->uv_height);
            result->uplane = ubuf + offset_from_bottom;
        }

        result->uv_stride = -result->uv_stride;
        result->y_stride = -result->y_stride;

    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->y_stride;

        int32_t offset_from_top = thread_index * result->uv_slice_height * result->uv_stride;

        if (ufirst)
        {
            uint8_t* ubuf = buf + (result->y_stride * height);
            result->uplane = ubuf + offset_from_top;
            uint8_t* vbuf = ubuf + (result->uv_stride * result->uv_height);
            result->vplane = vbuf + offset_from_top;
        }
        else
        {
            uint8_t* vbuf = buf + (result->y_stride * height);
            result->vplane = vbuf + offset_from_top;
            uint8_t* ubuf = vbuf + (result->uv_stride * result->uv_height);
            result->uplane = ubuf + offset_from_top;
        }
    }
}

void blipvert::Stage_I420(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_PlanarYUV(result, thread_index, thread_count, width, height, buf, stride, flipped, true, 2);
    result->format = &MVFMT_I420;
}

void blipvert::Stage_YV12(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_PlanarYUV(result, thread_index, thread_count, width, height, buf, stride, flipped, false, 2);
    result->format = &MVFMT_YV12;
}

void blipvert::Stage_YVU9(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_PlanarYUV(result, thread_index, thread_count, width, height, buf, stride, flipped, false, 4);
    result->format = &MVFMT_YVU9;
}

void blipvert::Stage_YUV9(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette )
{
    Stage_PlanarYUV(result, thread_index, thread_count, width, height, buf, stride, flipped, true, 4);
    result->format = &MVFMT_YUV9;
}

void blipvert::Stage_IYU1(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_IYU1;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 12 / 8)
        result->stride = width * 12 / 8;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_IYU2(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_IYU2;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 3)
        result->stride = width * 3;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_Y41P(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_Y41P;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width / 8 * 12)
        result->stride = width / 8 * 12;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_CLJR(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_CLJR;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width)
        result->stride = width;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_Y800(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_Y800;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width)
        result->stride = width;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_Y16(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_Y16;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 2)
        result->stride = width * 2;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_AYUV(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_Y16;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 4)
        result->stride = width * 4;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

shared_ptr<Stage> Stage_IMCx(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, bool ufirst, bool interlaced)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    result->uv_width = width / 2;
    result->uv_height = height / 2;
    result->uv_slice_height = result->uv_height / thread_count;

    if (result->stride < width)
        result->stride = width;

    if (flipped)
    {
        result->buf = buf + (result->y_stride * ((height - 1) - thread_index * slice_height));

        int32_t offset_from_bottom = result->stride * ((result->uv_height - 1) - thread_index * result->uv_slice_height);

        if (ufirst)
        {
            if (interlaced)
            {
                uint8_t* ubuf = buf + (result->stride * height);
                result->uplane = ubuf + offset_from_bottom;
                uint8_t* vbuf = ubuf + result->uv_width;
                result->vplane = vbuf + offset_from_bottom;
            }
            else
            {
                uint8_t* ubuf = buf + (((height + 15) & ~15) * result->stride);
                result->uplane = ubuf + offset_from_bottom;
                uint8_t* vbuf = buf + (((((height * 3) / 2) + 15) & ~15) * result->stride);
                result->vplane = vbuf + offset_from_bottom;
            }
        }
        else
        {
            if (interlaced)
            {
                uint8_t* vbuf = buf + (result->stride * height);
                result->vplane = vbuf + offset_from_bottom;
                uint8_t* ubuf = vbuf + result->uv_width;
                result->uplane = ubuf + offset_from_bottom;
            }
            else
            {
                uint8_t* vbuf = buf + (((height + 15) & ~15) * result->stride);
                result->vplane = vbuf + offset_from_bottom;
                uint8_t* ubuf = buf + (((((height * 3) / 2) + 15) & ~15) * result->stride);
                result->uplane = ubuf + offset_from_bottom;
            }
        }

        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;

        int32_t offset_from_top = thread_index * result->uv_slice_height * result->stride;

        if (ufirst)
        {
            if (interlaced)
            {
                uint8_t* ubuf = buf + (result->stride * height);
                result->uplane = ubuf + offset_from_top;
                uint8_t* vbuf = ubuf + result->uv_width;
                result->vplane = vbuf + offset_from_top;
            }
            else
            {
                uint8_t* ubuf = buf + (((height + 15) & ~15) * result->stride);
                result->uplane = ubuf + offset_from_top;
                uint8_t* vbuf = buf + (((((height * 3) / 2) + 15) & ~15) * result->stride);
                result->vplane = vbuf + offset_from_top;
            }
        }
        else
        {
            if (interlaced)
            {
                uint8_t* vbuf = buf + (result->stride * height);
                result->vplane = vbuf + offset_from_top;
                uint8_t* ubuf = vbuf + result->uv_width;
                result->uplane = ubuf + offset_from_top;
            }
            else
            {
                uint8_t* vbuf = buf + (((height + 15) & ~15) * result->stride);
                result->vplane = vbuf + offset_from_top;
                uint8_t* ubuf = buf + (((((height * 3) / 2) + 15) & ~15) * result->stride);
                result->uplane = ubuf + offset_from_top;
            }
        }
    }
}

void blipvert::Stage_IMC1(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_IMCx(result, thread_index, thread_count, width, height, buf, stride, flipped, false, false);
    result->format = &MVFMT_IMC1;
}

void blipvert::Stage_IMC2(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_IMCx(result, thread_index, thread_count, width, height, buf, stride, flipped, false, true);
    result->format = &MVFMT_IMC2;
}

void blipvert::Stage_IMC3(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_IMCx(result, thread_index, thread_count, width, height, buf, stride, flipped, true, false);
    result->format = &MVFMT_IMC3;
}

void blipvert::Stage_IMC4(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_IMCx(result, thread_index, thread_count, width, height, buf, stride, flipped, true, true);
    result->format = &MVFMT_IMC4;
}

shared_ptr<Stage> Stage_NVx(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, bool ufirst)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->width = width;
    result->height = slice_height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;

    result->uv_width = width;
    result->uv_height = height / 2;
    result->uv_slice_height = result->uv_height / thread_count;

    if (result->stride < width)
        result->stride = width;

    if (ufirst)
    {
        result->U_index = 0;
        result->V_index = 1;
    }
    else
    {
        result->U_index = 1;
        result->V_index = 0;
    }

    uint8_t* uvbuf = buf + (result->stride * height);

    if (flipped)
    {
        result->buf = buf + (result->y_stride * ((height - 1) - thread_index * slice_height));
        result->uvplane = uvbuf + result->stride * ((result->uv_height - 1) - thread_index * result->uv_slice_height);
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
        result->uvplane = uvbuf + thread_index * result->uv_slice_height * result->stride;
    }
}

void blipvert::Stage_NV12(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_NVx(result, thread_index, thread_count, width, height, buf, stride, flipped, true);
    result->format = &MVFMT_NV12;
}

void blipvert::Stage_NV21(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    Stage_NVx(result, thread_index, thread_count, width, height, buf, stride, flipped, false);
    result->format = &MVFMT_NV21;
}

void blipvert::Stage_Y42T(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_Y42T;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width * 2)
        result->stride = width * 2;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_Y41T(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_Y41T;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < width / 8 * 12)
        result->stride = width / 8 * 12;

    if (result->flipped)
    {
        result->buf = buf + (result->stride * ((height - 1) - thread_index * (slice_height)));
        result->stride = -result->stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->stride;
    }
}

void blipvert::Stage_YV16(Stage* result, uint8_t thread_index, uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    memset(result, 0, sizeof(Stage));

    int32_t slice_height = height / thread_count;

    result->format = &MVFMT_YV16;
    result->width = width;
    result->height = slice_height;
    result->stride = stride;
    result->flipped = flipped;

    result->uv_width = width / 2;

    if (result->stride <= width)
    {
        result->y_stride = width;
        result->uv_stride = result->uv_width;
    }
    else
    {
        result->y_stride = result->stride;
        result->uv_stride = result->stride;
    }

    if (flipped)
    {
        result->buf = buf + (result->y_stride * ((height - 1) - thread_index * slice_height));

        int32_t offset_from_bottom = result->uv_stride * ((height - 1) - thread_index * slice_height);

        uint8_t* vbuf = buf + (result->y_stride * height);
        result->vplane = vbuf + offset_from_bottom;
        uint8_t* ubuf = vbuf + (result->uv_stride * height);
        result->uplane = ubuf + offset_from_bottom;

        result->y_stride = -result->y_stride;
        result->uv_stride = -result->uv_stride;
    }
    else
    {
        result->buf = buf + thread_index * slice_height * result->y_stride;

        int32_t offset_from_top = thread_index * slice_height * result->uv_stride;
        uint8_t* vbuf = buf + (result->y_stride * height);
        result->vplane = vbuf + offset_from_top;
        uint8_t* ubuf = vbuf + (result->uv_stride * height);
        result->uplane = ubuf + offset_from_top;
    }
}
