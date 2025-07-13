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
#include "YUVtoYUV.h"
#include "CommonMacros.h"
#include "Utilities.h"
#include "LookupTables.h"

#include <cstring>

using namespace blipvert;

//
// Local generic YUV to YUV transform functions
//

void blipvert::PackedY422_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[out_y0] = psrc[in_y0];
            pdst[out_y1] = psrc[in_y1];
            pdst[out_u] = psrc[in_u];
            pdst[out_v] = psrc[in_v];
            psrc += 4;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::PlanarYUV_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    // Copy the y plane
    if (out_y_stride != in_y_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_y_stride;
            in_buf += in_y_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_y_stride * height);
    }

    if (out_decimation == in_decimation)
    {
        // Copy the u & v planes without scaling
        if (out_uv_stride != in_uv_stride || out->flipped)
        {
            for (int32_t line = 0; line < out_uv_height; line++)
            {
                memcpy(out_uplane, in_uplane, out_uv_width);
                memcpy(out_vplane, in_vplane, out_uv_width);
                out_uplane += out_uv_stride;
                out_vplane += out_uv_stride;
                in_uplane += in_uv_stride;
                in_vplane += in_uv_stride;
            }
        }
        else
        {
            memcpy(out_uplane, in_uplane, out_uv_stride * out_uv_height);
            memcpy(out_vplane, in_vplane, out_uv_stride * out_uv_height);
        }
    }
    else if (out_decimation > in_decimation)
    {
        // Scaling from 2 to 4 decimation
        int32_t in_uv_stride_x_2 = in_uv_stride * 2;
        uint8_t* in_u_line = in_uplane;
        uint8_t* out_u_line = out_uplane;
        uint8_t* in_v_line = in_vplane;
        uint8_t* out_v_line = out_vplane;

        for (int32_t y = 0; y < in_uv_height; y += 2)
        {
            int32_t out_index = 0;
            for (int32_t x = 0; x < in_uv_width; x += 2)
            {
                out_u_line[out_index] = static_cast<uint8_t>((static_cast<uint16_t>(in_u_line[x]) + \
                    static_cast<uint16_t>(in_u_line[x + 1]) + \
                    static_cast<uint16_t>(in_u_line[x + in_uv_stride]) + \
                    static_cast<uint16_t>(in_u_line[x + in_uv_stride + 1])) >> 2);
                out_v_line[out_index] = static_cast<uint8_t>((static_cast<uint16_t>(in_v_line[x]) + \
                    static_cast<uint16_t>(in_v_line[x + 1]) + \
                    static_cast<uint16_t>(in_v_line[x + in_uv_stride]) + \
                    static_cast<uint16_t>(in_v_line[x + in_uv_stride + 1])) >> 2);
                out_index++;
            }

            in_u_line += in_uv_stride_x_2;
            in_v_line += in_uv_stride_x_2;
            out_u_line += out_uv_stride;
            out_v_line += out_uv_stride;
        }
    }
    else
    {
        // Scaling from 4 to 2 decimation
        int32_t out_uv_stride_x_2 = out_uv_stride * 2;
        uint8_t* in_up = in_uplane;
        uint8_t* in_vp = in_vplane;
        uint8_t* out_up = out_uplane;
        uint8_t* out_vp = out_vplane;
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            int32_t dst_index = 0;
            for (int32_t x = 0; x < in_uv_width; x++)
            {
                out_up[dst_index] = in_up[x];
                out_up[dst_index + out_uv_stride] = in_up[x];
                out_vp[dst_index] = in_vp[x];
                out_vp[dst_index + out_uv_stride] = in_vp[x];
                dst_index++;
                out_up[dst_index] = in_up[x];
                out_up[dst_index + out_uv_stride] = in_up[x];
                out_vp[dst_index] = in_vp[x];
                out_vp[dst_index + out_uv_stride] = in_vp[x];
                dst_index++;
            }

            out_up += out_uv_stride_x_2;
            out_vp += out_uv_stride_x_2;

            in_up += in_uv_stride;
            in_vp += in_uv_stride;
        }
    }
}

void blipvert::PackedY422_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;
 
    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t out_y_stride_x_2 = out_y_stride * 2;
    int32_t out_y_stride_x_3 = out_y_stride * 3;
    int32_t out_y_stride_x_4 = out_y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x++)
            {
                yp[0] = psrc[in_y0];
                yp[out_y_stride] = psrc[in_y0 + in_stride];
                yp[1] = psrc[in_y1];
                yp[1 + out_y_stride] = psrc[in_y1 + in_stride];

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride])) >> 1);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + \
                    static_cast<uint16_t>(psrc[in_v + in_stride])) >> 1);
                psrc += 4;
                yp += 2;
            }

            in_buf += in_stride_x_2;
            out_buf += out_y_stride_x_2;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x++)
            {
                yp[0] = psrc[in_y0];
                yp[out_y_stride] = psrc[in_y0 + in_stride];
                yp[out_y_stride_x_2] = psrc[in_y0 + in_stride_x_2];
                yp[out_y_stride_x_3] = psrc[in_y0 + in_stride_x_3];

                yp[1] = psrc[in_y1];
                yp[1 + out_y_stride] = psrc[in_y1 + in_stride];
                yp[1 + out_y_stride_x_2] = psrc[in_y1 + in_stride_x_2];
                yp[1 + out_y_stride_x_3] = psrc[in_y1 + in_stride_x_3];

                yp[2] = psrc[in_y0 + 4];
                yp[2 + out_y_stride] = psrc[in_y0 + in_stride + 4];
                yp[2 + out_y_stride_x_2] = psrc[in_y0 + in_stride_x_2 + 4];
                yp[2 + out_y_stride_x_3] = psrc[in_y0 + in_stride_x_3 + 4];

                yp[3] = psrc[in_y1 + 4];
                yp[3 + out_y_stride] = psrc[in_y1 + in_stride + 4];
                yp[3 + out_y_stride_x_2] = psrc[in_y1 + in_stride_x_2 + 4];
                yp[3 + out_y_stride_x_3] = psrc[in_y1 + in_stride_x_3 + 4];

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + \
                    static_cast<uint16_t>(psrc[in_u + 4]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride + 4]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride_x_2 + 4]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride_x_3]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride_x_3 + 4])) >> 3);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + \
                    static_cast<uint16_t>(psrc[in_v + 4]) + \
                    static_cast<uint16_t>(psrc[in_v + in_stride]) + \
                    static_cast<uint16_t>(psrc[in_v + in_stride + 4]) + \
                    static_cast<uint16_t>(psrc[in_v + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[in_v + in_stride_x_2 + 4]) + \
                    static_cast<uint16_t>(psrc[in_v + in_stride_x_3]) + \
                    static_cast<uint16_t>(psrc[in_v + in_stride_x_3 + 4])) >> 3);
                psrc += 8;
                yp += 4;
            }

            in_buf += in_stride_x_4;
            out_buf += out_y_stride_x_4;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}

void blipvert::PlanarYUV_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t in_y_stride_x_2 = in_y_stride * 2;
    int32_t in_y_stride_x_3 = in_y_stride * 3;
    int32_t in_y_stride_x_4 = in_y_stride * 4;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 2)
            {
                pdst[out_u] = *up;
                pdst[out_v] = *vp;
                pdst[out_y0] = yp[0];
                pdst[out_y1] = yp[1];

                pdst[out_u + out_stride] = *up;
                pdst[out_v + out_stride] = *vp;
                pdst[out_y0 + out_stride] = yp[in_y_stride];
                pdst[out_y1 + out_stride] = yp[1 + in_y_stride];

                pdst += 4;
                yp += 2;
                up += 1;
                vp += 1;
            }

            in_buf += in_y_stride_x_2;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                // column 1 row 1
                pdst[out_u] = *up;
                pdst[out_v] = *vp;
                pdst[out_y0] = yp[0];
                pdst[out_y1] = yp[1];

                // column 1 row 2
                pdst[out_u + out_stride] = *up;
                pdst[out_v + out_stride] = *vp;
                pdst[out_y0 + out_stride] = yp[in_y_stride];
                pdst[out_y1 + out_stride] = yp[1 + in_y_stride];

                // column 1 row 3
                pdst[out_u + out_stride_x_2] = *up;
                pdst[out_v + out_stride_x_2] = *vp;
                pdst[out_y0 + out_stride_x_2] = yp[in_y_stride_x_2];
                pdst[out_y1 + out_stride_x_2] = yp[1 + in_y_stride_x_2];

                // column 1 row 4
                pdst[out_u + out_stride_x_3] = *up;
                pdst[out_v + out_stride_x_3] = *vp;
                pdst[out_y0 + out_stride_x_3] = yp[in_y_stride_x_3];
                pdst[out_y1 + out_stride_x_3] = yp[1 + in_y_stride_x_3];

                // column 2 row 1
                pdst[out_u + 4] = *up;
                pdst[out_v + 4] = *vp;
                pdst[out_y0 + 4] = yp[2];
                pdst[out_y1 + 4] = yp[3];

                // column 2 row 2
                pdst[out_u + out_stride + 4] = *up;
                pdst[out_v + out_stride + 4] = *vp;
                pdst[out_y0 + out_stride + 4] = yp[2 + in_y_stride];
                pdst[out_y1 + out_stride + 4] = yp[3 + in_y_stride];

                // column 2 row 3
                pdst[out_u + out_stride_x_2 + 4] = *up;
                pdst[out_v + out_stride_x_2 + 4] = *vp;
                pdst[out_y0 + out_stride_x_2 + 4] = yp[2 + in_y_stride_x_2];
                pdst[out_y1 + out_stride_x_2 + 4] = yp[3 + in_y_stride_x_2];

                // column 2 row 4
                pdst[out_u + out_stride_x_3 + 4] = *up;
                pdst[out_v + out_stride_x_3 + 4] = *vp;
                pdst[out_y0 + out_stride_x_3 + 4] = yp[2 + in_y_stride_x_3];
                pdst[out_y1 + out_stride_x_3 + 4] = yp[3 + in_y_stride_x_3];

                pdst += 8;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += in_y_stride_x_4;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_4;
        }
    }
}

void blipvert::PlanarYUV_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t in_y_stride_x_2 = in_y_stride * 2;
    int32_t in_y_stride_x_3 = in_y_stride * 3;
    int32_t in_y_stride_x_4 = in_y_stride * 4;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < in_uv_height - 1; y++)
        {
            yp = in_buf;
            up = in_uplane;
            vp = in_vplane;
            pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
                pdst[3] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
                pdst[1] = yp[0];
                pdst[2] = yp[1];
                pdst[4] = yp[2];
                pdst[5] = yp[3];

                pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[1]) + \
                    static_cast<uint16_t>(up[in_uv_stride]) + \
                    static_cast<uint16_t>(up[1 + in_uv_stride])) >> 2);
                pdst[3 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[in_uv_stride]) + \
                    static_cast<uint16_t>(vp[1 + in_uv_stride])) >> 2);
                pdst[1 + out_stride] = yp[in_y_stride];
                pdst[2 + out_stride] = yp[1 + in_y_stride];
                pdst[4 + out_stride] = yp[2 + in_y_stride];
                pdst[5 + out_stride] = yp[3 + in_y_stride];

                pdst += 6;
                yp += 4;
                up += 2;
                vp += 2;
            }

            in_buf += in_y_stride_x_2;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_2;
        }

        // Get the last line without reading past the input u & v buffers
        yp = in_buf;
        up = in_uplane;
        vp = in_vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            pdst[3] = pdst[3 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            pdst[1] = yp[0];
            pdst[2] = yp[1];
            pdst[4] = yp[2];
            pdst[5] = yp[3];
            pdst[1 + out_stride] = yp[in_y_stride];
            pdst[2 + out_stride] = yp[1 + in_y_stride];
            pdst[4 + out_stride] = yp[2 + in_y_stride];
            pdst[5 + out_stride] = yp[3 + in_y_stride];

            pdst += 6;
            yp += 4;
            up += 2;
            vp += 2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < in_uv_height - 1; y++)
        {
            yp = in_buf;
            up = in_uplane;
            vp = in_vplane;
            pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                pdst[0] = up[0];
                pdst[3] = vp[0];
                pdst[1] = yp[0];
                pdst[2] = yp[1];
                pdst[4] = yp[2];
                pdst[5] = yp[3];

                pdst[out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 768) + \
                    (static_cast<int32_t>(up[in_uv_stride]) * 256)) >> 10);
                pdst[3 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 768) + \
                    (static_cast<int32_t>(vp[in_uv_stride]) * 256)) >> 10);
                pdst[1 + out_stride] = yp[in_y_stride];
                pdst[2 + out_stride] = yp[1 + in_y_stride];
                pdst[4 + out_stride] = yp[2 + in_y_stride];
                pdst[5 + out_stride] = yp[3 + in_y_stride];

                pdst[out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[in_uv_stride])) >> 1);
                pdst[3 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[in_uv_stride])) >> 1);
                pdst[1 + out_stride_x_2] = yp[in_y_stride_x_2];
                pdst[2 + out_stride_x_2] = yp[1 + in_y_stride_x_2];
                pdst[4 + out_stride_x_2] = yp[2 + in_y_stride_x_2];
                pdst[5 + out_stride_x_2] = yp[3 + in_y_stride_x_2];

                pdst[out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 256) + \
                    (static_cast<int32_t>(up[in_uv_stride]) * 768)) >> 10);
                pdst[3 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 256) + \
                    (static_cast<int32_t>(vp[in_uv_stride]) * 768)) >> 10);
                pdst[1 + out_stride_x_3] = yp[in_y_stride_x_3];
                pdst[2 + out_stride_x_3] = yp[1 + in_y_stride_x_3];
                pdst[4 + out_stride_x_3] = yp[2 + in_y_stride_x_3];
                pdst[5 + out_stride_x_3] = yp[3 + in_y_stride_x_3];

                pdst += 6;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += in_y_stride_x_4;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_4;
        }

        // Get the last line without reading past the input u & v buffers
        yp = in_buf;
        up = in_uplane;
        vp = in_vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = pdst[out_stride] = pdst[out_stride_x_2] = pdst[out_stride_x_3] = up[0];
            pdst[3] = pdst[3 + out_stride] = pdst[3 + out_stride_x_2] = pdst[3 + out_stride_x_3] = vp[0];
            pdst[1] = yp[0];
            pdst[2] = yp[1];
            pdst[4] = yp[2];
            pdst[5] = yp[3];

            pdst[1 + out_stride] = yp[in_y_stride];
            pdst[2 + out_stride] = yp[1 + in_y_stride];
            pdst[4 + out_stride] = yp[2 + in_y_stride];
            pdst[5 + out_stride] = yp[3 + in_y_stride];

            pdst[1 + out_stride_x_2] = yp[in_y_stride_x_2];
            pdst[2 + out_stride_x_2] = yp[1 + in_y_stride_x_2];
            pdst[4 + out_stride_x_2] = yp[2 + in_y_stride_x_2];
            pdst[5 + out_stride_x_2] = yp[3 + in_y_stride_x_2];

            pdst[1 + out_stride_x_3] = yp[in_y_stride_x_3];
            pdst[2 + out_stride_x_3] = yp[1 + in_y_stride_x_3];
            pdst[4 + out_stride_x_3] = yp[2 + in_y_stride_x_3];
            pdst[5 + out_stride_x_3] = yp[3 + in_y_stride_x_3];

            pdst += 6;
            yp += 4;
            up++;
            vp++;
        }
    }
}

void blipvert::IYU1_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t out_y_stride_x_2 = out_y_stride * 2;
    int32_t out_y_stride_x_3 = out_y_stride * 3;
    int32_t out_y_stride_x_4 = out_y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x += 2)
            {
                yp[0] = psrc[1];
                yp[out_y_stride] = psrc[1 + in_stride];
                yp[1] = psrc[2];
                yp[1 + out_y_stride] = psrc[2 + in_stride];

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride])) >> 1);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[3]) + \
                    static_cast<uint16_t>(psrc[3 + in_stride])) >> 1);

                yp[2] = psrc[4];
                yp[2 + out_y_stride] = psrc[4 + in_stride];
                yp[3] = psrc[5];
                yp[3 + out_y_stride] = psrc[5 + in_stride];

                out_uplane[x + 1] = out_uplane[x];
                out_vplane[x + 1] = out_vplane[x];

                psrc += 6;
                yp += 4;
            }

            in_buf += in_stride_x_2;
            out_buf += out_y_stride_x_2;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x++)
            {
                yp[0] = psrc[1];
                yp[out_y_stride] = psrc[1 + in_stride];
                yp[out_y_stride_x_2] = psrc[1 + in_stride_x_2];
                yp[out_y_stride_x_3] = psrc[1 + in_stride_x_3];

                yp[1] = psrc[2];
                yp[1 + out_y_stride] = psrc[2 + in_stride];
                yp[1 + out_y_stride_x_2] = psrc[2 + in_stride_x_2];
                yp[1 + out_y_stride_x_3] = psrc[2 + in_stride_x_3];

                yp[2] = psrc[4];
                yp[2 + out_y_stride] = psrc[4 + in_stride];
                yp[2 + out_y_stride_x_2] = psrc[4 + in_stride_x_2];
                yp[2 + out_y_stride_x_3] = psrc[4 + in_stride_x_3];

                yp[3] = psrc[5];
                yp[3 + out_y_stride] = psrc[5 + in_stride];
                yp[3 + out_y_stride_x_2] = psrc[5 + in_stride_x_2];
                yp[3 + out_y_stride_x_3] = psrc[5 + in_stride_x_3];

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_3])) >> 2);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[3]) + \
                    static_cast<uint16_t>(psrc[3 + in_stride]) + \
                    static_cast<uint16_t>(psrc[3 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[3 + in_stride_x_3])) >> 2);
                psrc += 6;
                yp += 4;
            }

            in_buf += in_stride_x_4;
            out_buf += out_y_stride_x_4;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}

void blipvert::PackedY422_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + static_cast<uint16_t>(psrc[in_u + 4])) >> 1);
            pdst[3] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + static_cast<uint16_t>(psrc[in_v + 4])) >> 1);

            pdst[1] = psrc[in_y0];
            pdst[2] = psrc[in_y1];
            pdst[4] = psrc[in_y0 + 4];
            pdst[5] = psrc[in_y1 + 4];

            psrc += 8;
            pdst += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU1_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[out_y0] = psrc[1];
            pdst[out_y1] = psrc[2];
            pdst[out_u] = psrc[0];
            pdst[out_v] = psrc[3];

            pdst[out_y0 + 4] = psrc[4];
            pdst[out_y1 + 4] = psrc[5];
            pdst[out_u + 4] = psrc[0];
            pdst[out_v + 4] = psrc[3];

            psrc += 6;
            pdst += 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::PackedY422_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[0] = psrc[in_u];
            pdst[1] = psrc[in_y0];
            pdst[2] = psrc[in_v];

            pdst[3] = psrc[in_u];
            pdst[4] = psrc[in_y1];
            pdst[5] = psrc[in_v];

            psrc += 4;
            pdst += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU2_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[out_u] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[3])) >> 1);
            pdst[out_y0] = psrc[1];
            pdst[out_v] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[5])) >> 1);
            pdst[out_y1] = psrc[4];

            psrc += 6;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::PlanarYUV_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = out->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t in_y_stride_x_2 = in_y_stride * 2;
    int32_t in_y_stride_x_4 = in_y_stride * 4;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 2)
            {
                // column 1 row 1
                pdst[0] = *up;
                pdst[1] = yp[0];
                pdst[2] = *vp;

                // column 1 row 2
                pdst[out_stride] = *up;
                pdst[1 + out_stride] = yp[in_y_stride];
                pdst[2 + out_stride] = *vp;

                // column 2 row 1
                pdst[3] = *up;
                pdst[4] = yp[1];
                pdst[5] = *vp;

                // column 2 row 2
                pdst[3 + out_stride] = *up;
                pdst[4 + out_stride] = yp[1 + in_y_stride];
                pdst[5 + out_stride] = *vp;
                pdst += 6;
                yp += 2;
                up += 1;
                vp += 1;
            }

            in_buf += in_y_stride_x_2;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                for (int32_t row = 0; row < 4; row++)
                {
                    for (int32_t col = 0; col < 4; col++)
                    {
                        pdst[col * 3 + out_stride * row] = *up;
                        pdst[1 + col * 3 + out_stride * row] = yp[col + in_y_stride * row];
                        pdst[2 + col * 3 + out_stride * row] = *vp;
                    }
                }

                pdst += 12;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += in_y_stride_x_4;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_4;
        }
    }
}

void blipvert::IYU2_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t out_y_stride_x_2 = out_y_stride * 2;
    int32_t out_y_stride_x_4 = out_y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x++)
            {
                yp[0] = psrc[1];
                yp[out_y_stride] = psrc[1 + in_stride];
                yp[1] = psrc[4];
                yp[1 + out_y_stride] = psrc[4 + in_stride];

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride]) + \
                    static_cast<uint16_t>(psrc[3]) + \
                    static_cast<uint16_t>(psrc[3 + in_stride])) >> 2);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride]) + \
                    static_cast<uint16_t>(psrc[5]) + \
                    static_cast<uint16_t>(psrc[5 + in_stride])) >> 2);

                psrc += 6;
                yp += 2;
            }

            in_buf += in_stride_x_2;
            out_buf += out_y_stride_x_2;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 4)
            {
                uint16_t u_avg = 0;
                uint16_t v_avg = 0;

                int32_t is = 0;
                int32_t ys = 0;

                for (int32_t dec = 0; dec < 4; dec++)
                {
                    u_avg += psrc[is];
                    yp[ys] = psrc[1 + is];
                    v_avg += psrc[2 + is];

                    u_avg += psrc[3 + is];
                    yp[ys + 1] = psrc[4 + is];
                    v_avg += psrc[5 + is];

                    u_avg += psrc[6 + is];
                    yp[ys + 2] = psrc[7 + is];
                    v_avg += psrc[8 + is];

                    u_avg += psrc[9 + is];
                    yp[ys + 3] = psrc[10 + is];
                    v_avg += psrc[11 + is];

                    is += in_stride;
                    ys += out_y_stride;
                }

                u_avg >>= 4;
                v_avg >>= 4;

                out_uplane[x >> 2] = static_cast<uint8_t>(u_avg);
                out_vplane[x >> 2] = static_cast<uint8_t>(v_avg);

                psrc += 12;
                yp += 4;
            }
            in_buf += (in_stride_x_4);
            out_buf += (out_y_stride_x_4);

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}

void blipvert::IYU2_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;

        for (int32_t x = 0; x < out_uv_width; x++)
        {
            yp[0] = psrc[1];
            yp[out_stride] = psrc[1 + in_stride];
            yp[1] = psrc[4];
            yp[1 + out_stride] = psrc[4 + in_stride];

            out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[in_stride]) + \
                static_cast<uint16_t>(psrc[3]) + \
                static_cast<uint16_t>(psrc[3 + in_stride])) >> 2);

            out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                static_cast<uint16_t>(psrc[2 + in_stride]) + \
                static_cast<uint16_t>(psrc[5]) + \
                static_cast<uint16_t>(psrc[5 + in_stride])) >> 2);

            psrc += 6;
            yp += 2;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        out_uplane += out_stride;
        out_vplane += out_stride;
    }
}

