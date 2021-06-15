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
#include "YUVtoYUV.h"
#include "CommonMacros.h"
#include "Utilities.h"

#include <cstring>

using namespace blipvert;

//
// Local generic YUV fill functions
//

void Fill_PackedY422(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    int32_t y0_offset, int32_t y1_offset, int32_t u_offset, int32_t v_offset)
{
    if (!out_stride)
        out_stride = width * 2;

    uint32_t fill = static_cast<uint32_t>((y_level << (y0_offset * 8)) | (y_level << (y1_offset * 8)) | \
        (u_level << (u_offset * 8)) | (v_level << (v_offset * 8)));

    for (int32_t h = 0; h < height; h++)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        for (int32_t w = 0; w < width; w += 2)
        {
            *pdst++ = fill;
        }

        out_buf += out_stride;
    }
}

void Fill_PlanarYUV(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
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
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 2)
            {
                yp[0] = y_level;
                yp[1] = y_level;
                yp[y_stride] = y_level;
                yp[y_stride + 1] = y_level;
                uplane[x >> 1] = u_level;
                vplane[x >> 1] = v_level;
                yp += 2;
            }
            out_buf += (y_stride * 2);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
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
                    yp[ys] = y_level;
                    yp[ys + 1] = y_level;
                    yp[ys + 2] = y_level;
                    yp[ys + 3] = y_level;
                    ys += y_stride;
                }

                uplane[x >> 2] = u_level;
                vplane[x >> 2] = v_level;

                yp += 4;
            }
            out_buf += (y_stride * 4);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
}


bool Check_PlanarYUV(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    bool uFirst, int32_t decimation, bool flipped)
{
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
            uint8_t* yp = out_buf;
            for (int32_t x = 0; x < width; x += 2)
            {
                if (yp[0] != y_level) return false;
                if (yp[1] != y_level) return false;
                if (yp[y_stride] != y_level) return false;
                if (yp[y_stride + 1] != y_level) return false;
                if (uplane[x >> 1] != u_level) return false;
                if (vplane[x >> 1] != v_level) return false;
                yp += 2;
            }
            out_buf += (y_stride * 2);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }
    else if (decimation == 4)
    {
        for (int32_t y = 0; y < height; y += 4)
        {
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
                    if (yp[ys] != y_level) return false;
                    if (yp[ys + 1] != y_level) return false;
                    if (yp[ys + 2] != y_level) return false;
                    if (yp[ys + 3] != y_level) return false;
                    ys += y_stride;
                }

                if (uplane[x >> 2] != u_level) return false;
                if (vplane[x >> 2] != v_level) return false;

                yp += 4;
            }
            out_buf += (y_stride * 4);

            uplane += uv_stride;
            vplane += uv_stride;
        }
    }

    return true;
}

