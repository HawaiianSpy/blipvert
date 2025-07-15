
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
#include "framework.h"
#include "blipvert.h"
#include "CommonMacros.h"
#include "LookupTables.h"

#include "YUVtoRGB.h"
#include "RGBtoYUV.h"
#include "RGBtoRGB.h"
#include "YUVtoYUV.h"

#include <map>

using namespace std;
using namespace blipvert;

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
const Fourcc blipvert::FOURCC_HDYC = MAKEFOURCC('H', 'Y', 'D', 'C');
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
const Fourcc blipvert::FOURCC_Y16 = MAKEFOURCC('Y', '1', '6', ' ');
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

const Fourcc blipvert::FOURCC_NV12 = MAKEFOURCC('N', 'V', '1', '2');
const Fourcc blipvert::FOURCC_NV21 = MAKEFOURCC('N', 'V', '2', '1');
const Fourcc blipvert::FOURCC_IMC1 = MAKEFOURCC('I', 'M', 'C', '1');
const Fourcc blipvert::FOURCC_IMC2 = MAKEFOURCC('I', 'M', 'C', '2');
const Fourcc blipvert::FOURCC_IMC3 = MAKEFOURCC('I', 'M', 'C', '3');
const Fourcc blipvert::FOURCC_IMC4 = MAKEFOURCC('I', 'M', 'C', '4');
const Fourcc blipvert::FOURCC_YV16 = MAKEFOURCC('Y', 'V', '1', '6');


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

const MediaFormatID blipvert::MVFMT_UNDEFINED("");
const MediaFormatID blipvert::MVFMT_UYVY("UYVY");
const MediaFormatID blipvert::MVFMT_UYNV("UYNV");
const MediaFormatID blipvert::MVFMT_cyuv("cyuv");
const MediaFormatID blipvert::MVFMT_Y42T("Y42T");
const MediaFormatID blipvert::MVFMT_Y422("Y422");
const MediaFormatID blipvert::MVFMT_HDYC("HDYC");
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
const MediaFormatID blipvert::MVFMT_Y16("Y16");
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
const MediaFormatID blipvert::MVFMT_YV16("YV16");

const MediaFormatID blipvert::MVFMT_RGB1("RGB1");
const MediaFormatID blipvert::MVFMT_RGB4("RGB4");
const MediaFormatID blipvert::MVFMT_RGB8("RGB8");
const MediaFormatID blipvert::MVFMT_RGB565("RGB565");
const MediaFormatID blipvert::MVFMT_RGB555("RGB555");
const MediaFormatID blipvert::MVFMT_ARGB1555("ARGB1555");
const MediaFormatID blipvert::MVFMT_RGB24("RGB24");
const MediaFormatID blipvert::MVFMT_RGB32("RGB32");
const MediaFormatID blipvert::MVFMT_RGBA("RGBA");
const MediaFormatID blipvert::MVFMT_RGBT("RGBT");
const MediaFormatID blipvert::MVFMT_RGB_BITFIELDS("RGB_BITFIELDS");

