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

void PackedY422_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_y0, int32_t out_y1, int32_t out_u, int32_t out_v,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_y0, int32_t in_y1, int32_t in_u, int32_t in_v,
    bool flipped)
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

void PlanarYUV_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, int32_t out_decimation, bool out_ufirst,
    uint8_t* in_buf, int32_t in_stride, int32_t in_decimation, bool in_ufirst,
    bool flipped)
{
    int32_t out_uv_width = width / out_decimation;
    int32_t out_uv_height = height / out_decimation;
    int32_t in_uv_width = width / in_decimation;
    int32_t in_uv_height = height / in_decimation;

    int32_t out_y_stride, out_uv_stride;
    if (!out_stride)
    {
        out_y_stride = width;
        out_uv_stride = out_uv_width;
    }
    else
    {
        out_y_stride = out_stride;
        out_uv_stride = out_stride;
    }

    int32_t in_y_stride, in_uv_stride;
    if (!in_stride)
    {
        in_y_stride = width;
        in_uv_stride = in_uv_width;
    }
    else
    {
        in_y_stride = in_stride;
        in_uv_stride = in_stride;
    }

    uint8_t* out_yplane = out_buf;
    uint8_t* in_yplane = in_buf;

    uint8_t* out_vplane;
    uint8_t* out_uplane;
    if (out_ufirst)
    {
        out_uplane = out_buf + (out_y_stride * height);
        out_vplane = out_uplane + (out_uv_stride * out_uv_height);
    }
    else
    {
        out_vplane = out_buf + (out_y_stride * height);
        out_uplane = out_vplane + (out_uv_stride * out_uv_height);
    }

    uint8_t* in_vplane;
    uint8_t* in_uplane;
    if (in_ufirst)
    {
        in_uplane = in_buf + (in_y_stride * height);
        in_vplane = in_uplane + (in_uv_stride * in_uv_height);
    }
    else
    {
        in_vplane = in_buf + (in_y_stride * height);
        in_uplane = in_vplane + (in_uv_stride * in_uv_height);
    }

    if (flipped)
    {
        out_yplane += (out_y_stride * (height - 1));
        out_uplane += (out_uv_stride * (out_uv_height - 1));
        out_vplane += (out_uv_stride * (out_uv_height - 1));
        out_y_stride = -out_y_stride;
        out_uv_stride = -out_uv_stride;
    }

    // Copy the y plane
    if (out_y_stride != in_y_stride || flipped)
    {
        for (int32_t line = 0; line < height; line++)
        {
            memcpy(out_yplane, in_yplane, width);
            out_yplane += out_y_stride;
            in_yplane += in_y_stride;
        }
    }
    else
    {
        memcpy(out_yplane, in_yplane, out_y_stride * height);
    }

    if (out_decimation == in_decimation)
    {
        // Copy the u & v planes without scaling
        if (out_uv_stride != in_uv_stride || flipped)
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

void PackedY422_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, int32_t out_decimation, bool out_ufirst,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_y0, int32_t in_y1, int32_t in_u, int32_t in_v,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 2;

    int32_t uv_width = width / out_decimation;
    int32_t uv_height = height / out_decimation;

    int32_t y_stride, uv_stride;
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
    if (out_ufirst)
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

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_3 = y_stride * 3;
    int32_t y_stride_x_4 = y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;

            for (int32_t x = 0; x < uv_width; x++)
            {
                yp[0] = psrc[in_y0];
                yp[y_stride] = psrc[in_y0 + in_stride];
                yp[1] = psrc[in_y1];
                yp[1 + y_stride] = psrc[in_y1 + in_stride];

                up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride])) >> 1);

                vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + \
                    static_cast<uint16_t>(psrc[in_v + in_stride])) >> 1);
                psrc += 4;
                yp += 2;
            }

            in_buf += in_stride_x_2;
            out_buf += y_stride_x_2;

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;

            for (int32_t x = 0; x < uv_width; x++)
            {
                yp[0] = psrc[in_y0];
                yp[y_stride] = psrc[in_y0 + in_stride];
                yp[y_stride_x_2] = psrc[in_y0 + in_stride_x_2];
                yp[y_stride_x_3] = psrc[in_y0 + in_stride_x_3];

                yp[1] = psrc[in_y1];
                yp[1 + y_stride] = psrc[in_y1 + in_stride];
                yp[1 + y_stride_x_2] = psrc[in_y1 + in_stride_x_2];
                yp[1 + y_stride_x_3] = psrc[in_y1 + in_stride_x_3];

                yp[2] = psrc[in_y0 + 4];
                yp[2 + y_stride] = psrc[in_y0 + in_stride + 4];
                yp[2 + y_stride_x_2] = psrc[in_y0 + in_stride_x_2 + 4];
                yp[2 + y_stride_x_3] = psrc[in_y0 + in_stride_x_3 + 4];

                yp[3] = psrc[in_y1 + 4];
                yp[3 + y_stride] = psrc[in_y1 + in_stride + 4];
                yp[3 + y_stride_x_2] = psrc[in_y1 + in_stride_x_2 + 4];
                yp[3 + y_stride_x_3] = psrc[in_y1 + in_stride_x_3 + 4];

                up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + \
                    static_cast<uint16_t>(psrc[in_u + 4]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride + 4]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride_x_2 + 4]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride_x_3]) + \
                    static_cast<uint16_t>(psrc[in_u + in_stride_x_3 + 4])) >> 3);

                vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + \
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
            out_buf += y_stride_x_4;

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

void PlanarYUV_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_y0, int32_t out_y1, int32_t out_u, int32_t out_v,
    uint8_t* in_buf, int32_t in_stride, int32_t in_decimation, bool in_ufirst,
    bool flipped)
{
    if (!out_stride)
        out_stride = width * 2;

    int32_t uv_width = width / in_decimation;
    int32_t uv_height = height / in_decimation;

    int32_t y_stride, uv_stride;
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
    if (in_ufirst)
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
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_3 = y_stride * 3;
    int32_t y_stride_x_4 = y_stride * 4;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 2)
            {
                pdst[out_u] = *up;
                pdst[out_v] = *vp;
                pdst[out_y0] = yp[0];
                pdst[out_y1] = yp[1];

                pdst[out_u + out_stride] = *up;
                pdst[out_v + out_stride] = *vp;
                pdst[out_y0 + out_stride] = yp[y_stride];
                pdst[out_y1 + out_stride] = yp[1 + y_stride];

                pdst += 4;
                yp += 2;
                up += 1;
                vp += 1;
            }

            in_buf += y_stride_x_2;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
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
                pdst[out_y0 + out_stride] = yp[y_stride];
                pdst[out_y1 + out_stride] = yp[1 + y_stride];

                // column 1 row 3
                pdst[out_u + out_stride_x_2] = *up;
                pdst[out_v + out_stride_x_2] = *vp;
                pdst[out_y0 + out_stride_x_2] = yp[y_stride_x_2];
                pdst[out_y1 + out_stride_x_2] = yp[1 + y_stride_x_2];

                // column 1 row 4
                pdst[out_u + out_stride_x_3] = *up;
                pdst[out_v + out_stride_x_3] = *vp;
                pdst[out_y0 + out_stride_x_3] = yp[y_stride_x_3];
                pdst[out_y1 + out_stride_x_3] = yp[1 + y_stride_x_3];

                // column 2 row 1
                pdst[out_u + 4] = *up;
                pdst[out_v + 4] = *vp;
                pdst[out_y0 + 4] = yp[2];
                pdst[out_y1 + 4] = yp[3];

                // column 2 row 2
                pdst[out_u + out_stride + 4] = *up;
                pdst[out_v + out_stride + 4] = *vp;
                pdst[out_y0 + out_stride + 4] = yp[2 + y_stride];
                pdst[out_y1 + out_stride + 4] = yp[3 + y_stride];

                // column 2 row 3
                pdst[out_u + out_stride_x_2 + 4] = *up;
                pdst[out_v + out_stride_x_2 + 4] = *vp;
                pdst[out_y0 + out_stride_x_2 + 4] = yp[2 + y_stride_x_2];
                pdst[out_y1 + out_stride_x_2 + 4] = yp[3 + y_stride_x_2];

                // column 2 row 4
                pdst[out_u + out_stride_x_3 + 4] = *up;
                pdst[out_v + out_stride_x_3 + 4] = *vp;
                pdst[out_y0 + out_stride_x_3 + 4] = yp[2 + y_stride_x_3];
                pdst[out_y1 + out_stride_x_3 + 4] = yp[3 + y_stride_x_3];

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

void PlanarYUV_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride, int32_t in_decimation, bool in_ufirst,
    bool flipped)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    int32_t uv_width = width / in_decimation;
    int32_t uv_height = height / in_decimation;

    int32_t y_stride, uv_stride;
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
    if (in_ufirst)
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
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_3 = y_stride * 3;
    int32_t y_stride_x_4 = y_stride * 4;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < height - 2; y += 2)
        {
            yp = in_buf;
            up = uplane;
            vp = vplane;
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
                    static_cast<uint16_t>(up[uv_stride]) + \
                    static_cast<uint16_t>(up[1 + uv_stride])) >> 2);
                pdst[3 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[uv_stride]) + \
                    static_cast<uint16_t>(vp[1 + uv_stride])) >> 2);
                pdst[1 + out_stride] = yp[y_stride];
                pdst[2 + out_stride] = yp[1 + y_stride];
                pdst[4 + out_stride] = yp[2 + y_stride];
                pdst[5 + out_stride] = yp[3 + y_stride];

                pdst += 6;
                yp += 4;
                up += 2;
                vp += 2;
            }

            in_buf += y_stride_x_2;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_2;
        }

        // Get the last line without reading past the input u & v buffers
        yp = in_buf;
        up = uplane;
        vp = vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
            pdst[3] = pdst[3 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
            pdst[1] = yp[0];
            pdst[2] = yp[1];
            pdst[4] = yp[2];
            pdst[5] = yp[3];
            pdst[1 + out_stride] = yp[y_stride];
            pdst[2 + out_stride] = yp[1 + y_stride];
            pdst[4 + out_stride] = yp[2 + y_stride];
            pdst[5 + out_stride] = yp[3 + y_stride];

            pdst += 6;
            yp += 4;
            up += 2;
            vp += 2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < height - 4; y += 4)
        {
            yp = in_buf;
            up = uplane;
            vp = vplane;
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
                    (static_cast<int32_t>(up[uv_stride]) * 256)) >> 10);
                pdst[3 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 768) + \
                    (static_cast<int32_t>(vp[uv_stride]) * 256)) >> 10);
                pdst[1 + out_stride] = yp[y_stride];
                pdst[2 + out_stride] = yp[1 + y_stride];
                pdst[4 + out_stride] = yp[2 + y_stride];
                pdst[5 + out_stride] = yp[3 + y_stride];

                pdst[out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[uv_stride])) >> 1);
                pdst[3 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[uv_stride])) >> 1);
                pdst[1 + out_stride_x_2] = yp[y_stride_x_2];
                pdst[2 + out_stride_x_2] = yp[1 + y_stride_x_2];
                pdst[4 + out_stride_x_2] = yp[2 + y_stride_x_2];
                pdst[5 + out_stride_x_2] = yp[3 + y_stride_x_2];

                pdst[out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 256) + \
                    (static_cast<int32_t>(up[uv_stride]) * 768)) >> 10);
                pdst[3 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 256) + \
                    (static_cast<int32_t>(vp[uv_stride]) * 768)) >> 10);
                pdst[1 + out_stride_x_3] = yp[y_stride_x_3];
                pdst[2 + out_stride_x_3] = yp[1 + y_stride_x_3];
                pdst[4 + out_stride_x_3] = yp[2 + y_stride_x_3];
                pdst[5 + out_stride_x_3] = yp[3 + y_stride_x_3];

                pdst += 6;
                yp += 4;
                up++;
                vp++;
            }

            in_buf += y_stride_x_4;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_4;
        }

        // Get the last line without reading past the input u & v buffers
        yp = in_buf;
        up = uplane;
        vp = vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 4)
        {
            pdst[0] = pdst[out_stride] = pdst[out_stride_x_2] = pdst[out_stride_x_3] = up[0];
            pdst[3] = pdst[3 + out_stride] = pdst[3 + out_stride_x_2] = pdst[3 + out_stride_x_3] = vp[0];
            pdst[1] = yp[0];
            pdst[2] = yp[1];
            pdst[4] = yp[2];
            pdst[5] = yp[3];

            pdst[1 + out_stride] = yp[y_stride];
            pdst[2 + out_stride] = yp[1 + y_stride];
            pdst[4 + out_stride] = yp[2 + y_stride];
            pdst[5 + out_stride] = yp[3 + y_stride];

            pdst[1 + out_stride_x_2] = yp[y_stride_x_2];
            pdst[2 + out_stride_x_2] = yp[1 + y_stride_x_2];
            pdst[4 + out_stride_x_2] = yp[2 + y_stride_x_2];
            pdst[5 + out_stride_x_2] = yp[3 + y_stride_x_2];

            pdst[1 + out_stride_x_3] = yp[y_stride_x_3];
            pdst[2 + out_stride_x_3] = yp[1 + y_stride_x_3];
            pdst[4 + out_stride_x_3] = yp[2 + y_stride_x_3];
            pdst[5 + out_stride_x_3] = yp[3 + y_stride_x_3];

            pdst += 6;
            yp += 4;
            up++;
            vp++;
        }
    }
}

