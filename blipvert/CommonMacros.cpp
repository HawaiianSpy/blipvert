#include "pch.h"
#include "framework.h"
#include "CommonMacros.h"

using namespace blipvert;

// 32-bit bitmasks

const uint32_t blipvert::RGB555_RED_MASK = 0x00007C00;
const uint32_t blipvert::RGB555_GREEN_MASK = 0x000003E0;
const uint32_t blipvert::RGB555_BLUE_MASK = 0x0000001F;

const uint32_t blipvert::RGB565_RED_MASK = 0x0000F800;
const uint32_t blipvert::RGB565_GREEN_MASK = 0x000007E0;
const uint32_t blipvert::RGB565_BLUE_MASK = 0x0000001F;

const uint32_t blipvert::RGB32_ALPHA_MASK = 0xFF000000;
const uint32_t blipvert::RGB32_RED_MASK = 0x00FF0000;
const uint32_t blipvert::RGB32_GREEN_MASK = 0x0000FF00;
const uint32_t blipvert::RGB32_BLUE_MASK = 0x000000FF;

const uint32_t blipvert::RGB32_BLUEX_MASK = 0x00FF0000;
const uint32_t blipvert::RGB32_REDX_MASK = 0x000000FF;