map<std::string, t_transformfunc> TransformMap = {

    // RGB to RGB

    { MVFMT_RGBA + MVFMT_RGB32, RGBA_to_RGB32 },
    { MVFMT_RGBA + MVFMT_RGB24, RGB32_to_RGB24 },
    { MVFMT_RGBA + MVFMT_RGB565, RGB32_to_RGB565 },
    { MVFMT_RGBA + MVFMT_RGB555, RGB32_to_RGB555 },
    { MVFMT_RGBA + MVFMT_ARGB1555, RGBA_to_ARGB1555 },

    { MVFMT_RGB32 + MVFMT_RGBA, RGB32_to_RGBA },
    { MVFMT_RGB32 + MVFMT_RGB24, RGB32_to_RGB24 },
    { MVFMT_RGB32 + MVFMT_RGB565, RGB32_to_RGB565 },
    { MVFMT_RGB32 + MVFMT_RGB555, RGB32_to_RGB555 },
    { MVFMT_RGB32 + MVFMT_ARGB1555, RGB32_to_RGB555 },

    { MVFMT_RGB24 + MVFMT_RGBA, RGB24_to_RGB32 },
    { MVFMT_RGB24 + MVFMT_RGB32, RGB24_to_RGB32 },
    { MVFMT_RGB24 + MVFMT_RGB565, RGB24_to_RGB565 },
    { MVFMT_RGB24 + MVFMT_RGB555, RGB24_to_RGB555 },
    { MVFMT_RGB24 + MVFMT_ARGB1555, RGB24_to_RGB555 },

    { MVFMT_RGB555 + MVFMT_RGBA, RGB555_to_RGBA },
    { MVFMT_RGB555 + MVFMT_RGB32, RGB555_to_RGB32 },
    { MVFMT_RGB555 + MVFMT_RGB24, RGB555_to_RGB24 },
    { MVFMT_RGB555 + MVFMT_RGB565, RGB555_to_RGB565 },
    { MVFMT_RGB555 + MVFMT_ARGB1555, RGB555_to_ARGB1555 },

    { MVFMT_ARGB1555 + MVFMT_RGBA, ARGB1555_to_RGBA },
    { MVFMT_ARGB1555 + MVFMT_RGB32, RGB555_to_RGB32 },
    { MVFMT_ARGB1555 + MVFMT_RGB24, RGB555_to_RGB24 },
    { MVFMT_ARGB1555 + MVFMT_RGB565, RGB555_to_RGB565 },
    { MVFMT_ARGB1555 + MVFMT_RGB555, ARGB1555_to_RGB555 },

    { MVFMT_RGB565 + MVFMT_RGBA, RGB565_to_RGB32 },
    { MVFMT_RGB565 + MVFMT_RGB32, RGB565_to_RGB32 },
    { MVFMT_RGB565 + MVFMT_RGB24, RGB565_to_RGB24 },
    { MVFMT_RGB565 + MVFMT_RGB555, RGB565_to_RGB555 },
    { MVFMT_RGB565 + MVFMT_ARGB1555, RGB565_to_RGB555 },

    { MVFMT_RGB8 + MVFMT_RGBA, RGB8_to_RGB32 },
    { MVFMT_RGB8 + MVFMT_RGB32, RGB8_to_RGB32 },
    { MVFMT_RGB8 + MVFMT_RGB24, RGB8_to_RGB24 },
    { MVFMT_RGB8 + MVFMT_RGB565, RGB8_to_RGB565 },
    { MVFMT_RGB8 + MVFMT_RGB555, RGB8_to_RGB555 },
    { MVFMT_RGB8 + MVFMT_ARGB1555, RGB8_to_RGB555 },

    { MVFMT_RGB4 + MVFMT_RGBA, RGB4_to_RGB32 },
    { MVFMT_RGB4 + MVFMT_RGB32, RGB4_to_RGB32 },
    { MVFMT_RGB4 + MVFMT_RGB24, RGB4_to_RGB24 },
    { MVFMT_RGB4 + MVFMT_RGB565, RGB4_to_RGB565 },
    { MVFMT_RGB4 + MVFMT_RGB555, RGB4_to_RGB555 },
    { MVFMT_RGB4 + MVFMT_ARGB1555, RGB4_to_RGB555 },

    { MVFMT_RGB1 + MVFMT_RGBA, RGB1_to_RGB32 },
    { MVFMT_RGB1 + MVFMT_RGB32, RGB1_to_RGB32 },
    { MVFMT_RGB1 + MVFMT_RGB24, RGB1_to_RGB24 },
    { MVFMT_RGB1 + MVFMT_RGB565, RGB1_to_RGB565 },
    { MVFMT_RGB1 + MVFMT_RGB555, RGB1_to_RGB555 },
    { MVFMT_RGB1 + MVFMT_ARGB1555, RGB1_to_RGB555 },

    // YUV to RGB

    { MVFMT_YUY2 + MVFMT_RGBA, PackedY422_to_RGB32 },
    { MVFMT_YUY2 + MVFMT_RGB32, PackedY422_to_RGB32 },
    { MVFMT_YUY2 + MVFMT_RGB24, PackedY422_to_RGB24 },
    { MVFMT_YUY2 + MVFMT_RGB565, PackedY422_to_RGB565 },
    { MVFMT_YUY2 + MVFMT_RGB555, PackedY422_to_RGB555 },
    { MVFMT_YUY2 + MVFMT_ARGB1555, PackedY422_to_RGB555 },

    { MVFMT_UYVY + MVFMT_RGBA, PackedY422_to_RGB32 },
    { MVFMT_UYVY + MVFMT_RGB32, PackedY422_to_RGB32 },
    { MVFMT_UYVY + MVFMT_RGB24, PackedY422_to_RGB24 },
    { MVFMT_UYVY + MVFMT_RGB565, PackedY422_to_RGB565 },
    { MVFMT_UYVY + MVFMT_RGB555, PackedY422_to_RGB555 },
    { MVFMT_UYVY + MVFMT_ARGB1555, PackedY422_to_RGB555 },

    { MVFMT_YVYU + MVFMT_RGBA, PackedY422_to_RGB32 },
    { MVFMT_YVYU + MVFMT_RGB32, PackedY422_to_RGB32 },
    { MVFMT_YVYU + MVFMT_RGB24, PackedY422_to_RGB24 },
    { MVFMT_YVYU + MVFMT_RGB565, PackedY422_to_RGB565 },
    { MVFMT_YVYU + MVFMT_RGB555, PackedY422_to_RGB555 },
    { MVFMT_YVYU + MVFMT_ARGB1555, PackedY422_to_RGB555 },

    { MVFMT_VYUY + MVFMT_RGBA, PackedY422_to_RGB32 },
    { MVFMT_VYUY + MVFMT_RGB32, PackedY422_to_RGB32 },
    { MVFMT_VYUY + MVFMT_RGB24, PackedY422_to_RGB24 },
    { MVFMT_VYUY + MVFMT_RGB565, PackedY422_to_RGB565 },
    { MVFMT_VYUY + MVFMT_RGB555, PackedY422_to_RGB555 },
    { MVFMT_VYUY + MVFMT_ARGB1555, PackedY422_to_RGB555 },

    { MVFMT_I420 + MVFMT_RGBA, PlanarYUV_to_RGB32 },
    { MVFMT_I420 + MVFMT_RGB32, PlanarYUV_to_RGB32 },
    { MVFMT_I420 + MVFMT_RGB24, PlanarYUV_to_RGB24 },
    { MVFMT_I420 + MVFMT_RGB565, PlanarYUV_to_RGB565 },
    { MVFMT_I420 + MVFMT_RGB555, PlanarYUV_to_RGB555 },
    { MVFMT_I420 + MVFMT_ARGB1555, PlanarYUV_to_RGB555 },

    { MVFMT_YV12 + MVFMT_RGBA, PlanarYUV_to_RGB32 },
    { MVFMT_YV12 + MVFMT_RGB32, PlanarYUV_to_RGB32 },
    { MVFMT_YV12 + MVFMT_RGB24, PlanarYUV_to_RGB24 },
    { MVFMT_YV12 + MVFMT_RGB565, PlanarYUV_to_RGB565 },
    { MVFMT_YV12 + MVFMT_RGB555, PlanarYUV_to_RGB555 },
    { MVFMT_YV12 + MVFMT_ARGB1555, PlanarYUV_to_RGB555 },

    { MVFMT_YVU9 + MVFMT_RGBA, PlanarYUV_to_RGB32 },
    { MVFMT_YVU9 + MVFMT_RGB32, PlanarYUV_to_RGB32 },
    { MVFMT_YVU9 + MVFMT_RGB24, PlanarYUV_to_RGB24 },
    { MVFMT_YVU9 + MVFMT_RGB565, PlanarYUV_to_RGB565 },
    { MVFMT_YVU9 + MVFMT_RGB555, PlanarYUV_to_RGB555 },
    { MVFMT_YVU9 + MVFMT_ARGB1555, PlanarYUV_to_RGB555 },

    { MVFMT_YUV9 + MVFMT_RGBA, PlanarYUV_to_RGB32 },
    { MVFMT_YUV9 + MVFMT_RGB32, PlanarYUV_to_RGB32 },
    { MVFMT_YUV9 + MVFMT_RGB24, PlanarYUV_to_RGB24 },
    { MVFMT_YUV9 + MVFMT_RGB565, PlanarYUV_to_RGB565 },
    { MVFMT_YUV9 + MVFMT_RGB555, PlanarYUV_to_RGB555 },
    { MVFMT_YUV9 + MVFMT_ARGB1555, PlanarYUV_to_RGB555 },

    { MVFMT_IYU1 + MVFMT_RGBA, IYU1_to_RGB32 },
    { MVFMT_IYU1 + MVFMT_RGB32, IYU1_to_RGB32 },
    { MVFMT_IYU1 + MVFMT_RGB24, IYU1_to_RGB24 },
    { MVFMT_IYU1 + MVFMT_RGB565, IYU1_to_RGB565 },
    { MVFMT_IYU1 + MVFMT_RGB555, IYU1_to_RGB555 },
    { MVFMT_IYU1 + MVFMT_ARGB1555, IYU1_to_RGB555 },

    { MVFMT_IYU2 + MVFMT_RGBA, IYU2_to_RGB32 },
    { MVFMT_IYU2 + MVFMT_RGB32, IYU2_to_RGB32 },
    { MVFMT_IYU2 + MVFMT_RGB24, IYU2_to_RGB24 },
    { MVFMT_IYU2 + MVFMT_RGB565, IYU2_to_RGB565 },
    { MVFMT_IYU2 + MVFMT_RGB555, IYU2_to_RGB555 },
    { MVFMT_IYU2 + MVFMT_ARGB1555, IYU2_to_RGB555 },

    { MVFMT_CLJR + MVFMT_RGBA, CLJR_to_RGB32 },
    { MVFMT_CLJR + MVFMT_RGB32, CLJR_to_RGB32 },
    { MVFMT_CLJR + MVFMT_RGB24, CLJR_to_RGB24 },
    { MVFMT_CLJR + MVFMT_RGB565, CLJR_to_RGB565 },
    { MVFMT_CLJR + MVFMT_RGB555, CLJR_to_RGB555 },
    { MVFMT_CLJR + MVFMT_ARGB1555, CLJR_to_RGB555 },

    { MVFMT_Y41P + MVFMT_RGBA, Y41P_to_RGB32 },
    { MVFMT_Y41P + MVFMT_RGB32, Y41P_to_RGB32 },
    { MVFMT_Y41P + MVFMT_RGB24, Y41P_to_RGB24 },
    { MVFMT_Y41P + MVFMT_RGB565, Y41P_to_RGB565 },
    { MVFMT_Y41P + MVFMT_RGB555, Y41P_to_RGB555 },
    { MVFMT_Y41P + MVFMT_ARGB1555, Y41P_to_RGB555 },

    { MVFMT_IMC1 + MVFMT_RGBA, IMCx_to_RGB32 },
    { MVFMT_IMC1 + MVFMT_RGB32, IMCx_to_RGB32 },
    { MVFMT_IMC1 + MVFMT_RGB24, IMCx_to_RGB24 },
    { MVFMT_IMC1 + MVFMT_RGB565, IMCx_to_RGB565 },
    { MVFMT_IMC1 + MVFMT_RGB555, IMCx_to_RGB555 },
    { MVFMT_IMC1 + MVFMT_ARGB1555, IMCx_to_RGB555 },

    { MVFMT_IMC2 + MVFMT_RGBA, IMCx_to_RGB32 },
    { MVFMT_IMC2 + MVFMT_RGB32, IMCx_to_RGB32 },
    { MVFMT_IMC2 + MVFMT_RGB24, IMCx_to_RGB24 },
    { MVFMT_IMC2 + MVFMT_RGB565, IMCx_to_RGB565 },
    { MVFMT_IMC2 + MVFMT_RGB555, IMCx_to_RGB555 },
    { MVFMT_IMC2 + MVFMT_ARGB1555, IMCx_to_RGB555 },

    { MVFMT_IMC3 + MVFMT_RGBA, IMCx_to_RGB32 },
    { MVFMT_IMC3 + MVFMT_RGB32, IMCx_to_RGB32 },
    { MVFMT_IMC3 + MVFMT_RGB24, IMCx_to_RGB24 },
    { MVFMT_IMC3 + MVFMT_RGB565, IMCx_to_RGB565 },
    { MVFMT_IMC3 + MVFMT_RGB555, IMCx_to_RGB555 },
    { MVFMT_IMC3 + MVFMT_ARGB1555, IMCx_to_RGB555 },

    { MVFMT_IMC4 + MVFMT_RGBA, IMCx_to_RGB32 },
    { MVFMT_IMC4 + MVFMT_RGB32, IMCx_to_RGB32 },
    { MVFMT_IMC4 + MVFMT_RGB24, IMCx_to_RGB24 },
    { MVFMT_IMC4 + MVFMT_RGB565, IMCx_to_RGB565 },
    { MVFMT_IMC4 + MVFMT_RGB555, IMCx_to_RGB555 },
    { MVFMT_IMC4 + MVFMT_ARGB1555, IMCx_to_RGB555 },

    { MVFMT_Y800 + MVFMT_RGBA, Y800_to_RGB32 },
    { MVFMT_Y800 + MVFMT_RGB32, Y800_to_RGB32 },
    { MVFMT_Y800 + MVFMT_RGB24, Y800_to_RGB24 },
    { MVFMT_Y800 + MVFMT_RGB565, Y800_to_RGB565 },
    { MVFMT_Y800 + MVFMT_RGB555, Y800_to_RGB555 },
    { MVFMT_Y800 + MVFMT_ARGB1555, Y800_to_RGB555 },

    { MVFMT_Y16 + MVFMT_RGBA, Y16_to_RGB32 },
    { MVFMT_Y16 + MVFMT_RGB32, Y16_to_RGB32 },
    { MVFMT_Y16 + MVFMT_RGB24, Y16_to_RGB24 },
    { MVFMT_Y16 + MVFMT_RGB565, Y16_to_RGB565 },
    { MVFMT_Y16 + MVFMT_RGB555, Y16_to_RGB555 },
    { MVFMT_Y16 + MVFMT_ARGB1555, Y16_to_RGB555 },

    { MVFMT_AYUV + MVFMT_RGBA, AYUV_to_RGBA },
    { MVFMT_AYUV + MVFMT_RGB32, AYUV_to_RGB32 },
    { MVFMT_AYUV + MVFMT_RGB24, AYUV_to_RGB24 },
    { MVFMT_AYUV + MVFMT_RGB565, AYUV_to_RGB565 },
    { MVFMT_AYUV + MVFMT_RGB555, AYUV_to_RGB555 },
    { MVFMT_AYUV + MVFMT_ARGB1555, AYUV_to_ARGB1555 },

    { MVFMT_NV12 + MVFMT_RGBA, NVx_to_RGB32 },
    { MVFMT_NV12 + MVFMT_RGB32, NVx_to_RGB32 },
    { MVFMT_NV12 + MVFMT_RGB24, NVx_to_RGB24 },
    { MVFMT_NV12 + MVFMT_RGB565, NVx_to_RGB565 },
    { MVFMT_NV12 + MVFMT_RGB555, NVx_to_RGB555 },
    { MVFMT_NV12 + MVFMT_ARGB1555, NVx_to_RGB555 },

    { MVFMT_NV21 + MVFMT_RGBA, NVx_to_RGB32 },
    { MVFMT_NV21 + MVFMT_RGB32, NVx_to_RGB32 },
    { MVFMT_NV21 + MVFMT_RGB24, NVx_to_RGB24 },
    { MVFMT_NV21 + MVFMT_RGB565, NVx_to_RGB565 },
    { MVFMT_NV21 + MVFMT_RGB555, NVx_to_RGB555 },
    { MVFMT_NV21 + MVFMT_ARGB1555, NVx_to_RGB555 },

    { MVFMT_Y42T + MVFMT_RGBA, Y42T_to_RGBA },
    { MVFMT_Y42T + MVFMT_RGB32, Y42T_to_RGB32 },
    { MVFMT_Y42T + MVFMT_RGB24, Y42T_to_RGB24 },
    { MVFMT_Y42T + MVFMT_RGB565, Y42T_to_RGB565 },
    { MVFMT_Y42T + MVFMT_RGB555, Y42T_to_RGB555 },
    { MVFMT_Y42T + MVFMT_ARGB1555, Y42T_to_ARGB1555 },

    { MVFMT_Y41T + MVFMT_RGBA, Y41T_to_RGBA },
    { MVFMT_Y41T + MVFMT_RGB32, Y41T_to_RGB32 },
    { MVFMT_Y41T + MVFMT_RGB24, Y41T_to_RGB24 },
    { MVFMT_Y41T + MVFMT_RGB565, Y41T_to_RGB565 },
    { MVFMT_Y41T + MVFMT_RGB555, Y41T_to_RGB555 },
    { MVFMT_Y41T + MVFMT_ARGB1555, Y41T_to_ARGB1555 },

    { MVFMT_YV16 + MVFMT_RGBA, YV16_to_RGB32 },
    { MVFMT_YV16 + MVFMT_RGB32, YV16_to_RGB32 },
    { MVFMT_YV16 + MVFMT_RGB24, YV16_to_RGB24 },
    { MVFMT_YV16 + MVFMT_RGB565, YV16_to_RGB565 },
    { MVFMT_YV16 + MVFMT_RGB555, YV16_to_RGB555 },
    { MVFMT_YV16 + MVFMT_ARGB1555, YV16_to_RGB555 },

    // RGB to YUV

    { MVFMT_RGBA + MVFMT_AYUV, RGBA_to_AYUV },
    { MVFMT_RGBA + MVFMT_YUY2, RGB32_to_PackedY422 },
    { MVFMT_RGBA + MVFMT_UYVY, RGB32_to_PackedY422 },
    { MVFMT_RGBA + MVFMT_YVYU, RGB32_to_PackedY422 },
    { MVFMT_RGBA + MVFMT_VYUY, RGB32_to_PackedY422 },
    { MVFMT_RGBA + MVFMT_I420, RGB32_to_PlanarYUV },
    { MVFMT_RGBA + MVFMT_YV12, RGB32_to_PlanarYUV },
    { MVFMT_RGBA + MVFMT_YVU9, RGB32_to_PlanarYUV },
    { MVFMT_RGBA + MVFMT_YUV9, RGB32_to_PlanarYUV },
    { MVFMT_RGBA + MVFMT_IYU1, RGB32_to_IYU1 },
    { MVFMT_RGBA + MVFMT_IYU2, RGB32_to_IYU2 },
    { MVFMT_RGBA + MVFMT_Y41P, RGB32_to_Y41P },
    { MVFMT_RGBA + MVFMT_CLJR, RGB32_to_CLJR },
    { MVFMT_RGBA + MVFMT_Y800, RGB32_to_Y800 },
    { MVFMT_RGBA + MVFMT_Y16, RGB32_to_Y16 },
    { MVFMT_RGBA + MVFMT_AYUV, RGBA_to_AYUV },
    { MVFMT_RGBA + MVFMT_IMC1, RGB32_to_IMCx },
    { MVFMT_RGBA + MVFMT_IMC2, RGB32_to_IMCx },
    { MVFMT_RGBA + MVFMT_IMC3, RGB32_to_IMCx },
    { MVFMT_RGBA + MVFMT_IMC4, RGB32_to_IMCx },
    { MVFMT_RGBA + MVFMT_NV12, RGB32_to_NVx },
    { MVFMT_RGBA + MVFMT_NV21, RGB32_to_NVx },
    { MVFMT_RGBA + MVFMT_Y42T, RGBA_to_Y42T },
    { MVFMT_RGBA + MVFMT_Y41T, RGBA_to_Y41T },
    { MVFMT_RGBA + MVFMT_YV16, RGB32_to_YV16 },

    { MVFMT_RGB32 + MVFMT_YUY2, RGB32_to_PackedY422 },
    { MVFMT_RGB32 + MVFMT_UYVY, RGB32_to_PackedY422 },
    { MVFMT_RGB32 + MVFMT_YVYU, RGB32_to_PackedY422 },
    { MVFMT_RGB32 + MVFMT_VYUY, RGB32_to_PackedY422 },
    { MVFMT_RGB32 + MVFMT_I420, RGB32_to_PlanarYUV },
    { MVFMT_RGB32 + MVFMT_YV12, RGB32_to_PlanarYUV },
    { MVFMT_RGB32 + MVFMT_YVU9, RGB32_to_PlanarYUV },
    { MVFMT_RGB32 + MVFMT_YUV9, RGB32_to_PlanarYUV },
    { MVFMT_RGB32 + MVFMT_IYU1, RGB32_to_IYU1 },
    { MVFMT_RGB32 + MVFMT_IYU2, RGB32_to_IYU2 },
    { MVFMT_RGB32 + MVFMT_Y41P, RGB32_to_Y41P },
    { MVFMT_RGB32 + MVFMT_CLJR, RGB32_to_CLJR },
    { MVFMT_RGB32 + MVFMT_Y800, RGB32_to_Y800 },
    { MVFMT_RGB32 + MVFMT_Y16, RGB32_to_Y16 },
    { MVFMT_RGB32 + MVFMT_AYUV, RGB32_to_AYUV },
    { MVFMT_RGB32 + MVFMT_IMC1, RGB32_to_IMCx },
    { MVFMT_RGB32 + MVFMT_IMC2, RGB32_to_IMCx },
    { MVFMT_RGB32 + MVFMT_IMC3, RGB32_to_IMCx },
    { MVFMT_RGB32 + MVFMT_IMC4, RGB32_to_IMCx },
    { MVFMT_RGB32 + MVFMT_NV12, RGB32_to_NVx },
    { MVFMT_RGB32 + MVFMT_NV21, RGB32_to_NVx },
    { MVFMT_RGB32 + MVFMT_Y42T, RGB32_to_Y42T },
    { MVFMT_RGB32 + MVFMT_Y41T, RGB32_to_Y41T },
    { MVFMT_RGB32 + MVFMT_YV16, RGB32_to_YV16 },

    { MVFMT_RGB24 + MVFMT_YUY2, RGB24_to_PackedY422 },
    { MVFMT_RGB24 + MVFMT_UYVY, RGB24_to_PackedY422 },
    { MVFMT_RGB24 + MVFMT_YVYU, RGB24_to_PackedY422 },
    { MVFMT_RGB24 + MVFMT_VYUY, RGB24_to_PackedY422 },
    { MVFMT_RGB24 + MVFMT_I420, RGB24_to_PlanarYUV },
    { MVFMT_RGB24 + MVFMT_YV12, RGB24_to_PlanarYUV },
    { MVFMT_RGB24 + MVFMT_YVU9, RGB24_to_PlanarYUV },
    { MVFMT_RGB24 + MVFMT_YUV9, RGB24_to_PlanarYUV },
    { MVFMT_RGB24 + MVFMT_IYU1, RGB24_to_IYU1 },
    { MVFMT_RGB24 + MVFMT_IYU2, RGB24_to_IYU2 },
    { MVFMT_RGB24 + MVFMT_Y41P, RGB24_to_Y41P },
    { MVFMT_RGB24 + MVFMT_CLJR, RGB24_to_CLJR },
    { MVFMT_RGB24 + MVFMT_Y800, RGB24_to_Y800 },
    { MVFMT_RGB24 + MVFMT_Y16, RGB24_to_Y16 },
    { MVFMT_RGB24 + MVFMT_AYUV, RGB24_to_AYUV },
    { MVFMT_RGB24 + MVFMT_IMC1, RGB24_to_IMCx },
    { MVFMT_RGB24 + MVFMT_IMC2, RGB24_to_IMCx },
    { MVFMT_RGB24 + MVFMT_IMC3, RGB24_to_IMCx },
    { MVFMT_RGB24 + MVFMT_IMC4, RGB24_to_IMCx },
    { MVFMT_RGB24 + MVFMT_NV12, RGB24_to_NVx },
    { MVFMT_RGB24 + MVFMT_NV21, RGB24_to_NVx },
    { MVFMT_RGB24 + MVFMT_Y42T, RGB24_to_Y42T },
    { MVFMT_RGB24 + MVFMT_Y41T, RGB24_to_Y41T },
    { MVFMT_RGB24 + MVFMT_YV16, RGB24_to_YV16 },

    { MVFMT_RGB565 + MVFMT_YUY2, RGB565_to_PackedY422 },
    { MVFMT_RGB565 + MVFMT_UYVY, RGB565_to_PackedY422 },
    { MVFMT_RGB565 + MVFMT_YVYU, RGB565_to_PackedY422 },
    { MVFMT_RGB565 + MVFMT_VYUY, RGB565_to_PackedY422 },
    { MVFMT_RGB565 + MVFMT_I420, RGB565_to_PlanarYUV },
    { MVFMT_RGB565 + MVFMT_YV12, RGB565_to_PlanarYUV },
    { MVFMT_RGB565 + MVFMT_YVU9, RGB565_to_PlanarYUV },
    { MVFMT_RGB565 + MVFMT_YUV9, RGB565_to_PlanarYUV },
    { MVFMT_RGB565 + MVFMT_IYU1, RGB565_to_IYU1 },
    { MVFMT_RGB565 + MVFMT_IYU2, RGB565_to_IYU2 },
    { MVFMT_RGB565 + MVFMT_Y41P, RGB565_to_Y41P },
    { MVFMT_RGB565 + MVFMT_CLJR, RGB565_to_CLJR },
    { MVFMT_RGB565 + MVFMT_Y800, RGB565_to_Y800 },
    { MVFMT_RGB565 + MVFMT_Y16, RGB565_to_Y16 },
    { MVFMT_RGB565 + MVFMT_AYUV, RGB565_to_AYUV },
    { MVFMT_RGB565 + MVFMT_IMC1, RGB565_to_IMCx },
    { MVFMT_RGB565 + MVFMT_IMC2, RGB565_to_IMCx },
    { MVFMT_RGB565 + MVFMT_IMC3, RGB565_to_IMCx },
    { MVFMT_RGB565 + MVFMT_IMC4, RGB565_to_IMCx },
    { MVFMT_RGB565 + MVFMT_NV12, RGB565_to_NVx },
    { MVFMT_RGB565 + MVFMT_NV21, RGB565_to_NVx },
    { MVFMT_RGB565 + MVFMT_Y42T, RGB565_to_Y42T },
    { MVFMT_RGB565 + MVFMT_Y41T, RGB565_to_Y41T },
    { MVFMT_RGB565 + MVFMT_YV16, RGB565_to_YV16 },

    { MVFMT_RGB555 + MVFMT_YUY2, RGB555_to_PackedY422 },
    { MVFMT_RGB555 + MVFMT_UYVY, RGB555_to_PackedY422 },
    { MVFMT_RGB555 + MVFMT_YVYU, RGB555_to_PackedY422 },
    { MVFMT_RGB555 + MVFMT_VYUY, RGB555_to_PackedY422 },
    { MVFMT_RGB555 + MVFMT_I420, RGB555_to_PlanarYUV },
    { MVFMT_RGB555 + MVFMT_YV12, RGB555_to_PlanarYUV },
    { MVFMT_RGB555 + MVFMT_YVU9, RGB555_to_PlanarYUV },
    { MVFMT_RGB555 + MVFMT_YUV9, RGB555_to_PlanarYUV },
    { MVFMT_RGB555 + MVFMT_IYU1, RGB555_to_IYU1 },
    { MVFMT_RGB555 + MVFMT_IYU2, RGB555_to_IYU2 },
    { MVFMT_RGB555 + MVFMT_Y41P, RGB555_to_Y41P },
    { MVFMT_RGB555 + MVFMT_CLJR, RGB555_to_CLJR },
    { MVFMT_RGB555 + MVFMT_Y800, RGB555_to_Y800 },
    { MVFMT_RGB555 + MVFMT_Y16, RGB555_to_Y16 },
    { MVFMT_RGB555 + MVFMT_AYUV, RGB555_to_AYUV },
    { MVFMT_RGB555 + MVFMT_IMC1, RGB555_to_IMCx },
    { MVFMT_RGB555 + MVFMT_IMC2, RGB555_to_IMCx },
    { MVFMT_RGB555 + MVFMT_IMC3, RGB555_to_IMCx },
    { MVFMT_RGB555 + MVFMT_IMC4, RGB555_to_IMCx },
    { MVFMT_RGB555 + MVFMT_NV12, RGB555_to_NVx },
    { MVFMT_RGB555 + MVFMT_NV21, RGB555_to_NVx },
    { MVFMT_RGB555 + MVFMT_Y42T, RGB555_to_Y42T },
    { MVFMT_RGB555 + MVFMT_Y41T, RGB555_to_Y41T },
    { MVFMT_RGB555 + MVFMT_YV16, RGB555_to_YV16 },

    { MVFMT_ARGB1555 + MVFMT_YUY2, RGB555_to_PackedY422 },
    { MVFMT_ARGB1555 + MVFMT_UYVY, RGB555_to_PackedY422 },
    { MVFMT_ARGB1555 + MVFMT_YVYU, RGB555_to_PackedY422 },
    { MVFMT_ARGB1555 + MVFMT_VYUY, RGB555_to_PackedY422 },
    { MVFMT_ARGB1555 + MVFMT_I420, RGB555_to_PlanarYUV },
    { MVFMT_ARGB1555 + MVFMT_YV12, RGB555_to_PlanarYUV },
    { MVFMT_ARGB1555 + MVFMT_YVU9, RGB555_to_PlanarYUV },
    { MVFMT_ARGB1555 + MVFMT_YUV9, RGB555_to_PlanarYUV },
    { MVFMT_ARGB1555 + MVFMT_IYU1, RGB555_to_IYU1 },
    { MVFMT_ARGB1555 + MVFMT_IYU2, RGB555_to_IYU2 },
    { MVFMT_ARGB1555 + MVFMT_Y41P, RGB555_to_Y41P },
    { MVFMT_ARGB1555 + MVFMT_CLJR, RGB555_to_CLJR },
    { MVFMT_ARGB1555 + MVFMT_Y800, RGB555_to_Y800 },
    { MVFMT_ARGB1555 + MVFMT_Y16, RGB555_to_Y16 },
    { MVFMT_ARGB1555 + MVFMT_AYUV, ARGB1555_to_AYUV },
    { MVFMT_ARGB1555 + MVFMT_IMC1, RGB555_to_IMCx },
    { MVFMT_ARGB1555 + MVFMT_IMC2, RGB555_to_IMCx },
    { MVFMT_ARGB1555 + MVFMT_IMC3, RGB555_to_IMCx },
    { MVFMT_ARGB1555 + MVFMT_IMC4, RGB555_to_IMCx },
    { MVFMT_ARGB1555 + MVFMT_NV12, RGB555_to_NVx },
    { MVFMT_ARGB1555 + MVFMT_NV21, RGB555_to_NVx },
    { MVFMT_ARGB1555 + MVFMT_Y42T, ARGB1555_to_Y42T },
    { MVFMT_ARGB1555 + MVFMT_Y41T, ARGB1555_to_Y41T },
    { MVFMT_ARGB1555 + MVFMT_YV16, RGB555_to_YV16 },

    { MVFMT_RGB8 + MVFMT_YUY2, RGB8_to_PackedY422 },
    { MVFMT_RGB8 + MVFMT_UYVY, RGB8_to_PackedY422 },
    { MVFMT_RGB8 + MVFMT_YVYU, RGB8_to_PackedY422 },
    { MVFMT_RGB8 + MVFMT_VYUY, RGB8_to_PackedY422 },
    { MVFMT_RGB8 + MVFMT_I420, RGB8_to_PlanarYUV },
    { MVFMT_RGB8 + MVFMT_YV12, RGB8_to_PlanarYUV },
    { MVFMT_RGB8 + MVFMT_YVU9, RGB8_to_PlanarYUV },
    { MVFMT_RGB8 + MVFMT_YUV9, RGB8_to_PlanarYUV },
    { MVFMT_RGB8 + MVFMT_IYU1, RGB8_to_IYU1 },
    { MVFMT_RGB8 + MVFMT_IYU2, RGB8_to_IYU2 },
    { MVFMT_RGB8 + MVFMT_Y41P, RGB8_to_Y41P },
    { MVFMT_RGB8 + MVFMT_CLJR, RGB8_to_CLJR },
    { MVFMT_RGB8 + MVFMT_Y800, RGB8_to_Y800 },
    { MVFMT_RGB8 + MVFMT_Y16, RGB8_to_Y16 },
    { MVFMT_RGB8 + MVFMT_AYUV, RGB8_to_AYUV },
    { MVFMT_RGB8 + MVFMT_IMC1, RGB8_to_IMCx },
    { MVFMT_RGB8 + MVFMT_IMC2, RGB8_to_IMCx },
    { MVFMT_RGB8 + MVFMT_IMC3, RGB8_to_IMCx },
    { MVFMT_RGB8 + MVFMT_IMC4, RGB8_to_IMCx },
    { MVFMT_RGB8 + MVFMT_NV12, RGB8_to_NVx },
    { MVFMT_RGB8 + MVFMT_NV21, RGB8_to_NVx },
    { MVFMT_RGB8 + MVFMT_Y42T, RGB8_to_Y42T },
    { MVFMT_RGB8 + MVFMT_Y41T, RGB8_to_Y41T },
    { MVFMT_RGB8 + MVFMT_YV16, RGB8_to_YV16 },

    // YUV to YUV

    { MVFMT_AYUV + MVFMT_YUY2, AYUV_to_PackedY422 },
    { MVFMT_AYUV + MVFMT_UYVY, AYUV_to_PackedY422 },
    { MVFMT_AYUV + MVFMT_YVYU, AYUV_to_PackedY422 },
    { MVFMT_AYUV + MVFMT_VYUY, AYUV_to_PackedY422 },
    { MVFMT_AYUV + MVFMT_I420, AYUV_to_PlanarYUV },
    { MVFMT_AYUV + MVFMT_YV12, AYUV_to_PlanarYUV },
    { MVFMT_AYUV + MVFMT_YVU9, AYUV_to_PlanarYUV },
    { MVFMT_AYUV + MVFMT_YUV9, AYUV_to_PlanarYUV },
    { MVFMT_AYUV + MVFMT_IYU1, AYUV_to_IYU1 },
    { MVFMT_AYUV + MVFMT_IYU2, AYUV_to_IYU2 },
    { MVFMT_AYUV + MVFMT_Y800, AYUV_to_Y800 },
    { MVFMT_AYUV + MVFMT_Y16, AYUV_to_Y16 },
    { MVFMT_AYUV + MVFMT_CLJR, AYUV_to_CLJR },
    { MVFMT_AYUV + MVFMT_Y41P, AYUV_to_Y41P },
    { MVFMT_AYUV + MVFMT_IMC1, AYUV_to_IMCx },
    { MVFMT_AYUV + MVFMT_IMC2, AYUV_to_IMCx },
    { MVFMT_AYUV + MVFMT_IMC3, AYUV_to_IMCx },
    { MVFMT_AYUV + MVFMT_IMC4, AYUV_to_IMCx },
    { MVFMT_AYUV + MVFMT_NV12, AYUV_to_NVx },
    { MVFMT_AYUV + MVFMT_NV21, AYUV_to_NVx },
    { MVFMT_AYUV + MVFMT_Y42T, AYUV_to_Y42T },
    { MVFMT_AYUV + MVFMT_Y41T, AYUV_to_Y41T },
    { MVFMT_AYUV + MVFMT_YV16, AYUV_to_YV16 },

    { MVFMT_YUY2 + MVFMT_UYVY, PackedY422_to_PackedY422 },
    { MVFMT_YUY2 + MVFMT_YVYU, PackedY422_to_PackedY422 },
    { MVFMT_YUY2 + MVFMT_VYUY, PackedY422_to_PackedY422 },
    { MVFMT_YUY2 + MVFMT_I420, PackedY422_to_PlanarYUV },
    { MVFMT_YUY2 + MVFMT_YV12, PackedY422_to_PlanarYUV },
    { MVFMT_YUY2 + MVFMT_YVU9, PackedY422_to_PlanarYUV },
    { MVFMT_YUY2 + MVFMT_YUV9, PackedY422_to_PlanarYUV },
    { MVFMT_YUY2 + MVFMT_IYU1, PackedY422_to_IYU1 },
    { MVFMT_YUY2 + MVFMT_IYU2, PackedY422_to_IYU2 },
    { MVFMT_YUY2 + MVFMT_Y800, PackedY422_to_Y800 },
    { MVFMT_YUY2 + MVFMT_Y16, PackedY422_to_Y16 },
    { MVFMT_YUY2 + MVFMT_CLJR, PackedY422_to_CLJR },
    { MVFMT_YUY2 + MVFMT_Y41P, PackedY422_to_Y41P },
    { MVFMT_YUY2 + MVFMT_AYUV, PackedY422_to_AYUV },
    { MVFMT_YUY2 + MVFMT_IMC1, PackedY422_to_IMCx },
    { MVFMT_YUY2 + MVFMT_IMC2, PackedY422_to_IMCx },
    { MVFMT_YUY2 + MVFMT_IMC3, PackedY422_to_IMCx },
    { MVFMT_YUY2 + MVFMT_IMC4, PackedY422_to_IMCx },
    { MVFMT_YUY2 + MVFMT_NV12, PackedY422_to_NVx },
    { MVFMT_YUY2 + MVFMT_NV21, PackedY422_to_NVx },
    { MVFMT_YUY2 + MVFMT_Y42T, PackedY422_to_Y42T },
    { MVFMT_YUY2 + MVFMT_Y41T, PackedY422_to_Y41T },
    { MVFMT_YUY2 + MVFMT_YV16, PackedY422_to_YV16 },

    { MVFMT_UYVY + MVFMT_YUY2, PackedY422_to_PackedY422 },
    { MVFMT_UYVY + MVFMT_YVYU, PackedY422_to_PackedY422 },
    { MVFMT_UYVY + MVFMT_VYUY, PackedY422_to_PackedY422 },
    { MVFMT_UYVY + MVFMT_I420, PackedY422_to_PlanarYUV },
    { MVFMT_UYVY + MVFMT_YV12, PackedY422_to_PlanarYUV },
    { MVFMT_UYVY + MVFMT_YVU9, PackedY422_to_PlanarYUV },
    { MVFMT_UYVY + MVFMT_YUV9, PackedY422_to_PlanarYUV },
    { MVFMT_UYVY + MVFMT_IYU1, PackedY422_to_IYU1 },
    { MVFMT_UYVY + MVFMT_IYU2, PackedY422_to_IYU2 },
    { MVFMT_UYVY + MVFMT_Y800, PackedY422_to_Y800 },
    { MVFMT_UYVY + MVFMT_Y16, PackedY422_to_Y16 },
    { MVFMT_UYVY + MVFMT_CLJR, PackedY422_to_CLJR },
    { MVFMT_UYVY + MVFMT_Y41P, PackedY422_to_Y41P },
    { MVFMT_UYVY + MVFMT_IUYV, UYVY_to_IUYV },
    { MVFMT_UYVY + MVFMT_AYUV, PackedY422_to_AYUV },
    { MVFMT_UYVY + MVFMT_IMC1, PackedY422_to_IMCx },
    { MVFMT_UYVY + MVFMT_IMC2, PackedY422_to_IMCx },
    { MVFMT_UYVY + MVFMT_IMC3, PackedY422_to_IMCx },
    { MVFMT_UYVY + MVFMT_IMC4, PackedY422_to_IMCx },
    { MVFMT_UYVY + MVFMT_NV12, PackedY422_to_NVx },
    { MVFMT_UYVY + MVFMT_NV21, PackedY422_to_NVx },
    { MVFMT_UYVY + MVFMT_Y42T, PackedY422_to_Y42T },
    { MVFMT_UYVY + MVFMT_Y41T, PackedY422_to_Y41T },
    { MVFMT_UYVY + MVFMT_YV16, PackedY422_to_YV16 },

    { MVFMT_YVYU + MVFMT_YUY2, PackedY422_to_PackedY422 },
    { MVFMT_YVYU + MVFMT_UYVY, PackedY422_to_PackedY422 },
    { MVFMT_YVYU + MVFMT_VYUY, PackedY422_to_PackedY422 },
    { MVFMT_YVYU + MVFMT_I420, PackedY422_to_PlanarYUV },
    { MVFMT_YVYU + MVFMT_YV12, PackedY422_to_PlanarYUV },
    { MVFMT_YVYU + MVFMT_YVU9, PackedY422_to_PlanarYUV },
    { MVFMT_YVYU + MVFMT_YUV9, PackedY422_to_PlanarYUV },
    { MVFMT_YVYU + MVFMT_IYU1, PackedY422_to_IYU1 },
    { MVFMT_YVYU + MVFMT_IYU2, PackedY422_to_IYU2 },
    { MVFMT_YVYU + MVFMT_Y800, PackedY422_to_Y800 },
    { MVFMT_YVYU + MVFMT_Y16, PackedY422_to_Y16 },
    { MVFMT_YVYU + MVFMT_CLJR, PackedY422_to_CLJR },
    { MVFMT_YVYU + MVFMT_Y41P, PackedY422_to_Y41P },
    { MVFMT_YVYU + MVFMT_AYUV, PackedY422_to_AYUV },
    { MVFMT_YVYU + MVFMT_IMC1, PackedY422_to_IMCx },
    { MVFMT_YVYU + MVFMT_IMC2, PackedY422_to_IMCx },
    { MVFMT_YVYU + MVFMT_IMC3, PackedY422_to_IMCx },
    { MVFMT_YVYU + MVFMT_IMC4, PackedY422_to_IMCx },
    { MVFMT_YVYU + MVFMT_NV12, PackedY422_to_NVx },
    { MVFMT_YVYU + MVFMT_NV21, PackedY422_to_NVx },
    { MVFMT_YVYU + MVFMT_Y42T, PackedY422_to_Y42T },
    { MVFMT_YVYU + MVFMT_Y41T, PackedY422_to_Y41T },
    { MVFMT_YVYU + MVFMT_YV16, PackedY422_to_YV16 },

    { MVFMT_VYUY + MVFMT_YUY2, PackedY422_to_PackedY422 },
    { MVFMT_VYUY + MVFMT_UYVY, PackedY422_to_PackedY422 },
    { MVFMT_VYUY + MVFMT_YVYU, PackedY422_to_PackedY422 },
    { MVFMT_VYUY + MVFMT_I420, PackedY422_to_PlanarYUV },
    { MVFMT_VYUY + MVFMT_YV12, PackedY422_to_PlanarYUV },
    { MVFMT_VYUY + MVFMT_YVU9, PackedY422_to_PlanarYUV },
    { MVFMT_VYUY + MVFMT_YUV9, PackedY422_to_PlanarYUV },
    { MVFMT_VYUY + MVFMT_IYU1, PackedY422_to_IYU1 },
    { MVFMT_VYUY + MVFMT_IYU2, PackedY422_to_IYU2 },
    { MVFMT_VYUY + MVFMT_Y800, PackedY422_to_Y800 },
    { MVFMT_VYUY + MVFMT_Y16, PackedY422_to_Y16 },
    { MVFMT_VYUY + MVFMT_CLJR, PackedY422_to_CLJR },
    { MVFMT_VYUY + MVFMT_Y41P, PackedY422_to_Y41P },
    { MVFMT_VYUY + MVFMT_AYUV, PackedY422_to_AYUV },
    { MVFMT_VYUY + MVFMT_IMC1, PackedY422_to_IMCx },
    { MVFMT_VYUY + MVFMT_IMC2, PackedY422_to_IMCx },
    { MVFMT_VYUY + MVFMT_IMC3, PackedY422_to_IMCx },
    { MVFMT_VYUY + MVFMT_IMC4, PackedY422_to_IMCx },
    { MVFMT_VYUY + MVFMT_NV12, PackedY422_to_NVx },
    { MVFMT_VYUY + MVFMT_NV21, PackedY422_to_NVx },
    { MVFMT_VYUY + MVFMT_Y42T, PackedY422_to_Y42T },
    { MVFMT_VYUY + MVFMT_Y41T, PackedY422_to_Y41T },
    { MVFMT_VYUY + MVFMT_YV16, PackedY422_to_YV16 },

    { MVFMT_I420 + MVFMT_YVU9, PlanarYUV_to_PlanarYUV },
    { MVFMT_I420 + MVFMT_YUV9, PlanarYUV_to_PlanarYUV },
    { MVFMT_I420 + MVFMT_YV12, PlanarYUV_to_PlanarYUV },
    { MVFMT_I420 + MVFMT_YUY2, PlanarYUV_to_PackedY422 },
    { MVFMT_I420 + MVFMT_UYVY, PlanarYUV_to_PackedY422 },
    { MVFMT_I420 + MVFMT_YVYU, PlanarYUV_to_PackedY422 },
    { MVFMT_I420 + MVFMT_VYUY, PlanarYUV_to_PackedY422 },
    { MVFMT_I420 + MVFMT_IYU1, PlanarYUV_to_IYU1 },
    { MVFMT_I420 + MVFMT_IYU2, PlanarYUV_to_IYU2 },
    { MVFMT_I420 + MVFMT_Y800, PlanarYUV_to_Y800 },
    { MVFMT_I420 + MVFMT_Y16, PlanarYUV_to_Y16 },
    { MVFMT_I420 + MVFMT_CLJR, PlanarYUV_to_CLJR },
    { MVFMT_I420 + MVFMT_Y41P, PlanarYUV_to_Y41P },
    { MVFMT_I420 + MVFMT_AYUV, PlanarYUV_to_AYUV },
    { MVFMT_I420 + MVFMT_IMC1, PlanarYUV_to_IMCx },
    { MVFMT_I420 + MVFMT_IMC2, PlanarYUV_to_IMCx },
    { MVFMT_I420 + MVFMT_IMC3, PlanarYUV_to_IMCx },
    { MVFMT_I420 + MVFMT_IMC4, PlanarYUV_to_IMCx },
    { MVFMT_I420 + MVFMT_NV12, PlanarYUV_to_NVx },
    { MVFMT_I420 + MVFMT_NV21, PlanarYUV_to_NVx },
    { MVFMT_I420 + MVFMT_Y42T, PlanarYUV_to_Y42T },
    { MVFMT_I420 + MVFMT_Y41T, PlanarYUV_to_Y41T },
    { MVFMT_I420 + MVFMT_YV16, PlanarYUV_to_YV16 },

    { MVFMT_YV12 + MVFMT_YVU9, PlanarYUV_to_PlanarYUV },
    { MVFMT_YV12 + MVFMT_YUV9, PlanarYUV_to_PlanarYUV },
    { MVFMT_YV12 + MVFMT_I420, PlanarYUV_to_PlanarYUV },
    { MVFMT_YV12 + MVFMT_YUY2, PlanarYUV_to_PackedY422 },
    { MVFMT_YV12 + MVFMT_UYVY, PlanarYUV_to_PackedY422 },
    { MVFMT_YV12 + MVFMT_YVYU, PlanarYUV_to_PackedY422 },
    { MVFMT_YV12 + MVFMT_VYUY, PlanarYUV_to_PackedY422 },
    { MVFMT_YV12 + MVFMT_IYU1, PlanarYUV_to_IYU1 },
    { MVFMT_YV12 + MVFMT_IYU2, PlanarYUV_to_IYU2 },
    { MVFMT_YV12 + MVFMT_Y800, PlanarYUV_to_Y800 },
    { MVFMT_YV12 + MVFMT_Y16, PlanarYUV_to_Y16 },
    { MVFMT_YV12 + MVFMT_CLJR, PlanarYUV_to_CLJR },
    { MVFMT_YV12 + MVFMT_Y41P, PlanarYUV_to_Y41P },
    { MVFMT_YV12 + MVFMT_AYUV, PlanarYUV_to_AYUV },
    { MVFMT_YV12 + MVFMT_IMC1, PlanarYUV_to_IMCx },
    { MVFMT_YV12 + MVFMT_IMC2, PlanarYUV_to_IMCx },
    { MVFMT_YV12 + MVFMT_IMC3, PlanarYUV_to_IMCx },
    { MVFMT_YV12 + MVFMT_IMC4, PlanarYUV_to_IMCx },
    { MVFMT_YV12 + MVFMT_NV12, PlanarYUV_to_NVx },
    { MVFMT_YV12 + MVFMT_NV21, PlanarYUV_to_NVx },
    { MVFMT_YV12 + MVFMT_Y42T, PlanarYUV_to_Y42T },
    { MVFMT_YV12 + MVFMT_Y41T, PlanarYUV_to_Y41T },
    { MVFMT_YV12 + MVFMT_YV16, PlanarYUV_to_YV16 },

    { MVFMT_YVU9 + MVFMT_I420, PlanarYUV_to_PlanarYUV },
    { MVFMT_YVU9 + MVFMT_YV12, PlanarYUV_to_PlanarYUV },
    { MVFMT_YVU9 + MVFMT_YUV9, PlanarYUV_to_PlanarYUV },
    { MVFMT_YVU9 + MVFMT_YUY2, PlanarYUV_to_PackedY422 },
    { MVFMT_YVU9 + MVFMT_UYVY, PlanarYUV_to_PackedY422 },
    { MVFMT_YVU9 + MVFMT_YVYU, PlanarYUV_to_PackedY422 },
    { MVFMT_YVU9 + MVFMT_VYUY, PlanarYUV_to_PackedY422 },
    { MVFMT_YVU9 + MVFMT_IYU1, PlanarYUV_to_IYU1 },
    { MVFMT_YVU9 + MVFMT_IYU2, PlanarYUV_to_IYU2 },
    { MVFMT_YVU9 + MVFMT_Y800, PlanarYUV_to_Y800 },
    { MVFMT_YVU9 + MVFMT_Y16, PlanarYUV_to_Y16 },
    { MVFMT_YVU9 + MVFMT_CLJR, PlanarYUV_to_CLJR },
    { MVFMT_YVU9 + MVFMT_Y41P, PlanarYUV_to_Y41P },
    { MVFMT_YVU9 + MVFMT_AYUV, PlanarYUV_to_AYUV },
    { MVFMT_YVU9 + MVFMT_IMC1, PlanarYUV_to_IMCx },
    { MVFMT_YVU9 + MVFMT_IMC2, PlanarYUV_to_IMCx },
    { MVFMT_YVU9 + MVFMT_IMC3, PlanarYUV_to_IMCx },
    { MVFMT_YVU9 + MVFMT_IMC4, PlanarYUV_to_IMCx },
    { MVFMT_YVU9 + MVFMT_NV12, PlanarYUV_to_NVx },
    { MVFMT_YVU9 + MVFMT_NV21, PlanarYUV_to_NVx },
    { MVFMT_YVU9 + MVFMT_Y42T, PlanarYUV_to_Y42T },
    { MVFMT_YVU9 + MVFMT_Y41T, PlanarYUV_to_Y41T },
    { MVFMT_YVU9 + MVFMT_YV16, PlanarYUV_to_YV16 },

    { MVFMT_YUV9 + MVFMT_I420, PlanarYUV_to_PlanarYUV },
    { MVFMT_YUV9 + MVFMT_YV12, PlanarYUV_to_PlanarYUV },
    { MVFMT_YUV9 + MVFMT_YVU9, PlanarYUV_to_PlanarYUV },
    { MVFMT_YUV9 + MVFMT_YUY2, PlanarYUV_to_PackedY422 },
    { MVFMT_YUV9 + MVFMT_UYVY, PlanarYUV_to_PackedY422 },
    { MVFMT_YUV9 + MVFMT_YVYU, PlanarYUV_to_PackedY422 },
    { MVFMT_YUV9 + MVFMT_VYUY, PlanarYUV_to_PackedY422 },
    { MVFMT_YUV9 + MVFMT_IYU1, PlanarYUV_to_IYU1 },
    { MVFMT_YUV9 + MVFMT_IYU2, PlanarYUV_to_IYU2 },
    { MVFMT_YUV9 + MVFMT_Y800, PlanarYUV_to_Y800 },
    { MVFMT_YUV9 + MVFMT_Y16, PlanarYUV_to_Y16 },
    { MVFMT_YUV9 + MVFMT_CLJR, PlanarYUV_to_CLJR },
    { MVFMT_YUV9 + MVFMT_Y41P, PlanarYUV_to_Y41P },
    { MVFMT_YUV9 + MVFMT_AYUV, PlanarYUV_to_AYUV },
    { MVFMT_YUV9 + MVFMT_IMC1, PlanarYUV_to_IMCx },
    { MVFMT_YUV9 + MVFMT_IMC2, PlanarYUV_to_IMCx },
    { MVFMT_YUV9 + MVFMT_IMC3, PlanarYUV_to_IMCx },
    { MVFMT_YUV9 + MVFMT_IMC4, PlanarYUV_to_IMCx },
    { MVFMT_YUV9 + MVFMT_NV12, PlanarYUV_to_NVx },
    { MVFMT_YUV9 + MVFMT_NV21, PlanarYUV_to_NVx },
    { MVFMT_YUV9 + MVFMT_Y42T, PlanarYUV_to_Y42T },
    { MVFMT_YUV9 + MVFMT_Y41T, PlanarYUV_to_Y41T },
    { MVFMT_YUV9 + MVFMT_YV16, PlanarYUV_to_YV16 },

    { MVFMT_IYU1 + MVFMT_YUY2, IYU1_to_PackedY422 },
    { MVFMT_IYU1 + MVFMT_UYVY, IYU1_to_PackedY422 },
    { MVFMT_IYU1 + MVFMT_YVYU, IYU1_to_PackedY422 },
    { MVFMT_IYU1 + MVFMT_VYUY, IYU1_to_PackedY422 },
    { MVFMT_IYU1 + MVFMT_I420, IYU1_to_PlanarYUV },
    { MVFMT_IYU1 + MVFMT_YV12, IYU1_to_PlanarYUV },
    { MVFMT_IYU1 + MVFMT_YVU9, IYU1_to_PlanarYUV },
    { MVFMT_IYU1 + MVFMT_YUV9, IYU1_to_PlanarYUV },
    { MVFMT_IYU1 + MVFMT_IYU2, IYU1_to_IYU2 },
    { MVFMT_IYU1 + MVFMT_Y800, IYU1_to_Y800 },
    { MVFMT_IYU1 + MVFMT_Y16, IYU1_to_Y16 },
    { MVFMT_IYU1 + MVFMT_CLJR, IYU1_to_CLJR },
    { MVFMT_IYU1 + MVFMT_Y41P, IYU1_to_Y41P },
    { MVFMT_IYU1 + MVFMT_AYUV, IYU1_to_AYUV },
    { MVFMT_IYU1 + MVFMT_IMC1, IYU1_to_IMCx },
    { MVFMT_IYU1 + MVFMT_IMC2, IYU1_to_IMCx },
    { MVFMT_IYU1 + MVFMT_IMC3, IYU1_to_IMCx },
    { MVFMT_IYU1 + MVFMT_IMC4, IYU1_to_IMCx },
    { MVFMT_IYU1 + MVFMT_NV12, IYU1_to_NVx },
    { MVFMT_IYU1 + MVFMT_NV21, IYU1_to_NVx },
    { MVFMT_IYU1 + MVFMT_Y42T, IYU1_to_Y42T },
    { MVFMT_IYU1 + MVFMT_Y41T, IYU1_to_Y41T },
    { MVFMT_IYU1 + MVFMT_YV16, IYU1_to_YV16 },

    { MVFMT_IYU2 + MVFMT_YUY2, IYU2_to_PackedY422 },
    { MVFMT_IYU2 + MVFMT_UYVY, IYU2_to_PackedY422 },
    { MVFMT_IYU2 + MVFMT_YVYU, IYU2_to_PackedY422 },
    { MVFMT_IYU2 + MVFMT_VYUY, IYU2_to_PackedY422 },
    { MVFMT_IYU2 + MVFMT_I420, IYU2_to_PlanarYUV },
    { MVFMT_IYU2 + MVFMT_YV12, IYU2_to_PlanarYUV },
    { MVFMT_IYU2 + MVFMT_YVU9, IYU2_to_PlanarYUV },
    { MVFMT_IYU2 + MVFMT_YUV9, IYU2_to_PlanarYUV },
    { MVFMT_IYU2 + MVFMT_IYU1, IYU2_to_IYU1 },
    { MVFMT_IYU2 + MVFMT_Y800, IYU2_to_Y800 },
    { MVFMT_IYU2 + MVFMT_Y16, IYU2_to_Y16 },
    { MVFMT_IYU2 + MVFMT_CLJR, IYU2_to_CLJR },
    { MVFMT_IYU2 + MVFMT_Y41P, IYU2_to_Y41P },
    { MVFMT_IYU2 + MVFMT_AYUV, IYU2_to_AYUV },
    { MVFMT_IYU2 + MVFMT_IMC1, IYU2_to_IMCx },
    { MVFMT_IYU2 + MVFMT_IMC2, IYU2_to_IMCx },
    { MVFMT_IYU2 + MVFMT_IMC3, IYU2_to_IMCx },
    { MVFMT_IYU2 + MVFMT_IMC4, IYU2_to_IMCx },
    { MVFMT_IYU2 + MVFMT_NV12, IYU2_to_NVx },
    { MVFMT_IYU2 + MVFMT_NV21, IYU2_to_NVx },
    { MVFMT_IYU2 + MVFMT_Y42T, IYU2_to_Y42T },
    { MVFMT_IYU2 + MVFMT_Y41T, IYU2_to_Y41T},
    { MVFMT_IYU2 + MVFMT_YV16, IYU2_to_YV16 },

    { MVFMT_Y800 + MVFMT_YUY2, Y800_to_PackedY422 },
    { MVFMT_Y800 + MVFMT_UYVY, Y800_to_PackedY422 },
    { MVFMT_Y800 + MVFMT_YVYU, Y800_to_PackedY422 },
    { MVFMT_Y800 + MVFMT_VYUY, Y800_to_PackedY422 },
    { MVFMT_Y800 + MVFMT_I420, Y800_to_PlanarYUV },
    { MVFMT_Y800 + MVFMT_YV12, Y800_to_PlanarYUV },
    { MVFMT_Y800 + MVFMT_YVU9, Y800_to_PlanarYUV },
    { MVFMT_Y800 + MVFMT_YUV9, Y800_to_PlanarYUV },
    { MVFMT_Y800 + MVFMT_IYU1, Y800_to_IYU1 },
    { MVFMT_Y800 + MVFMT_IYU2, Y800_to_IYU2 },
    { MVFMT_Y800 + MVFMT_CLJR, Y800_to_CLJR },
    { MVFMT_Y800 + MVFMT_Y41P, Y800_to_Y41P },
    { MVFMT_Y800 + MVFMT_AYUV, Y800_to_AYUV },
    { MVFMT_Y800 + MVFMT_Y16, Y800_to_Y16 },
    { MVFMT_Y800 + MVFMT_IMC1, Y800_to_IMCx },
    { MVFMT_Y800 + MVFMT_IMC2, Y800_to_IMCx },
    { MVFMT_Y800 + MVFMT_IMC3, Y800_to_IMCx },
    { MVFMT_Y800 + MVFMT_IMC4, Y800_to_IMCx },
    { MVFMT_Y800 + MVFMT_NV12, Y800_to_NVx },
    { MVFMT_Y800 + MVFMT_NV21, Y800_to_NVx },
    { MVFMT_Y800 + MVFMT_Y42T, Y800_to_Y42T },
    { MVFMT_Y800 + MVFMT_Y41T, Y800_to_Y41T },
    { MVFMT_Y800 + MVFMT_YV16, Y800_to_YV16 },

    { MVFMT_Y16 + MVFMT_YUY2, Y16_to_PackedY422 },
    { MVFMT_Y16 + MVFMT_UYVY, Y16_to_PackedY422 },
    { MVFMT_Y16 + MVFMT_YVYU, Y16_to_PackedY422 },
    { MVFMT_Y16 + MVFMT_VYUY, Y16_to_PackedY422 },
    { MVFMT_Y16 + MVFMT_I420, Y16_to_PlanarYUV },
    { MVFMT_Y16 + MVFMT_YV12, Y16_to_PlanarYUV },
    { MVFMT_Y16 + MVFMT_YVU9, Y16_to_PlanarYUV },
    { MVFMT_Y16 + MVFMT_YUV9, Y16_to_PlanarYUV },
    { MVFMT_Y16 + MVFMT_IYU1, Y16_to_IYU1 },
    { MVFMT_Y16 + MVFMT_IYU2, Y16_to_IYU2 },
    { MVFMT_Y16 + MVFMT_CLJR, Y16_to_CLJR },
    { MVFMT_Y16 + MVFMT_Y41P, Y16_to_Y41P },
    { MVFMT_Y16 + MVFMT_AYUV, Y16_to_AYUV },
    { MVFMT_Y16 + MVFMT_Y800, Y16_to_Y800 },
    { MVFMT_Y16 + MVFMT_IMC1, Y16_to_IMCx },
    { MVFMT_Y16 + MVFMT_IMC2, Y16_to_IMCx },
    { MVFMT_Y16 + MVFMT_IMC3, Y16_to_IMCx },
    { MVFMT_Y16 + MVFMT_IMC4, Y16_to_IMCx },
    { MVFMT_Y16 + MVFMT_NV12, Y16_to_NVx },
    { MVFMT_Y16 + MVFMT_NV21, Y16_to_NVx },
    { MVFMT_Y16 + MVFMT_Y42T, Y16_to_Y42T },
    { MVFMT_Y16 + MVFMT_Y41T, Y16_to_Y41T },
    { MVFMT_Y16 + MVFMT_YV16, Y16_to_YV16 },

    { MVFMT_CLJR + MVFMT_YUY2, CLJR_to_PackedY422 },
    { MVFMT_CLJR + MVFMT_UYVY, CLJR_to_PackedY422 },
    { MVFMT_CLJR + MVFMT_YVYU, CLJR_to_PackedY422 },
    { MVFMT_CLJR + MVFMT_VYUY, CLJR_to_PackedY422 },
    { MVFMT_CLJR + MVFMT_I420, CLJR_to_PlanarYUV },
    { MVFMT_CLJR + MVFMT_YV12, CLJR_to_PlanarYUV },
    { MVFMT_CLJR + MVFMT_YVU9, CLJR_to_PlanarYUV },
    { MVFMT_CLJR + MVFMT_YUV9, CLJR_to_PlanarYUV },
    { MVFMT_CLJR + MVFMT_IYU1, CLJR_to_IYU1 },
    { MVFMT_CLJR + MVFMT_IYU2, CLJR_to_IYU2 },
    { MVFMT_CLJR + MVFMT_Y800, CLJR_to_Y800 },
    { MVFMT_CLJR + MVFMT_Y16, CLJR_to_Y16 },
    { MVFMT_CLJR + MVFMT_Y41P, CLJR_to_Y41P },
    { MVFMT_CLJR + MVFMT_AYUV, CLJR_to_AYUV },
    { MVFMT_CLJR + MVFMT_IMC1, CLJR_to_IMCx },
    { MVFMT_CLJR + MVFMT_IMC2, CLJR_to_IMCx },
    { MVFMT_CLJR + MVFMT_IMC3, CLJR_to_IMCx },
    { MVFMT_CLJR + MVFMT_IMC4, CLJR_to_IMCx },
    { MVFMT_CLJR + MVFMT_NV12, CLJR_to_NVx },
    { MVFMT_CLJR + MVFMT_NV21, CLJR_to_NVx },
    { MVFMT_CLJR + MVFMT_Y42T, CLJR_to_Y42T },
    { MVFMT_CLJR + MVFMT_Y41T, CLJR_to_Y41T },
    { MVFMT_CLJR + MVFMT_YV16, CLJR_to_YV16 },

    { MVFMT_Y41P + MVFMT_YUY2, Y41P_to_PackedY422 },
    { MVFMT_Y41P + MVFMT_UYVY, Y41P_to_PackedY422 },
    { MVFMT_Y41P + MVFMT_YVYU, Y41P_to_PackedY422 },
    { MVFMT_Y41P + MVFMT_VYUY, Y41P_to_PackedY422 },
    { MVFMT_Y41P + MVFMT_I420, Y41P_to_PlanarYUV },
    { MVFMT_Y41P + MVFMT_YV12, Y41P_to_PlanarYUV },
    { MVFMT_Y41P + MVFMT_YVU9, Y41P_to_PlanarYUV },
    { MVFMT_Y41P + MVFMT_YUV9, Y41P_to_PlanarYUV },
    { MVFMT_Y41P + MVFMT_IYU1, Y41P_to_IYU1 },
    { MVFMT_Y41P + MVFMT_IYU2, Y41P_to_IYU2 },
    { MVFMT_Y41P + MVFMT_Y800, Y41P_to_Y800 },
    { MVFMT_Y41P + MVFMT_Y16, Y41P_to_Y16 },
    { MVFMT_Y41P + MVFMT_CLJR, Y41P_to_CLJR },
    { MVFMT_Y41P + MVFMT_IY41, Y41P_to_IY41 },
    { MVFMT_Y41P + MVFMT_AYUV, Y41P_to_AYUV },
    { MVFMT_Y41P + MVFMT_IMC1, Y41P_to_IMCx },
    { MVFMT_Y41P + MVFMT_IMC2, Y41P_to_IMCx },
    { MVFMT_Y41P + MVFMT_IMC3, Y41P_to_IMCx },
    { MVFMT_Y41P + MVFMT_IMC4, Y41P_to_IMCx },
    { MVFMT_Y41P + MVFMT_NV12, Y41P_to_NVx },
    { MVFMT_Y41P + MVFMT_NV21, Y41P_to_NVx },
    { MVFMT_Y41P + MVFMT_Y42T, Y41P_to_Y42T },
    { MVFMT_Y41P + MVFMT_Y41T, Y41P_to_Y41T },
    { MVFMT_Y41P + MVFMT_YV16, Y41P_to_YV16 },

    { MVFMT_IMC1 + MVFMT_AYUV, IMCx_to_AYUV },
    { MVFMT_IMC1 + MVFMT_YUY2, IMCx_to_PackedY422 },
    { MVFMT_IMC1 + MVFMT_UYVY, IMCx_to_PackedY422 },
    { MVFMT_IMC1 + MVFMT_YVYU, IMCx_to_PackedY422 },
    { MVFMT_IMC1 + MVFMT_VYUY, IMCx_to_PackedY422 },
    { MVFMT_IMC1 + MVFMT_I420, IMCx_to_PlanarYUV },
    { MVFMT_IMC1 + MVFMT_YV12, IMCx_to_PlanarYUV },
    { MVFMT_IMC1 + MVFMT_YUV9, IMCx_to_PlanarYUV },
    { MVFMT_IMC1 + MVFMT_YVU9, IMCx_to_PlanarYUV },
    { MVFMT_IMC1 + MVFMT_IYU1, IMCx_to_IYU1 },
    { MVFMT_IMC1 + MVFMT_IYU2, IMCx_to_IYU2 },
    { MVFMT_IMC1 + MVFMT_Y800, IMCx_to_Y800 },
    { MVFMT_IMC1 + MVFMT_Y16, IMCx_to_Y16 },
    { MVFMT_IMC1 + MVFMT_CLJR, IMCx_to_CLJR },
    { MVFMT_IMC1 + MVFMT_Y41P, IMCx_to_Y41P },
    { MVFMT_IMC1 + MVFMT_IMC2, IMCx_to_IMCx },
    { MVFMT_IMC1 + MVFMT_IMC3, IMCx_to_IMCx },
    { MVFMT_IMC1 + MVFMT_IMC4, IMCx_to_IMCx },
    { MVFMT_IMC1 + MVFMT_NV12, IMCx_to_NVx },
    { MVFMT_IMC1 + MVFMT_NV21, IMCx_to_NVx },
    { MVFMT_IMC1 + MVFMT_Y42T, IMCx_to_Y42T },
    { MVFMT_IMC1 + MVFMT_Y41T, IMCx_to_Y41T },
    { MVFMT_IMC1 + MVFMT_YV16, IMCx_to_YV16 },

    { MVFMT_IMC2 + MVFMT_AYUV, IMCx_to_AYUV },
    { MVFMT_IMC2 + MVFMT_YUY2, IMCx_to_PackedY422 },
    { MVFMT_IMC2 + MVFMT_UYVY, IMCx_to_PackedY422 },
    { MVFMT_IMC2 + MVFMT_YVYU, IMCx_to_PackedY422 },
    { MVFMT_IMC2 + MVFMT_VYUY, IMCx_to_PackedY422 },
    { MVFMT_IMC2 + MVFMT_I420, IMCx_to_PlanarYUV },
    { MVFMT_IMC2 + MVFMT_YV12, IMCx_to_PlanarYUV },
    { MVFMT_IMC2 + MVFMT_YUV9, IMCx_to_PlanarYUV },
    { MVFMT_IMC2 + MVFMT_YVU9, IMCx_to_PlanarYUV },
    { MVFMT_IMC2 + MVFMT_IYU1, IMCx_to_IYU1 },
    { MVFMT_IMC2 + MVFMT_IYU2, IMCx_to_IYU2 },
    { MVFMT_IMC2 + MVFMT_Y800, IMCx_to_Y800 },
    { MVFMT_IMC2 + MVFMT_Y16, IMCx_to_Y16 },
    { MVFMT_IMC2 + MVFMT_CLJR, IMCx_to_CLJR },
    { MVFMT_IMC2 + MVFMT_Y41P, IMCx_to_Y41P },
    { MVFMT_IMC2 + MVFMT_IMC1, IMCx_to_IMCx },
    { MVFMT_IMC2 + MVFMT_IMC3, IMCx_to_IMCx },
    { MVFMT_IMC2 + MVFMT_IMC4, IMCx_to_IMCx },
    { MVFMT_IMC2 + MVFMT_NV12, IMCx_to_NVx },
    { MVFMT_IMC2 + MVFMT_NV21, IMCx_to_NVx },
    { MVFMT_IMC2 + MVFMT_Y42T, IMCx_to_Y42T },
    { MVFMT_IMC2 + MVFMT_Y41T, IMCx_to_Y41T },
    { MVFMT_IMC2 + MVFMT_YV16, IMCx_to_YV16 },

    { MVFMT_IMC3 + MVFMT_AYUV, IMCx_to_AYUV },
    { MVFMT_IMC3 + MVFMT_YUY2, IMCx_to_PackedY422 },
    { MVFMT_IMC3 + MVFMT_UYVY, IMCx_to_PackedY422 },
    { MVFMT_IMC3 + MVFMT_YVYU, IMCx_to_PackedY422 },
    { MVFMT_IMC3 + MVFMT_VYUY, IMCx_to_PackedY422 },
    { MVFMT_IMC3 + MVFMT_I420, IMCx_to_PlanarYUV },
    { MVFMT_IMC3 + MVFMT_YV12, IMCx_to_PlanarYUV },
    { MVFMT_IMC3 + MVFMT_YUV9, IMCx_to_PlanarYUV },
    { MVFMT_IMC3 + MVFMT_YVU9, IMCx_to_PlanarYUV },
    { MVFMT_IMC3 + MVFMT_IYU1, IMCx_to_IYU1 },
    { MVFMT_IMC3 + MVFMT_IYU2, IMCx_to_IYU2 },
    { MVFMT_IMC3 + MVFMT_Y800, IMCx_to_Y800 },
    { MVFMT_IMC3 + MVFMT_Y16, IMCx_to_Y16 },
    { MVFMT_IMC3 + MVFMT_CLJR, IMCx_to_CLJR },
    { MVFMT_IMC3 + MVFMT_Y41P, IMCx_to_Y41P },
    { MVFMT_IMC3 + MVFMT_IMC1, IMCx_to_IMCx },
    { MVFMT_IMC3 + MVFMT_IMC2, IMCx_to_IMCx },
    { MVFMT_IMC3 + MVFMT_IMC4, IMCx_to_IMCx },
    { MVFMT_IMC3 + MVFMT_NV12, IMCx_to_NVx },
    { MVFMT_IMC3 + MVFMT_NV21, IMCx_to_NVx },
    { MVFMT_IMC3 + MVFMT_Y42T, IMCx_to_Y42T },
    { MVFMT_IMC3 + MVFMT_Y41T, IMCx_to_Y41T },
    { MVFMT_IMC3 + MVFMT_YV16, IMCx_to_YV16 },

    { MVFMT_IMC4 + MVFMT_AYUV, IMCx_to_AYUV },
    { MVFMT_IMC4 + MVFMT_YUY2, IMCx_to_PackedY422 },
    { MVFMT_IMC4 + MVFMT_UYVY, IMCx_to_PackedY422 },
    { MVFMT_IMC4 + MVFMT_YVYU, IMCx_to_PackedY422 },
    { MVFMT_IMC4 + MVFMT_VYUY, IMCx_to_PackedY422 },
    { MVFMT_IMC4 + MVFMT_I420, IMCx_to_PlanarYUV },
    { MVFMT_IMC4 + MVFMT_YV12, IMCx_to_PlanarYUV },
    { MVFMT_IMC4 + MVFMT_YUV9, IMCx_to_PlanarYUV },
    { MVFMT_IMC4 + MVFMT_YVU9, IMCx_to_PlanarYUV },
    { MVFMT_IMC4 + MVFMT_IYU1, IMCx_to_IYU1 },
    { MVFMT_IMC4 + MVFMT_IYU2, IMCx_to_IYU2 },
    { MVFMT_IMC4 + MVFMT_Y800, IMCx_to_Y800 },
    { MVFMT_IMC4 + MVFMT_Y16, IMCx_to_Y16 },
    { MVFMT_IMC4 + MVFMT_CLJR, IMCx_to_CLJR },
    { MVFMT_IMC4 + MVFMT_Y41P, IMCx_to_Y41P },
    { MVFMT_IMC4 + MVFMT_IMC1, IMCx_to_IMCx },
    { MVFMT_IMC4 + MVFMT_IMC2, IMCx_to_IMCx },
    { MVFMT_IMC4 + MVFMT_IMC3, IMCx_to_IMCx },
    { MVFMT_IMC4 + MVFMT_NV12, IMCx_to_NVx },
    { MVFMT_IMC4 + MVFMT_NV21, IMCx_to_NVx },
    { MVFMT_IMC4 + MVFMT_Y42T, IMCx_to_Y42T },
    { MVFMT_IMC4 + MVFMT_Y41T, IMCx_to_Y41T },
    { MVFMT_IMC4 + MVFMT_YV16, IMCx_to_YV16 },

    { MVFMT_NV12 + MVFMT_AYUV, NVx_to_AYUV },
    { MVFMT_NV12 + MVFMT_YUY2, NVx_to_PackedY422 },
    { MVFMT_NV12 + MVFMT_UYVY, NVx_to_PackedY422 },
    { MVFMT_NV12 + MVFMT_YVYU, NVx_to_PackedY422 },
    { MVFMT_NV12 + MVFMT_VYUY, NVx_to_PackedY422 },
    { MVFMT_NV12 + MVFMT_I420, NVx_to_PlanarYUV },
    { MVFMT_NV12 + MVFMT_YV12, NVx_to_PlanarYUV },
    { MVFMT_NV12 + MVFMT_YUV9, NVx_to_PlanarYUV },
    { MVFMT_NV12 + MVFMT_YVU9, NVx_to_PlanarYUV },
    { MVFMT_NV12 + MVFMT_IYU1, NVx_to_IYU1 },
    { MVFMT_NV12 + MVFMT_IYU2, NVx_to_IYU2 },
    { MVFMT_NV12 + MVFMT_Y800, NVx_to_Y800 },
    { MVFMT_NV12 + MVFMT_Y16, NVx_to_Y16 },
    { MVFMT_NV12 + MVFMT_CLJR, NVx_to_CLJR },
    { MVFMT_NV12 + MVFMT_Y41P, NVx_to_Y41P },
    { MVFMT_NV12 + MVFMT_IMC1, NVx_to_IMCx },
    { MVFMT_NV12 + MVFMT_IMC2, NVx_to_IMCx },
    { MVFMT_NV12 + MVFMT_IMC3, NVx_to_IMCx },
    { MVFMT_NV12 + MVFMT_IMC4, NVx_to_IMCx },
    { MVFMT_NV12 + MVFMT_NV21, NVx_to_NVx },
    { MVFMT_NV12 + MVFMT_Y42T, NVx_to_Y42T },
    { MVFMT_NV12 + MVFMT_Y41T, NVx_to_Y41T },
    { MVFMT_NV12 + MVFMT_YV16, NVx_to_YV16 },

    { MVFMT_NV21 + MVFMT_AYUV, NVx_to_AYUV },
    { MVFMT_NV21 + MVFMT_YUY2, NVx_to_PackedY422 },
    { MVFMT_NV21 + MVFMT_UYVY, NVx_to_PackedY422 },
    { MVFMT_NV21 + MVFMT_YVYU, NVx_to_PackedY422 },
    { MVFMT_NV21 + MVFMT_VYUY, NVx_to_PackedY422 },
    { MVFMT_NV21 + MVFMT_I420, NVx_to_PlanarYUV },
    { MVFMT_NV21 + MVFMT_YV12, NVx_to_PlanarYUV },
    { MVFMT_NV21 + MVFMT_YUV9, NVx_to_PlanarYUV },
    { MVFMT_NV21 + MVFMT_YVU9, NVx_to_PlanarYUV },
    { MVFMT_NV21 + MVFMT_IYU1, NVx_to_IYU1 },
    { MVFMT_NV21 + MVFMT_IYU2, NVx_to_IYU2 },
    { MVFMT_NV21 + MVFMT_Y800, NVx_to_Y800 },
    { MVFMT_NV21 + MVFMT_Y16, NVx_to_Y16 },
    { MVFMT_NV21 + MVFMT_CLJR, NVx_to_CLJR },
    { MVFMT_NV21 + MVFMT_Y41P, NVx_to_Y41P },
    { MVFMT_NV21 + MVFMT_IMC1, NVx_to_IMCx },
    { MVFMT_NV21 + MVFMT_IMC2, NVx_to_IMCx },
    { MVFMT_NV21 + MVFMT_IMC3, NVx_to_IMCx },
    { MVFMT_NV21 + MVFMT_IMC4, NVx_to_IMCx },
    { MVFMT_NV21 + MVFMT_NV12, NVx_to_NVx },
    { MVFMT_NV21 + MVFMT_Y42T, NVx_to_Y42T },
    { MVFMT_NV21 + MVFMT_Y41T, NVx_to_Y41T },
    { MVFMT_NV21 + MVFMT_YV16, NVx_to_YV16 },

    { MVFMT_Y42T + MVFMT_YUY2, Y42T_to_PackedY422 },
    { MVFMT_Y42T + MVFMT_UYVY, Y42T_to_PackedY422 },
    { MVFMT_Y42T + MVFMT_YVYU, Y42T_to_PackedY422 },
    { MVFMT_Y42T + MVFMT_VYUY, Y42T_to_PackedY422 },
    { MVFMT_Y42T + MVFMT_I420, Y42T_to_PlanarYUV },
    { MVFMT_Y42T + MVFMT_YV12, Y42T_to_PlanarYUV },
    { MVFMT_Y42T + MVFMT_YUV9, Y42T_to_PlanarYUV },
    { MVFMT_Y42T + MVFMT_YVU9, Y42T_to_PlanarYUV },
    { MVFMT_Y42T + MVFMT_IYU1, Y42T_to_IYU1 },
    { MVFMT_Y42T + MVFMT_IYU2, Y42T_to_IYU2 },
    { MVFMT_Y42T + MVFMT_Y800, Y42T_to_Y800 },
    { MVFMT_Y42T + MVFMT_Y16, Y42T_to_Y16 },
    { MVFMT_Y42T + MVFMT_CLJR, Y42T_to_CLJR },
    { MVFMT_Y42T + MVFMT_Y41P, Y42T_to_Y41P },
    { MVFMT_Y42T + MVFMT_AYUV, Y42T_to_AYUV },
    { MVFMT_Y42T + MVFMT_IMC1, Y42T_to_IMCx },
    { MVFMT_Y42T + MVFMT_IMC2, Y42T_to_IMCx },
    { MVFMT_Y42T + MVFMT_IMC3, Y42T_to_IMCx },
    { MVFMT_Y42T + MVFMT_IMC4, Y42T_to_IMCx },
    { MVFMT_Y42T + MVFMT_NV12, Y42T_to_NVx },
    { MVFMT_Y42T + MVFMT_NV21, Y42T_to_NVx },
    { MVFMT_Y42T + MVFMT_Y41T, Y42T_to_Y41T },
    { MVFMT_Y42T + MVFMT_YV16, Y42T_to_YV16 },

    { MVFMT_Y41T + MVFMT_YUY2, Y41T_to_PackedY422 },
    { MVFMT_Y41T + MVFMT_UYVY, Y41T_to_PackedY422 },
    { MVFMT_Y41T + MVFMT_YVYU, Y41T_to_PackedY422 },
    { MVFMT_Y41T + MVFMT_VYUY, Y41T_to_PackedY422 },
    { MVFMT_Y41T + MVFMT_I420, Y41T_to_PlanarYUV },
    { MVFMT_Y41T + MVFMT_YV12, Y41T_to_PlanarYUV },
    { MVFMT_Y41T + MVFMT_YUV9, Y41T_to_PlanarYUV },
    { MVFMT_Y41T + MVFMT_YVU9, Y41T_to_PlanarYUV },
    { MVFMT_Y41T + MVFMT_IYU1, Y41T_to_IYU1 },
    { MVFMT_Y41T + MVFMT_IYU2, Y41T_to_IYU2 },
    { MVFMT_Y41T + MVFMT_Y800, Y41T_to_Y800 },
    { MVFMT_Y41T + MVFMT_Y16, Y41T_to_Y16 },
    { MVFMT_Y41T + MVFMT_CLJR, Y41T_to_CLJR },
    { MVFMT_Y41T + MVFMT_Y41P, Y41T_to_Y41P },
    { MVFMT_Y41T + MVFMT_AYUV, Y41T_to_AYUV },
    { MVFMT_Y41T + MVFMT_IMC1, Y41T_to_IMCx },
    { MVFMT_Y41T + MVFMT_IMC2, Y41T_to_IMCx },
    { MVFMT_Y41T + MVFMT_IMC3, Y41T_to_IMCx },
    { MVFMT_Y41T + MVFMT_IMC4, Y41T_to_IMCx },
    { MVFMT_Y41T + MVFMT_NV12, Y41T_to_NVx },
    { MVFMT_Y41T + MVFMT_NV21, Y41T_to_NVx },
    { MVFMT_Y41T + MVFMT_Y42T, Y41T_to_Y42T },
    { MVFMT_Y41T + MVFMT_YV16, Y41T_to_YV16 },

    { MVFMT_YV16 + MVFMT_YUY2, YV16_to_PackedY422 },
    { MVFMT_YV16 + MVFMT_UYVY, YV16_to_PackedY422 },
    { MVFMT_YV16 + MVFMT_YVYU, YV16_to_PackedY422 },
    { MVFMT_YV16 + MVFMT_VYUY, YV16_to_PackedY422 },
    { MVFMT_YV16 + MVFMT_I420, YV16_to_PlanarYUV },
    { MVFMT_YV16 + MVFMT_YV12, YV16_to_PlanarYUV },
    { MVFMT_YV16 + MVFMT_YUV9, YV16_to_PlanarYUV },
    { MVFMT_YV16 + MVFMT_YVU9, YV16_to_PlanarYUV },
    { MVFMT_YV16 + MVFMT_IYU1, YV16_to_IYU1 },
    { MVFMT_YV16 + MVFMT_IYU2, YV16_to_IYU2 },
    { MVFMT_YV16 + MVFMT_Y800, PlanarYUV_to_Y800 },
    { MVFMT_YV16 + MVFMT_Y16, PlanarYUV_to_Y16 },
    { MVFMT_YV16 + MVFMT_CLJR, YV16_to_CLJR },
    { MVFMT_YV16 + MVFMT_Y41P, YV16_to_Y41P },
    { MVFMT_YV16 + MVFMT_AYUV, YV16_to_AYUV },
    { MVFMT_YV16 + MVFMT_IMC1, YV16_to_IMCx },
    { MVFMT_YV16 + MVFMT_IMC2, YV16_to_IMCx },
    { MVFMT_YV16 + MVFMT_IMC3, YV16_to_IMCx },
    { MVFMT_YV16 + MVFMT_IMC4, YV16_to_IMCx },
    { MVFMT_YV16 + MVFMT_NV12, YV16_to_NVx },
    { MVFMT_YV16 + MVFMT_NV21, YV16_to_NVx },
    { MVFMT_YV16 + MVFMT_Y42T, YV16_to_Y42T },
    { MVFMT_YV16 + MVFMT_Y41T, YV16_to_Y41T },

    { MVFMT_IUYV + MVFMT_UYVY, IUYV_to_UYVY },
    { MVFMT_IY41 + MVFMT_Y41P, IY41_to_Y41P }
};

