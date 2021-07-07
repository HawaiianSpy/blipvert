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
#include "RGBtoYUV.h"
#include "CommonMacros.h"
#include "LookupTables.h"

using namespace blipvert;

//
// RGBX to PackedY422 transforms
//

void RGB32_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[Y0] = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            pdst[Y1] = static_cast<uint8_t>(((yr_table[psrc[6]] + yg_table[psrc[5]] + yb_table[psrc[4]]) >> 15) + 16);
            pdst[U] = static_cast<uint8_t>(((((ur_table[psrc[2]] + ug_table[psrc[1]] + ub_table[psrc[0]]) >> 15) + 128) + \
                (((ur_table[psrc[6]] + ug_table[psrc[5]] + ub_table[psrc[4]]) >> 15) + 128)) / 2);
            pdst[V] = static_cast<uint8_t>(((((vr_table[psrc[2]] + vg_table[psrc[1]] + vb_table[psrc[0]]) >> 15) + 128) + \
                (((vr_table[psrc[6]] + vg_table[psrc[5]] + vb_table[psrc[4]]) >> 15) + 128)) / 2);
            psrc += 8;
            pdst += 4;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void RGB24_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[Y0] = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            pdst[Y1] = static_cast<uint8_t>(((yr_table[psrc[5]] + yg_table[psrc[4]] + yb_table[psrc[3]]) >> 15) + 16);
            pdst[U] = static_cast<uint8_t>(((((ur_table[psrc[2]] + ug_table[psrc[1]] + ub_table[psrc[0]]) >> 15) + 128) + \
                (((ur_table[psrc[5]] + ug_table[psrc[4]] + ub_table[psrc[3]]) >> 15) + 128)) / 2);
            pdst[V] = static_cast<uint8_t>(((((vr_table[psrc[2]] + vg_table[psrc[1]] + vb_table[psrc[0]]) >> 15) + 128) + \
                (((vr_table[psrc[5]] + vg_table[psrc[4]] + vb_table[psrc[3]]) >> 15) + 128)) / 2);
            psrc += 6;
            pdst += 4;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;

        height--;
    }
}

void RGB565_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped)
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

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red1;
            uint8_t green1;
            uint8_t blue1;
            UnpackRGB565Word(psrc[0], red1, green1, blue1)

                uint8_t red2;
            uint8_t green2;
            uint8_t blue2;
            UnpackRGB565Word(psrc[1], red2, green2, blue2)

                pdst[Y0] = static_cast<uint8_t>(((yr_table[red1] + yg_table[green1] + yb_table[blue1]) >> 15) + 16);
            pdst[Y1] = static_cast<uint8_t>(((yr_table[red2] + yg_table[green2] + yb_table[blue2]) >> 15) + 16);
            pdst[U] = static_cast<uint8_t>(((((ur_table[red2] + ug_table[green2] + ub_table[blue2]) >> 15) + 128) + \
                (((ur_table[red1] + ug_table[green1] + ub_table[blue1]) >> 15) + 128)) / 2);
            pdst[V] = static_cast<uint8_t>(((((vr_table[red2] + vg_table[green2] + vb_table[blue2]) >> 15) + 128) + \
                (((vr_table[red1] + vg_table[green1] + vb_table[blue1]) >> 15) + 128)) / 2);
            psrc += 2;
            pdst += 4;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void RGB555_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped)
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

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red1;
            uint8_t green1;
            uint8_t blue1;
            UnpackRGB555Word(psrc[0], red1, green1, blue1)

                uint8_t red2;
            uint8_t green2;
            uint8_t blue2;
            UnpackRGB555Word(psrc[1], red2, green2, blue2)

                pdst[Y0] = static_cast<uint8_t>(((yr_table[red1] + yg_table[green1] + yb_table[blue1]) >> 15) + 16);
            pdst[Y1] = static_cast<uint8_t>(((yr_table[red2] + yg_table[green2] + yb_table[blue2]) >> 15) + 16);
            pdst[U] = static_cast<uint8_t>(((((ur_table[red2] + ug_table[green2] + ub_table[blue2]) >> 15) + 128) + \
                (((ur_table[red1] + ug_table[green1] + ub_table[blue1]) >> 15) + 128)) / 2);
            pdst[V] = static_cast<uint8_t>(((((vr_table[red2] + vg_table[green2] + vb_table[blue2]) >> 15) + 128) + \
                (((vr_table[red1] + vg_table[green1] + vb_table[blue1]) >> 15) + 128)) / 2);
            psrc += 2;
            pdst += 4;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void RGB8_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride, xRGBQUAD* in_palette,
    int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red1 = in_palette[*psrc].rgbRed;
            uint8_t green1 = in_palette[*psrc].rgbGreen;
            uint8_t blue1 = in_palette[*psrc++].rgbBlue;

            uint8_t red2 = in_palette[*psrc].rgbRed;
            uint8_t green2 = in_palette[*psrc].rgbGreen;
            uint8_t blue2 = in_palette[*psrc++].rgbBlue;

            pdst[Y0] = static_cast<uint8_t>(((yr_table[red1] + yg_table[green1] + yb_table[blue1]) >> 15) + 16);
            pdst[Y1] = static_cast<uint8_t>(((yr_table[red2] + yg_table[green2] + yb_table[blue2]) >> 15) + 16);
            pdst[U] = static_cast<uint8_t>(((((ur_table[red2] + ug_table[green2] + ub_table[blue2]) >> 15) + 128) + \
                (((ur_table[red1] + ug_table[green1] + ub_table[blue1]) >> 15) + 128)) / 2);
            pdst[V] = static_cast<uint8_t>(((((vr_table[red2] + vg_table[green2] + vb_table[blue2]) >> 15) + 128) + \
                (((vr_table[red1] + vg_table[green1] + vb_table[blue1]) >> 15) + 128)) / 2);
            pdst += 4;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// RGBx to PlanarYUV
//

void RGB32_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
    if (!in_stride)
        in_stride = width * 4;

    int32_t uv_width = width / decimation;
    int32_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!out_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = out_stride;
        uv_stride = out_stride;
    }

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = out_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = out_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (y_stride * (height - 1));
        uplane += (uv_stride * (uv_height - 1));
        vplane += (uv_stride * (uv_height - 1));
        y_stride = -y_stride;
        uv_stride = -uv_stride;
    }

    uint16_t red;
    uint16_t green;
    uint16_t blue;

    if (decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 2)
            {
                yp[0] = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
                red = psrc[2];
                green = psrc[1];
                blue = psrc[0];

                yp[1] = static_cast<uint8_t>(((yr_table[psrc[6]] + yg_table[psrc[5]] + yb_table[psrc[4]]) >> 15) + 16);
                red += psrc[6];
                green += psrc[5];
                blue += psrc[4];

                yp[y_stride] = static_cast<uint8_t>(((yr_table[psrc[2 + in_stride]] + yg_table[psrc[1 + in_stride]] + yb_table[psrc[0 + in_stride]]) >> 15) + 16);
                red += psrc[2 + in_stride];
                green += psrc[1 + in_stride];
                blue += psrc[0 + in_stride];

                yp[y_stride + 1] = static_cast<uint8_t>(((yr_table[psrc[6 + in_stride]] + yg_table[psrc[5 + in_stride]] + yb_table[psrc[4 + in_stride]]) >> 15) + 16);
                red += psrc[6 + in_stride];
                green += psrc[5 + in_stride];
                blue += psrc[4 + in_stride];

                red >>= 2;
                green >>= 2;
                blue >>= 2;

                uplane[x >> 1] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 1] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 8;
                yp += 2;
            }
            in_buf += (in_stride * 2);
            out_buf += (y_stride * 2);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 4)
            {
                red = 0;
                green = 0;
                blue = 0;

                int32_t is = 0;
                int32_t ys = 0;

                for (int32_t dec = 0; dec < 4; dec++)
                {
                    yp[ys] = static_cast<uint8_t>(((yr_table[psrc[2 + is]] + yg_table[psrc[1 + is]] + yb_table[psrc[is]]) >> 15) + 16);
                    red += psrc[2 + is];
                    green += psrc[1 + is];
                    blue += psrc[is];

                    yp[ys + 1] = static_cast<uint8_t>(((yr_table[psrc[6 + is]] + yg_table[psrc[5 + is]] + yb_table[psrc[4 + is]]) >> 15) + 16);
                    red += psrc[6 + is];
                    green += psrc[5 + is];
                    blue += psrc[4 + is];

                    yp[ys + 2] = static_cast<uint8_t>(((yr_table[psrc[10 + is]] + yg_table[psrc[9 + is]] + yb_table[psrc[8 + is]]) >> 15) + 16);
                    red += psrc[10 + is];
                    green += psrc[9 + is];
                    blue += psrc[8 + is];

                    yp[ys + 3] = static_cast<uint8_t>(((yr_table[psrc[14 + is]] + yg_table[psrc[13 + is]] + yb_table[psrc[12 + is]]) >> 15) + 16);
                    red += psrc[14 + is];
                    green += psrc[13 + is];
                    blue += psrc[12 + is];

                    is += in_stride;
                    ys += y_stride;
                }

                red >>= 4;
                green >>= 4;
                blue >>= 4;

                uplane[x >> 2] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 16;
                yp += 4;
            }
            in_buf += (in_stride * 4);
            out_buf += (y_stride * 4);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