//
// Local generic YUV to YUV conversion functions
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
        // Scaling down from 2 to 4 decimation
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
        // Scaling up from 4 to 2 decimation
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
                out_up[dst_index] = out_up[dst_index + out_uv_stride] = in_up[x];
                out_vp[dst_index] = out_vp[dst_index + out_uv_stride] = in_vp[x];
                dst_index++;
                out_up[dst_index] = out_up[dst_index + out_uv_stride] = \
                    static_cast<uint8_t>((static_cast<uint16_t>(in_up[x]) + \
                        static_cast<uint16_t>(in_up[x + 1])) >> 1);
                out_vp[dst_index] = out_vp[dst_index + out_uv_stride] = \
                    static_cast<uint8_t>((static_cast<uint16_t>(in_vp[x]) + \
                        static_cast<uint16_t>(in_vp[x + 1])) >> 1);
                dst_index++;
            }

            out_up += out_uv_stride_x_2;
            out_vp += out_uv_stride_x_2;

            in_up += in_uv_stride;
            in_vp += in_uv_stride;
        }

        uint8_t* prev_up = out_uplane + out_uv_stride;
        uint8_t* this_up = prev_up + out_uv_stride;
        uint8_t* prev_vp = out_vplane + out_uv_stride;
        uint8_t* this_vp = prev_vp + out_uv_stride;
        for (int32_t y = 2; y < out_uv_height; y += 2)
        {
            for (int32_t x = 0; x < out_uv_width; x++)
            {
                prev_up[x] = static_cast<uint8_t>((static_cast<uint16_t>(this_up[x]) + static_cast<uint16_t>(prev_up[x])) >> 1);
                prev_vp[x] = static_cast<uint8_t>((static_cast<uint16_t>(this_vp[x]) + static_cast<uint16_t>(prev_vp[x])) >> 1);
            }

            prev_up += out_uv_stride_x_2;
            this_up += out_uv_stride_x_2;

            prev_vp += out_uv_stride_x_2;
            this_vp += out_uv_stride_x_2;
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
                pdst[out_u] = up[0];
                pdst[out_v] = vp[0];
                pdst[out_y0] = yp[0];
                pdst[out_y1] = yp[1];

                pdst[out_u + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[uv_stride])) >> 1);
                pdst[out_v + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[uv_stride])) >> 1);
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
                pdst[out_u] = up[0];
                pdst[out_v] = vp[0];
                pdst[out_y0] = yp[0];
                pdst[out_y1] = yp[1];

                // column 1 row 2
                pdst[out_u + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 768) + \
                    (static_cast<int32_t>(up[uv_stride]) * 256)) >> 10);
                pdst[out_v + out_stride] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 768) + \
                    (static_cast<int32_t>(vp[uv_stride]) * 256)) >> 10);
                pdst[out_y0 + out_stride] = yp[y_stride];
                pdst[out_y1 + out_stride] = yp[1 + y_stride];

                // column 1 row 3
                pdst[out_u + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[uv_stride])) >> 1);
                pdst[out_v + out_stride_x_2] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[uv_stride])) >> 1);
                pdst[out_y0 + out_stride_x_2] = yp[y_stride_x_2];
                pdst[out_y1 + out_stride_x_2] = yp[1 + y_stride_x_2];

                // column 1 row 4
                pdst[out_u + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 256) + \
                    (static_cast<int32_t>(up[uv_stride]) * 768)) >> 10);
                pdst[out_v + out_stride_x_3] = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 256) + \
                    (static_cast<int32_t>(vp[uv_stride]) * 768)) >> 10);
                pdst[out_y0 + out_stride_x_3] = yp[y_stride_x_3];
                pdst[out_y1 + out_stride_x_3] = yp[1 + y_stride_x_3];

                // prep for column 2
                int32_t uval_top = (static_cast<int32_t>(up[0]) + static_cast<int32_t>(up[1])) >> 1;
                int32_t vval_top = (static_cast<int32_t>(vp[0]) + static_cast<int32_t>(vp[1])) >> 1;
                int32_t uval_bottom = (static_cast<int32_t>(up[uv_stride]) + static_cast<int32_t>(up[1 + uv_stride])) >> 1;
                int32_t vval_bottom = (static_cast<int32_t>(vp[uv_stride]) + static_cast<int32_t>(vp[1 + uv_stride])) >> 1;

                // column 2 row 1
                pdst[out_u + 4] = static_cast<uint8_t>(uval_top);
                pdst[out_v + 4] = static_cast<uint8_t>(vval_top);
                pdst[out_y0 + 4] = yp[2];
                pdst[out_y1 + 4] = yp[3];

                // column 2 row 2
                pdst[out_u + out_stride + 4] = static_cast<uint8_t>(((uval_top * 768) + \
                    (uval_bottom * 256)) >> 10);
                pdst[out_v + out_stride + 4] = static_cast<uint8_t>(((vval_top * 768) + \
                    (vval_bottom * 256)) >> 10);
                pdst[out_y0 + out_stride + 4] = yp[2 + y_stride];
                pdst[out_y1 + out_stride + 4] = yp[3 + y_stride];

                // column 2 row 3
                pdst[out_u + out_stride_x_2 + 4] = static_cast<uint8_t>((uval_top + uval_bottom) >> 1);
                pdst[out_v + out_stride_x_2 + 4] = static_cast<uint8_t>((vval_top + vval_bottom) >> 1);
                pdst[out_y0 + out_stride_x_2 + 4] = yp[2 + y_stride_x_2];
                pdst[out_y1 + out_stride_x_2 + 4] = yp[3 + y_stride_x_2];

                // column 2 row 4
                pdst[out_u + out_stride_x_3 + 4] = static_cast<uint8_t>(((uval_top * 256) + \
                    (uval_bottom * 768)) >> 10);
                pdst[out_v + out_stride_x_3 + 4] = static_cast<uint8_t>(((vval_top * 256) + \
                    (vval_bottom * 768)) >> 10);
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
        y_stride = in_stride;
        uv_stride = in_stride;
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
                pdst[0] = up[0];
                pdst[1] = yp[0];
                pdst[2] = vp[0];

                // column 1 row 2
                pdst[out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[uv_stride])) >> 1);
                pdst[1 + out_stride] = yp[y_stride];
                pdst[2 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[uv_stride])) >> 1);

                // column 2 row 1
                pdst[3] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1);
                pdst[4] = yp[1];
                pdst[5] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1);

                // column 2 row 2
                pdst[3 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                    static_cast<uint16_t>(up[1]) + \
                    static_cast<uint16_t>(up[uv_stride]) + \
                    static_cast<uint16_t>(up[1 + uv_stride])) >> 2);
                pdst[4 + out_stride] = yp[1 + y_stride];
                pdst[5 + out_stride] = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[uv_stride]) + \
                    static_cast<uint16_t>(vp[1 + uv_stride])) >> 2);
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
                uint16_t u_avg_col[4];
                u_avg_col[0] = up[0];
                u_avg_col[1] = static_cast<uint16_t>(((static_cast<uint32_t>(up[0]) * 768) + \
                    (static_cast<uint32_t>(up[1]) * 256)) >> 10);
                u_avg_col[2] = (static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[1])) >> 1;
                u_avg_col[3] = static_cast<uint16_t>(((static_cast<uint32_t>(up[0]) * 256) + \
                    (static_cast<uint32_t>(up[1]) * 768)) >> 10);

                uint16_t u_avg_row[4];
                u_avg_row[0] = up[0];
                u_avg_row[1] = static_cast<uint16_t>(((static_cast<uint32_t>(up[0]) * 768) + \
                    (static_cast<uint32_t>(up[uv_stride]) * 256)) >> 10);
                u_avg_row[2] = (static_cast<uint16_t>(up[0]) + static_cast<uint16_t>(up[uv_stride])) >> 1;
                u_avg_row[3] = static_cast<uint16_t>(((static_cast<uint32_t>(up[0]) * 256) + \
                    (static_cast<uint32_t>(up[uv_stride]) * 768)) >> 10);

                uint16_t v_avg_col[4];
                v_avg_col[0] = vp[0];
                v_avg_col[1] = static_cast<uint16_t>(((static_cast<uint32_t>(vp[0]) * 768) + \
                    (static_cast<uint32_t>(vp[1]) * 256)) >> 10);
                v_avg_col[2] = (static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[1])) >> 1;
                v_avg_col[3] = static_cast<uint16_t>(((static_cast<uint32_t>(vp[0]) * 256) + \
                    (static_cast<uint32_t>(vp[1]) * 768)) >> 10);

                uint16_t v_avg_row[4];
                v_avg_row[0] = vp[0];
                v_avg_row[1] = static_cast<uint16_t>(((static_cast<uint32_t>(vp[0]) * 768) + \
                    (static_cast<uint32_t>(vp[uv_stride]) * 256)) >> 10);
                v_avg_row[2] = (static_cast<uint16_t>(vp[0]) + static_cast<uint16_t>(vp[uv_stride])) >> 1;
                v_avg_row[3] = static_cast<uint16_t>(((static_cast<uint32_t>(vp[0]) * 256) + \
                    (static_cast<uint32_t>(vp[uv_stride]) * 768)) >> 10);

                for (int32_t row = 0; row < 4; row++)
                {
                    for (int32_t col = 0; col < 4; col++)
                    {
                        pdst[col * 3 + out_stride * row] = (u_avg_col[col] + u_avg_row[row]) >> 1;
                        pdst[1 + col * 3 + out_stride * row] = yp[col + y_stride * row];
                        pdst[2 + col * 3 + out_stride * row] = (v_avg_col[col] + v_avg_row[row]) >> 1;
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
        y_stride = in_stride;
        uv_stride = in_stride;
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

                    u_val = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                        static_cast<uint16_t>(up[1]) + \
                        static_cast<uint16_t>(up[uv_stride]) + \
                        static_cast<uint16_t>(up[1 + uv_stride])) >> 2);
                v_val = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[1]) + \
                    static_cast<uint16_t>(vp[uv_stride]) + \
                    static_cast<uint16_t>(vp[1 + uv_stride])) >> 2);
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
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst)), up[0], vp[0], yp[0], yp[1], yp[2], yp[3])

                    uint8_t u_val = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 768) + \
                        (static_cast<int32_t>(up[uv_stride]) * 256)) >> 10);
                uint8_t v_val = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 768) + \
                    (static_cast<int32_t>(vp[uv_stride]) * 256)) >> 10);
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride)), u_val, v_val, \
                    yp[y_stride], yp[1 + y_stride], yp[2 + y_stride], yp[3 + y_stride])

                    u_val = static_cast<uint8_t>((static_cast<uint16_t>(up[0]) + \
                        static_cast<uint16_t>(up[uv_stride])) >> 1);
                v_val = static_cast<uint8_t>((static_cast<uint16_t>(vp[0]) + \
                    static_cast<uint16_t>(vp[uv_stride])) >> 1);
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride_x_2)), u_val, v_val, \
                    yp[y_stride_x_2], yp[1 + y_stride_x_2], yp[2 + y_stride_x_2], yp[3 + y_stride_x_2])

                    u_val = static_cast<uint8_t>(((static_cast<int32_t>(up[0]) * 256) + \
                        (static_cast<int32_t>(up[uv_stride]) * 768)) >> 10);
                v_val = static_cast<uint8_t>(((static_cast<int32_t>(vp[0]) * 256) + \
                    (static_cast<int32_t>(vp[uv_stride]) * 768)) >> 10);
                PackCLJRDword(*(reinterpret_cast<uint32_t*>(pdst + out_stride_x_3)), u_val, v_val, \
                    yp[y_stride_x_3], yp[1 + y_stride_x_3], yp[2 + y_stride_x_3], yp[3 + y_stride_x_3])

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
        y_stride = in_stride;
        uv_stride = in_stride;
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
        y_stride = in_stride;
        uv_stride = in_stride;
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

