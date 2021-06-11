// blipvert.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "blipvert.h"
#include "CommonMacros.h"

#include "YUVtoRGB.h"

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

extern void blipvert::InitializeLibrary(void)
{
    InitYUVtoRGBTables();
}
