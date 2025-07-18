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
#include "ToGreyscale.h"
#include "LookupTables.h"
#include "CommonMacros.h"
#include "blipvert.h"
#include <cstring>

using namespace blipvert;

void blipvert::RGBA_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
    {
        uint32_t count = width * height;
        while (count)
        {
            *reinterpret_cast<uint32_t*>(buf) = (*reinterpret_cast<uint32_t*>(buf) & 0xFF000000) | rgba_greyscale[static_cast<uint8_t>(((yr_table[buf[2]] + yg_table[buf[1]] + yb_table[buf[0]]) >> 15) + 16)];
            buf += 4;
            count--;
        }
    }
    else
    {
        while (height)
        {
            uint8_t* pdst = buf;
            int32_t hcount = width;
            while (hcount)
            {
                *reinterpret_cast<uint32_t*>(pdst) = (*reinterpret_cast<uint32_t*>(pdst) & 0xFF000000) | rgba_greyscale[static_cast<uint8_t>(((yr_table[pdst[2]] + yg_table[pdst[1]] + yb_table[pdst[0]]) >> 15) + 16)];
                pdst += 4;
                hcount--;
            }

            buf += stride;
            height--;
        }
    }
}

void blipvert::RGB32_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
    {
        uint32_t count = width * height;
        while (count)
        {
            *reinterpret_cast<uint32_t*>(buf) = rgb32_greyscale[static_cast<uint8_t>(((yr_table[buf[2]] + yg_table[buf[1]] + yb_table[buf[0]]) >> 15) + 16)];
            buf += 4;
            count--;
        }
    }
    else
    {
        while (height)
        {
            uint8_t* pdst = buf;
            int32_t hcount = width;
            while (hcount)
            {
                *reinterpret_cast<uint32_t*>(pdst) = rgb32_greyscale[static_cast<uint8_t>(((yr_table[pdst[2]] + yg_table[pdst[1]] + yb_table[pdst[0]]) >> 15) + 16)];
                pdst += 4;
                hcount--;
            }

            buf += stride;
            height--;
        }
    }
}

void blipvert::RGB24_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
    {
        uint32_t count = width * height;
        while (count)
        {
            uint8_t Y = static_cast<uint8_t>(((yr_table[buf[2]] + yg_table[buf[1]] + yb_table[buf[0]]) >> 15) + 16);
            *buf++ = Y;
            *buf++ = Y;
            *buf++ = Y;
            count--;
        }
    }
    else
    {
        while (height)
        {
            uint8_t* pdst = buf;
            int32_t hcount = width;
            while (hcount)
            {
                uint8_t Y = static_cast<uint8_t>(((yr_table[pdst[2]] + yg_table[pdst[1]] + yb_table[pdst[0]]) >> 15) + 16);
                *pdst++ = Y;
                *pdst++ = Y;
                *pdst++ = Y;
                hcount--;
            }

            buf += stride;
            height--;
        }
    }
}

void blipvert::RGB565_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(buf);
        uint32_t count = width * height;
        while (count)
        {
            *pdst = rgb565_greyscale[((yr_table[UnpackRGB565Red(*pdst)] +  yg_table[UnpackRGB565Green(*pdst)] +  yb_table[UnpackRGB565Blue(*pdst)]) >> 15) + 16];
            pdst++;
            count--;
        }
    }
    else
    {
        while (height)
        {
            uint16_t* pdst = reinterpret_cast<uint16_t*>(buf);
            int32_t hcount = width;
            while (hcount)
            {
                *pdst = rgb565_greyscale[((yr_table[UnpackRGB565Red(*pdst)] +  yg_table[UnpackRGB565Green(*pdst)] +  yb_table[UnpackRGB565Blue(*pdst)]) >> 15) + 16];
                pdst++;
                hcount--;
            }

            buf += stride;
            height--;
        }
    }
}