void blipvert::IMCx_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int32_t y = 0; y < in_uv_height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 2)
        {
            // column 1 row 1
            pdst[0] = *up;
            pdst[1] = yp[0];
            pdst[2] = *vp;

            // column 1 row 2
            pdst[out_stride] = *up;
            pdst[1 + out_stride] = yp[in_stride];
            pdst[2 + out_stride] = *vp;

            // column 2 row 1
            pdst[3] = *up;
            pdst[4] = yp[1];
            pdst[5] = *vp;

            // column 2 row 2
            pdst[3 + out_stride] = *up;
            pdst[4 + out_stride] = yp[1 + in_stride];
            pdst[5 + out_stride] = *vp;
            pdst += 6;
            yp += 2;
            up += 1;
            vp += 1;
        }

        in_buf += in_stride_x_2;
        in_uplane += in_stride;
        in_vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void blipvert::PackedY422_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + static_cast<uint16_t>(psrc[in_u + 4])) >> 1);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + static_cast<uint16_t>(psrc[in_v + 4])) >> 1);

            pdst[1] = psrc[in_y0];
            pdst[3] = psrc[in_y1];
            pdst[5] = psrc[in_y0 + 4];
            pdst[7] = psrc[in_y1 + 4];

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u + 8]) + static_cast<uint16_t>(psrc[in_u + 12])) >> 1);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v + 8]) + static_cast<uint16_t>(psrc[in_v + 12])) >> 1);

            pdst[8] = psrc[in_y0 + 8];
            pdst[9] = psrc[in_y1 + 8];
            pdst[10] = psrc[in_y0 + 12];
            pdst[11] = psrc[in_y1 + 12];

            psrc += 16;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::PackedY422_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + static_cast<uint16_t>(psrc[in_u + 4])) >> 1);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + static_cast<uint16_t>(psrc[in_v + 4])) >> 1);

            pdst[1] = psrc[in_y0] | 0x01;
            pdst[3] = psrc[in_y1] | 0x01;
            pdst[5] = psrc[in_y0 + 4] | 0x01;
            pdst[7] = psrc[in_y1 + 4] | 0x01;

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u + 8]) + static_cast<uint16_t>(psrc[in_u + 12])) >> 1);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v + 8]) + static_cast<uint16_t>(psrc[in_v + 12])) >> 1);

            pdst[8] = psrc[in_y0 + 8] | 0x01;
            pdst[9] = psrc[in_y1 + 8] | 0x01;
            pdst[10] = psrc[in_y0 + 12] | 0x01;
            pdst[11] = psrc[in_y1 + 12] | 0x01;

            psrc += 16;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::PlanarYUV_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t in_y_stride_x_2 = in_y_stride * 2;
    int32_t in_y_stride_x_3 = in_y_stride * 3;
    int32_t in_y_stride_x_4 = in_y_stride * 4;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < in_uv_height - 1; y++)
        {
            yp = in_buf;
            up = in_uplane;
            vp = in_vplane;
            pdst = out_buf;

            for (int32_t x = 0; x < width; x += 8)
            {
                pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
                pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
                pdst[1] = yp[0];
                pdst[3] = yp[1];
                pdst[5] = yp[2];
                pdst[7] = yp[3];

                pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[1]) + \
                    static_cast<uint16_t>(up[in_uv_stride]) + \
                    static_cast<uint16_t>(up[1 + in_uv_stride])) >> 2);
                pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[in_uv_stride]) + \
                    static_cast<uint16_t>(vp[1 + in_uv_stride])) >> 2);
                pdst[1 + out_stride] = yp[in_y_stride];
                pdst[3 + out_stride] = yp[1 + in_y_stride];
                pdst[5 + out_stride] = yp[2 + in_y_stride];
                pdst[7 + out_stride] = yp[3 + in_y_stride];

                pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
                pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
                pdst[8] = yp[4];
                pdst[9] = yp[5];
                pdst[10] = yp[6];
                pdst[11] = yp[7];

                pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + \
                    static_cast<uint16_t>(up[3]) + \
                    static_cast<uint16_t>(up[2 + in_uv_stride]) + \
                    static_cast<uint16_t>(up[3 + in_uv_stride])) >> 2);
                pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + \
                    static_cast<uint16_t>(vp[3]) + \
                    static_cast<uint16_t>(vp[2 + in_uv_stride]) + \
                    static_cast<uint16_t>(vp[3 + in_uv_stride])) >> 2);
                pdst[8 + out_stride] = yp[4 + in_y_stride];
                pdst[9 + out_stride] = yp[5 + in_y_stride];
                pdst[10 + out_stride] = yp[6 + in_y_stride];
                pdst[11 + out_stride] = yp[7 + in_y_stride];

                pdst += 12;
                yp += 8;
                up += 4;
                vp += 4;
            }

            in_buf += in_y_stride_x_2;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_2;
        }

        // Get the last line without reading past the u & v buffers
        yp = in_buf;
        up = in_uplane;
        vp = in_vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            pdst[2] = pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            pdst[1] = yp[0];
            pdst[3] = yp[1];
            pdst[5] = yp[2];
            pdst[7] = yp[3];

            pdst[1 + out_stride] = yp[in_y_stride];
            pdst[3 + out_stride] = yp[1 + in_y_stride];
            pdst[5 + out_stride] = yp[2 + in_y_stride];
            pdst[7 + out_stride] = yp[3 + in_y_stride];

            pdst[4] = pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
            pdst[6] = pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
            pdst[8] = yp[4];
            pdst[9] = yp[5];
            pdst[10] = yp[6];
            pdst[11] = yp[7];

            pdst[8 + out_stride] = yp[4 + in_y_stride];
            pdst[9 + out_stride] = yp[5 + in_y_stride];
            pdst[10 + out_stride] = yp[6 + in_y_stride];
            pdst[11 + out_stride] = yp[7 + in_y_stride];

            pdst += 12;
            yp += 8;
            up += 4;
            vp += 4;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < in_uv_height - 1; y++)
        {
            yp = in_buf;
            up = in_uplane;
            vp = in_vplane;
            pdst = out_buf;

            for (int32_t x = 0; x < width; x += 8)
            {
                pdst[0] = up[0];
                pdst[2] = vp[0];
                pdst[1] = yp[0];
                pdst[3] = yp[1];
                pdst[5] = yp[2];
                pdst[7] = yp[3];

                pdst[out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 768) + \
                    (static_cast<int32_t>(up[in_uv_stride]) * 256)) >> 10);
                pdst[2 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 768) + \
                    (static_cast<int32_t>(vp[in_uv_stride]) * 256)) >> 10);
                pdst[1 + out_stride] = yp[in_y_stride];
                pdst[3 + out_stride] = yp[1 + in_y_stride];
                pdst[5 + out_stride] = yp[2 + in_y_stride];
                pdst[7 + out_stride] = yp[3 + in_y_stride];

                pdst[out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[in_uv_stride])) >> 1);
                pdst[2 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[in_uv_stride])) >> 1);
                pdst[1 + out_stride_x_2] = yp[in_y_stride_x_2];
                pdst[3 + out_stride_x_2] = yp[1 + in_y_stride_x_2];
                pdst[5 + out_stride_x_2] = yp[2 + in_y_stride_x_2];
                pdst[7 + out_stride_x_2] = yp[3 + in_y_stride_x_2];

                pdst[out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 256) + \
                    (static_cast<int32_t>(up[in_uv_stride]) * 768)) >> 10);
                pdst[2 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 256) + \
                    (static_cast<int32_t>(vp[in_uv_stride]) * 768)) >> 10);
                pdst[1 + out_stride_x_3] = yp[in_y_stride_x_3];
                pdst[3 + out_stride_x_3] = yp[1 + in_y_stride_x_3];
                pdst[5 + out_stride_x_3] = yp[2 + in_y_stride_x_3];
                pdst[7 + out_stride_x_3] = yp[3 + in_y_stride_x_3];

                pdst[4] = up[1];
                pdst[6] = vp[1];
                pdst[8] = yp[4];
                pdst[9] = yp[5];
                pdst[10] = yp[6];
                pdst[11] = yp[7];

                pdst[4 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(up[1]) * 768) + \
                    (static_cast<int32_t>(up[1 + in_uv_stride]) * 256)) >> 10);
                pdst[6 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(vp[1]) * 768) + \
                    (static_cast<int32_t>(vp[1 + in_uv_stride]) * 256)) >> 10);
                pdst[8 + out_stride] = yp[4 + in_y_stride];
                pdst[9 + out_stride] = yp[5 + in_y_stride];
                pdst[10 + out_stride] = yp[6 + in_y_stride];
                pdst[11 + out_stride] = yp[7 + in_y_stride];

                pdst[4 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(up[1]) + \
                    static_cast<uint16_t>(up[1 + in_uv_stride])) >> 1);
                pdst[6 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[1 + in_uv_stride])) >> 1);
                pdst[8 + out_stride_x_2] = yp[4 + in_y_stride_x_2];
                pdst[9 + out_stride_x_2] = yp[5 + in_y_stride_x_2];
                pdst[10 + out_stride_x_2] = yp[6 + in_y_stride_x_2];
                pdst[11 + out_stride_x_2] = yp[7 + in_y_stride_x_2];

                pdst[4 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(up[1]) * 256) + \
                    (static_cast<int32_t>(up[1 + in_uv_stride]) * 768)) >> 10);
                pdst[6 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(vp[1]) * 256) + \
                    (static_cast<int32_t>(vp[1 + in_uv_stride]) * 768)) >> 10);
                pdst[8 + out_stride_x_3] = yp[4 + in_y_stride_x_3];
                pdst[9 + out_stride_x_3] = yp[5 + in_y_stride_x_3];
                pdst[10 + out_stride_x_3] = yp[6 + in_y_stride_x_3];
                pdst[11 + out_stride_x_3] = yp[7 + in_y_stride_x_3];

                pdst += 12;
                yp += 8;
                up += 2;
                vp += 2;
            }

            in_buf += in_y_stride_x_4;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_4;
        }

        // Get the last line without reading past the u & v buffers
        yp = in_buf;
        up = in_uplane;
        vp = in_vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = pdst[out_stride] = pdst[out_stride_x_2] = pdst[out_stride_x_3] = up[0];
            pdst[2] = pdst[2 + out_stride] = pdst[2 + out_stride_x_2] = pdst[2 + out_stride_x_3] = vp[0];
            pdst[1] = yp[0];
            pdst[3] = yp[1];
            pdst[5] = yp[2];
            pdst[7] = yp[3];

            pdst[1 + out_stride] = yp[in_y_stride];
            pdst[3 + out_stride] = yp[1 + in_y_stride];
            pdst[5 + out_stride] = yp[2 + in_y_stride];
            pdst[7 + out_stride] = yp[3 + in_y_stride];

            pdst[1 + out_stride_x_2] = yp[in_y_stride_x_2];
            pdst[3 + out_stride_x_2] = yp[1 + in_y_stride_x_2];
            pdst[5 + out_stride_x_2] = yp[2 + in_y_stride_x_2];
            pdst[7 + out_stride_x_2] = yp[3 + in_y_stride_x_2];

            pdst[1 + out_stride_x_3] = yp[in_y_stride_x_3];
            pdst[3 + out_stride_x_3] = yp[1 + in_y_stride_x_3];
            pdst[5 + out_stride_x_3] = yp[2 + in_y_stride_x_3];
            pdst[7 + out_stride_x_3] = yp[3 + in_y_stride_x_3];

            pdst[4] = pdst[4 + out_stride] = pdst[4 + out_stride_x_2] = pdst[4 + out_stride_x_3] = up[1];
            pdst[6] = pdst[6 + out_stride] = pdst[6 + out_stride_x_2] = pdst[6 + out_stride_x_3] = vp[1];
            pdst[8] = yp[4];
            pdst[9] = yp[5];
            pdst[10] = yp[6];
            pdst[11] = yp[7];

            pdst[8 + out_stride] = yp[4 + in_y_stride];
            pdst[9 + out_stride] = yp[5 + in_y_stride];
            pdst[10 + out_stride] = yp[6 + in_y_stride];
            pdst[11 + out_stride] = yp[7 + in_y_stride];

            pdst[8 + out_stride_x_2] = yp[4 + in_y_stride_x_2];
            pdst[9 + out_stride_x_2] = yp[5 + in_y_stride_x_2];
            pdst[10 + out_stride_x_2] = yp[6 + in_y_stride_x_2];
            pdst[11 + out_stride_x_2] = yp[7 + in_y_stride_x_2];

            pdst[8 + out_stride_x_3] = yp[4 + in_y_stride_x_3];
            pdst[9 + out_stride_x_3] = yp[5 + in_y_stride_x_3];
            pdst[10 + out_stride_x_3] = yp[6 + in_y_stride_x_3];
            pdst[11 + out_stride_x_3] = yp[7 + in_y_stride_x_3];

            pdst += 12;
            yp += 8;
            up += 2;
            vp += 2;
        }
    }
}

void blipvert::PlanarYUV_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t in_y_stride_x_2 = in_y_stride * 2;
    int32_t in_y_stride_x_3 = in_y_stride * 3;
    int32_t in_y_stride_x_4 = in_y_stride * 4;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < in_uv_height - 1; y++)
        {
            yp = in_buf;
            up = in_uplane;
            vp = in_vplane;
            pdst = out_buf;

            for (int32_t x = 0; x < width; x += 8)
            {
                pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
                pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
                pdst[1] = yp[0] | 0x01;
                pdst[3] = yp[1] | 0x01;
                pdst[5] = yp[2] | 0x01;
                pdst[7] = yp[3] | 0x01;

                pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[1]) + \
                    static_cast<uint16_t>(up[in_uv_stride]) + \
                    static_cast<uint16_t>(up[1 + in_uv_stride])) >> 2);
                pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[in_uv_stride]) + \
                    static_cast<uint16_t>(vp[1 + in_uv_stride])) >> 2);
                pdst[1 + out_stride] = yp[in_y_stride] | 0x01;
                pdst[3 + out_stride] = yp[1 + in_y_stride] | 0x01;
                pdst[5 + out_stride] = yp[2 + in_y_stride] | 0x01;
                pdst[7 + out_stride] = yp[3 + in_y_stride] | 0x01;

                pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
                pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
                pdst[8] = yp[4] | 0x01;
                pdst[9] = yp[5] | 0x01;
                pdst[10] = yp[6] | 0x01;
                pdst[11] = yp[7] | 0x01;

                pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + \
                    static_cast<uint16_t>(up[3]) + \
                    static_cast<uint16_t>(up[2 + in_uv_stride]) + \
                    static_cast<uint16_t>(up[3 + in_uv_stride])) >> 2);
                pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + \
                    static_cast<uint16_t>(vp[3]) + \
                    static_cast<uint16_t>(vp[2 + in_uv_stride]) + \
                    static_cast<uint16_t>(vp[3 + in_uv_stride])) >> 2);
                pdst[8 + out_stride] = yp[4 + in_y_stride] | 0x01;
                pdst[9 + out_stride] = yp[5 + in_y_stride] | 0x01;
                pdst[10 + out_stride] = yp[6 + in_y_stride] | 0x01;
                pdst[11 + out_stride] = yp[7 + in_y_stride] | 0x01;

                pdst += 12;
                yp += 8;
                up += 4;
                vp += 4;
            }

            in_buf += in_y_stride_x_2;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_2;
        }

        // Get the last line without reading past the u & v buffers
        yp = in_buf;
        up = in_uplane;
        vp = in_vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            pdst[2] = pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            pdst[1] = yp[0] | 0x01;
            pdst[3] = yp[1] | 0x01;
            pdst[5] = yp[2] | 0x01;
            pdst[7] = yp[3] | 0x01;

            pdst[1 + out_stride] = yp[in_y_stride] | 0x01;
            pdst[3 + out_stride] = yp[1 + in_y_stride] | 0x01;
            pdst[5 + out_stride] = yp[2 + in_y_stride] | 0x01;
            pdst[7 + out_stride] = yp[3 + in_y_stride] | 0x01;

            pdst[4] = pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
            pdst[6] = pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
            pdst[8] = yp[4] | 0x01;
            pdst[9] = yp[5] | 0x01;
            pdst[10] = yp[6] | 0x01;
            pdst[11] = yp[7] | 0x01;

            pdst[8 + out_stride] = yp[4 + in_y_stride] | 0x01;
            pdst[9 + out_stride] = yp[5 + in_y_stride] | 0x01;
            pdst[10 + out_stride] = yp[6 + in_y_stride] | 0x01;
            pdst[11 + out_stride] = yp[7 + in_y_stride] | 0x01;

            pdst += 12;
            yp += 8;
            up += 4;
            vp += 4;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < in_uv_height - 1; y++)
        {
            yp = in_buf;
            up = in_uplane;
            vp = in_vplane;
            pdst = out_buf;

            for (int32_t x = 0; x < width; x += 8)
            {
                pdst[0] = up[0];
                pdst[2] = vp[0];
                pdst[1] = yp[0] | 0x01;
                pdst[3] = yp[1] | 0x01;
                pdst[5] = yp[2] | 0x01;
                pdst[7] = yp[3] | 0x01;

                pdst[out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 768) + \
                    (static_cast<int32_t>(up[in_uv_stride]) * 256)) >> 10);
                pdst[2 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 768) + \
                    (static_cast<int32_t>(vp[in_uv_stride]) * 256)) >> 10);
                pdst[1 + out_stride] = yp[in_y_stride] | 0x01;
                pdst[3 + out_stride] = yp[1 + in_y_stride] | 0x01;
                pdst[5 + out_stride] = yp[2 + in_y_stride] | 0x01;
                pdst[7 + out_stride] = yp[3 + in_y_stride] | 0x01;

                pdst[out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[in_uv_stride])) >> 1);
                pdst[2 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[in_uv_stride])) >> 1);
                pdst[1 + out_stride_x_2] = yp[in_y_stride_x_2] | 0x01;
                pdst[3 + out_stride_x_2] = yp[1 + in_y_stride_x_2] | 0x01;
                pdst[5 + out_stride_x_2] = yp[2 + in_y_stride_x_2] | 0x01;
                pdst[7 + out_stride_x_2] = yp[3 + in_y_stride_x_2] | 0x01;

                pdst[out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 256) + \
                    (static_cast<int32_t>(up[in_uv_stride]) * 768)) >> 10);
                pdst[2 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 256) + \
                    (static_cast<int32_t>(vp[in_uv_stride]) * 768)) >> 10);
                pdst[1 + out_stride_x_3] = yp[in_y_stride_x_3] | 0x01;
                pdst[3 + out_stride_x_3] = yp[1 + in_y_stride_x_3] | 0x01;
                pdst[5 + out_stride_x_3] = yp[2 + in_y_stride_x_3] | 0x01;
                pdst[7 + out_stride_x_3] = yp[3 + in_y_stride_x_3] | 0x01;

                pdst[4] = up[1];
                pdst[6] = vp[1];
                pdst[8] = yp[4] | 0x01;
                pdst[9] = yp[5] | 0x01;
                pdst[10] = yp[6] | 0x01;
                pdst[11] = yp[7] | 0x01;

                pdst[4 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(up[1]) * 768) + \
                    (static_cast<int32_t>(up[1 + in_uv_stride]) * 256)) >> 10);
                pdst[6 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(vp[1]) * 768) + \
                    (static_cast<int32_t>(vp[1 + in_uv_stride]) * 256)) >> 10);
                pdst[8 + out_stride] = yp[4 + in_y_stride] | 0x01;
                pdst[9 + out_stride] = yp[5 + in_y_stride] | 0x01;
                pdst[10 + out_stride] = yp[6 + in_y_stride] | 0x01;
                pdst[11 + out_stride] = yp[7 + in_y_stride] | 0x01;

                pdst[4 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(up[1]) + \
                    static_cast<uint16_t>(up[1 + in_uv_stride])) >> 1);
                pdst[6 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[1 + in_uv_stride])) >> 1);
                pdst[8 + out_stride_x_2] = yp[4 + in_y_stride_x_2] | 0x01;
                pdst[9 + out_stride_x_2] = yp[5 + in_y_stride_x_2] | 0x01;
                pdst[10 + out_stride_x_2] = yp[6 + in_y_stride_x_2] | 0x01;
                pdst[11 + out_stride_x_2] = yp[7 + in_y_stride_x_2] | 0x01;

                pdst[4 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(up[1]) * 256) + \
                    (static_cast<int32_t>(up[1 + in_uv_stride]) * 768)) >> 10);
                pdst[6 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(vp[1]) * 256) + \
                    (static_cast<int32_t>(vp[1 + in_uv_stride]) * 768)) >> 10);
                pdst[8 + out_stride_x_3] = yp[4 + in_y_stride_x_3] | 0x01;
                pdst[9 + out_stride_x_3] = yp[5 + in_y_stride_x_3] | 0x01;
                pdst[10 + out_stride_x_3] = yp[6 + in_y_stride_x_3] | 0x01;
                pdst[11 + out_stride_x_3] = yp[7 + in_y_stride_x_3] | 0x01;

                pdst += 12;
                yp += 8;
                up += 2;
                vp += 2;
            }

            in_buf += in_y_stride_x_4;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_4;
        }

        // Get the last line without reading past the u & v buffers
        yp = in_buf;
        up = in_uplane;
        vp = in_vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = pdst[out_stride] = pdst[out_stride_x_2] = pdst[out_stride_x_3] = up[0];
            pdst[2] = pdst[2 + out_stride] = pdst[2 + out_stride_x_2] = pdst[2 + out_stride_x_3] = vp[0];
            pdst[1] = yp[0] | 0x01;
            pdst[3] = yp[1] | 0x01;
            pdst[5] = yp[2] | 0x01;
            pdst[7] = yp[3] | 0x01;

            pdst[1 + out_stride] = yp[in_y_stride] | 0x01;
            pdst[3 + out_stride] = yp[1 + in_y_stride] | 0x01;
            pdst[5 + out_stride] = yp[2 + in_y_stride] | 0x01;
            pdst[7 + out_stride] = yp[3 + in_y_stride] | 0x01;

            pdst[1 + out_stride_x_2] = yp[in_y_stride_x_2] | 0x01;
            pdst[3 + out_stride_x_2] = yp[1 + in_y_stride_x_2] | 0x01;
            pdst[5 + out_stride_x_2] = yp[2 + in_y_stride_x_2] | 0x01;
            pdst[7 + out_stride_x_2] = yp[3 + in_y_stride_x_2] | 0x01;

            pdst[1 + out_stride_x_3] = yp[in_y_stride_x_3] | 0x01;
            pdst[3 + out_stride_x_3] = yp[1 + in_y_stride_x_3] | 0x01;
            pdst[5 + out_stride_x_3] = yp[2 + in_y_stride_x_3] | 0x01;
            pdst[7 + out_stride_x_3] = yp[3 + in_y_stride_x_3] | 0x01;

            pdst[4] = pdst[4 + out_stride] = pdst[4 + out_stride_x_2] = pdst[4 + out_stride_x_3] = up[1];
            pdst[6] = pdst[6 + out_stride] = pdst[6 + out_stride_x_2] = pdst[6 + out_stride_x_3] = vp[1];
            pdst[8] = yp[4] | 0x01;
            pdst[9] = yp[5] | 0x01;
            pdst[10] = yp[6] | 0x01;
            pdst[11] = yp[7] | 0x01;

            pdst[8 + out_stride] = yp[4 + in_y_stride] | 0x01;
            pdst[9 + out_stride] = yp[5 + in_y_stride] | 0x01;
            pdst[10 + out_stride] = yp[6 + in_y_stride] | 0x01;
            pdst[11 + out_stride] = yp[7 + in_y_stride] | 0x01;

            pdst[8 + out_stride_x_2] = yp[4 + in_y_stride_x_2] | 0x01;
            pdst[9 + out_stride_x_2] = yp[5 + in_y_stride_x_2] | 0x01;
            pdst[10 + out_stride_x_2] = yp[6 + in_y_stride_x_2] | 0x01;
            pdst[11 + out_stride_x_2] = yp[7 + in_y_stride_x_2] | 0x01;

            pdst[8 + out_stride_x_3] = yp[4 + in_y_stride_x_3] | 0x01;
            pdst[9 + out_stride_x_3] = yp[5 + in_y_stride_x_3] | 0x01;
            pdst[10 + out_stride_x_3] = yp[6 + in_y_stride_x_3] | 0x01;
            pdst[11 + out_stride_x_3] = yp[7 + in_y_stride_x_3] | 0x01;

            pdst += 12;
            yp += 8;
            up += 2;
            vp += 2;
        }
    }
}