void IYU1_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, int32_t out_decimation, bool out_ufirst,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 12 / 8;

    int32_t uv_width = width / out_decimation;
    int32_t uv_height = height / out_decimation;

    int32_t y_stride, uv_stride;
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
    if (out_ufirst)
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

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_3 = y_stride * 3;
    int32_t y_stride_x_4 = y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;

            for (int32_t x = 0; x < uv_width; x += 2)
            {
                yp[0] = psrc[1];
                yp[y_stride] = psrc[1 + in_stride];
                yp[1] = psrc[2];
                yp[1 + y_stride] = psrc[2 + in_stride];

                up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride])) >> 1);

                vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[3]) + \
                    static_cast<uint16_t>(psrc[3 + in_stride])) >> 1);

                yp[2] = psrc[4];
                yp[2 + y_stride] = psrc[4 + in_stride];
                yp[3] = psrc[5];
                yp[3 + y_stride] = psrc[5 + in_stride];

                up[x + 1] = up[x];
                vp[x + 1] = vp[x];

                psrc += 6;
                yp += 4;
            }

            in_buf += in_stride_x_2;
            out_buf += y_stride_x_2;

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;

            for (int32_t x = 0; x < uv_width; x++)
            {
                yp[0] = psrc[1];
                yp[y_stride] = psrc[1 + in_stride];
                yp[y_stride_x_2] = psrc[1 + in_stride_x_2];
                yp[y_stride_x_3] = psrc[1 + in_stride_x_3];

                yp[1] = psrc[2];
                yp[1 + y_stride] = psrc[2 + in_stride];
                yp[1 + y_stride_x_2] = psrc[2 + in_stride_x_2];
                yp[1 + y_stride_x_3] = psrc[2 + in_stride_x_3];

                yp[2] = psrc[4];
                yp[2 + y_stride] = psrc[4 + in_stride];
                yp[2 + y_stride_x_2] = psrc[4 + in_stride_x_2];
                yp[2 + y_stride_x_3] = psrc[4 + in_stride_x_3];

                yp[3] = psrc[5];
                yp[3 + y_stride] = psrc[5 + in_stride];
                yp[3 + y_stride_x_2] = psrc[5 + in_stride_x_2];
                yp[3 + y_stride_x_3] = psrc[5 + in_stride_x_3];

                up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_3])) >> 2);

                vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[3]) + \
                    static_cast<uint16_t>(psrc[3 + in_stride]) + \
                    static_cast<uint16_t>(psrc[3 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[3 + in_stride_x_3])) >> 2);
                psrc += 6;
                yp += 4;
            }

            in_buf += in_stride_x_4;
            out_buf += y_stride_x_4;

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

void PackedY422_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_y0, int32_t in_y1, int32_t in_u, int32_t in_v,
    bool flipped)
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

void IYU1_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_y0, int32_t out_y1, int32_t out_u, int32_t out_v,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 12 / 8;

    if (!out_stride)
        out_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void PackedY422_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_y0, int32_t in_y1, int32_t in_u, int32_t in_v,
    bool flipped)
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

void IYU2_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_y0, int32_t out_y1, int32_t out_u, int32_t out_v,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
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

void PlanarYUV_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride, int32_t in_decimation, bool in_ufirst,
    bool flipped)
{
    if (!out_stride)
        out_stride = width * 3;

    int32_t uv_width = width / in_decimation;
    int32_t uv_height = height / in_decimation;

    int32_t y_stride, uv_stride;
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
    if (in_ufirst)
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
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_3 = y_stride * 3;
    int32_t y_stride_x_4 = y_stride * 4;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 2)
            {
                // column 1 row 1
                pdst[0] = *up;
                pdst[1] = yp[0];
                pdst[2] = *vp;

                // column 1 row 2
                pdst[out_stride] = *up;
                pdst[1 + out_stride] = yp[y_stride];
                pdst[2 + out_stride] = *vp;

                // column 2 row 1
                pdst[3] = *up;
                pdst[4] = yp[1];
                pdst[5] = *vp;

                // column 2 row 2
                pdst[3 + out_stride] = *up;
                pdst[4 + out_stride] = yp[1 + y_stride];
                pdst[5 + out_stride] = *vp;
                pdst += 6;
                yp += 2;
                up += 1;
                vp += 1;
            }

            in_buf += y_stride_x_2;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                for (int32_t row = 0; row < 4; row++)
                {
                    for (int32_t col = 0; col < 4; col++)
                    {
                        pdst[col * 3 + out_stride * row] = *up;
                        pdst[1 + col * 3 + out_stride * row] = yp[col + y_stride * row];
                        pdst[2 + col * 3 + out_stride * row] = *vp;
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

void IYU2_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, int32_t out_decimation, bool out_ufirst,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 3;

    int32_t uv_width = width / out_decimation;
    int32_t uv_height = height / out_decimation;

    int32_t y_stride, uv_stride;
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
    if (out_ufirst)
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

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_4 = y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;

            for (int32_t x = 0; x < uv_width; x++)
            {
                yp[0] = psrc[1];
                yp[y_stride] = psrc[1 + in_stride];
                yp[1] = psrc[4];
                yp[1 + y_stride] = psrc[4 + in_stride];

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
            out_buf += y_stride_x_2;

            uplane += uv_stride;
            vplane += uv_stride;
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
                    ys += y_stride;
                }

                u_avg >>= 4;
                v_avg >>= 4;

                uplane[x >> 2] = static_cast<uint8_t>(u_avg);
                vplane[x >> 2] = static_cast<uint8_t>(v_avg);

                psrc += 12;
                yp += 4;
            }
            in_buf += (in_stride_x_4);
            out_buf += (y_stride_x_4);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

void PackedY422_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_y0, int32_t in_y1, int32_t in_u, int32_t in_v,
    bool flipped)
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