//
// Public YUV fill functions
//

void blipvert::Fill_YUY2(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        pBuffer, stride,
        0, 2, 1, 3);
}

void blipvert::Fill_UYVY(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        pBuffer, stride,
        1, 3, 0, 2);
}

void blipvert::Fill_YVYU(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        pBuffer, stride,
        0, 2, 3, 1);
}

void blipvert::Fill_VYUY(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PackedY422(y_level, u_level, v_level,
        width, height,
        pBuffer, stride,
        1, 3, 2, 0);
}

void blipvert::Fill_IYUV(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, pBuffer, stride, true, 2, true);
}

void blipvert::Fill_YV12(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, pBuffer, stride, false, 2, false);
}

void blipvert::Fill_YVU9(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, pBuffer, stride, false, 4, false);
}

void blipvert::Fill_YUV9(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    Fill_PlanarYUV(y_level, u_level, v_level, width, height, pBuffer, stride, true, 4, true);
}

void blipvert::Fill_IYU1(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 12 / 8;

    for (int32_t h = 0; h < height; h++)
    {
        uint8_t* pdst = pBuffer;
        for (int32_t w = 0; w < width; w += 4)
        {
            pdst[0] = u_level;
            pdst[1] = y_level;
            pdst[2] = y_level;
            pdst[3] = v_level;
            pdst[4] = y_level;
            pdst[5] = y_level;
            pdst += 6;
        }

        pBuffer += stride;
    }
}

