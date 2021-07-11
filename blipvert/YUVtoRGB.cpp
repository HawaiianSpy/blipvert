
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
#include "YUVtoRGB.h"
#include "CommonMacros.h"
#include "LookupTables.h"

using namespace blipvert;

//
// Local generic packed YUV to RGB functions
//

//
// Packed Y422 format to RGB
//

void PackedY422_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped)
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
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = u_table[psrc[U]];
            int32_t green = uv_table[psrc[U]][psrc[V]];
            int32_t red = v_table[psrc[V]];

            int32_t Y = luminance_table[psrc[Y0]];
            pdst[0] = saturation_table[Y + blue];
            pdst[1] = saturation_table[Y + green];
            pdst[2] = saturation_table[Y + red];
            pdst[3] = 0xFF;

            Y = luminance_table[psrc[Y1]];
            pdst[4] = saturation_table[Y + blue];
            pdst[5] = saturation_table[Y + green];
            pdst[6] = saturation_table[Y + red];
            pdst[7] = 0xFF;

            psrc += 4;
            pdst += 8;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;

        height--;
    }
}

void PackedY422_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int16_t Y0, int16_t Y1, int16_t U, int16_t V, bool flipped)
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
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = u_table[psrc[U]];
            int32_t green = uv_table[psrc[U]][psrc[V]];
            int32_t red = v_table[psrc[V]];

            int32_t Y = luminance_table[psrc[Y0]];
            pdst[0] = saturation_table[Y + blue];
            pdst[1] = saturation_table[Y + green];
            pdst[2] = saturation_table[Y + red];

            Y = luminance_table[psrc[Y1]];
            pdst[3] = saturation_table[Y + blue];
            pdst[4] = saturation_table[Y + green];
            pdst[5] = saturation_table[Y + red];

            psrc += 4;
            pdst += 6;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void PackedY422_to_RGB565(int32_t width, int32_t height,
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
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = u_table[psrc[U]];
            int32_t green = uv_table[psrc[U]][psrc[V]];
            int32_t red = v_table[psrc[V]];

            int32_t Y = luminance_table[psrc[Y0]];
            PackRGB565Word(pdst[0], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[Y1]];
            PackRGB565Word(pdst[1], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            psrc += 4;
            pdst += 2;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void PackedY422_to_RGB555(int32_t width, int32_t height,
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
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = u_table[psrc[U]];
            int32_t green = uv_table[psrc[U]][psrc[V]];
            int32_t red = v_table[psrc[V]];

            int32_t Y = luminance_table[psrc[Y0]];
            PackRGB555Word(pdst[0], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[Y1]];
            PackRGB555Word(pdst[1], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            psrc += 4;
            pdst += 2;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// Local generic planarYUV to RGB
//

void PlanarYUV_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
    if (!out_stride)
        out_stride = width * 4;

    int16_t uv_width = width / decimation;
    int16_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!in_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = in_stride;
        uv_stride = in_stride;
    }

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = in_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = in_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_4 = y_stride * 4;

    if (decimation == 2)
    {
        for (int16_t y = 0; y < height; y += 2)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 2)
            {
                int32_t bprime = u_table[*up];
                int32_t gprime = uv_table[*up][*vp];
                int32_t rprime = v_table[*vp];

                // column 1 row 1
                int32_t Y = luminance_table[yp[0]];
                pdst[0] = saturation_table[Y + bprime];                     // blue
                pdst[1] = saturation_table[Y + gprime];                     // green
                pdst[2] = saturation_table[Y + rprime];                     // red
                pdst[3] = 0xFF;

                // column 1 row 2
                Y = luminance_table[yp[y_stride]];
                pdst[out_stride] = saturation_table[Y + bprime];            // blue
                pdst[1 + out_stride] = saturation_table[Y + gprime];        // green
                pdst[2 + out_stride] = saturation_table[Y + rprime];        // red
                pdst[3 + out_stride] = 0xFF;

                // column 2 row 1
                Y = luminance_table[yp[1]];
                pdst[4] = saturation_table[Y + bprime];                     // blue
                pdst[5] = saturation_table[Y + gprime];                     // green
                pdst[6] = saturation_table[Y + rprime];                     // red
                pdst[7] = 0xFF;

                // column 2 row 2
                Y = luminance_table[yp[1 + y_stride]];
                pdst[4 + out_stride] = saturation_table[Y + bprime];        // blue
                pdst[5 + out_stride] = saturation_table[Y + gprime];        // green
                pdst[6 + out_stride] = saturation_table[Y + rprime];        // red
                pdst[7 + out_stride] = 0xFF;

                pdst += 8;
                yp += 2;
                up++;
                vp++;
            }

            in_buf += y_stride_x_2;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (decimation == 4)
    {
        for (int16_t y = 0; y < height; y += 4)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 4)
            {
                int32_t bprime = u_table[*up];
                int32_t gprime = uv_table[*up][*vp];
                int32_t rprime = v_table[*vp];

                for (int16_t row = 0; row < 4; row++)
                {
                    for (int16_t col = 0; col < 4; col++)
                    {
                        int32_t Y = luminance_table[yp[col + y_stride * row]];
                        pdst[col * 4 + out_stride * row] = saturation_table[Y + bprime];            // blue
                        pdst[1 + col * 4 + out_stride * row] = saturation_table[Y + gprime];        // green
                        pdst[2 + col * 4 + out_stride * row] = saturation_table[Y + rprime];        // red
                        pdst[3 + col * 4 + out_stride * row] = 0xFF;
                    }
                }

                pdst += 16;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += y_stride_x_4;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_4;
        }
    }
}

void PlanarYUV_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
    if (!out_stride)
        out_stride = width * 3;

    int16_t uv_width = width / decimation;
    int16_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!in_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = in_stride;
        uv_stride = in_stride;
    }

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = in_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = in_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_4 = y_stride * 4;

    if (decimation == 2)
    {
        for (int16_t y = 0; y < height; y += 2)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 2)
            {
                int32_t bprime = u_table[*up];
                int32_t gprime = uv_table[*up][*vp];
                int32_t rprime = v_table[*vp];

                // column 1 row 1
                int32_t Y = luminance_table[yp[0]];
                pdst[0] = saturation_table[Y + bprime];                     // blue
                pdst[1] = saturation_table[Y + gprime];                     // green
                pdst[2] = saturation_table[Y + rprime];                     // red

                // column 1 row 2
                Y = luminance_table[yp[y_stride]];
                pdst[out_stride] = saturation_table[Y + bprime];            // blue
                pdst[1 + out_stride] = saturation_table[Y + gprime];        // green
                pdst[2 + out_stride] = saturation_table[Y + rprime];        // red

                // column 2 row 1
                Y = luminance_table[yp[1]];
                pdst[3] = saturation_table[Y + bprime];                     // blue
                pdst[4] = saturation_table[Y + gprime];                     // green
                pdst[5] = saturation_table[Y + rprime];                     // red

                // column 2 row 2
                Y = luminance_table[yp[1 + y_stride]];
                pdst[3 + out_stride] = saturation_table[Y + bprime];        // blue
                pdst[4 + out_stride] = saturation_table[Y + gprime];        // green
                pdst[5 + out_stride] = saturation_table[Y + rprime];        // red

                pdst += 6;
                yp += 2;
                up++;
                vp++;
            }

            in_buf += y_stride_x_2;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (decimation == 4)
    {
        for (int16_t y = 0; y < height; y += 4)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 4)
            {
                int32_t bprime = u_table[*up];
                int32_t gprime = uv_table[*up][*vp];
                int32_t rprime = v_table[*vp];

                for (int16_t row = 0; row < 4; row++)
                {
                    for (int16_t col = 0; col < 4; col++)
                    {
                        int32_t Y = luminance_table[yp[col + y_stride * row]];
                        pdst[col * 3 + out_stride * row] = saturation_table[Y + bprime];            // blue
                        pdst[1 + col * 3 + out_stride * row] = saturation_table[Y + gprime];        // green
                        pdst[2 + col * 3 + out_stride * row] = saturation_table[Y + rprime];        // red
                    }
                }

                pdst += 12;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += y_stride_x_4;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_4;
        }
    }
}