void PlanarYUV_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_decimation, bool in_ufirst,
    bool flipped)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    int32_t uv_width = width / in_decimation;
    int32_t uv_height = height / in_decimation;

    int32_t y_stride, uv_stride;
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
    if (in_ufirst)
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
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_3 = y_stride * 3;
    int32_t y_stride_x_4 = y_stride * 4;

    uint8_t* yp;
    uint8_t* up;
    uint8_t* vp;
    uint8_t* pdst;

    if (in_decimation == 2)
    {
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
                    static_cast<uint16_t>(up[uv_stride]) + \
                    static_cast<uint16_t>(up[1 + uv_stride])) >> 2);
                pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[uv_stride]) + \
                    static_cast<uint16_t>(vp[1 + uv_stride])) >> 2);
                pdst[1 + out_stride] = yp[y_stride];
                pdst[3 + out_stride] = yp[1 + y_stride];
                pdst[5 + out_stride] = yp[2 + y_stride];
                pdst[7 + out_stride] = yp[3 + y_stride];

                pdst[4] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
                pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
                pdst[8] = yp[4];
                pdst[9] = yp[5];
                pdst[10] = yp[6];
                pdst[11] = yp[7];

                pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + \
                    static_cast<uint16_t>(up[3]) + \
                    static_cast<uint16_t>(up[2 + uv_stride]) + \
                    static_cast<uint16_t>(up[3 + uv_stride])) >> 2);
                pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + \
                    static_cast<uint16_t>(vp[3]) + \
                    static_cast<uint16_t>(vp[2 + uv_stride]) + \
                    static_cast<uint16_t>(vp[3 + uv_stride])) >> 2);
                pdst[8 + out_stride] = yp[4 + y_stride];
                pdst[9 + out_stride] = yp[5 + y_stride];
                pdst[10 + out_stride] = yp[6 + y_stride];
                pdst[11 + out_stride] = yp[7 + y_stride];

                pdst += 12;
                yp += 8;
                up += 4;
                vp += 4;
            }

            in_buf += y_stride_x_2;
            uplane += uv_stride;
            vplane += uv_stride;
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

            pdst[1 + out_stride] = yp[y_stride];
            pdst[3 + out_stride] = yp[1 + y_stride];
            pdst[5 + out_stride] = yp[2 + y_stride];
            pdst[7 + out_stride] = yp[3 + y_stride];

            pdst[4] = pdst[4 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[2]) + static_cast<uint16_t>(up[3])) >> 1);
            pdst[6] = pdst[6 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[2]) + static_cast<uint16_t>(vp[3])) >> 1);
            pdst[8] = yp[4];
            pdst[9] = yp[5];
            pdst[10] = yp[6];
            pdst[11] = yp[7];

            pdst[8 + out_stride] = yp[4 + y_stride];
            pdst[9 + out_stride] = yp[5 + y_stride];
            pdst[10 + out_stride] = yp[6 + y_stride];
            pdst[11 + out_stride] = yp[7 + y_stride];

            pdst += 12;
            yp += 8;
            up += 4;
            vp += 4;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < height - 4; y += 4)
        {
            yp = in_buf;
            up = uplane;
            vp = vplane;
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
                    (static_cast<int32_t>(up[uv_stride]) * 256)) >> 10);
                pdst[2 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 768) + \
                    (static_cast<int32_t>(vp[uv_stride]) * 256)) >> 10);
                pdst[1 + out_stride] = yp[y_stride];
                pdst[3 + out_stride] = yp[1 + y_stride];
                pdst[5 + out_stride] = yp[2 + y_stride];
                pdst[7 + out_stride] = yp[3 + y_stride];

                pdst[out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[uv_stride])) >> 1);
                pdst[2 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[uv_stride])) >> 1);
                pdst[1 + out_stride_x_2] = yp[y_stride_x_2];
                pdst[3 + out_stride_x_2] = yp[1 + y_stride_x_2];
                pdst[5 + out_stride_x_2] = yp[2 + y_stride_x_2];
                pdst[7 + out_stride_x_2] = yp[3 + y_stride_x_2];

                pdst[out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 256) + \
                    (static_cast<int32_t>(up[uv_stride]) * 768)) >> 10);
                pdst[2 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 256) + \
                    (static_cast<int32_t>(vp[uv_stride]) * 768)) >> 10);
                pdst[1 + out_stride_x_3] = yp[y_stride_x_3];
                pdst[3 + out_stride_x_3] = yp[1 + y_stride_x_3];
                pdst[5 + out_stride_x_3] = yp[2 + y_stride_x_3];
                pdst[7 + out_stride_x_3] = yp[3 + y_stride_x_3];

                pdst[4] = up[1];
                pdst[6] = vp[1];
                pdst[8] = yp[4];
                pdst[9] = yp[5];
                pdst[10] = yp[6];
                pdst[11] = yp[7];

                pdst[4 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(up[1]) * 768) + \
                    (static_cast<int32_t>(up[1 + uv_stride]) * 256)) >> 10);
                pdst[6 + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(vp[1]) * 768) + \
                    (static_cast<int32_t>(vp[1 + uv_stride]) * 256)) >> 10);
                pdst[8 + out_stride] = yp[4 + y_stride];
                pdst[9 + out_stride] = yp[5 + y_stride];
                pdst[10 + out_stride] = yp[6 + y_stride];
                pdst[11 + out_stride] = yp[7 + y_stride];

                pdst[4 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(up[1]) + \
                    static_cast<uint16_t>(up[1 + uv_stride])) >> 1);
                pdst[6 + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[1 + uv_stride])) >> 1);
                pdst[8 + out_stride_x_2] = yp[4 + y_stride_x_2];
                pdst[9 + out_stride_x_2] = yp[5 + y_stride_x_2];
                pdst[10 + out_stride_x_2] = yp[6 + y_stride_x_2];
                pdst[11 + out_stride_x_2] = yp[7 + y_stride_x_2];

                pdst[4 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(up[1]) * 256) + \
                    (static_cast<int32_t>(up[1 + uv_stride]) * 768)) >> 10);
                pdst[6 + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(vp[1]) * 256) + \
                    (static_cast<int32_t>(vp[1 + uv_stride]) * 768)) >> 10);
                pdst[8 + out_stride_x_3] = yp[4 + y_stride_x_3];
                pdst[9 + out_stride_x_3] = yp[5 + y_stride_x_3];
                pdst[10 + out_stride_x_3] = yp[6 + y_stride_x_3];
                pdst[11 + out_stride_x_3] = yp[7 + y_stride_x_3];

                pdst += 12;
                yp += 8;
                up += 2;
                vp += 2;
            }

            in_buf += y_stride_x_4;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_4;
        }

        // Get the last line without reading past the u & v buffers
        yp = in_buf;
        up = uplane;
        vp = vplane;
        pdst = out_buf;

        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = pdst[out_stride] = pdst[out_stride_x_2] = pdst[out_stride_x_3] = up[0];
            pdst[2] = pdst[2 + out_stride] = pdst[2 + out_stride_x_2] = pdst[2 + out_stride_x_3] = vp[0];
            pdst[1] = yp[0];
            pdst[3] = yp[1];
            pdst[5] = yp[2];
            pdst[7] = yp[3];

            pdst[1 + out_stride] = yp[y_stride];
            pdst[3 + out_stride] = yp[1 + y_stride];
            pdst[5 + out_stride] = yp[2 + y_stride];
            pdst[7 + out_stride] = yp[3 + y_stride];

            pdst[1 + out_stride_x_2] = yp[y_stride_x_2];
            pdst[3 + out_stride_x_2] = yp[1 + y_stride_x_2];
            pdst[5 + out_stride_x_2] = yp[2 + y_stride_x_2];
            pdst[7 + out_stride_x_2] = yp[3 + y_stride_x_2];

            pdst[1 + out_stride_x_3] = yp[y_stride_x_3];
            pdst[3 + out_stride_x_3] = yp[1 + y_stride_x_3];
            pdst[5 + out_stride_x_3] = yp[2 + y_stride_x_3];
            pdst[7 + out_stride_x_3] = yp[3 + y_stride_x_3];

            pdst[4] = pdst[4 + out_stride] = pdst[4 + out_stride_x_2] = pdst[4 + out_stride_x_3] = up[1];
            pdst[6] = pdst[6 + out_stride] = pdst[6 + out_stride_x_2] = pdst[6 + out_stride_x_3] = vp[1];
            pdst[8] = yp[4];
            pdst[9] = yp[5];
            pdst[10] = yp[6];
            pdst[11] = yp[7];

            pdst[8 + out_stride] = yp[4 + y_stride];
            pdst[9 + out_stride] = yp[5 + y_stride];
            pdst[10 + out_stride] = yp[6 + y_stride];
            pdst[11 + out_stride] = yp[7 + y_stride];

            pdst[8 + out_stride_x_2] = yp[4 + y_stride_x_2];
            pdst[9 + out_stride_x_2] = yp[5 + y_stride_x_2];
            pdst[10 + out_stride_x_2] = yp[6 + y_stride_x_2];
            pdst[11 + out_stride_x_2] = yp[7 + y_stride_x_2];

            pdst[8 + out_stride_x_3] = yp[4 + y_stride_x_3];
            pdst[9 + out_stride_x_3] = yp[5 + y_stride_x_3];
            pdst[10 + out_stride_x_3] = yp[6 + y_stride_x_3];
            pdst[11 + out_stride_x_3] = yp[7 + y_stride_x_3];

            pdst += 12;
            yp += 8;
            up += 2;
            vp += 2;
        }
    }
}

void PackedY422_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_y0, int32_t in_y1, int32_t in_u, int32_t in_v,
    bool flipped)
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

void PlanarYUV_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_decimation, bool in_ufirst,
    bool flipped)
{
    if (!out_stride)
        out_stride = width;

    int32_t uv_width = width / in_decimation;
    int32_t uv_height = height / in_decimation;

    int32_t y_stride, uv_stride;
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
    if (in_ufirst)
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
    int32_t out_stride_x_3 = out_stride * 3;
    int32_t out_stride_x_4 = out_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_3 = y_stride * 3;
    int32_t y_stride_x_4 = y_stride * 4;

    if (in_decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                uint8_t u_val = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
                uint8_t v_val = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst)), u_val, v_val, yp[0], yp[1], yp[2], yp[3])
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride)), u_val, v_val, yp[y_stride], yp[1 + y_stride], yp[2 + y_stride], yp[3 + y_stride])

                pdst += 4;
                yp += 4;
                up += 2;
                vp += 2;
            }

            in_buf += y_stride_x_2;
            uplane += uv_stride;
            vplane += uv_stride;
            out_buf += out_stride_x_2;
        }
    }
    else if (in_decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
            uint8_t* yp = in_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;
            uint8_t* pdst = out_buf;

            for (int32_t x = 0; x < width; x += 4)
            {
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst)), *up, *vp, yp[0], yp[1], yp[2], yp[3])
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride)), *up, *vp, yp[y_stride], yp[1 + y_stride], yp[2 + y_stride], yp[3 + y_stride])
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride_x_2)), *up, *vp, yp[y_stride_x_2], yp[1 + y_stride_x_2], yp[2 + y_stride_x_2], yp[3 + y_stride_x_2])
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride_x_3)), *up, *vp, yp[y_stride_x_3], yp[1 + y_stride_x_3], yp[2 + y_stride_x_3], yp[3 + y_stride_x_3])
                pdst += 4;
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

void PackedY422_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_y0, int32_t in_y1, int32_t in_u, int32_t in_v,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 2;

    if (!out_stride)
        out_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void PackedY422_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_y0, int32_t in_y1, int32_t in_u, int32_t in_v,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 2;

    if (!out_stride)
        out_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void PlanarYUV_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    // Copy the y plane

    if (out_stride != in_stride || flipped)
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

void PlanarYUV_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
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
// Y800 to YUV
//

void Y800_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_y0, int32_t out_y1, int32_t out_u, int32_t out_v,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
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

void Y800_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, int32_t out_decimation,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width;

    int32_t uv_width = width / out_decimation;
    int32_t uv_height = height / out_decimation;

    int32_t y_stride, uv_stride;
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

    uint8_t* uplane = out_buf + (y_stride * height);
    uint8_t* vplane = uplane + (uv_stride * uv_height);

    if (flipped)
    {
        out_buf += (y_stride * (height - 1));
        y_stride = -y_stride;
    }

    if (uv_stride == uv_width)
    {
        memset(uplane, 0, uv_width * uv_height);
        memset(vplane, 0, uv_width * uv_height);
    }
    else
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            memset(uplane, 0, uv_width);
            memset(vplane, 0, uv_width);
            uplane += uv_stride;
            vplane += uv_stride;
        }
    }

    if (y_stride == in_stride)
    {
        memcpy(out_buf, in_buf, y_stride * height);
    }
    else
    {
        for (int32_t y = 0; y < height; y++)
        {
            memcpy(out_buf, in_buf, width);
            out_buf += y_stride;
            in_buf += in_stride;
        }
    }
}

//
// Y16 to YUV
//

void Y16_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_y0, int32_t out_y1, int32_t out_u, int32_t out_v,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
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

void Y16_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, int32_t out_decimation,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 2;

    int32_t uv_width = width / out_decimation;
    int32_t uv_height = height / out_decimation;

    int32_t y_stride, uv_stride;
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

    uint8_t* uplane = out_buf + (y_stride * height);
    uint8_t* vplane = uplane + (uv_stride * uv_height);

    if (flipped)
    {
        out_buf += (y_stride * (height - 1));
        y_stride = -y_stride;
    }

    if (uv_stride == uv_width)
    {
        memset(uplane, 0, uv_width * uv_height);
        memset(vplane, 0, uv_width * uv_height);
    }
    else
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            memset(uplane, 0, uv_width);
            memset(vplane, 0, uv_width);
            uplane += uv_stride;
            vplane += uv_stride;
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
        out_buf += y_stride;
    }
}