void blipvert::PackedY422_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t x = 0; x < width; x += 4)
        {
            uint8_t u_val = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + static_cast<uint16_t>(psrc[in_u + 4])) >> 1);
            uint8_t v_val = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + static_cast<uint16_t>(psrc[in_v + 4])) >> 1);
            PackCLJRDword(*pdst++, u_val, v_val, psrc[in_y0], psrc[in_y1], psrc[in_y0 + 4], psrc[in_y1 + 4])
                psrc += 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::PlanarYUV_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t in_y_stride_x_2 = in_y_stride * 2;
    int32_t in_y_stride_x_3 = in_y_stride * 3;
    int32_t in_y_stride_x_4 = in_y_stride * 4;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                uint8_t u_val = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
                uint8_t v_val = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst)), u_val, v_val, yp[0], yp[1], yp[2], yp[3])
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride)), u_val, v_val, yp[in_y_stride], yp[1 + in_y_stride], yp[2 + in_y_stride], yp[3 + in_y_stride])

                pdst += 4;
                yp += 4;
                up += 2;
                vp += 2;
            }

            in_buf += in_y_stride_x_2;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst)), *up, *vp, yp[0], yp[1], yp[2], yp[3])
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride)), *up, *vp, yp[in_y_stride], yp[1 + in_y_stride], yp[2 + in_y_stride], yp[3 + in_y_stride])
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride_x_2)), *up, *vp, yp[in_y_stride_x_2], yp[1 + in_y_stride_x_2], yp[2 + in_y_stride_x_2], yp[3 + in_y_stride_x_2])
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride_x_3)), *up, *vp, yp[in_y_stride_x_3], yp[1 + in_y_stride_x_3], yp[2 + in_y_stride_x_3], yp[3 + in_y_stride_x_3])
                pdst += 4;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += in_y_stride_x_4;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_4;
        }
    }
}

void blipvert::PackedY422_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[0] = psrc[in_y0];
            pdst[1] = psrc[in_y1];

            psrc += 4;
            pdst += 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::PackedY422_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 2)
        {
            *pdst++ = 0x00;
            *pdst++ = psrc[in_y0];
            *pdst++ = 0x00;
            *pdst++ = psrc[in_y1];

            psrc += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::PlanarYUV_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    // Copy the y plane

    if (out_stride != in_y_stride || out->flipped)
    {
        for (int32_t y = 0; y < height; y++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_y_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, in_y_stride * height);
    }
}

void blipvert::PlanarYUV_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    // Copy the y plane

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x++)
        {
            *pdst++ = 0x00;
            *pdst++ = *psrc++;
        }

        in_buf += in_y_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y42T_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[out_y0] = psrc[1] & 0xFE;
            pdst[out_y1] = psrc[3] & 0xFE;
            pdst[out_u] = psrc[0];
            pdst[out_v] = psrc[2];
            psrc += 4;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}


void blipvert::PackedY422_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[1] = psrc[in_y0] | 0x01;
            pdst[3] = psrc[in_y1] | 0x01;
            pdst[0] = psrc[in_u];
            pdst[2] = psrc[in_v];
            psrc += 4;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y42T_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t out_y_stride_x_2 = out_y_stride * 2;
    int32_t out_y_stride_x_3 = out_y_stride * 3;
    int32_t out_y_stride_x_4 = out_y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x++)
            {
                yp[0] = psrc[1] & 0xFE;
                yp[out_y_stride] = psrc[1 + in_stride] & 0xFE;
                yp[1] = psrc[3] & 0xFE;
                yp[1 + out_y_stride] = psrc[3 + in_stride] & 0xFE;

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride])) >> 1);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);
                psrc += 4;
                yp += 2;
            }

            in_buf += in_stride_x_2;
            out_buf += out_y_stride_x_2;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x++)
            {
                yp[0] = psrc[1] & 0xFE;
                yp[out_y_stride] = psrc[1 + in_stride] & 0xFE;
                yp[out_y_stride_x_2] = psrc[1 + in_stride_x_2] & 0xFE;
                yp[out_y_stride_x_3] = psrc[1 + in_stride_x_3] & 0xFE;

                yp[1] = psrc[3] & 0xFE;
                yp[1 + out_y_stride] = psrc[3 + in_stride] & 0xFE;
                yp[1 + out_y_stride_x_2] = psrc[3 + in_stride_x_2] & 0xFE;
                yp[1 + out_y_stride_x_3] = psrc[3 + in_stride_x_3] & 0xFE;

                yp[2] = psrc[5] & 0xFE;
                yp[2 + out_y_stride] = psrc[5 + in_stride] & 0xFE;
                yp[2 + out_y_stride_x_2] = psrc[5 + in_stride_x_2] & 0xFE;
                yp[2 + out_y_stride_x_3] = psrc[5 + in_stride_x_3] & 0xFE;

                yp[3] = psrc[7] & 0xFE;
                yp[3 + out_y_stride] = psrc[7 + in_stride] & 0xFE;
                yp[3 + out_y_stride_x_2] = psrc[7 + in_stride_x_2] & 0xFE;
                yp[3 + out_y_stride_x_3] = psrc[7 + in_stride_x_3] & 0xFE;

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[4]) + \
                    static_cast<uint16_t>(psrc[in_stride]) + \
                    static_cast<uint16_t>(psrc[in_stride + 4]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_2 + 4]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_3]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_3 + 4])) >> 3);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                    static_cast<uint16_t>(psrc[6]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride_x_3]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride_x_3])) >> 3);
                psrc += 8;
                yp += 4;
            }

            in_buf += in_stride_x_4;
            out_buf += out_y_stride_x_4;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}

void blipvert::PlanarYUV_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;
    
    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t in_y_stride_x_2 = in_y_stride * 2;
    int32_t in_y_stride_x_3 = in_y_stride * 3;
    int32_t in_y_stride_x_4 = in_y_stride * 4;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 2)
            {
                pdst[0] = *up;
                pdst[2] = *vp;
                pdst[1] = yp[0] | 0x01;
                pdst[3] = yp[1] | 0x01;

                pdst[out_stride] = *up;
                pdst[2 + out_stride] = *vp;
                pdst[1 + out_stride] = yp[in_y_stride] | 0x01;
                pdst[3 + out_stride] = yp[1 + in_y_stride] | 0x01;

                pdst += 4;
                yp += 2;
                up += 1;
                vp += 1;
            }

            in_buf += in_y_stride_x_2;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                // column 1 row 1
                pdst[0] = *up;
                pdst[2] = *vp;
                pdst[1] = yp[0] | 0x01;
                pdst[3] = yp[1] | 0x01;

                // column 1 row 2
                pdst[out_stride] = *up;
                pdst[2 + out_stride] = *vp;
                pdst[1 + out_stride] = yp[in_y_stride] | 0x01;
                pdst[3 + out_stride] = yp[1 + in_y_stride] | 0x01;

                // column 1 row 3
                pdst[out_stride_x_2] = *up;
                pdst[2 + out_stride_x_2] = *vp;
                pdst[1 + out_stride_x_2] = yp[in_y_stride_x_2] | 0x01;
                pdst[3 + out_stride_x_2] = yp[1 + in_y_stride_x_2] | 0x01;

                // column 1 row 4
                pdst[out_stride_x_3] = *up;
                pdst[2 + out_stride_x_3] = *vp;
                pdst[1 + out_stride_x_3] = yp[in_y_stride_x_3] | 0x01;
                pdst[3 + out_stride_x_3] = yp[1 + in_y_stride_x_3] | 0x01;

                // column 2 row 1
                pdst[4] = *up;
                pdst[6] = *vp;
                pdst[5] = yp[2] | 0x01;
                pdst[7] = yp[3] | 0x01;

                // column 2 row 2
                pdst[out_stride + 4] = *up;
                pdst[6 + out_stride] = *vp;
                pdst[5 + out_stride] = yp[2 + in_y_stride] | 0x01;
                pdst[7 + out_stride] = yp[3 + in_y_stride] | 0x01;

                // column 2 row 3
                pdst[out_stride_x_2 + 4] = *up;
                pdst[6 + out_stride_x_2] = *vp;
                pdst[5 + out_stride_x_2] = yp[2 + in_y_stride_x_2] | 0x01;
                pdst[7 + out_stride_x_2] = yp[3 + in_y_stride_x_2] | 0x01;

                // column 2 row 4
                pdst[out_stride_x_3 + 4] = *up;
                pdst[6 + out_stride_x_3] = *vp;
                pdst[5 + out_stride_x_3] = yp[2 + in_y_stride_x_3] | 0x01;
                pdst[7 + out_stride_x_3] = yp[3 + in_y_stride_x_3] | 0x01;

                pdst += 8;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += in_y_stride_x_4;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_4;
        }
    }
}

//
// Y800 to YUV
//

void blipvert::Y800_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[out_y0] = psrc[0];
            pdst[out_y1] = psrc[1];
            pdst[out_u] = 0;
            pdst[out_v] = 0;
            psrc += 2;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y800_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;

    if (out_y_stride == in_stride)
    {
        memcpy(out_buf, in_buf, out_y_stride * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_y_stride;
            in_buf += in_stride;
        }
    }
}

void blipvert::Y800_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        memset(out_uplane, 0, out_uv_width);
        memset(out_vplane, 0, out_uv_width);
        out_uplane += out_stride;
        out_vplane += out_stride;
    }

    if (out_stride == in_stride)
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_stride;
        }
    }
}

void blipvert::IMCx_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    // Copy the y plane

    if (out_stride != in_stride || out->flipped)
    {
        for (int32_t y = 0; y < height; y++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }
}

//
// Y16 to YUV
//

void blipvert::Y16_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[out_y0] = psrc[1];
            pdst[out_y1] = psrc[3];
            pdst[out_u] = 0;
            pdst[out_v] = 0;
            psrc += 4;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y16_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf + 1;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x++)
        {
            *pdst++ = *psrc;
            psrc += 2;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;
    }
}

void blipvert::Y16_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        memset(out_uplane, 0, out_uv_width);
        memset(out_vplane, 0, out_uv_width);
        out_uplane += out_stride;
        out_vplane += out_stride;
    }

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf + 1;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x++)
        {
            *pdst++ = *psrc;
            psrc += 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IMCx_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    // Copy the y plane

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x++)
        {
            *pdst++ = 0x00;
            *pdst++ = *psrc++;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

//
// Y41P to YUV
//

void blipvert::Y41P_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[out_u] = psrc[0];          // U0
            pdst[out_v] = psrc[2];          // V0
            pdst[out_y0] = psrc[1];         // Y0
            pdst[out_y1] = psrc[3];         // Y1

            pdst[out_u + 4] = psrc[0];      // U0
            pdst[out_v + 4] = psrc[2];      // V0
            pdst[out_y0 + 4] = psrc[5];     // Y2
            pdst[out_y1 + 4] = psrc[7];     // Y3

            pdst[out_u + 8] = psrc[4];      // U4
            pdst[out_v + 8] = psrc[6];      // V4
            pdst[out_y0 + 8] = psrc[8];     // Y4
            pdst[out_y1 + 8] = psrc[9];     // Y5

            pdst[out_u + 12] = psrc[4];     // U4
            pdst[out_v + 12] = psrc[6];     // V4
            pdst[out_y0 + 12] = psrc[10];   // Y6
            pdst[out_y1 + 12] = psrc[11];   // Y7   

            psrc += 12;
            pdst += 16;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t out_y_stride_x_2 = out_y_stride * 2;
    int32_t out_y_stride_x_3 = out_y_stride * 3;
    int32_t out_y_stride_x_4 = out_y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x += 4)
            {
                yp[0] = psrc[1];
                yp[out_y_stride] = psrc[1 + in_stride];
                yp[1] = psrc[3];
                yp[1 + out_y_stride] = psrc[3 + in_stride];

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride])) >> 1);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);

                yp[2] = psrc[5];
                yp[2 + out_y_stride] = psrc[5 + in_stride];
                yp[3] = psrc[7];
                yp[3 + out_y_stride] = psrc[7 + in_stride];

                out_uplane[x + 1] = out_uplane[x];
                out_vplane[x + 1] = out_vplane[x];

                yp[4] = psrc[8];
                yp[4 + out_y_stride] = psrc[8 + in_stride];
                yp[5] = psrc[9];
                yp[5 + out_y_stride] = psrc[9 + in_stride];

                out_uplane[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride])) >> 1);

                out_vplane[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride])) >> 1);

                yp[6] = psrc[10];
                yp[6 + out_y_stride] = psrc[10 + in_stride];
                yp[7] = psrc[11];
                yp[7 + out_y_stride] = psrc[11 + in_stride];

                out_uplane[x + 3] = out_uplane[x + 2];
                out_vplane[x + 3] = out_vplane[x + 2];

                psrc += 12;
                yp += 8;
            }

            in_buf += in_stride_x_2;
            out_buf += out_y_stride_x_2;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x += 2)
            {
                yp[0] = psrc[1];
                yp[out_y_stride] = psrc[1 + in_stride];
                yp[out_y_stride_x_2] = psrc[1 + in_stride_x_2];
                yp[out_y_stride_x_3] = psrc[1 + in_stride_x_3];

                yp[1] = psrc[3];
                yp[1 + out_y_stride] = psrc[3 + in_stride];
                yp[1 + out_y_stride_x_2] = psrc[3 + in_stride_x_2];
                yp[1 + out_y_stride_x_3] = psrc[3 + in_stride_x_3];

                yp[2] = psrc[5];
                yp[2 + out_y_stride] = psrc[5 + in_stride];
                yp[2 + out_y_stride_x_2] = psrc[5 + in_stride_x_2];
                yp[2 + out_y_stride_x_3] = psrc[5 + in_stride_x_3];

                yp[3] = psrc[7];
                yp[3 + out_y_stride] = psrc[7 + in_stride];
                yp[3 + out_y_stride_x_2] = psrc[7 + in_stride_x_2];
                yp[3 + out_y_stride_x_3] = psrc[7 + in_stride_x_3];

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_3])) >> 2);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride_x_3])) >> 2);

                yp[4] = psrc[8];
                yp[4 + out_y_stride] = psrc[8 + in_stride];
                yp[4 + out_y_stride_x_2] = psrc[8 + in_stride_x_2];
                yp[4 + out_y_stride_x_3] = psrc[8 + in_stride_x_3];

                yp[5] = psrc[9];
                yp[5 + out_y_stride] = psrc[9 + in_stride];
                yp[5 + out_y_stride_x_2] = psrc[9 + in_stride_x_2];
                yp[5 + out_y_stride_x_3] = psrc[9 + in_stride_x_3];

                yp[6] = psrc[10];
                yp[6 + out_y_stride] = psrc[10 + in_stride];
                yp[6 + out_y_stride_x_2] = psrc[10 + in_stride_x_2];
                yp[6 + out_y_stride_x_3] = psrc[10 + in_stride_x_3];

                yp[7] = psrc[11];
                yp[7 + out_y_stride] = psrc[11 + in_stride];
                yp[7 + out_y_stride_x_2] = psrc[11 + in_stride_x_2];
                yp[7 + out_y_stride_x_3] = psrc[11 + in_stride_x_3];

                out_uplane[x + 1] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride_x_3])) >> 2);

                out_vplane[x + 1] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride_x_3])) >> 2);

                psrc += 12;
                yp += 8;
            }

            in_buf += in_stride_x_4;
            out_buf += out_y_stride_x_4;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}

void blipvert::Y41P_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;

        for (int32_t x = 0; x < out_uv_width; x += 4)
        {
            yp[0] = psrc[1];
            yp[out_stride] = psrc[1 + in_stride];
            yp[1] = psrc[3]; 
            yp[1 + out_stride] = psrc[3 + in_stride];

            out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[in_stride])) >> 1);

            out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);

            yp[2] = psrc[5];
            yp[2 + out_stride] = psrc[5 + in_stride];
            yp[3] = psrc[7];
            yp[3 + out_stride] = psrc[7 + in_stride];

            out_uplane[x + 1] = out_uplane[x];
            out_vplane[x + 1] = out_vplane[x];

            yp[4] = psrc[8];
            yp[4 + out_stride] = psrc[8 + in_stride];
            yp[5] = psrc[9];
            yp[5 + out_stride] = psrc[9 + in_stride];

            out_uplane[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                static_cast<uint16_t>(psrc[4 + in_stride])) >> 1);

            out_vplane[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                static_cast<uint16_t>(psrc[6 + in_stride])) >> 1);

            yp[6] = psrc[10];
            yp[6 + out_stride] = psrc[10 + in_stride];
            yp[7] = psrc[11];
            yp[7 + out_stride] = psrc[11 + in_stride];

            out_uplane[x + 3] = out_uplane[x + 2];
            out_vplane[x + 3] = out_vplane[x + 2];

            psrc += 12;
            yp += 8;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        out_uplane += out_stride;
        out_vplane += out_stride;
    }
}

void blipvert::IMCx_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t uv_width = in->uv_width;
    int32_t uv_height = in->uv_slice_height;
    uint8_t* uplane = in->uplane;
    uint8_t* vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    for (int32_t y = 0; y < height - 2; y += 2)
    {
        yp = in_buf;
        up = uplane;
        vp = vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            pdst[1] = yp[0];
            pdst[3] = yp[1];
            pdst[5] = yp[2];
            pdst[7] = yp[3];

            pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                static_cast<uint16_t>(up[1]) + \
                static_cast<uint16_t>(up[in_stride]) + \
                static_cast<uint16_t>(up[1 + in_stride])) >> 2);
            pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                static_cast<uint16_t>(vp[1]) + \
                static_cast<uint16_t>(vp[in_stride]) + \
                static_cast<uint16_t>(vp[1 + in_stride])) >> 2);
            pdst[1 + out_stride] = yp[in_stride];
            pdst[3 + out_stride] = yp[1 + in_stride];
            pdst[5 + out_stride] = yp[2 + in_stride];
            pdst[7 + out_stride] = yp[3 + in_stride];

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
            pdst[8] = yp[4];
            pdst[9] = yp[5];
            pdst[10] = yp[6];
            pdst[11] = yp[7];

            pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + \
                static_cast<uint16_t>(up[3]) + \
                static_cast<uint16_t>(up[2 + in_stride]) + \
                static_cast<uint16_t>(up[3 + in_stride])) >> 2);
            pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + \
                static_cast<uint16_t>(vp[3]) + \
                static_cast<uint16_t>(vp[2 + in_stride]) + \
                static_cast<uint16_t>(vp[3 + in_stride])) >> 2);
            pdst[8 + out_stride] = yp[4 + in_stride];
            pdst[9 + out_stride] = yp[5 + in_stride];
            pdst[10 + out_stride] = yp[6 + in_stride];
            pdst[11 + out_stride] = yp[7 + in_stride];

            pdst += 12;
            yp += 8;
            up += 4;
            vp += 4;
        }

        in_buf += in_stride_x_2;
        uplane += in_stride;
        vplane += in_stride;
        out_buf += out_stride_x_2;
    }

    // Get the last line without reading past the u & v buffers
    yp = in_buf;
    up = uplane;
    vp = vplane;
    pdst = out_buf;

    for (int32_t x = 0; x < width; x += 8)
    {
        pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
        pdst[2] = pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
        pdst[1] = yp[0];
        pdst[3] = yp[1];
        pdst[5] = yp[2];
        pdst[7] = yp[3];

        pdst[1 + out_stride] = yp[in_stride];
        pdst[3 + out_stride] = yp[1 + in_stride];
        pdst[5 + out_stride] = yp[2 + in_stride];
        pdst[7 + out_stride] = yp[3 + in_stride];

        pdst[4] = pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
        pdst[6] = pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
        pdst[8] = yp[4];
        pdst[9] = yp[5];
        pdst[10] = yp[6];
        pdst[11] = yp[7];

        pdst[8 + out_stride] = yp[4 + in_stride];
        pdst[9 + out_stride] = yp[5 + in_stride];
        pdst[10 + out_stride] = yp[6 + in_stride];
        pdst[11 + out_stride] = yp[7 + in_stride];

        pdst += 12;
        yp += 8;
        up += 4;
        vp += 4;
    }
}

void blipvert::IMCx_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t uv_width = in->uv_width;
    int32_t uv_height = in->uv_slice_height;
    uint8_t* uplane = in->uplane;
    uint8_t* vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    for (int32_t y = 0; y < height - 2; y += 2)
    {
        yp = in_buf;
        up = uplane;
        vp = vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            pdst[1] = yp[0] | 0x01;
            pdst[3] = yp[1] | 0x01;
            pdst[5] = yp[2] | 0x01;
            pdst[7] = yp[3] | 0x01;

            pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                static_cast<uint16_t>(up[1]) + \
                static_cast<uint16_t>(up[in_stride]) + \
                static_cast<uint16_t>(up[1 + in_stride])) >> 2);
            pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                static_cast<uint16_t>(vp[1]) + \
                static_cast<uint16_t>(vp[in_stride]) + \
                static_cast<uint16_t>(vp[1 + in_stride])) >> 2);
            pdst[1 + out_stride] = yp[in_stride] | 0x01;
            pdst[3 + out_stride] = yp[1 + in_stride] | 0x01;
            pdst[5 + out_stride] = yp[2 + in_stride] | 0x01;
            pdst[7 + out_stride] = yp[3 + in_stride] | 0x01;

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
            pdst[8] = yp[4] | 0x01;
            pdst[9] = yp[5] | 0x01;
            pdst[10] = yp[6] | 0x01;
            pdst[11] = yp[7] | 0x01;

            pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + \
                static_cast<uint16_t>(up[3]) + \
                static_cast<uint16_t>(up[2 + in_stride]) + \
                static_cast<uint16_t>(up[3 + in_stride])) >> 2);
            pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + \
                static_cast<uint16_t>(vp[3]) + \
                static_cast<uint16_t>(vp[2 + in_stride]) + \
                static_cast<uint16_t>(vp[3 + in_stride])) >> 2);
            pdst[8 + out_stride] = yp[4 + in_stride] | 0x01;
            pdst[9 + out_stride] = yp[5 + in_stride] | 0x01;
            pdst[10 + out_stride] = yp[6 + in_stride] | 0x01;
            pdst[11 + out_stride] = yp[7 + in_stride] | 0x01;

            pdst += 12;
            yp += 8;
            up += 4;
            vp += 4;
        }

        in_buf += in_stride_x_2;
        uplane += in_stride;
        vplane += in_stride;
        out_buf += out_stride_x_2;
    }

    // Get the last line without reading past the u & v buffers
    yp = in_buf;
    up = uplane;
    vp = vplane;
    pdst = out_buf;

    for (int32_t x = 0; x < width; x += 8)
    {
        pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
        pdst[2] = pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
        pdst[1] = yp[0] | 0x01;
        pdst[3] = yp[1] | 0x01;
        pdst[5] = yp[2] | 0x01;
        pdst[7] = yp[3] | 0x01;

        pdst[1 + out_stride] = yp[in_stride] | 0x01;
        pdst[3 + out_stride] = yp[1 + in_stride] | 0x01;
        pdst[5 + out_stride] = yp[2 + in_stride] | 0x01;
        pdst[7 + out_stride] = yp[3 + in_stride] | 0x01;

        pdst[4] = pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
        pdst[6] = pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
        pdst[8] = yp[4] | 0x01;
        pdst[9] = yp[5] | 0x01;
        pdst[10] = yp[6] | 0x01;
        pdst[11] = yp[7] | 0x01;

        pdst[8 + out_stride] = yp[4 + in_stride] | 0x01;
        pdst[9 + out_stride] = yp[5 + in_stride] | 0x01;
        pdst[10 + out_stride] = yp[6 + in_stride] | 0x01;
        pdst[11 + out_stride] = yp[7 + in_stride] | 0x01;

        pdst += 12;
        yp += 8;
        up += 4;
        vp += 4;
    }
}