map<MediaFormatID, t_greyscalefunc> GreyscaleMap = {
    { MVFMT_RGBA, RGBA_to_Greyscale },
    { MVFMT_RGB32, RGB32_to_Greyscale },
    { MVFMT_RGB24, RGB24_to_Greyscale },
    { MVFMT_RGB565, RGB565_to_Greyscale },
    { MVFMT_RGB555, RGB555_to_Greyscale },
    { MVFMT_ARGB1555, RGB555_to_Greyscale },
    { MVFMT_RGB8, RGB8_to_Greyscale },
    { MVFMT_RGB4, RGB4_to_Greyscale },
    { MVFMT_RGB1, RGB1_to_Greyscale },
    { MVFMT_AYUV, AYUV_to_Greyscale },
    { MVFMT_UYVY, UYVY_to_Greyscale },
    { MVFMT_YVYU, YVYU_to_Greyscale },
    { MVFMT_VYUY, VYUY_to_Greyscale },
    { MVFMT_YUY2, YUY2_to_Greyscale },
    { MVFMT_I420, I420_to_Greyscale },
    { MVFMT_YV12, YV12_to_Greyscale },
    { MVFMT_YVU9, YVU9_to_Greyscale },
    { MVFMT_YUV9, YUV9_to_Greyscale },
    { MVFMT_IYU1, IYU1_to_Greyscale },
    { MVFMT_IYU2, IYU2_to_Greyscale },
    { MVFMT_Y41P, Y41P_to_Greyscale },
    { MVFMT_CLJR, CLJR_to_Greyscale },
    { MVFMT_IMC1, IMC1_to_Greyscale },
    { MVFMT_IMC2, IMC2_to_Greyscale },
    { MVFMT_IMC3, IMC3_to_Greyscale },
    { MVFMT_IMC4, IMC4_to_Greyscale },
    { MVFMT_NV12, NV12_to_Greyscale },
    { MVFMT_NV21, NV21_to_Greyscale },
    { MVFMT_Y42T, Y42T_to_Greyscale },
    { MVFMT_Y41T, Y41T_to_Greyscale },
    { MVFMT_YV16, YV16_to_Greyscale }
};

