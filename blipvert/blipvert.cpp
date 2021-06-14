
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
#include "framework.h"
#include "blipvert.h"
#include "CommonMacros.h"

#include "YUVtoRGB.h"
#include "RGBtoYUV.h"
#include "RGBtoRGB.h"
#include "YUVtoYUV.h"

#include <map>

using namespace std;
using namespace blipvert;


typedef struct {
    const MediaFormatID& inputEncoding;
    const MediaFormatID& outputEncoding;
    t_transformfunc pProcAddr;
} TransformTableEntry;

typedef struct {
    const MediaFormatID& target;
    t_rgbcheckfunc pProcAddr;
} RGBCheckEntry;

typedef struct {
    const MediaFormatID& target;
    t_yuvcheckfunc pProcAddr;
} YUVCheckEntry;

typedef struct {
    const MediaFormatID formatId;       // The string format ID
    Fourcc fourcc;                      // Fourcc code
    Fourcc xRefFourcc;                  // cross-referenced fourcc or what this fourcc is a duplicate of format-wise.
    int16_t effectiveBitsPerPixel;       // Effective bits per pixel of this format.
} VideoFormatInfo;

typedef struct {
    Fourcc fourcc;
    MediaFormatID formatId;
} FourccToMediaID;

/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

const Fourcc blipvert::FOURCC_UNDEFINED = 0xFFFFFFFF;
const Fourcc blipvert::FOURCC_UYVY = MAKEFOURCC('U', 'Y', 'V', 'Y');
const Fourcc blipvert::FOURCC_UYNV = MAKEFOURCC('U', 'Y', 'N', 'V');
const Fourcc blipvert::FOURCC_cyuv = MAKEFOURCC('c', 'y', 'u', 'v');
const Fourcc blipvert::FOURCC_Y42T = MAKEFOURCC('Y', '4', '2', 'T');
const Fourcc blipvert::FOURCC_Y422 = MAKEFOURCC('Y', '4', '2', '2');
const Fourcc blipvert::FOURCC_IUYV = MAKEFOURCC('I', 'U', 'Y', 'V');
const Fourcc blipvert::FOURCC_YUY2 = MAKEFOURCC('Y', 'U', 'Y', '2');
const Fourcc blipvert::FOURCC_YUYV = MAKEFOURCC('Y', 'U', 'Y', 'V');
const Fourcc blipvert::FOURCC_YUNV = MAKEFOURCC('Y', 'U', 'N', 'V');
const Fourcc blipvert::FOURCC_V422 = MAKEFOURCC('V', '4', '2', '2');
const Fourcc blipvert::FOURCC_VYUY = MAKEFOURCC('V', 'Y', 'U', 'Y');
const Fourcc blipvert::FOURCC_Y411 = MAKEFOURCC('Y', '4', '1', '1');
const Fourcc blipvert::FOURCC_Y41P = MAKEFOURCC('Y', '4', '1', 'P');
const Fourcc blipvert::FOURCC_IY41 = MAKEFOURCC('I', 'Y', '4', '1');
const Fourcc blipvert::FOURCC_Y41T = MAKEFOURCC('Y', '4', '1', 'T');
const Fourcc blipvert::FOURCC_Y800 = MAKEFOURCC('Y', '8', '0', '0');
const Fourcc blipvert::FOURCC_Y8 = MAKEFOURCC('Y', '8', ' ', ' ');
const Fourcc blipvert::FOURCC_GREY = MAKEFOURCC('G', 'R', 'E', 'Y');
const Fourcc blipvert::FOURCC_IYU2 = MAKEFOURCC('I', 'Y', 'U', '2');
const Fourcc blipvert::FOURCC_Y444 = MAKEFOURCC('Y', '4', '4', '4');
const Fourcc blipvert::FOURCC_CLJR = MAKEFOURCC('C', 'L', 'J', 'R');
const Fourcc blipvert::FOURCC_YUVP = MAKEFOURCC('Y', 'U', 'V', 'P');
const Fourcc blipvert::FOURCC_UYVP = MAKEFOURCC('U', 'Y', 'V', 'P');
const Fourcc blipvert::FOURCC_YVYU = MAKEFOURCC('Y', 'V', 'Y', 'U');
const Fourcc blipvert::FOURCC_IYU1 = MAKEFOURCC('I', 'Y', 'U', '1');
const Fourcc blipvert::FOURCC_Y211 = MAKEFOURCC('Y', '2', '1', '1');
const Fourcc blipvert::FOURCC_V655 = MAKEFOURCC('V', '6', '5', '5');
const Fourcc blipvert::FOURCC_AYUV = MAKEFOURCC('A', 'Y', 'U', 'V');
const Fourcc blipvert::FOURCC_YVU9 = MAKEFOURCC('Y', 'V', 'U', '9');
const Fourcc blipvert::FOURCC_YUV9 = MAKEFOURCC('Y', 'U', 'V', '9');
const Fourcc blipvert::FOURCC_IF09 = MAKEFOURCC('I', 'F', '0', '9');
const Fourcc blipvert::FOURCC_IYUV = MAKEFOURCC('I', 'Y', 'U', 'V');
const Fourcc blipvert::FOURCC_P420 = MAKEFOURCC('P', '4', '2', '0');
const Fourcc blipvert::FOURCC_I420 = MAKEFOURCC('I', '4', '2', '0');
const Fourcc blipvert::FOURCC_CLPL = MAKEFOURCC('C', 'L', 'P', 'L');
const Fourcc blipvert::FOURCC_YV12 = MAKEFOURCC('Y', 'V', '1', '2');

const Fourcc blipvert::FOURCC_BI_RGB = BI_RGB;
const Fourcc blipvert::FOURCC_RGB = MAKEFOURCC('R', 'G', 'B', '2');
const Fourcc blipvert::FOURCC_BI_RLE8 = BI_RLE8;
const Fourcc blipvert::FOURCC_RLE8 = MAKEFOURCC('R', 'L', 'E', '8');
const Fourcc blipvert::FOURCC_BI_RLE4 = BI_RLE4;
const Fourcc blipvert::FOURCC_RLE4 = MAKEFOURCC('R', 'L', 'E', '4');
const Fourcc blipvert::FOURCC_BI_BITFIELDS = BI_BITFIELDS;
const Fourcc blipvert::FOURCC_BI_JPEG = BI_JPEG;
const Fourcc blipvert::FOURCC_BI_PNG = BI_PNG;

const Fourcc blipvert::FOURCC_RGBA = MAKEFOURCC('R', 'G', 'B', 'A');
const Fourcc blipvert::FOURCC_RGBT = MAKEFOURCC('R', 'G', 'B', 'T');

