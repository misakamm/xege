#pragma once

#include <windef.h>

#include "ege/stdint.h"

#include "ege_def.h"
#include "ege_math.h"

// 交换颜色中 R 通道和 B 通道: 0xAARRGGBB -> 0xAABBGGRR
#define RGBTOBGR(color) ((color_t)((((color) & 0xFF) << 16) | (((color) & 0xFF0000) >> 16) | ((color) & 0xFF00FF00)))

// 将 color_t 与 Bitmap Buffer 所用的 0xAARRGGBB 格式转换为 COLORREF 的 0x00BBGGRR 格式
// 仅用于向 GDI32 API 传递颜色时
#define ARGBTOZBGR(c) ((color_t)((((c) & 0xFF) << 16) | (((c) & 0xFF0000) >> 16) | ((c) & 0xFF00)))

namespace ege
{

#ifndef EGE_COLOR_T_TYPEDEF
#define EGE_COLOR_T_TYPEDEF
typedef uint32_t color_t;
#endif

typedef struct COLORHSL
{
    float h;
    float s;
    float l;
} COLORHSL;

typedef struct COLORHSV
{
    float h;
    float s;
    float v;
} COLORHSV;

typedef struct COLORRGB
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} COLORRGB;

/**
 * @brief RGB 颜色混合，混合结果保留背景色透明通道
 *
 * @param dst   背景色(RGB)
 * @param src   前景色(RGB)
 * @param alpha 透明度(0~255)
 * @return      混合后的 RGB 颜色，透明度与背景色一致
 */
EGE_FORCEINLINE color_t colorblend_inline(color_t dst, color_t src, uint8_t alpha)
{
    uint8_t r = DIVIDE_255_FAST(255 * EGEGET_R(dst) + ((int)(EGEGET_R(src) - EGEGET_R(dst)) * alpha + 255/2));
    uint8_t g = DIVIDE_255_FAST(255 * EGEGET_G(dst) + ((int)(EGEGET_G(src) - EGEGET_G(dst)) * alpha + 255/2));
    uint8_t b = DIVIDE_255_FAST(255 * EGEGET_B(dst) + ((int)(EGEGET_B(src) - EGEGET_B(dst)) * alpha + 255/2));

    return EGEARGB(EGEGET_A(dst),r, g, b);
}

/**
 * @brief RGB 颜色混合，混合结果保留背景色透明通道
 *
 * @param dst   背景色(RGB)
 * @param src   前景色(RGB)
 * @param alpha 透明度(0~255)
 * @return      混合后的 RGB 颜色，透明度与背景色一致
 * @note        结果与标准公式相比有一定误差
 */
EGE_FORCEINLINE color_t colorblend_inline_fast(color_t dst, color_t src, uint8_t alpha)
{
#define COLORBLEND_INLINE_FAST_OPTION  1
#if COLORBLEND_INLINE_FAST_OPTION == 0
    // 误差较大，可能取不到端点，而且无近似取整
    uint32_t rb = dst & 0x00FF00FF;
    uint32_t g  = dst & 0x0000FF00;

    rb += ((src & 0x00FF00FF) - rb) * alpha >> 8;
    g  += ((src & 0x0000FF00) - g ) * alpha >> 8;
    return (rb & 0x00FF00FF) | (g & 0x0000FF00) | (dst & 0xFF000000);
#elif COLORBLEND_INLINE_FAST_OPTION == 1
    // 有近似取整，端点正常，误差较小
    uint32_t rb = dst & 0x00FF00FF;
    uint32_t g  = dst & 0x0000FF00;
    int alphaFactor = DIVIDE_255_FAST((alpha << 8) + 255/2);

    rb += (((src & 0x00FF00FF) - rb) * alphaFactor + 0x007F007F) >> 8;
    g  += (((src & 0x0000FF00) - g ) * alphaFactor + 0x00007F00) >> 8;
    return (dst & 0xFF000000) | (rb & 0x00FF00FF) | (g & 0x0000FF00);
#endif
#undef COLORBLEND_INLINE_FAST_OPTION
}