map<MediaFormatID, t_fillcolorfunc> FillColorMap = {
    { MVFMT_RGBA, Fill_RGBA },
    { MVFMT_RGB32, Fill_RGB32 },
    { MVFMT_RGB24, Fill_RGB24 },
    { MVFMT_RGB565, Fill_RGB565 },
    { MVFMT_RGB555, Fill_RGB555 },
    { MVFMT_ARGB1555, Fill_ARGB1555 },
    { MVFMT_YUY2, Fill_YUY2 },
    { MVFMT_UYVY, Fill_UYVY },
    { MVFMT_YVYU, Fill_YVYU },
    { MVFMT_VYUY, Fill_VYUY },
    { MVFMT_I420, Fill_I420 },
    { MVFMT_YV12, Fill_YV12 },
    { MVFMT_YVU9, Fill_YVU9 },
    { MVFMT_YUV9, Fill_YUV9 },
    { MVFMT_IYU1, Fill_IYU1 },
    { MVFMT_IYU2, Fill_IYU2 },
    { MVFMT_Y800, Fill_Y800 },
    { MVFMT_Y16, Fill_Y16 },
    { MVFMT_Y41P, Fill_Y41P },
    { MVFMT_CLJR, Fill_CLJR },
    { MVFMT_AYUV, Fill_AYUV },
    { MVFMT_IMC1, Fill_IMC1 },
    { MVFMT_IMC2, Fill_IMC2 },
    { MVFMT_IMC3, Fill_IMC3 },
    { MVFMT_IMC4, Fill_IMC4 },
    { MVFMT_NV12, Fill_NV12 },
    { MVFMT_NV21, Fill_NV21 },
    { MVFMT_Y42T, Fill_Y42T },
    { MVFMT_Y41T, Fill_Y41T },
    { MVFMT_YV16, Fill_YV16 }
};

