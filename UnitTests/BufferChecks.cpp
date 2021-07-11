
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
#include "BufferChecks.h"
#include "Utilities.h"
#include "CommonMacros.h"

using namespace blipvert;
using namespace BlipvertUnitTests;

const uint32_t BlipvertUnitTests::TestBufferWidth = 16;
const uint32_t BlipvertUnitTests::TestBufferHeight = 16;
const uint32_t BlipvertUnitTests::StrideBumpTestValue = 16;

uint32_t BlipvertUnitTests::StrideBump = 0;

uint32_t BlipvertUnitTests::CalculateStrideBump(const MediaFormatID& inFormat, uint32_t width)
{
	if (!StrideBump)
	{
		return 0;
	}

	VideoFormatInfo info;
	if (!GetVideoFormatInfo(inFormat, info))
	{
		return 0;
	}

	if (info.effectiveBitsPerPixel <= 0)
	{
		return 0;
	}

	uint32_t bitsPerLine = width * static_cast<uint32_t>(info.effectiveBitsPerPixel);
	uint32_t bytesPerLine = ((bitsPerLine + 31) & (~31)) / 8;

	return bytesPerLine + StrideBump;
}

typedef struct {
	const MediaFormatID& target;
	t_buffercheckfunc pProcAddr;
} BufferCheckEntry;

BufferCheckEntry BufferCheckFuncTable[] = {
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
	{MVFMT_Y41P, Check_Y41P},
	{MVFMT_CLJR, Check_CLJR},
	{MVFMT_Y800, Check_Y800},
	{MVFMT_Y16, Check_Y16},
	{MVFMT_AYUV, Check_AYUV},
	{MVFMT_IMC1, Check_IMC1},
	{MVFMT_IMC2, Check_IMC2},
	{MVFMT_IMC3, Check_IMC3},
	{MVFMT_IMC4, Check_IMC4},
	{MVFMT_RGBA, Check_RGBA},
	{MVFMT_RGB32, Check_RGB32},
	{MVFMT_RGB24, Check_RGB24},
	{MVFMT_RGB565, Check_RGB565},
	{MVFMT_RGB555, Check_RGB555},
	{MVFMT_ARGB1555, Check_ARGB1555},
	{MVFMT_UNDEFINED, nullptr}
};

t_buffercheckfunc BlipvertUnitTests::FindBufferCheckFunction(const MediaFormatID& target)
{
	int16_t index = 0;
	while (BufferCheckFuncTable[index].pProcAddr != nullptr)
	{
		if (BufferCheckFuncTable[index].target == target)
		{
			return BufferCheckFuncTable[index].pProcAddr;
		}
		index++;
	}

	return nullptr;
}

bool Check_PackedY422(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level,
	int32_t width, int32_t height,
	uint8_t* pBuffer, int32_t stride,
	int32_t y0_offset, int32_t y1_offset, int32_t u_offset, int32_t v_offset)
{
	if (!stride)
		stride = width * 2;

	uint32_t fill = static_cast<uint32_t>((ry_level << (y0_offset * 8)) | (ry_level << (y1_offset * 8)) | \
		(gu_level << (u_offset * 8)) | (bv_level << (v_offset * 8)));

	for (int32_t h = 0; h < height; h++)
	{
		uint32_t* pdst = reinterpret_cast<uint32_t*>(pBuffer);
		for (int32_t w = 0; w < width; w += 2)
		{
			if (*pdst++ != fill) return false;
		}

		pBuffer += stride;
	}

	return true;
}


bool Check_PlanarYUV(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level,
	int32_t width, int32_t height,
	uint8_t* pBuffer, int32_t stride,
	bool uFirst, int32_t decimation)
{
	int32_t uv_width = width / decimation;
	int32_t uv_height = height / decimation;

	int16_t y_stride, uv_stride;
	if (!stride)
	{
		y_stride = width;
		uv_stride = uv_width;
	}
	else
	{
		y_stride = stride;
		uv_stride = stride;
	}

	uint8_t* vplane;
	uint8_t* uplane;
	if (uFirst)
	{
		uplane = pBuffer + (y_stride * height);
		vplane = uplane + (uv_stride * uv_height);
	}
	else
	{
		vplane = pBuffer + (y_stride * height);
		uplane = vplane + (uv_stride * uv_height);
	}

	if (decimation == 2)
	{
		for (int32_t y = 0; y < height; y += 2)
		{
			uint8_t* yp = pBuffer;
			for (int32_t x = 0; x < width; x += 2)
			{
				if (yp[0] != ry_level)
					return false;
				if (yp[1] != ry_level)
					return false;
				if (yp[y_stride] != ry_level)
					return false;
				if (yp[y_stride + 1] != ry_level)
					return false;

				if (uplane[x >> 1] != gu_level)
					return false;
				if (vplane[x >> 1] != bv_level)
					return false;
				yp += 2;
			}
			pBuffer += (y_stride * 2);

			uplane += uv_stride;
			vplane += uv_stride;
		}
	}
	else if (decimation == 4)
	{
		for (int32_t y = 0; y < height; y += 4)
		{
			uint8_t* yp = pBuffer;
			for (int32_t x = 0; x < width; x += 4)
			{
				int32_t ys = 0;

				for (int32_t dec = 0; dec < 4; dec++)
				{
					if (yp[ys] != ry_level)
						return false;
					if (yp[ys + 1] != ry_level)
						return false;
					if (yp[ys + 2] != ry_level)
						return false;
					if (yp[ys + 3] != ry_level)
						return false;

					ys += y_stride;
				}

				if (uplane[x >> 2] != gu_level)
					return false;
				if (vplane[x >> 2] != bv_level)
					return false;

				yp += 4;
			}
			pBuffer += (y_stride * 4);

			uplane += uv_stride;
			vplane += uv_stride;
		}
	}

	return true;
}