bool blipvert::Check_IYU1(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 12 / 8;

    for (int32_t h = 0; h < height; h++)
    {
        uint8_t* pdst = pBuffer;
        for (int32_t w = 0; w < width; w += 4)
        {
            if (pdst[0] != u_level) return false;
            if (pdst[1] != y_level) return false;
            if (pdst[2] != y_level) return false;
            if (pdst[3] != v_level) return false;
            if (pdst[4] != y_level) return false;
            if (pdst[5] != y_level) return false;
            pdst += 6;
        }

        pBuffer += stride;
    }

    return true;
}

void blipvert::Fill_IYU2(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 3;

    for (int32_t h = 0; h < height; h++)
    {
        uint8_t* pdst = pBuffer;
        for (int32_t w = 0; w < width; w++)
        {
            pdst[0] = u_level;
            pdst[1] = y_level;
            pdst[2] = v_level;
            pdst += 3;
        }

        pBuffer += stride;
    }
}

bool blipvert::Check_IYU2(uint8_t y_level, uint8_t u_level, uint8_t v_level,
    int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 3;

    for (int32_t h = 0; h < height; h++)
    {
        uint8_t* pdst = pBuffer;
        for (int32_t w = 0; w < width; w++)
        {
            if (pdst[0] != u_level) return false;
            if (pdst[1] != y_level) return false;
            if (pdst[2] != v_level) return false;
            pdst += 3;
        }

        pBuffer += stride;
    }

    return true;
}


