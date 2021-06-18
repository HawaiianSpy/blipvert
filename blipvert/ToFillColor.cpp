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
#include "ToFillColor.h"
#include "CommonMacros.h"
#include <cstring>

using namespace blipvert;

//
// RGB color fill functions
//

void blipvert::Fill_RGB32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 4;

    uint32_t fill = static_cast<uint32_t>(((((alpha) & 0xFF) << 24) | (((red) & 0xFF) << 16) | (((green) & 0xFF) << 8) | ((blue) & 0xFF)));
    do
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(pBuffer);
        int32_t hcount = width;
        do
        {
            *pdst++ = fill;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);
}

void blipvert::Fill_RGB24(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 3;

    uint32_t fill = static_cast<uint32_t>((0xFF000000 | (((red) & 0xFF) << 16) | (((green) & 0xFF) << 8) | ((blue) & 0xFF)));
    do
    {
        uint8_t* pdst = pBuffer;
        int32_t hcount = width / 4;
        do
        {
            *reinterpret_cast<uint32_t*>(pdst) = fill;
            pdst += 3;

            *reinterpret_cast<uint32_t*>(pdst) = fill;
            pdst += 3;

            *reinterpret_cast<uint32_t*>(pdst) = fill;
            pdst += 3;

            *reinterpret_cast<uint32_t*>(pdst) = fill;
            pdst += 3;

        } while (--hcount);

        pBuffer += stride;
    } while (--height);
}

void blipvert::Fill_RGB565(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint16_t fill;
    PackRGB565Word(fill, red, green, blue);

    do
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(pBuffer);
        int32_t hcount = width;
        do
        {
            *pdst = fill;
            pdst++;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);
}

void blipvert::Fill_RGB555(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint16_t fill;
    PackRGB555Word(fill, red, green, blue);
    if (alpha)
        fill |= 0x8000;
    else
        fill &= 0x7FFF;

    do
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(pBuffer);
        int32_t hcount = width;
        do
        {
            *pdst = fill;
            pdst++;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);
}

//
// Local generic YUV fill functions
//

void Fill_PackedY422(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t y0_offset, int32_t y1_offset, int32_t u_offset, int32_t v_offset)
{
    if (!out_stride)
        out_stride = width * 2;

    uint32_t fill = static_cast<uint32_t>((y_level << (y0_offset * 8)) | (y_level << (y1_offset * 8)) | \
        (u_level << (u_offset * 8)) | (v_level << (v_offset * 8)));

    for (int32_t h = 0; h < height; h++)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t w = 0; w < width; w += 2)
        {
            *pdst++ = fill;
        }

        out_buf += out_stride;
    }
}

void Fill_PlanarYUV(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
    int32_t uv_width = width / decimation;
    int32_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!out_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = out_stride;
        uv_stride = out_stride;
    }

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = out_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = out_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (y_stride * (height - 1));
        uplane += (uv_stride * (uv_height - 1));
        vplane += (uv_stride * (uv_height - 1));
        y_stride = -y_stride;
        uv_stride = -uv_stride;
    }

    uint16_t red;
    uint16_t green;
    uint16_t blue;

    if (decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
        {
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 2)
            {
                yp[0] = y_level;
                yp[1] = y_level;
                yp[y_stride] = y_level;
                yp[y_stride + 1] = y_level;
                uplane[x >> 1] = u_level;
                vplane[x >> 1] = v_level;
                yp += 2;
            }
            out_buf += (y_stride * 2);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 4)
            {
                red = 0;
                green = 0;
                blue = 0;

                int32_t is = 0;
                int32_t ys = 0;

                for (int32_t dec = 0; dec < 4; dec++)
                {
                    yp[ys] = y_level;
                    yp[ys + 1] = y_level;
                    yp[ys + 2] = y_level;
                    yp[ys + 3] = y_level;
                    ys += y_stride;
                }

                uplane[x >> 2] = u_level;
                vplane[x >> 2] = v_level;

                yp += 4;
            }
            out_buf += (y_stride * 4);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}


//
// Public YUV fill functions
//

void blipvert::Fill_YUY2(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        pBuffer, stride,
        0, 2, 1, 3);
}

void blipvert::Fill_UYVY(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        pBuffer, stride,
        1, 3, 0, 2);
}

void blipvert::Fill_YVYU(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        pBuffer, stride,
        0, 2, 3, 1);
}

void blipvert::Fill_VYUY(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        pBuffer, stride,
        1, 3, 2, 0);
}

void blipvert::Fill_IYUV(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, pBuffer, stride, true, 2, true);
}

void blipvert::Fill_YV12(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, pBuffer, stride, false, 2, false);
}

void blipvert::Fill_YVU9(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, pBuffer, stride, false, 4, false);
}

void blipvert::Fill_YUV9(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, pBuffer, stride, true, 4, true);
}

void blipvert::Fill_IYU1(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 12 / 8;

    for (int32_t h = 0; h < height; h++)
    {
        uint8_t* pdst = pBuffer;
        for (int32_t w = 0; w < width; w += 4)
        {
            pdst[0] = u_level;
            pdst[1] = y_level;
            pdst[2] = y_level;
            pdst[3] = v_level;
            pdst[4] = y_level;
            pdst[5] = y_level;
            pdst += 6;
        }

        pBuffer += stride;
    }
}

void blipvert::Fill_IYU2(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 3;

    for (int32_t h = 0; h < height; h++)
    {
        uint8_t* pdst = pBuffer;
        for (int32_t w = 0; w < width; w++)
        {
            pdst[0] = u_level;
            pdst[1] = y_level;
            pdst[2] = v_level;
            pdst += 3;
        }

        pBuffer += stride;
    }
}


void blipvert::Fill_Y800(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
    {
        memset(pBuffer, y_level, width * height);
        return;
    }

    for (int32_t h = 0; h < height; h++)
    {
        uint8_t* pdst = pBuffer;
        for (int32_t w = 0; w < width; w++)
        {
            *pdst++ = y_level;
        }

        pBuffer += stride;
    }
}

void blipvert::Fill_Y41P(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width / 8 * 12;

    while (height)
    {
        uint8_t* pdst = pBuffer;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = u_level;
            pdst[2] = v_level;

            pdst[1] = y_level;
            pdst[3] = y_level;
            pdst[5] = y_level;
            pdst[7] = y_level;

            pdst[4] = u_level;
            pdst[6] = v_level;

            pdst[8] = y_level;
            pdst[9] = y_level;
            pdst[10] = y_level;
            pdst[11] = y_level;

            pdst += 12;
            hcount -= 8;
        }

        pBuffer += stride;
        height--;
    }
}

void blipvert::Fill_CLJR(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width;

    uint32_t fill;
    PackCLJRDword(fill, u_level, v_level, y_level, y_level, y_level, y_level);
    while (height)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(pBuffer);
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = fill;
            hcount -= 4;
        }

        pBuffer += stride;
        height--;
    }
}