void RGB24_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
    if (!in_stride)
        in_stride = width * 3;

    int32_t uv_width = width / decimation;
    int32_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!out_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = out_stride;
        uv_stride = out_stride;
    }

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = out_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = out_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (y_stride * (height - 1));
        uplane += (uv_stride * (uv_height - 1));
        vplane += (uv_stride * (uv_height - 1));
        y_stride = -y_stride;
        uv_stride = -uv_stride;
    }

    uint16_t red;
    uint16_t green;
    uint16_t blue;

    if (decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 2)
            {
                yp[0] = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
                red = psrc[2];
                green = psrc[1];
                blue = psrc[0];

                yp[1] = static_cast<uint8_t>(((yr_table[psrc[5]] + yg_table[psrc[4]] + yb_table[psrc[3]]) >> 15) + 16);
                red += psrc[5];
                green += psrc[4];
                blue += psrc[3];

                yp[y_stride] = static_cast<uint8_t>(((yr_table[psrc[2 + in_stride]] + yg_table[psrc[1 + in_stride]] + yb_table[psrc[0 + in_stride]]) >> 15) + 16);
                red += psrc[2 + in_stride];
                green += psrc[1 + in_stride];
                blue += psrc[0 + in_stride];

                yp[y_stride + 1] = static_cast<uint8_t>(((yr_table[psrc[5 + in_stride]] + yg_table[psrc[4 + in_stride]] + yb_table[psrc[3 + in_stride]]) >> 15) + 16);
                red += psrc[5 + in_stride];
                green += psrc[4 + in_stride];
                blue += psrc[3 + in_stride];

                red >>= 2;
                green >>= 2;
                blue >>= 2;

                uplane[x >> 1] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 1] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 6;
                yp += 2;
            }
            in_buf += (in_stride * 2);
            out_buf += (y_stride * 2);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 4)
            {
                red = 0;
                green = 0;
                blue = 0;

                int32_t is = 0;
                int32_t ys = 0;

                for (int32_t dec = 0; dec < 4; dec++)
                {
                    yp[ys] = static_cast<uint8_t>(((yr_table[psrc[2 + is]] + yg_table[psrc[1 + is]] + yb_table[psrc[is]]) >> 15) + 16);
                    red += psrc[2 + is];
                    green += psrc[1 + is];
                    blue += psrc[is];

                    yp[ys + 1] = static_cast<uint8_t>(((yr_table[psrc[5 + is]] + yg_table[psrc[4 + is]] + yb_table[psrc[3 + is]]) >> 15) + 16);
                    red += psrc[5 + is];
                    green += psrc[4 + is];
                    blue += psrc[3 + is];

                    yp[ys + 2] = static_cast<uint8_t>(((yr_table[psrc[8 + is]] + yg_table[psrc[7 + is]] + yb_table[psrc[6 + is]]) >> 15) + 16);
                    red += psrc[8 + is];
                    green += psrc[7 + is];
                    blue += psrc[6 + is];

                    yp[ys + 3] = static_cast<uint8_t>(((yr_table[psrc[11 + is]] + yg_table[psrc[10 + is]] + yb_table[psrc[9 + is]]) >> 15) + 16);
                    red += psrc[11 + is];
                    green += psrc[10 + is];
                    blue += psrc[9 + is];

                    is += in_stride;
                    ys += y_stride;
                }

                red >>= 4;
                green >>= 4;
                blue >>= 4;

                uplane[x >> 2] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 12;
                yp += 4;
            }
            in_buf += (in_stride * 4);
            out_buf += (y_stride * 4);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

void RGB565_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
    if (!in_stride)
        in_stride = width * 2;

    int32_t uv_width = width / decimation;
    int32_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!out_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = out_stride;
        uv_stride = out_stride;
    }

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = out_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = out_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (y_stride * (height - 1));
        uplane += (uv_stride * (uv_height - 1));
        vplane += (uv_stride * (uv_height - 1));
        y_stride = -y_stride;
        uv_stride = -uv_stride;
    }

    uint16_t red;
    uint16_t green;
    uint16_t blue;

    if (decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 2)
            {
                uint8_t redb;
                uint8_t greenb;
                uint8_t blueb;
                uint16_t pixel = *(reinterpret_cast<uint16_t*>(psrc));
                UnpackRGB565Word(pixel, redb, greenb, blueb)
                    yp[0] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red = redb;
                green = greenb;
                blue = blueb;

                pixel = *(reinterpret_cast<uint16_t*>(psrc + 2));
                UnpackRGB565Word(pixel, redb, greenb, blueb)
                    yp[1] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red += redb;
                green += greenb;
                blue += blueb;

                pixel = *(reinterpret_cast<uint16_t*>(psrc + in_stride));
                UnpackRGB565Word(pixel, redb, greenb, blueb)
                    yp[y_stride] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red += redb;
                green += greenb;
                blue += blueb;

                pixel = *(reinterpret_cast<uint16_t*>(psrc + in_stride + 2));
                UnpackRGB565Word(pixel, redb, greenb, blueb)
                    yp[y_stride + 1] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red += redb;
                green += greenb;
                blue += blueb;

                red >>= 2;
                green >>= 2;
                blue >>= 2;

                uplane[x >> 1] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 1] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 4;
                yp += 2;
            }
            in_buf += (in_stride * 2);
            out_buf += (y_stride * 2);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 4)
            {
                red = 0;
                green = 0;
                blue = 0;

                int32_t is = 0;
                int32_t ys = 0;

                for (int32_t dec = 0; dec < 4; dec++)
                {
                    uint8_t redb;
                    uint8_t greenb;
                    uint8_t blueb;

                    uint16_t pixel = *(reinterpret_cast<uint16_t*>(psrc + is));
                    UnpackRGB565Word(pixel, redb, greenb, blueb)
                        yp[ys] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    pixel = *(reinterpret_cast<uint16_t*>(psrc + is + 2));
                    UnpackRGB565Word(pixel, redb, greenb, blueb)
                        yp[ys + 1] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    pixel = *(reinterpret_cast<uint16_t*>(psrc + is + 4));
                    UnpackRGB565Word(pixel, redb, greenb, blueb)
                        yp[ys + 2] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    pixel = *(reinterpret_cast<uint16_t*>(psrc + is + 6));
                    UnpackRGB565Word(pixel, redb, greenb, blueb)
                        yp[ys + 3] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    is += in_stride;
                    ys += y_stride;
                }

                red >>= 4;
                green >>= 4;
                blue >>= 4;

                uplane[x >> 2] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 8;
                yp += 4;
            }
            in_buf += (in_stride * 4);
            out_buf += (y_stride * 4);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