//
// CLJR to YUV
//

void blipvert::CLJR_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t width_m_4 = width - 4;
    uint32_t mpixel;

    for (int32_t y = 0; y < height; y++)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width_m_4; x += 4)
        {
            mpixel = *psrc++;
            pdst[out_u] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            pdst[out_v] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
            pdst[out_y0] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
            pdst[out_y1] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));

            pdst[out_u + 4] = static_cast<uint8_t>((static_cast<uint16_t>(pdst[out_u]) + static_cast<uint16_t>(UnpackCLJR_U(*psrc))) >> 1);
            pdst[out_v + 4] = static_cast<uint8_t>((static_cast<uint16_t>(pdst[out_v]) + static_cast<uint16_t>(UnpackCLJR_V(*psrc))) >> 1);
            pdst[out_y0 + 4] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
            pdst[out_y1 + 4] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));

            pdst += 8;
        }

        // Get last macropixel to prevent reading beyond line
        mpixel = *psrc;
        pdst[out_u] = pdst[out_u + 4] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
        pdst[out_v] = pdst[out_v + 4] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
        pdst[out_y0] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
        pdst[out_y1] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
        pdst[out_y0 + 4] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
        pdst[out_y1 + 4] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::CLJR_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t y_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;

        for (int32_t x = 0; x < out_uv_width; x += 2)
        {
            uint32_t mpixel_top = *(reinterpret_cast<uint32_t*>(psrc));
            uint32_t mpixel_bot = *(reinterpret_cast<uint32_t*>(psrc + in_stride));

            yp[0] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel_top));
            yp[out_stride] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel_bot));
            yp[1] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel_top));
            yp[1 + out_stride] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel_bot));

            out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_U(mpixel_top)) + \
                static_cast<uint16_t>(UnpackCLJR_U(mpixel_bot))) >> 1);

            out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_V(mpixel_top)) + \
                static_cast<uint16_t>(UnpackCLJR_V(mpixel_bot))) >> 1);

            yp[2] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel_top));
            yp[2 + out_stride] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel_bot));
            yp[3] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel_top));
            yp[3 + out_stride] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel_bot));

            out_uplane[x + 1] = out_uplane[x];
            out_vplane[x + 1] = out_vplane[x];

            psrc += 4;
            yp += 4;
        }

        in_buf += in_stride_x_2;
        out_buf += y_stride_x_2;

        out_uplane += out_stride;
        out_vplane += out_stride;
    }
}

void blipvert::IMCx_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int32_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            uint8_t u_val = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            uint8_t v_val = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst)), u_val, v_val, yp[0], yp[1], yp[2], yp[3])
            PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride)), u_val, v_val, yp[in_stride], yp[1 + in_stride], yp[2 + in_stride], yp[3 + in_stride])

            pdst += 4;
            yp += 4;
            up += 2;
            vp += 2;
        }

        in_buf += in_stride_x_2;
        in_uplane += in_stride;
        in_vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void blipvert::CLJR_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t out_y_stride_x_2 = out_y_stride * 2;
    int32_t out_y_stride_x_3 = out_y_stride * 3;
    int32_t out_y_stride_x_4 = out_y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x += 2)
            {
                uint32_t mpixel_top = *(reinterpret_cast<uint32_t*>(psrc));
                uint32_t mpixel_bot = *(reinterpret_cast<uint32_t*>(psrc + in_stride));

                yp[0] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel_top));
                yp[out_y_stride] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel_bot));
                yp[1] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel_top));
                yp[1 + out_y_stride] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel_bot));

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_U(mpixel_top)) + \
                    static_cast<uint16_t>(UnpackCLJR_U(mpixel_bot))) >> 1);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_V(mpixel_top)) + \
                    static_cast<uint16_t>(UnpackCLJR_V(mpixel_bot))) >> 1);

                yp[2] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel_top));
                yp[2 + out_y_stride] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel_bot));
                yp[3] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel_top));
                yp[3 + out_y_stride] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel_bot));

                out_uplane[x + 1] = out_uplane[x];
                out_vplane[x + 1] = out_vplane[x];

                psrc += 4;
                yp += 4;
            }

            in_buf += in_stride_x_2;
            out_buf += out_y_stride_x_2;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x++)
            {
                uint32_t mpixel0 = *(reinterpret_cast<uint32_t*>(psrc));
                uint32_t mpixel1 = *(reinterpret_cast<uint32_t*>(psrc + in_stride));
                uint32_t mpixel2 = *(reinterpret_cast<uint32_t*>(psrc + in_stride_x_2));
                uint32_t mpixel3 = *(reinterpret_cast<uint32_t*>(psrc + in_stride_x_3));

                yp[0] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel0));
                yp[out_y_stride] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel1));
                yp[out_y_stride_x_2] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel2));
                yp[out_y_stride_x_3] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel3));

                yp[1] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel0));
                yp[1 + out_y_stride] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel1));
                yp[1 + out_y_stride_x_2] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel2));
                yp[1 + out_y_stride_x_3] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel3));

                yp[2] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel0));
                yp[2 + out_y_stride] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel1));
                yp[2 + out_y_stride_x_2] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel2));
                yp[2 + out_y_stride_x_3] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel3));

                yp[3] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel0));
                yp[3 + out_y_stride] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel1));
                yp[3 + out_y_stride_x_2] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel2));
                yp[3 + out_y_stride_x_3] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel3));

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_U(mpixel0)) + \
                    static_cast<uint16_t>(UnpackCLJR_U(mpixel1)) + \
                    static_cast<uint16_t>(UnpackCLJR_U(mpixel2)) + \
                    static_cast<uint16_t>(UnpackCLJR_U(mpixel3))) >> 2);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_V(mpixel0)) + \
                    static_cast<uint16_t>(UnpackCLJR_V(mpixel1)) + \
                    static_cast<uint16_t>(UnpackCLJR_V(mpixel2)) + \
                    static_cast<uint16_t>(UnpackCLJR_V(mpixel3))) >> 2);
                psrc += 4;
                yp += 4;
            }

            in_buf += in_stride_x_4;
            out_buf += out_y_stride_x_4;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}


void blipvert::AYUV_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[out_v] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) / 2);
            pdst[out_u] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[1]) + static_cast<uint16_t>(psrc[5])) / 2);
            pdst[out_y0] = psrc[2];
            pdst[out_y1] = psrc[6];
            psrc += 8;
            pdst += 4;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::PackedY422_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = psrc[in_v];
            pdst[1] = psrc[in_u];
            pdst[2] = psrc[in_y0];
            pdst[3] = 0xFF;

            pdst[4] = psrc[in_v];
            pdst[5] = psrc[in_u];
            pdst[6] = psrc[in_y1];
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

void blipvert::AYUV_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    uint16_t v_average;
    uint16_t u_average;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 2)
            {
                v_average = psrc[0];
                u_average = psrc[1];
                yp[0] = psrc[2];

                v_average += psrc[4];
                u_average += psrc[5];
                yp[1] = psrc[6];

                v_average += psrc[0 + in_stride];
                u_average += psrc[1 + in_stride];
                yp[out_y_stride] = psrc[2 + in_stride];

                v_average += psrc[4 + in_stride];
                u_average += psrc[5 + in_stride];
                yp[out_y_stride + 1] = psrc[6 + in_stride];

                v_average >>= 2;
                u_average >>= 2;

                out_vplane[x >> 1] = static_cast<uint8_t>(v_average);
                out_uplane[x >> 1] = static_cast<uint8_t>(u_average);

                psrc += 8;
                yp += 2;
            }
            in_buf += (in_stride * 2);
            out_buf += (out_y_stride * 2);

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 4)
            {
                v_average = 0;
                u_average = 0;

                int32_t is = 0;
                int32_t ys = 0;

                for (int32_t dec = 0; dec < 4; dec++)
                {
                    v_average += psrc[0 + is];
                    u_average += psrc[1 + is];
                    yp[ys] = psrc[2 + is];

                    v_average += psrc[4 + is];
                    u_average += psrc[5 + is];
                    yp[ys + 1] = psrc[6 + is];

                    v_average += psrc[8 + is];
                    u_average += psrc[9 + is];
                    yp[ys + 2] = psrc[10 + is];

                    v_average += psrc[12 + is];
                    u_average += psrc[13 + is];
                    yp[ys + 3] = psrc[14 + is];

                    is += in_stride;
                    ys += out_y_stride;
                }

                v_average >>= 4;
                u_average >>= 4;

                out_vplane[x >> 2] = static_cast<uint8_t>(v_average);
                out_uplane[x >> 2] = static_cast<uint8_t>(u_average);

                psrc += 16;
                yp += 4;
            }
            in_buf += (in_stride * 4);
            out_buf += (out_y_stride * 4);

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}

void blipvert::PlanarYUV_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t in_y_stride_x_2 = in_y_stride * 2;
    int32_t in_y_stride_x_4 = in_y_stride * 4;

    if (in_decimation == 2)
    {
        for (int16_t y = 0; y < height; y += 2)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 2)
            {
                // column 1 row 1
                pdst[0] = *vp;
                pdst[1] = *up;
                pdst[2] = yp[0];
                pdst[3] = 0xFF;

                // column 1 row 2
                pdst[out_stride] = *vp;
                pdst[1 + out_stride] = *up;
                pdst[2 + out_stride] = yp[in_y_stride];
                pdst[3 + out_stride] = 0xFF;

                // column 2 row 1
                pdst[4] = *vp;
                pdst[5] = up[0];
                pdst[6] = yp[1];
                pdst[7] = 0xFF;

                // column 2 row 2
                pdst[4 + out_stride] = *vp;
                pdst[5 + out_stride] = *up;
                pdst[6 + out_stride] = yp[1 + in_y_stride];
                pdst[7 + out_stride] = 0xFF;

                pdst += 8;
                yp += 2;
                up++;
                vp++;
            }

            in_buf += in_y_stride_x_2;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int16_t y = 0; y < height; y += 4)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = in_uplane;
            uint8_t* vp = in_vplane;
            uint8_t* pdst = out_buf;

            for (int16_t x = 0; x < width; x += 4)
            {
                for (int16_t row = 0; row < 4; row++)
                {
                    for (int16_t col = 0; col < 4; col++)
                    {
                        pdst[col * 4 + out_stride * row] = *vp;
                        pdst[1 + col * 4 + out_stride * row] = *up;
                        pdst[2 + col * 4 + out_stride * row] = yp[col + in_y_stride * row];
                        pdst[3 + col * 4 + out_stride * row] = 0xFF;
                    }
                }

                pdst += 16;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += in_y_stride_x_4;
            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
            out_buf += out_stride_x_4;
        }
    }
}

void blipvert::AYUV_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < height; y += 2)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            yp[0] = psrc[2];
            uint16_t U = psrc[1];
            uint16_t V = psrc[0];

            yp[1] = psrc[6];
            U += psrc[5];
            V += psrc[4];

            yp[out_stride] = psrc[2 + in_stride];
            U += psrc[1 + in_stride];
            V += psrc[0 + in_stride];

            yp[out_stride + 1] = psrc[6 + in_stride];
            U += psrc[5 + in_stride];
            V += psrc[4 + in_stride];

            out_uplane[x >> 1] = U >> 2;
            out_vplane[x >> 1] = V >> 2;

            psrc += 8;
            yp += 2;
        }
        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        out_uplane += out_stride;
        out_vplane += out_stride;
    }
}

void blipvert::IMCx_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int16_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int16_t x = 0; x < width; x += 2)
        {
            // column 1 row 1
            pdst[0] = *vp;
            pdst[1] = *up;
            pdst[2] = yp[0];
            pdst[3] = 0xFF;

            // column 1 row 2
            pdst[out_stride] = *vp;
            pdst[1 + out_stride] = *up;
            pdst[2 + out_stride] = yp[in_stride];
            pdst[3 + out_stride] = 0xFF;

            // column 2 row 1
            pdst[4] = *vp;
            pdst[5] = *up;
            pdst[6] = yp[1];
            pdst[7] = 0xFF;

            // column 2 row 2
            pdst[4 + out_stride] = *vp;
            pdst[5 + out_stride] = *up;
            pdst[6 + out_stride] = yp[1 + in_stride];
            pdst[7 + out_stride] = 0xFF;

            pdst += 8;
            yp += 2;
            up++;
            vp++;
        }

        in_buf += in_stride_x_2;
        in_uplane += in_stride;
        in_vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void blipvert::PackedY422_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;

        for (int32_t x = 0; x < out_uv_width; x++)
        {
            yp[0] = psrc[in_y0];
            yp[out_stride] = psrc[in_y0 + in_stride];
            yp[1] = psrc[in_y1];
            yp[1 + out_stride] = psrc[in_y1 + in_stride];

            out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + \
                static_cast<uint16_t>(psrc[in_u + in_stride])) >> 1);

            out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + \
                static_cast<uint16_t>(psrc[in_v + in_stride])) >> 1);
            psrc += 4;
            yp += 2;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        out_uplane += out_stride;
        out_vplane += out_stride;
    }
}

void blipvert::IMCx_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int32_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[out_u] = *up;
            pdst[out_v] = *vp;
            pdst[out_y0] = yp[0];
            pdst[out_y1] = yp[1];

            pdst[out_u + out_stride] = *up;
            pdst[out_v + out_stride] = *vp;
            pdst[out_y0 + out_stride] = yp[in_stride];
            pdst[out_y1 + out_stride] = yp[1 + in_stride];

            pdst += 4;
            yp += 2;
            up += 1;
            vp += 1;
        }

        in_buf += in_stride_x_2;
        in_uplane += in_stride;
        in_vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void blipvert::PlanarYUV_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    // Copy the y plane
    if (out_stride != in_y_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_y_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }

    if (in_decimation == 2)
    {
        // Copy the u & v planes without scaling
        if (out_stride != in_uv_stride || out->flipped || out->interlaced)
        {
            for (int32_t line = 0; line < out_uv_height; line++)
            {
                memcpy(out_uplane, in_uplane, out_uv_width);
                memcpy(out_vplane, in_vplane, out_uv_width);
                out_uplane += out_stride;
                out_vplane += out_stride;
                in_uplane += in_uv_stride;
                in_vplane += in_uv_stride;
            }
        }
        else
        {
            memcpy(out_uplane, in_uplane, out_stride * out_uv_height);
            memcpy(out_vplane, in_vplane, out_stride * out_uv_height);
        }
    }
    else
    {
        // Scaling from 4 to 2 decimation
        int32_t out_uv_stride_x_2 = out_stride * 2;
        uint8_t* in_up = in_uplane;
        uint8_t* in_vp = in_vplane;
        uint8_t* out_up = out_uplane;
        uint8_t* out_vp = out_vplane;
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            int32_t dst_index = 0;
            for (int32_t x = 0; x < in_uv_width; x++)
            {
                out_up[dst_index] = in_up[x];
                out_up[dst_index + out_stride] = in_up[x];
                out_vp[dst_index] = in_vp[x];
                out_vp[dst_index + out_stride] = in_vp[x];
                dst_index++;
                out_up[dst_index] = in_up[x];
                out_up[dst_index + out_stride] = in_up[x];
                out_vp[dst_index] = in_vp[x];
                out_vp[dst_index + out_stride] = in_vp[x];
                dst_index++;
            }

            out_up += out_uv_stride_x_2;
            out_vp += out_uv_stride_x_2;

            in_up += in_uv_stride;
            in_vp += in_uv_stride;
        }
    }
}

void blipvert::IMCx_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    // Copy the y plane
    if (out_y_stride != in_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_y_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_y_stride * height);
    }

    if (out_decimation == 2)
    {
        // Copy the u & v planes without scaling
        if (out_uv_stride != in_stride || out->flipped || in->interlaced)
        {
            for (int32_t line = 0; line < out_uv_height; line++)
            {
                memcpy(out_uplane, in_uplane, out_uv_width);
                memcpy(out_vplane, in_vplane, out_uv_width);
                out_uplane += out_uv_stride;
                out_vplane += out_uv_stride;
                in_uplane += in_stride;
                in_vplane += in_stride;
            }
        }
        else
        {
            memcpy(out_uplane, in_uplane, out_uv_stride * out_uv_height);
            memcpy(out_vplane, in_vplane, out_uv_stride * out_uv_height);
        }
    }
    else
    {
        // Scaling from 2 to 4 decimation
        int32_t in_uv_stride_x_2 = in_stride * 2;

        for (int32_t y = 0; y < in_uv_height; y += 2)
        {
            int32_t out_index = 0;
            for (int32_t x = 0; x < in_uv_width; x += 2)
            {
                out_uplane[out_index] = static_cast<uint8_t>((static_cast<uint16_t>(in_uplane[x]) + \
                    static_cast<uint16_t>(in_uplane[x + 1]) + \
                    static_cast<uint16_t>(in_uplane[x + in_stride]) + \
                    static_cast<uint16_t>(in_uplane[x + in_stride + 1])) >> 2);
                out_vplane[out_index] = static_cast<uint8_t>((static_cast<uint16_t>(in_vplane[x]) + \
                    static_cast<uint16_t>(in_vplane[x + 1]) + \
                    static_cast<uint16_t>(in_vplane[x + in_stride]) + \
                    static_cast<uint16_t>(in_vplane[x + in_stride + 1])) >> 2);
                out_index++;
            }

            in_uplane += in_uv_stride_x_2;
            in_vplane += in_uv_stride_x_2;
            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}

void blipvert::IYU1_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;

        for (int32_t x = 0; x < out_uv_width; x += 2)
        {
            yp[0] = psrc[1];
            yp[out_stride] = psrc[1 + in_stride];
            yp[1] = psrc[2];
            yp[1 + out_stride] = psrc[2 + in_stride];

            out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[in_stride])) >> 1);

            out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[3]) + \
                static_cast<uint16_t>(psrc[3 + in_stride])) >> 1);

            yp[2] = psrc[4];
            yp[2 + out_stride] = psrc[4 + in_stride];
            yp[3] = psrc[5];
            yp[3 + out_stride] = psrc[5 + in_stride];

            out_uplane[x + 1] = out_uplane[x];
            out_vplane[x + 1] = out_vplane[x];

            psrc += 6;
            yp += 4;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        out_uplane += out_stride;
        out_vplane += out_stride;
    }
}

void blipvert::IMCx_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    for (int32_t y = 0; y < uv_height - 1; y++)
    {
        yp = in_buf;
        up = in_uplane;
        vp = in_vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            pdst[3] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            pdst[1] = yp[0];
            pdst[2] = yp[1];
            pdst[4] = yp[2];
            pdst[5] = yp[3];

            pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                static_cast<uint16_t>(up[1]) + \
                static_cast<uint16_t>(up[in_stride]) + \
                static_cast<uint16_t>(up[1 + in_stride])) >> 2);
            pdst[3 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                static_cast<uint16_t>(vp[1]) + \
                static_cast<uint16_t>(vp[in_stride]) + \
                static_cast<uint16_t>(vp[1 + in_stride])) >> 2);
            pdst[1 + out_stride] = yp[in_stride];
            pdst[2 + out_stride] = yp[1 + in_stride];
            pdst[4 + out_stride] = yp[2 + in_stride];
            pdst[5 + out_stride] = yp[3 + in_stride];

            pdst += 6;
            yp += 4;
            up += 2;
            vp += 2;
        }

        in_buf += in_stride_x_2;
        in_uplane += in_stride;
        in_vplane += in_stride;
        out_buf += out_stride_x_2;
    }

    // Get the last line without reading past the input u & v buffers
    yp = in_buf;
    up = in_uplane;
    vp = in_vplane;
    pdst = out_buf;

    for (int32_t x = 0; x < width; x += 4)
    {
        pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
        pdst[3] = pdst[3 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
        pdst[1] = yp[0];
        pdst[2] = yp[1];
        pdst[4] = yp[2];
        pdst[5] = yp[3];
        pdst[1 + out_stride] = yp[in_stride];
        pdst[2 + out_stride] = yp[1 + in_stride];
        pdst[4 + out_stride] = yp[2 + in_stride];
        pdst[5 + out_stride] = yp[3 + in_stride];

        pdst += 6;
        yp += 4;
        up += 2;
        vp += 2;
    }
}

void blipvert::PackedY422_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uvplane + out_u;
        uint8_t* vp = out_uvplane + out_v;

        for (int32_t x = 0; x < out_uv_width; x += 2)
        {
            yp[0] = psrc[in_y0];
            yp[out_stride] = psrc[in_y0 + in_stride];
            yp[1] = psrc[in_y1];
            yp[1 + out_stride] = psrc[in_y1 + in_stride];

            *up = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + \
                static_cast<uint16_t>(psrc[in_u + in_stride])) >> 1);

            *vp = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + \
                static_cast<uint16_t>(psrc[in_v + in_stride])) >> 1);
            psrc += 4;
            yp += 2;
            up += 2;
            vp += 2;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;
        out_uvplane += out_stride;
    }
}

void blipvert::NVx_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;


    for (int32_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uvplane + in_u;
        uint8_t* vp = in_uvplane + in_v;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[out_u] = *up;
            pdst[out_v] = *vp;
            pdst[out_y0] = yp[0];
            pdst[out_y1] = yp[1];

            pdst[out_u + out_stride] = *up;
            pdst[out_v + out_stride] = *vp;
            pdst[out_y0 + out_stride] = yp[in_stride];
            pdst[out_y1 + out_stride] = yp[1 + in_stride];

            pdst += 4;
            yp += 2;
            up += 2;
            vp += 2;
        }

        in_buf += in_stride_x_2;
        in_uvplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void blipvert::PlanarYUV_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t out_stride_x_2 = out_stride * 2;

    // Copy the y plane
    if (out_stride != in_y_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_y_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }

    if (in_decimation == 2)
    {
        // Copy the u & v planes without scaling
        for (uint16_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* in_up = in_uplane;
            uint8_t* in_vp = in_vplane;
            uint8_t* out_up = out_uvplane + out_u;
            uint8_t* out_vp = out_uvplane + out_v;

            for (uint16_t x = 0; x < in_uv_width; x++)
            {
                *out_up = *in_up++;
                *out_vp = *in_vp++;
                out_up += 2;
                out_vp += 2;
            }

            in_up += in_uv_stride;
            in_vp += in_uv_stride;
            out_uvplane += out_stride;
        }
    }
    else if (in_decimation == 4)
    {
        // Scaling from 4 to 2 decimation
        int32_t out_uv_stride_x_2 = out_stride * 2;
        uint8_t* in_up = in_uplane;
        uint8_t* in_vp = in_vplane;
        uint8_t* out_up = out_uvplane + out_u;
        uint8_t* out_vp = out_uvplane + out_v;
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            int32_t dst_index = 0;
            for (int32_t x = 0; x < in_uv_width; x++)
            {
                out_up[dst_index] = in_up[x];
                out_up[dst_index + out_stride] = in_up[x];
                out_vp[dst_index] = in_vp[x];
                out_vp[dst_index + out_stride] = in_vp[x];
                dst_index += 2;
                out_up[dst_index] = in_up[x];
                out_up[dst_index + out_stride] = in_up[x];
                out_vp[dst_index] = in_vp[x];
                out_vp[dst_index + out_stride] = in_vp[x];
                dst_index += 2;
            }

            out_up += out_uv_stride_x_2;
            out_vp += out_uv_stride_x_2;
            out_uvplane += out_stride;
        }
    }
}