map<MediaFormatID, t_setpixelfunc> SetPixelMap = {
    { MVFMT_RGBA, SetPixel_RGBA },
    { MVFMT_RGB32, SetPixel_RGB32 },
    { MVFMT_RGB24, SetPixel_RGB24 },
    { MVFMT_RGB565, SetPixel_RGB565 },
    { MVFMT_RGB555, SetPixel_RGB555 },
    { MVFMT_ARGB1555, SetPixel_ARGB1555 },
    { MVFMT_YUY2, SetPixel_YUY2 },
    { MVFMT_UYVY, SetPixel_UYVY },
    { MVFMT_YVYU, SetPixel_YVYU },
    { MVFMT_VYUY, SetPixel_VYUY },
    { MVFMT_I420, SetPixel_I420 },
    { MVFMT_YV12, SetPixel_YV12 },
    { MVFMT_YVU9, SetPixel_YVU9 },
    { MVFMT_YUV9, SetPixel_YUV9 },
    { MVFMT_IYU1, SetPixel_IYU1 },
    { MVFMT_IYU2, SetPixel_IYU2 },
    { MVFMT_Y800, SetPixel_Y800 },
    { MVFMT_Y16, SetPixel_Y16 },
    { MVFMT_Y41P, SetPixel_Y41P },
    { MVFMT_CLJR, SetPixel_CLJR },
    { MVFMT_AYUV, SetPixel_AYUV },
    { MVFMT_IMC1, SetPixel_IMC1 },
    { MVFMT_IMC2, SetPixel_IMC2 },
    { MVFMT_IMC3, SetPixel_IMC3 },
    { MVFMT_IMC4, SetPixel_IMC4 },
    { MVFMT_NV12, SetPixel_NV12 },
    { MVFMT_NV21, SetPixel_NV21 },
    { MVFMT_Y42T, SetPixel_Y42T },
    { MVFMT_Y41T, SetPixel_Y41T },
    { MVFMT_YV16, SetPixel_YV16 }
};