void PlanarYUV_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
    if (!out_stride)
        out_stride = width * 2;

    int16_t uv_width = width / decimation;
    int16_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!in_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = in_stride;
        uv_stride = in_stride;
    }

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = in_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = in_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_4 = y_stride * 4;

    if (decimation == 2)
    {
        for (int16_t y = 0; y < height; y += 2)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 2)
            {
                int32_t bprime = u_table[*up];
                int32_t gprime = uv_table[*up][*vp];
                int32_t rprime = v_table[*vp];

                // column 1 row 1
                int32_t Y = luminance_table[yp[0]];
                PackRGB565Word(*(reinterpret_cast<uint16_t*>(pdst)),
                    saturation_table[Y + rprime],       // red
                    saturation_table[Y + gprime],       // green
                    saturation_table[Y + bprime]);      // blue

                // column 1 row 2
                Y = luminance_table[yp[y_stride]];
                PackRGB565Word(*(reinterpret_cast<uint16_t*>(pdst + out_stride)),
                    saturation_table[Y + rprime],       // red
                    saturation_table[Y + gprime],       // green
                    saturation_table[Y + bprime]);      // blue

                // column 2 row 1
                Y = luminance_table[yp[1]];
                PackRGB565Word(*(reinterpret_cast<uint16_t*>(pdst + 2)),
                    saturation_table[Y + rprime],       // red
                    saturation_table[Y + gprime],       // green
                    saturation_table[Y + bprime]);      // blue

                // column 2 row 2
                Y = luminance_table[yp[1 + y_stride]];
                PackRGB565Word(*(reinterpret_cast<uint16_t*>(pdst + 2 + out_stride)),
                    saturation_table[Y + rprime],       // red
                    saturation_table[Y + gprime],       // green
                    saturation_table[Y + bprime]);      // blue

                pdst += 4;
                yp += 2;
                up++;
                vp++;
            }

            in_buf += y_stride_x_2;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (decimation == 4)
    {
        for (int16_t y = 0; y < height; y += 4)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 4)
            {
                int32_t bprime = u_table[*up];
                int32_t gprime = uv_table[*up][*vp];
                int32_t rprime = v_table[*vp];

                for (int16_t row = 0; row < 4; row++)
                {
                    for (int16_t col = 0; col < 4; col++)
                    {
                        int32_t Y = luminance_table[yp[col + y_stride * row]];
                        PackRGB565Word(*(reinterpret_cast<uint16_t*>(pdst + col * 2 + out_stride * row)),
                            saturation_table[Y + rprime],       // red
                            saturation_table[Y + gprime],       // green
                            saturation_table[Y + bprime]);      // blue
                    }
                }

                pdst += 8;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += y_stride_x_4;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_4;
        }
    }
}

