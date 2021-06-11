#pragma once

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

namespace blipvert
{
    using Fourcc = uint32_t;

    extern const Fourcc FOURCC_UNDEFINED;
    extern const Fourcc FOURCC_UYVY;
    extern const Fourcc FOURCC_UYNV;
    extern const Fourcc FOURCC_cyuv;
    extern const Fourcc FOURCC_Y42T;
    extern const Fourcc FOURCC_Y422;
    extern const Fourcc FOURCC_IUYV;
    extern const Fourcc FOURCC_YUY2;
    extern const Fourcc FOURCC_YUYV;
    extern const Fourcc FOURCC_YUNV;
    extern const Fourcc FOURCC_V422;
    extern const Fourcc FOURCC_VYUY;
    extern const Fourcc FOURCC_Y411;
    extern const Fourcc FOURCC_Y41P;
    extern const Fourcc FOURCC_IY41;
    extern const Fourcc FOURCC_Y41T;
    extern const Fourcc FOURCC_Y800;
    extern const Fourcc FOURCC_Y8;
    extern const Fourcc FOURCC_GREY;
    extern const Fourcc FOURCC_IYU2;
    extern const Fourcc FOURCC_Y444;
    extern const Fourcc FOURCC_CLJR;
    extern const Fourcc FOURCC_YUVP;
    extern const Fourcc FOURCC_UYVP;
    extern const Fourcc FOURCC_YVYU;
    extern const Fourcc FOURCC_IYU1;
    extern const Fourcc FOURCC_Y211;
    extern const Fourcc FOURCC_V655;
    extern const Fourcc FOURCC_AYUV;
    extern const Fourcc FOURCC_YVU9;
    extern const Fourcc FOURCC_YUV9;
    extern const Fourcc FOURCC_IF09;
    extern const Fourcc FOURCC_IYUV;
    extern const Fourcc FOURCC_P420;
    extern const Fourcc FOURCC_I420;
    extern const Fourcc FOURCC_CLPL;
    extern const Fourcc FOURCC_YV12;

    extern const Fourcc FOURCC_BI_RGB;
    extern const Fourcc FOURCC_RGB;
    extern const Fourcc FOURCC_BI_RLE8;
    extern const Fourcc FOURCC_RLE8;
    extern const Fourcc FOURCC_BI_RLE4;
    extern const Fourcc FOURCC_RLE4;
    extern const Fourcc FOURCC_BI_BITFIELDS;
    extern const Fourcc FOURCC_BI_JPEG;
    extern const Fourcc FOURCC_BI_PNG;

    extern const Fourcc FOURCC_RGBA;
    extern const Fourcc FOURCC_RGBT;

    extern const Fourcc FOURCC_BGRA;

    extern void InitializeLibrary(void);
}