void blipvert::Fill_Y800(uint8_t y_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
    {
        memset(pBuffer, y_level, width * height);
        return;
    }

    for (int32_t h = 0; h < height; h++)
    {
        uint8_t* pdst = pBuffer;
        for (int32_t w = 0; w < width; w++)
        {
            *pdst++ = y_level;
        }

        pBuffer += stride;
    }
}

void blipvert::Fill_Y41P(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width / 8 * 12;

    while (height)
    {
        uint8_t* pdst = pBuffer;
        int32_t hcount = width;
        while (hcount)
        {
            pdst[0] = u_level;
            pdst[2] = v_level;

            pdst[1] = y_level;
            pdst[3] = y_level;
            pdst[5] = y_level;
            pdst[7] = y_level;

            pdst[4] = u_level;
            pdst[6] = v_level;

            pdst[8] = y_level;
            pdst[9] = y_level;
            pdst[10] = y_level;
            pdst[11] = y_level;

            pdst += 12;
            hcount -= 8;
        }

        pBuffer += stride;
        height--;
    }
}

void Fill_CLJR(uint8_t y_level, uint8_t u_level, uint8_t v_level, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width;

    uint32_t fill;
    PackCLJRDword(fill, u_level, v_level, y_level, y_level, y_level, y_level);
    while (height)
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(pBuffer);
        int32_t hcount = width;
        while (hcount)
        {
            *pdst++ = fill;
            hcount -= 4;
        }

        pBuffer += stride;
        height--;
    }
}

//
// Public Yuv to Yuv conversion functions
//


//
// YUY2 to YUV
//

void blipvert::YUY2_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PackedY422_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 1, 3,
        flipped);
}
void blipvert::YUY2_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PackedY422_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 1, 3,
        flipped);
}

//
// UYVY to YUV
//

void blipvert::UYVY_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PackedY422_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 0, 2,
        flipped);
}
void blipvert::UYVY_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PackedY422_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 0, 2,
        flipped);
}

void blipvert::UYVY_to_IUYV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    Progressive_to_Interlaced(height, width * 2, flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}

//
// YVYU to YUV
//

void blipvert::YVYU_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PackedY422_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 3, 1,
        flipped);
}

void blipvert::YVYU_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PackedY422_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        0, 2, 3, 1,
        flipped);
}

//
// VYUY to YUV
//

void blipvert::VYUY_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PackedY422_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 2, 0,
        flipped);
}

void blipvert::VYUY_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PackedY422_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        1, 3, 2, 0,
        flipped);
}

//
// IYUV to YUV
//

void blipvert::IYUV_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,        // YVU9
        in_buf, in_stride, 2, true,           // IYUV
        flipped);
}

void blipvert::IYUV_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,        // YUV9
        in_buf, in_stride, 2, true,          // IYUV
        flipped);
}

void blipvert::IYUV_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,        // YV12
        in_buf, in_stride, 2, true,           // IYUV
        flipped);
}

void blipvert::IYUV_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, true,
        flipped);
}

void blipvert::IYUV_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, true,
        flipped);
}

void blipvert::IYUV_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYUV_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, true,
        flipped);
}

void blipvert::IYUV_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, true,
        flipped);
}

//
// YV12 to YUV
//

void blipvert::YV12_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,        // YVU9
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,        // YUV9
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,        // IYUV
        in_buf, in_stride, 2, false,        // YV12
        flipped);
}

void blipvert::YV12_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, false,
        flipped);
}

void blipvert::YV12_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, false,
        flipped);
}

void blipvert::YV12_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::YV12_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, false,
        flipped);
}

void blipvert::YV12_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 2, false,
        flipped);
}

//
// YVU9 to YUV
//