const Fourcc blipvert::FOURCC_BGRA = MAKEFOURCC('B', 'G', 'R', 'A');

const Fourcc blipvert::FOURCC_CPLA = MAKEFOURCC('C', 'P', 'L', 'A');
const Fourcc blipvert::FOURCC_IJPG = MAKEFOURCC('I', 'J', 'P', 'G');
const Fourcc blipvert::FOURCC_MJPG = MAKEFOURCC('M', 'J', 'P', 'G');
const Fourcc blipvert::FOURCC_TVMJ = MAKEFOURCC('T', 'V', 'M', 'J');
const Fourcc blipvert::FOURCC_WAKE = MAKEFOURCC('W', 'A', 'K', 'E');
const Fourcc blipvert::FOURCC_CFCC = MAKEFOURCC('C', 'F', 'C', 'C');
const Fourcc blipvert::FOURCC_Plum = MAKEFOURCC('P', 'l', 'u', 'm');
const Fourcc blipvert::FOURCC_DVCS = MAKEFOURCC('D', 'V', 'C', 'S');
const Fourcc blipvert::FOURCC_DVSD = MAKEFOURCC('D', 'V', 'S', 'D');
const Fourcc blipvert::FOURCC_MDVF = MAKEFOURCC('M', 'D', 'V', 'F');
const Fourcc blipvert::FOURCC_dvhd = MAKEFOURCC('d', 'v', 'h', 'd');
const Fourcc blipvert::FOURCC_dvsd = MAKEFOURCC('d', 'v', 's', 'd');
const Fourcc blipvert::FOURCC_dvsl = MAKEFOURCC('d', 'v', 's', 'l');

const MediaFormatID blipvert::MVFMT_UNDEFINED("");
const MediaFormatID blipvert::MVFMT_UYVY("UYVY");
const MediaFormatID blipvert::MVFMT_UYNV("UYNV");
const MediaFormatID blipvert::MVFMT_cyuv("cyuv");
const MediaFormatID blipvert::MVFMT_Y42T("Y42T");
const MediaFormatID blipvert::MVFMT_Y422("Y422");
const MediaFormatID blipvert::MVFMT_IUYV("IUYV");
const MediaFormatID blipvert::MVFMT_YUY2("YUY2");
const MediaFormatID blipvert::MVFMT_YUYV("YUYV");
const MediaFormatID blipvert::MVFMT_YUNV("YUNV");
const MediaFormatID blipvert::MVFMT_V422("V422");
const MediaFormatID blipvert::MVFMT_VYUY("VYUY");
const MediaFormatID blipvert::MVFMT_Y411("Y411");
const MediaFormatID blipvert::MVFMT_Y41P("Y41P");
const MediaFormatID blipvert::MVFMT_IY41("IY41");
const MediaFormatID blipvert::MVFMT_Y41T("Y41T");
const MediaFormatID blipvert::MVFMT_Y800("Y800");
const MediaFormatID blipvert::MVFMT_Y8("Y8");
const MediaFormatID blipvert::MVFMT_GREY("GREY");
const MediaFormatID blipvert::MVFMT_IYU2("IYU2");
const MediaFormatID blipvert::MVFMT_Y444("Y444");
const MediaFormatID blipvert::MVFMT_CLJR("CLJR");
const MediaFormatID blipvert::MVFMT_YUVP("YUVP");
const MediaFormatID blipvert::MVFMT_UYVP("UYVP");
const MediaFormatID blipvert::MVFMT_YVYU("YVYU");
const MediaFormatID blipvert::MVFMT_IYU1("IYU1");
const MediaFormatID blipvert::MVFMT_Y211("Y211");
const MediaFormatID blipvert::MVFMT_V655("V655");
const MediaFormatID blipvert::MVFMT_AYUV("AYUV");

const MediaFormatID blipvert::MVFMT_YVU9("YVU9");
const MediaFormatID blipvert::MVFMT_YUV9("YUV9");
const MediaFormatID blipvert::MVFMT_IF09("IF09");
const MediaFormatID blipvert::MVFMT_IYUV("IYUV");
const MediaFormatID blipvert::MVFMT_P420("P420");
const MediaFormatID blipvert::MVFMT_I420("I420");
const MediaFormatID blipvert::MVFMT_CLPL("CLPL");
const MediaFormatID blipvert::MVFMT_YV12("YV12");
const MediaFormatID blipvert::MVFMT_NV12("NV12");
const MediaFormatID blipvert::MVFMT_NV21("NV21");
const MediaFormatID blipvert::MVFMT_IMC1("IMC1");
const MediaFormatID blipvert::MVFMT_IMC2("IMC2");
const MediaFormatID blipvert::MVFMT_IMC3("IMC3");
const MediaFormatID blipvert::MVFMT_IMC4("IMC4");
const MediaFormatID blipvert::MVFMT_S340("S340");
const MediaFormatID blipvert::MVFMT_S342("S342");

const MediaFormatID blipvert::MVFMT_RGB1("RGB1");
const MediaFormatID blipvert::MVFMT_RGB4("RGB4");
const MediaFormatID blipvert::MVFMT_RGB8("RGB8");
const MediaFormatID blipvert::MVFMT_RGB565("RGB565");
const MediaFormatID blipvert::MVFMT_RGB555("RGB555");
const MediaFormatID blipvert::MVFMT_RGB24("RGB24");
const MediaFormatID blipvert::MVFMT_RGB32("RGB32");
const MediaFormatID blipvert::MVFMT_ARGB32("ARGB32");
const MediaFormatID blipvert::MVFMT_RGBA("RGBA");
const MediaFormatID blipvert::MVFMT_RGBT("RGBT");
const MediaFormatID blipvert::MVFMT_RGB_BITFIELDS("RGB_BITFIELDS");

const MediaFormatID blipvert::MVFMT_CPLA("CPLA");
const MediaFormatID blipvert::MVFMT_IJPG("IJPG");
const MediaFormatID blipvert::MVFMT_MJPG("MJPG");
const MediaFormatID blipvert::MVFMT_TVMJ("TVMJ");
const MediaFormatID blipvert::MVFMT_WAKE("WAKE");
const MediaFormatID blipvert::MVFMT_CFCC("CFCC");
const MediaFormatID blipvert::MVFMT_Plum("Plum");
const MediaFormatID blipvert::MVFMT_DVCS("DVCS");
const MediaFormatID blipvert::MVFMT_DVSD("DVSD");
const MediaFormatID blipvert::MVFMT_MDVF("MDVF");
const MediaFormatID blipvert::MVFMT_dvhd("dvhd");
const MediaFormatID blipvert::MVFMT_dvsd("dvsd");
const MediaFormatID blipvert::MVFMT_dvsl("dvsl");

