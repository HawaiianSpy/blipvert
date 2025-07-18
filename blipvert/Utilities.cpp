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
#include "Utilities.h"
#include "LookupTables.h"
#include "CommonMacros.h"
#include <cstring>

using namespace blipvert;

void blipvert::SlowYUVtoRGB(double Y, double U, double V, double* R, double* G, double* B)
{
    double RF = 1.164 * Y + 1.596 * V - 204.288;
    if (RF > 255.0)
        RF = 255.0;
    else if (RF < 0.0)
        RF = 0.0;

    *R = RF;

    double GF = 1.164 * Y - 0.813 * V - 0.391 * U + 154.112;
    if (GF > 255.0)
        GF = 255.0;
    else if (GF < 0.0)
        GF = 0.0;

    *G = GF;

    double BF = 1.164 * Y + 2.018 * U - 276.928;
    if (BF > 255.0)
        BF = 255.0;
    else if (BF < 0.0)
        BF = 0.0;

    *B = BF;
}

void blipvert::SlowRGBtoYUV(double R, double G, double B, double* Y, double* U, double* V)
{
    *Y = 0.257 * R + 0.504 * G + 0.098 *B + 16;
    *U = -0.148 * R - 0.291 * G + 0.439 * B + 128;
    *V = 0.439 * R - 0.368 * G - 0.071 * B + 128;
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

uint32_t blipvert::CalculateBufferSize(const MediaFormatID& inFormat, uint32_t width, uint32_t height, int32_t stride)
{
    if (!ValidateGlobalBitmapDimension(width,height))
    {
        return 0;
    }

    t_calcbuffsizefunc calcfunct = FindBufSizeCalculator(inFormat);
    if (!calcfunct)
    {
        return 0;
    };

    return calcfunct(width, height, stride);
}

int32_t blipvert::CalculateMinimumLineStride(const MediaFormatID& inFormat, uint32_t width, uint32_t height)
{
    if (!ValidateGlobalBitmapDimension(width, height))
    {
        return 0;
    }

    t_calcbuffsizefunc calcfunct = FindBufSizeCalculator(inFormat);
    if (!calcfunct)
    {
        return 0;
    };

    int32_t stride = 0;
    uint32_t result = calcfunct(width, height, stride);
    return stride;
}

bool blipvert::IsRGBColorspace(const MediaFormatID& encoding)
{
    VideoFormatInfo info;
    if (GetVideoFormatInfo(encoding, info))
    {
        return info.type == ColorspaceType::RGB;
    }

    return false;
}

bool blipvert::IsRGBColorspace(const Fourcc fourcc)
{
    MediaFormatID mid;
    if (GetVideoFormatID(fourcc, mid))
    {
        return IsRGBColorspace(mid);
    }
    return false;
}

bool blipvert::IsYUVColorspace(const MediaFormatID& encoding)
{
    VideoFormatInfo info;
    if (GetVideoFormatInfo(encoding, info))
    {
        return info.type == ColorspaceType::YUV;
    }

    return false;
}

bool blipvert::IsYUVColorspace(const Fourcc fourcc)
{
    MediaFormatID mid;
    if (GetVideoFormatID(fourcc, mid))
    {
        return IsYUVColorspace(mid);
    }
    return false;
}

bool blipvert::IsPalletizedEncoding(const MediaFormatID& encoding)
{
    return (encoding == MVFMT_RGB1 ||
            encoding == MVFMT_RGB4 ||
            encoding == MVFMT_RGB8);
}

bool blipvert::IsPalletizedEncoding(const Fourcc fourcc)
{
    MediaFormatID mid;
    if (GetVideoFormatID(fourcc, mid))
    {
        return IsPalletizedEncoding(mid);
    }
    return false;
}


bool blipvert::IsPlanarYUV(const MediaFormatID& encoding)
{
    return  (encoding == MVFMT_I420 || encoding == MVFMT_YV12 || encoding == MVFMT_YVU9 ||
            encoding == MVFMT_YUV9 || encoding == MVFMT_NV12 || encoding == MVFMT_NV21 ||
            encoding == MVFMT_IMC1 || encoding == MVFMT_IMC2 || encoding == MVFMT_IMC3 ||
            encoding == MVFMT_IMC4 || encoding == MVFMT_YV16);
}

bool blipvert::IsPlanarYUV(const Fourcc fourcc)
{
    MediaFormatID mid;
    if (GetVideoFormatID(fourcc, mid))
    {
        return IsPlanarYUV(mid);
    }
    return false;
}

bool blipvert::IsGloballyValidBitmapDimension(int32_t width, int32_t height)
{
    return ValidateGlobalBitmapDimension(width, height);
}