void blipvert::YVU9_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,        // IYUV
        in_buf, in_stride, 4, false,        // YVU9
        flipped);
}

void blipvert::YVU9_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,        // YV12
        in_buf, in_stride, 4, false,        // YVU9
        flipped);
}

void blipvert::YVU9_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,        // YUV9
        in_buf, in_stride, 4, false,        // YVU9
        flipped);
}

void blipvert::YVU9_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, false,
        flipped);
}

void blipvert::YVU9_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, false,
        flipped);
}

void blipvert::YVU9_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::YVU9_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, false,
        flipped);
}

void blipvert::YVU9_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, false,
        flipped);
}

//
// YUV9 to YUV
//

void blipvert::YUV9_to_IYUV(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,        // IYUV
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,        // YV12
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,        // YVU9
        in_buf, in_stride, 4, true,            // YUV9
        flipped);
}

void blipvert::YUV9_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_IYU1(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, true,
        flipped);
}

void blipvert::YUV9_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_IYU2(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, true,
        flipped);
}

void blipvert::YUV9_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_Y800(width, height,
        out_buf, out_stride,
        in_buf, in_stride,
        flipped);
}

void blipvert::YUV9_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_CLJR(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, true,
        flipped);
}

void blipvert::YUV9_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    PlanarYUV_to_Y41P(width, height,
        out_buf, out_stride,
        in_buf, in_stride, 4, true,
        flipped);
}

//
// IYU1 to YUV
//

void blipvert::IYU1_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    IYU1_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    IYU1_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    IYU1_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    IYU1_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU1_to_IYU2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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

void blipvert::IYU1_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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

//
// IYU2 to YUV
//

void blipvert::IYU2_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    IYU2_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    IYU2_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    IYU2_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    IYU2_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::IYU2_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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

void blipvert::IYU2_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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

void blipvert::Y800_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    Y800_to_PlanarYUV(width, height,
        out_buf, out_stride, 2,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y800_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    Y800_to_PlanarYUV(width, height,
        out_buf, out_stride, 2,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y800_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    Y800_to_PlanarYUV(width, height,
        out_buf, out_stride, 4,
        in_buf, in_stride,
        flipped);
}


void blipvert::Y800_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    Y800_to_PlanarYUV(width, height,
        out_buf, out_stride, 4,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y800_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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

//
// CLJR to YUV
//

void blipvert::CLJR_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    CLJR_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    CLJR_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    CLJR_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    CLJR_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::CLJR_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
        uint32_t* psrc = reinterpret_cast<uint32_t*>(out_buf);
        uint8_t* pdst = in_buf;
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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

void blipvert::CLJR_to_Y41P(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
        uint32_t* psrc = reinterpret_cast<uint32_t*>(out_buf);
        uint8_t* pdst = in_buf;
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

//
// Y41P to YUV
//

void blipvert::Y41P_to_YUY2(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    Y41P_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_YV12(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    Y41P_to_PlanarYUV(width, height,
        out_buf, out_stride, 2, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_YVU9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    Y41P_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, false,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_YUV9(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    Y41P_to_PlanarYUV(width, height,
        out_buf, out_stride, 4, true,
        in_buf, in_stride,
        flipped);
}

void blipvert::Y41P_to_IYU1(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
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

        pdst[12] = psrc[4];    // U
        pdst[13] = psrc[8];    // Y4
        pdst[14] = psrc[6];    // V

        pdst[15] = psrc[4];    // U
        pdst[16] = psrc[9];    // Y5
        pdst[17] = psrc[6];    // V

        pdst[18] = psrc[4];    // U
        pdst[19] = psrc[10];// Y6
        pdst[20] = psrc[6];    // V

        pdst[21] = psrc[4];    // U
        pdst[22] = psrc[11];// Y7
        pdst[23] = psrc[6];    // V

        in_buf += in_stride;
        out_buf += out_stride;
    }
}

void blipvert::Y41P_to_Y800(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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

void blipvert::Y41P_to_CLJR(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    Progressive_to_Interlaced(height, width * 2, flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}

//
// Misc YUV format conversions
//

void blipvert::IUYV_to_UYVY(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
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
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    Interlaced_to_Progressive(height, width * 2, flipped,
        out_buf, out_stride,
        in_buf, in_stride);
}