void blipvert::NVx_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;

    // Copy the y plane
    if (out_y_stride != in_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_y_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_y_stride * height);
    }

    if (out_decimation == 2)
    {
        // Copy the u & v planes without scaling
        for (uint16_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* in_up = in_uvplane + in_u;
            uint8_t* in_vp = in_uvplane + in_v;
            uint8_t* out_up = out_uplane;
            uint8_t* out_vp = out_vplane;

            for (uint16_t x = 0; x < out_uv_width; x++)
            {
                *out_up++ = *in_up;
                *out_vp++ = *in_vp;
                in_up += 2;
                in_vp += 2;
            }

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
            in_uvplane += in_stride;
        }
    }
    else
    {
        // Scaling from 2 to 4 decimation
        int32_t in_uv_stride_x_2 = in_stride * 2;
        uint8_t* in_u_line = in_uvplane + in_u;
        uint8_t* in_v_line = in_uvplane + in_v;

        for (int32_t y = 0; y < in_uv_height; y += 2)
        {
            for (int32_t x = 0; x < in_uv_width; x += 4)
            {
                out_uplane[x >> 2] = static_cast<uint8_t>((static_cast<uint16_t>(in_u_line[x]) + \
                    static_cast<uint16_t>(in_u_line[x + 2]) + \
                    static_cast<uint16_t>(in_u_line[x + in_stride]) + \
                    static_cast<uint16_t>(in_u_line[x + in_stride + 2])) >> 2);
                out_vplane[x >> 2] = static_cast<uint8_t>((static_cast<uint16_t>(in_v_line[x]) + \
                    static_cast<uint16_t>(in_v_line[x + 2]) + \
                    static_cast<uint16_t>(in_v_line[x + in_stride]) + \
                    static_cast<uint16_t>(in_v_line[x + in_stride + 2])) >> 2);
            }

            in_u_line += in_stride;
            in_v_line += in_stride;
            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}

void blipvert::PackedY422_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_stride = in->stride;
    int16_t in_y0 = in->y0_index;
    int16_t in_y1 = in->y1_index;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uplane;
        uint8_t* vp = out_vplane;

        for (int32_t x = 0; x < out_uv_width; x++)
        {
            *yp++ = psrc[in_y0];
            *yp++ = psrc[in_y1];
            *up++ = psrc[in_u];
            *vp++ = psrc[in_v];
            psrc += 4;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;
        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;
    }
}

void blipvert::PlanarYUV_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_y_stride = in->y_stride;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;
    int32_t in_decimation = in->decimation;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    // Copy the y plane
    if (out_y_stride != in_y_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_y_stride;
            in_buf += in_y_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_y_stride * height);
    }

    int16_t out_uv_stride_x_2 = out_uv_stride * 2;
    int16_t out_uv_stride_x_3 = out_uv_stride * 3;
    int16_t out_uv_stride_x_4 = out_uv_stride * 4;


    if (in_decimation == 2)
    {
        uint8_t* in_up = in_uplane;
        uint8_t* in_vp = in_vplane;
        uint8_t* out_up = out_uplane;
        uint8_t* out_vp = out_vplane;

        // Scaling from 2 x 2 to 2 x 1 decimation
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            memcpy(out_up, in_up, in_uv_width);
            out_up += out_uv_stride;
            memcpy(out_up, in_up, in_uv_width);
            out_up += out_uv_stride;
            in_up += in_uv_stride;

            memcpy(out_vp, in_vp, in_uv_width);
            out_vp += out_uv_stride;
            memcpy(out_vp, in_vp, in_uv_width);
            out_vp += out_uv_stride;
            in_vp += in_uv_stride;
        }
    }
    else
    {
        // Scaling from 4 x 4 to 2 x 1 decimation
        for (int32_t y = 0; y < in_uv_height; y++)
        {
            uint8_t* in_up = in_uplane;
            uint8_t* in_vp = in_vplane;
            uint8_t* out_up = out_uplane;
            uint8_t* out_vp = out_vplane;

            int32_t dst_index = 0;
            for (int32_t x = 0; x < in_uv_width; x++)
            {
                uint8_t in_u = in_up[x];
                *out_up = in_u;
                *(out_up + out_uv_stride) = in_u;
                *(out_up + out_uv_stride_x_2) = in_u;
                *(out_up + out_uv_stride_x_3) = in_u;
                *(out_up + 1) = in_u;
                *(out_up + 1 + out_uv_stride) = in_u;
                *(out_up + 1 + out_uv_stride_x_2) = in_u;
                *(out_up + 1 + out_uv_stride_x_3) = in_u;

                uint8_t in_v = in_vp[x];
                *out_vp = in_v;
                *(out_vp + out_uv_stride) = in_v;
                *(out_vp + out_uv_stride_x_2) = in_v;
                *(out_vp + out_uv_stride_x_3) = in_v;
                *(out_vp + 1) = in_v;
                *(out_vp + 1 + out_uv_stride) = in_v;
                *(out_vp + 1 + out_uv_stride_x_2) = in_v;
                *(out_vp + 1 + out_uv_stride_x_3) = in_v;

                out_up += 2;
                out_vp += 2;
            }

            out_uplane += out_uv_stride_x_4;
            out_vplane += out_uv_stride_x_4;

            in_uplane += in_uv_stride;
            in_vplane += in_uv_stride;
        }
    }
}

void blipvert::IYU1_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uplane;
        uint8_t* vp = out_vplane;

        for (int32_t x = 0; x < out_uv_width; x += 2)
        {
            up[x] = up[x + 1] = psrc[0];
            yp[0] = psrc[1];
            yp[1] = psrc[2];
            vp[x] = vp[x + 1] = psrc[3];
            yp[2] = psrc[4];
            yp[3] = psrc[5];

            psrc += 6;
            yp += 4;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;

        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;
    }
}


void blipvert::IYU2_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uplane;
        uint8_t* vp = out_vplane;

        for (int32_t x = 0; x < out_uv_width; x++)
        {
            *yp++ = psrc[1];
            *yp++ = psrc[4];
            *up++ = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) +  static_cast<uint16_t>(psrc[3])) >> 1);
            *vp++= static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[5])) >> 1);
            psrc += 6;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;

        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;
    }
}


void blipvert::Y800_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    if (out_uv_stride == out_uv_width)
    {
        memset(out_uplane, 0, out_uv_width * height);
        memset(out_vplane, 0, out_uv_width * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memset(out_uplane, 0, out_uv_width);
            memset(out_vplane, 0, out_uv_width);
            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }

    if (out_y_stride == in_stride)
    {
        memcpy(out_buf, in_buf, out_y_stride * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_y_stride;
            in_buf += in_stride;
        }
    }
}

void blipvert::Y16_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    if (out_uv_stride == out_uv_width)
    {
        memset(out_uplane, 0, out_uv_width * height);
        memset(out_vplane, 0, out_uv_width * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memset(out_uplane, 0, out_uv_width);
            memset(out_vplane, 0, out_uv_width);
            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf + 1;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x++)
        {
            *pdst++ = *psrc;
            psrc += 2;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;
    }
}

void blipvert::CLJR_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uplane;
        uint8_t* vp = out_vplane;

        for (int32_t x = 0; x < out_uv_width; x += 2)
        {
            uint32_t mpixel = *(reinterpret_cast<uint32_t*>(psrc));

            *yp++ = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
            *yp++ = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
            *yp++ = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
            *yp++ = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));
            up[x] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            vp[x] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
            up[x + 1] = up[x];
            vp[x + 1] = vp[x];
            psrc += 4;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;

        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;
    }
}

void blipvert::Y41P_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uplane;
        uint8_t* vp = out_vplane;

        for (int32_t x = 0; x < out_uv_width; x += 4)
        {
            *yp++ = psrc[1];
            *yp++ = psrc[3];
            up[x] = psrc[0];
            vp[x] = psrc[2];
            *yp++ = psrc[5];
            *yp++ = psrc[7];

            up[x + 1] = psrc[0];
            vp[x + 1] = psrc[2];

            *yp++ = psrc[8];
            *yp++ = psrc[9];
            up[x + 2] = psrc[4];
            vp[x + 2] = psrc[6];

            *yp++ = psrc[10];
            *yp++ = psrc[11];
            up[x + 3] = psrc[4];
            vp[x + 3] = psrc[6];

            psrc += 12;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;

        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;
    }
}


void blipvert::IMCx_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    // Copy the y plane
    if (out_y_stride != in_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_y_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_y_stride * height);
    }

    for (int32_t line = 0; line < in_uv_height; line++)
    {
        memcpy(out_uplane, in_uplane, out_uv_width);
        memcpy(out_vplane, in_vplane, out_uv_width);
        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;

        memcpy(out_uplane, in_uplane, out_uv_width);
        memcpy(out_vplane, in_vplane, out_uv_width);
        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;

        in_uplane += in_stride;
        in_vplane += in_stride;
    }
}


void blipvert::NVx_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;


    // Copy the y plane
    if (out_y_stride != in_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_y_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_y_stride * height);
    }

    for (uint16_t y = 0; y < in_uv_height; y++)
    {
        uint8_t* in_up = in_uvplane + in_u;
        uint8_t* in_vp = in_uvplane + in_v;
        uint8_t* out_up = out_uplane;
        uint8_t* out_vp = out_vplane;

        for (uint16_t x = 0; x < out_uv_width; x++)
        {
            *out_up = *in_up;
            *(out_up + out_uv_stride) = *in_up;
            *out_vp = *in_vp;
            *(out_vp + out_uv_stride) = *in_vp;
            out_up++;
            out_vp++;
            in_up += 2;
            in_vp += 2;
        }

        out_uplane += out_uv_stride * 2;
        out_vplane += out_uv_stride * 2;
        in_uvplane += in_stride;
    }
}

//
// Public Yuv to Yuv transform functions
//


void blipvert::AYUV_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[1]) + static_cast<uint16_t>(psrc[5]) + \
                static_cast<uint16_t>(psrc[9]) + static_cast<uint16_t>(psrc[13])) >> 2); // U
            pdst[3] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4]) + \
                static_cast<uint16_t>(psrc[8]) + static_cast<uint16_t>(psrc[12])) >> 2); // V
            pdst[1] = psrc[2];  // Y0
            pdst[2] = psrc[6];  // Y1
            pdst[4] = psrc[10]; // Y2
            pdst[5] = psrc[14]; // Y3

            psrc += 16;
            pdst += 6;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::AYUV_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = psrc[1]; // U
            pdst[1] = psrc[2]; // Y
            pdst[2] = psrc[0]; // V

            psrc += 4;
            pdst += 3;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::AYUV_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t v_average = psrc[0];
            int16_t u_average = psrc[1];

            v_average += psrc[4];
            u_average += psrc[5];

            v_average += psrc[8];
            u_average += psrc[9];

            v_average += psrc[12];
            u_average += psrc[13];

            v_average >>= 2;
            u_average >>= 2;

            pdst[0] = static_cast<uint8_t>(u_average);
            pdst[2] = static_cast<uint8_t>(v_average);

            pdst[1] = psrc[2];
            pdst[3] = psrc[6];
            pdst[5] = psrc[10];
            pdst[7] = psrc[14];

            v_average = psrc[16];
            u_average = psrc[17];

            v_average += psrc[20];
            u_average += psrc[21];

            v_average += psrc[24];
            u_average += psrc[25];

            v_average += psrc[28];
            u_average += psrc[29];

            v_average >>= 2;
            u_average >>= 2;

            pdst[4] = static_cast<uint8_t>(u_average);
            pdst[6] = static_cast<uint8_t>(v_average);

            pdst[8] = psrc[18];
            pdst[9] = psrc[22];
            pdst[10] = psrc[26];
            pdst[11] = psrc[30];

            psrc += 32;
            pdst += 12;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::AYUV_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width;
        while (hcount)
        {
            uint32_t v_average = psrc[0];
            uint32_t u_average = psrc[1];

            v_average += psrc[4];
            u_average += psrc[5];

            v_average += psrc[8];
            u_average += psrc[9];

            v_average += psrc[12];
            u_average += psrc[13];

            v_average >>= 2;
            u_average >>= 2;

            PackCLJRDword(*pdst++, u_average, v_average, static_cast<uint32_t>(psrc[2]), static_cast<uint32_t>(psrc[6]), static_cast<uint32_t>(psrc[10]), static_cast<uint32_t>(psrc[14]));

            psrc += 16;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::AYUV_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf + 2;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = *psrc;
            psrc += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::AYUV_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf + 2;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = 0x00;
            *pdst++ = *psrc;
            psrc += 4;
            hcount--;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::AYUV_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    uint16_t uavg;
    uint16_t vavg;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < height; y += 2)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uvplane + out_u;
        uint8_t* vp = out_uvplane + out_v;
        for (int32_t x = 0; x < width; x += 2)
        {
            yp[0] = psrc[2];
            uavg = psrc[1];
            vavg = psrc[0];

            yp[1] = psrc[6];
            uavg += psrc[5];
            vavg += psrc[4];

            yp[out_stride] = psrc[2 + in_stride];
            uavg += psrc[1 + in_stride];
            vavg += psrc[0 + in_stride];

            yp[out_stride + 1] = psrc[6 + in_stride];
            uavg += psrc[5 + in_stride];
            vavg += psrc[4 + in_stride];

            *up = uavg >> 2;
            *vp = vavg >> 2;

            psrc += 8;
            yp += 2;
            up += 2;
            vp += 2;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;
        out_uvplane += out_stride;
    }
}

void blipvert::AYUV_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) / 2);
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[1]) + static_cast<uint16_t>(psrc[5])) / 2);
            pdst[1] = psrc[3] > 127 ? psrc[2] | 0x01 : psrc[2] & 0xFE;
            pdst[3] = psrc[7] > 127 ? psrc[6] | 0x01 : psrc[6] & 0xFE;
            psrc += 8;
            pdst += 4;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::AYUV_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            int16_t v_average = psrc[0];
            int16_t u_average = psrc[1];

            v_average += psrc[4];
            u_average += psrc[5];

            v_average += psrc[8];
            u_average += psrc[9];

            v_average += psrc[12];
            u_average += psrc[13];

            v_average >>= 2;
            u_average >>= 2;

            pdst[0] = static_cast<uint8_t>(u_average);
            pdst[2] = static_cast<uint8_t>(v_average);

            pdst[1] = psrc[3] > 127 ? psrc[2] | 0x01 : psrc[2] & 0xFE;
            pdst[3] = psrc[7] > 127 ? psrc[6] | 0x01 : psrc[6] & 0xFE;
            pdst[5] = psrc[11] > 127 ? psrc[10] | 0x01 : psrc[10] & 0xFE;
            pdst[7] = psrc[15] > 127 ? psrc[14] | 0x01 : psrc[14] & 0xFE;

            v_average = psrc[16];
            u_average = psrc[17];

            v_average += psrc[20];
            u_average += psrc[21];

            v_average += psrc[24];
            u_average += psrc[25];

            v_average += psrc[28];
            u_average += psrc[29];

            v_average >>= 2;
            u_average >>= 2;

            pdst[4] = static_cast<uint8_t>(u_average);
            pdst[6] = static_cast<uint8_t>(v_average);

            pdst[8] = psrc[19] > 127 ? psrc[18] | 0x01 : psrc[18] & 0xFE;
            pdst[9] = psrc[23] > 127 ? psrc[22] | 0x01 : psrc[22] & 0xFE;
            pdst[10] = psrc[27] > 127 ? psrc[26] | 0x01 : psrc[26] & 0xFE;
            pdst[11] = psrc[31] > 127 ? psrc[30] | 0x01 : psrc[30] & 0xFE;

            psrc += 32;
            pdst += 12;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::AYUV_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* vp = out_vplane;
        uint8_t* up = out_uplane;
        for (int32_t x = 0; x < out_uv_width; x++)
        {
            *yp++ = psrc[2];
            *yp++ = psrc[6];
            *up++ = (psrc[1] + psrc[5]) / 2;
            *vp++ = (psrc[0] + psrc[4]) / 2;

            psrc += 8;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;

        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;
    }
}


//
// IYU1 to YUV
//

void blipvert::IYU1_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t width_m_4 = width - 4;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width_m_4; x += 4)
        {
            pdst[0] = psrc[0];    // U
            pdst[1] = psrc[1];    // Y0
            pdst[2] = psrc[3];    // V

            pdst[3] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[0]) * 768) + (static_cast<uint32_t>(psrc[6]) * 256)) >> 10);
            pdst[4] = psrc[2];    // Y1
            pdst[5] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[3]) * 768) + (static_cast<uint32_t>(psrc[9]) * 256)) >> 10);

            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[6])) >> 1);
            pdst[7] = psrc[4];    // Y2
            pdst[8] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[3]) + static_cast<uint16_t>(psrc[9])) >> 1);

            pdst[9] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[0]) * 256) + (static_cast<uint32_t>(psrc[6]) * 768)) >> 10);
            pdst[10] = psrc[5];    // Y3
            pdst[11] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[3]) * 256) + (static_cast<uint32_t>(psrc[9]) * 768)) >> 10);

            psrc += 6;
            pdst += 12;
        }

        // Take care of the last block here to prevent
        // a read outside of the source line.
        pdst[0] = psrc[0];    // U
        pdst[1] = psrc[1];    // Y0
        pdst[2] = psrc[3];    // V

        pdst[3] = psrc[0];    // U
        pdst[4] = psrc[2];    // Y1
        pdst[5] = psrc[3];    // V

        pdst[6] = psrc[0];    // U
        pdst[7] = psrc[4];    // Y2
        pdst[8] = psrc[3];    // V

        pdst[9] = psrc[0];    // U
        pdst[10] = psrc[5];    // Y3
        pdst[11] = psrc[3];    // V

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU1_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = psrc[1];
            pdst[1] = psrc[2];
            pdst[2] = psrc[4];
            pdst[3] = psrc[5];

            psrc += 6;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU1_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = 0;
            pdst[1] = psrc[1];
            pdst[2] = 0;
            pdst[3] = psrc[2];
            pdst[4] = 0;
            pdst[5] = psrc[4];
            pdst[6] = 0;
            pdst[7] = psrc[5];

            pdst += 8;
            psrc += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU1_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t x = 0; x < width; x += 4)
        {
            PackCLJRDword(*pdst++, psrc[0], psrc[3], psrc[1], psrc[2], psrc[4], psrc[5])
                psrc += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU1_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[0];          // U0
            pdst[2] = psrc[3];          // V0
            pdst[1] = psrc[1];          // Y0
            pdst[3] = psrc[2];          // Y1
            pdst[5] = psrc[4];          // Y2
            pdst[7] = psrc[5];          // Y3

            pdst[4] = psrc[6];          // U4
            pdst[6] = psrc[9];          // V4
            pdst[8] = psrc[7];          // Y4
            pdst[9] = psrc[8];          // Y5
            pdst[10] = psrc[10];        // Y6
            pdst[11] = psrc[11];        // Y7

            psrc += 12;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU1_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = psrc[3];  // V
            pdst[1] = psrc[0];  // U
            pdst[2] = psrc[1];  // Y
            pdst[3] = 0xFF;

            pdst[4] = psrc[3];  // V
            pdst[5] = psrc[0];  // U
            pdst[6] = psrc[2];  // Y
            pdst[7] = 0xFF;

            pdst[8] = psrc[3];  // V
            pdst[9] = psrc[0];  // U
            pdst[10] = psrc[4]; // Y
            pdst[11] = 0xFF;

            pdst[12] = psrc[3];  // V
            pdst[13] = psrc[0];  // U
            pdst[14] = psrc[5];  // Y
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

void blipvert::IYU1_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uvplane + out_u;
        uint8_t* vp = out_uvplane + out_v;

        for (int32_t x = 0; x < out_uv_width; x += 4)
        {
            yp[0] = psrc[1];
            yp[out_stride] = psrc[1 + in_stride];
            yp[1] = psrc[2];
            yp[1 + out_stride] = psrc[2 + in_stride];

            up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[in_stride])) >> 1);

            vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[3]) + \
                static_cast<uint16_t>(psrc[3 + in_stride])) >> 1);

            yp[2] = psrc[4];
            yp[2 + out_stride] = psrc[4 + in_stride];
            yp[3] = psrc[5];
            yp[3 + out_stride] = psrc[5 + in_stride];

            up[x + 2] = up[x];
            vp[x + 2] = vp[x];

            psrc += 6;
            yp += 4;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;
        out_uvplane += out_stride;
    }
}

void blipvert::IYU1_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[1] = psrc[1] | 0x01;
            pdst[3] = psrc[2] | 0x01;
            pdst[0] = psrc[0];
            pdst[2] = psrc[3];

            pdst[5] = psrc[4] | 0x01;
            pdst[7] = psrc[5] | 0x01;
            pdst[4] = psrc[0];
            pdst[6] = psrc[3];

            psrc += 6;
            pdst += 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU1_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[0];          // U0
            pdst[2] = psrc[3];          // V0
            pdst[1] = psrc[1] | 0x01;   // Y0
            pdst[3] = psrc[2] | 0x01;   // Y1
            pdst[5] = psrc[4] | 0x01;   // Y2
            pdst[7] = psrc[5] | 0x01;   // Y3

            pdst[4] = psrc[6];          // U4
            pdst[6] = psrc[9];          // V4
            pdst[8] = psrc[7] | 0x01;   // Y4
            pdst[9] = psrc[8] | 0x01;   // Y5
            pdst[10] = psrc[10] | 0x01; // Y6
            pdst[11] = psrc[11] | 0x01; // Y7

            psrc += 12;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

//
// IYU2 to YUV
//

void blipvert::IYU2_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[3]) + \
                static_cast<uint16_t>(psrc[6]) + static_cast<uint16_t>(psrc[9])) >> 2);
            pdst[3] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[5]) + \
                static_cast<uint16_t>(psrc[8]) + static_cast<uint16_t>(psrc[11])) >> 2);
            pdst[1] = psrc[1];
            pdst[2] = psrc[4];
            pdst[4] = psrc[7];
            pdst[5] = psrc[10];

            psrc += 12;
            pdst += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU2_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf + 1;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x++)
        {
            *pdst++ = *psrc;
            psrc += 3;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU2_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf + 1;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x++)
        {
            *pdst++ = 0x00;
            *pdst++ = *psrc;
            psrc += 3;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU2_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t x = 0; x < width; x += 4)
        {
            uint8_t u_val = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[3]) + \
                static_cast<uint16_t>(psrc[6]) + static_cast<uint16_t>(psrc[9])) >> 2);
            uint8_t v_val = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[5]) + \
                static_cast<uint16_t>(psrc[8]) + static_cast<uint16_t>(psrc[11])) >> 2);
            PackCLJRDword(*pdst++, u_val, v_val, psrc[1], psrc[4], psrc[7], psrc[10])
                psrc += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU2_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[3]) + \
                static_cast<uint16_t>(psrc[6]) + static_cast<uint16_t>(psrc[9])) >> 2);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[5]) + \
                static_cast<uint16_t>(psrc[8]) + static_cast<uint16_t>(psrc[11])) >> 2);
            pdst[1] = psrc[1];
            pdst[3] = psrc[4];
            pdst[5] = psrc[7];
            pdst[7] = psrc[10];

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[12]) + static_cast<uint16_t>(psrc[15]) + \
                static_cast<uint16_t>(psrc[18]) + static_cast<uint16_t>(psrc[21])) >> 2);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[14]) + static_cast<uint16_t>(psrc[17]) + \
                static_cast<uint16_t>(psrc[20]) + static_cast<uint16_t>(psrc[23])) >> 2);
            pdst[8] = psrc[13];
            pdst[9] = psrc[16];
            pdst[10] = psrc[19];
            pdst[11] = psrc[22];

            psrc += 24;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU2_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = psrc[2];  // V
            pdst[1] = psrc[0];  // U
            pdst[2] = psrc[1];  // Y
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

void blipvert::IYU2_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uvplane + out_u;
        uint8_t* vp = out_uvplane + out_v;

        for (int32_t x = 0; x < out_uv_width; x += 2)
        {
            yp[0] = psrc[1];
            yp[out_stride] = psrc[1 + in_stride];
            yp[1] = psrc[4];
            yp[1 + out_stride] = psrc[4 + in_stride];

            up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[in_stride]) + \
                static_cast<uint16_t>(psrc[3]) + \
                static_cast<uint16_t>(psrc[3 + in_stride])) >> 2);

            vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                static_cast<uint16_t>(psrc[2 + in_stride]) + \
                static_cast<uint16_t>(psrc[5]) + \
                static_cast<uint16_t>(psrc[5 + in_stride])) >> 2);

            psrc += 6;
            yp += 2;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;
        out_uvplane += out_stride;
    }
}