void PlanarYUV_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
    if (!out_stride)
        out_stride = width * 2;

    int16_t uv_width = width / decimation;
    int16_t uv_height = height / decimation;

    int16_t y_stride, uv_stride;
    if (!in_stride)
    {
        y_stride = width;
        uv_stride = uv_width;
    }
    else
    {
        y_stride = in_stride;
        uv_stride = in_stride;
    }

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        uplane = in_buf + (y_stride * height);
        vplane = uplane + (uv_stride * uv_height);
    }
    else
    {
        vplane = in_buf + (y_stride * height);
        uplane = vplane + (uv_stride * uv_height);
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_4 = y_stride * 4;

    if (decimation == 2)
    {
        for (int16_t y = 0; y < height; y += 2)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 2)
            {
                int32_t bprime = u_table[*up];
                int32_t gprime = uv_table[*up][*vp];
                int32_t rprime = v_table[*vp];

                // column 1 row 1
                int32_t Y = luminance_table[yp[0]];
                PackRGB555Word(*reinterpret_cast<uint16_t*>(pdst),
                    saturation_table[Y + rprime],       // red
                    saturation_table[Y + gprime],       // green
                    saturation_table[Y + bprime]);      // blue

                // column 1 row 2
                Y = luminance_table[yp[y_stride]];
                PackRGB555Word(*reinterpret_cast<uint16_t*>(pdst + out_stride),
                    saturation_table[Y + rprime],       // red
                    saturation_table[Y + gprime],       // green
                    saturation_table[Y + bprime]);      // blue

                // column 2 row 1
                Y = luminance_table[yp[1]];
                PackRGB555Word(*reinterpret_cast<uint16_t*>(pdst + 2),
                    saturation_table[Y + rprime],       // red
                    saturation_table[Y + gprime],       // green
                    saturation_table[Y + bprime]);      // blue

                // column 2 row 2
                Y = luminance_table[yp[1 + y_stride]];
                PackRGB555Word(*reinterpret_cast<uint16_t*>(pdst + 2 + out_stride),
                    saturation_table[Y + rprime],       // red
                    saturation_table[Y + gprime],       // green
                    saturation_table[Y + bprime]);      // blue

                pdst += 4;
                yp += 2;
                up++;
                vp++;
            }

            in_buf += y_stride_x_2;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (decimation == 4)
    {
        for (int16_t y = 0; y < height; y += 4)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 4)
            {
                int32_t bprime = u_table[*up];
                int32_t gprime = uv_table[*up][*vp];
                int32_t rprime = v_table[*vp];

                for (int16_t row = 0; row < 4; row++)
                {
                    for (int16_t col = 0; col < 4; col++)
                    {
                        int32_t Y = luminance_table[yp[col + y_stride * row]];
                        PackRGB555Word(*(reinterpret_cast<uint16_t*>(pdst + col * 2 + out_stride * row)),
                            saturation_table[Y + rprime],       // red
                            saturation_table[Y + gprime],       // green
                            saturation_table[Y + bprime]);      // blue
                    }
                }

                pdst += 8;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += y_stride_x_4;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_4;
        }
    }
}