//
// Y41P to YUV
//

void Y41P_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_y0, int32_t out_y1, int32_t out_u, int32_t out_v,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 12 / 8;

    if (!out_stride)
        out_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    int32_t width_m_8 = width - 8;
    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width_m_8; x += 8)
        {
            pdst[out_u] = pdst[out_u + 4] = psrc[0];
            pdst[out_v] = pdst[out_v + 4] = psrc[2];
            pdst[out_y0] = psrc[1];
            pdst[out_y1] = psrc[3];

            pdst[out_u + 4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) >> 1);
            pdst[out_v + 4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[6])) >> 1);
            pdst[out_y0 + 4] = psrc[5];
            pdst[out_y1 + 4] = psrc[7];

            pdst[out_u + 8] = psrc[4];
            pdst[out_v + 8] = psrc[6];
            pdst[out_y0 + 8] = psrc[8];
            pdst[out_y1 + 8] = psrc[9];

            pdst[out_u + 12] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + static_cast<uint16_t>(psrc[12])) >> 1);
            pdst[out_v + 12] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + static_cast<uint16_t>(psrc[14])) >> 1);
            pdst[out_y0 + 12] = psrc[10];
            pdst[out_y1 + 12] = psrc[11];

            psrc += 12;
            pdst += 16;
        }
        // Convert last set here to prevent reading past the line
        pdst[out_u] = pdst[out_u + 4] = psrc[0];
        pdst[out_v] = pdst[out_v + 4] = psrc[2];
        pdst[out_y0] = psrc[1];
        pdst[out_y1] = psrc[3];

        pdst[out_u + 4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) >> 1);
        pdst[out_v + 4] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[6])) >> 1);
        pdst[out_y0 + 4] = psrc[5];
        pdst[out_y1 + 4] = psrc[7];

        pdst[out_u + 8] = psrc[4];
        pdst[out_v + 8] = psrc[6];
        pdst[out_y0 + 8] = psrc[8];
        pdst[out_y1 + 8] = psrc[9];

        pdst[out_u + 12] = psrc[4];
        pdst[out_v + 12] = psrc[6];
        pdst[out_y0 + 12] = psrc[10];
        pdst[out_y1 + 12] = psrc[11];

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void Y41P_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_decimation, bool out_ufirst,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 12 / 8;

    int32_t uv_width = width / out_decimation;
    int32_t uv_height = height / out_decimation;

    int32_t y_stride, uv_stride;
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
    if (out_ufirst)
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

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_3 = y_stride * 3;
    int32_t y_stride_x_4 = y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;

            for (int32_t x = 0; x < uv_width; x += 4)
            {
                yp[0] = psrc[1];
                yp[y_stride] = psrc[1 + in_stride];
                yp[1] = psrc[3];
                yp[1 + y_stride] = psrc[3 + in_stride];

                up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride])) >> 1);

                vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride])) >> 1);

                yp[2] = psrc[5];
                yp[2 + y_stride] = psrc[5 + in_stride];
                yp[3] = psrc[7];
                yp[3 + y_stride] = psrc[7 + in_stride];

                up[x + 1] = up[x];
                vp[x + 1] = vp[x];

                yp[4] = psrc[8];
                yp[4 + y_stride] = psrc[8 + in_stride];
                yp[5] = psrc[9];
                yp[5 + y_stride] = psrc[9 + in_stride];

                up[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride])) >> 1);

                vp[x + 2] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride])) >> 1);

                yp[6] = psrc[10];
                yp[6 + y_stride] = psrc[10 + in_stride];
                yp[7] = psrc[11];
                yp[7 + y_stride] = psrc[11 + in_stride];

                up[x + 3] = up[x + 2];

                vp[x + 3] = vp[x + 2];

                psrc += 12;
                yp += 8;
            }

            in_buf += in_stride_x_2;
            out_buf += y_stride_x_2;

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;

            for (int32_t x = 0; x < uv_width; x += 2)
            {
                yp[0] = psrc[1];
                yp[y_stride] = psrc[1 + in_stride];
                yp[y_stride_x_2] = psrc[1 + in_stride_x_2];
                yp[y_stride_x_3] = psrc[1 + in_stride_x_3];

                yp[1] = psrc[3];
                yp[1 + y_stride] = psrc[3 + in_stride];
                yp[1 + y_stride_x_2] = psrc[3 + in_stride_x_2];
                yp[1 + y_stride_x_3] = psrc[3 + in_stride_x_3];

                yp[2] = psrc[5];
                yp[2 + y_stride] = psrc[5 + in_stride];
                yp[2 + y_stride_x_2] = psrc[5 + in_stride_x_2];
                yp[2 + y_stride_x_3] = psrc[5 + in_stride_x_3];

                yp[3] = psrc[7];
                yp[3 + y_stride] = psrc[7 + in_stride];
                yp[3 + y_stride_x_2] = psrc[7 + in_stride_x_2];
                yp[3 + y_stride_x_3] = psrc[7 + in_stride_x_3];

                up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + \
                    static_cast<uint16_t>(psrc[in_stride]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[in_stride_x_3])) >> 2);

                vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[2 + in_stride_x_3])) >> 2);

                yp[4] = psrc[8];
                yp[4 + y_stride] = psrc[8 + in_stride];
                yp[4 + y_stride_x_2] = psrc[8 + in_stride_x_2];
                yp[4 + y_stride_x_3] = psrc[8 + in_stride_x_3];

                yp[5] = psrc[9];
                yp[5 + y_stride] = psrc[9 + in_stride];
                yp[5 + y_stride_x_2] = psrc[9 + in_stride_x_2];
                yp[5 + y_stride_x_3] = psrc[9 + in_stride_x_3];

                yp[6] = psrc[10];
                yp[6 + y_stride] = psrc[10 + in_stride];
                yp[6 + y_stride_x_2] = psrc[10 + in_stride_x_2];
                yp[6 + y_stride_x_3] = psrc[10 + in_stride_x_3];

                yp[7] = psrc[11];
                yp[7 + y_stride] = psrc[11 + in_stride];
                yp[7 + y_stride_x_2] = psrc[11 + in_stride_x_2];
                yp[7 + y_stride_x_3] = psrc[11 + in_stride_x_3];

                up[x + 1] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[4 + in_stride_x_3])) >> 2);

                vp[x + 1] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride_x_2]) + \
                    static_cast<uint16_t>(psrc[6 + in_stride_x_3])) >> 2);

                psrc += 12;
                yp += 8;
            }

            in_buf += in_stride_x_4;
            out_buf += y_stride_x_4;

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

//
// CLJR to YUV
//

void CLJR_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_y0, int32_t out_y1, int32_t out_u, int32_t out_v,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width;

    if (!out_stride)
        out_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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


void CLJR_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_decimation, bool out_ufirst,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width;

    int32_t uv_width = width / out_decimation;
    int32_t uv_height = height / out_decimation;

    int32_t y_stride, uv_stride;
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
    if (out_ufirst)
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

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t in_stride_x_3 = in_stride * 3;
    int32_t in_stride_x_4 = in_stride * 4;
    int32_t y_stride_x_2 = y_stride * 2;
    int32_t y_stride_x_3 = y_stride * 3;
    int32_t y_stride_x_4 = y_stride * 4;

    if (out_decimation == 2)
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;

            for (int32_t x = 0; x < uv_width; x += 2)
            {
                uint32_t mpixel_top = *(reinterpret_cast<uint32_t*>(psrc));
                uint32_t mpixel_bot = *(reinterpret_cast<uint32_t*>(psrc + in_stride));

                yp[0] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel_top));
                yp[y_stride] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel_bot));
                yp[1] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel_top));
                yp[1 + y_stride] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel_bot));

                up[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_U(mpixel_top)) + \
                    static_cast<uint16_t>(UnpackCLJR_U(mpixel_bot))) >> 1);

                vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_V(mpixel_top)) + \
                    static_cast<uint16_t>(UnpackCLJR_V(mpixel_bot))) >> 1);

                yp[2] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel_top));
                yp[2 + y_stride] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel_bot));
                yp[3] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel_top));
                yp[3 + y_stride] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel_bot));

                up[x + 1] = up[x];
                vp[x + 1] = vp[x];

                psrc += 4;
                yp += 4;
            }

            in_buf += in_stride_x_2;
            out_buf += y_stride_x_2;

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (out_decimation == 4)
    {
        for (int32_t y = 0; y < uv_height; y++)
        {
            uint8_t* psrc = in_buf;
            uint8_t* yp = out_buf;
            uint8_t* up = uplane;
            uint8_t* vp = vplane;

            for (int32_t x = 0; x < uv_width; x++)
            {
                uint32_t mpixel0 = *(reinterpret_cast<uint32_t*>(psrc));
                uint32_t mpixel1 = *(reinterpret_cast<uint32_t*>(psrc + in_stride));
                uint32_t mpixel2 = *(reinterpret_cast<uint32_t*>(psrc + in_stride_x_2));
                uint32_t mpixel3 = *(reinterpret_cast<uint32_t*>(psrc + in_stride_x_3));

                yp[0] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel0));
                yp[y_stride] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel1));
                yp[y_stride_x_2] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel2));
                yp[y_stride_x_3] = static_cast<uint8_t>(UnpackCLJR_Y0(mpixel3));

                yp[1] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel0));
                yp[1 + y_stride] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel1));
                yp[1 + y_stride_x_2] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel2));
                yp[1 + y_stride_x_3] = static_cast<uint8_t>(UnpackCLJR_Y1(mpixel3));

                yp[2] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel0));
                yp[2 + y_stride] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel1));
                yp[2 + y_stride_x_2] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel2));
                yp[2 + y_stride_x_3] = static_cast<uint8_t>(UnpackCLJR_Y2(mpixel3));

                yp[3] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel0));
                yp[3 + y_stride] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel1));
                yp[3 + y_stride_x_2] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel2));
                yp[3 + y_stride_x_3] = static_cast<uint8_t>(UnpackCLJR_Y3(mpixel3));

                up[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_U(mpixel0)) + \
                    static_cast<uint16_t>(UnpackCLJR_U(mpixel1)) + \
                    static_cast<uint16_t>(UnpackCLJR_U(mpixel2)) + \
                    static_cast<uint16_t>(UnpackCLJR_U(mpixel3))) >> 2);

                vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(UnpackCLJR_V(mpixel0)) + \
                    static_cast<uint16_t>(UnpackCLJR_V(mpixel1)) + \
                    static_cast<uint16_t>(UnpackCLJR_V(mpixel2)) + \
                    static_cast<uint16_t>(UnpackCLJR_V(mpixel3))) >> 2);
                psrc += 4;
                yp += 4;
            }

            in_buf += in_stride_x_4;
            out_buf += y_stride_x_4;

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}

