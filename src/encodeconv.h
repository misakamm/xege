/**
 * @file  encodeconv.h
 * @brief 编码转换
 */

#pragma once

#include <string>

namespace ege
{

// convert wide char string to multibyte string, using ege::getcodepage
std::string w2mb(const wchar_t wStr[]);

// convert multibyte string to wide char string, using ege::getcodepage
std::wstring mb2w(const char mbStr[]);

}
