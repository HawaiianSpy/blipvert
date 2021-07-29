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
#include "SetPixel.h"
#include "blipvert.h"
#include "CommonMacros.h"

using namespace blipvert;

void blipvert::SetPixel_RGBA(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 4;

    uint8_t* pixel = buf + (y * stride) + (x * 4);
    pixel[0] = bv_level;
    pixel[1] = gu_level;
    pixel[2] = ry_level;
    pixel[3] = alpha;
}

void blipvert::SetPixel_RGB32(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 4;

    uint8_t* pixel = buf + (y * stride) + (x * 4);
    pixel[0] = bv_level;
    pixel[1] = gu_level;
    pixel[2] = ry_level;
    pixel[3] = 0xFF;
}

void blipvert::SetPixel_RGB24(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 3;

    uint8_t* pixel = buf + (y * stride) + (x * 3);
    pixel[0] = bv_level;
    pixel[1] = gu_level;
    pixel[2] = ry_level;
}

void blipvert::SetPixel_RGB565(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint8_t* pixel = buf + (y * stride) + (x * 2);
    PackRGB565Word(*reinterpret_cast<uint16_t*>(pixel), ry_level, gu_level, bv_level);
}

void blipvert::SetPixel_RGB555(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint8_t* pixel = buf + (y * stride) + (x * 2);
    PackRGB555Word(*reinterpret_cast<uint16_t*>(pixel), ry_level, gu_level, bv_level);

}

void blipvert::SetPixel_ARGB1555(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint8_t* pixel = buf + (y * stride) + (x * 2);
    PackARGB555Word(*reinterpret_cast<uint16_t*>(pixel), (alpha > 127 ? RGB555_ALPHA_MASK : 0x0000), ry_level, gu_level, bv_level);
}

void SetPixel_PackedY422(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
    int32_t width, int32_t height, int32_t x, int32_t y, uint8_t* buf, int32_t stride,
    int16_t Y0, int16_t Y1, int16_t U, int16_t V)
{
    if (!stride)
        stride = width * 2;

    uint8_t* mpixel = buf + (y * stride) + (x / 2 * 4);
    mpixel[U] = gu_level;
    mpixel[V] = bv_level;
    if (x & 1)
    {
        mpixel[Y1] = ry_level;
    }
    else
    {
        mpixel[Y0] = ry_level;
    }
}

void blipvert::SetPixel_YUY2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_PackedY422(ry_level, gu_level, bv_level, alpha, width, height, x, y, buf, stride, 0, 2, 1, 3);
}

void blipvert::SetPixel_UYVY(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_PackedY422(ry_level, gu_level, bv_level, alpha, width, height, x, y, buf, stride, 1, 3, 0, 2);
}

void blipvert::SetPixel_YVYU(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_PackedY422(ry_level, gu_level, bv_level, alpha, width, height, x, y, buf, stride, 0, 2, 3, 1);
}

void blipvert::SetPixel_VYUY(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_PackedY422(ry_level, gu_level, bv_level, alpha, width, height, x, y, buf, stride, 1, 3, 2, 0);
}

void SetPixel_PlanarYUV(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
    int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride,
    bool ufirst, int32_t decimation)
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

    uint8_t* vplane;
    uint8_t* uplane;
    if (ufirst)
    {
        uplane = buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    *(buf + (y * y_stride) + x) = ry_level;
    *(uplane + (y / decimation * uv_stride) + x / decimation) = gu_level;
    *(vplane + (y / decimation * uv_stride) + x / decimation) = bv_level;
}

void blipvert::SetPixel_I420(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_PlanarYUV(ry_level, gu_level, bv_level, alpha,
        x, y, width, height, buf, stride,
        true, 2);
}

void blipvert::SetPixel_YV12(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_PlanarYUV(ry_level, gu_level, bv_level, alpha,
        x, y, width, height, buf, stride,
        false, 2);
}

void blipvert::SetPixel_YVU9(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_PlanarYUV(ry_level, gu_level, bv_level, alpha,
        x, y, width, height, buf, stride,
        false, 4);
}

