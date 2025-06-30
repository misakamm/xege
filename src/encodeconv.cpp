/**
 * @file  encodeconv.cpp
 * @brief 编码转换
 */
#include "ege_head.h"

#include "encodeconv.h"

#include <windows.h>

namespace ege
{

/**
 * @brief 将宽字符串转为多字节字符串（多字节字符编码由 setcodepage 确定）
 *
 * @param wStr 以终止字符结尾的宽字符串
 * @return std::string 转换后的字符串
 */
std::string w2mb(const wchar_t wStr[])
{
    unsigned int codepage = ege::getcodepage();
    int bufsize = WideCharToMultiByte(codepage, 0, wStr, -1, NULL, 0, 0, 0);
    std::string mbStr(bufsize, '\0');
    WideCharToMultiByte(codepage, 0, wStr, -1, &mbStr[0], bufsize, 0, 0);
    return mbStr;
}

/**
 * @brief 将多字节字符串转为宽字符串（多字节字符编码由 setcodepage 确定）
 *
 * @param mbStr 以终止字符结尾的多字节字符串
 * @return std::wstring 转换后的宽字符
 */
std::wstring mb2w(const char mbStr[])
{
    unsigned int codepage = ege::getcodepage();
    int bufsize = MultiByteToWideChar(codepage, 0, mbStr, -1, NULL, 0);
    std::wstring wStr(bufsize, L'\0');
    MultiByteToWideChar(codepage, 0, mbStr, -1, &wStr[0], bufsize);
    return wStr;
}

} // namespace ege
