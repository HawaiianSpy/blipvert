#include "pch.h"
#include "CppUnitTest.h"

#include "blipvert.h"
#include "YUVtoRGB.h"
#include "RGBtoYUV.h"
#include "RGBtoRGB.h"
#include "YUVtoYUV.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace blipvert;

namespace BlipvertUnitTests
{
	TEST_MODULE_INITIALIZE(FindVideoTransform_ModuleInit)
	{
		InitializeLibrary();
	}

	TEST_CLASS(FindVideoTransform_UnitTests)
	{
	public:

		TEST_METHOD(FindValidTransform_UnitTest)
		{
			t_transformfunc func = FindVideoTransform(MVFMT_UYVY, MVFMT_RGB32);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindValidTransform_UnitTest returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(UYVY_to_RGB32), reinterpret_cast<void*>(func), L"FindValidTransform_UnitTest returned the wrong function pointer.");

			func = FindVideoTransform(MVFMT_cyuv, MVFMT_Y411);
			Assert::IsNotNull(reinterpret_cast<void*>(func), L"FindValidTransform_UnitTest returned a null function pointer.");
			Assert::AreEqual(reinterpret_cast<void*>(UYVY_to_IYU1), reinterpret_cast<void*>(func), L"FindValidTransform_UnitTest returned the wrong function pointer.");
		}

		TEST_METHOD(FindInvalidTransform_UnitTest)
		{
			t_transformfunc func = FindVideoTransform(MVFMT_UYVY, MVFMT_Plum);
			Assert::IsNull(reinterpret_cast<void*>(func), L"FindInvalidTransform_UnitTest returned a non-null function pointer.");
		}
	};
}
