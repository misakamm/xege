#pragma once

#include <cmath>

namespace ege
{

// MSVC 在 C++11 中支持 round
#if defined(_MSC_VER)
#if __cplusplus < 201103L
inline int round(double x)
{
    return (int)((x > 0.0) ? (x + 0.5) : (x - 0.5));
}

inline int round(float  x)
{
    return (int)((x > 0.0f) ? (x + 0.5f) : (x - 0.5f));
}

inline int positiveRound(double x)
{
    return (int)(x + 0.5);
}

inline int positiveRound(float  x)
{
    return (int)(x + 0.5f);
}
#else
using std::round;
#endif  // __cplusplus < 201103L

#else
// round() 函数在低版本 GCC 中可用 (如 2005-11-30 的 GCC 3.4.5)

#endif

// 快除 255，有效范围：[0, 65790)
#define DIVIDE_255_FAST(x)  (((x) + (((x) + 257) >> 8)) >> 8)

template<typename T>
T clamp(T value, T min, T max)
{
    return (value < min) ? min : ((value > max) ? max : value);
}

// Overflow 判断
inline bool sumIsOverflow(int a, int b)
{
    return (a > 0) && (b > 0) && (b > (INT_MAX - a));
}

// Underflow 判断
inline bool sumIsUnderflow(int a, int b)
{
    return (a < 0) && (b < 0) && (b < (INT_MIN - a));
}

}  // namespace ege