void AYUV_to_PackedY422(int32_t width, int32_t height,
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
            pdst[V] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) / 2);
            pdst[U] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[1]) + static_cast<uint16_t>(psrc[5])) / 2);
            pdst[Y0] = psrc[2];
            pdst[Y1] = psrc[6];
            psrc += 8;
            pdst += 4;
            hcount -= 2;
        }

        in_buf += in_stride;
        out_buf += out_stride;
        height--;
    }
}

void PackedY422_to_AYUV(int32_t width, int32_t height,
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
            pdst[0] = psrc[V];
            pdst[1] = psrc[U];
            pdst[2] = psrc[Y0];
            pdst[3] = 0xFF;

            pdst[4] = psrc[V];
            pdst[5] = psrc[U];
            pdst[6] = psrc[Y1];
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

void AYUV_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool ufirst, int32_t decimation, bool flipped)
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
    if (ufirst)
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

    uint16_t v_average;
    uint16_t u_average;

    if (decimation == 2)
    {
        for (int32_t y = 0; y < height; y += 2)
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
                yp[y_stride] = psrc[2 + in_stride];

                v_average += psrc[4 + in_stride];
                u_average += psrc[5 + in_stride];
                yp[y_stride + 1] = psrc[6 + in_stride];

                v_average >>= 2;
                u_average >>= 2;

                vplane[x >> 1] = static_cast<uint8_t>(v_average);
                uplane[x >> 1] = static_cast<uint8_t>(u_average);

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
                    ys += y_stride;
                }

                v_average >>= 4;
                u_average >>= 4;

                vplane[x >> 2] = static_cast<uint8_t>(v_average);
                uplane[x >> 2] = static_cast<uint8_t>(u_average);

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

void PlanarYUV_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool ufirst, int32_t decimation, bool flipped)
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
    if (ufirst)
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
                // column 1 row 1
                pdst[0] = *vp;
                pdst[1] = *up;
                pdst[2] = yp[0];
                pdst[3] = 0xFF;

                // column 1 row 2
                pdst[out_stride] = *vp;
                pdst[1 + out_stride] = *up;
                pdst[2 + out_stride] = yp[y_stride];
                pdst[3 + out_stride] = 0xFF;

                // column 2 row 1
                pdst[4] = *vp;
                pdst[5] = up[0];
                pdst[6] = yp[1];
                pdst[7] = 0xFF;

                // column 2 row 2
                pdst[4 + out_stride] = *vp;
                pdst[5 + out_stride] = *up;
                pdst[6 + out_stride] = yp[1 + y_stride];
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
                for (int16_t row = 0; row < 4; row++)
                {
                    for (int16_t col = 0; col < 4; col++)
                    {
                        pdst[col * 4 + out_stride * row] = *vp;
                        pdst[1 + col * 4 + out_stride * row] = *up;
                        pdst[2 + col * 4 + out_stride * row] = yp[col + y_stride * row];
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

void AYUV_to_IMCx(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool ufirst, bool interlaced, bool flipped)
{
    if (!in_stride)
        in_stride = width * 4;

    int32_t uv_width = width / 2;

    if (!out_stride)
        out_stride = width;

    uint8_t* vplane;
    uint8_t* uplane;
    if (ufirst)
    {
        if (interlaced)
        {
            uplane = out_buf + (out_stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            uplane = out_buf + (((height + 15) & ~15) * out_stride);
            vplane = out_buf + (((((height * 3) / 2) + 15) & ~15) * out_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            vplane = out_buf + (out_stride * height);
            uplane = vplane + uv_width;
        }
        else
        {
            vplane = out_buf + (((height + 15) & ~15) * out_stride);
            uplane = out_buf + (((((height * 3) / 2) + 15) & ~15) * out_stride);
        }
    }

    uint16_t U;
    uint16_t V;

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < height; y += 2)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        for (int32_t x = 0; x < width; x += 2)
        {
            yp[0] = psrc[2];
            U = psrc[1];
            V = psrc[0];

            yp[1] = psrc[6];
            U += psrc[5];
            V += psrc[4];

            yp[out_stride] = psrc[2 + in_stride];
            U += psrc[1 + in_stride];
            V += psrc[0 + in_stride];

            yp[out_stride + 1] = psrc[6 + in_stride];
            U += psrc[5 + in_stride];
            V += psrc[4 + in_stride];

            uplane[x >> 1] = U >> 2;
            vplane[x >> 1] = V >> 2;

            psrc += 8;
            yp += 2;
        }
        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        uplane += out_stride;
        vplane += out_stride;
    }
}

void IMCx_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool ufirst, bool interlaced, bool flipped)
{
    if (!out_stride)
        out_stride = width * 4;

    int16_t uv_width = width / 2;

    if (in_stride < width)
        in_stride = width;

    uint8_t* vplane;
    uint8_t* uplane;
    if (ufirst)
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
        uplane += in_stride;
        vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}

void PackedY422_to_IMCx(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, bool ufirst, bool interlaced,
    uint8_t* in_buf, int32_t in_stride,
    int32_t in_y0, int32_t in_y1, int32_t in_u, int32_t in_v,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 2;

    int32_t uv_width = width / 2;
    int32_t uv_height = height / 2;

    if (!out_stride)
        out_stride = width;

    uint8_t* vplane;
    uint8_t* uplane;
    if (ufirst)
    {
        if (interlaced)
        {
            uplane = out_buf + (out_stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            uplane = out_buf + (((height + 15) & ~15) * out_stride);
            vplane = out_buf + (((((height * 3) / 2) + 15) & ~15) * out_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            vplane = out_buf + (out_stride * height);
            uplane = vplane + uv_width;
        }
        else
        {
            vplane = out_buf + (((height + 15) & ~15) * out_stride);
            uplane = out_buf + (((((height * 3) / 2) + 15) & ~15) * out_stride);
        }
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        uplane += (out_stride * (uv_height - 1));
        vplane += (out_stride * (uv_height - 1));
        out_stride = -out_stride;
    }

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = uplane;
        uint8_t* vp = vplane;

        for (int32_t x = 0; x < uv_width; x++)
        {
            yp[0] = psrc[in_y0];
            yp[out_stride] = psrc[in_y0 + in_stride];
            yp[1] = psrc[in_y1];
            yp[1 + out_stride] = psrc[in_y1 + in_stride];

            up[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_u]) + \
                static_cast<uint16_t>(psrc[in_u + in_stride])) >> 1);

            vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[in_v]) + \
                static_cast<uint16_t>(psrc[in_v + in_stride])) >> 1);
            psrc += 4;
            yp += 2;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        uplane += out_stride;
        vplane += out_stride;
    }
}

void IMCx_to_PackedY422(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t out_y0, int32_t out_y1, int32_t out_u, int32_t out_v,
    uint8_t* in_buf, int32_t in_stride, bool ufirst, bool interlaced,
    bool flipped)
{
    if (!in_stride)
        in_stride = width;

    if (!out_stride)
        out_stride = width * 2;

    int32_t uv_width = width / 2;
    int32_t uv_height = height / 2;

    uint8_t* vplane;
    uint8_t* uplane;
    if (ufirst)
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

    for (int32_t y = 0; y < height; y += 2)
    {
        uint8_t* yp = in_buf;
        uint8_t* up = uplane;
        uint8_t* vp = vplane;
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
        uplane += in_stride;
        vplane += in_stride;
        out_buf += out_stride_x_2;
    }
}


void PlanarYUV_to_IMCx(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, bool out_ufirst, bool interlaced,
    uint8_t* in_buf, int32_t in_stride, int32_t in_decimation, bool in_ufirst,
    bool flipped)
{
    int32_t out_uv_width = width / 2;
    int32_t out_uv_height = height / 2;

    if (!out_stride)
        out_stride = width;

    uint8_t* out_vplane;
    uint8_t* out_uplane;
    if (out_ufirst)
    {
        if (interlaced)
        {
            out_uplane = out_buf + (out_stride * height);
            out_vplane = out_uplane + out_uv_width;
        }
        else
        {
            out_uplane = out_buf + (((height + 15) & ~15) * out_stride);
            out_vplane = out_buf + (((((height * 3) / 2) + 15) & ~15) * out_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            out_vplane = out_buf + (out_stride * height);
            out_uplane = out_vplane + out_uv_width;
        }
        else
        {
            out_vplane = out_buf + (((height + 15) & ~15) * out_stride);
            out_uplane = out_buf + (((((height * 3) / 2) + 15) & ~15) * out_stride);
        }
    }

    int32_t in_uv_width = width / in_decimation;
    int32_t in_uv_height = height / in_decimation;

    int32_t in_y_stride, in_uv_stride;
    if (!in_stride)
    {
        in_y_stride = width;
        in_uv_stride = in_uv_width;
    }
    else
    {
        in_y_stride = in_stride;
        in_uv_stride = in_stride;
    }

    uint8_t* in_vplane;
    uint8_t* in_uplane;
    if (in_ufirst)
    {
        in_uplane = in_buf + (in_y_stride * height);
        in_vplane = in_uplane + (in_uv_stride * in_uv_height);
    }
    else
    {
        in_vplane = in_buf + (in_y_stride * height);
        in_uplane = in_vplane + (in_uv_stride * in_uv_height);
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_uplane += (out_stride * (out_uv_height - 1));
        out_vplane += (out_stride * (out_uv_height - 1));
        out_stride = -out_stride;
    }

    // Copy the y plane
    if (out_stride != in_y_stride || flipped)
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
        if (out_stride != in_uv_stride || flipped || interlaced)
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

void IMCx_to_PlanarYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, int32_t out_decimation, bool out_ufirst,
    uint8_t* in_buf, int32_t in_stride, bool in_ufirst, bool interlaced,
    bool flipped)
{
    int32_t out_uv_width = width / out_decimation;
    int32_t out_uv_height = height / out_decimation;

    int32_t out_y_stride, out_uv_stride;
    if (!out_stride)
    {
        out_y_stride = width;
        out_uv_stride = out_uv_width;
    }
    else
    {
        out_y_stride = out_stride;
        out_uv_stride = out_stride;
    }

    uint8_t* out_vplane;
    uint8_t* out_uplane;
    if (out_ufirst)
    {
        out_uplane = out_buf + (out_y_stride * height);
        out_vplane = out_uplane + (out_uv_stride * out_uv_height);
    }
    else
    {
        out_vplane = out_buf + (out_y_stride * height);
        out_uplane = out_vplane + (out_uv_stride * out_uv_height);
    }
    int32_t in_uv_width = width / 2;
    int32_t in_uv_height = height / 2;

    if (!in_stride)
        in_stride = width;

    uint8_t* in_vplane;
    uint8_t* in_uplane;
    if (in_ufirst)
    {
        if (interlaced)
        {
            in_uplane = in_buf + (in_stride * height);
            in_vplane = in_uplane + in_uv_width;
        }
        else
        {
            in_uplane = in_buf + (((height + 15) & ~15) * in_stride);
            in_vplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            in_vplane = in_buf + (in_stride * height);
            in_uplane = in_vplane + in_uv_width;
        }
        else
        {
            in_vplane = in_buf + (((height + 15) & ~15) * in_stride);
            in_uplane = in_buf + (((((height * 3) / 2) + 15) & ~15) * in_stride);
        }
    }

    if (flipped)
    {
        out_buf += (out_y_stride * (height - 1));
        out_uplane += (out_uv_stride * (out_uv_height - 1));
        out_vplane += (out_uv_stride * (out_uv_height - 1));
        out_y_stride = -out_y_stride;
        out_uv_stride = -out_uv_stride;
    }

    // Copy the y plane
    if (out_y_stride != in_stride || flipped)
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
        if (out_uv_stride != in_stride || flipped || interlaced)
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
                    static_cast<uint16_t>(in_u_line[x + in_stride]) + \
                    static_cast<uint16_t>(in_u_line[x + in_stride + 1])) >> 2);
                out_v_line[out_index] = static_cast<uint8_t>((static_cast<uint16_t>(in_v_line[x]) + \
                    static_cast<uint16_t>(in_v_line[x + 1]) + \
                    static_cast<uint16_t>(in_v_line[x + in_stride]) + \
                    static_cast<uint16_t>(in_v_line[x + in_stride + 1])) >> 2);
                out_index++;
            }

            in_u_line += in_uv_stride_x_2;
            in_v_line += in_uv_stride_x_2;
            out_u_line += out_uv_stride;
            out_v_line += out_uv_stride;
        }
    }
}

