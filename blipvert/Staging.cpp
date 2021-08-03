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

using namespace blipvert;
using namespace std;

shared_ptr<TransformStaging> blipvert::Stage_RGBA(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    shared_ptr<TransformStaging> result = make_shared<TransformStaging>();
    memset(result.get(), 0, sizeof(TransformStaging));

    result->thread_index = 0;
    result->format = &MVFMT_RGBA;
    result->width = width;
    result->height = height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < result->width)
        result->stride = result->width * 4;

    if (result->flipped)
    {
        result->buf += (result->stride * (result->height - 1));
        result->stride = -result->stride;
    }

    return result;
}

shared_ptr<TransformStaging> blipvert::Stage_RGB32(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    shared_ptr<TransformStaging> result = make_shared<TransformStaging>();
    memset(result.get(), 0, sizeof(TransformStaging));

    result->thread_index = 0;
    result->format = &MVFMT_RGB32;
    result->width = width;
    result->height = height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < result->width)
        result->stride = result->width * 4;

    if (result->flipped)
    {
        result->buf += (result->stride * (result->height - 1));
        result->stride = -result->stride;
    }

    return result;
}

shared_ptr<TransformStaging> blipvert::Stage_RGB24(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    shared_ptr<TransformStaging> result = make_shared<TransformStaging>();
    memset(result.get(), 0, sizeof(TransformStaging));

    result->thread_index = 0;
    result->format = &MVFMT_RGB24;
    result->width = width;
    result->height = height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < result->width)
        result->stride = result->width * 3;

    if (result->flipped)
    {
        result->buf += (result->stride * (result->height - 1));
        result->stride = -result->stride;
    }

    return result;
}

shared_ptr<TransformStaging> blipvert::Stage_RGB565(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    shared_ptr<TransformStaging> result = make_shared<TransformStaging>();
    memset(result.get(), 0, sizeof(TransformStaging));

    result->thread_index = 0;
    result->format = &MVFMT_RGB565;
    result->width = width;
    result->height = height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < result->width)
        result->stride = result->width * 2;

    if (result->flipped)
    {
        result->buf += (result->stride * (result->height - 1));
        result->stride = -result->stride;
    }

    return result;
}

shared_ptr<TransformStaging> blipvert::Stage_RGB555(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    shared_ptr<TransformStaging> result = make_shared<TransformStaging>();
    memset(result.get(), 0, sizeof(TransformStaging));

    result->thread_index = 0;
    result->format = &MVFMT_RGB555;
    result->width = width;
    result->height = height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < result->width)
        result->stride = result->width * 2;

    if (result->flipped)
    {
        result->buf += (result->stride * (result->height - 1));
        result->stride = -result->stride;
    }

    return result;
}

shared_ptr<TransformStaging> blipvert::Stage_ARGB1555(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    shared_ptr<TransformStaging> result = make_shared<TransformStaging>();
    memset(result.get(), 0, sizeof(TransformStaging));

    result->thread_index = 0;
    result->format = &MVFMT_ARGB1555;
    result->width = width;
    result->height = height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;

    if (result->stride < result->width)
        result->stride = result->width * 2;

    if (result->flipped)
    {
        result->buf += (result->stride * (result->height - 1));
        result->stride = -result->stride;
    }

    return result;
}

shared_ptr<TransformStaging> blipvert::Stage_RGB8(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    shared_ptr<TransformStaging> result = make_shared<TransformStaging>();
    memset(result.get(), 0, sizeof(TransformStaging));

    result->thread_index = 0;
    result->format = &MVFMT_RGB8;
    result->width = width;
    result->height = height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;
    result->palette = palette;

    if (result->stride < result->width)
        result->stride = result->width;

    if (result->palette == nullptr)
        result->palette = rgb8_greyscale_palette;

    if (result->flipped)
    {
        result->buf += (result->stride * (result->height - 1));
        result->stride = -result->stride;
    }

    return result;
}

shared_ptr<TransformStaging> blipvert::Stage_RGB4(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    shared_ptr<TransformStaging> result = make_shared<TransformStaging>();
    memset(result.get(), 0, sizeof(TransformStaging));

    result->thread_index = 0;
    result->format = &MVFMT_RGB4;
    result->width = width;
    result->height = height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;
    result->palette = palette;

    if (result->stride < result->width)
        result->stride = result->width;

    result->has_odd = width % 2 != 0;
    if (!result->stride)
    {
        result->stride = width / 2;
        if (result->has_odd)
        {
            result->stride++;
        }
    }

    if (result->palette == nullptr)
        result->palette = rgb8_greyscale_palette;

    if (result->flipped)
    {
        result->buf += (result->stride * (result->height - 1));
        result->stride = -result->stride;
    }

    return result;
}

shared_ptr<TransformStaging> blipvert::Stage_RGB1(uint8_t thread_count, int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool flipped, xRGBQUAD* palette)
{
    shared_ptr<TransformStaging> result = make_shared<TransformStaging>();
    memset(result.get(), 0, sizeof(TransformStaging));

    result->thread_index = 0;
    result->format = &MVFMT_RGB1;
    result->width = width;
    result->height = height;
    result->buf = buf;
    result->stride = stride;
    result->flipped = flipped;
    result->palette = palette;

    if (result->stride < result->width)
        result->stride = result->width;

    result->remainder = width % 8;
    if (!result->stride)
    {
        result->stride = width / 8;
        if (result->remainder)
        {
            result->stride++;
        }
    }

    if (result->palette == nullptr)
        result->palette = rgb8_greyscale_palette;

    if (result->flipped)
    {
        result->buf += (result->stride * (result->height - 1));
        result->stride = -result->stride;
    }

    return result;
}
