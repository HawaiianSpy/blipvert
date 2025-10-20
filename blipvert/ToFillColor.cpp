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
#include "ToFillColor.h"
#include "blipvert.h"
#include "CommonMacros.h"
#include "LookupTables.h"
#include <cstring>
#include <algorithm>

using namespace blipvert;

//
// RGB color fill functions
//

void blipvert::Fill_RGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    uint32_t fill = static_cast<uint32_t>(((((alpha) & 0xFF) << 24) | (((red) & 0xFF) << 16) | (((green) & 0xFF) << 8) | ((blue) & 0xFF)));

    if (!stride)
    {
        uint32_t* buf32 = reinterpret_cast<uint32_t*>(buf);
        uint32_t count = width * height;
        std::fill(buf32, buf32 + count, fill);
    }
    else
    {
        do
        {
            uint32_t* buf32 = reinterpret_cast<uint32_t*>(buf);
            std::fill(buf32, buf32 + width, fill);
            buf += stride;
        } while (--height);
    }
}

void blipvert::Fill_RGB32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_RGBA(red, green, blue, 0xFF, width, height, buf, stride);
}

void blipvert::Fill_RGB24(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    typedef struct RGB { uint8_t b, g, r; } RGB;
    RGB pattern = { blue, green, red };

    if (!stride)
    {
        RGB* rgbdst = reinterpret_cast<RGB*>(buf);
        std::fill_n(rgbdst, width * height, pattern);
    }
    else
    {
        do
        {
            RGB* rgbdst = reinterpret_cast<RGB*>(buf);
            std::fill_n(rgbdst, width, pattern);
            buf += stride;
        } while (--height);
    }
}

void blipvert::Fill_RGB565(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    uint16_t fill;
    PackRGB565Word(fill, red, green, blue);

    if (!stride)
    {
        uint16_t* buf16 = reinterpret_cast<uint16_t*>(buf);
        uint32_t count = width * height;
        std::fill(buf16, buf16 + count, fill);
    }
    else
    {
        do
        {
            uint16_t* buf16 = reinterpret_cast<uint16_t*>(buf);
            std::fill(buf16, buf16 + width, fill);
            buf += stride;
        } while (--height);
    }
}

void blipvert::Fill_ARGB1555(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    uint16_t fill;
    PackARGB555Word(fill, (alpha > 127 ? RGB555_ALPHA_MASK : 0x0000), red, green, blue);

    if (!stride)
    {
        uint16_t* buf16 = reinterpret_cast<uint16_t*>(buf);
        uint32_t count = width * height;
        std::fill(buf16, buf16 + count, fill);
    }
    else
    {
        do
        {
            uint16_t* buf16 = reinterpret_cast<uint16_t*>(buf);
            std::fill(buf16, buf16 + width, fill);
            buf += stride;
        } while (--height);
    }
}

void blipvert::Fill_RGB555(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_ARGB1555(red, green, blue, 255, width, height, buf, stride);
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

#if 0
    for (int32_t h = 0; h < height; h++)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t w = 0; w < width; w += 2)
        {
            *pdst++ = fill;
        }

        out_buf += out_stride;
    }
#else
    for (int32_t h = 0; h < height; h++)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        std::fill(pdst, pdst + (width / 2), fill);
        out_buf += out_stride;
    }
#endif
}

void Fill_PlanarYUV(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    bool ufirst, int32_t decimation)
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
    if (ufirst)
    {
        uplane = out_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = out_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (y_stride == width)
    {
        memset(out_buf, y_level, y_stride * height);
        memset(uplane, u_level, uv_stride * uv_height);
        memset(vplane, v_level, uv_stride * uv_height);
    }
    else
    {
#if 1
        for (int32_t y = 0; y < height; y++)
        {
            memset(out_buf, y_level, width);
            out_buf += y_stride;
        }
        for (int32_t y = 0; y < uv_height; y++)
        {
            memset(uplane, u_level, uv_width);
            memset(vplane, v_level, uv_width);
            uplane += uv_stride;
            vplane += uv_stride;
        }
#else
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
#endif
    }
}

