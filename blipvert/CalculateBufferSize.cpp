/
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
#include "CalculateBufferSize.h"
#include "blipvert.h"

using namespace blipvert;

uint32_t blipvert::CalcBufferSize_RGBA(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 4)
    {
        stride = width * 4;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_RGB32(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 4)
    {
        stride = width * 4;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_RGB24(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 3)
    {
        stride = width * 3;
    }

    uint32_t result = height * stride;
    if (UseFasterLooping) result++;
    return result;
}

uint32_t blipvert::CalcBufferSize_RGB565(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 2)
    {
        stride = width * 2;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_RGB555(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 2)
    {
        stride = width * 2;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_ARGB1555(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 2)
    {
        stride = width * 2;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_RGB8(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width)
    {
        stride = width;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_RGB4(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width / 2)
    {
        stride = width / 2;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_RGB1(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width / 8)
    {
        stride = width / 8;
    }

    return height * stride;
}


uint32_t blipvert::CalcBufferSize_AYUV(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 4)
    {
        stride = width * 4;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_UYVY(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 2)
    {
        stride = width * 2;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_YVYU(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 2)
    {
        stride = width * 2;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_VYUY(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 2)
    {
        stride = width * 2;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_YUY2(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 2)
    {
        stride = width * 2;
    }

    return height * stride;
}

uint32_t CalcBufferSize_PlanarYUV(int32_t width, int32_t height, int32_t& stride, int32_t decimation)
{
    int32_t uv_width = width / decimation;
    int32_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (stride <= width)
    {
        stride = width;
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = stride;
        uv_stride = stride;
    }

    uint32_t result = height * y_stride;
    result += uv_stride * uv_height * 2;
    return result;
}

uint32_t blipvert::CalcBufferSize_I420(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_PlanarYUV(width, height, stride, 2);
}

uint32_t blipvert::CalcBufferSize_YV12(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_PlanarYUV(width, height, stride, 2);
}

uint32_t blipvert::CalcBufferSize_YVU9(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_PlanarYUV(width, height, stride, 4);
}

uint32_t blipvert::CalcBufferSize_YUV9(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_PlanarYUV(width, height, stride, 4);
}

uint32_t blipvert::CalcBufferSize_IYU1(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 12 / 8)
    {
        stride = width * 12 / 8;
    }
    return height * stride;

}

uint32_t blipvert::CalcBufferSize_IYU2(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 3)
    {
        stride = width * 3;
    }

    uint32_t result = height * stride;
    if (UseFasterLooping) result++;
    return result;
}

uint32_t blipvert::CalcBufferSize_Y800(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width)
    {
        stride = width;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_Y16(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width * 2)
    {
        stride = width * 2;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_Y41P(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width / 8 * 12)
    {
        stride = width / 8 * 12;
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_CLJR(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width)
    {
        stride = width;
    }

    return height * stride;
}

uint32_t CalcBufferSize_IMCx(int32_t width, int32_t height, int32_t& stride, bool interlaced)
{
    int32_t uv_height = height / 2;

    if (stride < width)
        stride = width;
    
    if (interlaced)
    {
        height += uv_height;
    }
    else
    {
        // Use the line # of the last UV plane on 16 line boundry + the height of the UV plane
        // for the actual number of vertical lines in the bitmap.
        height = (((((height * 3) / 2) + 15) & (~15)) + height / 2);
    }

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_IMC1(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_IMCx(width, height, stride, false);
}

uint32_t blipvert::CalcBufferSize_IMC2(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_IMCx(width, height, stride, true);
}

uint32_t blipvert::CalcBufferSize_IMC3(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_IMCx(width, height, stride, false);
}

uint32_t blipvert::CalcBufferSize_IMC4(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_IMCx(width, height, stride, true);
}

uint32_t blipvert::CalcBufferSize_NV12(int32_t width, int32_t height, int32_t& stride)
{
    if (stride < width)
        stride = width;

    height += height / 2;

    return height * stride;
}

uint32_t blipvert::CalcBufferSize_NV21(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_NV12(width, height, stride);
}

uint32_t blipvert::CalcBufferSize_Y42T(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_UYVY(width, height, stride);
}

uint32_t blipvert::CalcBufferSize_Y41T(int32_t width, int32_t height, int32_t& stride)
{
    return CalcBufferSize_Y41P(width, height, stride);
}

uint32_t blipvert::CalcBufferSize_YV16(int32_t width, int32_t height, int32_t& stride)
{
    int32_t uv_width = width / 2;

    int16_t y_stride, uv_stride;
    if (stride <= width)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = stride;
        uv_stride = stride;
    }

    return static_cast<uint32_t>(y_stride * height + (uv_stride * height * 2));
}