map<MediaFormatID, t_calcbuffsizefunc> CalcBufSizeMap = {
    { MVFMT_RGBA, CalcBufferSize_RGBA },
    { MVFMT_RGB32, CalcBufferSize_RGB32 },
    { MVFMT_RGB24, CalcBufferSize_RGB24 },
    { MVFMT_RGB565, CalcBufferSize_RGB565 },
    { MVFMT_RGB555, CalcBufferSize_RGB555 },
    { MVFMT_ARGB1555, CalcBufferSize_ARGB1555 },
    { MVFMT_RGB8, CalcBufferSize_RGB8 },
    { MVFMT_RGB4, CalcBufferSize_RGB4 },
    { MVFMT_RGB1, CalcBufferSize_RGB1 },
    { MVFMT_AYUV, CalcBufferSize_AYUV },
    { MVFMT_UYVY, CalcBufferSize_UYVY },
    { MVFMT_YVYU, CalcBufferSize_YVYU },
    { MVFMT_VYUY, CalcBufferSize_VYUY },
    { MVFMT_YUY2, CalcBufferSize_YUY2 },
    { MVFMT_I420, CalcBufferSize_I420 },
    { MVFMT_YV12, CalcBufferSize_YV12 },
    { MVFMT_YVU9, CalcBufferSize_YVU9 },
    { MVFMT_YUV9, CalcBufferSize_YUV9 },
    { MVFMT_IYU1, CalcBufferSize_IYU1 },
    { MVFMT_IYU2, CalcBufferSize_IYU2 },
    { MVFMT_Y800, CalcBufferSize_Y800 },
    { MVFMT_Y16, CalcBufferSize_Y16 },
    { MVFMT_Y41P, CalcBufferSize_Y41P },
    { MVFMT_CLJR, CalcBufferSize_CLJR },
    { MVFMT_IMC1, CalcBufferSize_IMC1 },
    { MVFMT_IMC2, CalcBufferSize_IMC2 },
    { MVFMT_IMC3, CalcBufferSize_IMC3 },
    { MVFMT_IMC4, CalcBufferSize_IMC4 },
    { MVFMT_NV12, CalcBufferSize_NV12 },
    { MVFMT_NV21, CalcBufferSize_NV21 },
    { MVFMT_Y42T, CalcBufferSize_Y42T },
    { MVFMT_Y41T, CalcBufferSize_Y41T },
    { MVFMT_YV16, CalcBufferSize_YV16 }
};

