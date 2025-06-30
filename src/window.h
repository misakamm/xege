#pragma once

#include <windows.h>
#include <windef.h>
#include <string>
#include <wchar.h>

namespace ege
{

HWND getParentWindow();

void getParentSize(int* width, int* height);

HWND createWindow(HWND parentWindow, const wchar_t* caption, DWORD style, DWORD exstyle, POINT pos, SIZE size);

BOOL init_instance(HINSTANCE hInstance);

ATOM register_classW(struct _graph_setting* pg, HINSTANCE hInstance);

} // namespace ege