bool Check_IMCx(uint8_t y_level, uint8_t u_level, uint8_t v_level,
	int32_t width, int32_t height,
	uint8_t* out_buf, int32_t out_stride,
	bool uFirst, bool interlaced)
{
	int32_t uv_width = width / 2;

	if (!out_stride)
		out_stride = width;

	uint8_t* vplane;
	uint8_t* uplane;
	if (uFirst)
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

	int32_t out_stride_x_2 = out_stride * 2;

	for (int32_t y = 0; y < height; y += 2)
	{
		uint8_t* yp = out_buf;
		for (int32_t x = 0; x < width; x += 2)
		{
			if (yp[0] != y_level) return false;
			if (yp[1] != y_level) return false;
			if (yp[out_stride] != y_level) return false;
			if (yp[out_stride + 1] != y_level) return false;
			if (uplane[x >> 1] != u_level) return false;
			if (vplane[x >> 1] != v_level) return false;
			yp += 2;
		}
		out_buf += out_stride_x_2;

		uplane += out_stride;
		vplane += out_stride;
	}

	return true;
}


bool BlipvertUnitTests::Check_IYUV(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_PlanarYUV(ry_level, gu_level, bv_level, width, height, pBuffer, stride, true, 2);
}

bool BlipvertUnitTests::Check_YV12(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_PlanarYUV(ry_level, gu_level, bv_level, width, height, pBuffer, stride, false, 2);
}

bool BlipvertUnitTests::Check_YVU9(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_PlanarYUV(ry_level, gu_level, bv_level, width, height, pBuffer, stride, false, 4);
}

bool BlipvertUnitTests::Check_YUV9(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_PlanarYUV(ry_level, gu_level, bv_level, width, height, pBuffer, stride, true, 4);
}

bool BlipvertUnitTests::Check_IMC1(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_IMCx(ry_level, gu_level, bv_level, width, height, pBuffer, stride, false, false);
}

bool BlipvertUnitTests::Check_IMC2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_IMCx(ry_level, gu_level, bv_level, width, height, pBuffer, stride, false, true);
}

bool BlipvertUnitTests::Check_IMC3(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_IMCx(ry_level, gu_level, bv_level, width, height, pBuffer, stride, true, false);
}

bool BlipvertUnitTests::Check_IMC4(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_IMCx(ry_level, gu_level, bv_level, width, height, pBuffer, stride, true, true);
}

bool BlipvertUnitTests::Check_YUY2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_PackedY422(ry_level, gu_level, bv_level,
		width, height,
		pBuffer, stride,
		0, 2, 1, 3);
}

bool BlipvertUnitTests::Check_UYVY(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
	int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_PackedY422(ry_level, gu_level, bv_level,
		width, height,
		pBuffer, stride,
		1, 3, 0, 2);
}

bool BlipvertUnitTests::Check_YVYU(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
	int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_PackedY422(ry_level, gu_level, bv_level,
		width, height,
		pBuffer, stride,
		0, 2, 3, 1);
}

bool BlipvertUnitTests::Check_VYUY(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
	int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_PackedY422(ry_level, gu_level, bv_level,
		width, height,
		pBuffer, stride,
		1, 3, 2, 0);
}

bool BlipvertUnitTests::Check_Y41P(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
	int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width / 8 * 12;

	while (height)
	{
		uint8_t* pdst = pBuffer;
		int32_t hcount = width;
		while (hcount)
		{
			if (pdst[0] != gu_level)
				return false;
			if (pdst[2] != bv_level)
				return false;

			if (pdst[1] != ry_level)
				return false;
			if (pdst[3] != ry_level)
				return false;
			if (pdst[5] != ry_level)
				return false;
			if (pdst[7] != ry_level)
				return false;

			if (pdst[4] != gu_level)
				return false;
			if (pdst[6] != bv_level)
				return false;

			if (pdst[8] != ry_level)
				return false;
			if (pdst[9] != ry_level)
				return false;
			if (pdst[10] != ry_level)
				return false;
			if (pdst[11] != ry_level)
				return false;

			pdst += 12;
			hcount -= 8;
		}

		pBuffer += stride;
		height--;
	}

	return true;
}

