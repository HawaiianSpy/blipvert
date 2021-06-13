#include "pch.h"
#include "CppUnitTest.h"

#include "Utilities.h"
#include "YUVtoRGB.h"
#include "RGBtoYUV.h"
#include "RGBtoRGB.h"
#include "YUVtoYUV.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;

namespace BlipvertUnitTests
{
	TEST_CLASS(PixelConversion_UnitTests)
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

						uint8_t sY;
						uint8_t sU;
						uint8_t sV;
						SlowRGBtoYUV(static_cast<uint8_t>(R), static_cast<uint8_t>(G), static_cast<uint8_t>(B), &sY, &sU, &sV);

						if (sY != fY || sU != fU || sV != fV)
						{
							uint8_t dy = delta(sY, fY);
							uint8_t du = delta(sU, fU);
							uint8_t dv = delta(sV, fV);
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

			Assert::IsTrue(maxdelta <= 1, L"RGBtoVUVPixel_UnitTest Maximum deviation from slow to fast calculations exceeds 1.");
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

						uint8_t sR;
						uint8_t sG;
						uint8_t sB;
						SlowYUVtoRGB(static_cast<uint8_t>(Y), static_cast<uint8_t>(U), static_cast<uint8_t>(V), &sR, &sG, &sB);

						if (sR != fR || sG != fG || sB != fB)
						{
							uint8_t dr = delta(sR, fR);
							uint8_t dg = delta(sG, fG);
							uint8_t db = delta(sB, fB);
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

			Assert::IsTrue(maxdelta <= 1, L"YUVtoRGBPixel_UnitTest Maximum deviation from slow to fast calculations exceeds 1.");
			Assert::AreEqual(static_cast<int>(1), static_cast<int>(maxdelta), L"YUVtoRGBPixel_UnitTest Maximum deviation from slow to fast calculations != 1.");
		}

	private:
		inline uint8_t delta(uint8_t s, uint8_t f)
		{
			return static_cast<uint8_t>(abs(static_cast<int16_t>(s) - static_cast<int16_t>(f)));
		}

	};
}