void IMCx_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, bool interlaced, bool flipped)
{
    if (!out_stride)
        out_stride = width * 4;

    int16_t uv_width = width / 2;

    if (in_stride < width)
        in_stride = width;

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        if (interlaced)
        {
            uplane = in_buf + (in_stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            uplane = in_buf + (((height + 15) & ~15) * in_stride);
            vplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            vplane = in_buf + (in_stride * height);
            uplane = vplane + uv_width;
        }
        else
        {
            vplane = in_buf + (((height + 15) & ~15) * in_stride);
            uplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int16_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = uplane;
        uint8_t* vp = vplane;
        uint8_t* pdst = out_buf;

        for (int16_t x = 0; x < width; x += 2)
        {
            int32_t bprime = u_table[*up];
            int32_t gprime = uv_table[*up][*vp];
            int32_t rprime = v_table[*vp];

            // column 1 row 1
            int32_t Y = luminance_table[yp[0]];
            pdst[0] = saturation_table[Y + bprime];                     // blue
            pdst[1] = saturation_table[Y + gprime];                     // green
            pdst[2] = saturation_table[Y + rprime];                     // red
            pdst[3] = 0xFF;

            // column 1 row 2
            Y = luminance_table[yp[in_stride]];
            pdst[out_stride] = saturation_table[Y + bprime];            // blue
            pdst[1 + out_stride] = saturation_table[Y + gprime];        // green
            pdst[2 + out_stride] = saturation_table[Y + rprime];        // red
            pdst[3 + out_stride] = 0xFF;

            // column 2 row 1
            Y = luminance_table[yp[1]];
            pdst[4] = saturation_table[Y + bprime];                     // blue
            pdst[5] = saturation_table[Y + gprime];                     // green
            pdst[6] = saturation_table[Y + rprime];                     // red
            pdst[7] = 0xFF;

            // column 2 row 2
            Y = luminance_table[yp[1 + in_stride]];
            pdst[4 + out_stride] = saturation_table[Y + bprime];        // blue
            pdst[5 + out_stride] = saturation_table[Y + gprime];        // green
            pdst[6 + out_stride] = saturation_table[Y + rprime];        // red
            pdst[7 + out_stride] = 0xFF;

            pdst += 8;
            yp += 2;
            up++;
            vp++;
        }

        in_buf += in_stride_x_2;
        uplane += in_stride;
        vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void IMCx_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, bool interlaced, bool flipped)
{
    if (!out_stride)
        out_stride = width * 3;

    int16_t uv_width = width / 2;

    if (in_stride < width)
        in_stride = width;

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        if (interlaced)
        {
            uplane = in_buf + (in_stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            uplane = in_buf + (((height + 15) & ~15) * in_stride);
            vplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            vplane = in_buf + (in_stride * height);
            uplane = vplane + uv_width;
        }
        else
        {
            vplane = in_buf + (((height + 15) & ~15) * in_stride);
            uplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int16_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = uplane;
        uint8_t* vp = vplane;
        uint8_t* pdst = out_buf;

        for (int16_t x = 0; x < width; x += 2)
        {
            int32_t bprime = u_table[*up];
            int32_t gprime = uv_table[*up][*vp];
            int32_t rprime = v_table[*vp];

            // column 1 row 1
            int32_t Y = luminance_table[yp[0]];
            pdst[0] = saturation_table[Y + bprime];                     // blue
            pdst[1] = saturation_table[Y + gprime];                     // green
            pdst[2] = saturation_table[Y + rprime];                     // red

            // column 1 row 2
            Y = luminance_table[yp[in_stride]];
            pdst[out_stride] = saturation_table[Y + bprime];            // blue
            pdst[1 + out_stride] = saturation_table[Y + gprime];        // green
            pdst[2 + out_stride] = saturation_table[Y + rprime];        // red

            // column 2 row 1
            Y = luminance_table[yp[1]];
            pdst[3] = saturation_table[Y + bprime];                     // blue
            pdst[4] = saturation_table[Y + gprime];                     // green
            pdst[5] = saturation_table[Y + rprime];                     // red

            // column 2 row 2
            Y = luminance_table[yp[1 + in_stride]];
            pdst[3 + out_stride] = saturation_table[Y + bprime];        // blue
            pdst[4 + out_stride] = saturation_table[Y + gprime];        // green
            pdst[5 + out_stride] = saturation_table[Y + rprime];        // red

            pdst += 6;
            yp += 2;
            up++;
            vp++;
        }

        in_buf += in_stride_x_2;
        uplane += in_stride;
        vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void IMCx_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, bool interlaced, bool flipped)
{
    if (!out_stride)
        out_stride = width * 2;

    int16_t uv_width = width / 2;

    if (in_stride < width)
        in_stride = width;

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        if (interlaced)
        {
            uplane = in_buf + (in_stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            uplane = in_buf + (((height + 15) & ~15) * in_stride);
            vplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            vplane = in_buf + (in_stride * height);
            uplane = vplane + uv_width;
        }
        else
        {
            vplane = in_buf + (((height + 15) & ~15) * in_stride);
            uplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int16_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = uplane;
        uint8_t* vp = vplane;
        uint8_t* pdst = out_buf;

        for (int16_t x = 0; x < width; x += 2)
        {
            int32_t bprime = u_table[*up];
            int32_t gprime = uv_table[*up][*vp];
            int32_t rprime = v_table[*vp];

            // column 1 row 1
            int32_t Y = luminance_table[yp[0]];
            PackRGB565Word(*(reinterpret_cast<uint16_t*>(pdst)),
                saturation_table[Y + rprime],       // red
                saturation_table[Y + gprime],       // green
                saturation_table[Y + bprime]);      // blue

            // column 1 row 2
            Y = luminance_table[yp[in_stride]];
            PackRGB565Word(*(reinterpret_cast<uint16_t*>(pdst + out_stride)),
                saturation_table[Y + rprime],       // red
                saturation_table[Y + gprime],       // green
                saturation_table[Y + bprime]);      // blue

            // column 2 row 1
            Y = luminance_table[yp[1]];
            PackRGB565Word(*(reinterpret_cast<uint16_t*>(pdst + 2)),
                saturation_table[Y + rprime],       // red
                saturation_table[Y + gprime],       // green
                saturation_table[Y + bprime]);      // blue

            // column 2 row 2
            Y = luminance_table[yp[1 + in_stride]];
            PackRGB565Word(*(reinterpret_cast<uint16_t*>(pdst + 2 + out_stride)),
                saturation_table[Y + rprime],       // red
                saturation_table[Y + gprime],       // green
                saturation_table[Y + bprime]);      // blue

            pdst += 4;
            yp += 2;
            up++;
            vp++;
        }

        in_buf += in_stride_x_2;
        uplane += in_stride;
        vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void IMCx_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool uFirst, bool interlaced, bool flipped)
{
    if (!out_stride)
        out_stride = width * 2;

    int16_t uv_width = width / 2;

    if (in_stride < width)
        in_stride = width;

    uint8_t* vplane;
    uint8_t* uplane;
    if (uFirst)
    {
        if (interlaced)
        {
            uplane = in_buf + (in_stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            uplane = in_buf + (((height + 15) & ~15) * in_stride);
            vplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            vplane = in_buf + (in_stride * height);
            uplane = vplane + uv_width;
        }
        else
        {
            vplane = in_buf + (((height + 15) & ~15) * in_stride);
            uplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int16_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = uplane;
        uint8_t* vp = vplane;
        uint8_t* pdst = out_buf;

        for (int16_t x = 0; x < width; x += 2)
        {
            int32_t bprime = u_table[*up];
            int32_t gprime = uv_table[*up][*vp];
            int32_t rprime = v_table[*vp];

            // column 1 row 1
            int32_t Y = luminance_table[yp[0]];
            PackRGB555Word(*reinterpret_cast<uint16_t*>(pdst),
                saturation_table[Y + rprime],       // red
                saturation_table[Y + gprime],       // green
                saturation_table[Y + bprime]);      // blue

            // column 1 row 2
            Y = luminance_table[yp[in_stride]];
            PackRGB555Word(*reinterpret_cast<uint16_t*>(pdst + out_stride),
                saturation_table[Y + rprime],       // red
                saturation_table[Y + gprime],       // green
                saturation_table[Y + bprime]);      // blue

            // column 2 row 1
            Y = luminance_table[yp[1]];
            PackRGB555Word(*reinterpret_cast<uint16_t*>(pdst + 2),
                saturation_table[Y + rprime],       // red
                saturation_table[Y + gprime],       // green
                saturation_table[Y + bprime]);      // blue

            // column 2 row 2
            Y = luminance_table[yp[1 + in_stride]];
            PackRGB555Word(*reinterpret_cast<uint16_t*>(pdst + 2 + out_stride),
                saturation_table[Y + rprime],       // red
                saturation_table[Y + gprime],       // green
                saturation_table[Y + bprime]);      // blue

            pdst += 4;
            yp += 2;
            up++;
            vp++;
        }

        in_buf += in_stride_x_2;
        uplane += in_stride;
        vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

//
// Public packed YUY2 to RGB functions
//

void blipvert::YUY2_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

void blipvert::YUY2_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

void blipvert::YUY2_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

void blipvert::YUY2_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

//
// UYVY to RGB
//

void blipvert::UYVY_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

void blipvert::UYVY_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

void blipvert::UYVY_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

void blipvert::UYVY_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

//
// YVYU to RGB
//

void blipvert::YVYU_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

void blipvert::YVYU_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

void blipvert::YVYU_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

void blipvert::YVYU_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

//
// VYUY to RGB
//

void blipvert::VYUY_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

void blipvert::VYUY_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

void blipvert::VYUY_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

void blipvert::VYUY_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

//
// CLJR format to RGB
//

void blipvert::CLJR_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint32_t mpixel = *psrc++;
            uint32_t U = UnpackCLJR_U(mpixel);
            uint32_t V = UnpackCLJR_V(mpixel);
            int32_t blue = u_table[U];
            int32_t green = uv_table[U][V];
            int32_t red = v_table[V];

            int32_t Y = luminance_table[UnpackCLJR_Y0(mpixel)];
            pdst[0] = saturation_table[Y + blue];
            pdst[1] = saturation_table[Y + green];
            pdst[2] = saturation_table[Y + red];
            pdst[3] = 0xFF;

            Y = luminance_table[UnpackCLJR_Y1(mpixel)];
            pdst[4] = saturation_table[Y + blue];
            pdst[5] = saturation_table[Y + green];
            pdst[6] = saturation_table[Y + red];
            pdst[7] = 0xFF;

            Y = luminance_table[UnpackCLJR_Y2(mpixel)];
            pdst[8] = saturation_table[Y + blue];
            pdst[9] = saturation_table[Y + green];
            pdst[10] = saturation_table[Y + red];
            pdst[11] = 0xFF;

            Y = luminance_table[UnpackCLJR_Y3(mpixel)];
            pdst[12] = saturation_table[Y + blue];
            pdst[13] = saturation_table[Y + green];
            pdst[14] = saturation_table[Y + red];
            pdst[15] = 0xFF;

            pdst += 16;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::CLJR_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint32_t mpixel = *psrc++;
            uint32_t U = UnpackCLJR_U(mpixel);
            uint32_t V = UnpackCLJR_V(mpixel);
            uint32_t blue = u_table[U];
            uint32_t green = uv_table[U][V];
            uint32_t red = v_table[V];

            uint32_t Y = luminance_table[UnpackCLJR_Y0(mpixel)];
            pdst[0] = saturation_table[Y + blue];
            pdst[1] = saturation_table[Y + green];
            pdst[2] = saturation_table[Y + red];

            Y = luminance_table[UnpackCLJR_Y1(mpixel)];
            pdst[3] = saturation_table[Y + blue];
            pdst[4] = saturation_table[Y + green];
            pdst[5] = saturation_table[Y + red];

            Y = luminance_table[UnpackCLJR_Y2(mpixel)];
            pdst[6] = saturation_table[Y + blue];
            pdst[7] = saturation_table[Y + green];
            pdst[8] = saturation_table[Y + red];

            Y = luminance_table[UnpackCLJR_Y3(mpixel)];
            pdst[9] = saturation_table[Y + blue];
            pdst[10] = saturation_table[Y + green];
            pdst[11] = saturation_table[Y + red];

            pdst += 12;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::CLJR_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            uint32_t mpixel = *psrc++;
            uint32_t U = UnpackCLJR_U(mpixel);
            uint32_t V = UnpackCLJR_V(mpixel);
            uint32_t blue = u_table[U];
            uint32_t green = uv_table[U][V];
            uint32_t red = v_table[V];

            uint32_t Y = luminance_table[UnpackCLJR_Y0(mpixel)];
            PackRGB565Word(pdst[0], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[UnpackCLJR_Y1(mpixel)];
            PackRGB565Word(pdst[1], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[UnpackCLJR_Y2(mpixel)];
            PackRGB565Word(pdst[2], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[UnpackCLJR_Y3(mpixel)];
            PackRGB565Word(pdst[3], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            pdst += 4;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::CLJR_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            uint32_t mpixel = *psrc++;
            uint32_t U = UnpackCLJR_U(mpixel);
            uint32_t V = UnpackCLJR_V(mpixel);
            uint32_t blue = u_table[U];
            uint32_t green = uv_table[U][V];
            uint32_t red = v_table[V];

            uint32_t Y = luminance_table[UnpackCLJR_Y0(mpixel)];
            PackRGB555Word(pdst[0], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[UnpackCLJR_Y1(mpixel)];
            PackRGB555Word(pdst[1], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[UnpackCLJR_Y2(mpixel)];
            PackRGB555Word(pdst[2], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[UnpackCLJR_Y3(mpixel)];
            PackRGB555Word(pdst[3], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            pdst += 4;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// Y41P format to RGB
//

void blipvert::Y41P_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width / 8 * 12;

    flipped ^= true;
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
            int32_t blue = u_table[psrc[0]];
            int32_t green = uv_table[psrc[0]][psrc[2]];
            int32_t red = v_table[psrc[2]];

            int32_t Y = luminance_table[psrc[1]];
            pdst[0] = saturation_table[Y + blue];
            pdst[1] = saturation_table[Y + green];
            pdst[2] = saturation_table[Y + red];
            pdst[3] = 0xFF;

            Y = luminance_table[psrc[3]];
            pdst[4] = saturation_table[Y + blue];
            pdst[5] = saturation_table[Y + green];
            pdst[6] = saturation_table[Y + red];
            pdst[7] = 0xFF;

            Y = luminance_table[psrc[5]];
            pdst[8] = saturation_table[Y + blue];
            pdst[9] = saturation_table[Y + green];
            pdst[10] = saturation_table[Y + red];
            pdst[11] = 0xFF;

            Y = luminance_table[psrc[7]];
            pdst[12] = saturation_table[Y + blue];
            pdst[13] = saturation_table[Y + green];
            pdst[14] = saturation_table[Y + red];
            pdst[15] = 0xFF;

            blue = u_table[psrc[4]];
            green = uv_table[psrc[4]][psrc[6]];
            red = v_table[psrc[6]];

            Y = luminance_table[psrc[8]];
            pdst[16] = saturation_table[Y + blue];
            pdst[17] = saturation_table[Y + green];
            pdst[18] = saturation_table[Y + red];
            pdst[19] = 0xFF;

            Y = luminance_table[psrc[9]];
            pdst[20] = saturation_table[Y + blue];
            pdst[21] = saturation_table[Y + green];
            pdst[22] = saturation_table[Y + red];
            pdst[23] = 0xFF;

            Y = luminance_table[psrc[10]];
            pdst[24] = saturation_table[Y + blue];
            pdst[25] = saturation_table[Y + green];
            pdst[26] = saturation_table[Y + red];
            pdst[27] = 0xFF;

            Y = luminance_table[psrc[11]];
            pdst[28] = saturation_table[Y + blue];
            pdst[29] = saturation_table[Y + green];
            pdst[30] = saturation_table[Y + red];
            pdst[31] = 0xFF;

            psrc += 12;
            pdst += 32;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y41P_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width / 8 * 12;

    flipped ^= true;
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
            int32_t blue = u_table[psrc[0]];
            int32_t green = uv_table[psrc[0]][psrc[2]];
            int32_t red = v_table[psrc[2]];

            int32_t Y = luminance_table[psrc[1]];
            pdst[0] = saturation_table[Y + blue];
            pdst[1] = saturation_table[Y + green];
            pdst[2] = saturation_table[Y + red];

            Y = luminance_table[psrc[3]];
            pdst[3] = saturation_table[Y + blue];
            pdst[4] = saturation_table[Y + green];
            pdst[5] = saturation_table[Y + red];

            Y = luminance_table[psrc[5]];
            pdst[6] = saturation_table[Y + blue];
            pdst[7] = saturation_table[Y + green];
            pdst[8] = saturation_table[Y + red];

            Y = luminance_table[psrc[7]];
            pdst[9] = saturation_table[Y + blue];
            pdst[10] = saturation_table[Y + green];
            pdst[11] = saturation_table[Y + red];

            blue = u_table[psrc[4]];
            green = uv_table[psrc[4]][psrc[6]];
            red = v_table[psrc[6]];

            Y = luminance_table[psrc[8]];
            pdst[12] = saturation_table[Y + blue];
            pdst[13] = saturation_table[Y + green];
            pdst[14] = saturation_table[Y + red];

            Y = luminance_table[psrc[9]];
            pdst[15] = saturation_table[Y + blue];
            pdst[16] = saturation_table[Y + green];
            pdst[17] = saturation_table[Y + red];

            Y = luminance_table[psrc[10]];
            pdst[18] = saturation_table[Y + blue];
            pdst[19] = saturation_table[Y + green];
            pdst[20] = saturation_table[Y + red];

            Y = luminance_table[psrc[11]];
            pdst[21] = saturation_table[Y + blue];
            pdst[22] = saturation_table[Y + green];
            pdst[23] = saturation_table[Y + red];

            psrc += 12;
            pdst += 24;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y41P_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width / 8 * 12;

    flipped ^= true;
    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = u_table[psrc[0]];
            int32_t green = uv_table[psrc[0]][psrc[2]];
            int32_t red = v_table[psrc[2]];

            int32_t Y = luminance_table[psrc[1]];
            PackRGB565Word(pdst[0], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[3]];
            PackRGB565Word(pdst[1], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[5]];
            PackRGB565Word(pdst[2], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[7]];
            PackRGB565Word(pdst[3], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            blue = u_table[psrc[4]];
            green = uv_table[psrc[4]][psrc[6]];
            red = v_table[psrc[6]];

            Y = luminance_table[psrc[8]];
            PackRGB565Word(pdst[4], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[9]];
            PackRGB565Word(pdst[5], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[10]];
            PackRGB565Word(pdst[6], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[11]];
            PackRGB565Word(pdst[7], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            psrc += 12;
            pdst += 8;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y41P_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width / 8 * 12;

    flipped ^= true;
    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = u_table[psrc[0]];
            int32_t green = uv_table[psrc[0]][psrc[2]];
            int32_t red = v_table[psrc[2]];

            int32_t Y = luminance_table[psrc[1]];
            PackRGB555Word(pdst[0], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[3]];
            PackRGB555Word(pdst[1], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[5]];
            PackRGB555Word(pdst[2], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[7]];
            PackRGB555Word(pdst[3], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            blue = u_table[psrc[4]];
            green = uv_table[psrc[4]][psrc[6]];
            red = v_table[psrc[6]];

            Y = luminance_table[psrc[8]];
            PackRGB555Word(pdst[4], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[9]];
            PackRGB555Word(pdst[5], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[10]];
            PackRGB555Word(pdst[6], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[11]];
            PackRGB555Word(pdst[7], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            psrc += 12;
            pdst += 8;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// IYU1 (IEEE 1394 Digital Camera 1.04 spec, mode 1) format to RGB
//

void blipvert::IYU1_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 12 / 8;

    flipped ^= true;
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
            int32_t blue = u_table[psrc[0]];
            int32_t green = uv_table[psrc[0]][psrc[3]];
            int32_t red = v_table[psrc[3]];

            int32_t Y = luminance_table[psrc[1]];
            pdst[0] = saturation_table[Y + blue];
            pdst[1] = saturation_table[Y + green];
            pdst[2] = saturation_table[Y + red];
            pdst[3] = 0xFF;

            Y = luminance_table[psrc[2]];
            pdst[4] = saturation_table[Y + blue];
            pdst[5] = saturation_table[Y + green];
            pdst[6] = saturation_table[Y + red];
            pdst[7] = 0xFF;

            Y = luminance_table[psrc[4]];
            pdst[8] = saturation_table[Y + blue];
            pdst[9] = saturation_table[Y + green];
            pdst[10] = saturation_table[Y + red];
            pdst[11] = 0xFF;

            Y = luminance_table[psrc[5]];
            pdst[12] = saturation_table[Y + blue];
            pdst[13] = saturation_table[Y + green];
            pdst[14] = saturation_table[Y + red];
            pdst[15] = 0xFF;

            psrc += 6;
            pdst += 16;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::IYU1_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width * 12 / 8;

    flipped ^= true;
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
            int32_t blue = u_table[psrc[0]];
            int32_t green = uv_table[psrc[0]][psrc[3]];
            int32_t red = v_table[psrc[3]];

            int32_t Y = luminance_table[psrc[1]];
            pdst[0] = saturation_table[Y + blue];
            pdst[1] = saturation_table[Y + green];
            pdst[2] = saturation_table[Y + red];

            Y = luminance_table[psrc[2]];
            pdst[3] = saturation_table[Y + blue];
            pdst[4] = saturation_table[Y + green];
            pdst[5] = saturation_table[Y + red];

            Y = luminance_table[psrc[4]];
            pdst[6] = saturation_table[Y + blue];
            pdst[7] = saturation_table[Y + green];
            pdst[8] = saturation_table[Y + red];

            Y = luminance_table[psrc[5]];
            pdst[9] = saturation_table[Y + blue];
            pdst[10] = saturation_table[Y + green];
            pdst[11] = saturation_table[Y + red];

            psrc += 6;
            pdst += 12;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::IYU1_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 12 / 8;

    flipped ^= true;
    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = u_table[psrc[0]];
            int32_t green = uv_table[psrc[0]][psrc[3]];
            int32_t red = v_table[psrc[3]];

            int32_t Y = luminance_table[psrc[1]];
            PackRGB565Word(pdst[0], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[2]];
            PackRGB565Word(pdst[1], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[4]];
            PackRGB565Word(pdst[2], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[5]];
            PackRGB565Word(pdst[3], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            psrc += 6;
            pdst += 4;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::IYU1_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 12 / 8;

    flipped ^= true;
    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t blue = u_table[psrc[0]];
            int32_t green = uv_table[psrc[0]][psrc[3]];
            int32_t red = v_table[psrc[3]];

            int32_t Y = luminance_table[psrc[1]];
            PackRGB555Word(pdst[0], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[2]];
            PackRGB555Word(pdst[1], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[4]];
            PackRGB555Word(pdst[2], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            Y = luminance_table[psrc[5]];
            PackRGB555Word(pdst[3], saturation_table[Y + red],
                saturation_table[Y + green],
                saturation_table[Y + blue]);

            psrc += 6;
            pdst += 4;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// IYU2 (IEEE 1394 Digital Camera 1.04 spec, mode 2) format to RGB
//

void blipvert::IYU2_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 3;

    flipped ^= true;
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
            int32_t Yprime = luminance_table[psrc[1]];
            int32_t U = psrc[0];
            int32_t V = psrc[2];
            pdst[0] = saturation_table[Yprime + u_table[U]];        // blue
            pdst[1] = saturation_table[Yprime + uv_table[U][V]];    // green
            pdst[2] = saturation_table[Yprime + v_table[V]];        // red
            pdst[3] = 0xFF;

            psrc += 3;
            pdst += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::IYU2_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width * 3;

    flipped ^= true;
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
            int32_t Yprime = luminance_table[psrc[1]];
            int32_t U = psrc[0];
            int32_t V = psrc[2];
            pdst[0] = saturation_table[Yprime + u_table[U]];        // blue
            pdst[1] = saturation_table[Yprime + uv_table[U][V]];    // green
            pdst[2] = saturation_table[Yprime + v_table[V]];        // red

            psrc += 3;
            pdst += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::IYU2_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 3;

    flipped ^= true;
    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t Yprime = luminance_table[psrc[1]];
            int32_t U = psrc[0];
            int32_t V = psrc[2];
            PackRGB565Word(*pdst++, saturation_table[Yprime + v_table[V]],
                saturation_table[Yprime + uv_table[U][V]],
                saturation_table[Yprime + u_table[U]]);

            psrc += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::IYU2_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 3;

    flipped ^= true;
    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t Yprime = luminance_table[psrc[1]];
            int32_t U = psrc[0];
            int32_t V = psrc[2];
            PackRGB555Word(*pdst++, saturation_table[Yprime + v_table[V]],
                saturation_table[Yprime + uv_table[U][V]],
                saturation_table[Yprime + u_table[U]]);
            psrc += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// YVU9 format to RGB
//

void blipvert::YVU9_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

void blipvert::YVU9_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

void blipvert::YVU9_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

void blipvert::YVU9_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

//
// YUV9 format to RGB
//

void blipvert::YUV9_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

void blipvert::YUV9_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

void blipvert::YUV9_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

void blipvert::YUV9_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

//
// IYUV format to RGB
//

void blipvert::IYUV_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, true);
}

void blipvert::IYUV_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, true);
}

void blipvert::IYUV_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, true);
}

void blipvert::IYUV_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, true);
}

//
// YV12 format to RGB
//

void blipvert::YV12_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

void blipvert::YV12_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

void blipvert::YV12_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

void blipvert::YV12_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

//
// IMC1 format to RGB
//

void blipvert::IMC1_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, false, false, true);
}

void blipvert::IMC1_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, false, false, true);
}

void blipvert::IMC1_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, false, false, true);
}

void blipvert::IMC1_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, false, false, true);
}

//
// IMC2 format to RGB
//

void blipvert::IMC2_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, false, true, true);
}

void blipvert::IMC2_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, false, true, true);
}

void blipvert::IMC2_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, false, true, true);
}

void blipvert::IMC2_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, false, true, true);
}

