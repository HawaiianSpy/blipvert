#pragma once

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

#include "blipverttypes.h"
#include "ToGreyscale.h"
#include "ToFillColor.h"
#include "SetPixel.h"
#include "FlipVertical.h"
#include "CalculateBufferSize.h"
#include "Staging.h"
#include <string>
#include <memory>
#include <vector>

namespace blipvert
{
    typedef enum class ColorspaceType : unsigned short
    {
        Unknown = 0,        // I dunno, beats me.
        YUV = 1,            // YUV
        RGB = 2,            // RGB
        Codec = 3           // Codec of some kind
    } ColorspaceType;

    typedef struct {
        const MediaFormatID formatId;       // The string format ID
        Fourcc fourcc;                      // Fourcc code
        Fourcc xRefFourcc;                  // Cross-referenced fourcc or what this fourcc is a duplicate of format-wise.
        int16_t effectiveBitsPerPixel;      // Effective bits per pixel of this format.
        ColorspaceType type;                // YUV, RGB, etc.
        bool hasAlpha;                      // The format has an alpha / transparency value.
    } VideoFormatInfo;

    //
    // Fourcc video format definitions.
    //

    extern const Fourcc FOURCC_UNDEFINED;
    extern const Fourcc FOURCC_UYVY;            // https://www.fourcc.org/pixel-format/yuv-uyvy/
    extern const Fourcc FOURCC_UYNV;            // A direct copy of YUY2
    extern const Fourcc FOURCC_cyuv;            // https://www.fourcc.org/pixel-format/yuv-cyuv/
    extern const Fourcc FOURCC_Y42T;            // https://www.fourcc.org/pixel-format/yuv-y42t/
    extern const Fourcc FOURCC_Y422;            // Direct copy of UYVY
    extern const Fourcc FOURCC_HDYC;            // Direct copy of UYVY
    extern const Fourcc FOURCC_IUYV;            // Interlaced version of UYVY
    extern const Fourcc FOURCC_YUY2;            // https://www.fourcc.org/pixel-format/yuv-yuy2/
    extern const Fourcc FOURCC_YUYV;            // Duplicate of YUY2
    extern const Fourcc FOURCC_YUNV;            // Duplicate of YUY2
    extern const Fourcc FOURCC_V422;            // An upside down version of UYVY.
    extern const Fourcc FOURCC_VYUY;            // ATI Packed YUV Data (format unknown)
    extern const Fourcc FOURCC_Y411;            // https://www.fourcc.org/pixel-format/yuv-y411/
    extern const Fourcc FOURCC_Y41P;            // https://www.fourcc.org/pixel-format/yuv-y41p/
    extern const Fourcc FOURCC_IY41;            // https://www.fourcc.org/pixel-format/yuv-iy41/
    extern const Fourcc FOURCC_Y41T;            // https://www.fourcc.org/pixel-format/yuv-y41t/
    extern const Fourcc FOURCC_Y800;            // https://www.fourcc.org/pixel-format/yuv-y800/
    extern const Fourcc FOURCC_Y8;              // Duplicate of Y800
    extern const Fourcc FOURCC_GREY;            // Duplicate of Y800
    extern const Fourcc FOURCC_Y16;             // https://www.fourcc.org/pixel-format/yuv-y16/
    extern const Fourcc FOURCC_IYU2;            // https://www.fourcc.org/pixel-format/yuv-iyu2/
    extern const Fourcc FOURCC_Y444;
    extern const Fourcc FOURCC_CLJR;            // https://www.fourcc.org/pixel-format/yuv-cljr/
    extern const Fourcc FOURCC_YUVP;            // https://www.fourcc.org/pixel-format/yuv-yuvp/   
    extern const Fourcc FOURCC_UYVP;            // YCbCr 4:2:2 extended precision 10-bits per component in U0Y0V0Y1 order. ???
    extern const Fourcc FOURCC_YVYU;            // https://www.fourcc.org/pixel-format/yuv-yvyu/
    extern const Fourcc FOURCC_IYU1;            // https://www.fourcc.org/pixel-format/yuv-iyu1/
    extern const Fourcc FOURCC_Y211;            // https://www.fourcc.org/pixel-format/yuv-y211/
    extern const Fourcc FOURCC_V655;            // 16 bit YUV 4:2:2 format registered by Vitec Multimedia. ???
    extern const Fourcc FOURCC_AYUV;            // https://www.fourcc.org/pixel-format/yuv-ayuv/
    extern const Fourcc FOURCC_YVU9;            // https://www.fourcc.org/pixel-format/yuv-yvu9/
    extern const Fourcc FOURCC_YUV9;            // https://www.fourcc.org/pixel-format/yuv-yuv9/
    extern const Fourcc FOURCC_IF09;            // https://www.fourcc.org/pixel-format/yuv-if09/
    extern const Fourcc FOURCC_IYUV;            // Duplicate FOURCC, identical to I420.
    extern const Fourcc FOURCC_P420;
    extern const Fourcc FOURCC_I420;            // https://www.fourcc.org/pixel-format/yuv-i420/
    extern const Fourcc FOURCC_CLPL;            // https://www.fourcc.org/pixel-format/yuv-clpl/
    extern const Fourcc FOURCC_YV12;            // https://www.fourcc.org/pixel-format/yuv-yv12/