void Fill_IMCx(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    bool ufirst, bool interlaced)
{
    int32_t uv_width = width / 2;
    int32_t uv_height = height / 2;

    if (!out_stride)
        out_stride = width;


    uint8_t* vplane;
    uint8_t* uplane;
    if (ufirst)
    {
        if (interlaced)
        {
            uplane = out_buf + (out_stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            int32_t uoffset = Align16(height);
            uplane = out_buf + (uoffset * out_stride);
            vplane = out_buf + (Align16(uoffset + uv_height) * out_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            vplane = out_buf + (out_stride * height);
            uplane = vplane + uv_width;
        }
        else
        {
            int32_t voffset = Align16(height);
            vplane = out_buf + (voffset * out_stride);
            uplane = out_buf + (Align16(voffset + uv_height) * out_stride);
        }
    }

    if (out_stride == width)
    {
        memset(out_buf, y_level, width * height);
        for (int32_t y = 0; y < uv_height; y++)
        {
            memset(uplane, u_level, uv_width);
            memset(vplane, v_level, uv_width);
            uplane += out_stride;
            vplane += out_stride;
        }
    }
    else
    {
#if 1
        for (int32_t y = 0; y < height; y++)
        {
            memset(out_buf, y_level, width);
            out_buf += out_stride;
        }
        for (int32_t y = 0; y < uv_height; y++)
        {
            memset(uplane, u_level, uv_width);
            memset(vplane, v_level, uv_width);
            uplane += out_stride;
            vplane += out_stride;
        }
#else
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
#endif
    }
}


//
// Public YUV fill functions
//

void blipvert::Fill_YUY2(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        buf, stride,
        0, 2, 1, 3);
}

void blipvert::Fill_UYVY(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        buf, stride,
        1, 3, 0, 2);
}

void blipvert::Fill_YVYU(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        buf, stride,
        0, 2, 3, 1);
}

void blipvert::Fill_VYUY(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        buf, stride,
        1, 3, 2, 0);
}

void blipvert::Fill_I420(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, buf, stride, true, 2);
}

void blipvert::Fill_YV12(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, buf, stride, false, 2);
}

void blipvert::Fill_YVU9(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, buf, stride, false, 4);
}

void blipvert::Fill_YUV9(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, buf, stride, true, 4);
}

void blipvert::Fill_IMC1(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_IMCx(y_level, u_level, v_level, width, height, buf, stride, false, false);
}

void blipvert::Fill_IMC2(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_IMCx(y_level, u_level, v_level, width, height, buf, stride, false, true);
}

void blipvert::Fill_IMC3(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_IMCx(y_level, u_level, v_level, width, height, buf, stride, true, false);
}

void blipvert::Fill_IMC4(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_IMCx(y_level, u_level, v_level, width, height, buf, stride, true, true);
}

void blipvert::Fill_IYU1(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    typedef struct IYU1 { uint8_t u, y1, y2, v, y3, y4;  } IYU1;
    IYU1 pattern = { u_level, y_level, y_level, v_level, y_level, y_level };

    //if (!stride)
    //{
    //    stride = width * 12 / 8;
    //}


    if (!stride)
    {
        IYU1* iyu1dst = reinterpret_cast<IYU1*>(buf);
        std::fill_n(iyu1dst, (width / 4) * height, pattern);
    }
    else
    {
        do
        {
            IYU1* iyu1dst = reinterpret_cast<IYU1*>(buf);
            std::fill_n(iyu1dst, width / 4, pattern);
            buf += stride;
        } while (--height);
    }
}

void blipvert::Fill_IYU2(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    typedef struct IYU2 { uint8_t u, y, v; } IYU2;
    IYU2 pattern = { u_level, y_level, v_level };

    if (!stride)
    {
        IYU2* iyu2dst = reinterpret_cast<IYU2*>(buf);
        std::fill_n(iyu2dst, width * height, pattern);
    }
    else
    {
        do
        {
            IYU2* iyu2dst = reinterpret_cast<IYU2*>(buf);
            std::fill_n(iyu2dst, width, pattern);
            buf += stride;
        } while (--height);
    }
}


void blipvert::Fill_Y800(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    if (!stride)
    {
        memset(buf, y_level, width * height);
    }
    else
    {
        for (int32_t h = 0; h < height; h++)
        {
            uint8_t* pdst = buf;
            memset(pdst, y_level, width);
            buf += stride;
        }
    }
}

void blipvert::Fill_Y16(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    uint16_t fill = Scale8BitTo16Bit(y_level);
    if (IsBigEndian)
    {
        fill = Swap16BitEndian(fill);
    }

#if 1
    if (!stride)
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(buf);
        std::fill(pdst, pdst + (width * height), fill);
    }
    else
    {
        for (int32_t h = 0; h < height; h++)
        {
            uint16_t* pdst = reinterpret_cast<uint16_t*>(buf);
            std::fill(pdst, pdst + width, fill);
            buf += stride;
        }
    }