void RGB555_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
    if (!in_stride)
        in_stride = width * 2;

    int32_t uv_width = width / decimation;
    int32_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!out_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = out_stride;
        uv_stride = out_stride;
    }

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = out_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = out_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (y_stride * (height - 1));
        uplane += (uv_stride * (uv_height - 1));
        vplane += (uv_stride * (uv_height - 1));
        y_stride = -y_stride;
        uv_stride = -uv_stride;
    }

    uint16_t red;
    uint16_t green;
    uint16_t blue;

    if (decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 2)
            {
                uint8_t redb;
                uint8_t greenb;
                uint8_t blueb;
                uint16_t pixel = *(reinterpret_cast<uint16_t*>(psrc));
                UnpackRGB555Word(pixel, redb, greenb, blueb)
                    yp[0] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red = redb;
                green = greenb;
                blue = blueb;

                pixel = *(reinterpret_cast<uint16_t*>(psrc + 2));
                UnpackRGB555Word(pixel, redb, greenb, blueb)
                    yp[1] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red += redb;
                green += greenb;
                blue += blueb;

                pixel = *(reinterpret_cast<uint16_t*>(psrc + in_stride));
                UnpackRGB555Word(pixel, redb, greenb, blueb)
                    yp[y_stride] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red += redb;
                green += greenb;
                blue += blueb;

                pixel = *(reinterpret_cast<uint16_t*>(psrc + in_stride + 2));
                UnpackRGB555Word(pixel, redb, greenb, blueb)
                    yp[y_stride + 1] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red += redb;
                green += greenb;
                blue += blueb;

                red >>= 2;
                green >>= 2;
                blue >>= 2;

                uplane[x >> 1] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 1] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 4;
                yp += 2;
            }
            in_buf += (in_stride * 2);
            out_buf += (y_stride * 2);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 4)
            {
                red = 0;
                green = 0;
                blue = 0;

                int32_t is = 0;
                int32_t ys = 0;

                for (int32_t dec = 0; dec < 4; dec++)
                {
                    uint8_t redb;
                    uint8_t greenb;
                    uint8_t blueb;

                    uint16_t pixel = *(reinterpret_cast<uint16_t*>(psrc + is));
                    UnpackRGB555Word(pixel, redb, greenb, blueb)
                        yp[ys] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    pixel = *(reinterpret_cast<uint16_t*>(psrc + is + 2));
                    UnpackRGB555Word(pixel, redb, greenb, blueb)
                        yp[ys + 1] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    pixel = *(reinterpret_cast<uint16_t*>(psrc + is + 4));
                    UnpackRGB555Word(pixel, redb, greenb, blueb)
                        yp[ys + 2] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    pixel = *(reinterpret_cast<uint16_t*>(psrc + is + 6));
                    UnpackRGB555Word(pixel, redb, greenb, blueb)
                        yp[ys + 3] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    is += in_stride;
                    ys += y_stride;
                }

                red >>= 4;
                green >>= 4;
                blue >>= 4;

                uplane[x >> 2] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 8;
                yp += 4;
            }
            in_buf += (in_stride * 4);
            out_buf += (y_stride * 4);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

void RGB8_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride, xRGBQUAD* in_palette,
    bool uFirst, int32_t decimation, bool flipped)
{
    if (!in_stride)
        in_stride = width;

    int32_t uv_width = width / decimation;
    int32_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!out_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = out_stride;
        uv_stride = out_stride;
    }

    if (in_palette == nullptr)
        in_palette = rgb8_greyscale_palette;

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = out_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = out_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (y_stride * (height - 1));
        uplane += (uv_stride * (uv_height - 1));
        vplane += (uv_stride * (uv_height - 1));
        y_stride = -y_stride;
        uv_stride = -uv_stride;
    }

    uint16_t red;
    uint16_t green;
    uint16_t blue;

    if (decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 2)
            {
                uint8_t redb = in_palette[psrc[0]].rgbRed;
                uint8_t greenb = in_palette[psrc[0]].rgbGreen;
                uint8_t blueb = in_palette[psrc[0]].rgbBlue;
                yp[0] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red = redb;
                green = greenb;
                blue = blueb;

                redb = in_palette[psrc[1]].rgbRed;
                greenb = in_palette[psrc[1]].rgbGreen;
                blueb = in_palette[psrc[1]].rgbBlue;
                yp[1] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red += redb;
                green += greenb;
                blue += blueb;

                redb = in_palette[psrc[in_stride]].rgbRed;
                greenb = in_palette[psrc[in_stride]].rgbGreen;
                blueb = in_palette[psrc[in_stride]].rgbBlue;
                yp[y_stride] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red += redb;
                green += greenb;
                blue += blueb;

                redb = in_palette[psrc[in_stride + 1]].rgbRed;
                greenb = in_palette[psrc[in_stride + 1]].rgbGreen;
                blueb = in_palette[psrc[in_stride + 1]].rgbBlue;
                yp[y_stride + 1] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                red += redb;
                green += greenb;
                blue += blueb;

                red >>= 2;
                green >>= 2;
                blue >>= 2;

                uplane[x >> 1] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 1] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 2;
                yp += 2;
            }
            in_buf += (in_stride * 2);
            out_buf += (y_stride * 2);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 4)
            {
                red = 0;
                green = 0;
                blue = 0;

                int32_t is = 0;
                int32_t ys = 0;

                for (int32_t dec = 0; dec < 4; dec++)
                {
                    uint8_t redb = in_palette[psrc[is]].rgbRed;
                    uint8_t greenb = in_palette[psrc[is]].rgbGreen;
                    uint8_t blueb = in_palette[psrc[is]].rgbBlue;
                    yp[ys] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    redb = in_palette[psrc[is + 1]].rgbRed;
                    greenb = in_palette[psrc[is + 1]].rgbGreen;
                    blueb = in_palette[psrc[is + 1]].rgbBlue;
                    yp[ys + 1] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    redb = in_palette[psrc[is + 2]].rgbRed;
                    greenb = in_palette[psrc[is + 2]].rgbGreen;
                    blueb = in_palette[psrc[is + 2]].rgbBlue;
                    yp[ys + 2] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    redb = in_palette[psrc[is + 3]].rgbRed;
                    greenb = in_palette[psrc[is + 3]].rgbGreen;
                    blueb = in_palette[psrc[is + 3]].rgbBlue;
                    yp[ys + 3] = static_cast<uint8_t>(((yr_table[redb] + yg_table[greenb] + yb_table[blueb]) >> 15) + 16);
                    red += redb;
                    green += greenb;
                    blue += blueb;

                    is += in_stride;
                    ys += y_stride;
                }

                red >>= 4;
                green >>= 4;
                blue >>= 4;

                uplane[x >> 2] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
                vplane[x >> 2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

                psrc += 4;
                yp += 4;
            }
            in_buf += (in_stride * 4);
            out_buf += (y_stride * 4);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

//
// Public RGBA to YUV transforms
//

void blipvert::RGBA_to_AYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = static_cast<uint32_t>(((vr_table[psrc[2]] + vg_table[psrc[1]] + vb_table[psrc[0]]) >> 15) + 128);
            *pdst++ = static_cast<uint32_t>(((ur_table[psrc[2]] + ug_table[psrc[1]] + ub_table[psrc[0]]) >> 15) + 128);
            *pdst++ = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            *pdst++ = psrc[3];
            psrc += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}


//
// Public RGB32 to YUV transforms
//

void blipvert::RGB32_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB32_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

void blipvert::RGB32_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB32_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

void blipvert::RGB32_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB32_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

void blipvert::RGB32_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB32_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

void blipvert::RGB32_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB32_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, true);
}

void blipvert::RGB32_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB32_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

void blipvert::RGB32_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB32_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

void blipvert::RGB32_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB32_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