TransformTableEntry TransformTable[] = {
    { MVFMT_RGB32, MVFMT_RGB24, RGB32_to_RGB24 },
    { MVFMT_RGB32, MVFMT_RGB565, RGB32_to_RGB565 },
    { MVFMT_RGB32, MVFMT_RGB555, RGB32_to_RGB555 },

    { MVFMT_RGB24, MVFMT_RGB32, RGB24_to_RGB32 },
    { MVFMT_RGB24, MVFMT_RGB565, RGB24_to_RGB565 },
    { MVFMT_RGB24, MVFMT_RGB555, RGB24_to_RGB555 },

    { MVFMT_RGB555, MVFMT_RGB32, RGB555_to_RGB32 },
    { MVFMT_RGB555, MVFMT_RGB24, RGB555_to_RGB24 },
    { MVFMT_RGB555, MVFMT_RGB565, RGB555_to_RGB565 },

    { MVFMT_RGB565, MVFMT_RGB32, RGB565_to_RGB32 },
    { MVFMT_RGB565, MVFMT_RGB24, RGB565_to_RGB24 },
    { MVFMT_RGB565, MVFMT_RGB555, RGB565_to_RGB555 },

    { MVFMT_RGB8, MVFMT_RGB32, RGB8_to_RGB32 },
    { MVFMT_RGB8, MVFMT_RGB24, RGB8_to_RGB24 },
    { MVFMT_RGB8, MVFMT_RGB565, RGB8_to_RGB565 },
    { MVFMT_RGB8, MVFMT_RGB555, RGB8_to_RGB555 },

    { MVFMT_YUY2, MVFMT_RGB32, YUY2_to_RGB32 },
    { MVFMT_YUY2, MVFMT_RGB24, YUY2_to_RGB24 },
    { MVFMT_YUY2, MVFMT_RGB565, YUY2_to_RGB565 },
    { MVFMT_YUY2, MVFMT_RGB555, YUY2_to_RGB555 },

    { MVFMT_UYVY, MVFMT_RGB32, UYVY_to_RGB32 },
    { MVFMT_UYVY, MVFMT_RGB24, UYVY_to_RGB24 },
    { MVFMT_UYVY, MVFMT_RGB565, UYVY_to_RGB565 },
    { MVFMT_UYVY, MVFMT_RGB555, UYVY_to_RGB555 },

    { MVFMT_YVYU, MVFMT_RGB32, YVYU_to_RGB32 },
    { MVFMT_YVYU, MVFMT_RGB24, YVYU_to_RGB24 },
    { MVFMT_YVYU, MVFMT_RGB565, YVYU_to_RGB565 },
    { MVFMT_YVYU, MVFMT_RGB555, YVYU_to_RGB555 },

    { MVFMT_VYUY, MVFMT_RGB32, VYUY_to_RGB32 },
    { MVFMT_VYUY, MVFMT_RGB24, VYUY_to_RGB24 },
    { MVFMT_VYUY, MVFMT_RGB565, VYUY_to_RGB565 },
    { MVFMT_VYUY, MVFMT_RGB555, VYUY_to_RGB555 },

    { MVFMT_CLJR, MVFMT_RGB32, CLJR_to_RGB32 },
    { MVFMT_CLJR, MVFMT_RGB24, CLJR_to_RGB24 },
    { MVFMT_CLJR, MVFMT_RGB565, CLJR_to_RGB565 },
    { MVFMT_CLJR, MVFMT_RGB555, CLJR_to_RGB555 },

    { MVFMT_Y41P, MVFMT_RGB32, Y41P_to_RGB32 },
    { MVFMT_Y41P, MVFMT_RGB24, Y41P_to_RGB24 },
    { MVFMT_Y41P, MVFMT_RGB565, Y41P_to_RGB565 },
    { MVFMT_Y41P, MVFMT_RGB555, Y41P_to_RGB555 },

    { MVFMT_IYU1, MVFMT_RGB32, IYU1_to_RGB32 },
    { MVFMT_IYU1, MVFMT_RGB24, IYU1_to_RGB24 },
    { MVFMT_IYU1, MVFMT_RGB565, IYU1_to_RGB565 },
    { MVFMT_IYU1, MVFMT_RGB555, IYU1_to_RGB555 },

    { MVFMT_IYU2, MVFMT_RGB32, IYU2_to_RGB32 },
    { MVFMT_IYU2, MVFMT_RGB24, IYU2_to_RGB24 },
    { MVFMT_IYU2, MVFMT_RGB565, IYU2_to_RGB565 },
    { MVFMT_IYU2, MVFMT_RGB555, IYU2_to_RGB555 },

    { MVFMT_YVU9, MVFMT_RGB32, YVU9_to_RGB32 },
    { MVFMT_YVU9, MVFMT_RGB24, YVU9_to_RGB24 },
    { MVFMT_YVU9, MVFMT_RGB565, YVU9_to_RGB565 },
    { MVFMT_YVU9, MVFMT_RGB555, YVU9_to_RGB555 },

    { MVFMT_YUV9, MVFMT_RGB32, YUV9_to_RGB32 },
    { MVFMT_YUV9, MVFMT_RGB24, YUV9_to_RGB24 },
    { MVFMT_YUV9, MVFMT_RGB565, YUV9_to_RGB565 },
    { MVFMT_YUV9, MVFMT_RGB555, YUV9_to_RGB555 },

    { MVFMT_IYUV, MVFMT_RGB32, IYUV_to_RGB32 },
    { MVFMT_IYUV, MVFMT_RGB24, IYUV_to_RGB24 },
    { MVFMT_IYUV, MVFMT_RGB565, IYUV_to_RGB565 },
    { MVFMT_IYUV, MVFMT_RGB555, IYUV_to_RGB555 },

    { MVFMT_YV12, MVFMT_RGB32, YV12_to_RGB32 },
    { MVFMT_YV12, MVFMT_RGB24, YV12_to_RGB24 },
    { MVFMT_YV12, MVFMT_RGB565, YV12_to_RGB565 },
    { MVFMT_YV12, MVFMT_RGB555, YV12_to_RGB555 },

    { MVFMT_Y800, MVFMT_RGB32, Y800_to_RGB32 },
    { MVFMT_Y800, MVFMT_RGB24, Y800_to_RGB24 },
    { MVFMT_Y800, MVFMT_RGB565, Y800_to_RGB565 },
    { MVFMT_Y800, MVFMT_RGB555, Y800_to_RGB555 },

    { MVFMT_RGB32, MVFMT_YUY2, RGB32_to_YUY2 },
    { MVFMT_RGB32, MVFMT_UYVY, RGB32_to_UYVY },
    { MVFMT_RGB32, MVFMT_YVYU, RGB32_to_YVYU },
    { MVFMT_RGB32, MVFMT_VYUY, RGB32_to_VYUY },
    { MVFMT_RGB32, MVFMT_IYUV, RGB32_to_IYUV },
    { MVFMT_RGB32, MVFMT_YV12, RGB32_to_YV12 },
    { MVFMT_RGB32, MVFMT_YVU9, RGB32_to_YVU9 },
    { MVFMT_RGB32, MVFMT_YUV9, RGB32_to_YUV9 },
    { MVFMT_RGB32, MVFMT_IYU1, RGB32_to_IYU1 },
    { MVFMT_RGB32, MVFMT_IYU2, RGB32_to_IYU2 },
    { MVFMT_RGB32, MVFMT_Y41P, RGB32_to_Y41P },
    { MVFMT_RGB32, MVFMT_CLJR, RGB32_to_CLJR },
    { MVFMT_RGB32, MVFMT_Y800, RGB32_to_Y800 },

    { MVFMT_RGB24, MVFMT_YUY2, RGB24_to_YUY2 },
    { MVFMT_RGB24, MVFMT_UYVY, RGB24_to_UYVY },
    { MVFMT_RGB24, MVFMT_YVYU, RGB24_to_YVYU },
    { MVFMT_RGB24, MVFMT_VYUY, RGB24_to_VYUY },
    { MVFMT_RGB24, MVFMT_IYUV, RGB24_to_IYUV },
    { MVFMT_RGB24, MVFMT_YV12, RGB24_to_YV12 },
    { MVFMT_RGB24, MVFMT_YVU9, RGB24_to_YVU9 },
    { MVFMT_RGB24, MVFMT_YUV9, RGB24_to_YUV9 },
    { MVFMT_RGB24, MVFMT_IYU1, RGB24_to_IYU1 },
    { MVFMT_RGB24, MVFMT_IYU2, RGB24_to_IYU2 },
    { MVFMT_RGB24, MVFMT_Y41P, RGB24_to_Y41P },
    { MVFMT_RGB24, MVFMT_CLJR, RGB24_to_CLJR },
    { MVFMT_RGB24, MVFMT_Y800, RGB24_to_Y800 },

    { MVFMT_RGB565, MVFMT_YUY2, RGB565_to_YUY2 },
    { MVFMT_RGB565, MVFMT_UYVY, RGB565_to_UYVY },
    { MVFMT_RGB565, MVFMT_YVYU, RGB565_to_YVYU },
    { MVFMT_RGB565, MVFMT_VYUY, RGB565_to_VYUY },
    { MVFMT_RGB565, MVFMT_IYUV, RGB565_to_IYUV },
    { MVFMT_RGB565, MVFMT_YV12, RGB565_to_YV12 },
    { MVFMT_RGB565, MVFMT_YVU9, RGB565_to_YVU9 },
    { MVFMT_RGB565, MVFMT_YUV9, RGB565_to_YUV9 },
    { MVFMT_RGB565, MVFMT_IYU1, RGB565_to_IYU1 },
    { MVFMT_RGB565, MVFMT_IYU2, RGB565_to_IYU2 },
    { MVFMT_RGB565, MVFMT_Y41P, RGB565_to_Y41P },
    { MVFMT_RGB565, MVFMT_CLJR, RGB565_to_CLJR },
    { MVFMT_RGB565, MVFMT_Y800, RGB565_to_Y800 },

    { MVFMT_RGB555, MVFMT_YUY2, RGB555_to_YUY2 },
    { MVFMT_RGB555, MVFMT_UYVY, RGB555_to_UYVY },
    { MVFMT_RGB555, MVFMT_YVYU, RGB555_to_YVYU },
    { MVFMT_RGB555, MVFMT_VYUY, RGB555_to_VYUY },
    { MVFMT_RGB555, MVFMT_IYUV, RGB555_to_IYUV },
    { MVFMT_RGB555, MVFMT_YV12, RGB555_to_YV12 },
    { MVFMT_RGB555, MVFMT_YVU9, RGB555_to_YVU9 },
    { MVFMT_RGB555, MVFMT_YUV9, RGB555_to_YUV9 },
    { MVFMT_RGB555, MVFMT_IYU1, RGB555_to_IYU1 },
    { MVFMT_RGB555, MVFMT_IYU2, RGB555_to_IYU2 },
    { MVFMT_RGB555, MVFMT_Y41P, RGB555_to_Y41P },
    { MVFMT_RGB555, MVFMT_CLJR, RGB555_to_CLJR },
    { MVFMT_RGB555, MVFMT_Y800, RGB555_to_Y800 },

    { MVFMT_RGB8, MVFMT_YUY2, RGB8_to_YUY2 },
    { MVFMT_RGB8, MVFMT_UYVY, RGB8_to_UYVY },
    { MVFMT_RGB8, MVFMT_YVYU, RGB8_to_YVYU },
    { MVFMT_RGB8, MVFMT_VYUY, RGB8_to_VYUY },
    { MVFMT_RGB8, MVFMT_IYUV, RGB8_to_IYUV },
    { MVFMT_RGB8, MVFMT_YV12, RGB8_to_YV12 },
    { MVFMT_RGB8, MVFMT_YVU9, RGB8_to_YVU9 },
    { MVFMT_RGB8, MVFMT_YUV9, RGB8_to_YUV9 },
    { MVFMT_RGB8, MVFMT_IYU1, RGB8_to_IYU1 },
    { MVFMT_RGB8, MVFMT_IYU2, RGB8_to_IYU2 },
    { MVFMT_RGB8, MVFMT_Y41P, RGB8_to_Y41P },
    { MVFMT_RGB8, MVFMT_CLJR, RGB8_to_CLJR },
    { MVFMT_RGB8, MVFMT_Y800, RGB8_to_Y800 },

    { MVFMT_YUY2, MVFMT_UYVY, YUY2_to_UYVY },
    { MVFMT_YUY2, MVFMT_YVYU, YUY2_to_YVYU },
    { MVFMT_YUY2, MVFMT_VYUY, YUY2_to_VYUY },
    { MVFMT_YUY2, MVFMT_IYUV, YUY2_to_IYUV },
    { MVFMT_YUY2, MVFMT_YV12, YUY2_to_YV12 },
    { MVFMT_YUY2, MVFMT_YVU9, YUY2_to_YVU9 },
    { MVFMT_YUY2, MVFMT_YUV9, YUY2_to_YUV9 },
    { MVFMT_YUY2, MVFMT_IYU1, YUY2_to_IYU1 },
    { MVFMT_YUY2, MVFMT_IYU2, YUY2_to_IYU2 },
    { MVFMT_YUY2, MVFMT_Y800, YUY2_to_Y800 },
    { MVFMT_YUY2, MVFMT_CLJR, YUY2_to_CLJR },
    { MVFMT_YUY2, MVFMT_Y41P, YUY2_to_Y41P },

    { MVFMT_UYVY, MVFMT_YUY2, UYVY_to_YUY2 },
    { MVFMT_UYVY, MVFMT_YVYU, UYVY_to_YVYU },
    { MVFMT_UYVY, MVFMT_VYUY, UYVY_to_VYUY },
    { MVFMT_UYVY, MVFMT_IYUV, UYVY_to_IYUV },
    { MVFMT_UYVY, MVFMT_YV12, UYVY_to_YV12 },
    { MVFMT_UYVY, MVFMT_YVU9, UYVY_to_YVU9 },
    { MVFMT_UYVY, MVFMT_YUV9, UYVY_to_YUV9 },
    { MVFMT_UYVY, MVFMT_IYU1, UYVY_to_IYU1 },
    { MVFMT_UYVY, MVFMT_IYU2, UYVY_to_IYU2 },
    { MVFMT_UYVY, MVFMT_Y800, UYVY_to_Y800 },
    { MVFMT_UYVY, MVFMT_CLJR, UYVY_to_CLJR },
    { MVFMT_UYVY, MVFMT_Y41P, UYVY_to_Y41P },
    { MVFMT_UYVY, MVFMT_IUYV, UYVY_to_IUYV },

    { MVFMT_YVYU, MVFMT_YUY2, YVYU_to_YUY2 },
    { MVFMT_YVYU, MVFMT_UYVY, YVYU_to_UYVY },
    { MVFMT_YVYU, MVFMT_VYUY, YVYU_to_VYUY },
    { MVFMT_YVYU, MVFMT_IYUV, YVYU_to_IYUV },
    { MVFMT_YVYU, MVFMT_YV12, YVYU_to_YV12 },
    { MVFMT_YVYU, MVFMT_YVU9, YVYU_to_YVU9 },
    { MVFMT_YVYU, MVFMT_YUV9, YVYU_to_YUV9 },
    { MVFMT_YVYU, MVFMT_IYU1, YVYU_to_IYU1 },
    { MVFMT_YVYU, MVFMT_IYU2, YVYU_to_IYU2 },
    { MVFMT_YVYU, MVFMT_Y800, YVYU_to_Y800 },
    { MVFMT_YVYU, MVFMT_CLJR, YVYU_to_CLJR },
    { MVFMT_YVYU, MVFMT_Y41P, YVYU_to_Y41P },

    { MVFMT_VYUY, MVFMT_YUY2, VYUY_to_YUY2 },
    { MVFMT_VYUY, MVFMT_UYVY, VYUY_to_UYVY },
    { MVFMT_VYUY, MVFMT_YVYU, VYUY_to_YVYU },
    { MVFMT_VYUY, MVFMT_IYUV, VYUY_to_IYUV },
    { MVFMT_VYUY, MVFMT_YV12, VYUY_to_YV12 },
    { MVFMT_VYUY, MVFMT_YVU9, VYUY_to_YVU9 },
    { MVFMT_VYUY, MVFMT_YUV9, VYUY_to_YUV9 },
    { MVFMT_VYUY, MVFMT_IYU1, VYUY_to_IYU1 },
    { MVFMT_VYUY, MVFMT_IYU2, VYUY_to_IYU2 },
    { MVFMT_VYUY, MVFMT_Y800, VYUY_to_Y800 },
    { MVFMT_VYUY, MVFMT_CLJR, VYUY_to_CLJR },
    { MVFMT_VYUY, MVFMT_Y41P, VYUY_to_Y41P },

    { MVFMT_IYUV, MVFMT_YVU9, IYUV_to_YVU9 },
    { MVFMT_IYUV, MVFMT_YUV9, IYUV_to_YUV9 },
    { MVFMT_IYUV, MVFMT_YV12, IYUV_to_YV12 },
    { MVFMT_IYUV, MVFMT_YUY2, IYUV_to_YUY2 },
    { MVFMT_IYUV, MVFMT_UYVY, IYUV_to_UYVY },
    { MVFMT_IYUV, MVFMT_YVYU, IYUV_to_YVYU },
    { MVFMT_IYUV, MVFMT_VYUY, IYUV_to_VYUY },
    { MVFMT_IYUV, MVFMT_IYU1, IYUV_to_IYU1 },
    { MVFMT_IYUV, MVFMT_IYU2, IYUV_to_IYU2 },
    { MVFMT_IYUV, MVFMT_Y800, IYUV_to_Y800 },
    { MVFMT_IYUV, MVFMT_CLJR, IYUV_to_CLJR },
    { MVFMT_IYUV, MVFMT_Y41P, IYUV_to_Y41P },

    { MVFMT_YV12, MVFMT_YVU9, YV12_to_YVU9 },
    { MVFMT_YV12, MVFMT_YUV9, YV12_to_YUV9 },
    { MVFMT_YV12, MVFMT_IYUV, YV12_to_IYUV },
    { MVFMT_YV12, MVFMT_YUY2, YV12_to_YUY2 },
    { MVFMT_YV12, MVFMT_UYVY, YV12_to_UYVY },
    { MVFMT_YV12, MVFMT_YVYU, YV12_to_YVYU },
    { MVFMT_YV12, MVFMT_VYUY, YV12_to_VYUY },
    { MVFMT_YV12, MVFMT_IYU1, YV12_to_IYU1 },
    { MVFMT_YV12, MVFMT_IYU2, YV12_to_IYU2 },
    { MVFMT_YV12, MVFMT_Y800, YV12_to_Y800 },
    { MVFMT_YV12, MVFMT_CLJR, YV12_to_CLJR },
    { MVFMT_YV12, MVFMT_Y41P, YV12_to_Y41P },

    { MVFMT_YVU9, MVFMT_IYUV, YVU9_to_IYUV },
    { MVFMT_YVU9, MVFMT_YV12, YVU9_to_YV12 },
    { MVFMT_YVU9, MVFMT_YUV9, YVU9_to_YUV9 },
    { MVFMT_YVU9, MVFMT_YUY2, YVU9_to_YUY2 },
    { MVFMT_YVU9, MVFMT_UYVY, YVU9_to_UYVY },
    { MVFMT_YVU9, MVFMT_YVYU, YVU9_to_YVYU },
    { MVFMT_YVU9, MVFMT_VYUY, YVU9_to_VYUY },
    { MVFMT_YVU9, MVFMT_IYU1, YVU9_to_IYU1 },
    { MVFMT_YVU9, MVFMT_IYU2, YVU9_to_IYU2 },
    { MVFMT_YVU9, MVFMT_Y800, YVU9_to_Y800 },
    { MVFMT_YVU9, MVFMT_CLJR, YVU9_to_CLJR },
    { MVFMT_YVU9, MVFMT_Y41P, YVU9_to_Y41P },

    { MVFMT_YUV9, MVFMT_IYUV, YUV9_to_IYUV },
    { MVFMT_YUV9, MVFMT_YV12, YUV9_to_YV12 },
    { MVFMT_YUV9, MVFMT_YVU9, YUV9_to_YVU9 },
    { MVFMT_YUV9, MVFMT_YUY2, YUV9_to_YUY2 },
    { MVFMT_YUV9, MVFMT_UYVY, YUV9_to_UYVY },
    { MVFMT_YUV9, MVFMT_YVYU, YUV9_to_YVYU },
    { MVFMT_YUV9, MVFMT_VYUY, YUV9_to_VYUY },
    { MVFMT_YUV9, MVFMT_IYU1, YUV9_to_IYU1 },
    { MVFMT_YUV9, MVFMT_IYU2, YUV9_to_IYU2 },
    { MVFMT_YUV9, MVFMT_Y800, YUV9_to_Y800 },
    { MVFMT_YUV9, MVFMT_CLJR, YUV9_to_CLJR },
    { MVFMT_YUV9, MVFMT_Y41P, YUV9_to_Y41P },

    { MVFMT_IYU1, MVFMT_YUY2, IYU1_to_YUY2 },
    { MVFMT_IYU1, MVFMT_UYVY, IYU1_to_UYVY },
    { MVFMT_IYU1, MVFMT_YVYU, IYU1_to_YVYU },
    { MVFMT_IYU1, MVFMT_VYUY, IYU1_to_VYUY },
    { MVFMT_IYU1, MVFMT_IYUV, IYU1_to_IYUV },
    { MVFMT_IYU1, MVFMT_YV12, IYU1_to_YV12 },
    { MVFMT_IYU1, MVFMT_YVU9, IYU1_to_YVU9 },
    { MVFMT_IYU1, MVFMT_YUV9, IYU1_to_YUV9 },
    { MVFMT_IYU1, MVFMT_IYU2, IYU1_to_IYU2 },
    { MVFMT_IYU1, MVFMT_Y800, IYU1_to_Y800 },
    { MVFMT_IYU1, MVFMT_CLJR, IYU1_to_CLJR },
    { MVFMT_IYU1, MVFMT_Y41P, IYU1_to_Y41P },

    { MVFMT_IYU2, MVFMT_YUY2, IYU2_to_YUY2 },
    { MVFMT_IYU2, MVFMT_UYVY, IYU2_to_UYVY },
    { MVFMT_IYU2, MVFMT_YVYU, IYU2_to_YVYU },
    { MVFMT_IYU2, MVFMT_VYUY, IYU2_to_VYUY },
    { MVFMT_IYU2, MVFMT_IYUV, IYU2_to_IYUV },
    { MVFMT_IYU2, MVFMT_YV12, IYU2_to_YV12 },
    { MVFMT_IYU2, MVFMT_YVU9, IYU2_to_YVU9 },
    { MVFMT_IYU2, MVFMT_YUV9, IYU2_to_YUV9 },
    { MVFMT_IYU2, MVFMT_IYU1, IYU2_to_IYU1 },
    { MVFMT_IYU2, MVFMT_Y800, IYU2_to_Y800 },
    { MVFMT_IYU2, MVFMT_CLJR, IYU2_to_CLJR },
    { MVFMT_IYU2, MVFMT_Y41P, IYU2_to_Y41P },

    { MVFMT_Y800, MVFMT_YUY2, Y800_to_YUY2 },
    { MVFMT_Y800, MVFMT_UYVY, Y800_to_UYVY },
    { MVFMT_Y800, MVFMT_YVYU, Y800_to_YVYU },
    { MVFMT_Y800, MVFMT_VYUY, Y800_to_VYUY },
    { MVFMT_Y800, MVFMT_IYUV, Y800_to_IYUV },
    { MVFMT_Y800, MVFMT_YV12, Y800_to_YV12 },
    { MVFMT_Y800, MVFMT_YVU9, Y800_to_YVU9 },
    { MVFMT_Y800, MVFMT_YUV9, Y800_to_YUV9 },
    { MVFMT_Y800, MVFMT_IYU1, Y800_to_IYU1 },
    { MVFMT_Y800, MVFMT_IYU2, Y800_to_IYU2 },
    { MVFMT_Y800, MVFMT_CLJR, Y800_to_CLJR },
    { MVFMT_Y800, MVFMT_Y41P, Y800_to_Y41P },

    { MVFMT_CLJR, MVFMT_YUY2, CLJR_to_YUY2 },
    { MVFMT_CLJR, MVFMT_UYVY, CLJR_to_UYVY },
    { MVFMT_CLJR, MVFMT_YVYU, CLJR_to_YVYU },
    { MVFMT_CLJR, MVFMT_VYUY, CLJR_to_VYUY },
    { MVFMT_CLJR, MVFMT_IYUV, CLJR_to_IYUV },
    { MVFMT_CLJR, MVFMT_YV12, CLJR_to_YV12 },
    { MVFMT_CLJR, MVFMT_YVU9, CLJR_to_YVU9 },
    { MVFMT_CLJR, MVFMT_YUV9, CLJR_to_YUV9 },
    { MVFMT_CLJR, MVFMT_IYU1, CLJR_to_IYU1 },
    { MVFMT_CLJR, MVFMT_IYU2, CLJR_to_IYU2 },
    { MVFMT_CLJR, MVFMT_Y800, CLJR_to_Y800 },
    { MVFMT_CLJR, MVFMT_Y41P, CLJR_to_Y41P },

    { MVFMT_Y41P, MVFMT_YUY2, Y41P_to_YUY2 },
    { MVFMT_Y41P, MVFMT_UYVY, Y41P_to_UYVY },
    { MVFMT_Y41P, MVFMT_YVYU, Y41P_to_YVYU },
    { MVFMT_Y41P, MVFMT_VYUY, Y41P_to_VYUY },
    { MVFMT_Y41P, MVFMT_IYUV, Y41P_to_IYUV },
    { MVFMT_Y41P, MVFMT_YV12, Y41P_to_YV12 },
    { MVFMT_Y41P, MVFMT_YVU9, Y41P_to_YVU9 },
    { MVFMT_Y41P, MVFMT_YUV9, Y41P_to_YUV9 },
    { MVFMT_Y41P, MVFMT_IYU1, Y41P_to_IYU1 },
    { MVFMT_Y41P, MVFMT_IYU2, Y41P_to_IYU2 },
    { MVFMT_Y41P, MVFMT_Y800, Y41P_to_Y800 },
    { MVFMT_Y41P, MVFMT_CLJR, Y41P_to_CLJR },
    { MVFMT_Y41P, MVFMT_IY41, Y41P_to_IY41 },

    { MVFMT_IUYV, MVFMT_UYVY, IUYV_to_UYVY },

    { MVFMT_IY41, MVFMT_Y41P, IY41_to_Y41P },

    { MVFMT_UNDEFINED, MVFMT_UNDEFINED, nullptr }
};