/**
 * @brief 将两个 ARGB 颜色以指定的 alpha 进行混合 (忽略前景色透明度)
 *
 * @param dst   背景色(ARGB)
 * @param src   前景色(ARGB)
 * @param alpha 透明度(0~255)
 * @return      混合后的 ARGB 颜色
 * @note 混合公式 (公式中 alpha 为 0.0~1.0):
 * A = A(dst) + alpha * （255    - A(dst));
 * R = R(dst) + alpha * （R(src) - R(dst));
 * G = G(dst) + alpha * （G(src) - G(dst));
 * B = B(dst) + alpha * （B(src) - B(dst));
 */
EGE_FORCEINLINE color_t alphablend_specify_inline(color_t dst, color_t src, uint8_t alpha)
{
    const uint8_t a = DIVIDE_255_FAST(255 * EGEGET_A(dst) + ((int)(          255 - EGEGET_A(dst)) * alpha + 255/2));
    const uint8_t r = DIVIDE_255_FAST(255 * EGEGET_R(dst) + ((int)(EGEGET_R(src) - EGEGET_R(dst)) * alpha + 255/2));
    const uint8_t g = DIVIDE_255_FAST(255 * EGEGET_G(dst) + ((int)(EGEGET_G(src) - EGEGET_G(dst)) * alpha + 255/2));
    const uint8_t b = DIVIDE_255_FAST(255 * EGEGET_B(dst) + ((int)(EGEGET_B(src) - EGEGET_B(dst)) * alpha + 255/2));

    return EGEARGB(a, r, g, b);
}

/**
 * @brief ARGB 颜色混合 (alpha = 前景色透明度)
 *
 * @param dst 背景色
 * @param src 前景色
 * @return    混合后的 ARGB 颜色
 */
EGE_FORCEINLINE color_t alphablend_inline(color_t dst, color_t src)
{
    return alphablend_specify_inline(dst, src, EGEGET_A(src));
}

/**
 * @brief  ARGB 颜色混合 (alpha = 前景色透明度 * 比例系数)
 *
 * @param dst 背景色(ARGB)
 * @param src 前景色(ARGB)
 * @param srcAlphaFactor 前景色的比例系数，0~255 对应 0.0~1.0
 * @return    混合后的 ARGB 颜色
 */
EGE_FORCEINLINE color_t alphablend_inline(color_t dst, color_t src, uint8_t srcAlphaFactor)
{
    uint8_t alpha = DIVIDE_255_FAST(EGEGET_A(src) * srcAlphaFactor + 255/2);
    return alphablend_specify_inline(dst, src, alpha);
}

/**
 * @brief 将两个预乘 alpha 的 ARGB 颜色进行混合
 *
 * @param dst   背景色(PARGB)
 * @param src   前景色(PARGB)
 * @return      混合后的 PARGB 颜色
 * @note 混合公式:
 * A = A(src) + (1.0 - alpha) * A(dst);;
 * R = R(src) + (1.0 - alpha) * R(dst);
 * G = G(src) + (1.0 - alpha) * G(dst);
 * B = B(src) + (1.0 - alpha) * B(dst);
 */
EGE_FORCEINLINE color_t alphablend_premultiplied_inline(color_t dst, color_t src)
{
    const uint8_t a = DIVIDE_255_FAST(255 * EGEGET_A(src) + (255 - EGEGET_A(src)) * EGEGET_A(dst));
    const uint8_t r = DIVIDE_255_FAST(255 * EGEGET_R(src) + (255 - EGEGET_A(src)) * EGEGET_R(dst));
    const uint8_t g = DIVIDE_255_FAST(255 * EGEGET_G(src) + (255 - EGEGET_A(src)) * EGEGET_G(dst));
    const uint8_t b = DIVIDE_255_FAST(255 * EGEGET_B(src) + (255 - EGEGET_A(src)) * EGEGET_B(dst));

    return EGEARGB(a, r, g, b);
}

} //namespace ege