void blipvert::RGB32_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = (static_cast<int32_t>(psrc[0]) + static_cast<int32_t>(psrc[4]) + \
                static_cast<int32_t>(psrc[8]) + static_cast<int32_t>(psrc[12])) >> 2;
            int32_t green = (static_cast<int32_t>(psrc[1]) + static_cast<int32_t>(psrc[5]) + \
                static_cast<int32_t>(psrc[9]) + static_cast<int32_t>(psrc[13])) >> 2;
            int32_t red = (static_cast<int32_t>(psrc[2]) + static_cast<int32_t>(psrc[6]) + \
                static_cast<int32_t>(psrc[10]) + static_cast<int32_t>(psrc[14])) >> 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            pdst[3] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((yr_table[psrc[6]] + yg_table[psrc[5]] + yb_table[psrc[4]]) >> 15) + 16);
            pdst[4] = static_cast<uint8_t>(((yr_table[psrc[10]] + yg_table[psrc[9]] + yb_table[psrc[8]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[psrc[14]] + yg_table[psrc[13]] + yb_table[psrc[12]]) >> 15) + 16);

            psrc += 16;
            pdst += 6;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB32_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red = psrc[2];
            uint8_t green = psrc[1];
            uint8_t blue = psrc[0];
            pdst[0] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            pdst[1] = static_cast<uint8_t>(((yr_table[red] + yg_table[green] + yb_table[blue]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

            psrc += 4;
            pdst += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB32_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width / 8 * 12;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t bluea = psrc[0];
            int16_t greena = psrc[1];
            int16_t reda = psrc[2];

            bluea += psrc[4];
            greena += psrc[5];
            reda += psrc[6];

            bluea += psrc[8];
            greena += psrc[9];
            reda += psrc[10];

            bluea += psrc[12];
            greena += psrc[13];
            reda += psrc[14];

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[2] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            pdst[3] = static_cast<uint8_t>(((yr_table[psrc[6]] + yg_table[psrc[5]] + yb_table[psrc[4]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[psrc[10]] + yg_table[psrc[9]] + yb_table[psrc[8]]) >> 15) + 16);
            pdst[7] = static_cast<uint8_t>(((yr_table[psrc[14]] + yg_table[psrc[13]] + yb_table[psrc[12]]) >> 15) + 16);

            bluea = psrc[16];
            greena = psrc[17];
            reda = psrc[18];

            bluea += psrc[20];
            greena += psrc[21];
            reda += psrc[22];

            bluea += psrc[24];
            greena += psrc[25];
            reda += psrc[26];

            bluea += psrc[28];
            greena += psrc[29];
            reda += psrc[30];

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[4] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[6] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[8] = static_cast<uint8_t>(((yr_table[psrc[18]] + yg_table[psrc[17]] + yb_table[psrc[16]]) >> 15) + 16);
            pdst[9] = static_cast<uint8_t>(((yr_table[psrc[22]] + yg_table[psrc[21]] + yb_table[psrc[20]]) >> 15) + 16);
            pdst[10] = static_cast<uint8_t>(((yr_table[psrc[26]] + yg_table[psrc[25]] + yb_table[psrc[24]]) >> 15) + 16);
            pdst[11] = static_cast<uint8_t>(((yr_table[psrc[30]] + yg_table[psrc[29]] + yb_table[psrc[28]]) >> 15) + 16);

            psrc += 32;
            pdst += 12;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB32_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int16_t bluea = psrc[0];
            int16_t greena = psrc[1];
            int16_t reda = psrc[2];

            bluea += psrc[4];
            greena += psrc[5];
            reda += psrc[6];

            bluea += psrc[8];
            greena += psrc[9];
            reda += psrc[10];

            bluea += psrc[12];
            greena += psrc[13];
            reda += psrc[14];

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            uint32_t U = static_cast<uint32_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            uint32_t V = static_cast<uint32_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            uint32_t Y0 = static_cast<uint32_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            uint32_t Y1 = static_cast<uint32_t>(((yr_table[psrc[6]] + yg_table[psrc[5]] + yb_table[psrc[4]]) >> 15) + 16);
            uint32_t Y2 = static_cast<uint32_t>(((yr_table[psrc[10]] + yg_table[psrc[9]] + yb_table[psrc[8]]) >> 15) + 16);
            uint32_t Y3 = static_cast<uint32_t>(((yr_table[psrc[14]] + yg_table[psrc[13]] + yb_table[psrc[12]]) >> 15) + 16);

            PackCLJRDword(*pdst++, U, V, Y0, Y1, Y2, Y3);

            psrc += 16;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB32_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            psrc += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}


void blipvert::RGB32_to_Y16(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = 0x00;
            *pdst++ = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            psrc += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB32_to_AYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = static_cast<uint32_t>(((vr_table[psrc[2]] + vg_table[psrc[1]] + vb_table[psrc[0]]) >> 15) + 128);
            *pdst++ = static_cast<uint32_t>(((ur_table[psrc[2]] + ug_table[psrc[1]] + ub_table[psrc[0]]) >> 15) + 128);
            *pdst++ = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            *pdst++ = 0xFF;
            psrc += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// RGB24 to YUV transforms
//

void blipvert::RGB24_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB24_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

void blipvert::RGB24_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB24_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

void blipvert::RGB24_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB24_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

void blipvert::RGB24_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB24_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

void blipvert::RGB24_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB24_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, true);
}

void blipvert::RGB24_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB24_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

void blipvert::RGB24_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB24_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

void blipvert::RGB24_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB24_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

void blipvert::RGB24_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = (static_cast<int32_t>(psrc[0]) + static_cast<int32_t>(psrc[3]) + \
                static_cast<int32_t>(psrc[6]) + static_cast<int32_t>(psrc[9])) >> 2;
            int32_t green = (static_cast<int32_t>(psrc[1]) + static_cast<int32_t>(psrc[4]) + \
                static_cast<int32_t>(psrc[7]) + static_cast<int32_t>(psrc[10])) >> 2;
            int32_t red = (static_cast<int32_t>(psrc[2]) + static_cast<int32_t>(psrc[5]) + \
                static_cast<int32_t>(psrc[8]) + static_cast<int32_t>(psrc[11])) >> 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            pdst[3] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((yr_table[psrc[5]] + yg_table[psrc[4]] + yb_table[psrc[3]]) >> 15) + 16);
            pdst[4] = static_cast<uint8_t>(((yr_table[psrc[8]] + yg_table[psrc[7]] + yb_table[psrc[6]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[psrc[11]] + yg_table[psrc[10]] + yb_table[psrc[9]]) >> 15) + 16);

            psrc += 12;
            pdst += 6;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB24_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red = psrc[2];
            uint8_t green = psrc[1];
            uint8_t blue = psrc[0];
            pdst[0] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            pdst[1] = static_cast<uint8_t>(((yr_table[red] + yg_table[green] + yb_table[blue]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

            psrc += 3;
            pdst += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB24_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width / 8 * 12;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t bluea = psrc[0];
            int16_t greena = psrc[1];
            int16_t reda = psrc[2];

            bluea += psrc[3];
            greena += psrc[4];
            reda += psrc[5];

            bluea += psrc[6];
            greena += psrc[7];
            reda += psrc[8];

            bluea += psrc[9];
            greena += psrc[10];
            reda += psrc[11];

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[2] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            pdst[3] = static_cast<uint8_t>(((yr_table[psrc[5]] + yg_table[psrc[4]] + yb_table[psrc[3]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[psrc[8]] + yg_table[psrc[7]] + yb_table[psrc[6]]) >> 15) + 16);
            pdst[7] = static_cast<uint8_t>(((yr_table[psrc[11]] + yg_table[psrc[10]] + yb_table[psrc[9]]) >> 15) + 16);

            bluea = psrc[12];
            greena = psrc[13];
            reda = psrc[14];

            bluea += psrc[15];
            greena += psrc[16];
            reda += psrc[17];

            bluea += psrc[18];
            greena += psrc[19];
            reda += psrc[20];

            bluea += psrc[21];
            greena += psrc[22];
            reda += psrc[23];

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[4] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[6] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[8] = static_cast<uint8_t>(((yr_table[psrc[14]] + yg_table[psrc[13]] + yb_table[psrc[12]]) >> 15) + 16);
            pdst[9] = static_cast<uint8_t>(((yr_table[psrc[17]] + yg_table[psrc[16]] + yb_table[psrc[15]]) >> 15) + 16);
            pdst[10] = static_cast<uint8_t>(((yr_table[psrc[20]] + yg_table[psrc[19]] + yb_table[psrc[18]]) >> 15) + 16);
            pdst[11] = static_cast<uint8_t>(((yr_table[psrc[23]] + yg_table[psrc[22]] + yb_table[psrc[21]]) >> 15) + 16);

            psrc += 24;
            pdst += 12;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB24_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int16_t bluea = psrc[0];
            int16_t greena = psrc[1];
            int16_t reda = psrc[2];

            bluea += psrc[3];
            greena += psrc[4];
            reda += psrc[5];

            bluea += psrc[6];
            greena += psrc[7];
            reda += psrc[8];

            bluea += psrc[9];
            greena += psrc[10];
            reda += psrc[11];

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            uint32_t U = static_cast<uint32_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            uint32_t V = static_cast<uint32_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            uint32_t Y0 = static_cast<uint32_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            uint32_t Y1 = static_cast<uint32_t>(((yr_table[psrc[5]] + yg_table[psrc[4]] + yb_table[psrc[3]]) >> 15) + 16);
            uint32_t Y2 = static_cast<uint32_t>(((yr_table[psrc[8]] + yg_table[psrc[7]] + yb_table[psrc[6]]) >> 15) + 16);
            uint32_t Y3 = static_cast<uint32_t>(((yr_table[psrc[11]] + yg_table[psrc[10]] + yb_table[psrc[9]]) >> 15) + 16);

            PackCLJRDword(*pdst++, U, V, Y0, Y1, Y2, Y3);

            psrc += 12;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB24_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 3;


    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            psrc += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB24_to_Y16(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = 0x00;
            *pdst++ = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            psrc += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB24_to_AYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = static_cast<uint32_t>(((vr_table[psrc[2]] + vg_table[psrc[1]] + vb_table[psrc[0]]) >> 15) + 128);
            *pdst++ = static_cast<uint32_t>(((ur_table[psrc[2]] + ug_table[psrc[1]] + ub_table[psrc[0]]) >> 15) + 128);
            *pdst++ = static_cast<uint8_t>(((yr_table[psrc[2]] + yg_table[psrc[1]] + yb_table[psrc[0]]) >> 15) + 16);
            *pdst++ = 0xFF;
            psrc += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// RGB565 to YUV transforms
//

void blipvert::RGB565_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB565_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

void blipvert::RGB565_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB565_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

void blipvert::RGB565_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB565_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

void blipvert::RGB565_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB565_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

void blipvert::RGB565_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB565_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, true);
}

void blipvert::RGB565_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB565_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

void blipvert::RGB565_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB565_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

void blipvert::RGB565_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB565_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

void blipvert::RGB565_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t bluea = 0;
            int16_t greena = 0;
            int16_t reda = 0;
            int16_t src_blue[4];
            int16_t src_green[4];
            int16_t src_red[4];

            bluea += (src_blue[0] = static_cast<int16_t>(UnpackRGB565Blue(psrc[0])));
            greena += (src_green[0] = static_cast<int16_t>(UnpackRGB565Green(psrc[0])));
            reda += (src_red[0] = static_cast<int16_t>(UnpackRGB565Red(psrc[0])));

            bluea += (src_blue[1] = static_cast<int16_t>(UnpackRGB565Blue(psrc[1])));
            greena += (src_green[1] = static_cast<int16_t>(UnpackRGB565Green(psrc[1])));
            reda += (src_red[1] = static_cast<int16_t>(UnpackRGB565Red(psrc[1])));

            bluea += (src_blue[2] = static_cast<int16_t>(UnpackRGB565Blue(psrc[2])));
            greena += (src_green[2] = static_cast<int16_t>(UnpackRGB565Green(psrc[2])));
            reda += (src_red[2] = static_cast<int16_t>(UnpackRGB565Red(psrc[2])));

            bluea += (src_blue[3] = static_cast<int16_t>(UnpackRGB565Blue(psrc[3])));
            greena += (src_green[3] = static_cast<int16_t>(UnpackRGB565Green(psrc[3])));
            reda += (src_red[3] = static_cast<int16_t>(UnpackRGB565Red(psrc[3])));

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[3] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[src_red[0]] + yg_table[src_green[0]] + yb_table[src_blue[0]]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((yr_table[src_red[1]] + yg_table[src_green[1]] + yb_table[src_blue[1]]) >> 15) + 16);
            pdst[4] = static_cast<uint8_t>(((yr_table[src_red[2]] + yg_table[src_green[2]] + yb_table[src_blue[2]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[src_red[3]] + yg_table[src_green[3]] + yb_table[src_blue[3]]) >> 15) + 16);

            psrc += 4;
            pdst += 6;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB565_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red = static_cast<uint8_t>(UnpackRGB565Red(*psrc));
            uint8_t green = static_cast<uint8_t>(UnpackRGB565Green(*psrc));
            uint8_t blue = static_cast<uint8_t>(UnpackRGB565Blue(*psrc));
            pdst[0] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            pdst[1] = static_cast<uint8_t>(((yr_table[red] + yg_table[green] + yb_table[blue]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

            psrc++;
            pdst += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB565_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width / 8 * 12;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t blue[4];
            int16_t green[4];
            int16_t red[4];

            int16_t bluea = blue[0] = static_cast<int16_t>(UnpackRGB565Blue(psrc[0]));
            int16_t greena = green[0] = static_cast<int16_t>(UnpackRGB565Green(psrc[0]));
            int16_t reda = red[0] = static_cast<int16_t>(UnpackRGB565Red(psrc[0]));

            bluea += (blue[1] = static_cast<int16_t>(UnpackRGB565Blue(psrc[1])));
            greena += (green[1] = static_cast<int16_t>(UnpackRGB565Green(psrc[1])));
            reda += (red[1] = static_cast<int16_t>(UnpackRGB565Red(psrc[1])));

            bluea += (blue[2] = static_cast<int16_t>(UnpackRGB565Blue(psrc[2])));
            greena += (green[2] = static_cast<int16_t>(UnpackRGB565Green(psrc[2])));
            reda += (red[2] = static_cast<int16_t>(UnpackRGB565Red(psrc[2])));

            bluea += (blue[3] = static_cast<int16_t>(UnpackRGB565Blue(psrc[3])));
            greena += (green[3] = static_cast<int16_t>(UnpackRGB565Green(psrc[3])));
            reda += (red[3] = static_cast<int16_t>(UnpackRGB565Red(psrc[3])));

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[2] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[red[0]] + yg_table[green[0]] + yb_table[blue[0]]) >> 15) + 16);
            pdst[3] = static_cast<uint8_t>(((yr_table[red[1]] + yg_table[green[1]] + yb_table[blue[1]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[red[2]] + yg_table[green[2]] + yb_table[blue[2]]) >> 15) + 16);
            pdst[7] = static_cast<uint8_t>(((yr_table[red[3]] + yg_table[green[3]] + yb_table[blue[3]]) >> 15) + 16);

            bluea = blue[0] = static_cast<int16_t>(UnpackRGB565Blue(psrc[4]));
            greena = green[0] = static_cast<int16_t>(UnpackRGB565Green(psrc[4]));
            reda = red[0] = static_cast<int16_t>(UnpackRGB565Red(psrc[4]));

            bluea += (blue[1] = static_cast<int16_t>(UnpackRGB565Blue(psrc[5])));
            greena += (green[1] = static_cast<int16_t>(UnpackRGB565Green(psrc[5])));
            reda += (red[1] = static_cast<int16_t>(UnpackRGB565Red(psrc[5])));

            bluea += (blue[2] = static_cast<int16_t>(UnpackRGB565Blue(psrc[6])));
            greena += (green[2] = static_cast<int16_t>(UnpackRGB565Green(psrc[6])));
            reda += (red[2] = static_cast<int16_t>(UnpackRGB565Red(psrc[6])));

            bluea += (blue[3] = static_cast<int16_t>(UnpackRGB565Blue(psrc[7])));
            greena += (green[3] = static_cast<int16_t>(UnpackRGB565Green(psrc[7])));
            reda += (red[3] = static_cast<int16_t>(UnpackRGB565Red(psrc[7])));

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[4] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[6] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[8] = static_cast<uint8_t>(((yr_table[red[0]] + yg_table[green[0]] + yb_table[blue[0]]) >> 15) + 16);
            pdst[9] = static_cast<uint8_t>(((yr_table[red[1]] + yg_table[green[1]] + yb_table[blue[1]]) >> 15) + 16);
            pdst[10] = static_cast<uint8_t>(((yr_table[red[2]] + yg_table[green[2]] + yb_table[blue[2]]) >> 15) + 16);
            pdst[11] = static_cast<uint8_t>(((yr_table[red[3]] + yg_table[green[3]] + yb_table[blue[3]]) >> 15) + 16);

            psrc += 8;
            pdst += 12;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB565_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int16_t blue[4];
            int16_t green[4];
            int16_t red[4];

            int16_t bluea = blue[0] = static_cast<int16_t>(UnpackRGB565Blue(psrc[0]));
            int16_t greena = green[0] = static_cast<int16_t>(UnpackRGB565Green(psrc[0]));
            int16_t reda = red[0] = static_cast<int16_t>(UnpackRGB565Red(psrc[0]));

            bluea += (blue[1] = static_cast<int16_t>(UnpackRGB565Blue(psrc[1])));
            greena += (green[1] = static_cast<int16_t>(UnpackRGB565Green(psrc[1])));
            reda += (red[1] = static_cast<int16_t>(UnpackRGB565Red(psrc[1])));

            bluea += (blue[2] = static_cast<int16_t>(UnpackRGB565Blue(psrc[2])));
            greena += (green[2] = static_cast<int16_t>(UnpackRGB565Green(psrc[2])));
            reda += (red[2] = static_cast<int16_t>(UnpackRGB565Red(psrc[2])));

            bluea += (blue[3] = static_cast<int16_t>(UnpackRGB565Blue(psrc[3])));
            greena += (green[3] = static_cast<int16_t>(UnpackRGB565Green(psrc[3])));
            reda += (red[3] = static_cast<int16_t>(UnpackRGB565Red(psrc[3])));

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            uint32_t U = static_cast<uint32_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            uint32_t V = static_cast<uint32_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            uint32_t Y0 = static_cast<uint32_t>(((yr_table[red[0]] + yg_table[green[0]] + yb_table[blue[0]]) >> 15) + 16);
            uint32_t Y1 = static_cast<uint32_t>(((yr_table[red[1]] + yg_table[green[1]] + yb_table[blue[1]]) >> 15) + 16);
            uint32_t Y2 = static_cast<uint32_t>(((yr_table[red[2]] + yg_table[green[2]] + yb_table[blue[2]]) >> 15) + 16);
            uint32_t Y3 = static_cast<uint32_t>(((yr_table[red[3]] + yg_table[green[3]] + yb_table[blue[3]]) >> 15) + 16);

            PackCLJRDword(*pdst++, U, V, Y0, Y1, Y2, Y3);

            psrc += 4;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB565_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (uint16_t x = 0; x < width; x++)
        {
            pdst[x] = static_cast<uint8_t>(((yr_table[UnpackRGB565Red(psrc[x])] + \
                yg_table[UnpackRGB565Green(psrc[x])] + \
                yb_table[static_cast<int16_t>(UnpackRGB565Blue(psrc[x]))]) >> 15) + 16);
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB565_to_Y16(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = 0x00;
            *pdst++ = static_cast<uint8_t>(((yr_table[UnpackRGB565Red(*psrc)] + \
                yg_table[UnpackRGB565Green(*psrc)] + \
                yb_table[static_cast<int16_t>(UnpackRGB565Blue(*psrc))]) >> 15) + 16);
            psrc++;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB565_to_AYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red = static_cast<uint8_t>(UnpackRGB565Red(*psrc));
            uint8_t green = static_cast<uint8_t>(UnpackRGB565Green(*psrc));
            uint8_t blue = static_cast<uint8_t>(UnpackRGB565Blue(*psrc));
            *pdst++ = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);
            *pdst++ = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            *pdst++ = static_cast<uint8_t>(((yr_table[red] + yg_table[green] + yb_table[blue]) >> 15) + 16);
            *pdst++ = 0xFF;

            psrc++;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// RGB555 to YUV transforms
//

void blipvert::RGB555_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB555_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

void blipvert::RGB555_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB555_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

void blipvert::RGB555_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB555_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

void blipvert::RGB555_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB555_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

void blipvert::RGB555_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB555_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, true);
}

void blipvert::RGB555_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB555_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

void blipvert::RGB555_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB555_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

void blipvert::RGB555_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB555_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

void blipvert::RGB555_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t bluea = 0;
            int16_t greena = 0;
            int16_t reda = 0;
            int16_t src_blue[4];
            int16_t src_green[4];
            int16_t src_red[4];

            bluea += (src_blue[0] = static_cast<int16_t>(UnpackRGB555Blue(psrc[0])));
            greena += (src_green[0] = static_cast<int16_t>(UnpackRGB555Green(psrc[0])));
            reda += (src_red[0] = static_cast<int16_t>(UnpackRGB555Red(psrc[0])));

            bluea += (src_blue[1] = static_cast<int16_t>(UnpackRGB555Blue(psrc[1])));
            greena += (src_green[1] = static_cast<int16_t>(UnpackRGB555Green(psrc[1])));
            reda += (src_red[1] = static_cast<int16_t>(UnpackRGB555Red(psrc[1])));

            bluea += (src_blue[2] = static_cast<int16_t>(UnpackRGB555Blue(psrc[2])));
            greena += (src_green[2] = static_cast<int16_t>(UnpackRGB555Green(psrc[2])));
            reda += (src_red[2] = static_cast<int16_t>(UnpackRGB555Red(psrc[2])));

            bluea += (src_blue[3] = static_cast<int16_t>(UnpackRGB555Blue(psrc[3])));
            greena += (src_green[3] = static_cast<int16_t>(UnpackRGB555Green(psrc[3])));
            reda += (src_red[3] = static_cast<int16_t>(UnpackRGB555Red(psrc[3])));

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[3] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[src_red[0]] + yg_table[src_green[0]] + yb_table[src_blue[0]]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((yr_table[src_red[1]] + yg_table[src_green[1]] + yb_table[src_blue[1]]) >> 15) + 16);
            pdst[4] = static_cast<uint8_t>(((yr_table[src_red[2]] + yg_table[src_green[2]] + yb_table[src_blue[2]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[src_red[3]] + yg_table[src_green[3]] + yb_table[src_blue[3]]) >> 15) + 16);

            psrc += 4;
            pdst += 6;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB555_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red = static_cast<uint8_t>(UnpackRGB555Red(*psrc));
            uint8_t green = static_cast<uint8_t>(UnpackRGB555Green(*psrc));
            uint8_t blue = static_cast<uint8_t>(UnpackRGB555Blue(*psrc));
            pdst[0] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            pdst[1] = static_cast<uint8_t>(((yr_table[red] + yg_table[green] + yb_table[blue]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

            psrc++;
            pdst += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB555_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width / 8 * 12;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t blue[4];
            int16_t green[4];
            int16_t red[4];

            int16_t bluea = blue[0] = static_cast<int16_t>(UnpackRGB555Blue(psrc[0]));
            int16_t greena = green[0] = static_cast<int16_t>(UnpackRGB555Green(psrc[0]));
            int16_t reda = red[0] = static_cast<int16_t>(UnpackRGB555Red(psrc[0]));

            bluea += (blue[1] = static_cast<int16_t>(UnpackRGB555Blue(psrc[1])));
            greena += (green[1] = static_cast<int16_t>(UnpackRGB555Green(psrc[1])));
            reda += (red[1] = static_cast<int16_t>(UnpackRGB555Red(psrc[1])));

            bluea += (blue[2] = static_cast<int16_t>(UnpackRGB555Blue(psrc[2])));
            greena += (green[2] = static_cast<int16_t>(UnpackRGB555Green(psrc[2])));
            reda += (red[2] = static_cast<int16_t>(UnpackRGB555Red(psrc[2])));

            bluea += (blue[3] = static_cast<int16_t>(UnpackRGB555Blue(psrc[3])));
            greena += (green[3] = static_cast<int16_t>(UnpackRGB555Green(psrc[3])));
            reda += (red[3] = static_cast<int16_t>(UnpackRGB555Red(psrc[3])));

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[2] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[red[0]] + yg_table[green[0]] + yb_table[blue[0]]) >> 15) + 16);
            pdst[3] = static_cast<uint8_t>(((yr_table[red[1]] + yg_table[green[1]] + yb_table[blue[1]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[red[2]] + yg_table[green[2]] + yb_table[blue[2]]) >> 15) + 16);
            pdst[7] = static_cast<uint8_t>(((yr_table[red[3]] + yg_table[green[3]] + yb_table[blue[3]]) >> 15) + 16);

            bluea = blue[0] = static_cast<int16_t>(UnpackRGB555Blue(psrc[4]));
            greena = green[0] = static_cast<int16_t>(UnpackRGB555Green(psrc[4]));
            reda = red[0] = static_cast<int16_t>(UnpackRGB555Red(psrc[4]));

            bluea += (blue[1] = static_cast<int16_t>(UnpackRGB555Blue(psrc[5])));
            greena += (green[1] = static_cast<int16_t>(UnpackRGB555Green(psrc[5])));
            reda += (red[1] = static_cast<int16_t>(UnpackRGB555Red(psrc[5])));

            bluea += (blue[2] = static_cast<int16_t>(UnpackRGB555Blue(psrc[6])));
            greena += (green[2] = static_cast<int16_t>(UnpackRGB555Green(psrc[6])));
            reda += (red[2] = static_cast<int16_t>(UnpackRGB555Red(psrc[6])));

            bluea += (blue[3] = static_cast<int16_t>(UnpackRGB555Blue(psrc[7])));
            greena += (green[3] = static_cast<int16_t>(UnpackRGB555Green(psrc[7])));
            reda += (red[3] = static_cast<int16_t>(UnpackRGB555Red(psrc[7])));

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[4] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[6] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[8] = static_cast<uint8_t>(((yr_table[red[0]] + yg_table[green[0]] + yb_table[blue[0]]) >> 15) + 16);
            pdst[9] = static_cast<uint8_t>(((yr_table[red[1]] + yg_table[green[1]] + yb_table[blue[1]]) >> 15) + 16);
            pdst[10] = static_cast<uint8_t>(((yr_table[red[2]] + yg_table[green[2]] + yb_table[blue[2]]) >> 15) + 16);
            pdst[11] = static_cast<uint8_t>(((yr_table[red[3]] + yg_table[green[3]] + yb_table[blue[3]]) >> 15) + 16);

            psrc += 8;
            pdst += 12;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB555_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int16_t blue[4];
            int16_t green[4];
            int16_t red[4];

            int16_t bluea = blue[0] = static_cast<int16_t>(UnpackRGB555Blue(psrc[0]));
            int16_t greena = green[0] = static_cast<int16_t>(UnpackRGB555Green(psrc[0]));
            int16_t reda = red[0] = static_cast<int16_t>(UnpackRGB555Red(psrc[0]));

            bluea += (blue[1] = static_cast<int16_t>(UnpackRGB555Blue(psrc[1])));
            greena += (green[1] = static_cast<int16_t>(UnpackRGB555Green(psrc[1])));
            reda += (red[1] = static_cast<int16_t>(UnpackRGB555Red(psrc[1])));

            bluea += (blue[2] = static_cast<int16_t>(UnpackRGB555Blue(psrc[2])));
            greena += (green[2] = static_cast<int16_t>(UnpackRGB555Green(psrc[2])));
            reda += (red[2] = static_cast<int16_t>(UnpackRGB555Red(psrc[2])));

            bluea += (blue[3] = static_cast<int16_t>(UnpackRGB555Blue(psrc[3])));
            greena += (green[3] = static_cast<int16_t>(UnpackRGB555Green(psrc[3])));
            reda += (red[3] = static_cast<int16_t>(UnpackRGB555Red(psrc[3])));

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            uint32_t U = static_cast<uint32_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            uint32_t V = static_cast<uint32_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            uint32_t Y0 = static_cast<uint32_t>(((yr_table[red[0]] + yg_table[green[0]] + yb_table[blue[0]]) >> 15) + 16);
            uint32_t Y1 = static_cast<uint32_t>(((yr_table[red[1]] + yg_table[green[1]] + yb_table[blue[1]]) >> 15) + 16);
            uint32_t Y2 = static_cast<uint32_t>(((yr_table[red[2]] + yg_table[green[2]] + yb_table[blue[2]]) >> 15) + 16);
            uint32_t Y3 = static_cast<uint32_t>(((yr_table[red[3]] + yg_table[green[3]] + yb_table[blue[3]]) >> 15) + 16);

            PackCLJRDword(*pdst++, U, V, Y0, Y1, Y2, Y3);

            psrc += 4;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB555_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (uint16_t x = 0; x < width; x++)
        {
            pdst[x] = static_cast<uint8_t>(((yr_table[UnpackRGB555Red(psrc[x])] + \
                yg_table[UnpackRGB555Green(psrc[x])] + \
                yb_table[UnpackRGB555Blue(psrc[x])]) >> 15) + 16);
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB555_to_Y16(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = 0x00;
            *pdst++ = static_cast<uint8_t>(((yr_table[UnpackRGB555Red(*psrc)] + \
                yg_table[UnpackRGB555Green(*psrc)] + \
                yb_table[static_cast<int16_t>(UnpackRGB555Blue(*psrc))]) >> 15) + 16);
            psrc++;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB555_to_AYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red = static_cast<uint8_t>(UnpackRGB555Red(*psrc));
            uint8_t green = static_cast<uint8_t>(UnpackRGB555Green(*psrc));
            uint8_t blue = static_cast<uint8_t>(UnpackRGB555Blue(*psrc));
            *pdst++ = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);
            *pdst++ = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            *pdst++ = static_cast<uint8_t>(((yr_table[red] + yg_table[green] + yb_table[blue]) >> 15) + 16);
            *pdst++ = static_cast<uint8_t>(UnpackRGB555Alpha(*psrc));

            psrc++;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// RGB8 to YUV transforms
//

void blipvert::RGB8_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB8_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, in_palette, 0, 2, 1, 3, true);
}

void blipvert::RGB8_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB8_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, in_palette, 1, 3, 0, 2, true);
}

void blipvert::RGB8_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB8_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, in_palette, 0, 2, 3, 1, true);
}

void blipvert::RGB8_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB8_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, in_palette, 1, 3, 2, 0, true);
}