VideoFormatInfo VideoFmtTable[] = {
    // Packed YUV Formats:
    // UYVY master format
    {MVFMT_UYVY, FOURCC_UYVY, FOURCC_UYVY, 16},
    {MVFMT_Y422, FOURCC_Y422, FOURCC_UYVY, 16},
    {MVFMT_UYNV, FOURCC_UYNV, FOURCC_UYVY, 16},
    {MVFMT_cyuv, FOURCC_cyuv, FOURCC_UYVY, 16},
    {MVFMT_V422, FOURCC_V422, FOURCC_UYVY, 16},

    // YUY2 master format
    {MVFMT_YUY2, FOURCC_YUY2, FOURCC_YUY2, 16},
    {MVFMT_YUYV, FOURCC_YUYV, FOURCC_YUY2, 16},
    {MVFMT_YUNV, FOURCC_YUNV, FOURCC_YUY2, 16},

    // Y800 master format
    {MVFMT_Y800, FOURCC_Y800, FOURCC_Y800, 8},
    {MVFMT_Y8, FOURCC_Y8, FOURCC_Y800, 8},
    {MVFMT_GREY, FOURCC_GREY, FOURCC_Y800},

    // IYU1 master format
    {MVFMT_IYU1, FOURCC_IYU1, FOURCC_IYU1, 12},
    {MVFMT_Y411, FOURCC_Y411, FOURCC_IYU1, 12},

    // IYU2 master format
    {MVFMT_IYU2, FOURCC_IYU2, FOURCC_IYU2, 24},
    {MVFMT_Y444, FOURCC_Y444, FOURCC_IYU2, 24},

    // Packed YUV formats with no known duplicate definitions
    {MVFMT_Y41P, FOURCC_Y41P, FOURCC_UNDEFINED, 12},
    // Y41T is identical to Y41P except for the fact that the least significant bit of
    // each Y component forms a chromakey channel.
    {MVFMT_Y41T, FOURCC_Y41T, FOURCC_UNDEFINED, 12},
    // Y42T is identical to UYVY except for the fact that the least significant bit of
    // each Y component forms a chromakey channel. 
    {MVFMT_Y42T, FOURCC_Y42T, FOURCC_UNDEFINED, 16},
    // IY41 is basically the same as Y41P with the exception that the data is interlaced. Lines are
    // ordered 0,2,4,....,1,3,5.... instead of 0,1,2,3,4,5,....
    {MVFMT_IY41, FOURCC_IY41, FOURCC_UNDEFINED, 12},
    // IUYV is basically the same as UYVY with the exception that the data is interlaced. Lines are
    // ordered 0,2,4,....,1,3,5.... instead of 0,1,2,3,4,5,....
    {MVFMT_IUYV, FOURCC_IUYV, FOURCC_UNDEFINED, 16},
    {MVFMT_CLJR, FOURCC_CLJR, FOURCC_UNDEFINED, 8},
    {MVFMT_YUVP, FOURCC_YUVP, FOURCC_UNDEFINED, 24},
    {MVFMT_UYVP, FOURCC_UYVP, FOURCC_UNDEFINED, 24},
    {MVFMT_YVYU, FOURCC_YVYU, FOURCC_UNDEFINED, 16},
    {MVFMT_Y211, FOURCC_Y211, FOURCC_UNDEFINED, 8},
    {MVFMT_V655, FOURCC_V655, FOURCC_UNDEFINED, 16},
    {MVFMT_AYUV, FOURCC_AYUV, FOURCC_UNDEFINED, 32},
    {MVFMT_VYUY, FOURCC_VYUY, FOURCC_UNDEFINED, 16},

    // Planar YUV Formats:
    // IYUV master format
    {MVFMT_IYUV, FOURCC_IYUV, FOURCC_IYUV, 12},
    {MVFMT_P420, FOURCC_P420, FOURCC_IYUV, 12},
    {MVFMT_I420, FOURCC_I420, FOURCC_IYUV, 12},
    {MVFMT_CLPL, FOURCC_CLPL, FOURCC_IYUV, 12},

    // Planar YUV with no known duplicate definitions
    {MVFMT_YV12, FOURCC_YV12, FOURCC_UNDEFINED, 12},
    {MVFMT_YVU9, FOURCC_YVU9, FOURCC_UNDEFINED, 9},
    {MVFMT_YUV9, FOURCC_YUV9, FOURCC_UNDEFINED, 9},
    {MVFMT_IF09, FOURCC_IF09, FOURCC_UNDEFINED, 9},

    // RGB Formats:
    {MVFMT_RGB1, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 1},
    {MVFMT_RGB4, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 4},
    {MVFMT_RGB8, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 8},
    {MVFMT_RGB555, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 16},
    {MVFMT_RGB565, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 16},
    {MVFMT_RGB24, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 24},
    {MVFMT_RGB32, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 32},
    {MVFMT_ARGB32, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 32},
    {MVFMT_RGBA, FOURCC_RGBA, FOURCC_UNDEFINED, 32},
    {MVFMT_RGBT, FOURCC_RGBT, FOURCC_UNDEFINED, -1},
    {MVFMT_RGB_BITFIELDS, FOURCC_BI_BITFIELDS, FOURCC_UNDEFINED, -1},

    // Misc formats for which MS has a GUIDs defined.
    {MVFMT_CPLA, FOURCC_CPLA, FOURCC_UNDEFINED, -1},
    {MVFMT_IJPG, FOURCC_IJPG, FOURCC_UNDEFINED, -1},
    {MVFMT_MJPG, FOURCC_MJPG, FOURCC_UNDEFINED, -1},
    {MVFMT_TVMJ, FOURCC_TVMJ, FOURCC_UNDEFINED, -1},
    {MVFMT_WAKE, FOURCC_WAKE, FOURCC_UNDEFINED, -1},
    {MVFMT_CFCC, FOURCC_CFCC, FOURCC_UNDEFINED, -1},
    {MVFMT_Plum, FOURCC_Plum, FOURCC_UNDEFINED, -1},
    {MVFMT_DVCS, FOURCC_DVCS, FOURCC_UNDEFINED, -1},
    {MVFMT_DVSD, FOURCC_DVSD, FOURCC_UNDEFINED, -1},
    {MVFMT_MDVF, FOURCC_MDVF, FOURCC_UNDEFINED, -1},

    {MVFMT_dvhd, FOURCC_dvhd, FOURCC_UNDEFINED, -1},
    {MVFMT_dvsd, FOURCC_dvsd, FOURCC_UNDEFINED, -1},
    {MVFMT_dvsl, FOURCC_dvsl, FOURCC_UNDEFINED, -1},

    {MVFMT_UNDEFINED, FOURCC_UNDEFINED, FOURCC_UNDEFINED, -1}
};

