
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
#include "RGBtoRGB.h"
#include "CommonMacros.h"
#include "blipvert.h"
#include "LookupTables.h"
#include <cstring>

using namespace blipvert;


//
// RGB32 to RGBX conversions
//

void blipvert::RGB32_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    if (UseFasterLooping)
    {
        do
        {
            uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
            uint8_t* pdst = out_buf;
            int32_t hcount = width / 4;
            do
            {
                *reinterpret_cast<uint32_t*>(pdst) = *psrc++;
                pdst += 3;

                *reinterpret_cast<uint32_t*>(pdst) = *psrc++;
                pdst += 3;

                *reinterpret_cast<uint32_t*>(pdst) = *psrc++;
                pdst += 3;

                *reinterpret_cast<uint32_t*>(pdst) = *psrc++;
                pdst += 3;

            } while (--hcount);

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
    else
    {
        do
        {
            uint8_t* psrc = in_buf;
            uint8_t* pdst = out_buf;
            int32_t hcount = width;
            do
            {
                *pdst++ = *psrc++;
                *pdst++ = *psrc++;
                *pdst++ = *psrc++;
                psrc++;
            } while (--hcount);

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
}

void blipvert::RGB32_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB565Word(*pdst++, psrc[2], psrc[1], psrc[0]);
            psrc += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB32_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB555Word(*pdst++, psrc[2], psrc[1], psrc[0]);
            psrc += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGBA_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackARGB555Word(*pdst++, psrc[3], psrc[2], psrc[1], psrc[0]);
            psrc += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGBA_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            *pdst++ = *psrc++ | 0xFF000000;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB32_to_RGBA(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    if (out_stride == in_stride && !flipped)
    {
        memcpy(out_buf, in_buf, out_stride * height);
        return;
    }

    do
    {
        memcpy(out_buf, in_buf, width * 4);
        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB24 to RGBX conversions
//

void blipvert::RGB24_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    if (UseFasterLooping)
    {
        do
        {
            uint8_t* psrc = in_buf;
            uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
            int32_t hcount = width;
            do
            {
                *pdst++ = *reinterpret_cast<uint32_t*>(psrc) | 0xFF000000;
                psrc += 3;
            } while (--hcount);

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
    else
    {
        do
        {
            uint8_t* psrc = in_buf;
            uint8_t* pdst = out_buf;
            int32_t hcount = width;
            do
            {
                *pdst++ = *psrc++;
                *pdst++ = *psrc++;
                *pdst++ = *psrc++;
                *pdst++ = 0xFF;
            } while (--hcount);

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
}

void blipvert::RGB24_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB565Word(*pdst++, psrc[2], psrc[1], psrc[0]);
            psrc += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB24_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB555Word(*pdst++, psrc[2], psrc[1], psrc[0]);
            psrc += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB555 to RGBX conversions
//

void blipvert::RGB555_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            uint16_t source = *psrc;
            UnpackRGB555Word(source, pdst[2], pdst[1], pdst[0])
            pdst[3] = 0xFF;
            psrc++;
            pdst += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB555_to_RGBA(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            uint16_t source = *psrc;
            UnpackARGB555Word(source, pdst[3], pdst[2], pdst[1], pdst[0])
            psrc++;
            pdst += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB555_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            uint16_t source = *psrc;
            UnpackRGB555Word(source, pdst[2], pdst[1], pdst[0])

            psrc++;
            pdst += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB555_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            *pdst++ = ((*psrc & (RGB555_RED_MASK | RGB555_GREEN_MASK)) << 1) | (*psrc & RGB555_BLUE_MASK);
            psrc++;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB565 to RGBX conversions
//

void blipvert::RGB565_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            uint16_t source = *psrc;
            UnpackRGB565Word(source, pdst[2], pdst[1], pdst[0])
                pdst[3] = 0xFF;

            psrc++;
            pdst += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB565_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            uint16_t source = *psrc;
            UnpackRGB565Word(source, pdst[2], pdst[1], pdst[0])
            psrc++;
            pdst += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}


void blipvert::RGB565_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            *pdst++ = 0x8000 | ((*psrc & (RGB565_RED_MASK | RGB565_GREEN_MASK)) >> 1) | (*psrc & RGB565_BLUE_MASK);
            psrc++;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB8 to RGBX conversions
//

void blipvert::RGB8_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width;

    if (in_palette == nullptr)
        in_palette = rgb8_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            *pdst++ = (*reinterpret_cast<uint32_t*>(&in_palette[*psrc++]) | 0xFF000000);
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB8_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width;

    if (in_palette == nullptr)
        in_palette = rgb8_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    if (UseFasterLooping)
    {
        do
        {
            uint8_t* psrc = in_buf;
            uint8_t* pdst = out_buf;
            int32_t hcount = width;
            do
            {
                *reinterpret_cast<uint32_t*>(pdst) = *reinterpret_cast<uint32_t*>(&in_palette[*psrc++]);
                pdst += 3;
            } while (--hcount);

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
    else
    {
        do
        {
            uint8_t* psrc = in_buf;
            uint8_t* pdst = out_buf;
            int32_t hcount = width;
            do
            {
                *pdst++ = in_palette[*psrc].rgbBlue; // Blue
                *pdst++ = in_palette[*psrc].rgbGreen; // Green
                *pdst++ = in_palette[*psrc].rgbRed; // Red
                psrc++;
            } while (--hcount);

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
}

void blipvert::RGB8_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width;

    if (in_palette == nullptr)
        in_palette = rgb8_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB565Word(*pdst++, in_palette[*psrc].rgbRed,
                in_palette[*psrc].rgbGreen,
                in_palette[*psrc].rgbBlue);
            psrc++;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB8_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width;

    if (in_palette == nullptr)
        in_palette = rgb8_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB555Word(*pdst++, in_palette[*psrc].rgbRed,
                in_palette[*psrc].rgbGreen,
                in_palette[*psrc].rgbBlue);
            psrc++;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB4 to RGBX conversions
//

void blipvert::RGB4_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    bool has_odd = width % 2 != 0;
    if (!in_stride)
    {
        in_stride = width / 2;
        if (has_odd)
        {
            in_stride++;
        }
    }

    if (in_palette == nullptr)
        in_palette = rgb4_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width / 2;
        do
        {
            *pdst++ = (*reinterpret_cast<uint32_t*>(&in_palette[*psrc & 0x0F]) | 0xFF000000);
            *pdst++ = (*reinterpret_cast<uint32_t*>(&in_palette[(*psrc & 0xF0) >> 4]) | 0xFF000000);
            psrc++;
        } while (--hcount);

        if (has_odd)
        {
            *pdst = (*reinterpret_cast<uint32_t*>(&in_palette[*psrc & 0x0F]) | 0xFF000000);
        }

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB4_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    bool has_odd = width % 2 != 0;
    if (!in_stride)
    {
        in_stride = width / 2;
        if (has_odd)
        {
            in_stride++;
        }
    }

    if (in_palette == nullptr)
        in_palette = rgb4_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    if (UseFasterLooping)
    {
        do
        {
            uint8_t* psrc = in_buf;
            uint8_t* pdst = out_buf;
            int32_t hcount = width / 2;
            do
            {
                *reinterpret_cast<uint32_t*>(pdst) = *reinterpret_cast<uint32_t*>(&in_palette[*psrc & 0x0F]);
                pdst += 3;
                psrc++;

                *reinterpret_cast<uint32_t*>(pdst) = *reinterpret_cast<uint32_t*>(&in_palette[(*psrc & 0xF0) >> 4]);
                pdst += 3;
                psrc++;
            } while (--hcount);

            if (has_odd)
            {
                *reinterpret_cast<uint32_t*>(pdst) = (*reinterpret_cast<uint32_t*>(&in_palette[*psrc & 0x0F]));
            }

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
    else
    {
        do
        {
            uint8_t* psrc = in_buf;
            uint8_t* pdst = out_buf;
            int32_t hcount = width / 2;
            uint8_t index = 0;
            do
            {
                index = *psrc & 0x0F;
                *pdst++ = in_palette[index].rgbBlue;    // Blue
                *pdst++ = in_palette[index].rgbGreen;   // Green
                *pdst++ = in_palette[index].rgbRed;     // Red
                psrc++;

                index = (*psrc & 0xF0) >> 4;
                *pdst++ = in_palette[index].rgbBlue;    // Blue
                *pdst++ = in_palette[index].rgbGreen;   // Green
                *pdst++ = in_palette[index].rgbRed;     // Red
                psrc++;
            } while (--hcount);

            if (has_odd)
            {
                index = *psrc & 0x0F;
                *pdst++ = in_palette[index].rgbBlue;    // Blue
                *pdst++ = in_palette[index].rgbGreen;   // Green
                *pdst = in_palette[index].rgbRed;     // Red
            }

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
}

void blipvert::RGB4_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    bool has_odd = width % 2 != 0;
    if (!in_stride)
    {
        in_stride = width / 2;
        if (has_odd)
        {
            in_stride++;
        }
    }

    if (in_palette == nullptr)
        in_palette = rgb4_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width / 2;
        do
        {
            xRGBQUAD color = in_palette[*psrc & 0x0F];
            PackRGB565Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
            color = in_palette[(*psrc & 0xF0) >> 4];
            PackRGB565Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
            psrc++;
        } while (--hcount);

        if (has_odd)
        {
            xRGBQUAD color = in_palette[*psrc & 0x0F];
            PackRGB565Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
        }

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB4_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    bool has_odd = width % 2 != 0;
    if (!in_stride)
    {
        in_stride = width / 2;
        if (has_odd)
        {
            in_stride++;
        }
    }

    if (in_palette == nullptr)
        in_palette = rgb4_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width / 2;
        do
        {
            xRGBQUAD color = in_palette[*psrc & 0x0F];
            PackRGB555Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
            color = in_palette[(*psrc & 0xF0) >> 4];
            PackRGB555Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
            psrc++;
        } while (--hcount);

        if (has_odd)
        {
            xRGBQUAD color = in_palette[*psrc & 0x0F];
            PackRGB555Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
        }

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB1 to RGBX conversions
//

void blipvert::RGB1_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    uint16_t remainder = width % 8;
    if (!in_stride)
    {
        in_stride = width / 8;
        if (remainder)
        {
            in_stride++;
        }
    }

    if (in_palette == nullptr)
        in_palette = rgb1_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width / 8;
        do
        {
            uint8_t mask = 1;
            do
            {
                *pdst++ = (*reinterpret_cast<uint32_t*>(&in_palette[*psrc & mask ? 1 : 0]) | 0xFF000000);
            } while (mask <<= 1);
            psrc++;
        } while (--hcount);

        if (remainder)
        {
            uint16_t bcount = remainder;
            uint8_t mask = 1;
            do
            {
                *pdst++ = (*reinterpret_cast<uint32_t*>(&in_palette[*psrc & mask ? 1 : 0]) | 0xFF000000);
                mask <<= 1;
            } while (--bcount);
        }

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB1_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    uint16_t remainder = width % 8;
    if (!in_stride)
    {
        in_stride = width / 8;
        if (remainder)
        {
            in_stride++;
        }
    }

    if (in_palette == nullptr)
        in_palette = rgb1_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    if (UseFasterLooping)
    {
        do
        {
            uint8_t* psrc = in_buf;
            uint8_t* pdst = out_buf;
            int32_t hcount = width / 8;
            do
            {
                uint8_t mask = 1;
                do
                {
                    *reinterpret_cast<uint32_t*>(pdst++) = (*reinterpret_cast<uint32_t*>(&in_palette[*psrc & mask ? 1 : 0]));
                } while (mask <<= 1);
                psrc++;
            } while (--hcount);

            if (remainder)
            {
                uint16_t bcount = remainder;
                uint8_t mask = 1;
                do
                {
                    *reinterpret_cast<uint32_t*>(pdst) = (*reinterpret_cast<uint32_t*>(&in_palette[*psrc & mask ? 1 : 0]));
                    mask <<= 1;
                } while (--bcount);
            }

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
    else
    {
        do
        {
            uint8_t* psrc = in_buf;
            uint8_t* pdst = out_buf;
            int32_t hcount = width / 8;
            do
            {
                uint8_t mask = 1;
                do
                {
                    uint8_t index = *psrc & mask ? 1 : 0;
                    *pdst++ = in_palette[index].rgbBlue;    // Blue
                    *pdst++ = in_palette[index].rgbGreen;   // Green
                    *pdst++ = in_palette[index].rgbRed;     // Red
                } while (mask <<= 1);
                psrc++;
            } while (--hcount);

            if (remainder)
            {
                uint16_t bcount = remainder;
                uint8_t mask = 1;
                do
                {
                    uint8_t index = *psrc & mask ? 1 : 0;
                    *pdst++ = in_palette[index].rgbBlue;    // Blue
                    *pdst++ = in_palette[index].rgbGreen;   // Green
                    *pdst++ = in_palette[index].rgbRed;     // Red
                    mask <<= 1;
                } while (--bcount);
            }

            in_buf += in_stride;
            out_buf += out_stride;
        } while (--height);
    }
}

void blipvert::RGB1_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    uint16_t remainder = width % 8;
    if (!in_stride)
    {
        in_stride = width / 8;
        if (remainder)
        {
            in_stride++;
        }
    }

    if (in_palette == nullptr)
        in_palette = rgb1_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width / 8;
        do
        {
            uint8_t mask = 1;
            do
            {
                xRGBQUAD color = in_palette[*psrc & mask ? 1 : 0];
                PackRGB565Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
            } while (mask <<= 1);
            psrc++;
        } while (--hcount);

        if (remainder)
        {
            uint16_t bcount = remainder;
            uint8_t mask = 1;
            do
            {
                xRGBQUAD color = in_palette[*psrc & mask ? 1 : 0];
                PackRGB565Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
                mask <<= 1;
            } while (--bcount);
        }

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB1_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    uint16_t remainder = width % 8;
    if (!in_stride)
    {
        in_stride = width / 8;
        if (remainder)
        {
            in_stride++;
        }
    }

    if (in_palette == nullptr)
        in_palette = rgb1_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width / 8;
        do
        {
            uint8_t mask = 1;
            do
            {
                xRGBQUAD color = in_palette[*psrc & mask ? 1 : 0];
                PackRGB555Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
            } while (mask <<= 1);
            psrc++;
        } while (--hcount);

        if (remainder)
        {
            uint16_t bcount = remainder;
            uint8_t mask = 1;
            do
            {
                xRGBQUAD color = in_palette[*psrc & mask ? 1 : 0];
                PackRGB555Word(*pdst++, color.rgbRed, color.rgbGreen, color.rgbBlue);
                mask <<= 1;
            } while (--bcount);
        }

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}