void blipvert::SetPixel_YUV9(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_PlanarYUV(ry_level, gu_level, bv_level, alpha,
        x, y, width, height, buf, stride,
        true, 4);
}

void blipvert::SetPixel_IYU1(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 12 / 8;

    uint8_t* mpixel = buf + (y * stride) + (x / 4 * 6);
    mpixel[0] = gu_level;
    mpixel[3] = bv_level;
    switch (x % 4) {
    case 0:
        mpixel[1] = ry_level;
        break;
    case 1:
        mpixel[2] = ry_level;
        break;
    case 2:
        mpixel[4] = ry_level;
        break;
    case 3:
        mpixel[5] = ry_level;
        break;
    };
}

void blipvert::SetPixel_IYU2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 3;

    uint8_t* pixel = buf + (y * stride) + (x * 3);
    pixel[0] = gu_level;
    pixel[1] = ry_level;
    pixel[2] = bv_level;
}

void blipvert::SetPixel_Y800(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width;

    *(buf + (y * stride) + x) = ry_level;
}

void blipvert::SetPixel_Y16(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint8_t* pixel = buf + (y * stride) + (x * 2);
    buf[0] = 0;
    buf[1] = ry_level;
}

void blipvert::SetPixel_Y41P(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width / 8 * 12;

    uint8_t* mpixel = buf + (y * stride) + (x / 8 * 12);

    switch (x % 8) {
    case 0:
        mpixel[1] = ry_level;
        mpixel[0] = gu_level;
        mpixel[2] = bv_level;
        break;
    case 1:
        mpixel[3] = ry_level;
        mpixel[0] = gu_level;
        mpixel[2] = bv_level;
        break;
    case 2:
        mpixel[5] = ry_level;
        mpixel[0] = gu_level;
        mpixel[2] = bv_level;
        break;
    case 3:
        mpixel[7] = ry_level;
        mpixel[0] = gu_level;
        mpixel[2] = bv_level;
        break;
    case 4:
        mpixel[8] = ry_level;
        mpixel[4] = gu_level;
        mpixel[6] = bv_level;
        break;
    case 5:
        mpixel[9] = ry_level;
        mpixel[4] = gu_level;
        mpixel[6] = bv_level;
        break;
    case 6:
        mpixel[10] = ry_level;
        mpixel[4] = gu_level;
        mpixel[6] = bv_level;
        break;
    case 7:
        mpixel[11] = ry_level;
        mpixel[4] = gu_level;
        mpixel[6] = bv_level;
        break;
    };
}

void blipvert::SetPixel_CLJR(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width;

    uint8_t* mpixel = buf + (y * stride) + (x / 4 * 4);
    uint8_t pixel = x % 4;
    uint32_t andMask;
    uint32_t orMask;
    switch (pixel) {
    case 0:
        PackCLJRDword(andMask, 0x00, 0x00, 0x00, 0xFF, 0XFF, 0xFF);
        PackCLJRDword(orMask, gu_level, bv_level, ry_level, 0x00, 0X00, 0x00);
        *reinterpret_cast<uint32_t*>(mpixel) = (*reinterpret_cast<uint32_t*>(mpixel) & andMask) | orMask;
        break;
    case 1:
        PackCLJRDword(andMask, 0x00, 0x00, 0xFF, 0x00, 0XFF, 0xFF);
        PackCLJRDword(orMask, gu_level, bv_level, 0x00, ry_level, 0X00, 0x00);
        *reinterpret_cast<uint32_t*>(mpixel) = (*reinterpret_cast<uint32_t*>(mpixel) & andMask) | orMask;
        break;
    case 2:
        PackCLJRDword(andMask, 0x00, 0x00, 0xFF, 0XFF, 0x00, 0xFF);
        PackCLJRDword(orMask, gu_level, bv_level, 0x00, 0X00, ry_level, 0x00);
        *reinterpret_cast<uint32_t*>(mpixel) = (*reinterpret_cast<uint32_t*>(mpixel) & andMask) | orMask;
        break;
    case 3:
        PackCLJRDword(andMask, 0x00, 0x00, 0xFF, 0XFF, 0xFF, 0x00);
        PackCLJRDword(orMask, gu_level, bv_level, 0x00, 0X00, 0x00, ry_level);
        *reinterpret_cast<uint32_t*>(mpixel) = (*reinterpret_cast<uint32_t*>(mpixel) & andMask) | orMask;
        break;
    };
}