void blipvert::IYU2_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[3])) >> 1);
            pdst[1] = psrc[1] | 0x01;
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[5])) >> 1);
            pdst[3] = psrc[4] | 0x01;

            psrc += 6;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::IYU2_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[3]) + \
                static_cast<uint16_t>(psrc[6]) + static_cast<uint16_t>(psrc[9])) >> 2);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[5]) + \
                static_cast<uint16_t>(psrc[8]) + static_cast<uint16_t>(psrc[11])) >> 2);
            pdst[1] = psrc[1] | 0x01;
            pdst[3] = psrc[4] | 0x01;
            pdst[5] = psrc[7] | 0x01;
            pdst[7] = psrc[10] | 0x01;

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[12]) + static_cast<uint16_t>(psrc[15]) + \
                static_cast<uint16_t>(psrc[18]) + static_cast<uint16_t>(psrc[21])) >> 2);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[14]) + static_cast<uint16_t>(psrc[17]) + \
                static_cast<uint16_t>(psrc[20]) + static_cast<uint16_t>(psrc[23])) >> 2);
            pdst[8] = psrc[13] | 0x01;
            pdst[9] = psrc[16] | 0x01;
            pdst[10] = psrc[19] | 0x01;
            pdst[11] = psrc[22] | 0x01;

            psrc += 24;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y800_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = 0;
            pdst[1] = psrc[0];
            pdst[2] = psrc[1];
            pdst[3] = 0;
            pdst[4] = psrc[2];
            pdst[5] = psrc[3];

            psrc += 4;
            pdst += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y800_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x++)
        {
            pdst[0] = 0;
            pdst[1] = *psrc++;
            pdst[2] = 0;
            pdst += 3;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y800_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t x = 0; x < width; x += 4)
        {
            PackCLJRDword(*pdst++, 0, 0, psrc[0], psrc[1], psrc[2], psrc[3])
                psrc += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y800_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = 0;
            pdst[2] = 0;
            pdst[1] = psrc[0];
            pdst[3] = psrc[1];
            pdst[5] = psrc[2];
            pdst[7] = psrc[3];

            pdst[4] = 0;
            pdst[6] = 0;
            pdst[8] = psrc[4];
            pdst[9] = psrc[5];
            pdst[10] = psrc[6];
            pdst[11] = psrc[7];

            psrc += 8;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y800_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (uint16_t index = 0; index < width; index++)
        {
            *pdst++ = 0x00;
            *pdst++ = 0x00;
            *pdst++ = *psrc++;
            *pdst++ = 0xFF;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y800_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (uint16_t index = 0; index < width; index++)
        {
            *pdst++ = 0x00;
            *pdst++ = *psrc++;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y800_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        memset(out_uvplane, 0, out_uv_width);
        out_uvplane += out_stride;
    }

    if (out_stride == in_stride)
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_stride;
        }
    }
}

void blipvert::Y800_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[1] = psrc[0] | 0x01;
            pdst[3] = psrc[1] | 0x01;
            pdst[0] = 0;
            pdst[2] = 0;
            psrc += 2;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y800_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = 0;
            pdst[2] = 0;
            pdst[1] = psrc[0] | 0x01;
            pdst[3] = psrc[1] | 0x01;
            pdst[5] = psrc[2] | 0x01;
            pdst[7] = psrc[3] | 0x01;

            pdst[4] = 0;
            pdst[6] = 0;
            pdst[8] = psrc[4] | 0x01;
            pdst[9] = psrc[5] | 0x01;
            pdst[10] = psrc[6] | 0x01;
            pdst[11] = psrc[7] | 0x01;

            psrc += 8;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}


//
// Y16 to YUV
//

void blipvert::Y16_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = 0;
            pdst[1] = psrc[1];
            pdst[2] = psrc[3];
            pdst[3] = 0;
            pdst[4] = psrc[5];
            pdst[5] = psrc[7];

            psrc += 8;
            pdst += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y16_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf + 1; // Upper byte of 16-bit little-endian
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x++)
        {
            pdst[0] = 0;
            pdst[1] = *psrc;
            pdst[2] = 0;
            psrc += 2;
            pdst += 3;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y16_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t x = 0; x < width; x += 4)
        {
            PackCLJRDword(*pdst++, 0, 0, psrc[1], psrc[3], psrc[5], psrc[7])
                psrc += 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y16_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = 0;
            pdst[2] = 0;
            pdst[1] = psrc[1];
            pdst[3] = psrc[3];
            pdst[5] = psrc[5];
            pdst[7] = psrc[7];

            pdst[4] = 0;
            pdst[6] = 0;
            pdst[8] = psrc[9];
            pdst[9] = psrc[11];
            pdst[10] = psrc[13];
            pdst[11] = psrc[15];

            psrc += 16;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y16_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf + 1;
        uint8_t* pdst = out_buf;
        for (uint16_t index = 0; index < width; index++)
        {
            *pdst++ = 0x00;
            *pdst++ = 0x00;
            *pdst++ = *psrc;
            *pdst++ = 0xFF;
            psrc += 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y16_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf + 1;
        uint8_t* pdst = out_buf;
        for (uint16_t index = 0; index < width; index++)
        {
            *pdst++ = *psrc;
            psrc += 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y16_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        memset(out_uvplane, 0, out_uv_width);
        out_uvplane += out_stride;
    }

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf + 1;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x++)
        {
            *pdst++ = *psrc;
            psrc += 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y16_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[1] = psrc[1] | 0x01;
            pdst[3] = psrc[3] | 0x01;
            pdst[0] = 0;
            pdst[2] = 0;
            psrc += 4;
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y16_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = 0;
            pdst[2] = 0;
            pdst[1] = psrc[1] | 0x01;
            pdst[3] = psrc[3] | 0x01;
            pdst[5] = psrc[5] | 0x01;
            pdst[7] = psrc[7] | 0x01;

            pdst[4] = 0;
            pdst[6] = 0;
            pdst[8] = psrc[9] | 0x01;
            pdst[9] = psrc[11] | 0x01;
            pdst[10] = psrc[13] | 0x01;
            pdst[11] = psrc[15] | 0x01;

            psrc += 16;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

//
// CLJR to YUV
//

void blipvert::CLJR_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            uint32_t mpixel = *psrc++;
            pdst[0] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            pdst[1] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
            pdst[2] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
            pdst[3] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
            pdst[4] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
            pdst[5] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));
            pdst += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::CLJR_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t width_m_4 = width - 4;
    uint32_t mpixel;

    for (int32_t y = 0; y < height; y++)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width_m_4; x += 4)
        {
            mpixel = *psrc++;
            uint8_t next_u_val = static_cast<uint8_t>(UnpackCLJR_U(*psrc));
            uint8_t next_v_val = static_cast<uint8_t>(UnpackCLJR_V(*psrc));

            pdst[0] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            pdst[1] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
            pdst[2] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));

            pdst[3] = static_cast<uint8_t>(((static_cast<uint32_t>(pdst[0]) * 768) + (static_cast<uint32_t>(next_u_val) * 256)) >> 10);
            pdst[4] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
            pdst[5] = static_cast<uint8_t>(((static_cast<uint32_t>(pdst[2]) * 768) + (static_cast<uint32_t>(next_v_val) * 256)) >> 10);

            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(pdst[0]) + static_cast<uint16_t>(next_u_val)) >> 1);
            pdst[7] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
            pdst[8] = static_cast<uint8_t>((static_cast<uint16_t>(pdst[2]) + static_cast<uint16_t>(next_v_val)) >> 1);

            pdst[9] = static_cast<uint8_t>(((static_cast<uint32_t>(pdst[0]) * 256) + (static_cast<uint32_t>(next_u_val) * 768)) >> 10);
            pdst[10] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));
            pdst[11] = static_cast<uint8_t>(((static_cast<uint32_t>(pdst[2]) * 256) + (static_cast<uint32_t>(next_v_val) * 768)) >> 10);

            pdst += 12;
        }

        // Take care of the last block here to prevent
        // a read outside of the source line.
        mpixel = *psrc;
        uint8_t u_val = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
        uint8_t v_val = static_cast<uint8_t>(UnpackCLJR_V(mpixel));

        pdst[0] = u_val;
        pdst[1] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
        pdst[2] = v_val;

        pdst[3] = u_val;
        pdst[4] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
        pdst[5] = v_val;

        pdst[6] = u_val;
        pdst[7] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
        pdst[8] = v_val;

        pdst[9] = u_val;
        pdst[10] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));
        pdst[11] = v_val;

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::CLJR_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            uint32_t mpixel = *psrc++;
            pdst[0] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
            pdst[1] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
            pdst[2] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
            pdst[3] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));
            pdst += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::CLJR_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            uint32_t mpixel = *psrc++;
            pdst[0] = 0;
            pdst[1] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
            pdst[2] = 0;
            pdst[3] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
            pdst[4] = 0;
            pdst[5] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
            pdst[6] = 0;
            pdst[7] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));
            pdst += 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::CLJR_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            uint32_t mpixel = *psrc++;
            pdst[0] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            pdst[2] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
            pdst[1] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
            pdst[3] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
            pdst[5] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
            pdst[7] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));

            mpixel = *psrc++;
            pdst[4] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            pdst[6] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
            pdst[8] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
            pdst[9] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
            pdst[10] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
            pdst[11] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));

            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::CLJR_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint32_t mpixel = *psrc++;
            uint8_t U = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            uint8_t V = static_cast<uint8_t>(UnpackCLJR_V(mpixel));

            pdst[0] = V;
            pdst[1] = U;
            pdst[2] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel));
            pdst[3] = 0xFF;

            pdst[4] = V;
            pdst[5] = U;
            pdst[6] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel));
            pdst[7] = 0xFF;

            pdst[8] = V;
            pdst[9] = U;
            pdst[10] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel));
            pdst[11] = 0xFF;

            pdst[12] = V;
            pdst[13] = U;
            pdst[14] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel));
            pdst[15] = 0xFF;

            pdst += 16;
            hcount -= 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::CLJR_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t y_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uvplane + out_u;
        uint8_t* vp = out_uvplane + out_v;

        for (int32_t x = 0; x < out_uv_width; x += 4)
        {
            uint32_t mpixel_top = *(reinterpret_cast<uint32_t*>(psrc));
            uint32_t mpixel_bot = *(reinterpret_cast<uint32_t*>(psrc + in_stride));

            yp[0] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel_top));
            yp[out_stride] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel_bot));
            yp[1] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel_top));
            yp[1 + out_stride] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel_bot));

            up[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_U(mpixel_top)) + \
                static_cast<uint16_t>(UnpackCLJR_U(mpixel_bot))) >> 1);

            vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_V(mpixel_top)) + \
                static_cast<uint16_t>(UnpackCLJR_V(mpixel_bot))) >> 1);

            yp[2] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel_top));
            yp[2 + out_stride] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel_bot));
            yp[3] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel_top));
            yp[3 + out_stride] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel_bot));

            up[x + 2] = up[x];
            vp[x + 2] = vp[x];

            psrc += 4;
            yp += 4;
        }

        in_buf += in_stride_x_2;
        out_buf += y_stride_x_2;
        out_uvplane += out_stride;
    }
}

void blipvert::CLJR_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t width_m_4 = width - 4;
    uint32_t mpixel;

    for (int32_t y = 0; y < height; y++)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width_m_4; x += 4)
        {
            mpixel = *psrc++;
            pdst[0] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            pdst[2] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
            pdst[1] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel)) | 0x01;
            pdst[3] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel)) | 0x01;

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(pdst[0]) + static_cast<uint16_t>(UnpackCLJR_U(*psrc))) >> 1);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(pdst[2]) + static_cast<uint16_t>(UnpackCLJR_V(*psrc))) >> 1);
            pdst[5] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel)) | 0x01;
            pdst[7] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel)) | 0x01;

            pdst += 8;
        }

        // Get last macropixel to prevent reading beyond line
        mpixel = *psrc;
        pdst[0] = pdst[4] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
        pdst[2] = pdst[6] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
        pdst[1] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel)) | 0x01;
        pdst[3] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel)) | 0x01;
        pdst[5] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel)) | 0x01;
        pdst[7] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel)) | 0x01;

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::CLJR_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            uint32_t mpixel = *psrc++;
            pdst[0] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            pdst[2] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
            pdst[1] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel)) | 0x01;
            pdst[3] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel)) | 0x01;
            pdst[5] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel)) | 0x01;
            pdst[7] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel)) | 0x01;

            mpixel = *psrc++;
            pdst[4] = static_cast<uint8_t>(UnpackCLJR_U(mpixel));
            pdst[6] = static_cast<uint8_t>(UnpackCLJR_V(mpixel));
            pdst[8] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel)) | 0x01;
            pdst[9] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel)) | 0x01;
            pdst[10] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel)) | 0x01;
            pdst[11] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel)) | 0x01;

            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

//
// Y41P to YUV
//

void blipvert::Y41P_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[0];      // U0
            pdst[3] = psrc[2];      // V0
            pdst[1] = psrc[1];      // Y0
            pdst[2] = psrc[3];      // Y1
            pdst[4] = psrc[5];      // Y2
            pdst[5] = psrc[7];      // Y3

            pdst[6] = psrc[4];      // U4
            pdst[9] = psrc[6];      // V4
            pdst[7] = psrc[8];      // Y4
            pdst[8] = psrc[9];      // Y5
            pdst[10] = psrc[10];    // Y6
            pdst[11] = psrc[11];    // Y7

            psrc += 12;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[0];      // U0
            pdst[1] = psrc[1];      // Y0
            pdst[2] = psrc[2];      // V0

            pdst[3] = psrc[0];      // U0      
            pdst[4] = psrc[3];      // Y1
            pdst[5] = psrc[2];      // V0

            pdst[6] = psrc[0];      // U0
            pdst[7] = psrc[5];      // Y2
            pdst[8] = psrc[2];      // V0

            pdst[9] = psrc[0];      // U0
            pdst[10] = psrc[7];     // Y3
            pdst[11] = psrc[2];     // V0

            pdst[12] = psrc[4];     // U4
            pdst[13] = psrc[8];     // Y4
            pdst[14] = psrc[6];     // V4

            pdst[15] = psrc[4];     // U4
            pdst[16] = psrc[9];     // Y5
            pdst[17] = psrc[6];     // V4

            pdst[18] = psrc[4];     // U4
            pdst[19] = psrc[10];    // Y6
            pdst[20] = psrc[6];     // V4

            pdst[21] = psrc[4];     // U4
            pdst[22] = psrc[11];    // Y7
            pdst[23] = psrc[6];     // V4

            psrc += 12;
            pdst += 24;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[1];      // Y0
            pdst[1] = psrc[3];      // Y1
            pdst[2] = psrc[5];      // Y2
            pdst[3] = psrc[7];      // Y3
            pdst[4] = psrc[8];      // Y4
            pdst[5] = psrc[9];      // Y5
            pdst[6] = psrc[10];     // Y6
            pdst[7] = psrc[11];     // Y7

            psrc += 12;
            pdst += 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = 0;
            pdst[1] = psrc[1];      // Y0
            pdst[2] = 0;
            pdst[3] = psrc[3];      // Y1
            pdst[4] = 0;
            pdst[5] = psrc[5];      // Y2
            pdst[6] = 0;
            pdst[7] = psrc[7];      // Y3
            pdst[8] = 0;
            pdst[9] = psrc[8];      // Y4
            pdst[10] = 0;
            pdst[11] = psrc[9];      // Y5
            pdst[12] = 0;
            pdst[13] = psrc[10];     // Y6
            pdst[14] = 0;
            pdst[15] = psrc[11];     // Y7

            psrc += 12;
            pdst += 16;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}


void blipvert::Y41P_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t x = 0; x < width; x += 8)
        {
            PackCLJRDword(*pdst++, psrc[0], psrc[2], psrc[1], psrc[3], psrc[5], psrc[7])
            PackCLJRDword(*pdst++, psrc[4], psrc[6], psrc[8], psrc[9], psrc[10], psrc[11])
            psrc += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_IY41(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    Progressive_to_Interlaced(height, width * 2, out->flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}

void blipvert::Y41P_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t U = psrc[0];
            int32_t V = psrc[2];

            int32_t Y = luminance_table[psrc[1]];
            pdst[0] = V;
            pdst[1] = U;
            pdst[2] = psrc[1];
            pdst[3] = 0xFF;

            Y = luminance_table[psrc[3]];
            pdst[4] = V;
            pdst[5] = U;
            pdst[6] = psrc[3];
            pdst[7] = 0xFF;

            Y = luminance_table[psrc[5]];
            pdst[8] = V;
            pdst[9] = U;
            pdst[10] = psrc[5];
            pdst[11] = 0xFF;

            Y = luminance_table[psrc[7]];
            pdst[12] = V;
            pdst[13] = U;
            pdst[14] = psrc[7];
            pdst[15] = 0xFF;

            U = psrc[4];
            V = psrc[6];

            Y = luminance_table[psrc[8]];
            pdst[16] = V;
            pdst[17] = U;
            pdst[18] = psrc[8];
            pdst[19] = 0xFF;

            Y = luminance_table[psrc[9]];
            pdst[20] = V;
            pdst[21] = U;
            pdst[22] = psrc[9];
            pdst[23] = 0xFF;

            Y = luminance_table[psrc[10]];
            pdst[24] = V;
            pdst[25] = U;
            pdst[26] = psrc[10];
            pdst[27] = 0xFF;

            Y = luminance_table[psrc[11]];
            pdst[28] = V;
            pdst[29] = U;
            pdst[30] = psrc[11];
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

void blipvert::Y41P_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uvplane + out_u;
        uint8_t* vp = out_uvplane + out_v;

        for (int32_t x = 0; x < out_uv_width; x += 8)
        {
            yp[0] = psrc[1];
            yp[out_stride] = psrc[1 + in_stride];
            yp[1] = psrc[3];
            yp[1 + out_stride] = psrc[3 + in_stride];

            up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[in_stride])) >> 1);

            vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);

            yp[2] = psrc[5];
            yp[2 + out_stride] = psrc[5 + in_stride];
            yp[3] = psrc[7];
            yp[3 + out_stride] = psrc[7 + in_stride];

            up[x + 2] = up[x];
            vp[x + 2] = vp[x];

            yp[4] = psrc[8];
            yp[4 + out_stride] = psrc[8 + in_stride];
            yp[5] = psrc[9];
            yp[5 + out_stride] = psrc[9 + in_stride];

            up[x + 4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                static_cast<uint16_t>(psrc[4 + in_stride])) >> 1);

            vp[x + 4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                static_cast<uint16_t>(psrc[6 + in_stride])) >> 1);

            yp[6] = psrc[10];
            yp[6 + out_stride] = psrc[10 + in_stride];
            yp[7] = psrc[11];
            yp[7 + out_stride] = psrc[11 + in_stride];

            up[x + 6] = up[x + 2];

            vp[x + 6] = vp[x + 2];

            psrc += 12;
            yp += 8;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;
        out_uvplane += out_stride;
    }
}

void blipvert::Y41P_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[0];              // U0
            pdst[2] = psrc[2];              // V0
            pdst[1] = psrc[1] | 0x1;        // Y0
            pdst[3] = psrc[3] | 0x1;        // Y1

            pdst[0 + 4] = psrc[0];          // U0
            pdst[2 + 4] = psrc[2];          // V0
            pdst[1 + 4] = psrc[5] | 0x1;    // Y2
            pdst[3 + 4] = psrc[7] | 0x1;    // Y3

            pdst[0 + 8] = psrc[4];          // U4
            pdst[2 + 8] = psrc[6];          // V4
            pdst[1 + 8] = psrc[8] | 0x1;    // Y4
            pdst[3 + 8] = psrc[9] | 0x1;    // Y5

            pdst[0 + 12] = psrc[4];         // U4
            pdst[2 + 12] = psrc[6];         // V4
            pdst[1 + 12] = psrc[10] | 0x1;  // Y6
            pdst[3 + 12] = psrc[11] | 0x1;  // Y7

            psrc += 12;
            pdst += 16;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = *psrc++;          // U0
            *pdst++ = *psrc++ | 0x01;   // Y0
            *pdst++ = *psrc++;          // V0
            *pdst++ = *psrc++ | 0x01;   // Y1
            *pdst++ = *psrc++;          // U4
            *pdst++ = *psrc++ | 0x01;   // Y2
            *pdst++ = *psrc++;          // V4
            *pdst++ = *psrc++ | 0x01;   // Y3
            *pdst++ = *psrc++ | 0x01;   // Y4
            *pdst++ = *psrc++ | 0x01;   // Y5
            *pdst++ = *psrc++ | 0x01;   // Y6
            *pdst++ = *psrc++ | 0x01;   // Y7

            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::IMCx_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    // Copy the y plane
    if (out_stride != in_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }

    // Copy the u & v planes without scaling
    if (out_stride != in_stride || out->flipped || in->interlaced || out->interlaced)
    {
        for (int32_t line = 0; line < in_uv_height; line++)
        {
            memcpy(out_uplane, in_uplane, in_uv_width);
            memcpy(out_vplane, in_vplane, in_uv_width);
            out_uplane += out_stride;
            out_vplane += out_stride;
            in_uplane += in_stride;
            in_vplane += in_stride;
        }
    }
    else
    {
        memcpy(out_uplane, in_uplane, out_stride * in_uv_height);
        memcpy(out_vplane, in_vplane, out_stride * in_uv_height);
    }
}

void blipvert::IMCx_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    // Copy the y plane
    if (out_stride != in_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }

    // Copy the u & v planes without scaling
    for (int32_t y = 0; y < in_uv_height; y++)
    {
        uint8_t* in_up = in_uplane;
        uint8_t* in_vp = in_vplane;
        uint8_t* out_up = out_uvplane + out_u;
        uint8_t* out_vp = out_uvplane + out_v;

        for (int32_t x = 0; x < in_uv_width; x++)
        {
            *out_up = *in_up++;
            *out_vp = *in_vp++;
            out_up += 2;
            out_vp += 2;
        }

        out_uvplane += out_stride;
        in_uplane += in_stride;
        in_vplane += in_stride;
    }
}

void blipvert::IMCx_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int32_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[0] = *up;
            pdst[2] = *vp;
            pdst[1] = yp[0] | 0x01;
            pdst[3] = yp[1] | 0x01;

            pdst[0 + out_stride] = *up;
            pdst[2 + out_stride] = *vp;
            pdst[1 + out_stride] = yp[in_stride] | 0x01;
            pdst[3 + out_stride] = yp[1 + in_stride] | 0x01;

            pdst += 4;
            yp += 2;
            up += 1;
            vp += 1;
        }

        in_buf += in_stride_x_2;
        in_uplane += in_stride;
        in_vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

//
// NVx to YUV
//

void blipvert::NVx_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int16_t y = 0; y < in_uv_height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uvplane + in_u;
        uint8_t* vp = in_uvplane + in_v;
        uint8_t* pdst = out_buf;

        for (int16_t x = 0; x < width; x += 2)
        {
            // column 1 row 1
            pdst[0] = *vp;
            pdst[1] = *up;
            pdst[2] = yp[0];
            pdst[3] = 0xFF;

            // column 1 row 2
            pdst[out_stride] = *vp;
            pdst[1 + out_stride] = *up;
            pdst[2 + out_stride] = yp[in_stride];
            pdst[3 + out_stride] = 0xFF;

            // column 2 row 1
            pdst[4] = *vp;
            pdst[5] = *up;
            pdst[6] = yp[1];
            pdst[7] = 0xFF;

            // column 2 row 2
            pdst[4 + out_stride] = *vp;
            pdst[5 + out_stride] = *up;
            pdst[6 + out_stride] = yp[1 + in_stride];
            pdst[7 + out_stride] = 0xFF;

            pdst += 8;
            yp += 2;
            up += 2;
            vp += 2;
        }

        in_buf += in_stride_x_2;
        in_uvplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