void blipvert::RGB555_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(buf);
        uint32_t count = width * height;
        while (count)
        {
            *pdst = (*reinterpret_cast<uint16_t*>(buf) & RGB555_ALPHA_MASK) | rgba555_greyscale[((yr_table[UnpackRGB555Red(*pdst)] +  yg_table[UnpackRGB555Green(*pdst)] +  yb_table[UnpackRGB555Blue(*pdst)]) >> 15) + 16];
            pdst++;
            count--;
        }
    }
    else
    {
        while (height)
        {
            uint16_t* pdst = reinterpret_cast<uint16_t*>(buf);
            int32_t hcount = width;
            while (hcount)
            {
                *pdst = (*reinterpret_cast<uint16_t*>(buf) & RGB555_ALPHA_MASK) | rgba555_greyscale[((yr_table[UnpackRGB555Red(*pdst)] +  yg_table[UnpackRGB555Green(*pdst)] + yb_table[UnpackRGB555Blue(*pdst)]) >> 15) + 16];
                pdst++;
                hcount--;
            }

            buf += stride;
            height--;
        }
    }
}

void Palletized_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette, uint16_t num_colors)
{
    for (uint16_t index = 0; index < num_colors; index++)
    {
        uint8_t Y = static_cast<uint8_t>(((yr_table[in_palette[index].rgbRed] + \
            yg_table[in_palette[index].rgbGreen] + \
            yb_table[in_palette[index].rgbBlue]) >> 15) + 16);

        in_palette[index].rgbRed = Y;
        in_palette[index].rgbGreen = Y;
        in_palette[index].rgbBlue = Y;
    }
}

void blipvert::RGB8_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    Palletized_to_Greyscale(width, height, buf, stride, in_palette, 256);
}

void blipvert::RGB4_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    Palletized_to_Greyscale(width, height, buf, stride, in_palette, 16);
}

void blipvert::RGB1_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    Palletized_to_Greyscale(width, height, buf, stride, in_palette, 2);
}


void PackedY422_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride, int32_t out_u, int32_t out_v)
{
    if (!stride)
        stride = width * 2;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* pdst = buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[out_u] = 0;
            pdst[out_v] = 0;
            pdst += 4;
        }

        buf += stride;
    }
}


void blipvert::AYUV_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
    {
        uint32_t count = width * height;
        while (count)
        {
            *reinterpret_cast<uint32_t*>(buf) = (*reinterpret_cast<uint32_t*>(buf) & 0xFFFF0000);
            buf += 4;
            count--;
        }
    }
    else
    {
        while (height)
        {
            uint8_t* pdst = buf;
            int32_t hcount = width;
            while (hcount)
            {
                *reinterpret_cast<uint32_t*>(pdst) = (*reinterpret_cast<uint32_t*>(pdst) & 0xFFFF0000);
                pdst += 4;
                hcount--;
            }

            buf += stride;
            height--;
        }
    }
}

void blipvert::UYVY_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    PackedY422_to_Greyscale(width, height, buf, stride, 0, 2);
}

void blipvert::YVYU_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    PackedY422_to_Greyscale(width, height, buf, stride, 3, 1);
}

void blipvert::VYUY_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    PackedY422_to_Greyscale(width, height, buf, stride, 2, 0);
}

void blipvert::YUY2_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    PackedY422_to_Greyscale(width, height, buf, stride, 1, 3);
}

void PlanarYUV_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride, int32_t decimation)
{
    int32_t out_uv_width = width / decimation;
    int32_t out_uv_height = height / decimation;

    int32_t out_y_stride, out_uv_stride;
    if (stride < width)
    {
        out_y_stride = width;
        out_uv_stride = out_uv_width;
    }
    else
    {
        out_y_stride = stride;
        out_uv_stride = stride;
    }

    uint8_t* out_uplane = buf + (out_y_stride * height);
    uint8_t* out_vplane = out_uplane + (out_uv_stride * out_uv_height);
    if (out_y_stride == width)
    {
        memset(out_uplane, 0, out_uv_stride * out_uv_height);
        memset(out_vplane, 0, out_uv_stride * out_uv_height);
    }
    else
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            memset(out_uplane, 0, out_uv_width);
            memset(out_vplane, 0, out_uv_width);
            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}