#else
    if (!stride)
    {
        stride = width * 2;
    }

    for (int32_t h = 0; h < height; h++)
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(buf);
        for (int32_t w = 0; w < width; w++)
        {
            *pdst++ = fill;
        }

        buf += stride;
    }
#endif
}

void blipvert::Fill_Y41P(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    typedef struct Y41P { uint32_t yuv1, yuv2, y; } Y41P;

    uint32_t yuvfill = (y_level << 24 | v_level << 16 | y_level << 8 | u_level);
    uint32_t yfill = (y_level << 24 | y_level << 16 | y_level << 8 | y_level);

    Y41P pattern = { yuvfill, yuvfill, yfill };

#if 1
    if (!stride)
    {
        Y41P* y41pdst = reinterpret_cast<Y41P*>(buf);
        std::fill_n(y41pdst, (width / 8) * height, pattern);
    }
    else
    {
        do
        {
            Y41P* y41pdst = reinterpret_cast<Y41P*>(buf);
            std::fill_n(y41pdst, width / 8, pattern);
            buf += stride;
        } while (--height);
    }

#else

    if (!stride)
    {
        stride = width / 8 * 12;
    }

    while (height)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(buf);
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = yuvfill;
            *pdst++ = yuvfill;
            *pdst++ = yfill;
            hcount -= 8;
        }

        buf += stride;
        height--;
    }
#endif
}

void blipvert::Fill_CLJR(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{

    uint32_t fill;
    PackCLJRDword(fill, u_level, v_level, y_level, y_level, y_level, y_level);

#if 1
    if (!stride)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(buf);
        std::fill(pdst, pdst + (width / 4 * height), fill);
    }
    else
    {
        for (int32_t h = 0; h < height; h++)
        {
            uint32_t* pdst = reinterpret_cast<uint32_t*>(buf);
            std::fill(pdst, pdst + width / 4, fill);
            buf += stride;
        }
    }
#else
    if (!stride)
    {
        stride = width;
    }

    while (height)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(buf);
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = fill;
            hcount -= 4;
        }

        buf += stride;
        height--;
    }
#endif
}

void blipvert::Fill_AYUV(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_RGBA(y_level, u_level, v_level, alpha, width, height, buf, stride);
}

void Fill_NVx(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride, bool ufirst)
{
    int32_t uv_width = width / 2;
    int32_t uv_height = height / 2;

    if (stride < width)
        stride = width;

    uint8_t* uvplane = buf + (stride * height);

    uint8_t U;
    uint8_t V;

    if (ufirst)
    {
        U = 0;
        V = 1;
    }
    else
    {
        U = 1;
        V = 0;
    }

    if (stride == width)
    {
        memset(buf, y_level, width * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memset(buf, y_level, width);
            buf += stride;
        }
    }

    for (int32_t y = 0; y < uv_height; y++)
    {
        uint8_t* up = uvplane + U;
        uint8_t* vp = uvplane + V;
        for (int32_t x = 0; x < uv_width; x++)
        {
            *up = u_level;
            *vp = v_level;
            up += 2;
            vp += 2;
        }
        uvplane += stride;
    }
}

void blipvert::Fill_NV12(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_NVx(y_level, u_level, v_level, alpha,
        width, height, buf, stride, true);
}

void blipvert::Fill_NV21(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_NVx(y_level, u_level, v_level, alpha,
        width, height, buf, stride, false);
}

void blipvert::Fill_Y42T(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_PackedY422(alpha > 127 ? y_level | 0x01 : y_level & 0xFE, u_level, v_level,
        width, height,
        buf, stride,
        1, 3, 0, 2);
}

void blipvert::Fill_Y41T(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
{
    Fill_Y41P(alpha > 127 ? y_level | 0x01 : y_level & 0xFE, u_level, v_level, alpha, width, height, buf, stride);
}

void blipvert::Fill_YV16(uint8_t y_level, uint8_t u_level, uint8_t v_level, uint8_t alpha,
    int32_t width, int32_t height, uint8_t* buf, int32_t stride)
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
        memset(buf, y_level, y_stride * height);
        memset(vplane, v_level, uv_stride * height);
        memset(uplane, u_level, uv_stride * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memset(buf, y_level, width);
            buf += y_stride;
        }

        for (int32_t y = 0; y < height; y++)
        {
            memset(uplane, u_level, uv_width);
            memset(vplane, v_level, uv_width);
            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}