//
// IMC3 format to RGB
//

void blipvert::IMC3_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, true, false, true);
}

void blipvert::IMC3_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, true, false, true);
}

void blipvert::IMC3_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, true, false, true);
}

void blipvert::IMC3_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, true, false, true);
}

//
// IMC4 format to RGB
//

void blipvert::IMC4_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB32(width, height, out_buf, out_stride, in_buf, in_stride, true, true, true);
}

void blipvert::IMC4_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB24(width, height, out_buf, out_stride, in_buf, in_stride, true, true, true);
}

void blipvert::IMC4_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB565(width, height, out_buf, out_stride, in_buf, in_stride, true, true, true);
}

void blipvert::IMC4_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_RGB555(width, height, out_buf, out_stride, in_buf, in_stride, true, true, true);
}

//
// Y800 format to RGB
//

void blipvert::Y800_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (uint16_t index = 0; index < width; index++)
        {
            *reinterpret_cast<uint32_t*>(pdst) = rgb32_greyscale[psrc[index]];
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y800_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (uint16_t index = 0; index < width; index++)
        {
            *pdst++ = psrc[index];
            *pdst++ = psrc[index];
            *pdst++ = psrc[index];
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y800_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        for (uint16_t index = 0; index < width; index++)
        {
            pdst[index] = rgb565_greyscale[psrc[index]];
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y800_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        for (uint16_t index = 0; index < width; index++)
        {
            pdst[index] = rgb555_greyscale[psrc[index]];
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// Y16 format to RGB
//

void blipvert::Y16_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
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
        uint8_t* psrc = in_buf + 1; // we want the upper 8 bits of little-endian.
        uint8_t* pdst = out_buf;
        uint32_t hcount = width;
        while (hcount)
        {
            *reinterpret_cast<uint32_t*>(pdst) = rgb32_greyscale[*psrc];
            psrc += 2;
            pdst += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y16_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
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
        uint8_t* psrc = in_buf + 1; // we want the upper 8 bits of little-endian.
        uint8_t* pdst = out_buf;
        uint32_t hcount = width;
        while (hcount)
        {
            *pdst++ = *psrc;
            *pdst++ = *psrc;
            *pdst++ = *psrc;
            psrc += 2;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y16_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
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
        uint8_t* psrc = in_buf + 1; // we want the upper 8 bits of little-endian.
        uint8_t* pdst = out_buf;
        uint32_t hcount = width;
        while (hcount)
        {
            *reinterpret_cast<uint16_t*>(pdst) = rgb565_greyscale[*psrc];
            psrc += 2;
            pdst += 2;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y16_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if(!out_stride)
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
        uint8_t* psrc = in_buf + 1; // we want the upper 8 bits of little-endian.
        uint8_t* pdst = out_buf;
        uint32_t hcount = width;
        while (hcount)
        {
            *reinterpret_cast<uint16_t*>(pdst) = rgb555_greyscale[*psrc];
            psrc += 2;
            pdst += 2;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

//
// AYUV format to RGB
//

void blipvert::AYUV_to_RGBA(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD*)
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
            int32_t Y = luminance_table[psrc[2]];
            pdst[0] = saturation_table[Y + u_table[psrc[1]]];               // blue
            pdst[1] = saturation_table[Y + uv_table[psrc[1]][psrc[0]]];     // green
            pdst[2] = saturation_table[Y + v_table[psrc[0]]];               // red
            pdst[3] = psrc[3];  

            psrc += 4;
            pdst += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;

        height--;
    }
}

void blipvert::AYUV_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD*)
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
            int32_t Y = luminance_table[psrc[2]];
            pdst[0] = saturation_table[Y + u_table[psrc[1]]];               // blue
            pdst[1] = saturation_table[Y + uv_table[psrc[1]][psrc[0]]];     // green
            pdst[2] = saturation_table[Y + v_table[psrc[0]]];               // red
            pdst[3] = 0xFF;

            psrc += 4;
            pdst += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;

        height--;
    }
}

void blipvert::AYUV_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD*)
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
            int32_t Y = luminance_table[psrc[2]];
            pdst[0] = saturation_table[Y + u_table[psrc[1]]];               // blue
            pdst[1] = saturation_table[Y + uv_table[psrc[1]][psrc[0]]];     // green
            pdst[2] = saturation_table[Y + v_table[psrc[0]]];               // red

            psrc += 4;
            pdst += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;

        height--;
    }
}