void IYU1_to_IMCx(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride, bool ufirst, bool interlaced,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped)
{
    if (!in_stride)
        in_stride = width * 12 / 8;


    int32_t uv_width = width / 2;
    int32_t uv_height = height / 2;

    if (!out_stride)
        out_stride = width;

    uint8_t* vplane;
    uint8_t* uplane;
    if (ufirst)
    {
        if (interlaced)
        {
            uplane = out_buf + (out_stride * height);
            vplane = uplane + uv_width;
        }
        else
        {
            uplane = out_buf + (((height + 15) & ~15) * out_stride);
            vplane = out_buf + (((((height * 3) / 2) + 15) & ~15) * out_stride);
        }
    }
    else
    {
        if (interlaced)
        {
            vplane = out_buf + (out_stride * height);
            uplane = vplane + uv_width;
        }
        else
        {
            vplane = out_buf + (((height + 15) & ~15) * out_stride);
            uplane = out_buf + (((((height * 3) / 2) + 15) & ~15) * out_stride);
        }
    }

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        uplane += (out_stride * (uv_height - 1));
        vplane += (out_stride * (uv_height - 1));
        out_stride = -out_stride;
    }

    int32_t in_stride_x_2 = in_stride * 2;
    int32_t out_stride_x_2 = out_stride * 2;

    for (int32_t y = 0; y < uv_height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* yp = out_buf;
        uint8_t* up = uplane;
        uint8_t* vp = vplane;

        for (int32_t x = 0; x < uv_width; x += 2)
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

            up[x + 1] = up[x];
            vp[x + 1] = vp[x];

            psrc += 6;
            yp += 4;
        }

        in_buf += in_stride_x_2;
        out_buf += out_stride_x_2;

        uplane += out_stride;
        vplane += out_stride;
    }
}

//
// Public Yuv to Yuv transform functions
//

void blipvert::AYUV_to_YUY2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

void blipvert::AYUV_to_UYVY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

void blipvert::AYUV_to_YVYU(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

void blipvert::AYUV_to_VYUY(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_PackedY422(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

void blipvert::AYUV_to_IYUV(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, true);
}

void blipvert::AYUV_to_YV12(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

void blipvert::AYUV_to_YVU9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

void blipvert::AYUV_to_YUV9(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_PlanarYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

void blipvert::AYUV_to_IYU1(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

void blipvert::AYUV_to_IYU2(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

void blipvert::AYUV_to_Y41P(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

void blipvert::AYUV_to_CLJR(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

void blipvert::AYUV_to_Y800(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

void blipvert::AYUV_to_Y16(int32_t width, int32_t height, uint8_t* out_buf, int32_t out_stride, uint8_t* in_buf, int32_t in_stride, bool flipped, xRGBQUAD* in_palette)
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

void blipvert::AYUV_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_IMCx(width, height, out_buf, out_stride, in_buf, in_stride, false, false, true);
}

void blipvert::AYUV_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_IMCx(width, height, out_buf, out_stride, in_buf, in_stride, false, true, true);
}

void blipvert::AYUV_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_IMCx(width, height, out_buf, out_stride, in_buf, in_stride, true, false, true);
}

void blipvert::AYUV_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    AYUV_to_IMCx(width, height, out_buf, out_stride, in_buf, in_stride, true, true, true);
}

//
// YUY2 to YUV
//

void blipvert::YUY2_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,                        // UYVY
        in_buf, in_stride,
        0, 2, 1, 3,                        // YUY2
        flipped);
}

void blipvert::YUY2_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,                        // YVYU
        in_buf, in_stride,
        0, 2, 1, 3,                        // YUY2
        flipped);
}

void blipvert::YUY2_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,                        // VYUY
        in_buf, in_stride,
        0, 2, 1, 3,                        // YUY2
        flipped);
}

void blipvert::YUY2_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,    // IYUV
        in_buf, in_stride,
        0, 2, 1, 3,                        // YUY2
        flipped);
}

void blipvert::YUY2_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,    // YV12
        in_buf, in_stride,
        0, 2, 1, 3,                        // YUY2
        flipped);
}

void blipvert::YUY2_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,    // YVU9
        in_buf, in_stride,
        0, 2, 1, 3,                       // YUY2
        flipped);
}

void blipvert::YUY2_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,    // YUV9
        in_buf, in_stride,
        0, 2, 1, 3,                      // YUY2
        flipped);
}

void blipvert::YUY2_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 1, 3,
        flipped);
}

void blipvert::YUY2_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 1, 3,
        flipped);
}

void blipvert::YUY2_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 1, 3,
        flipped);
}

void blipvert::YUY2_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y16(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 1, 3,
        flipped);
}

void blipvert::YUY2_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 1, 3,
        flipped);
}

void blipvert::YUY2_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 1, 3,
        flipped);
}

void blipvert::YUY2_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 1, 3, true);
}

void blipvert::YUY2_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, false, false,  // IMC1
        in_buf, in_stride, 0, 2, 1, 3,      // YUY2
        flipped);
}

void blipvert::YUY2_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, false, true,   // IMC2
        in_buf, in_stride, 0, 2, 1, 3,      // YUY2
        flipped);
}

void blipvert::YUY2_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, true, false,   // IMC3
        in_buf, in_stride, 0, 2, 1, 3,      // YUY2
        flipped);
}

void blipvert::YUY2_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, true, true,    // IMC4
        in_buf, in_stride, 0, 2, 1, 3,      // YUY2
        flipped);
}

//
// UYVY to YUV
//

void blipvert::UYVY_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,                        // YUY2
        in_buf, in_stride,
        1, 3, 0, 2,                        // UYVY
        flipped);
}

void blipvert::UYVY_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,                        // YVYU
        in_buf, in_stride,
        1, 3, 0, 2,                        // UYVY
        flipped);
}

void blipvert::UYVY_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,                        // VYUY
        in_buf, in_stride,
        1, 3, 0, 2,                        // UYVY
        flipped);
}

void blipvert::UYVY_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,    // IYUV
        in_buf, in_stride,
        1, 3, 0, 2,                        // UYVY
        flipped);
}

void blipvert::UYVY_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,    // YV12
        in_buf, in_stride,
        1, 3, 0, 2,                        // UYVY
        flipped);
}

void blipvert::UYVY_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,    // YVU9
        in_buf, in_stride,
        1, 3, 0, 2,                        // UYVY
        flipped);
}

void blipvert::UYVY_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,    // YUV9
        in_buf, in_stride,
        1, 3, 0, 2,                        // UYVY
        flipped);
}

void blipvert::UYVY_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 0, 2,
        flipped);
}

void blipvert::UYVY_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 0, 2,
        flipped);
}

void blipvert::UYVY_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 0, 2,
        flipped);
}

void blipvert::UYVY_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y16(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 0, 2,
        flipped);
}

void blipvert::UYVY_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 0, 2,
        flipped);
}

void blipvert::UYVY_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 0, 2,
        flipped);
}

void blipvert::UYVY_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 0, 2, true);
}

void blipvert::UYVY_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, false, false,  // UYVY
        in_buf, in_stride, 1, 3, 0, 2,      // YUY2
        flipped);
}

void blipvert::UYVY_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, false, true,   // UYVY
        in_buf, in_stride, 1, 3, 0, 2,      // YUY2
        flipped);
}

void blipvert::UYVY_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, true, false,   // UYVY
        in_buf, in_stride, 1, 3, 0, 2,      // YUY2
        flipped);
}

void blipvert::UYVY_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, true, true,    // UYVY
        in_buf, in_stride, 1, 3, 0, 2,      // YUY2
        flipped);
}

//
// YVYU to YUV
//

void blipvert::YVYU_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,                        // YUY2
        in_buf, in_stride,
        0, 2, 3, 1,                        // YVYU
        flipped);
}

void blipvert::YVYU_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,                        // UYVY
        in_buf, in_stride,
        0, 2, 3, 1,                        // YVYU
        flipped);
}

void blipvert::YVYU_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,                        // VYUY
        in_buf, in_stride,
        0, 2, 3, 1,                        // YVYU
        flipped);
}

void blipvert::YVYU_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,    // IYUV
        in_buf, in_stride,
        0, 2, 3, 1,                        // YVYU
        flipped);
}

void blipvert::YVYU_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,    // YV12
        in_buf, in_stride,
        0, 2, 3, 1,                        // YVYU
        flipped);
}

void blipvert::YVYU_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,    // YVU9
        in_buf, in_stride,
        0, 2, 3, 1,                        // YVYU
        flipped);
}

void blipvert::YVYU_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,    // YUV9
        in_buf, in_stride,
        0, 2, 3, 1,                        // YVYU
        flipped);
}

void blipvert::YVYU_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 3, 1,
        flipped);
}

void blipvert::YVYU_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 3, 1,
        flipped);
}

void blipvert::YVYU_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 3, 1,
        flipped);
}

void blipvert::YVYU_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y16(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 3, 1,
        flipped);
}

void blipvert::YVYU_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 3, 1,
        flipped);
}

void blipvert::YVYU_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 3, 1,
        flipped);
}

void blipvert::YVYU_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, 0, 2, 3, 1, true);
}

void blipvert::YVYU_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, false, false,  // IMC1
        in_buf, in_stride, 0, 2, 3, 1,      // YVYU
        flipped);
}

void blipvert::YVYU_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, false, true,   // IMC2
        in_buf, in_stride, 0, 2, 3, 1,      // YVYU
        flipped);
}

void blipvert::YVYU_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, true, false,   // IMC3
        in_buf, in_stride, 0, 2, 3, 1,      // YVYU
        flipped);
}

void blipvert::YVYU_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, true, true,    // IMC4
        in_buf, in_stride, 0, 2, 3, 1,      // YVYU
        flipped);
}

//
// VYUY to YUV
//