    extern const Fourcc FOURCC_NV12;            // https://www.fourcc.org/pixel-format/yuv-nv12/
    extern const Fourcc FOURCC_NV21;            // https://www.fourcc.org/pixel-format/yuv-nv21/
    extern const Fourcc FOURCC_IMC1;            // https://www.fourcc.org/pixel-format/yuv-imc1/
    extern const Fourcc FOURCC_IMC2;            // https://www.fourcc.org/pixel-format/yuv-imc2/
    extern const Fourcc FOURCC_IMC3;            // As IMC1 except that U and V are swapped
    extern const Fourcc FOURCC_IMC4;            // As IMC2 except that U and V are swapped
    extern const Fourcc FOURCC_YV16;            // https://www.fourcc.org/pixel-format/yuv-yv16/

    extern const Fourcc FOURCC_BI_RGB;          // https://www.fourcc.org/pixel-format/rgb-bi_rgb/
    extern const Fourcc FOURCC_RGB;             // Alias for BI_RGB
    extern const Fourcc FOURCC_BI_RLE8;         // https://www.fourcc.org/pixel-format/rgb-bi_rle8/
    extern const Fourcc FOURCC_RLE8;            // Alias for BI_RLE8
    extern const Fourcc FOURCC_BI_RLE4;         // https://www.fourcc.org/pixel-format/rgb-bi_rle4/
    extern const Fourcc FOURCC_RLE4;            // Alias for BI_RLE4
    extern const Fourcc FOURCC_BI_BITFIELDS;    // https://www.fourcc.org/pixel-format/rgb-bi_bitfields/
    extern const Fourcc FOURCC_BI_JPEG;         // Noteed here for completeness.
    extern const Fourcc FOURCC_BI_PNG;          // ...

    extern const Fourcc FOURCC_RGBA;            // https://www.fourcc.org/pixel-format/rgb-rgba/
                                                // I've never encountered an RGBA that wasn't 32 bits, therfore 32-bits is assumed here.
    extern const Fourcc FOURCC_RGBT;

    extern const Fourcc FOURCC_BGRA;

    //
    // Media type constants
    //

    extern const MediaFormatID MVFMT_UNDEFINED;
    extern const MediaFormatID MVFMT_UYVY;
    extern const MediaFormatID MVFMT_UYNV;
    extern const MediaFormatID MVFMT_cyuv;
    extern const MediaFormatID MVFMT_Y42T;
    extern const MediaFormatID MVFMT_Y422;
    extern const MediaFormatID MVFMT_HDYC;
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
    extern const MediaFormatID MVFMT_Y16;
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
    extern const MediaFormatID MVFMT_YV16;

    extern const MediaFormatID MVFMT_RGB1;
    extern const MediaFormatID MVFMT_RGB4;
    extern const MediaFormatID MVFMT_RGB8;
    extern const MediaFormatID MVFMT_RGB565;
    extern const MediaFormatID MVFMT_RGB555;
    extern const MediaFormatID MVFMT_ARGB1555;
    extern const MediaFormatID MVFMT_RGB24;
    extern const MediaFormatID MVFMT_RGB32;
    extern const MediaFormatID MVFMT_RGBA; 
    extern const MediaFormatID MVFMT_RGBT;
    extern const MediaFormatID MVFMT_RGB_BITFIELDS;

