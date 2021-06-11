#pragma once

namespace blipvert
{
    // 32-bit bitmasks

    extern const uint32_t RGB555_RED_MASK;
    extern const uint32_t RGB555_GREEN_MASK;
    extern const uint32_t RGB555_BLUE_MASK;

    extern const uint32_t RGB565_RED_MASK;
    extern const uint32_t RGB565_GREEN_MASK;
    extern const uint32_t RGB565_BLUE_MASK;

    extern const uint32_t RGB32_ALPHA_MASK;
    extern const uint32_t RGB32_RED_MASK;
    extern const uint32_t RGB32_GREEN_MASK;
    extern const uint32_t RGB32_BLUE_MASK;

    extern const uint32_t RGB32_BLUEX_MASK;
    extern const uint32_t RGB32_REDX_MASK;


#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
                (static_cast<uint32_t>(static_cast<uint8_t>(ch0)) | static_cast<uint32_t>(static_cast<uint8_t>(ch1) << 8) | \
                static_cast<uint32_t>(static_cast<uint8_t>(ch2) << 16) | static_cast<uint32_t>(static_cast<uint8_t>(ch3) << 24))


#define PackRGB565Word(dest, r, g, b) (dest = static_cast<uint16_t>((((b) & 0xF8) >> 3) | \
                                                                (((g) & 0xFC) << 3) | \
                                                                (((r) & 0xF8) << 8)));

#define PackRGB555Word(dest, r, g, b) (dest = static_cast<uint16_t>(0x8000 | \
                                                                (((b) & 0xF8) >> 3) | \
                                                                (((g) & 0xF8) << 2) | \
                                                                (((r) & 0xF8) << 7)));

#define UnpackRGB565Red(src) ((src & RGB565_RED_MASK) >> 8)
#define UnpackRGB565Green(src) ((src & RGB565_GREEN_MASK) >> 3)
#define UnpackRGB565Blue(src) ((src & RGB565_BLUE_MASK) << 3)

#define UnpackRGB555Red(src) ((src & RGB555_RED_MASK) >> 7)
#define UnpackRGB555Green(src) ((src & RGB555_GREEN_MASK) >> 2)
#define UnpackRGB555Blue(src) ((src & RGB555_BLUE_MASK) << 3)

#define UnpackRGB565Word(src, r, g, b)    r = static_cast<BYTE>(UnpackRGB565Red(src));\
                                        g = static_cast<BYTE>(UnpackRGB565Green(src));\
                                        b = static_cast<BYTE>(UnpackRGB565Blue(src));

#define UnpackRGB555Word(src, r, g, b)    r = static_cast<BYTE>(UnpackRGB555Red(src));\
                                        g = static_cast<BYTE>(UnpackRGB555Green(src));\
                                        b = static_cast<BYTE>(UnpackRGB555Blue(src));

#define PackCLJRDword(dest, u, v, y0, y1, y2, y3) (dest = static_cast<int64_t>(\
                                                    (u & 0xFC) >> 2) | ((v & 0xFC) << 4) | \
                                                    ((y0 & 0xF8) << 9) | ((y1 & 0xF8) << 14)| \
                                                    ((y2 & 0xF8) << 19) | ((y3 & 0xF8) << 24));
#define UnpackCLJR_U(src) ((src & 0x0000003F) << 2)
#define UnpackCLJR_V(src) ((src & 0x00000FC0) >> 4)
#define UnpackCLJR_Y0(src) ((src & 0x0001F000) >> 9)
#define UnpackCLJR_Y1(src) ((src & 0x003E0000) >> 14)
#define UnpackCLJR_Y2(src) ((src & 0x07C00000) >> 19)
#define UnpackCLJR_Y3(src) ((src & 0xF8000000) >> 24)

}