map<MediaFormatID, t_flipverticalfunc> FlipVerticalMap = {
    { MVFMT_RGBA, FlipVertical_RGBA },
    { MVFMT_RGB32, FlipVertical_RGB32 },
    { MVFMT_RGB24, FlipVertical_RGB24 },
    { MVFMT_RGB565, FlipVertical_RGB565 },
    { MVFMT_RGB555, FlipVertical_RGB555 },
    { MVFMT_ARGB1555, FlipVertical_ARGB1555 },
    { MVFMT_YUY2, FlipVertical_YUY2 },
    { MVFMT_UYVY, FlipVertical_UYVY },
    { MVFMT_YVYU, FlipVertical_YVYU },
    { MVFMT_VYUY, FlipVertical_VYUY },
    { MVFMT_I420, FlipVertical_I420 },
    { MVFMT_YV12, FlipVertical_YV12 },
    { MVFMT_YVU9, FlipVertical_YVU9 },
    { MVFMT_YUV9, FlipVertical_YUV9 },
    { MVFMT_IYU1, FlipVertical_IYU1 },
    { MVFMT_IYU2, FlipVertical_IYU2 },
    { MVFMT_Y800, FlipVertical_Y800 },
    { MVFMT_Y16, FlipVertical_Y16 },
    { MVFMT_Y41P, FlipVertical_Y41P },
    { MVFMT_CLJR, FlipVertical_CLJR },
    { MVFMT_AYUV, FlipVertical_AYUV },
    { MVFMT_IMC1, FlipVertical_IMC1 },
    { MVFMT_IMC2, FlipVertical_IMC2 },
    { MVFMT_IMC3, FlipVertical_IMC3 },
    { MVFMT_IMC4, FlipVertical_IMC4 },
    { MVFMT_NV12, FlipVertical_NV12 },
    { MVFMT_NV21, FlipVertical_NV21 },
    { MVFMT_Y42T, FlipVertical_Y42T },
    { MVFMT_Y41T, FlipVertical_Y41T },
    { MVFMT_YV16, FlipVertical_YV16 }
};