void blipvert::VYUY_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,                        // YUY2
        in_buf, in_stride,
        1, 3, 2, 0,                        // VYUY
        flipped);
}

void blipvert::VYUY_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,                        // UYVY
        in_buf, in_stride,
        1, 3, 2, 0,                        // VYUY
        flipped);
}

void blipvert::VYUY_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,                        // YVYU
        in_buf, in_stride,
        1, 3, 2, 0,                        // VYUY
        flipped);
}

void blipvert::VYUY_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,    // IYUV
        in_buf, in_stride,
        1, 3, 2, 0,                        // VYUY
        flipped);
}

void blipvert::VYUY_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,    // YV12
        in_buf, in_stride,
        1, 3, 2, 0,                        // VYUY
        flipped);
}

void blipvert::VYUY_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,    // YVU9
        in_buf, in_stride,
        1, 3, 2, 0,                        // VYUY
        flipped);
}

void blipvert::VYUY_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,    // YUV9
        in_buf, in_stride,
        1, 3, 2, 0,                        // VYUY
        flipped);
}

void blipvert::VYUY_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 2, 0,
        flipped);
}

void blipvert::VYUY_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 2, 0,
        flipped);
}

void blipvert::VYUY_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 2, 0,
        flipped);
}

void blipvert::VYUY_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y16(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 2, 0,
        flipped);
}

void blipvert::VYUY_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 2, 0,
        flipped);
}

void blipvert::VYUY_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 2, 0,
        flipped);
}

void blipvert::VYUY_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, 1, 3, 2, 0, true);
}

void blipvert::VYUY_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, false, false,  // IMC1
        in_buf, in_stride, 1, 3, 2, 0,      // YUY2
        flipped);
}

void blipvert::VYUY_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, false, true,   // IMC2
        in_buf, in_stride, 1, 3, 2, 0,      // YUY2
        flipped);
}

void blipvert::VYUY_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, true, false,   // IMC3
        in_buf, in_stride, 1, 3, 2, 0,      // VYUY
        flipped);
}

void blipvert::VYUY_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PackedY422_to_IMCx(width, height,
        out_buf, out_stride, true, true,    // IMC4
        in_buf, in_stride, 1, 3, 2, 0,      // VYUY
        flipped);
}

//
// IYUV to YUV
//

void blipvert::IYUV_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,        // YVU9
        in_buf, in_stride, 2, true,           // IYUV
        flipped);
}

void blipvert::IYUV_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,        // YUV9
        in_buf, in_stride, 2, true,          // IYUV
        flipped);
}

void blipvert::IYUV_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,        // YV12
        in_buf, in_stride, 2, true,           // IYUV
        flipped);
}

void blipvert::IYUV_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,                        // YUY2
        in_buf, in_stride, 2, true,        // IYUV
        flipped);
}

void blipvert::IYUV_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,                        // UYVY
        in_buf, in_stride, 2, true,        // IYUV
        flipped);
}

void blipvert::IYUV_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,                        // YVYU
        in_buf, in_stride, 2, true,        // IYUV
        flipped);
}

void blipvert::IYUV_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,                        // VYUY
        in_buf, in_stride, 2, true,        // IYUV
        flipped);
}

void blipvert::IYUV_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IYU1(width, height, out_buf, out_stride, in_buf, in_stride, 2, true, flipped);
}

void blipvert::IYUV_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IYU2(width, height, out_buf, out_stride, in_buf, in_stride, 2, true, flipped);
}

void blipvert::IYUV_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y800(width, height, out_buf, out_stride, in_buf, in_stride, flipped);
}

void blipvert::IYUV_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y16(width, height, out_buf, out_stride, in_buf, in_stride, flipped);
}

void blipvert::IYUV_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_CLJR(width, height, out_buf, out_stride, in_buf, in_stride, 2, true, flipped);
}

void blipvert::IYUV_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y41P(width, height, out_buf, out_stride, in_buf, in_stride, 2, true, flipped);
}

void blipvert::IYUV_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 2, flipped);
}

void blipvert::IYUV_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, false, false, in_buf, in_stride, 2, true, flipped);
}

void blipvert::IYUV_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, false, true, in_buf, in_stride, 2, true, flipped);
}

void blipvert::IYUV_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, true, false, in_buf, in_stride, 2, true, flipped);
}

void blipvert::IYUV_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, true, true, in_buf, in_stride, 2, true, flipped);
}


//
// YV12 to YUV
//

void blipvert::YV12_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,        // YVU9
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,        // YUV9
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,        // IYUV
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,                            // YUY2
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,                            // UYVY
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,                            // YVYU
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,                            // VYUY
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, false,
        flipped);
}

void blipvert::YV12_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, false,
        flipped);
}

void blipvert::YV12_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::YV12_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y16(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::YV12_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, false,
        flipped);
}

void blipvert::YV12_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, false,
        flipped);
}

void blipvert::YV12_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 2, true);
}

void blipvert::YV12_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, false, false, in_buf, in_stride, 2, false, flipped);
}

void blipvert::YV12_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, false, true, in_buf, in_stride, 2, false, flipped);
}

void blipvert::YV12_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, true, false, in_buf, in_stride, 2, false, flipped);
}

void blipvert::YV12_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, true, true, in_buf, in_stride, 2, false, flipped);
}

//
// YVU9 to YUV
//

void blipvert::YVU9_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,        // IYUV
        in_buf, in_stride, 4, false,        // YVU9
        flipped);
}

void blipvert::YVU9_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,        // YV12
        in_buf, in_stride, 4, false,        // YVU9
        flipped);
}

void blipvert::YVU9_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,        // YUV9
        in_buf, in_stride, 4, false,        // YVU9
        flipped);
}

void blipvert::YVU9_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,                            // YUY2
        in_buf, in_stride, 4, false,        // YVU9
        flipped);
}

void blipvert::YVU9_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,                     // UYVY
        in_buf, in_stride, 4, false,    // YVU9
        flipped);
}

void blipvert::YVU9_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,                            // YVYU
        in_buf, in_stride, 4, false,        // YVU9
        flipped);
}

void blipvert::YVU9_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,                            // VYUY
        in_buf, in_stride, 4, false,        // YVU9
        flipped);
}

void blipvert::YVU9_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, false,
        flipped);
}

void blipvert::YVU9_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, false,
        flipped);
}

void blipvert::YVU9_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::YVU9_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y16(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::YVU9_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, false,
        flipped);
}

void blipvert::YVU9_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, false,
        flipped);
}

void blipvert::YVU9_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, 4, true);
}

void blipvert::YVU9_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, false, false, in_buf, in_stride, 4, false, flipped);
}

void blipvert::YVU9_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, false, true, in_buf, in_stride, 4, false, flipped);
}

void blipvert::YVU9_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, true, false, in_buf, in_stride, 4, false, flipped);
}

void blipvert::YVU9_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, true, true, in_buf, in_stride, 4, false, flipped);
}

//
// YUV9 to YUV
//

void blipvert::YUV9_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,        // IYUV
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,        // YV12
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,        // YVU9
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,                            // YUY2
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,                            // UYVY
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,                            // YVYU
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,                            // VYUY
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, true,
        flipped);
}

void blipvert::YUV9_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, true,
        flipped);
}

void blipvert::YUV9_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::YUV9_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y16(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::YUV9_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, true,
        flipped);
}

void blipvert::YUV9_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, true,
        flipped);
}

void blipvert::YUV9_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, 4, true);
}

void blipvert::YUV9_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, false, false, in_buf, in_stride, 4, true, flipped);
}

void blipvert::YUV9_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, false, true, in_buf, in_stride, 4, true, flipped);
}

void blipvert::YUV9_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, true, false, in_buf, in_stride, 4, true, flipped);
}

void blipvert::YUV9_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    PlanarYUV_to_IMCx(width, height, out_buf, out_stride, true, true, in_buf, in_stride, 4, true, flipped);
}

//
// IYU1 to YUV
//

void blipvert::IYU1_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU1_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU1_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU1_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU1_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU1_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU1_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU1_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU1_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width * 12 / 8;

    if (!out_stride)
        out_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::IYU1_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width * 12 / 8;

    if (!out_stride)
        out_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::IYU1_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width * 12 / 8;

    if (!out_stride)
        out_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::IYU1_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 12 / 8;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::IYU1_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width * 12 / 8;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::IYU1_to_AYUV(int32_t width, int32_t height,
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

void  blipvert::IYU1_to_IMC1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palett)
{
    IYU1_to_IMCx(width, height, out_buf, out_stride, false, false, in_buf, in_stride, flipped);
}

void  blipvert::IYU1_to_IMC2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palett)
{
    IYU1_to_IMCx(width, height, out_buf, out_stride, false, true, in_buf, in_stride, flipped);
}

void  blipvert::IYU1_to_IMC3(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palett)
{
    IYU1_to_IMCx(width, height, out_buf, out_stride, true, false, in_buf, in_stride, flipped);
}

void  blipvert::IYU1_to_IMC4(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palett)
{
    IYU1_to_IMCx(width, height, out_buf, out_stride, true, true, in_buf, in_stride, flipped);
}


//
// IYU2 to YUV
//

void blipvert::IYU2_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU2_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU2_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU2_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU2_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU2_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU2_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU2_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IYU2_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width * 3;

    if (!out_stride)
        out_stride = width * 12 / 8;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::IYU2_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width * 3;

    if (!out_stride)
        out_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::IYU2_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width * 3;

    if (!out_stride)
        out_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::IYU2_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
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

