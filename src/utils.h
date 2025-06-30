#pragma once

// 交换 a 和 b 的值(t 为临时变量)
#ifndef SWAP
#define SWAP(a, b, t) \
    {                    \
        t = a;         \
        a = b;         \
        b = t;         \
    }
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif


namespace ege
{

} // namespace ege