VideoFormatInfo VideoFmtTable[] = {
    // Unpacked YUV formats:
    {MVFMT_AYUV, FOURCC_AYUV, FOURCC_UNDEFINED, 32, ColorspaceType::YUV, true},

    // Packed YUV Formats:

    // UYVY master format
    {MVFMT_UYVY, FOURCC_UYVY, FOURCC_UYVY, 16, ColorspaceType::YUV, false},
    {MVFMT_Y422, FOURCC_Y422, FOURCC_UYVY, 16, ColorspaceType::YUV, false},
    {MVFMT_UYNV, FOURCC_UYNV, FOURCC_UYVY, 16, ColorspaceType::YUV, false},
    {MVFMT_cyuv, FOURCC_cyuv, FOURCC_UYVY, 16, ColorspaceType::YUV, false},
    {MVFMT_V422, FOURCC_V422, FOURCC_UYVY, 16, ColorspaceType::YUV, false},
    {MVFMT_HDYC, FOURCC_HDYC, FOURCC_UYVY, 16, ColorspaceType::YUV, false},

    // YUY2 master format
    {MVFMT_YUY2, FOURCC_YUY2, FOURCC_YUY2, 16, ColorspaceType::YUV, false},
    {MVFMT_YUYV, FOURCC_YUYV, FOURCC_YUY2, 16, ColorspaceType::YUV, false},
    {MVFMT_YUNV, FOURCC_YUNV, FOURCC_YUY2, 16, ColorspaceType::YUV, false},

    // Y800 master format
    {MVFMT_Y800, FOURCC_Y800, FOURCC_Y800, 8, ColorspaceType::YUV, false},
    {MVFMT_Y8, FOURCC_Y8, FOURCC_Y800, 8, ColorspaceType::YUV, false},
    {MVFMT_GREY, FOURCC_GREY, FOURCC_Y800, 8, ColorspaceType::YUV, false},

    // Y16
    {MVFMT_Y16, FOURCC_Y16, FOURCC_UNDEFINED, 16, ColorspaceType::YUV, false},

    // IYU1 master format
    {MVFMT_IYU1, FOURCC_IYU1, FOURCC_IYU1, 12, ColorspaceType::YUV, false},
    {MVFMT_Y411, FOURCC_Y411, FOURCC_IYU1, 12, ColorspaceType::YUV, false},

    // IYU2 master format
    {MVFMT_IYU2, FOURCC_IYU2, FOURCC_IYU2, 24, ColorspaceType::YUV, false},
    {MVFMT_Y444, FOURCC_Y444, FOURCC_IYU2, 24, ColorspaceType::YUV, false},

    // Packed YUV formats with no known duplicate definitions
    {MVFMT_Y41P, FOURCC_Y41P, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},
    // Y41T is identical to Y41P except for the fact that the least significant bit of
    // each Y component forms a chromakey channel.
    {MVFMT_Y41T, FOURCC_Y41T, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, true},
    // Y42T is identical to UYVY except for the fact that the least significant bit of
    // each Y component forms a chromakey channel. 
    {MVFMT_Y42T, FOURCC_Y42T, FOURCC_UNDEFINED, 16, ColorspaceType::YUV, true},
    // IY41 is basically the same as Y41P with the exception that the data is interlaced. Lines are
    // ordered 0,2,4,....,1,3,5.... instead of 0,1,2,3,4,5,....
    {MVFMT_IY41, FOURCC_IY41, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},
    // IUYV is basically the same as UYVY with the exception that the data is interlaced. Lines are
    // ordered 0,2,4,....,1,3,5.... instead of 0,1,2,3,4,5,....
    {MVFMT_IUYV, FOURCC_IUYV, FOURCC_UNDEFINED, 16, ColorspaceType::YUV, false},
    {MVFMT_CLJR, FOURCC_CLJR, FOURCC_UNDEFINED,  8, ColorspaceType::YUV, false},
    {MVFMT_YUVP, FOURCC_YUVP, FOURCC_UNDEFINED, 24, ColorspaceType::YUV, false},
    {MVFMT_UYVP, FOURCC_UYVP, FOURCC_UNDEFINED, 24, ColorspaceType::YUV, false},
    {MVFMT_YVYU, FOURCC_YVYU, FOURCC_UNDEFINED, 16, ColorspaceType::YUV, false},
    {MVFMT_Y211, FOURCC_Y211, FOURCC_UNDEFINED,  8, ColorspaceType::YUV, false},
    {MVFMT_V655, FOURCC_V655, FOURCC_UNDEFINED, 16, ColorspaceType::YUV, false},
    {MVFMT_VYUY, FOURCC_VYUY, FOURCC_UNDEFINED, 16, ColorspaceType::YUV, false},

    // Planar YUV Formats:
    // I420 master format
    {MVFMT_I420, FOURCC_I420, FOURCC_I420, 12, ColorspaceType::YUV, false},
    {MVFMT_P420, FOURCC_P420, FOURCC_I420, 12, ColorspaceType::YUV, false},
    {MVFMT_I420, FOURCC_I420, FOURCC_I420, 12, ColorspaceType::YUV, false},
    {MVFMT_CLPL, FOURCC_CLPL, FOURCC_I420, 12, ColorspaceType::YUV, false},

    // Planar YUV with no known duplicate definitions
    {MVFMT_YV12, FOURCC_YV12, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},
    {MVFMT_YVU9, FOURCC_YVU9, FOURCC_UNDEFINED,  9, ColorspaceType::YUV, false},
    {MVFMT_YUV9, FOURCC_YUV9, FOURCC_UNDEFINED,  9, ColorspaceType::YUV, false},
    {MVFMT_IF09, FOURCC_IF09, FOURCC_UNDEFINED,  9, ColorspaceType::YUV, false},
    {MVFMT_IMC1, FOURCC_IMC1, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},
    {MVFMT_IMC2, FOURCC_IMC2, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},
    {MVFMT_IMC3, FOURCC_IMC3, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},
    {MVFMT_IMC4, FOURCC_IMC4, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},
    {MVFMT_NV12, FOURCC_NV12, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},
    {MVFMT_NV21, FOURCC_NV21, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},
    {MVFMT_YV16, FOURCC_YV16, FOURCC_UNDEFINED, 12, ColorspaceType::YUV, false},

    // RGB Formats:
    {MVFMT_RGB1, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 1, ColorspaceType::RGB, false},
    {MVFMT_RGB4, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 4, ColorspaceType::RGB, false},
    {MVFMT_RGB8, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 8, ColorspaceType::RGB, false},
    {MVFMT_RGB555, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 16, ColorspaceType::RGB, false},
    {MVFMT_ARGB1555, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 16, ColorspaceType::RGB, true},
    {MVFMT_RGB565, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 16, ColorspaceType::RGB, false},
    {MVFMT_RGB24, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 24, ColorspaceType::RGB, false},
    {MVFMT_RGB32, FOURCC_UNDEFINED, FOURCC_UNDEFINED, 32, ColorspaceType::RGB, false},
    {MVFMT_RGBA, FOURCC_RGBA, FOURCC_UNDEFINED, 32,  ColorspaceType::RGB, true},
    {MVFMT_RGBT, FOURCC_RGBT, FOURCC_UNDEFINED, -1, ColorspaceType::RGB, true},
    {MVFMT_RGB_BITFIELDS, FOURCC_BI_BITFIELDS, FOURCC_UNDEFINED, -1, ColorspaceType::RGB, false},

    {MVFMT_UNDEFINED, FOURCC_UNDEFINED, FOURCC_UNDEFINED, -1, ColorspaceType::Unknown, false}
};

map<MediaFormatID, t_stagetransformfunc> StagingMap = {
    { MVFMT_RGBA, Stage_RGBA },
    { MVFMT_RGB32, Stage_RGB32 },
    { MVFMT_RGB24, Stage_RGB24 },
    { MVFMT_RGB565, Stage_RGB565 },
    { MVFMT_RGB555, Stage_RGB555 },
    { MVFMT_ARGB1555, Stage_ARGB1555 },
    { MVFMT_RGB8, Stage_RGB8 },
    { MVFMT_RGB4, Stage_RGB4 },
    { MVFMT_RGB1, Stage_RGB1 },
    { MVFMT_YUY2, Stage_YUY2 },
    { MVFMT_UYVY, Stage_UYVY },
    { MVFMT_YVYU, Stage_YVYU },
    { MVFMT_VYUY, Stage_VYUY },
    { MVFMT_I420, Stage_I420 },
    { MVFMT_YV12, Stage_YV12 },
    { MVFMT_YVU9, Stage_YVU9 },
    { MVFMT_YUV9, Stage_YUV9 },
    { MVFMT_IYU1, Stage_IYU1 },
    { MVFMT_IYU2, Stage_IYU2 },
    { MVFMT_Y41P, Stage_Y41P },
    { MVFMT_CLJR, Stage_CLJR },
    { MVFMT_Y800, Stage_Y800 },
    { MVFMT_Y16, Stage_Y16 },
    { MVFMT_AYUV, Stage_AYUV },
    { MVFMT_IMC1, Stage_IMC1 },
    { MVFMT_IMC2, Stage_IMC2 },
    { MVFMT_IMC3, Stage_IMC3 },
    { MVFMT_IMC4, Stage_IMC4 },
    { MVFMT_NV12, Stage_NV12 },
    { MVFMT_NV21, Stage_NV21 },
    { MVFMT_Y42T, Stage_Y42T },
    { MVFMT_Y41T, Stage_Y41T },
    { MVFMT_YV16, Stage_YV16 }
};

map<MediaFormatID, VideoFormatInfo*> MediaFormatInfoMap;
map<Fourcc, const MediaFormatID> FourccToIDMap;

bool blipvert::IsInitialized = false;
bool blipvert::UseFasterLooping = false;

void blipvert::InitializeLibrary(void)
{
    InitLookupTables();

    uint16_t index = 0;
    while (VideoFmtTable[index].formatId != MVFMT_UNDEFINED)
    {
        MediaFormatInfoMap.insert(make_pair(VideoFmtTable[index].formatId, &VideoFmtTable[index]));

        if (VideoFmtTable[index].fourcc != FOURCC_UNDEFINED)
        {
            FourccToIDMap.insert(make_pair(VideoFmtTable[index].fourcc, VideoFmtTable[index].formatId));
        }

        index++;
    }

    IsInitialized = true;
}

bool blipvert::get_UseFasterLooping()
{
    return UseFasterLooping;
}

void blipvert::set_UseFasterLooping(bool value)
{
    UseFasterLooping = value;
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

    VideoFormatInfo inInfo;
    VideoFormatInfo outInfo;
    if (GetVideoFormatInfo(inFormat, inInfo) &&
        GetVideoFormatInfo(outFormat, outInfo))
    {
        MediaFormatID inid;
        MediaFormatID outid;
        if (GetVideoFormatID(inInfo.xRefFourcc, inid) && GetVideoFormatID(outInfo.xRefFourcc, outid))
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

t_greyscalefunc blipvert::FindGreyscaleTransform(const MediaFormatID& inFormat)
{
    map<MediaFormatID, t_greyscalefunc>::iterator it = GreyscaleMap.find(inFormat);
    if (it != GreyscaleMap.end())
    {
        return *(it->second);
    }

    // Not found, so try cross-referenced formats in case there's a known duplicate definition.

    VideoFormatInfo inInfo;
    if (GetVideoFormatInfo(inFormat, inInfo))
    {
        MediaFormatID inid;
        if (GetVideoFormatID(inInfo.xRefFourcc, inid))
        {
            map<MediaFormatID, t_greyscalefunc>::iterator it = GreyscaleMap.find(inid);
            if (it != GreyscaleMap.end())
            {
                return *(it->second);
            }
        }
    }

    return nullptr;
}

t_fillcolorfunc blipvert::FindFillColorTransform(const MediaFormatID& inFormat)
{
    map<MediaFormatID, t_fillcolorfunc>::iterator it = FillColorMap.find(inFormat);
    if (it != FillColorMap.end())
    {
        return *(it->second);
    }

    // Not found, so try cross-referenced formats in case there's a known duplicate definition.

    VideoFormatInfo inInfo;
    if (GetVideoFormatInfo(inFormat, inInfo))
    {
        MediaFormatID inid;
        if (GetVideoFormatID(inInfo.xRefFourcc, inid))
        {
            map<MediaFormatID, t_fillcolorfunc>::iterator it = FillColorMap.find(inid);
            if (it != FillColorMap.end())
            {
                return *(it->second);
            }
        }
    }

    return nullptr;
}

t_setpixelfunc blipvert::FindSetPixelColor(const MediaFormatID& inFormat)
{
    map<MediaFormatID, t_setpixelfunc>::iterator it = SetPixelMap.find(inFormat);
    if (it != SetPixelMap.end())
    {
        return *(it->second);
    }

    // Not found, so try cross-referenced formats in case there's a known duplicate definition.

    VideoFormatInfo inInfo;
    if (GetVideoFormatInfo(inFormat, inInfo))
    {
        MediaFormatID inid;
        if (GetVideoFormatID(inInfo.xRefFourcc, inid))
        {
            map<MediaFormatID, t_setpixelfunc>::iterator it = SetPixelMap.find(inid);
            if (it != SetPixelMap.end())
            {
                return *(it->second);
            }
        }
    }

    return nullptr;
}

t_flipverticalfunc blipvert::FindFlipVerticalTransform(const MediaFormatID& inFormat)
{
    map<MediaFormatID, t_flipverticalfunc>::iterator it = FlipVerticalMap.find(inFormat);
    if (it != FlipVerticalMap.end())
    {
        return *(it->second);
    }

    // Not found, so try cross-referenced formats in case there's a known duplicate definition.

    VideoFormatInfo inInfo;
    if (GetVideoFormatInfo(inFormat, inInfo))
    {
        MediaFormatID inid;
        if (GetVideoFormatID(inInfo.xRefFourcc, inid))
        {
            map<MediaFormatID, t_flipverticalfunc>::iterator it = FlipVerticalMap.find(inid);
            if (it != FlipVerticalMap.end())
            {
                return *(it->second);
            }
        }
    }

    return nullptr;
}

t_calcbuffsizefunc blipvert::FindBufSizeCalculator(const MediaFormatID& inFormat)
{
    map<MediaFormatID, t_calcbuffsizefunc>::iterator it = CalcBufSizeMap.find(inFormat);
    if (it != CalcBufSizeMap.end())
    {
        return *(it->second);
    }

    // Not found, so try cross-referenced formats in case there's a known duplicate definition.

    VideoFormatInfo inInfo;
    if (GetVideoFormatInfo(inFormat, inInfo))
    {
        MediaFormatID inid;
        if (GetVideoFormatID(inInfo.xRefFourcc, inid))
        {
            map<MediaFormatID, t_calcbuffsizefunc>::iterator it = CalcBufSizeMap.find(inid);
            if (it != CalcBufSizeMap.end())
            {
                return *(it->second);
            }
        }
    }

    return nullptr;
}

bool blipvert::GetVideoFormatInfo(const MediaFormatID& format, VideoFormatInfo& info)
{
    map<MediaFormatID, VideoFormatInfo*>::iterator it = MediaFormatInfoMap.find(format);
    if (it != MediaFormatInfoMap.end())
    {
        info.fourcc = it->second->fourcc;
        info.xRefFourcc = it->second->xRefFourcc;
        info.effectiveBitsPerPixel = it->second->effectiveBitsPerPixel;
        info.type = it->second->type;
        info.hasAlpha = it->second->hasAlpha;

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

t_stagetransformfunc blipvert::FindTransformStage(const MediaFormatID& format)
{
    map<MediaFormatID, t_stagetransformfunc>::iterator it = StagingMap.find(format);
    if (it != StagingMap.end())
    {
        return *(it->second);
    }

    // Not found, so try cross-referenced formats in case there's a known duplicate definition.

    VideoFormatInfo inInfo;
    if (GetVideoFormatInfo(format, inInfo))
    {
        MediaFormatID inid;
        if (GetVideoFormatID(inInfo.xRefFourcc, inid))
        {
            map<MediaFormatID, t_stagetransformfunc>::iterator it = StagingMap.find(inid);
            if (it != StagingMap.end())
            {
                return *(it->second);
            }
        }
    }

    return nullptr;
}