void blipvert::IYU2_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width * 3;

    if (!out_stride)
        out_stride = width * 12 / 8;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::IYU2_to_AYUV(int32_t width, int32_t height,
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

void blipvert::Y800_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y800_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y800_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y800_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y800_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y800_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y800_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y800_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y800_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y800_to_PlanarYUV(width, height,
        out_buf, out_stride, 2,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y800_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y800_to_PlanarYUV(width, height,
        out_buf, out_stride, 2,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y800_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y800_to_PlanarYUV(width, height,
        out_buf, out_stride, 4,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y800_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y800_to_PlanarYUV(width, height,
        out_buf, out_stride, 4,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y800_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::Y800_to_IYU2(int32_t width, int32_t height,
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

void blipvert::Y800_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::Y800_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::Y800_to_AYUV(int32_t width, int32_t height,
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

void blipvert::Y800_to_Y16(int32_t width, int32_t height,
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

//
// Y16 to YUV
//

void blipvert::Y16_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y16_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y16_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y16_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y16_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y16_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y16_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y16_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y16_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y16_to_PlanarYUV(width, height,
        out_buf, out_stride, 2,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y16_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y16_to_PlanarYUV(width, height,
        out_buf, out_stride, 2,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y16_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y16_to_PlanarYUV(width, height,
        out_buf, out_stride, 4,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y16_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y16_to_PlanarYUV(width, height,
        out_buf, out_stride, 4,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y16_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
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

void blipvert::Y16_to_IYU2(int32_t width, int32_t height,
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

void blipvert::Y16_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
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

void blipvert::Y16_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
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

void blipvert::Y16_to_AYUV(int32_t width, int32_t height,
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

void blipvert::Y16_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
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

//
// CLJR to YUV
//

void blipvert::CLJR_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    CLJR_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    CLJR_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    CLJR_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    CLJR_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    CLJR_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    CLJR_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    CLJR_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    CLJR_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::CLJR_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width;

    if (!out_stride)
        out_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::CLJR_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width;

    if (!out_stride)
        out_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::CLJR_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width;

    if (!out_stride)
        out_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::CLJR_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::CLJR_to_AYUV(int32_t width, int32_t height,
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

//
// Y41P to YUV
//

void blipvert::Y41P_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y41P_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 1, 3,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y41P_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 0, 2,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y41P_to_PackedY422(width, height,
        out_buf, out_stride,
        0, 2, 3, 1,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y41P_to_PackedY422(width, height,
        out_buf, out_stride,
        1, 3, 2, 0,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y41P_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y41P_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y41P_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    Y41P_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 12 / 8;

    if (!in_stride)
        in_stride = width * 12 / 8;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[0];        // U0
            pdst[3] = psrc[2];        // V0
            pdst[1] = psrc[1];        // Y0
            pdst[2] = psrc[3];        // Y1
            pdst[4] = psrc[5];        // Y2
            pdst[5] = psrc[7];        // Y3

            pdst[6] = psrc[4];        // U4
            pdst[9] = psrc[6];        // V4
            pdst[7] = psrc[8];        // Y4
            pdst[8] = psrc[9];        // Y5
            pdst[10] = psrc[10];    // Y6
            pdst[11] = psrc[11];    // Y7

            psrc += 12;
            pdst += 12;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!in_stride)
        in_stride = width * 12 / 8;

    if (!out_stride)
        out_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    int32_t width_m_8 = width - 8;

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width_m_8; x += 8)
        {
            pdst[0] = psrc[0];    // U
            pdst[1] = psrc[1];    // Y0
            pdst[2] = psrc[2];    // V

            pdst[3] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[0]) * 768) + (static_cast<uint32_t>(psrc[4]) * 256)) >> 10);
            pdst[4] = psrc[3];    // Y1
            pdst[5] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[2]) * 768) + (static_cast<uint32_t>(psrc[6]) * 256)) >> 10);

            pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) >> 1);
            pdst[7] = psrc[5];    // Y2
            pdst[8] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[6])) >> 1);

            pdst[9] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[0]) * 256) + (static_cast<uint32_t>(psrc[4]) * 768)) >> 10);
            pdst[10] = psrc[7];    // Y3
            pdst[11] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[2]) * 256) + (static_cast<uint32_t>(psrc[6]) * 768)) >> 10);

            pdst[12] = psrc[4];    // U
            pdst[13] = psrc[8];    // Y4
            pdst[14] = psrc[6];    // V

            pdst[15] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[4]) * 768) + (static_cast<uint32_t>(psrc[12]) * 256)) >> 10);
            pdst[16] = psrc[9];    // Y5
            pdst[17] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[6]) * 768) + (static_cast<uint32_t>(psrc[14]) * 256)) >> 10);

            pdst[18] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[4]) + static_cast<uint16_t>(psrc[12])) >> 1);
            pdst[19] = psrc[10];// Y6
            pdst[20] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[6]) + static_cast<uint16_t>(psrc[14])) >> 1);

            pdst[21] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[4]) * 256) + (static_cast<uint32_t>(psrc[12]) * 768)) >> 10);
            pdst[22] = psrc[11];// Y7
            pdst[23] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[6]) * 256) + (static_cast<uint32_t>(psrc[14]) * 768)) >> 10);

            psrc += 12;
            pdst += 24;
        }

        // Take care of the last block here to prevent
        // a read outside of the source line.
        pdst[0] = psrc[0];    // U
        pdst[1] = psrc[1];    // Y0
        pdst[2] = psrc[2];    // V

        pdst[3] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[0]) * 768) + (static_cast<uint32_t>(psrc[4]) * 256)) >> 10);
        pdst[4] = psrc[3];    // Y1
        pdst[5] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[2]) * 768) + (static_cast<uint32_t>(psrc[6]) * 256)) >> 10);

        pdst[6] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[0]) + static_cast<uint16_t>(psrc[4])) >> 1);
        pdst[7] = psrc[5];    // Y2
        pdst[8] = static_cast<uint8_t>((static_cast<uint16_t>(psrc[2]) + static_cast<uint16_t>(psrc[6])) >> 1);

        pdst[9] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[0]) * 256) + (static_cast<uint32_t>(psrc[4]) * 768)) >> 10);
        pdst[10] = psrc[7];    // Y3
        pdst[11] = static_cast<uint8_t>(((static_cast<uint32_t>(psrc[2]) * 256) + (static_cast<uint32_t>(psrc[6]) * 768)) >> 10);

        pdst[12] = psrc[4];     // U
        pdst[13] = psrc[8];     // Y4
        pdst[14] = psrc[6];     // V

        pdst[15] = psrc[4];     // U
        pdst[16] = psrc[9];     // Y5
        pdst[17] = psrc[6];     // V

        pdst[18] = psrc[4];     // U
        pdst[19] = psrc[10];    // Y6
        pdst[20] = psrc[6];     // V

        pdst[21] = psrc[4];     // U
        pdst[22] = psrc[11];    // Y7
        pdst[23] = psrc[6];     // V

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 12 / 8;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    for (int32_t y = 0; y < height; y++)
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        for (int32_t x = 0; x < width; x += 8)
        {
            pdst[0] = psrc[1];    // Y0
            pdst[1] = psrc[3];    // Y1
            pdst[2] = psrc[5];    // Y2
            pdst[3] = psrc[7];    // Y3
            pdst[4] = psrc[8];    // Y4
            pdst[5] = psrc[9];    // Y5
            pdst[6] = psrc[10];    // Y6
            pdst[7] = psrc[11];    // Y7

            psrc += 12;
            pdst += 8;
        }

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_Y16(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 12 / 8;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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


void blipvert::Y41P_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width;

    if (!in_stride)
        in_stride = width * 12 / 8;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

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

void blipvert::Y41P_to_IY41(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    Progressive_to_Interlaced(height, width * 2, flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}

void blipvert::Y41P_to_AYUV(int32_t width, int32_t height,
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

//
//  IMC1 to YUV
//

void blipvert::IMC1_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, false, true);
}

void blipvert::IMC1_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 0, 2, 1, 3,	// YUY2
        in_buf, in_stride, false, false,    // IMC1
        flipped);
}

void blipvert::IMC1_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 1, 3, 0, 2,	// UYVY
        in_buf, in_stride, false, false,    // IMC1
        flipped);
}

void blipvert::IMC1_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 0, 2, 3, 1,	// YVYU
        in_buf, in_stride, false, false,    // IMC1
        flipped);
}

void blipvert::IMC1_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 1, 3, 2, 0,	// YVYU
        in_buf, in_stride, false, false,    // IMC1
        flipped);
}

void blipvert::IMC1_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 2, true, in_buf, in_stride, false, false, flipped);
}

void blipvert::IMC1_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 2, false, in_buf, in_stride, false, false, flipped);
}

void blipvert::IMC1_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 4, true, in_buf, in_stride, false, false, flipped);
}

void blipvert::IMC1_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 4, false, in_buf, in_stride, false, false, flipped);
}

//
//  IMC2 to YUV
//

void blipvert::IMC2_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, false, true, true);
}

void blipvert::IMC2_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 0, 2, 1, 3,	// YUY2
        in_buf, in_stride, false, true,     // IMC2
        flipped);
}

void blipvert::IMC2_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 1, 3, 0, 2,	// UYVY
        in_buf, in_stride, false, true,     // IMC2
        flipped);
}

void blipvert::IMC2_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 0, 2, 3, 1,	// YVYU
        in_buf, in_stride, false, true,     // IMC2
        flipped);
}

void blipvert::IMC2_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 1, 3, 2, 0,	// YVYU
        in_buf, in_stride, false, true,     // IMC2
        flipped);
}

void blipvert::IMC2_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 2, true, in_buf, in_stride, false, true, flipped);
}

void blipvert::IMC2_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 2, false, in_buf, in_stride, false, true, flipped);
}

void blipvert::IMC2_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 4, true, in_buf, in_stride, false, true, flipped);
}

void blipvert::IMC2_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 4, false, in_buf, in_stride, false, true, flipped);
}

//
//  IMC3 to YUV
//

void blipvert::IMC3_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, false, true);
}

void blipvert::IMC3_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 0, 2, 1, 3,	// YUY2
        in_buf, in_stride, true, false,     // IMC3
        flipped);
}

void blipvert::IMC3_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 1, 3, 0, 2,	// UYVY
        in_buf, in_stride, true, false,     // IMC3
        flipped);
}

void blipvert::IMC3_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 0, 2, 3, 1,	// YVYU
        in_buf, in_stride, true, false,     // IMC3
        flipped);
}

void blipvert::IMC3_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 1, 3, 2, 0,	// YVYU
        in_buf, in_stride, true, false,     // IMC3
        flipped);
}

void blipvert::IMC3_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 2, true, in_buf, in_stride, true, false, flipped);
}

void blipvert::IMC3_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 2, false, in_buf, in_stride, true, false, flipped);
}

void blipvert::IMC3_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 4, true, in_buf, in_stride, true, false, flipped);
}

void blipvert::IMC3_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 4, false, in_buf, in_stride, true, false, flipped);
}

//
//  IMC4 to YUV
//

void blipvert::IMC4_to_AYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_AYUV(width, height, out_buf, out_stride, in_buf, in_stride, true, true, true);
}

void blipvert::IMC4_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 0, 2, 1, 3,	// YUY2
        in_buf, in_stride, true, true,      // IMC4
        flipped);
}

void blipvert::IMC4_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 1, 3, 0, 2,	// UYVY
        in_buf, in_stride, true, true,      // IMC4
        flipped);
}

void blipvert::IMC4_to_YVYU(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 0, 2, 3, 1,	// YVYU
        in_buf, in_stride, true, true,      // IMC4
        flipped);
}

void blipvert::IMC4_to_VYUY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PackedY422(width, height,
        out_buf, out_stride, 1, 3, 2, 0,	// YVYU
        in_buf, in_stride, true, true,      // IMC4
        flipped);
}

void blipvert::IMC4_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 2, true, in_buf, in_stride, true, true, flipped);
}

void blipvert::IMC4_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 2, false, in_buf, in_stride, true, true, flipped);
}

void blipvert::IMC4_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 4, true, in_buf, in_stride, true, true, flipped);
}

void blipvert::IMC4_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    IMCx_to_PlanarYUV(width, height, out_buf, out_stride, 4, false, in_buf, in_stride, true, true, flipped);
}

//
// Misc YUV format transforms
//

void blipvert::UYVY_to_IUYV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    Progressive_to_Interlaced(height, width * 2, flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}

void blipvert::IUYV_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    Interlaced_to_Progressive(height, width * 2, flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}

void blipvert::IY41_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, xRGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    Interlaced_to_Progressive(height, width * 2, flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}