void IMCx_to_Greyscale(int32_t width, int32_t height,
    uint8_t* buf, int32_t stride, bool interlaced)
{
    int32_t uv_width = width / 2;
    int32_t uv_height = height / 2;

    if (stride < width)
        stride = width;

    uint8_t* vplane;
    uint8_t* uplane;

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

    for (int32_t y = 0; y < uv_height; y++)
    {
        memset(uplane, 0, uv_width);
        memset(vplane, 0, uv_width);
        uplane += stride;
        vplane += stride;
    }
}

void blipvert::IMC1_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    IMCx_to_Greyscale(width, height, buf, stride, false);
}

void blipvert::IMC2_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    IMCx_to_Greyscale(width, height, buf, stride, true);
}

void blipvert::IMC3_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    IMCx_to_Greyscale(width, height, buf, stride, false);
}

void blipvert::IMC4_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    IMCx_to_Greyscale(width, height, buf, stride, true);
}

void blipvert::I420_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Greyscale(width, height, buf, stride, 2);
}

void blipvert::YV12_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Greyscale(width, height, buf, stride, 2);
}


void blipvert::YVU9_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Greyscale(width, height, buf, stride, 4);
}


void blipvert::YUV9_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Greyscale(width, height, buf, stride, 4);
}


void blipvert::IYU1_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
        stride = width * 12 / 8;

    while (height)
    {
        uint8_t* pdst = buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = 0;
            pdst[3] = 0;
            pdst += 6;
            hcount -= 4;
        }

        buf += stride;
        height--;
    }
}

void blipvert::IYU2_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
        stride = width * 3;

    while (height)
    {
        uint8_t* pdst = buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = 0;
            pdst[2] = 0;
            pdst += 3;
            hcount--;
        }
        buf += stride;
        height--;
    }
}


void blipvert::Y41P_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
        stride = width / 8 * 12;

    while (height)
    {
        uint8_t* pdst = buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = 0;
            pdst[2] = 0;
            pdst[4] = 0;
            pdst[6] = 0;
            pdst += 12;
            hcount -= 8;
        }

        buf += stride;
        height--;
    }
}

void blipvert::CLJR_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    if (!stride)
        stride = width;

    while (height)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(buf);
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = *pdst & 0xFFFFF000;
            hcount -= 4;
        }

        buf += stride;
        height--;
    }
}

void NVx_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride)
{
    int32_t uv_height = height / 2;

    if (stride < width)
        stride = width;

    uint8_t* uvplane = buf + (stride * height);

    if (stride == width)
    {
        memset(uvplane, 0, width * uv_height);
    }
    else
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            memset(uvplane, 0, width);
            uvplane += stride;
        }
    }
}

void blipvert::NV12_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    NVx_to_Greyscale(width, height, buf, stride);
}

void blipvert::NV21_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    NVx_to_Greyscale(width, height, buf, stride);
}

void blipvert::Y42T_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    PackedY422_to_Greyscale(width, height, buf, stride, 0, 2);
}

void blipvert::Y41T_to_Greyscale(int32_t  width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
{
    Y41P_to_Greyscale(width, height, buf, stride);
}

void blipvert::YV16_to_Greyscale(int32_t width, int32_t height, uint8_t* buf, int32_t stride, xRGBQUAD* in_palette)
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

    if (y_stride == width)
    {
        memset(vplane, 0, uv_stride * height);
        memset(uplane, 0, uv_stride * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memset(uplane, 0, uv_width);
            memset(vplane, 0, uv_width);
            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}