RGBCheckEntry RGBCheckFuncTable[] = {
    {MVFMT_RGB32, Check_RGB32},
    {MVFMT_RGB24, Check_RGB24},
    {MVFMT_RGB565, Check_RGB565},
    {MVFMT_RGB555, Check_RGB555},
    {MVFMT_UNDEFINED, nullptr}
};

YUVCheckEntry YUVCheckFuncTable[] = {
    {MVFMT_YUY2, Check_YUY2},
    {MVFMT_UYVY, Check_UYVY},
    {MVFMT_YVYU, Check_YVYU},
    {MVFMT_VYUY, Check_VYUY},
    {MVFMT_IYUV, Check_IYUV},
    {MVFMT_YV12, Check_YV12},
    {MVFMT_YVU9, Check_YVU9},
    {MVFMT_YUV9, Check_YUV9},
    {MVFMT_IYU1, Check_IYU1},
    {MVFMT_IYU2, Check_IYU2},
    {MVFMT_Y800, Check_Y800},
    {MVFMT_Y41P, Check_Y41P},
    {MVFMT_CLJR, Check_CLJR},
    {MVFMT_UNDEFINED, nullptr}
};

map<MediaFormatID, t_transformfunc> TransformMap;
map<MediaFormatID, VideoFormatInfo*> MediaFormatInfoMap;
map<Fourcc, const MediaFormatID> FourccToIDMap;
map<MediaFormatID, t_rgbcheckfunc> RGBCheckFuncMap;
map<MediaFormatID, t_yuvcheckfunc> YUVCheckFuncMap;

