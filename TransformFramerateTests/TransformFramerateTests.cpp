
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

//
// This console application runs a frames per second speed test on the transforms.
// The output goes to the console.
//


#include <iostream>
#include <memory>
#include <chrono>
#include <vector>

#include "blipvert.h"
#include "Utilities.h"
#include "YUVtoRGB.h"
#include "ToFillColor.h"

using namespace std;
using namespace blipvert;

uint32_t width = 1920;
uint32_t height = 1080;
int32_t numframes = 60;

vector<const MediaFormatID*> RGBFormats = {
	&MVFMT_RGBA,
	&MVFMT_RGB32,
	&MVFMT_RGB24,
	&MVFMT_RGB565,
	&MVFMT_RGB555
};

vector<const MediaFormatID*> YUVFormats = {
	&MVFMT_AYUV,
	&MVFMT_YUY2,
	&MVFMT_UYVY,
	&MVFMT_YVYU,
	&MVFMT_VYUY,
	&MVFMT_Y42T,
	&MVFMT_Y800,
	&MVFMT_Y16,
	&MVFMT_IYU1,
	&MVFMT_IYU2,
	&MVFMT_Y41P,
	&MVFMT_CLJR,
	&MVFMT_I420,
	&MVFMT_YV12,
	&MVFMT_YUV9,
	&MVFMT_YVU9,
	&MVFMT_IMC1,
	&MVFMT_IMC2,
	&MVFMT_IMC3,
	&MVFMT_IMC4,
	&MVFMT_NV12
};

void FramerateTest(const MediaFormatID& in_format, const MediaFormatID& out_format, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	t_transformfunc encodeTransPtr = FindVideoTransform(in_format, out_format);
	if (encodeTransPtr == nullptr)
	{
		std::cout << "Framerate test: " << in_format << " to " << out_format << " aborted: no transform of that type available." << std::endl;
		return;
	}

	t_fillcolorfunc fillBufFunctPtr = FindFillColorTransform(in_format);
	if (fillBufFunctPtr == nullptr)
	{
		std::cout << "Framerate test: " << in_format << " to " << out_format << " aborted: No transform for " << in_format <<" is available." << std::endl;
		return;
	}

	uint32_t inBufBize = CalculateBufferSize(in_format, width, height);
	uint32_t outBufBize = CalculateBufferSize(out_format, width, height);

	std::unique_ptr<uint8_t[]> inBuf(new uint8_t[inBufBize]);
	uint8_t* inBufPtr = inBuf.get();
	memset(inBufPtr, 0, inBufBize);

	std::unique_ptr<uint8_t[]> outBuf(new uint8_t[outBufBize]);
	uint8_t* outBufPtr = outBuf.get();
	memset(outBufPtr, 0, outBufBize);

	if (IsYUVColorspace(in_format))
	{
		uint8_t Y;
		uint8_t U;
		uint8_t V;
		FastRGBtoYUV(red, green, blue, &Y, &U, &V);
		fillBufFunctPtr(Y, U, V, alpha, width, height, inBufPtr, 0);
	}
	else
	{
		fillBufFunctPtr(red, green, blue, alpha, width, height, inBufPtr, 0);
	}

	std::cout << "Framerate test: " << in_format << " to " << out_format;

	auto start = chrono::steady_clock::now();

	for (int count = 0; count < numframes; count++)
	{
		encodeTransPtr(width, height, outBufPtr, 0, inBufPtr, 0, false, nullptr);
	}

	auto end = chrono::steady_clock::now();

	double ms = static_cast<double>(chrono::duration_cast<chrono::milliseconds>(end - start).count());
	double framerate = 1000.0 / (ms / static_cast<double>(numframes));
	std::cout << " @ " << framerate << " fps."<< std::endl;
}

void RunTest(const MediaFormatID& in_format, const MediaFormatID& out_format)
{
	if (IsYUVColorspace(in_format))
	{
		FramerateTest(in_format, out_format, 128, 128, 128, 255);
	}
	else if (IsRGBColorspace(in_format))
	{
		FramerateTest(in_format, out_format, 128, 128, 128, 255);
	}
}

void RunAllTransforms()
{
	cout << "Transform test run for frame resolution: " << width << " x " << height << "..." << endl << endl;

	// RGB to YUV
	for (const MediaFormatID* in_format : RGBFormats)
	{
		for (const MediaFormatID* out_format : YUVFormats)
		{
			RunTest(*in_format, *out_format);
		}
	}

	// YUV to RGB
	for (const MediaFormatID* in_format : YUVFormats)
	{
		for (const MediaFormatID* out_format : RGBFormats)
		{
			RunTest(*in_format, *out_format);
		}
	}

	// RGB to RGB
	for (const MediaFormatID* in_format : RGBFormats)
	{
		for (const MediaFormatID* out_format : RGBFormats)
		{
			if (*in_format != *out_format)
			{
				RunTest(*in_format, *out_format);
			}
		}
	}

	// YUV to YUV
	for (const MediaFormatID* in_format : YUVFormats)
	{
		for (const MediaFormatID* out_format : YUVFormats)
		{
			if (*in_format != *out_format)
			{
				RunTest(*in_format, *out_format);
			}
		}
	}

	cout << endl << endl;
}

int main()
{
	InitializeLibrary();

	bool save = get_UseFasterLooping();
	set_UseFasterLooping(true);

	width = 1920;
	height = 1080;
	RunAllTransforms();

	width = 4096;
	height = 2160;
	RunAllTransforms();

	set_UseFasterLooping(save);
}
