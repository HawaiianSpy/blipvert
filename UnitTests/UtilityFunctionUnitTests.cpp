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
#include "CppUnitTest.h"

#include "blipvert.h"
#include "Utilities.h"
#include "YUVtoRGB.h"
#include "RGBtoYUV.h"
#include "RGBtoRGB.h"
#include "YUVtoYUV.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;

namespace BlipvertUnitTests
{
	TEST_CLASS(Utilities_UnitTests)
	{
	public:

		TEST_METHOD(RGBtoVUVPixel_UnitTest)
		{
			uint8_t maxdelta = 0;
			for (uint16_t R = 0; R < 256; R++)
			{
				for (uint16_t G = 0; G < 256; G++)
				{
					for (uint16_t B = 0; B < 256; B++)
					{
						uint8_t fY;
						uint8_t fU;
						uint8_t fV;
						FastRGBtoYUV(static_cast<uint8_t>(R), static_cast<uint8_t>(G), static_cast<uint8_t>(B), &fY, &fU, &fV);

						double sY;
						double sU;
						double sV;
						SlowRGBtoYUV(R, G, B, &sY, &sU, &sV);

						if (sY != fY || sU != fU || sV != fV)
						{
							uint8_t dy = delta(static_cast<uint8_t>(sY), fY);
							uint8_t du = delta(static_cast<uint8_t>(sU), fU);
							uint8_t dv = delta(static_cast<uint8_t>(sV), fV);
							if (dy > maxdelta)
							{
								maxdelta = dy;
							}
							if (du > maxdelta)
							{
								maxdelta = du;
							}
							if (dv > maxdelta)
							{
								maxdelta = dv;
							}
						}
					}
				}
			}

			Assert::AreEqual(static_cast<int>(1), static_cast<int>(maxdelta), L"RGBtoVUVPixel_UnitTest Maximum deviation from slow to fast calculations != 1.");
		}

		TEST_METHOD(YUVtoRGBPixel_UnitTest)
		{
			uint8_t maxdelta = 0;
			for (uint16_t Y = 0; Y < 256; Y++)
			{
				for (uint16_t U = 0; U < 256; U++)
				{
					for (uint16_t V = 0; V < 256; V++)
					{
						uint8_t fR;
						uint8_t fG;
						uint8_t fB;
						FastYUVtoRGB(static_cast<uint8_t>(Y), static_cast<uint8_t>(U), static_cast<uint8_t>(V), &fR, &fG, &fB);

						double sR;
						double sG;
						double sB;
						SlowYUVtoRGB(Y, U, V, &sR, &sG, &sB);

						if (sR != fR || sG != fG || sB != fB)
						{
							uint8_t dr = delta(static_cast<uint8_t>(sR), fR);
							uint8_t dg = delta(static_cast<uint8_t>(sG), fG);
							uint8_t db = delta(static_cast<uint8_t>(sB), fB);
							if (dr > maxdelta)
							{
								maxdelta = dr;
							}
							if (dg > maxdelta)
							{
								maxdelta = dg;
							}
							if (db > maxdelta)
							{
								maxdelta = db;
							}
						}
					}
				}
			}

			Assert::AreEqual(static_cast<int>(1), static_cast<int>(maxdelta), L"YUVtoRGBPixel_UnitTest Maximum deviation from slow to fast calculations != 1.");
		}

		TEST_METHOD(CalculateBufferSize_UnitTest)
		{
			uint32_t size = CalculateBufferSize(MVFMT_UYVY, 2592, 1944);
			Assert::AreEqual(static_cast<uint32_t>(10077696), size, L"Unexpected size returned for buffer calculation.");

			size = CalculateBufferSize(MVFMT_UYVY, 2592, 1944, 5500);
			Assert::AreEqual(static_cast<uint32_t>(5500 * 1944), size, L"Unexpected size returned for buffer calculation.");

			bool savelooping = get_UseFasterLooping();

			set_UseFasterLooping(false);
			size = CalculateBufferSize(MVFMT_RGB24, 12, 12);
			Assert::AreEqual(static_cast<uint32_t>(432), size, L"Unexpected size returned for buffer calculation.");

			set_UseFasterLooping(true);
			size = CalculateBufferSize(MVFMT_RGB24, 12, 12);
			Assert::AreEqual(static_cast<uint32_t>(433), size, L"Unexpected size returned for buffer calculation.");

			set_UseFasterLooping(savelooping);

			size = CalculateBufferSize(MVFMT_IYU1, 12, 12);
			Assert::AreEqual(static_cast<uint32_t>(216), size, L"Unexpected size returned for buffer calculation.");
		}

	private:
		inline uint8_t delta(uint8_t s, uint8_t f)
		{
			return static_cast<uint8_t>(abs(static_cast<int16_t>(s) - static_cast<int16_t>(f)));
		}

	};
}