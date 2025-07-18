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

#include "pch.h"
#include "FlipVertical.h"
#include "blipvert.h"
#include "CommonMacros.h"

using namespace blipvert;

void FlipSinglePlane(int32_t height, uint8_t* buf, int32_t stride)
{
    uint8_t* yp_top = buf;
    uint8_t* yp_bottom = buf + (stride * (height - 1));
    int32_t count = stride / sizeof(uint32_t);
    int32_t remainder = stride % sizeof(uint32_t);
    for (int32_t y = 0; y < height / 2; y++)
    {
        uint32_t* xp_top = reinterpret_cast<uint32_t*>(yp_top);
        uint32_t* xp_bottom = reinterpret_cast<uint32_t*>(yp_bottom);

        for (int32_t x = 0; x < count; x++)
        {
            uint32_t temp = *xp_top;
            *xp_top++ = *xp_bottom;
            *xp_bottom++ = temp;
        }

        if (remainder)
        {
            uint8_t* r_top = reinterpret_cast<uint8_t*>(xp_top);
            uint8_t* r_bottom = reinterpret_cast<uint8_t*>(xp_bottom);
            for (int32_t x = 0; x < remainder; x++)
            {
                uint8_t temp = *r_top;
                *r_top++ = *r_bottom;
                *r_bottom++ = temp;
            }
        }

        yp_top += stride;
        yp_bottom -= stride;
    }
}

void blipvert::FlipVertical_RGBA(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 4;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_RGB32(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 4;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_RGB24(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 3;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_RGB565(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_RGB555(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_ARGB1555(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_AYUV(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 4;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_UYVY(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_YVYU(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_VYUY(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_YUY2(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    FlipSinglePlane(height, buf, stride);
}

void FlipVertical_PlanarYUV(int32_t width, int32_t height, uint8_t* buf, int32_t stride, int32_t decimation)
{
    int32_t uv_width = width / decimation;
    int32_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = stride;
        uv_stride = stride;
    }

    uint8_t* uplane = buf + (y_stride * height);
    uint8_t* vplane = uplane + (uv_stride * uv_height);

    FlipSinglePlane(height, buf, y_stride);
    FlipSinglePlane(uv_height, uplane, uv_stride);
    FlipSinglePlane(uv_height, vplane, uv_stride);
}


void blipvert::FlipVertical_I420(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_PlanarYUV(width, height, buf, stride, 2);
}

void blipvert::FlipVertical_YV12(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_PlanarYUV(width, height, buf, stride, 2);
}

void blipvert::FlipVertical_YVU9(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_PlanarYUV(width, height, buf, stride, 4);
}

void blipvert::FlipVertical_YUV9(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_PlanarYUV(width, height, buf, stride, 4);
}

void blipvert::FlipVertical_IYU1(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 12 / 8;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_IYU2(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 3;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_Y800(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_Y16(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_Y41P(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width / 8 * 12;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_CLJR(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width;

    FlipSinglePlane(height, buf, stride);
}

void FlipVertical_IMCx(int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool ufirst, bool interlaced)
{
    if (!stride)
        stride = width;

    int32_t uv_width = width / 2;
    int32_t uv_height = height / 2;

    uint8_t* vplane = nullptr;
    uint8_t* uplane = nullptr;
    uint8_t* uvplane = nullptr;

    if (ufirst)
    {
        if (interlaced)
        {
            uplane = buf + (stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            int32_t uoffset = Align16(height);
            uplane = buf + (uoffset * stride);
            vplane = buf + (Align16(uoffset + uv_height) * stride);
        }
        uvplane = uplane;
    }
    else
    {
        if (interlaced)
        {
            vplane = buf + (stride * height);
            uplane = vplane + uv_width;
        }
        else
        {
            int32_t voffset = Align16(height);
            vplane = buf + (voffset * stride);
            uplane = buf + (Align16(voffset + uv_height) * stride);
        }
        uvplane = vplane;
    }

    FlipSinglePlane(height, buf, stride);
    if (interlaced)
    {
        FlipSinglePlane(uv_height, uvplane, stride);
    }
    else
    {
        FlipSinglePlane(uv_height, uplane, stride);
        FlipSinglePlane(uv_height, vplane, stride);
    }
}

void blipvert::FlipVertical_IMC1(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_IMCx(width, height, buf, stride, false, false);
}

void blipvert::FlipVertical_IMC2(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_IMCx(width, height, buf, stride, false, true);
}

void blipvert::FlipVertical_IMC3(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_IMCx(width, height, buf, stride, true, false);
}

void blipvert::FlipVertical_IMC4(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_IMCx(width, height, buf, stride, true, true);
}

void FlipVertical_NVx(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width;

    int32_t uv_width = width;
    int32_t uv_height = height / 2;

    uint8_t* uvplane = buf + (stride * height);

    FlipSinglePlane(height, buf, stride);
    FlipSinglePlane(uv_height, uvplane, stride);
}

void blipvert::FlipVertical_NV12(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_NVx(width, height, buf, stride);
}

void blipvert::FlipVertical_NV21(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    FlipVertical_NVx(width, height, buf, stride);
}

void blipvert::FlipVertical_Y42T(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_Y41T(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width / 8 * 12;

    FlipSinglePlane(height, buf, stride);
}

void blipvert::FlipVertical_YV16(int32_t width, int32_t height, uint8_t* buf, int32_t stride)
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

    uint8_t* vplane = buf + (y_stride * height);
    uint8_t* uplane = vplane + (uv_stride * height);

    FlipSinglePlane(height, buf, y_stride);
    FlipSinglePlane(height, vplane, uv_stride);
    FlipSinglePlane(height, uplane, uv_stride);
}