bool IsInitialized = false;

void blipvert::InitializeLibrary(void)
{
    InitYUVtoRGBTables();
    InitRGBtoYUVTables();

    // Build lookup maps
    uint16_t index = 0;
    while (TransformTable[index].pProcAddr != nullptr)
    {
        MediaFormatID key = TransformTable[index].inputEncoding + TransformTable[index].outputEncoding;
        TransformMap.insert(make_pair(key, TransformTable[index].pProcAddr));
        index++;
    }

    index = 0;
    while (VideoFmtTable[index].formatId != MVFMT_UNDEFINED)
    {
        MediaFormatInfoMap.insert(make_pair(VideoFmtTable[index].formatId, &VideoFmtTable[index]));

        if (VideoFmtTable[index].fourcc != FOURCC_UNDEFINED)
        {
            FourccToIDMap.insert(make_pair(VideoFmtTable[index].fourcc, VideoFmtTable[index].formatId));
        }

        index++;
    }

    index = 0;
    while (RGBCheckFuncTable[index].pProcAddr != nullptr)
    {
        RGBCheckFuncMap.insert(make_pair(RGBCheckFuncTable[index].target, RGBCheckFuncTable[index].pProcAddr));
        index++;
    }

    index = 0;
    while (YUVCheckFuncTable[index].pProcAddr != nullptr)
    {
        YUVCheckFuncMap.insert(make_pair(YUVCheckFuncTable[index].target, YUVCheckFuncTable[index].pProcAddr));
        index++;
    }

    IsInitialized = true;
}