void blipvert::SetPixel_AYUV(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
        stride = width * 4;

    uint8_t* pixel = buf + (y * stride) + (x * 4);
    pixel[0] = bv_level;
    pixel[1] = gu_level;
    pixel[2] = ry_level;
    pixel[3] = alpha;

}

void SetPixel_IMCx(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
    int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride,
    bool ufirst, bool interlaced)
{
    if (!stride)
        stride = width;

    int32_t uv_width = width / 2;
    int32_t uv_height = height / 2;

    uint8_t* vplane;
    uint8_t* uplane;
    if (ufirst)
    {
        if (interlaced)
        {
            uplane = buf + (stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            uplane = buf + (((height + 15) & ~15) * stride);
            vplane = buf + (((((height * 3) / 2) + 15) & ~15) * stride);
        }
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
            vplane = buf + (((height + 15) & ~15) * stride);
            uplane = buf + (((((height * 3) / 2) + 15) & ~15) * stride);
        }
    }

    *(buf + (y * stride) + x) = ry_level;
    *(uplane + (y / 2 * stride) + x / 2) = gu_level;
    *(vplane + (y / 2 * stride) + x / 2) = bv_level;
}

void blipvert::SetPixel_IMC1(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_IMCx(ry_level, gu_level, bv_level, alpha, x, y, width, height, buf, stride, false, false);
}

void blipvert::SetPixel_IMC2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_IMCx(ry_level, gu_level, bv_level, alpha, x, y, width, height, buf, stride, false, true);
}

void blipvert::SetPixel_IMC3(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_IMCx(ry_level, gu_level, bv_level, alpha, x, y, width, height, buf, stride, true, false);
}

void blipvert::SetPixel_IMC4(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_IMCx(ry_level, gu_level, bv_level, alpha, x, y, width, height, buf, stride, true, true);
}

void SetPixel_NVx(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
    int32_t x, int32_t y, int32_t width, int32_t height,
    uint8_t* buf, int32_t stride, bool ufirst)
{
    if (!stride)
        stride = width;

    int32_t uv_width = width;
    int32_t uv_height = height / 2;

    uint8_t* uvplane = buf + (stride * height);

    *(buf + (y * stride) + x) = ry_level;
    uint8_t* uvp = uvplane + (y / 2 * stride) + x / 2 * 2;
    if (ufirst)
    {
        uvp[0] = gu_level;
        uvp[1] = bv_level;
    }
    else
    {
        uvp[0] = bv_level;
        uvp[1] = gu_level;
    }
}

void blipvert::SetPixel_NV12(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_NVx(ry_level, gu_level, bv_level, alpha, x, y, width, height, buf, stride, true);
}

void blipvert::SetPixel_NV21(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_NVx(ry_level, gu_level, bv_level, alpha, x, y, width, height, buf, stride, false);
}

void blipvert::SetPixel_Y42T(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_UYVY(alpha > 127 ? ry_level | 0x01 : ry_level & 0xFE, gu_level, bv_level, alpha, x, y, width, height, buf, stride);
}

void blipvert::SetPixel_Y41T(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    SetPixel_Y41P(alpha > 127 ? ry_level | 0x01 : ry_level & 0xFE, gu_level, bv_level, alpha, x, y, width, height, buf, stride);
}

void blipvert::SetPixel_YV16(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    int32_t uv_width = width / 2;
    int32_t uv_height = height;

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

    uint8_t* vplane = buf + (y_stride * height);
    uint8_t* uplane = vplane + (uv_stride * uv_height);
 
    *(buf + (y * y_stride) + x) = ry_level;
    *(uplane + (y * uv_stride) + x / 2) = gu_level;
    *(vplane + (y * uv_stride) + x / 2) = bv_level;
}