//
// NV12 to YUV
//

void blipvert::NVx_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    for (int32_t y = 0; y < height - 2; y += 2)
    {
        yp = in_buf;
        up = in_uvplane + in_u;
        vp = in_uvplane + in_v;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[2])) >> 1);
            pdst[3] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[2])) >> 1);
            pdst[1] = yp[0];
            pdst[2] = yp[1];
            pdst[4] = yp[2];
            pdst[5] = yp[3];

            pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                static_cast<uint16_t>(up[2]) + \
                static_cast<uint16_t>(up[in_stride]) + \
                static_cast<uint16_t>(up[2 + in_stride])) >> 2);
            pdst[3 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                static_cast<uint16_t>(vp[2]) + \
                static_cast<uint16_t>(vp[in_stride]) + \
                static_cast<uint16_t>(vp[2 + in_stride])) >> 2);
            pdst[1 + out_stride] = yp[in_stride];
            pdst[2 + out_stride] = yp[1 + in_stride];
            pdst[4 + out_stride] = yp[2 + in_stride];
            pdst[5 + out_stride] = yp[3 + in_stride];

            pdst += 6;
            yp += 4;
            up += 4;
            vp += 4;
        }

        in_buf += in_stride_x_2;
        in_uvplane += in_stride;
        out_buf += out_stride_x_2;
    }

    // Get the last line without reading past the input u & v buffers
    yp = in_buf;
    up = in_uvplane + in_u;
    vp = in_uvplane + in_v;
    pdst = out_buf;

    for (int32_t x = 0; x < width; x += 4)
    {
        pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[2])) >> 1);
        pdst[3] = pdst[3 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[2])) >> 1);
        pdst[1] = yp[0];
        pdst[2] = yp[1];
        pdst[4] = yp[2];
        pdst[5] = yp[3];
        pdst[1 + out_stride] = yp[in_stride];
        pdst[2 + out_stride] = yp[1 + in_stride];
        pdst[4 + out_stride] = yp[2 + in_stride];
        pdst[5 + out_stride] = yp[3 + in_stride];

        pdst += 6;
        yp += 4;
        up += 4;
        vp += 4;
    }
}

void blipvert::NVx_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int16_t y = 0; y < in_uv_height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uvplane + in_u;
        uint8_t* vp = in_uvplane + in_v;
        uint8_t* pdst = out_buf;

        for (int16_t x = 0; x < width; x += 2)
        {
            // column 1 row 1
            pdst[0] = *up;
            pdst[1] = yp[0];
            pdst[2] = *vp;

            // column 1 row 2
            pdst[out_stride] = *up;
            pdst[1 + out_stride] = yp[in_stride];
            pdst[2 + out_stride] = *vp;

            // column 2 row 1
            pdst[3] = *up;
            pdst[4] = yp[1];
            pdst[5] = *vp;

            // column 2 row 2
            pdst[3 + out_stride] = *up;
            pdst[4 + out_stride] = yp[1 + in_stride];
            pdst[5 + out_stride] = *vp;

            pdst += 6;
            yp += 2;
            up += 2;
            vp += 2;
        }

        in_buf += in_stride_x_2;
        in_uvplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void blipvert::NVx_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    // Copy the y plane

    if (out_stride != in_stride || out->flipped)
    {
        for (int32_t y = 0; y < height; y++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }
}

void blipvert::NVx_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    // Copy the y plane

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x++)
        {
            *pdst++ = 0x00;
            *pdst++ = *psrc++;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::NVx_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    for (int32_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uvplane + in_u;
        uint8_t* vp = in_uvplane + in_v;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            uint8_t u_val = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[2])) >> 1);
            uint8_t v_val = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[2])) >> 1);
            PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst)), u_val, v_val, yp[0], yp[1], yp[2], yp[3])
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride)), u_val, v_val, yp[in_stride], yp[1 + in_stride], yp[2 + in_stride], yp[3 + in_stride])

                pdst += 4;
            yp += 4;
            up += 4;
            vp += 4;
        }

        in_buf += in_stride_x_2;
        in_uvplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void blipvert::NVx_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    for (int32_t y = 0; y < in_uv_height - 1; y++)
    {
        yp = in_buf;
        up = in_uvplane + in_u;
        vp = in_uvplane + in_v;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[2])) >> 1);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[2])) >> 1);
            pdst[1] = yp[0];
            pdst[3] = yp[1];
            pdst[5] = yp[2];
            pdst[7] = yp[3];

            pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                static_cast<uint16_t>(up[2]) + \
                static_cast<uint16_t>(up[in_stride]) + \
                static_cast<uint16_t>(up[2 + in_stride])) >> 2);
            pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                static_cast<uint16_t>(vp[2]) + \
                static_cast<uint16_t>(vp[in_stride]) + \
                static_cast<uint16_t>(vp[2 + in_stride])) >> 2);
            pdst[1 + out_stride] = yp[in_stride];
            pdst[3 + out_stride] = yp[1 + in_stride];
            pdst[5 + out_stride] = yp[2 + in_stride];
            pdst[7 + out_stride] = yp[3 + in_stride];

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(up[4]) + static_cast<uint16_t>(up[6])) >> 1);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(vp[4]) + static_cast<uint16_t>(vp[6])) >> 1);
            pdst[8] = yp[4];
            pdst[9] = yp[5];
            pdst[10] = yp[6];
            pdst[11] = yp[7];

            pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[4]) + \
                static_cast<uint16_t>(up[6]) + \
                static_cast<uint16_t>(up[4 + in_stride]) + \
                static_cast<uint16_t>(up[6 + in_stride])) >> 2);
            pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[4]) + \
                static_cast<uint16_t>(vp[6]) + \
                static_cast<uint16_t>(vp[4 + in_stride]) + \
                static_cast<uint16_t>(vp[6 + in_stride])) >> 2);
            pdst[8 + out_stride] = yp[4 + in_stride];
            pdst[9 + out_stride] = yp[5 + in_stride];
            pdst[10 + out_stride] = yp[6 + in_stride];
            pdst[11 + out_stride] = yp[7 + in_stride];

            pdst += 12;
            yp += 8;
            up += 8;
            vp += 8;
        }

        in_buf += in_stride_x_2;
        in_uvplane += in_stride;
        out_buf += out_stride_x_2;
    }

    // Get the last line without reading past the u & v buffers
    yp = in_buf;
    up = in_uvplane + in_u;
    vp = in_uvplane + in_v;
    pdst = out_buf;

    for (int32_t x = 0; x < width; x += 8)
    {
        pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[2])) >> 1);
        pdst[2] = pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[2])) >> 1);
        pdst[1] = yp[0];
        pdst[3] = yp[1];
        pdst[5] = yp[2];
        pdst[7] = yp[3];

        pdst[1 + out_stride] = yp[in_stride];
        pdst[3 + out_stride] = yp[1 + in_stride];
        pdst[5 + out_stride] = yp[2 + in_stride];
        pdst[7 + out_stride] = yp[3 + in_stride];

        pdst[4] = pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[4]) + static_cast<uint16_t>(up[6])) >> 1);
        pdst[6] = pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[4]) + static_cast<uint16_t>(vp[6])) >> 1);
        pdst[8] = yp[4];
        pdst[9] = yp[5];
        pdst[10] = yp[6];
        pdst[11] = yp[7];

        pdst[8 + out_stride] = yp[4 + in_stride];
        pdst[9 + out_stride] = yp[5 + in_stride];
        pdst[10 + out_stride] = yp[6 + in_stride];
        pdst[11 + out_stride] = yp[7 + in_stride];

        pdst += 12;
        yp += 8;
        up += 4;
        vp += 4;
    }
}

void blipvert::NVx_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;


    // Copy the y plane
    if (out_stride != in_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }

    // Copy the u & v planes without scaling
    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* in_up = in_uvplane + in_u;
        uint8_t* in_vp = in_uvplane + in_v;
        uint8_t* out_up = out_uplane;
        uint8_t* out_vp = out_vplane;

        for (int32_t x = 0; x < out_uv_width; x++)
        {
            *out_up++ = *in_up;
            *out_vp++ = *in_vp;
            in_up += 2;
            in_vp += 2;
        }

        in_uvplane += in_stride;
        out_uplane += out_stride;
        out_vplane += out_stride;
    }
}

void blipvert::NVx_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width / 2;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;


    // Copy the y plane
    if (out_stride != in_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }

    // Copy the u & v planes
    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* in_up = in_uvplane + in_u;
        uint8_t* in_vp = in_uvplane + in_v;
        uint8_t* out_up = out_uvplane + out_u;
        uint8_t* out_vp = out_uvplane+ out_v;

        for (int32_t x = 0; x < out_uv_width; x++)
        {
            *out_up = *in_up;
            *out_vp = *in_vp;
            in_up += 2;
            in_vp += 2;
            out_up += 2;
            out_vp += 2;
        }

        in_uvplane += in_stride;
        out_uvplane += out_stride;
    }
}

void blipvert::NVx_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;


    for (int32_t y = 0; y < in_uv_height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uvplane + in_u;
        uint8_t* vp = in_uvplane + in_v;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[0] = *up;
            pdst[2] = *vp;
            pdst[1] = yp[0] | 0x01;
            pdst[3] = yp[1] | 0x01;

            pdst[0 + out_stride] = *up;
            pdst[2 + out_stride] = *vp;
            pdst[1 + out_stride] = yp[in_stride] | 0x01;
            pdst[3 + out_stride] = yp[1 + in_stride] | 0x01;

            pdst += 4;
            yp += 2;
            up += 2;
            vp += 2;
        }

        in_buf += in_stride_x_2;
        in_uvplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void blipvert::NVx_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_width = in->uv_width;
    int32_t in_uv_height = in->uv_slice_height;
    uint8_t* in_uvplane = in->uvplane;
    int16_t in_u = in->u_index;
    int16_t in_v = in->v_index;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    int32_t out_stride_x_2 = out_stride * 2;
    int32_t in_stride_x_2 = in_stride * 2;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    for (int32_t y = 0; y < in_uv_height - 1; y++)
    {
        yp = in_buf;
        up = in_uvplane + in_u;
        vp = in_uvplane + in_v;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[2])) >> 1);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[2])) >> 1);
            pdst[1] = yp[0] | 0x01;
            pdst[3] = yp[1] | 0x01;
            pdst[5] = yp[2] | 0x01;
            pdst[7] = yp[3] | 0x01;

            pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                static_cast<uint16_t>(up[2]) + \
                static_cast<uint16_t>(up[in_stride]) + \
                static_cast<uint16_t>(up[2 + in_stride])) >> 2);
            pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                static_cast<uint16_t>(vp[2]) + \
                static_cast<uint16_t>(vp[in_stride]) + \
                static_cast<uint16_t>(vp[2 + in_stride])) >> 2);
            pdst[1 + out_stride] = yp[in_stride] | 0x01;
            pdst[3 + out_stride] = yp[1 + in_stride] | 0x01;
            pdst[5 + out_stride] = yp[2 + in_stride] | 0x01;
            pdst[7 + out_stride] = yp[3 + in_stride] | 0x01;

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(up[4]) + static_cast<uint16_t>(up[6])) >> 1);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(vp[4]) + static_cast<uint16_t>(vp[6])) >> 1);
            pdst[8] = yp[4] | 0x01;
            pdst[9] = yp[5] | 0x01;
            pdst[10] = yp[6] | 0x01;
            pdst[11] = yp[7] | 0x01;

            pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[4]) + \
                static_cast<uint16_t>(up[6]) + \
                static_cast<uint16_t>(up[4 + in_stride]) + \
                static_cast<uint16_t>(up[6 + in_stride])) >> 2);
            pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[4]) + \
                static_cast<uint16_t>(vp[6]) + \
                static_cast<uint16_t>(vp[4 + in_stride]) + \
                static_cast<uint16_t>(vp[6 + in_stride])) >> 2);
            pdst[8 + out_stride] = yp[4 + in_stride] | 0x01;
            pdst[9 + out_stride] = yp[5 + in_stride] | 0x01;
            pdst[10 + out_stride] = yp[6 + in_stride] | 0x01;
            pdst[11 + out_stride] = yp[7 + in_stride] | 0x01;

            pdst += 12;
            yp += 8;
            up += 8;
            vp += 8;
        }

        in_buf += in_stride_x_2;
        in_uvplane += in_stride;
        out_buf += out_stride_x_2;
    }

    // Get the last line without reading past the u & v buffers
    yp = in_buf;
    up = in_uvplane + in_u;
    vp = in_uvplane + in_v;
    pdst = out_buf;

    for (int32_t x = 0; x < width; x += 8)
    {
        pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[2])) >> 1);
        pdst[2] = pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[2])) >> 1);
        pdst[1] = yp[0] | 0x01;
        pdst[3] = yp[1] | 0x01;
        pdst[5] = yp[2] | 0x01;
        pdst[7] = yp[3] | 0x01;

        pdst[1 + out_stride] = yp[in_stride] | 0x01;
        pdst[3 + out_stride] = yp[1 + in_stride] | 0x01;
        pdst[5 + out_stride] = yp[2 + in_stride] | 0x01;
        pdst[7 + out_stride] = yp[3 + in_stride] | 0x01;

        pdst[4] = pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[4]) + static_cast<uint16_t>(up[6])) >> 1);
        pdst[6] = pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[4]) + static_cast<uint16_t>(vp[6])) >> 1);
        pdst[8] = yp[4] | 0x01;
        pdst[9] = yp[5] | 0x01;
        pdst[10] = yp[6] | 0x01;
        pdst[11] = yp[7] | 0x01;

        pdst[8 + out_stride] = yp[4 + in_stride] | 0x01;
        pdst[9 + out_stride] = yp[5 + in_stride] | 0x01;
        pdst[10 + out_stride] = yp[6 + in_stride] | 0x01;
        pdst[11 + out_stride] = yp[7 + in_stride] | 0x01;

        pdst += 12;
        yp += 8;
        up += 4;
        vp += 4;
    }
}


//
// Y42T to YUV
//

void blipvert::Y42T_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) >> 1); // U
            pdst[1] = psrc[1] & 0xFE;
            pdst[2] = psrc[3] & 0xFE;
            pdst[3] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[6])) >> 1); // V
            pdst[4] = psrc[5] & 0xFE;
            pdst[5] = psrc[7] & 0xFE;

            psrc += 8;
            pdst += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y42T_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[0] = psrc[0];
            pdst[1] = psrc[1] & 0xFE;
            pdst[2] = psrc[2];

            pdst[3] = psrc[0];
            pdst[4] = psrc[3] & 0xFE;
            pdst[5] = psrc[2];

            psrc += 4;
            pdst += 6;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y42T_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 2)
        {
            pdst[0] = psrc[1] & 0xFE;
            pdst[1] = psrc[3] & 0xFE;

            psrc += 4;
            pdst += 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y42T_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 2)
        {
            *pdst++ = 0x00;
            *pdst++ = psrc[1] & 0xFE;
            *pdst++ = 0x00;
            *pdst++ = psrc[3] & 0xFE;

            psrc += 4;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y42T_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t x = 0; x < width; x += 4)
        {
            uint8_t u_val = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) >> 1);
            uint8_t v_val = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[6])) >> 1);
            PackCLJRDword(*pdst++, u_val, v_val, psrc[1] & 0xFE, psrc[3] & 0xFE, psrc[5] & 0xFE, psrc[7] & 0xFE)
            psrc += 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y42T_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) >> 1);     // U0
            pdst[1] = psrc[1] & 0xFE;                                                                                   // Y0
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[6])) >> 1);     // V0
            pdst[3] = psrc[3] & 0xFE;                                                                                   // Y1
            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[8]) + static_cast<uint16_t>(psrc[12])) >> 1);    // U4
            pdst[5] = psrc[5] & 0xFE;                                                                                   // Y2
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[10]) + static_cast<uint16_t>(psrc[14])) >> 1);   // V4
            pdst[7] = psrc[7] & 0xFE;                                                                                   // Y3
            pdst[8] = psrc[9] & 0xFE;                                                                                   // Y4
            pdst[9] = psrc[11] & 0xFE;                                                                                  // Y5
            pdst[10] = psrc[13] & 0xFE;                                                                                 // Y6
            pdst[11] = psrc[15] & 0xFE;                                                                                 // Y7

            psrc += 16;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y42T_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = psrc[2];
            pdst[1] = psrc[0];
            pdst[2] = psrc[1] & 0xFE;
            pdst[3] = psrc[1] & 0x01 ? 0xFF : 0x00;

            pdst[4] = psrc[2];
            pdst[5] = psrc[0];
            pdst[6] = psrc[3] & 0xFE;
            pdst[7] = psrc[3] & 0x01 ? 0xFF : 0x00;

            psrc += 4;
            pdst += 8;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;

        height--;
    }
}

void blipvert::Y42T_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;

        for (int32_t x = 0; x < out_uv_width; x++)
        {
            yp[0] = psrc[1] & 0xFE;
            yp[out_stride] = psrc[1 + in_stride] & 0xFE;
            yp[1] = psrc[3] & 0xFE;
            yp[1 + out_stride] = psrc[3 + in_stride] & 0xFE;

            out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[0 + in_stride])) >> 1);

            out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);
            psrc += 4;
            yp += 2;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        out_uplane += out_stride;
        out_vplane += out_stride;
    }
}

void blipvert::Y42T_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uvplane + out_u;
        uint8_t* vp = out_uvplane + out_v;

        for (int32_t x = 0; x < out_uv_width; x += 2)
        {
            yp[0] = psrc[1] & 0xFE;
            yp[out_stride] = psrc[1 + in_stride] & 0xFE;
            yp[1] = psrc[3] & 0xFE;
            yp[1 + out_stride] = psrc[3 + in_stride] & 0xFE;

            *up = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[0 + in_stride])) >> 1); // U

            *vp = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);  // V
            psrc += 4;
            yp += 2;
            up += 2;
            vp += 2;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;
        out_uvplane += out_stride;
    }
}

void blipvert::Y42T_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) >> 1);     // U0
            pdst[1] = psrc[1];                                                                                          // Y0
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[6])) >> 1);     // V0
            pdst[3] = psrc[3];                                                                                          // Y1
            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[8]) + static_cast<uint16_t>(psrc[12])) >> 1);    // U4
            pdst[5] = psrc[5];                                                                                          // Y2
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[10]) + static_cast<uint16_t>(psrc[14])) >> 1);   // V4
            pdst[7] = psrc[7];                                                                                          // Y3
            pdst[8] = psrc[9];                                                                                          // Y4
            pdst[9] = psrc[11];                                                                                         // Y5
            pdst[10] = psrc[13];                                                                                        // Y6
            pdst[11] = psrc[15];                                                                                        // Y7

            psrc += 16;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y42T_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;

        for (int32_t x = 0; x < out_uv_width; x++)
        {
            yp[0] = psrc[1] & 0xFE;
            yp[1] = psrc[3] & 0xFE;
            out_uplane[x] = psrc[0];
            out_vplane[x] = psrc[2];

            psrc += 4;
            yp += 2;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;

        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;
    }
}

//
// Misc YUV format transforms
//

void blipvert::UYVY_to_IUYV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    Progressive_to_Interlaced(height, width * 2, out->flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}

void blipvert::IUYV_to_UYVY(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    Interlaced_to_Progressive(height, width * 2, out->flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}

void blipvert::IY41_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    Interlaced_to_Progressive(height, width * 2, out->flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}

//
// Y41T to YUV
//


void blipvert::Y41T_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[out_u] = psrc[0];                  // U0
            pdst[out_v] = psrc[2];                  // V0
            pdst[out_y0] = psrc[1] & 0xFE;          // Y0
            pdst[out_y1] = psrc[3] & 0xFE;          // Y1

            pdst[out_u + 4] = psrc[0];              // U0
            pdst[out_v + 4] = psrc[2];              // V0
            pdst[out_y0 + 4] = psrc[5] & 0xFE;      // Y2
            pdst[out_y1 + 4] = psrc[7] & 0xFE;      // Y3

            pdst[out_u + 8] = psrc[4];              // U4
            pdst[out_v + 8] = psrc[6];              // V4
            pdst[out_y0 + 8] = psrc[8] & 0xFE;      // Y4
            pdst[out_y1 + 8] = psrc[9] & 0xFE;      // Y5

            pdst[out_u + 12] = psrc[4];             // U4
            pdst[out_v + 12] = psrc[6];             // V4
            pdst[out_y0 + 12] = psrc[10] & 0xFE;    // Y6
            pdst[out_y1 + 12] = psrc[11] & 0xFE;    // Y7   

            psrc += 12;
            pdst += 16;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41T_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;


    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t out_y_stride_x_2 = out_y_stride * 2;
    int32_t out_y_stride_x_3 = out_y_stride * 3;
    int32_t out_y_stride_x_4 = out_y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x += 4)
            {
                yp[0] = psrc[1] & 0xFE;
                yp[out_y_stride] = psrc[1 + in_stride] & 0xFE;
                yp[1] = psrc[3] & 0xFE;
                yp[1 + out_y_stride] = psrc[3 + in_stride] & 0xFE;

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride])) >> 1);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);

                yp[2] = psrc[5] & 0xFE;
                yp[2 + out_y_stride] = psrc[5 + in_stride] & 0xFE;
                yp[3] = psrc[7] & 0xFE;
                yp[3 + out_y_stride] = psrc[7 + in_stride] & 0xFE;

                out_uplane[x + 1] = out_uplane[x];
                out_vplane[x + 1] = out_vplane[x];

                yp[4] = psrc[8] & 0xFE;
                yp[4 + out_y_stride] = psrc[8 + in_stride] & 0xFE;
                yp[5] = psrc[9] & 0xFE;
                yp[5 + out_y_stride] = psrc[9 + in_stride] & 0xFE;

                out_uplane[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride])) >> 1);

                out_vplane[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride])) >> 1);

                yp[6] = psrc[10] & 0xFE;
                yp[6 + out_y_stride] = psrc[10 + in_stride] & 0xFE;
                yp[7] = psrc[11] & 0xFE;
                yp[7 + out_y_stride] = psrc[11 + in_stride] & 0xFE;

                out_uplane[x + 3] = out_uplane[x + 2];

                out_vplane[x + 3] = out_vplane[x + 2];

                psrc += 12;
                yp += 8;
            }

            in_buf += in_stride_x_2;
            out_buf += out_y_stride_x_2;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;

            for (int32_t x = 0; x < out_uv_width; x += 2)
            {
                yp[0] = psrc[1] & 0xFE;
                yp[out_y_stride] = psrc[1 + in_stride] & 0xFE;
                yp[out_y_stride_x_2] = psrc[1 + in_stride_x_2] & 0xFE;
                yp[out_y_stride_x_3] = psrc[1 + in_stride_x_3] & 0xFE;

                yp[1] = psrc[3] & 0xFE;
                yp[1 + out_y_stride] = psrc[3 + in_stride] & 0xFE;
                yp[1 + out_y_stride_x_2] = psrc[3 + in_stride_x_2] & 0xFE;
                yp[1 + out_y_stride_x_3] = psrc[3 + in_stride_x_3] & 0xFE;

                yp[2] = psrc[5] & 0xFE;
                yp[2 + out_y_stride] = psrc[5 + in_stride] & 0xFE;
                yp[2 + out_y_stride_x_2] = psrc[5 + in_stride_x_2] & 0xFE;
                yp[2 + out_y_stride_x_3] = psrc[5 + in_stride_x_3] & 0xFE;

                yp[3] = psrc[7] & 0xFE;
                yp[3 + out_y_stride] = psrc[7 + in_stride] & 0xFE;
                yp[3 + out_y_stride_x_2] = psrc[7 + in_stride_x_2] & 0xFE;
                yp[3 + out_y_stride_x_3] = psrc[7 + in_stride_x_3] & 0xFE;

                out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_3])) >> 2);

                out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride_x_3])) >> 2);

                yp[4] = psrc[8] & 0xFE;
                yp[4 + out_y_stride] = psrc[8 + in_stride] & 0xFE;
                yp[4 + out_y_stride_x_2] = psrc[8 + in_stride_x_2] & 0xFE;
                yp[4 + out_y_stride_x_3] = psrc[8 + in_stride_x_3] & 0xFE;

                yp[5] = psrc[9] & 0xFE;
                yp[5 + out_y_stride] = psrc[9 + in_stride] & 0xFE;
                yp[5 + out_y_stride_x_2] = psrc[9 + in_stride_x_2] & 0xFE;
                yp[5 + out_y_stride_x_3] = psrc[9 + in_stride_x_3] & 0xFE;

                yp[6] = psrc[10] & 0xFE;
                yp[6 + out_y_stride] = psrc[10 + in_stride] & 0xFE;
                yp[6 + out_y_stride_x_2] = psrc[10 + in_stride_x_2] & 0xFE;
                yp[6 + out_y_stride_x_3] = psrc[10 + in_stride_x_3] & 0xFE;

                yp[7] = psrc[11] & 0xFE;
                yp[7 + out_y_stride] = psrc[11 + in_stride] & 0xFE;
                yp[7 + out_y_stride_x_2] = psrc[11 + in_stride_x_2] & 0xFE;
                yp[7 + out_y_stride_x_3] = psrc[11 + in_stride_x_3] & 0xFE;

                out_uplane[x + 1] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride_x_3])) >> 2);

                out_vplane[x + 1] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride_x_3])) >> 2);

                psrc += 12;
                yp += 8;
            }

            in_buf += in_stride_x_4;
            out_buf += out_y_stride_x_4;

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;
        }
    }
}