t_rgbcheckfunc blipvert::FindRGBCheckFunction(const MediaFormatID& target)
{
    map<MediaFormatID, t_rgbcheckfunc>::iterator it = RGBCheckFuncMap.find(target);
    if (it != RGBCheckFuncMap.end())
    {
        return *(it->second);
    }

    return nullptr;
}

t_yuvcheckfunc blipvert::FindYUVCheckFunction(const MediaFormatID& target)
{
    map<MediaFormatID, t_yuvcheckfunc>::iterator it = YUVCheckFuncMap.find(target);
    if (it != YUVCheckFuncMap.end())
    {
        return *(it->second);
    }

    return nullptr;
}

t_transformfunc blipvert::FindVideoTransform(const MediaFormatID& inFormat, const MediaFormatID& outFormat)
{
    MediaFormatID key = inFormat + outFormat;
    map<MediaFormatID, t_transformfunc>::iterator it = TransformMap.find(key);
    if (it != TransformMap.end())
    {
        return *(it->second);
    }

    // Not found, so try cross-referenced formats in case there's a known duplicate definition.
    Fourcc infourcc;
    Fourcc inxRefFourcc;
    int16_t ineffctiveBitsPerPixel;

    Fourcc outfourcc;
    Fourcc outxRefFourcc;
    int16_t outeffctiveBitsPerPixel;

    if (GetVideoFormatInfo(inFormat, infourcc, inxRefFourcc, ineffctiveBitsPerPixel) &&
        GetVideoFormatInfo(outFormat, outfourcc, outxRefFourcc, outeffctiveBitsPerPixel))
    {
        MediaFormatID inid;
        MediaFormatID outid;
        if (GetVideoFormatID(inxRefFourcc, inid) && GetVideoFormatID(outxRefFourcc, outid))
        {
            key = inid + outid;
            map<MediaFormatID, t_transformfunc>::iterator it = TransformMap.find(key);
            if (it != TransformMap.end())
            {
                return *(it->second);
            }
        }
    }


    return nullptr;
}

bool blipvert::GetVideoFormatInfo(const MediaFormatID& inFormat, Fourcc& fourcc, Fourcc& xRefFourcc, int16_t& effectiveBitsPerPixel)
{
    map<MediaFormatID, VideoFormatInfo*>::iterator it = MediaFormatInfoMap.find(inFormat);
    if (it != MediaFormatInfoMap.end())
    {
        fourcc = it->second->fourcc;
        xRefFourcc = it->second->xRefFourcc;
        effectiveBitsPerPixel = it->second->effectiveBitsPerPixel;
        return true;
    }

    return false;
}

bool blipvert::GetVideoFormatID(Fourcc fourcc, MediaFormatID& outFormat)
{
    map<Fourcc, const MediaFormatID>::iterator it = FourccToIDMap.find(fourcc);
    if (it != FourccToIDMap.end())
    {
        outFormat = it->second;
        return true;
    }

    outFormat = MVFMT_UNDEFINED;
    return false;
}