void blipvert::AYUV_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD*)
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
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t Y = luminance_table[psrc[2]];
            PackRGB565Word(*pdst,
                saturation_table[Y + v_table[psrc[0]]],                 // red
                saturation_table[Y + uv_table[psrc[1]][psrc[0]]],       // green
                saturation_table[Y + u_table[psrc[1]]]);                // blue
                
            psrc += 4;
            pdst++;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;

        height--;
    }
}

void blipvert::AYUV_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD*)
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
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t Y = luminance_table[psrc[2]];
            PackRGB555Word(*pdst,
                saturation_table[Y + v_table[psrc[0]]],                 // red
                saturation_table[Y + uv_table[psrc[1]][psrc[0]]],       // green
                saturation_table[Y + u_table[psrc[1]]]);                // blue

            psrc += 4;
            pdst++;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;

        height--;
    }
}

void blipvert::AYUV_to_ARGB1555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD*)
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
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            int32_t Y = luminance_table[psrc[2]];
            PackARGB555Word(*pdst, psrc[3],
                saturation_table[Y + v_table[psrc[0]]],                 // red
                saturation_table[Y + uv_table[psrc[1]][psrc[0]]],       // green
                saturation_table[Y + u_table[psrc[1]]]);                // blue

            psrc += 4;
            pdst++;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;

        height--;
    }
}