void blipvert::Y41T_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[0];          // U0
            pdst[3] = psrc[2];          // V0
            pdst[1] = psrc[1] & 0xFE;   // Y0
            pdst[2] = psrc[3] & 0xFE;   // Y1
            pdst[4] = psrc[5] & 0xFE;   // Y2
            pdst[5] = psrc[7] & 0xFE;   // Y3

            pdst[6] = psrc[4];          // U4
            pdst[9] = psrc[6];          // V4
            pdst[7] = psrc[8] & 0xFE;   // Y4
            pdst[8] = psrc[9] & 0xFE;   // Y5
            pdst[10] = psrc[10] & 0xFE; // Y6
            pdst[11] = psrc[11] & 0xFE; // Y7

            psrc += 12;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41T_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[0];          // U0
            pdst[1] = psrc[1] & 0xFE;   // Y0
            pdst[2] = psrc[2];          // V0

            pdst[3] = psrc[0];          // U0      
            pdst[4] = psrc[3] & 0xFE;   // Y1
            pdst[5] = psrc[2];          // V0

            pdst[6] = psrc[0];          // U0
            pdst[7] = psrc[5] & 0xFE;   // Y2
            pdst[8] = psrc[2];          // V0

            pdst[9] = psrc[0];          // U0
            pdst[10] = psrc[7] & 0xFE;  // Y3
            pdst[11] = psrc[2];         // V0

            pdst[12] = psrc[4];         // U4
            pdst[13] = psrc[8] & 0xFE;  // Y4
            pdst[14] = psrc[6];         // V4

            pdst[15] = psrc[4];         // U4
            pdst[16] = psrc[9] & 0xFE;  // Y5
            pdst[17] = psrc[6];         // V4

            pdst[18] = psrc[4];         // U4
            pdst[19] = psrc[10] & 0xFE; // Y6
            pdst[20] = psrc[6];         // V4

            pdst[21] = psrc[4];         // U4
            pdst[22] = psrc[11] & 0xFE; // Y7
            pdst[23] = psrc[6];         // V4

            psrc += 12;
            pdst += 24;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41T_to_Y800(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[1] & 0xFE;      // Y0
            pdst[1] = psrc[3] & 0xFE;      // Y1
            pdst[2] = psrc[5] & 0xFE;      // Y2
            pdst[3] = psrc[7] & 0xFE;      // Y3
            pdst[4] = psrc[8] & 0xFE;      // Y4
            pdst[5] = psrc[9] & 0xFE;      // Y5
            pdst[6] = psrc[10] & 0xFE;     // Y6
            pdst[7] = psrc[11] & 0xFE;     // Y7

            psrc += 12;
            pdst += 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41T_to_Y16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = 0;
            pdst[1] = psrc[1] & 0xFE;       // Y0
            pdst[2] = 0;
            pdst[3] = psrc[3] & 0xFE;       // Y1
            pdst[4] = 0;
            pdst[5] = psrc[5] & 0xFE;       // Y2
            pdst[6] = 0;
            pdst[7] = psrc[7] & 0xFE;       // Y3
            pdst[8] = 0;
            pdst[9] = psrc[8] & 0xFE;       // Y4
            pdst[10] = 0;
            pdst[11] = psrc[9] & 0xFE;      // Y5
            pdst[12] = 0;
            pdst[13] = psrc[10] & 0xFE;     // Y6
            pdst[14] = 0;
            pdst[15] = psrc[11] & 0xFE;     // Y7

            psrc += 12;
            pdst += 16;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}


void blipvert::Y41T_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t x = 0; x < width; x += 8)
        {
            PackCLJRDword(*pdst++, psrc[0], psrc[2], psrc[1] & 0xFE, psrc[3] & 0xFE, psrc[5] & 0xFE, psrc[7] & 0xFE)
            PackCLJRDword(*pdst++, psrc[4], psrc[6], psrc[8] & 0xFE, psrc[9] & 0xFE, psrc[10] & 0xFE, psrc[11] & 0xFE)
            psrc += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41T_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = *psrc++;          // U0
            *pdst++ = *psrc++ & 0xFE;   // Y0
            *pdst++ = *psrc++;          // V0
            *pdst++ = *psrc++ & 0xFE;   // Y1
            *pdst++ = *psrc++;          // U4
            *pdst++ = *psrc++ & 0xFE;   // Y2
            *pdst++ = *psrc++;          // V4
            *pdst++ = *psrc++ & 0xFE;   // Y3
            *pdst++ = *psrc++ & 0xFE;   // Y4
            *pdst++ = *psrc++ & 0xFE;   // Y5
            *pdst++ = *psrc++ & 0xFE;   // Y6
            *pdst++ = *psrc++ & 0xFE;   // Y7

             hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void blipvert::Y41T_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    while (height)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        while (hcount)
        {
            uint8_t U = psrc[0];
            int32_t V = psrc[2];

            pdst[0] = V;
            pdst[1] = U;
            pdst[2] = psrc[1] & 0xFE;
            pdst[3] = psrc[1] & 0x01 ? 0xFF : 0x00;

            pdst[4] = V;
            pdst[5] = U;
            pdst[6] = psrc[3] & 0xFE;
            pdst[7] = psrc[3] & 0x01 ? 0xFF : 0x00;

            pdst[8] = V;
            pdst[9] = U;
            pdst[10] = psrc[5] & 0xFE;
            pdst[11] = psrc[5] & 0x01 ? 0xFF : 0x00;

            pdst[12] = V;
            pdst[13] = U;
            pdst[14] = psrc[7] & 0xFE;
            pdst[15] = psrc[7] & 0x01 ? 0xFF : 0x00;

            U = psrc[4];
            V = psrc[6];

            pdst[16] = V;
            pdst[17] = U;
            pdst[18] = psrc[8] & 0xFE;
            pdst[19] = psrc[8] & 0x01 ? 0xFF : 0x00;

            pdst[20] = V;
            pdst[21] = U;
            pdst[22] = psrc[9] & 0xFE;
            pdst[23] = psrc[9] & 0x01 ? 0xFF : 0x00;

            pdst[24] = V;
            pdst[25] = U;
            pdst[26] = psrc[10] & 0xFE;
            pdst[27] = psrc[10] & 0x01 ? 0xFF : 0x00;

            pdst[28] = V;
            pdst[29] = U;
            pdst[30] = psrc[11] & 0xFE;
            pdst[31] = psrc[11] & 0x01 ? 0xFF : 0x00;

            psrc += 12;
            pdst += 32;
            hcount -= 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}


void blipvert::Y41T_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;

        for (int32_t x = 0; x < out_uv_width; x += 4)
        {
            yp[0] = psrc[1] & 0xFE;
            yp[out_stride] = psrc[1 + in_stride] & 0xFE;
            yp[1] = psrc[3] & 0xFE;
            yp[1 + out_stride] = psrc[3 + in_stride] & 0xFE;

            out_uplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[in_stride])) >> 1);

            out_vplane[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);

            yp[2] = psrc[5] & 0xFE;
            yp[2 + out_stride] = psrc[5 + in_stride] & 0xFE;
            yp[3] = psrc[7] & 0xFE;
            yp[3 + out_stride] = psrc[7 + in_stride] & 0xFE;

            out_uplane[x + 1] = out_uplane[x];
            out_vplane[x + 1] = out_vplane[x];

            yp[4] = psrc[8] & 0xFE;
            yp[4 + out_stride] = psrc[8 + in_stride] & 0xFE;
            yp[5] = psrc[9] & 0xFE;
            yp[5 + out_stride] = psrc[9 + in_stride] & 0xFE;

            out_uplane[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                static_cast<uint16_t>(psrc[4 + in_stride])) >> 1);

            out_vplane[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                static_cast<uint16_t>(psrc[6 + in_stride])) >> 1);

            yp[6] = psrc[10] & 0xFE;
            yp[6 + out_stride] = psrc[10 + in_stride] & 0xFE;
            yp[7] = psrc[11] & 0xFE;
            yp[7 + out_stride] = psrc[11 + in_stride] & 0xFE;

            out_uplane[x + 3] = out_uplane[x + 2];

            out_vplane[x + 3] = out_vplane[x + 2];

            psrc += 12;
            yp += 8;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        out_uplane += out_stride;
        out_vplane += out_stride;
    }
}

void blipvert::Y41T_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = out_uvplane + out_u;
        uint8_t* vp = out_uvplane + out_v;

        for (int32_t x = 0; x < out_uv_width; x += 8)
        {
            yp[0] = psrc[1] & 0xFE;
            yp[out_stride] = psrc[1 + in_stride] & 0xFE;
            yp[1] = psrc[3] & 0xFE;
            yp[1 + out_stride] = psrc[3 + in_stride] & 0xFE;

            up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                static_cast<uint16_t>(psrc[in_stride])) >> 1);

            vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);

            yp[2] = psrc[5] & 0xFE;
            yp[2 + out_stride] = psrc[5 + in_stride] & 0xFE;
            yp[3] = psrc[7] & 0xFE;
            yp[3 + out_stride] = psrc[7 + in_stride] & 0xFE;

            up[x + 2] = up[x];
            vp[x + 2] = vp[x];

            yp[4] = psrc[8] & 0xFE;
            yp[4 + out_stride] = psrc[8 + in_stride] & 0xFE;
            yp[5] = psrc[9] & 0xFE;
            yp[5 + out_stride] = psrc[9 + in_stride] & 0xFE;

            up[x + 4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                static_cast<uint16_t>(psrc[4 + in_stride])) >> 1);

            vp[x + 4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                static_cast<uint16_t>(psrc[6 + in_stride])) >> 1);

            yp[6] = psrc[10] & 0xFE;
            yp[6 + out_stride] = psrc[10 + in_stride] & 0xFE;
            yp[7] = psrc[11] & 0xFE;
            yp[7 + out_stride] = psrc[11 + in_stride] & 0xFE;

            up[x + 6] = up[x + 2];

            vp[x + 6] = vp[x + 2];

            psrc += 12;
            yp += 8;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;
        out_uvplane += out_stride;
    }
}

void blipvert::Y41T_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[0];          // U0
            pdst[2] = psrc[2];          // V0
            pdst[1] = psrc[1];          // Y0
            pdst[3] = psrc[3];          // Y1

            pdst[0 + 4] = psrc[0];      // U0
            pdst[2 + 4] = psrc[2];      // V0
            pdst[1 + 4] = psrc[5];      // Y2
            pdst[3 + 4] = psrc[7];      // Y3

            pdst[0 + 8] = psrc[4];      // U4
            pdst[2 + 8] = psrc[6];      // V4
            pdst[1 + 8] = psrc[8];      // Y4
            pdst[3 + 8] = psrc[9];      // Y5

            pdst[0 + 12] = psrc[4];     // U4
            pdst[2 + 12] = psrc[6];     // V4
            pdst[1 + 12] = psrc[10];    // Y6
            pdst[3 + 12] = psrc[11];    // Y7

            psrc += 12;
            pdst += 16;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}
void blipvert::Y41T_to_YV16(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_stride = in->stride;
    int32_t width = in->width;
    int32_t height = in->height;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;

        for (int32_t x = 0; x < out_uv_width; x += 4)
        {
            yp[0] = psrc[1] & 0xFE;
            yp[1] = psrc[3] & 0xFE;
            out_uplane[x] = psrc[0];
            out_vplane[x] = psrc[2];

            yp[2] = psrc[5] & 0xFE;
            yp[3] = psrc[7] & 0xFE;
            out_uplane[x + 1] = psrc[0];
            out_vplane[x + 1] = psrc[2];

            yp[4] = psrc[8] & 0xFE;
            yp[5] = psrc[9] & 0xFE;
            out_uplane[x + 2] = psrc[4];
            out_vplane[x + 2] = psrc[6];

            yp[6] = psrc[10] & 0xFE;
            yp[7] = psrc[11] & 0xFE;
            out_uplane[x + 3] = psrc[4];
            out_vplane[x + 3] = psrc[6];

            psrc += 12;
            yp += 8;
        }

        in_buf += in_stride;
        out_buf += out_y_stride;

        out_uplane += out_uv_stride;
        out_vplane += out_uv_stride;
    }
}

//
// YV16 to YUV
//

void blipvert::YV16_to_PackedY422(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int16_t out_y0 = out->y0_index;
    int16_t out_y1 = out->y1_index;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < in_uv_width; x++)
        {
            pdst[out_y0] = *yp++;
            pdst[out_y1] = *yp++;
            pdst[out_u] = *up++;
            pdst[out_v] = *vp++;
            pdst += 4;
       }

        in_buf += in_y_stride;
        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
        out_buf += out_stride;
    }
}

void blipvert::YV16_to_PlanarYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_y_stride = out->y_stride;
    int32_t out_uv_stride = out->uv_stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;
    int32_t out_decimation = out->decimation;


    // Copy the y plane
    if (out_y_stride != in_y_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_y_stride;
            in_buf += in_y_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_y_stride * height);
    }

    int16_t in_uv_stride_x_2 = in_uv_stride * 2;
    int16_t in_uv_stride_x_3 = in_uv_stride * 3;
    int16_t in_uv_stride_x_4 = in_uv_stride * 4;

    if (out_decimation == 2)
    {
        // Copy the u & v planes from 2 x 1 to 2 x 2 scaling
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* in_up = in_uplane;
            uint8_t* in_vp = in_vplane;
            uint8_t* out_up = out_uplane;
            uint8_t* out_vp = out_vplane;

            for (int32_t x = 0; x < out_uv_width; x++)
            {
                *out_up++ = (static_cast<uint16_t>(*in_up) + static_cast<uint16_t>(*(in_up + in_uv_stride))) >> 1;
                *out_vp++ = (static_cast<uint16_t>(*in_vp) + static_cast<uint16_t>(*(in_vp + in_uv_stride))) >> 1;
                in_up++;
                in_vp++;
            }

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;

            in_uplane += in_uv_stride_x_2;
            in_vplane += in_uv_stride_x_2;
        }
    }
    else
    {
        // Copy the u & v planes from 2 x 1 to 4 x 4 scaling
        for (int32_t y = 0; y < out_uv_height; y++)
        {
            uint8_t* in_up = in_uplane;
            uint8_t* in_vp = in_vplane;
            uint8_t* out_up = out_uplane;
            uint8_t* out_vp = out_vplane;

            for (int32_t x = 0; x < out_uv_width; x++)
            {

                *out_up++ = (   static_cast<uint16_t>(*in_up) +                         static_cast<uint16_t>(*in_up + 1) + \
                                static_cast<uint16_t>(*(in_up + in_uv_stride)) +        static_cast<uint16_t>(*(in_up + 1 + in_uv_stride)) + \
                                static_cast<uint16_t>(*(in_up + in_uv_stride_x_2)) +    static_cast<uint16_t>(*(in_up + 1 + in_uv_stride_x_2)) + \
                                static_cast<uint16_t>(*(in_up + in_uv_stride_x_3)) +    static_cast<uint16_t>(*(in_up + 1 + in_uv_stride_x_3))) >> 3;

                *out_vp++ = (   static_cast<uint16_t>(*in_vp) +                         static_cast<uint16_t>(*in_vp + 1) + \
                                static_cast<uint16_t>(*(in_vp + in_uv_stride)) +        static_cast<uint16_t>(*(in_vp + 1 + in_uv_stride)) + \
                                static_cast<uint16_t>(*(in_vp + in_uv_stride_x_2)) +    static_cast<uint16_t>(*(in_vp + 1 + in_uv_stride_x_2)) + \
                                static_cast<uint16_t>(*(in_vp + in_uv_stride_x_3)) +    static_cast<uint16_t>(*(in_vp + 1 + in_uv_stride_x_3))) >> 3;

                in_up += 2;
                in_vp += 2;
            }

            out_uplane += out_uv_stride;
            out_vplane += out_uv_stride;

            in_uplane += in_uv_stride_x_4;
            in_vplane += in_uv_stride_x_4;
        }
    }
}

void blipvert::YV16_to_IYU1(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            *pdst++ = static_cast<uint8_t>((static_cast<uint16_t>(*up++) + static_cast<uint16_t>(*up++)) >> 1);
            *pdst++ = *yp++;
            *pdst++ = *yp++;
            *pdst++ = static_cast<uint8_t>((static_cast<uint16_t>(*vp++) + static_cast<uint16_t>(*vp++)) >> 1);
            *pdst++ = *yp++;
            *pdst++ = *yp++;
        }

        in_buf += in_y_stride;
        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
        out_buf += out_stride;
    }
}


void blipvert::YV16_to_IYU2(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < in_uv_width; x++)
        {
            *pdst++ = *up;
            *pdst++ = *yp++;
            *pdst++ = *vp;

            *pdst++ = *up++;
            *pdst++ = *yp++;
            *pdst++ = *vp++;
        }

        in_buf += in_y_stride;
        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
        out_buf += out_stride;
    }
}

void blipvert::YV16_to_CLJR(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            uint8_t u_val = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            uint8_t v_val = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst)), u_val, v_val, yp[0], yp[1], yp[2], yp[3])

            pdst += 4;
            yp += 4;
            up += 2;
            vp += 2;
        }

        in_buf += in_y_stride;
        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
        out_buf += out_stride;
    }
}


void blipvert::YV16_to_Y41P(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            pdst[1] = yp[0];
            pdst[3] = yp[1];
            pdst[5] = yp[2];
            pdst[7] = yp[3];

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
            pdst[8] = yp[4];
            pdst[9] = yp[5];
            pdst[10] = yp[6];
            pdst[11] = yp[7];
            pdst += 12;
            yp += 8;
            up += 4;
            vp += 4;
        }

        in_buf += in_y_stride;
        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
        out_buf += out_stride;
    }
}

void blipvert::YV16_to_AYUV(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int16_t y = 0; y < height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int16_t x = 0; x < in_uv_width; x++)
        {
            *pdst++ = *vp;
            *pdst++ = *up;
            *pdst++ = *yp++;
            *pdst++ = 0xFF;

            *pdst++ = *vp++;
            *pdst++ = *up++;
            *pdst++ = *yp++;
            *pdst++ = 0xFF;
        }

        in_buf += in_y_stride;
        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
        out_buf += out_stride;
    }
}

void blipvert::YV16_to_IMCx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uplane = out->uplane;
    uint8_t* out_vplane = out->vplane;

    // Copy the y plane
    if (out_stride != in_y_stride || out->flipped)
    {
        for (int32_t y = 0; y < height; y++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_y_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }

    // Copy the u & v planes 2 x 1 to 2 x 2
    for (int32_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* in_up = in_uplane;
        uint8_t* in_vp = in_vplane;
        uint8_t* out_up = out_uplane;
        uint8_t* out_vp = out_vplane;

        for (int32_t x = 0; x < out_uv_width; x++)
        {
            *out_up++ = (static_cast<uint16_t>(*in_up) + static_cast<uint16_t>(*(in_up++ + in_uv_stride))) >> 1;
            *out_vp++ = (static_cast<uint16_t>(*in_vp) + static_cast<uint16_t>(*(in_vp++ + in_uv_stride))) >> 1;
        }
        out_uplane += out_stride;
        out_vplane += out_stride;

        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
    }
}

void blipvert::YV16_to_NVx(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;
    int32_t out_uv_width = out->uv_width;
    int32_t out_uv_height = out->uv_slice_height;
    uint8_t* out_uvplane = out->uvplane;
    int16_t out_u = out->u_index;
    int16_t out_v = out->v_index;

    // Copy the y plane
    if (out_stride != in_y_stride || out->flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += out_stride;
            in_buf += in_y_stride;
        }
    }
    else
    {
        memcpy(out_buf, in_buf, out_stride * height);
    }

    // Copy the u & v planes 2 x 1 to 2 x 2 scaling
    for (uint16_t y = 0; y < out_uv_height; y++)
    {
        uint8_t* in_up = in_uplane;
        uint8_t* in_vp = in_vplane;
        uint8_t* out_up = out_uvplane + out_u;
        uint8_t* out_vp = out_uvplane + out_v;

        for (uint16_t x = 0; x < in_uv_width; x++)
        {
            *out_up = (static_cast<uint16_t>(*in_up) + static_cast<uint16_t>(*(in_up++ + in_uv_stride))) >> 1;
            *out_vp = (static_cast<uint16_t>(*in_vp) + static_cast<uint16_t>(*(in_vp++ + in_uv_stride))) >> 1;
            out_up += 2;
            out_vp += 2;
        }

        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
        out_uvplane += out_stride;
    }
}

void blipvert::YV16_to_Y42T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < in_uv_width; x++)
        {
            pdst[0] = *up++;
            pdst[2] = *vp++;
            pdst[1] = yp[0] | 0x01;
            pdst[3] = yp[1] | 0x01;

            pdst += 4;
            yp += 2;
        }

        in_buf += in_y_stride;
        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
        out_buf += out_stride;
    }
}

void blipvert::YV16_to_Y41T(Stage* in, Stage* out)
{
    uint8_t* in_buf = in->buf;
    int32_t in_y_stride = in->y_stride;
    int32_t width = in->width;
    int32_t height = in->height;
    int32_t in_uv_stride = in->uv_stride;
    int32_t in_uv_width = in->uv_width;
    uint8_t* in_uplane = in->uplane;
    uint8_t* in_vplane = in->vplane;

    uint8_t* out_buf = out->buf;
    int32_t out_stride = out->stride;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = in_uplane;
        uint8_t* vp = in_vplane;
        uint8_t* pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            pdst[2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            pdst[1] = yp[0] | 0x01;
            pdst[3] = yp[1] | 0x01;
            pdst[5] = yp[2] | 0x01;
            pdst[7] = yp[3] | 0x01;

            pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
            pdst[8] = yp[4] | 0x01;
            pdst[9] = yp[5] | 0x01;
            pdst[10] = yp[6] | 0x01;
            pdst[11] = yp[7] | 0x01;

            pdst += 12;
            yp += 8;
            up += 4;
            vp += 4;
        }

        in_buf += in_y_stride;
        in_uplane += in_uv_stride;
        in_vplane += in_uv_stride;
        out_buf += out_stride;
    }
}
