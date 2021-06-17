//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021 Don Jordan
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this softwareand associated documentation files(the "Software"), to deal
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
#include "Utilities.h"
#include "LookupTables.h"
#include <cstring>

using namespace blipvert;

void blipvert::SlowYUVtoRGB(uint8_t Y, uint8_t U, uint8_t V, uint8_t* R, uint8_t* G, uint8_t* B)
{
    double RF = 1.164 * static_cast<double>(Y) + 1.596 * static_cast<double>(V) - 204.288;
    if (RF > 255.0)
        RF = 255.0;
    else if (RF < 0.0)
        RF = 0.0;

    *R = static_cast<uint8_t>(RF);

    double GF = 1.164 * static_cast<double>(Y) - 0.813 * static_cast<double>(V) - \
        0.391 * static_cast<double>(U) + 154.112;
    if (GF > 255.0)
        GF = 255.0;
    else if (GF < 0.0)
        GF = 0.0;

    *G = static_cast<uint8_t>(GF);

    double BF = 1.164 * static_cast<double>(Y) + 2.018 * static_cast<double>(U) - 276.928;
    if (BF > 255.0)
        BF = 255.0;
    else if (BF < 0.0)
        BF = 0.0;

    *B = static_cast<uint8_t>(BF);
}

void blipvert::SlowRGBtoYUV(uint8_t R, uint8_t G, uint8_t B, uint8_t* Y, uint8_t* U, uint8_t* V)
{
    *Y = static_cast<uint8_t>((0.257 * static_cast<double>(R)) + (0.504 * static_cast<double>(G)) + \
        (0.098 * static_cast<double>(B)) + 16);
    *U = static_cast<uint8_t>(-(0.148 * static_cast<double>(R)) - (0.291 * static_cast<double>(G)) + \
        (0.439 * static_cast<double>(B)) + 128);
    *V = static_cast<uint8_t>((0.439 * static_cast<double>(R)) - (0.368 * static_cast<double>(G)) - \
        (0.071 * static_cast<double>(B)) + 128);
}

void blipvert::FastYUVtoRGB(uint8_t Y, uint8_t U, uint8_t V, uint8_t* R, uint8_t* G, uint8_t* B)
{
    int32_t Yprime = luminance_table[Y];
    *B = saturation_table[Yprime + u_table[U]];
    *G = saturation_table[Yprime + uv_table[U][V]];
    *R = saturation_table[Yprime + v_table[V]];
}

void blipvert::FastRGBtoYUV(uint8_t R, uint8_t G, uint8_t B, uint8_t* Y, uint8_t* U, uint8_t* V)
{
    *Y = static_cast<uint8_t>(((yr_table[R] + yg_table[G] + yb_table[B]) >> 15) + 16);
    *U = static_cast<uint8_t>(((ur_table[R] + ug_table[G] + ub_table[B]) >> 15) + 128);
    *V = static_cast<uint8_t>(((vr_table[R] + vg_table[G] + vb_table[B]) >> 15) + 128);
}

void blipvert::Progressive_to_Interlaced(int32_t height, int32_t line_bytes, bool flipped,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride)
{
    if (flipped)
    {
        in_buf += (in_stride * (height - 1));
        in_stride = -in_stride;
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    uint8_t* psrc = in_buf;
    uint8_t* pevendst = out_buf;
    uint8_t* podddst = out_buf + (height / 2 * out_stride);
    for (int32_t y = 0; y < height; y++)
    {
        if (y & 1)
        {
            memcpy(podddst, psrc, line_bytes);
            podddst += out_stride;
        }
        else
        {
            memcpy(pevendst, psrc, line_bytes);
            pevendst += out_stride;
        }
        psrc += in_stride;
    }
}

void blipvert::Interlaced_to_Progressive(int32_t height, int32_t line_bytes, bool flipped,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride)
{
    if (flipped)
    {
        in_buf += (in_stride * (height - 1));
        in_stride = -in_stride;
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    uint8_t* pevensrc = in_buf;
    uint8_t* poddsrc = in_buf + (height / 2 * in_stride);
    uint8_t* pdst = out_buf;
    for (int32_t y = 0; y < height; y++)
    {
        if (y & 1)
        {
            memcpy(pdst, poddsrc, line_bytes);
            poddsrc += in_stride;
        }
        else
        {
            memcpy(pdst, pevensrc, line_bytes);
            pevensrc += in_stride;
        }
        pdst += out_stride;
    }
}


uint32_t blipvert::CalculateBufferSize(const MediaFormatID& inFormat, uint32_t width, uint32_t height, uint32_t in_stride)
{
    Fourcc fourcc;
    Fourcc xRefFourcc;
    int16_t effectiveBitsPerPixel;

    if (width == 0 || height == 0)
    {
        return 0;
    }

    if (!GetVideoFormatInfo(inFormat, fourcc, xRefFourcc, effectiveBitsPerPixel))
    {
        return 0;
    }

    if (effectiveBitsPerPixel <= 0)
    {
        return 0;
    }

    uint32_t bitsPerLine = width * static_cast<uint32_t>(effectiveBitsPerPixel);
    uint32_t bytesPerLine = ((bitsPerLine + 31) & (~31)) / 8;

    if (in_stride > bytesPerLine)
    {
        bytesPerLine = in_stride;
    }

    return bytesPerLine * height;
}

bool blipvert::IsRGBEncoding(const MediaFormatID& encoding)
{
    if (encoding == MVFMT_RGB1 ||
        encoding == MVFMT_RGB4 ||
        encoding == MVFMT_RGB8 ||
        encoding == MVFMT_RGB555 ||
        encoding == MVFMT_RGB565 ||
        encoding == MVFMT_RGB24 ||
        encoding == MVFMT_RGB32 ||
        encoding == MVFMT_ARGB32 ||
        encoding == MVFMT_RGBA ||
        encoding == MVFMT_RGBT ||
        encoding == MVFMT_RGB_BITFIELDS)
    {
        return true;
    }

    return false;
}

bool blipvert::IsPalletizedEncoding(const MediaFormatID& encoding)
{
    if (encoding == MVFMT_RGB1 ||
        encoding == MVFMT_RGB4 ||
        encoding == MVFMT_RGB8)
    {
        return true;
    }

    return false;
}

bool blipvert::IsRGBFourcc(Fourcc fourcc)
{
    if (fourcc == FOURCC_BI_RGB ||
        fourcc == FOURCC_RGB ||
        fourcc == FOURCC_BI_BITFIELDS)
    {
        return true;
    }

    return false;
}