    extern bool IsInitialized;      // true / false that the library has been initialized.
    extern bool IsBigEndian;        // true indicates running on a big endian processor.

    //
    // Function pointer type for all colorspace transforms.
    //
    typedef void(__cdecl* t_transformfunc) (Stage* in, Stage* out);


    // IMPORTANT: This must be called before using any of the colorspace transforms since it initializes the lookup tables.
    void InitializeLibrary(void);

    // Finds a video transform for the given input / output media formats.
    // Returns a t_transformfunc pointer for the requested transform function. Retuns nullptr if a match couldn't be found.
    // Note: Since there exists duplicate fourcc definitions for the same bitmap format, the main 
    //       definition name will be used if a duplicate format was requested.
    t_transformfunc FindVideoTransform(const MediaFormatID& inFormat, const MediaFormatID& outFormat);

    // Finds a greyscale video transform for the given input media format.
    // Returns a t_greyscalefunc pointer for the requested transform function. Retuns nullptr if a match couldn't be found.
    // Note: Since there exists duplicate fourcc definitions for the same bitmap format, the main 
    //       definition name will be used if a duplicate format was requested.
    t_greyscalefunc FindGreyscaleTransform(const MediaFormatID& inFormat);

    // Finds a color fill video transform for the given input media format.
    // Returns a t_fillcolorfunc pointer for the requested transform function. Retuns nullptr if a match couldn't be found.
    // Note: Since there exists duplicate fourcc definitions for the same bitmap format, the main 
    //       definition name will be used if a duplicate format was requested.
    t_fillcolorfunc FindFillColorTransform(const MediaFormatID& inFormat);

    // Finds a individual pixel setter for the given format.
    // Returns a t_setpixelfunc pointer for the requested function. Retuns nullptr if a match couldn't be found.
    // Note: Since there exists duplicate fourcc definitions for the same bitmap format, the main 
    //       definition name will be used if a duplicate format was requested.
    t_setpixelfunc FindSetPixelColor(const MediaFormatID& inFormat);

    // Finds a vertical flip in place video transform for the given input media format.
    // Returns a t_flipverticalfunc pointer for the requested transform function. Retuns nullptr if a match couldn't be found.
    // Note: Since there exists duplicate fourcc definitions for the same bitmap format, the main 
    //       definition name will be used if a duplicate format was requested.
    t_flipverticalfunc FindFlipVerticalTransform(const MediaFormatID& inFormat);

    // Finds a buffer size calculation function for the given format.
    // Returns a t_calcbuffsizefunc pointer for the requested transform function. Retuns nullptr if a match couldn't be found.
    // Note: Since there exists duplicate fourcc definitions for the same bitmap format, the main 
    //       definition name will be used if a duplicate format was requested.
    t_calcbuffsizefunc FindBufSizeCalculator(const MediaFormatID& inFormat);

    // Finds a staging function for the given input media format.
    // Returns a t_stagetransformfunc pointer for the requested media format. Retuns nullptr if a match couldn't be found.
    // Note: Since there exists duplicate fourcc definitions for the same bitmap format, the main 
    //       definition name will be used if a duplicate format was requested.
    t_stagetransformfunc FindTransformStage(const MediaFormatID& format);

    // Returns information about the given video format.
    //
    // Parameters:
    //      inFormat:       IN  -> The ID of the media type.
    //      info:           OUT -> The info on the media type.
    //  Returns true if the format was found, false if not.
    bool GetVideoFormatInfo(const MediaFormatID& format, VideoFormatInfo& info);

    // Returns the MediaFormatID for a given fourcc code.
    //
    // Parameters:
    //      fourcc:         IN -> The fourcc codue to lookup.
    //      outFormat:      OUT -> The MediaFormatID that matches the fourcc code.
    // Returns true if a match was found, false otherwise.
    bool GetVideoFormatID(Fourcc fourcc, MediaFormatID& outFormat);
}