bool BlipvertUnitTests::Check_CLJR(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
	int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width;

	uint32_t fill;
	PackCLJRDword(fill, gu_level, bv_level, ry_level, ry_level, ry_level, ry_level);

	while (height)
	{
		uint32_t* pdst = reinterpret_cast<uint32_t*>(pBuffer);
		int32_t hcount = width;
		while (hcount)
		{
			if (*pdst++ != fill)
				return false;

			hcount -= 4;
		}

		pBuffer += stride;
		height--;
	}

	return true;
}

bool BlipvertUnitTests::Check_Y800(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width;

	for (int32_t h = 0; h < height; h++)
	{
		uint8_t* pdst = pBuffer;
		for (int32_t w = 0; w < width; w++)
		{
			if (*pdst++ != ry_level) return false;
		}

		pBuffer += stride;
	}

	return true;
}

bool BlipvertUnitTests::Check_Y16(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width * 2;

	uint16_t fill = static_cast<uint16_t>(ry_level << 8);
	do
	{
		uint16_t* pdst = reinterpret_cast<uint16_t*>(pBuffer);
		int32_t hcount = width;
		do
		{
			if (*pdst++ != fill) return false;
		} while (--hcount);

		pBuffer += stride;
	} while (--height);

	return true;
}
bool BlipvertUnitTests::Check_AYUV(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_RGBA(ry_level, gu_level, bv_level, alpha, width, height, pBuffer, stride);
}

bool BlipvertUnitTests::Check_IYU1(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
	int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width * 12 / 8;

	for (int32_t h = 0; h < height; h++)
	{
		uint8_t* pdst = pBuffer;
		for (int32_t w = 0; w < width; w += 4)
		{
			if (pdst[0] != gu_level) return false;
			if (pdst[1] != ry_level) return false;
			if (pdst[2] != ry_level) return false;
			if (pdst[3] != bv_level) return false;
			if (pdst[4] != ry_level) return false;
			if (pdst[5] != ry_level) return false;
			pdst += 6;
		}

		pBuffer += stride;
	}

	return true;
}

bool BlipvertUnitTests::Check_IYU2(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha,
	int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width * 3;

	for (int32_t h = 0; h < height; h++)
	{
		uint8_t* pdst = pBuffer;
		for (int32_t w = 0; w < width; w++)
		{
			if (pdst[0] != gu_level) return false;
			if (pdst[1] != ry_level) return false;
			if (pdst[2] != bv_level) return false;
			pdst += 3;
		}

		pBuffer += stride;
	}

	return true;
}

bool BlipvertUnitTests::Check_RGBA(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width * 4;

	uint32_t fill = static_cast<uint32_t>((alpha << 24) | (ry_level << 16) | (gu_level << 8) | bv_level);
	do
	{
		uint32_t* pdst = reinterpret_cast<uint32_t*>(pBuffer);
		int32_t hcount = width;
		do
		{
			if (*pdst++ != fill)
				return false;
		} while (--hcount);

		pBuffer += stride;
	} while (--height);

	return true;
}

bool BlipvertUnitTests::Check_RGB32(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_RGBA(ry_level, gu_level, bv_level, 255, width, height, pBuffer, stride);
}


bool BlipvertUnitTests::Check_RGB24(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width * 3;

	do
	{
		uint8_t* pdst = pBuffer;
		int32_t hcount = width;
		do
		{
			if (*pdst++ != bv_level) return false;
			if (*pdst++ != gu_level) return false;
			if (*pdst++ != ry_level) return false;
		} while (--hcount);

		pBuffer += stride;
	} while (--height);

	return true;
}
bool BlipvertUnitTests::Check_RGB565(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width * 2;

	uint16_t fill;
	PackRGB565Word(fill, ry_level, gu_level, bv_level);

	do
	{
		uint16_t* pdst = reinterpret_cast<uint16_t*>(pBuffer);
		int32_t hcount = width;
		do
		{
			if (*pdst++ != fill) return false;
		} while (--hcount);

		pBuffer += stride;
	} while (--height);

	return true;
}

bool BlipvertUnitTests::Check_ARGB1555(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	if (!stride)
		stride = width * 2;

	uint16_t fill;
	PackARGB555Word(fill, alpha, ry_level, gu_level, bv_level);

	do
	{
		uint16_t* pdst = reinterpret_cast<uint16_t*>(pBuffer);
		int32_t hcount = width;
		do
		{
			if (*pdst++ != fill) return false;
		} while (--hcount);

		pBuffer += stride;
	} while (--height);

	return true;
}

bool BlipvertUnitTests::Check_RGB555(uint8_t ry_level, uint8_t gu_level, uint8_t bv_level, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
	return Check_ARGB1555(ry_level, gu_level, bv_level, 255, width, height, pBuffer, stride);
}