void blipvert::RGB8_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB8_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, in_palette, true, 2, true);
}

void blipvert::RGB8_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB8_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, in_palette, false, 2, true);
}

void blipvert::RGB8_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB8_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, in_palette, false, 4, true);
}

void blipvert::RGB8_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    RGB8_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, in_palette, true, 4, true);
}

void blipvert::RGB8_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width;

    if (in_palette == nullptr)
        in_palette = rgb8_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t bluea = 0;
            int16_t greena = 0;
            int16_t reda = 0;
            int16_t src_blue[4];
            int16_t src_green[4];
            int16_t src_red[4];

            bluea += (src_blue[0] = static_cast<int16_t>(in_palette[psrc[0]].rgbBlue));
            greena += (src_green[0] = static_cast<int16_t>(in_palette[psrc[0]].rgbGreen));
            reda += (src_red[0] = static_cast<int16_t>(in_palette[psrc[0]].rgbRed));

            bluea += (src_blue[1] = static_cast<int16_t>(in_palette[psrc[1]].rgbBlue));
            greena += (src_green[1] = static_cast<int16_t>(in_palette[psrc[1]].rgbGreen));
            reda += (src_red[1] = static_cast<int16_t>(in_palette[psrc[1]].rgbRed));

            bluea += (src_blue[2] = static_cast<int16_t>(in_palette[psrc[2]].rgbBlue));
            greena += (src_green[2] = static_cast<int16_t>(in_palette[psrc[2]].rgbGreen));
            reda += (src_red[2] = static_cast<int16_t>(in_palette[psrc[2]].rgbRed));

            bluea += (src_blue[3] = static_cast<int16_t>(in_palette[psrc[3]].rgbBlue));
            greena += (src_green[3] = static_cast<int16_t>(in_palette[psrc[3]].rgbGreen));
            reda += (src_red[3] = static_cast<int16_t>(in_palette[psrc[3]].rgbRed));

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[3] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[src_red[0]] + yg_table[src_green[0]] + yb_table[src_blue[0]]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((yr_table[src_red[1]] + yg_table[src_green[1]] + yb_table[src_blue[1]]) >> 15) + 16);
            pdst[4] = static_cast<uint8_t>(((yr_table[src_red[2]] + yg_table[src_green[2]] + yb_table[src_blue[2]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[src_red[3]] + yg_table[src_green[3]] + yb_table[src_blue[3]]) >> 15) + 16);

            psrc += 4;
            pdst += 6;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB8_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red = in_palette[*psrc].rgbRed;
            uint8_t green = in_palette[*psrc].rgbGreen;
            uint8_t blue = in_palette[*psrc].rgbBlue;
            pdst[0] = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            pdst[1] = static_cast<uint8_t>(((yr_table[red] + yg_table[green] + yb_table[blue]) >> 15) + 16);
            pdst[2] = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);

            psrc++;
            pdst += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB8_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width / 8 * 12;

    if (!in_stride)
        in_stride = width;

    if (in_palette == nullptr)
        in_palette = rgb8_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t blue[4];
            int16_t green[4];
            int16_t red[4];

            int16_t bluea = blue[0] = in_palette[psrc[0]].rgbBlue;
            int16_t greena = green[0] = in_palette[psrc[0]].rgbGreen;
            int16_t reda = red[0] = in_palette[psrc[0]].rgbRed;

            bluea += (blue[1] = in_palette[psrc[1]].rgbBlue);
            greena += (green[1] = in_palette[psrc[1]].rgbGreen);
            reda += (red[1] = in_palette[psrc[1]].rgbRed);

            bluea += (blue[2] = in_palette[psrc[2]].rgbBlue);
            greena += (green[2] = in_palette[psrc[2]].rgbGreen);
            reda += (red[2] = in_palette[psrc[2]].rgbRed);

            bluea += (blue[3] = in_palette[psrc[3]].rgbBlue);
            greena += (green[3] = in_palette[psrc[3]].rgbGreen);
            reda += (red[3] = in_palette[psrc[3]].rgbRed);

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[0] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[2] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[1] = static_cast<uint8_t>(((yr_table[red[0]] + yg_table[green[0]] + yb_table[blue[0]]) >> 15) + 16);
            pdst[3] = static_cast<uint8_t>(((yr_table[red[1]] + yg_table[green[1]] + yb_table[blue[1]]) >> 15) + 16);
            pdst[5] = static_cast<uint8_t>(((yr_table[red[2]] + yg_table[green[2]] + yb_table[blue[2]]) >> 15) + 16);
            pdst[7] = static_cast<uint8_t>(((yr_table[red[3]] + yg_table[green[3]] + yb_table[blue[3]]) >> 15) + 16);

            bluea = blue[0] = in_palette[psrc[4]].rgbBlue;
            greena = green[0] = in_palette[psrc[4]].rgbGreen;
            reda = red[0] = in_palette[psrc[4]].rgbRed;

            bluea += (blue[1] = in_palette[psrc[5]].rgbBlue);
            greena += (green[1] = in_palette[psrc[5]].rgbGreen);
            reda += (red[1] = in_palette[psrc[5]].rgbRed);

            bluea += (blue[2] = in_palette[psrc[6]].rgbBlue);
            greena += (green[2] = in_palette[psrc[6]].rgbGreen);
            reda += (red[2] = in_palette[psrc[6]].rgbRed);

            bluea += (blue[3] = in_palette[psrc[7]].rgbBlue);
            greena += (green[3] = in_palette[psrc[7]].rgbGreen);
            reda += (red[3] = in_palette[psrc[7]].rgbRed);

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            pdst[4] = static_cast<uint8_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            pdst[6] = static_cast<uint8_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            pdst[8] = static_cast<uint8_t>(((yr_table[red[0]] + yg_table[green[0]] + yb_table[blue[0]]) >> 15) + 16);
            pdst[9] = static_cast<uint8_t>(((yr_table[red[1]] + yg_table[green[1]] + yb_table[blue[1]]) >> 15) + 16);
            pdst[10] = static_cast<uint8_t>(((yr_table[red[2]] + yg_table[green[2]] + yb_table[blue[2]]) >> 15) + 16);
            pdst[11] = static_cast<uint8_t>(((yr_table[red[3]] + yg_table[green[3]] + yb_table[blue[3]]) >> 15) + 16);

            psrc += 8;
            pdst += 12;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB8_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width;

    if (in_palette == nullptr)
        in_palette = rgb8_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int16_t blue[4];
            int16_t green[4];
            int16_t red[4];

            int16_t bluea = blue[0] = in_palette[psrc[0]].rgbBlue;
            int16_t greena = green[0] = in_palette[psrc[0]].rgbGreen;
            int16_t reda = red[0] = in_palette[psrc[0]].rgbRed;

            bluea += (blue[1] = in_palette[psrc[1]].rgbBlue);
            greena += (green[1] = in_palette[psrc[1]].rgbGreen);
            reda += (red[1] = in_palette[psrc[1]].rgbRed);

            bluea += (blue[2] = in_palette[psrc[2]].rgbBlue);
            greena += (green[2] = in_palette[psrc[2]].rgbGreen);
            reda += (red[2] = in_palette[psrc[2]].rgbRed);

            bluea += (blue[3] = in_palette[psrc[3]].rgbBlue);
            greena += (green[3] = in_palette[psrc[3]].rgbGreen);
            reda += (red[3] = in_palette[psrc[3]].rgbRed);

            bluea >>= 2;
            greena >>= 2;
            reda >>= 2;

            uint32_t U = static_cast<uint32_t>(((ur_table[reda] + ug_table[greena] + ub_table[bluea]) >> 15) + 128);
            uint32_t V = static_cast<uint32_t>(((vr_table[reda] + vg_table[greena] + vb_table[bluea]) >> 15) + 128);

            uint32_t Y0 = static_cast<uint32_t>(((yr_table[red[0]] + yg_table[green[0]] + yb_table[blue[0]]) >> 15) + 16);
            uint32_t Y1 = static_cast<uint32_t>(((yr_table[red[1]] + yg_table[green[1]] + yb_table[blue[1]]) >> 15) + 16);
            uint32_t Y2 = static_cast<uint32_t>(((yr_table[red[2]] + yg_table[green[2]] + yb_table[blue[2]]) >> 15) + 16);
            uint32_t Y3 = static_cast<uint32_t>(((yr_table[red[3]] + yg_table[green[3]] + yb_table[blue[3]]) >> 15) + 16);

            PackCLJRDword(*pdst++, U, V, Y0, Y1, Y2, Y3);

            psrc += 4;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB8_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width;

    if (in_palette == nullptr)
        in_palette = rgb8_greyscale_palette;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        
        for (uint16_t x = 0; x < width; x++)
        {
            pdst[x] = static_cast<uint8_t>(((yr_table[in_palette[psrc[x]].rgbRed] + \
                yg_table[in_palette[psrc[x]].rgbGreen] + \
                yb_table[in_palette[psrc[x]].rgbBlue]) >> 15) + 16);
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::RGB8_to_Y16(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = 0x00;
            *pdst++ = static_cast<uint8_t>(((yr_table[in_palette[*psrc].rgbRed] + \
                yg_table[in_palette[*psrc].rgbGreen] + \
                yb_table[in_palette[*psrc].rgbBlue]) >> 15) + 16);
            psrc++;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}


void blipvert::RGB8_to_AYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t red = in_palette[*psrc].rgbRed;
            uint8_t green = in_palette[*psrc].rgbGreen;
            uint8_t blue = in_palette[*psrc].rgbBlue;
            *pdst++ = static_cast<uint8_t>(((vr_table[red] + vg_table[green] + vb_table[blue]) >> 15) + 128);
            *pdst++ = static_cast<uint8_t>(((ur_table[red] + ug_table[green] + ub_table[blue]) >> 15) + 128);
            *pdst++ = static_cast<uint8_t>(((yr_table[red] + yg_table[green] + yb_table[blue]) >> 15) + 16);
            *pdst++ = 0xFF;

            psrc++;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}
