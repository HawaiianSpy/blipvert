#pragma once

#include "legacydefs.h"

namespace blipvert
{
    extern void InitYUVtoRGBTables(void);

    // YUV (YCbCr) to RGB conversions
    void PackedY422_to_RGB32(int32_t width, int32_t height,
        uint8_t* out_buf, int32_t out_stride,
        uint8_t* in_buf, int32_t in_stride,
        int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped);

    void PackedY422_to_RGB24(int32_t width, int32_t height,
        uint8_t* out_buf, int32_t out_stride,
        uint8_t* in_buf, int32_t in_stride,
        int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped);

    void PackedY422_to_RGB565(int32_t width, int32_t height,
        uint8_t* out_buf, int32_t out_stride,
        uint8_t* in_buf, int32_t in_stride,
        int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped);

    void PackedY422_to_RGB555(int32_t width, int32_t height,
        uint8_t* out_buf, int32_t out_stride,
        uint8_t* in_buf, int32_t in_stride,
        int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped);

    void YUY2_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YUY2_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YUY2_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YUY2_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void UYVY_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void UYVY_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void UYVY_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void UYVY_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void YVYU_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YVYU_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YVYU_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YVYU_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void VYUY_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void VYUY_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void VYUY_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void VYUY_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void CLJR_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void CLJR_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void CLJR_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void CLJR_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void Y41P_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void Y41P_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void Y41P_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void Y41P_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void IYU1_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void IYU1_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void IYU1_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void IYU1_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void IYU2_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void IYU2_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void IYU2_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void IYU2_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void PlanarYUV_to_RGB32(int32_t width, int32_t height,
        uint8_t* out_buf, int32_t out_stride,
        uint8_t* in_buf, int32_t in_stride,
        bool uFirst, int32_t decimation, bool flipped);

    void PlanarYUV_to_RGB24(int32_t width, int32_t height,
        uint8_t* out_buf, int32_t out_stride,
        uint8_t* in_buf, int32_t in_stride,
        bool uFirst, int32_t decimation, bool flipped);

    void PlanarYUV_to_RGB565(int32_t width, int32_t height,
        uint8_t* out_buf, int32_t out_stride,
        uint8_t* in_buf, int32_t in_stride,
        bool uFirst, int32_t decimation, bool flipped);

    void PlanarYUV_to_RGB555(int32_t width, int32_t height,
        uint8_t* out_buf, int32_t out_stride,
        uint8_t* in_buf, int32_t in_stride,
        bool uFirst, int32_t decimation, bool flipped);

    void YVU9_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YVU9_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YVU9_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YVU9_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void YUV9_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YUV9_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YUV9_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YUV9_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void IYUV_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void IYUV_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void IYUV_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void IYUV_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void YV12_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YV12_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YV12_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void YV12_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);

    void Y800_to_RGB32(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void Y800_to_RGB24(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void Y800_to_RGB565(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
    void Y800_to_RGB555(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped = false, RGBPalette* in_palette = 0);
}

