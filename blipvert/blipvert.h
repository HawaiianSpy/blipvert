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

#include "legacydefs.h"
#include <string>

namespace blipvert
{
    using Fourcc = uint32_t;
    using MediaFormatID = std::string;

    //
    // Fourcc video format definitions.
    //

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

    extern const Fourcc FOURCC_CPLA;
    extern const Fourcc FOURCC_IJPG;
    extern const Fourcc FOURCC_MJPG;
    extern const Fourcc FOURCC_TVMJ;
    extern const Fourcc FOURCC_WAKE;
    extern const Fourcc FOURCC_CFCC;
    extern const Fourcc FOURCC_Plum;
    extern const Fourcc FOURCC_DVCS;
    extern const Fourcc FOURCC_DVSD;
    extern const Fourcc FOURCC_MDVF;
    extern const Fourcc FOURCC_dvhd;
    extern const Fourcc FOURCC_dvsd;
    extern const Fourcc FOURCC_dvsl;

    //
    // Media type constants
    //

    extern const MediaFormatID MVFMT_UNDEFINED;
    extern const MediaFormatID MVFMT_UYVY;
    extern const MediaFormatID MVFMT_UYNV;
    extern const MediaFormatID MVFMT_cyuv;
    extern const MediaFormatID MVFMT_Y42T;
    extern const MediaFormatID MVFMT_Y422;
    extern const MediaFormatID MVFMT_IUYV;
    extern const MediaFormatID MVFMT_YUY2;
    extern const MediaFormatID MVFMT_YUYV;
    extern const MediaFormatID MVFMT_YUNV;
    extern const MediaFormatID MVFMT_V422;
    extern const MediaFormatID MVFMT_VYUY;
    extern const MediaFormatID MVFMT_Y411;
    extern const MediaFormatID MVFMT_Y41P;
    extern const MediaFormatID MVFMT_IY41;
    extern const MediaFormatID MVFMT_Y41T;
    extern const MediaFormatID MVFMT_Y800;
    extern const MediaFormatID MVFMT_Y8;
    extern const MediaFormatID MVFMT_GREY;
    extern const MediaFormatID MVFMT_IYU2;
    extern const MediaFormatID MVFMT_Y444;
    extern const MediaFormatID MVFMT_CLJR;
    extern const MediaFormatID MVFMT_YUVP;
    extern const MediaFormatID MVFMT_UYVP;
    extern const MediaFormatID MVFMT_YVYU;
    extern const MediaFormatID MVFMT_IYU1;
    extern const MediaFormatID MVFMT_Y211;
    extern const MediaFormatID MVFMT_V655;
    extern const MediaFormatID MVFMT_AYUV;

    extern const MediaFormatID MVFMT_YVU9;
    extern const MediaFormatID MVFMT_YUV9;
    extern const MediaFormatID MVFMT_IF09;
    extern const MediaFormatID MVFMT_IYUV;
    extern const MediaFormatID MVFMT_P420;
    extern const MediaFormatID MVFMT_I420;
    extern const MediaFormatID MVFMT_CLPL;
    extern const MediaFormatID MVFMT_YV12;
    extern const MediaFormatID MVFMT_NV12;
    extern const MediaFormatID MVFMT_NV21;
    extern const MediaFormatID MVFMT_IMC1;
    extern const MediaFormatID MVFMT_IMC2;
    extern const MediaFormatID MVFMT_IMC3;
    extern const MediaFormatID MVFMT_IMC4;
    extern const MediaFormatID MVFMT_S340;
    extern const MediaFormatID MVFMT_S342;

    extern const MediaFormatID MVFMT_RGB1;
    extern const MediaFormatID MVFMT_RGB4;
    extern const MediaFormatID MVFMT_RGB8;
    extern const MediaFormatID MVFMT_RGB565;
    extern const MediaFormatID MVFMT_RGB555;
    extern const MediaFormatID MVFMT_RGB24;
    extern const MediaFormatID MVFMT_RGB32;
    extern const MediaFormatID MVFMT_ARGB32;
    extern const MediaFormatID MVFMT_RGBA;
    extern const MediaFormatID MVFMT_RGBT;
    extern const MediaFormatID MVFMT_RGB_BITFIELDS;

    extern const MediaFormatID MVFMT_CPLA;
    extern const MediaFormatID MVFMT_IJPG;
    extern const MediaFormatID MVFMT_MJPG;
    extern const MediaFormatID MVFMT_TVMJ;
    extern const MediaFormatID MVFMT_WAKE;
    extern const MediaFormatID MVFMT_CFCC;
    extern const MediaFormatID MVFMT_Plum;
    extern const MediaFormatID MVFMT_DVCS;
    extern const MediaFormatID MVFMT_DVSD;
    extern const MediaFormatID MVFMT_MDVF;
    extern const MediaFormatID MVFMT_dvhd;
    extern const MediaFormatID MVFMT_dvsd;
    extern const MediaFormatID MVFMT_dvsl;

    //
    // Function pointer type for all colorspace transforms.
    //
    // Parameters:
    //      width & height: Dimensions in pixels of the video frame (inout and output).
    //      out_buf:        Byte pointer to the output buffer for the transform function.
    //      out_stride:     The number of bytes per row for the output buffer. 0 (zero) tells the function to
    //                      use the default bytes/row for the target output format.
    //      in_buf:         Byte pointer to the input buffer for the transform function.
    //      in_stride:      The number of bytes per row for the input buffer. 0 (zero) tells the function to
    //                      use the default bytes/row for the target input format.
    //      flipped:        true = flip the output bitmap vertically.
    //      in_palette:     Pointer to the palette for a palletized input bitmap. Ignored for non-palletized input bitmap formats.
    typedef void(__cdecl* t_transformfunc) (int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, RGBQUAD* in_palette);

    // IMPORTANT: This must be called before using any of the colorspace transforms since it initializes the lookup tables.
    void InitializeLibrary(void);

    // Finds a video transform for the given input / output media formats.
    // Returns a t_transformfunc pointer for the requested transform function. Retuns nullptr if a match couldn't be found.
    // Note: Since there exists duplicate fourcc definitions for the same bitmap format, the main 
    //       definition name will be used if a duplicate format was requested.
    t_transformfunc FindVideoTransform(const MediaFormatID& inFormat, const MediaFormatID& outFormat);

    // Returns information about the given video format.
    //
    // Parameters:
    //      inFormat:               IN -> The media ID to query.
    //      fourcc:                 OUT -> The fourcc code for the media format. Returns FOURCC_UNDEFINED if no fourcc is available.
    //      xRefFourcc:             OUT -> The master fourcc format if the given format is a duplicate, FOURCC_UNDEFINED otherwise.
    //      effctiveBitsPerPixel:   OUT -> The effective bits per pixel for the given format. Returns -1 if undefined.
    //  Returns true if the format was found, false if not.
    bool GetVideoFormatInfo(const MediaFormatID& inFormat, Fourcc& fourcc, Fourcc& xRefFourcc, int16_t& effctiveBitsPerPixel);

    // Returns the MediaFormatID for a given fourcc code.
    //
    // Parameters:
    //      fourcc:         IN -> The fourcc codue to lookup.
    //      outFormat:      OUT -> The MediaFormatID that matches the fourcc code.
    // Returns true if a match was found, false otherwise.
    bool GetVideoFormatID(Fourcc fourcc, MediaFormatID& outFormat);
}