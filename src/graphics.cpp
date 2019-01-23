/*
* EGE (Easy Graphics Engine)
* FileName      graphics.cpp
* HomePage1     http://misakamm.github.com/xege
* HomePage2     http://misakamm.bitbucket.org/index.htm
* teiba1        http://tieba.baidu.com/f?kw=ege
* teiba2        http://tieba.baidu.com/f?kw=ege%C4%EF
* Blog:         http://misakamm.com
* E-Mail:       mailto:misakamm[at gmail com]

编译说明：编译为动态库时，需要定义 PNG_BULIDDLL，以导出dll函数

本图形库创建时间2010 0916

本文件定义平台密切相关的操作及接口
*/

//编译器版本，目前仅支持vc6/vc2008/vc2010/vc2012/mingw
#ifdef _WIN64
#	define SYSBITS TEXT("x64")
#else
#	define SYSBITS TEXT("x86")
#endif

#ifdef _MSC_VER
#	if (_MSC_VER >= 2000)
#		define COMPILER_VER TEXT("VC201x") SYSBITS
#	elif (_MSC_VER >= 1910)
#		define COMPILER_VER TEXT("VC2017") SYSBITS
#	elif (_MSC_VER >= 1900)
#		define COMPILER_VER TEXT("VC2015") SYSBITS
#	elif (_MSC_VER >= 1800)
#		define COMPILER_VER TEXT("VC2013") SYSBITS
#	elif (_MSC_VER >= 1700)
#		define COMPILER_VER TEXT("VC2012") SYSBITS
#	elif (_MSC_VER >= 1600)
#		define COMPILER_VER TEXT("VC2010") SYSBITS
#	elif (_MSC_VER >= 1500)
#		define COMPILER_VER TEXT("VC2008") SYSBITS
#	elif (_MSC_VER > 1200)
#		define COMPILER_VER TEXT("VC2005") SYSBITS
#	else
#		define COMPILER_VER TEXT("VC6") SYSBITS
#	endif
#else
#	define TOSTRING_(x) #x
#	define TOSTRING(x) TOSTRING_(x)
#	define GCC_VER TEXT(TOSTRING(__GNUC__)) TEXT(".") TEXT(TOSTRING(__GNUC_MINOR__))
#	define COMPILER_VER TEXT("GCC") GCC_VER SYSBITS
#endif

#define EGE_TITLE TEXT("EGE15.04 ") COMPILER_VER

#ifndef _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#endif
#ifndef _ALLOW_RUNTIME_LIBRARY_MISMATCH
#define _ALLOW_RUNTIME_LIBRARY_MISMATCH
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "ege_head.h"

#ifdef _ITERATOR_DEBUG_LEVEL
#undef _ITERATOR_DEBUG_LEVEL
#endif


#ifdef _MSC_VER
#pragma warning(disable: 4100 4127 4706)
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Winmm.lib")
#ifdef EGE_GDIPLUS
#if _MSC_VER > 1200
#pragma comment(lib, "gdiplus.lib")
#endif
#endif
#endif

namespace ege {

struct _graph_setting& graph_setting = *(struct _graph_setting*)malloc(sizeof(struct _graph_setting));

static bool     g_has_init = false;
static DWORD    g_windowstyle = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_CLIPCHILDREN|WS_VISIBLE;
static DWORD    g_windowexstyle = WS_EX_LEFT|WS_EX_LTRREADING;
static int      g_windowpos_x = CW_USEDEFAULT;
static int      g_windowpos_y = CW_USEDEFAULT;
static int      g_initoption  = INIT_DEFAULT, g_initcall = 0;
static HWND     g_attach_hwnd = 0;

#ifdef __cplusplus
extern "C" {
#endif
char* getlogodata();
unsigned long getlogodatasize();
#ifdef __cplusplus
}
#endif

float _GetFPS(int add);
DWORD WINAPI messageloopthread(LPVOID lpParameter);


/*private function*/
static
void
ui_msg_process(EGEMSG& qmsg) {
	struct _graph_setting * pg = &graph_setting;
	if ( (qmsg.flag & 1) )
		return;
	qmsg.flag |= 1;
	if (qmsg.message >= WM_KEYFIRST && qmsg.message <= WM_KEYLAST) {
		if (qmsg.message == WM_KEYDOWN) {
			pg->egectrl_root->keymsgdown((unsigned)qmsg.wParam, 0); // 以后补加flag
		} else if (qmsg.message == WM_KEYUP) {
			pg->egectrl_root->keymsgup((unsigned)qmsg.wParam, 0); // 以后补加flag
		} else if (qmsg.message == WM_CHAR) {
			pg->egectrl_root->keymsgchar((unsigned)qmsg.wParam, 0); // 以后补加flag
		}
	} else if (qmsg.message >= WM_MOUSEFIRST && qmsg.message <= WM_MOUSELAST) {
		int x = (short int)((UINT)qmsg.lParam & 0xFFFF), y = (short int)((UINT)qmsg.lParam >> 16);
		if (qmsg.message == WM_LBUTTONDOWN) {
			pg->egectrl_root->mouse(x, y, mouse_msg_down | mouse_flag_left);
		} else if (qmsg.message == WM_LBUTTONUP) {
			pg->egectrl_root->mouse(x, y, mouse_msg_up | mouse_flag_left);
		} else if (qmsg.message == WM_RBUTTONDOWN) {
			pg->egectrl_root->mouse(x, y, mouse_msg_down | mouse_flag_right);
		} else if (qmsg.message == WM_RBUTTONUP) {
			pg->egectrl_root->mouse(x, y, mouse_msg_up | mouse_flag_right);
		} else if (qmsg.message == WM_MOUSEMOVE) {
			int flag = 0;
			if (pg->keystatemap[VK_LBUTTON]) flag |= mouse_flag_left;
			if (pg->keystatemap[VK_RBUTTON]) flag |= mouse_flag_right;
			pg->egectrl_root->mouse(x, y, mouse_msg_move | flag);
		}
	}
}

/*private function*/
static
int
redraw_window(_graph_setting* pg, HDC dc) {
	int page = pg->visual_page;
	HDC hDC = pg->img_page[page]->m_hDC;
	int left = pg->img_page[page]->m_vpt.left, top = pg->img_page[page]->m_vpt.top;
	//HRGN rgn = pg->img_page[page]->m_rgn;

	BitBlt(dc, 0, 0, pg->base_w, pg->base_h, hDC, pg->base_x - left, pg->base_y - top, SRCCOPY);

	pg->update_mark_count = UPDATE_MAX_CALL;
	return 0;
}

/*private function*/
static
int
graphupdate(_graph_setting* pg) {
	if (pg->exit_window) {
		return grNoInitGraph;
	}
	{
		HDC hdc;
		if( IsWindowVisible(pg->hwnd)) {
			hdc = pg->window_dc;

			if (hdc == NULL) {
				return grNullPointer;
			}
			redraw_window(pg, hdc);
		} else {
			pg->update_mark_count = UPDATE_MAX_CALL;
		}
		_GetFPS(0x100);
		{
			RECT rect, crect;
			HWND hwnd;
			int _dw, _dh;
			GetClientRect(pg->hwnd, &crect);
			GetWindowRect(pg->hwnd, &rect);
			int w = pg->dc_w, h = pg->dc_h;
			_dw = w - (crect.right - crect.left);
			_dh = h - (crect.bottom - crect.top);
			if (_dw != 0 || _dh != 0) {
				hwnd = ::GetParent(pg->hwnd);
				if (hwnd) {
					POINT pt = {0, 0};
					ClientToScreen(hwnd, &pt);
					rect.left   -= pt.x;
					rect.top    -= pt.y;
					rect.right  -= pt.x;
					rect.bottom -= pt.y;
				}
				SetWindowPos(pg->hwnd, NULL, 0, 0,
						rect.right  + _dw - rect.left,
						rect.bottom + _dh - rect.top,
						SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
			}
		}

		return grOk;
	}
}

/*private function*/
int
dealmessage(_graph_setting* pg, bool force_update) {
	if (force_update || pg->update_mark_count <= 0) {
		graphupdate(pg);
	}
	return !pg->exit_window;
}

/*private function*/
void
guiupdate(_graph_setting* pg, egeControlBase* &root) {
	pg->msgkey_queue->process(ui_msg_process);
	pg->msgmouse_queue->process(ui_msg_process);
	root->update();
}

/*private function*/
static
int
waitdealmessage(_graph_setting* pg) {
	//MSG msg;
	if (pg->update_mark_count < UPDATE_MAX_CALL) {
		egeControlBase* &root = pg->egectrl_root;
		root->draw(NULL);

		graphupdate(pg);
		guiupdate(pg, root);
	}
	ege_sleep(1);
	return !pg->exit_window;
}

/*private function*/
static
int
_getkey(_graph_setting* pg) {
	EGEMSG msg;

	while (pg->msgkey_queue->pop(msg)) {
		if (msg.message == WM_CHAR) {
			return (KEYMSG_CHAR | ((int)msg.wParam & 0xFFFF));
		} else if (msg.message == WM_KEYDOWN) {
			return (KEYMSG_DOWN | ((int)msg.wParam & 0xFFFF) | (msg.lParam & 0x40000000 ? 0: KEYMSG_FIRSTDOWN));
		} else if (msg.message == WM_KEYUP) {
			return (KEYMSG_UP   | ((int)msg.wParam & 0xFFFF));
		}
	}
	return 0;
}

/*private function*/
static
int
peekkey(_graph_setting* pg) {
	EGEMSG msg;

	while (pg->msgkey_queue->pop(msg)) {
		if (msg.message == WM_CHAR || msg.message == WM_KEYDOWN) {
			if (msg.message == WM_KEYDOWN) {
				if (msg.wParam <= key_space
					|| msg.wParam >= key_0 && msg.wParam < key_f1
					|| msg.wParam >= key_semicolon && msg.wParam <= key_quote)
					continue;
			}
			pg->msgkey_queue->unpop();
			if (msg.message == WM_CHAR) {
				return (KEYMSG_CHAR | ((int)msg.wParam & 0xFFFF));
			} else if (msg.message == WM_KEYDOWN) {
				if (msg.wParam >= 0x70 && msg.wParam < 0x80) {
					return (KEYMSG_DOWN | ((int)msg.wParam + 0x100));
				}
				return (KEYMSG_DOWN | ((int)msg.wParam & 0xFFFF));
			} else if (msg.message == WM_KEYUP) {
				return (KEYMSG_UP   | ((int)msg.wParam & 0xFFFF));
			}
		}
	}
	return 0;
}

/*private function*/
static
int
peekallkey(_graph_setting* pg, int flag) {
	EGEMSG msg;

	while (pg->msgkey_queue->pop(msg)) {
		if (
			(msg.message == WM_CHAR && (flag & KEYMSG_CHAR_FLAG)) ||
			(msg.message == WM_KEYUP && (flag & KEYMSG_UP_FLAG) ) ||
			(msg.message == WM_KEYDOWN && (flag&KEYMSG_DOWN_FLAG) ))
		{
			pg->msgkey_queue->unpop();
			if (msg.message == WM_CHAR) {
				return (KEYMSG_CHAR | ((int)msg.wParam & 0xFFFF));
			} else if (msg.message == WM_KEYDOWN) {
				return (KEYMSG_DOWN | ((int)msg.wParam & 0xFFFF) | (msg.lParam & 0x40000000 ? 0: KEYMSG_FIRSTDOWN));
			} else if (msg.message == WM_KEYUP) {
				return (KEYMSG_UP   | ((int)msg.wParam & 0xFFFF));
			}
		}
	}
	return 0;
}

int
getflush() {
	struct _graph_setting * pg = &graph_setting;
	EGEMSG msg;
	int lastkey = 0;
	if (pg->msgkey_queue->empty()) {
		dealmessage(pg, NORMAL_UPDATE);
	}
	if (! pg->msgkey_queue->empty()) {
		while (pg->msgkey_queue->pop(msg)) {
			if (msg.message == WM_CHAR) {
				if (msg.message == WM_CHAR) {
					lastkey = (int)msg.wParam;
				}
			}
		}
	}
	return lastkey;
}

int
kbmsg() {
	struct _graph_setting * pg = &graph_setting;
	if (pg->exit_window)
		return grNoInitGraph;
	return peekallkey(pg, 1);
}

int
kbhitEx(int flag) {
	struct _graph_setting * pg = &graph_setting;
	if (pg->exit_window)
		return grNoInitGraph;
	if (flag == 0) {
		return peekkey(pg);
	} else {
		return peekallkey(pg, flag);
	}
}

int
getchEx(int flag) {
	struct _graph_setting * pg = &graph_setting;
	if (pg->exit_window)
		return grNoInitGraph;

	{
		int key;
		EGEMSG msg;
		DWORD dw = GetTickCount();
		do {
			key = kbhitEx(flag);
			if (key < 0)
				break;
			if (key > 0) {
				key = _getkey(pg);
				if (key) {
					msg = pg->msgkey_queue->last();
					if (dw < msg.time + 1000) {
						int ogn_key = key;
						int ret = 0;
						key &= 0xFFFF;
						ret = key;
						if (flag) {
							ret = ogn_key;
						} else {
							if ((ogn_key & KEYMSG_DOWN) && (msg.wParam >= 0x70 && msg.wParam < 0x80 || msg.wParam > ' ' && msg.wParam < '0')) {
								ret |= 0x100;
							}
						}
						return ret;
					}
				}
			}
		}while (!pg->exit_window && !pg->exit_flag && waitdealmessage(pg));
	}
	return 0;
}

int
kbhit() {
	return kbhitEx(0);
}

int
getch() {
	return getchEx(0);
}

key_msg
getkey() {
	struct _graph_setting * pg = &graph_setting;
	key_msg ret = {0};
	if (pg->exit_window)
		return ret;

	{
		int key = 0;
		do {
			key = _getkey(pg);
			if (key) {
				key_msg msg = {0};
				if (key & KEYMSG_DOWN)
					msg.msg = key_msg_down;
				else if (key & KEYMSG_UP)
					msg.msg = key_msg_up;
				else if (key & KEYMSG_CHAR)
					msg.msg = key_msg_char;
				msg.key = key & 0xFFFF;
				if (keystate(VK_CONTROL)) msg.flags |= key_flag_ctrl;
				if (keystate(VK_SHIFT)) msg.flags |= key_flag_shift;
				return msg;
			}
		}while (!pg->exit_window && !pg->exit_flag && waitdealmessage(pg));
	}
	return ret;
}

void
flushkey() {
	struct _graph_setting * pg = &graph_setting;
	EGEMSG msg;
	if (pg->msgkey_queue->empty()) {
		dealmessage(pg, NORMAL_UPDATE);
	}
	if (! pg->msgkey_queue->empty()) {
		while (pg->msgkey_queue->pop(msg)) {
			;
		}
	}
	return ;
}

int
keystate(int key) {
	struct _graph_setting * pg = &graph_setting;
	if (key < 0 || key >= MAX_KEY_VCODE) {
		return -1;
	}
	SHORT s = GetKeyState(key);
	if ( ((USHORT)s & 0x8000) == 0 ) {
		pg->keystatemap[key] = 0;
	}
	return pg->keystatemap[key];
}

/*private function*/
static
EGEMSG
_getmouse(_graph_setting* pg) {
	EGEMSG msg = {0};

	while (pg->msgmouse_queue->pop(msg)) {
		return msg;
	}
	return msg;
}

/*private function*/
static
EGEMSG
peekmouse(_graph_setting* pg) {
	EGEMSG msg = {0};

	if (pg->msgmouse_queue->empty()) {
		dealmessage(pg, NORMAL_UPDATE);
	}
	while (pg->msgmouse_queue->pop(msg)) {
		pg->msgmouse_queue->unpop();
		return msg;
	}
	return msg;
}

void
flushmouse() {
	struct _graph_setting * pg = &graph_setting;
	EGEMSG msg;
	if (pg->msgmouse_queue->empty()) {
		dealmessage(pg, NORMAL_UPDATE);
	}
	if (! pg->msgmouse_queue->empty()) {
		while (pg->msgmouse_queue->pop(msg)) {
			;
		}
	}
	return ;
}

int
mousemsg() {
	struct _graph_setting * pg = &graph_setting;
	if (pg->exit_window)
		return 0;
	EGEMSG msg;
	msg = peekmouse(pg);
	if (msg.hwnd) return 1;
	return 0;
}

mouse_msg
getmouse() {
	struct _graph_setting * pg = &graph_setting;
	mouse_msg mmsg = {0};
	if (pg->exit_window)
		return mmsg;

	{
		EGEMSG msg;
		do {
			msg = _getmouse(pg);
			if (msg.hwnd) {
				mmsg.flags |= ((msg.wParam & MK_CONTROL) != 0 ? mouse_flag_ctrl : 0);
				mmsg.flags |= ((msg.wParam & MK_SHIFT) != 0 ? mouse_flag_shift : 0);
				mmsg.x = (short)((int)msg.lParam & 0xFFFF);
				mmsg.y = (short)((unsigned)msg.lParam >> 16);
				mmsg.msg = mouse_msg_move;
				if (msg.message == WM_LBUTTONDOWN) {
					mmsg.msg = mouse_msg_down;
					mmsg.flags |= mouse_flag_left;
				} else if (msg.message == WM_RBUTTONDOWN) {
					mmsg.msg = mouse_msg_down;
					mmsg.flags |= mouse_flag_right;
				} else if (msg.message == WM_MBUTTONDOWN) {
					mmsg.msg = mouse_msg_down;
					mmsg.flags |= mouse_flag_mid;
				} else if (msg.message == WM_LBUTTONUP) {
					mmsg.msg = mouse_msg_up;
					mmsg.flags |= mouse_flag_left;
				} else if (msg.message == WM_RBUTTONUP) {
					mmsg.msg = mouse_msg_up;
					mmsg.flags |= mouse_flag_right;
				} else if (msg.message == WM_MBUTTONUP) {
					mmsg.msg = mouse_msg_up;
					mmsg.flags |= mouse_flag_mid;
				} else if (msg.message == WM_MOUSEWHEEL) {
					mmsg.msg = mouse_msg_wheel;
					mmsg.wheel = (short)((unsigned)msg.wParam >> 16);
				}
				return mmsg;
			}
		}while (!pg->exit_window && !pg->exit_flag && waitdealmessage(pg));
	}
	return mmsg;
}

MOUSEMSG
GetMouseMsg() {
	struct _graph_setting * pg = &graph_setting;
	MOUSEMSG mmsg = {0};
	if (pg->exit_window)
		return mmsg;

	{
		EGEMSG msg;
		do {
			msg = _getmouse(pg);
			if (msg.hwnd) {
				mmsg.uMsg = msg.message;
				mmsg.mkCtrl = ((msg.wParam & MK_CONTROL) != 0);
				mmsg.mkShift = ((msg.wParam & MK_SHIFT) != 0);
				mmsg.x = (short)((int)msg.lParam & 0xFFFF);
				mmsg.y = (short)((unsigned)msg.lParam >> 16);
				if (msg.mousekey & 1) {
					mmsg.mkLButton = 1;
				}
				if (msg.mousekey & 4) {
					mmsg.mkMButton = 1;
				}
				if (msg.mousekey & 2) {
					mmsg.mkRButton = 1;
				}
				if (msg.message == WM_MOUSEWHEEL) {
					mmsg.wheel = (short)((unsigned)msg.wParam >> 16);
				}
				return mmsg;
			}
		}while (!pg->exit_window && waitdealmessage(pg));
	}
	return mmsg;
}

/*private function*/
static
void
setmode(int gdriver, int gmode) {
	struct _graph_setting * pg = &graph_setting;

	if (gdriver == TRUECOLORSIZE){
		RECT rect;
		if (g_attach_hwnd) {
			GetClientRect(g_attach_hwnd, &rect);
		} else {
			GetWindowRect(GetDesktopWindow(), &rect);
		}
		pg->dc_w = (short)(gmode & 0xFFFF);
		pg->dc_h = (short)((unsigned int)gmode >> 16);
		if (pg->dc_w < 0) {
			pg->dc_w = rect.right;
		}
		if (pg->dc_h < 0) {
			pg->dc_h = rect.bottom;
		}
	} else {
		pg->dc_w = 640;
		pg->dc_h = 480;
	}
}

/*private function*/
static
BOOL
init_instance(HINSTANCE hInstance, int nCmdShow) {
	struct _graph_setting * pg = &graph_setting;
	int dw = 0, dh = 0;
	//WCHAR Title[256] = {0};
	//WCHAR Title2[256] = {0};

	//WideCharToMultiByte(CP_UTF8, 0, pg->window_caption, lstrlenW(pg->window_caption), (LPSTR)Title, 256, 0, 0);
	//MultiByteToWideChar(CP_UTF8, 0, (LPSTR)Title, -1, Title2, 256);
	dw = GetSystemMetrics(SM_CXFRAME) * 2;
	dh = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) * 2;
	if (g_attach_hwnd) {
		LONG_PTR style = GetWindowLongPtrW(g_attach_hwnd, GWL_STYLE);
		style |= WS_CHILDWINDOW | WS_CLIPCHILDREN;
		SetWindowLongPtrW(g_attach_hwnd, GWL_STYLE, style);
	}

	pg->hwnd = CreateWindowEx(
		g_windowexstyle,
		pg->window_class_name,
		pg->window_caption,
		g_windowstyle & ~WS_VISIBLE,
		g_windowpos_x,
		g_windowpos_y,
		pg->dc_w + dw,
		pg->dc_h + dh,
		g_attach_hwnd,
		NULL,
		hInstance,
		NULL
		);

	if (!pg->hwnd) {
		return FALSE;
	}

	if (g_attach_hwnd) {
		//SetParent(pg->hwnd, g_attach_hwnd);
		wchar_t name[64];
		swprintf(name, L"ege_%X", (DWORD)(DWORD_PTR)g_attach_hwnd);
		if (CreateEventW(NULL, FALSE, TRUE, name)) {
			if (GetLastError() == ERROR_ALREADY_EXISTS) {
				PostMessage(pg->hwnd, WM_CLOSE, 0, 0);
			}
		}
	}
	//SetWindowTextA(pg->hwnd, (LPCSTR)Title);
	SetWindowLongPtrW(pg->hwnd, GWLP_USERDATA, (LONG_PTR)pg);

	/* {
		LOGFONTW lf = {0};
		lf.lfHeight         = 12;
		lf.lfWidth          = 6;
		lf.lfEscapement     = 0;
		lf.lfOrientation    = 0;
		lf.lfWeight         = FW_DONTCARE;
		lf.lfItalic         = 0;
		lf.lfUnderline      = 0;
		lf.lfStrikeOut      = 0;
		lf.lfCharSet        = DEFAULT_CHARSET;
		lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
		lf.lfQuality        = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH;
		lstrcpyW(lf.lfFaceName, L"宋体");
		HFONT hfont = CreateFontIndirectW(&lf);
		::SendMessage(pg->hwnd, WM_SETFONT, (WPARAM)hfont, NULL);
		//DeleteObject(hfont);
	} //*/


	pg->exit_window = 0;
	ShowWindow(pg->hwnd, nCmdShow);
	if (g_windowexstyle & WS_EX_TOPMOST) {
		SetWindowPos(pg->hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	}
	return TRUE;
}

/*private callback function*/
#if !defined(UNICODE)
BOOL
CALLBACK
EnumResNameProc(
	HMODULE hModule,
	LPCSTR lpszType,
	LPSTR lpszName,
	LONG_PTR lParam
	)
{
	HICON hico = (HICON)LoadImage(hModule, lpszName, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	if (hico) {
		*((HICON*)lParam) = hico;
		return FALSE;
	}
	return TRUE;
}

#else

BOOL
CALLBACK
EnumResNameProc(
	HMODULE hModule,
	LPCWSTR lpszType,
	LPWSTR lpszName,
	LONG_PTR lParam
	)
{
	HICON hico = (HICON)LoadImageW(hModule, lpszName, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	if (hico) {
		*((HICON*)lParam) = hico;
		return FALSE;
	}
	return TRUE;
}
#endif

void
DefCloseHandler() {
	struct _graph_setting * pg = &graph_setting;
	pg->exit_flag = 1;
}

/*private function*/
static
void
on_repaint(struct _graph_setting * pg, HWND hwnd, HDC dc) {
	int page = pg->visual_page;
	bool release = false;
	pg->img_timer_update->copyimage(pg->img_page[page]);
	if (dc == NULL) {
		dc = GetDC(hwnd);
		release = true;
	}
	int left = pg->img_timer_update->m_vpt.left, top = pg->img_timer_update->m_vpt.top;

	BitBlt(dc, 0, 0, pg->base_w, pg->base_h, pg->img_timer_update->m_hDC, pg->base_x - left, pg->base_y - top, SRCCOPY);
	if (release)
		ReleaseDC(hwnd, dc);
}

/*private function*/
static
void
on_timer(struct _graph_setting * pg, HWND hwnd, unsigned id) {
	if (!pg->skip_timer_mark && id == RENDER_TIMER_ID) {
		if (pg->update_mark_count <= 0) {
			pg->update_mark_count = UPDATE_MAX_CALL;
			on_repaint(pg, hwnd, NULL);
		}
		if (pg->timer_stop_mark) {
			pg->timer_stop_mark = false;
			pg->skip_timer_mark = true;
		}
	}
}

/*private function*/
static
void
on_paint(struct _graph_setting * pg, HWND hwnd) {
	if (! pg->lock_window) {
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		on_repaint(pg, hwnd, hdc);
		EndPaint(hwnd, &ps);
	} else {
		ValidateRect( hwnd, NULL );
		pg->update_mark_count --;
	}
}

/*private function*/
static
void
on_destroy(struct _graph_setting * pg) {
	pg->exit_window = 1;
	if (pg->dc) {
		ReleaseDC(pg->hwnd, pg->window_dc);
		// release objects, not finish
	}
	PostQuitMessage(0);
	if (pg->close_manually && pg->use_force_exit) {
		ExitProcess(0);
	}
}

/*private function*/
static
void
on_setcursor(struct _graph_setting * pg, HWND hwnd) {
	if (pg->mouse_show) {
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	} else {
		RECT rect;
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hwnd, &pt);
		GetClientRect(hwnd, &rect);
		if (pt.x >= rect.left && pt.x < rect.right && pt.y >= rect.top && pt.y <= rect.bottom) {
			SetCursor(NULL);
		} else {
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
	}
}

/*private function*/
static
void
on_ime_control(struct _graph_setting * pg, HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	if (wparam == IMC_SETSTATUSWINDOWPOS) {
		HIMC hImc = ImmGetContext(hwnd);
		COMPOSITIONFORM cpf = {0};
		cpf.dwStyle = CFS_POINT;
		cpf.ptCurrentPos = *(LPPOINT)lparam;
		ImmSetCompositionWindow(hImc, &cpf);
	}
}

/*private function*/
static
void
windowmanager(ege::_graph_setting* pg, bool create, struct msg_createwindow* msg) {
	if (create) {
		msg->hwnd = ::CreateWindowExW(
						msg->exstyle,
						msg->classname,
						NULL,
						msg->style,
						0,
						0,
						0,
						0,
						getHWnd(),
						(HMENU)msg->id,
						getHInstance(),
						NULL);
		if (msg->hEvent) ::SetEvent(msg->hEvent);
	} else {
		if (msg->hwnd) {
			::DestroyWindow(msg->hwnd);
		}
		if (msg->hEvent) ::SetEvent(msg->hEvent);
	}
}

/*private function*/
static
void
on_key(struct _graph_setting * pg, UINT message, unsigned long keycode, LPARAM keyflag) {
	unsigned msg = 0;
	if (message == WM_KEYDOWN && keycode < MAX_KEY_VCODE) {
		msg = 1;
		pg->keystatemap[keycode] = 1;
	}
	if (message == WM_KEYUP && keycode < MAX_KEY_VCODE) {
		pg->keystatemap[keycode] = 0;
	}
	if (pg->callback_key) {
		int ret;
		if (message == WM_CHAR) {
			msg = 2;
		}
		ret = pg->callback_key(pg->callback_key_param, msg, (int)keycode);
		if (ret == 0) {
			return;
		}
	}
	{
		EGEMSG msg ={0};
		msg.hwnd = pg->hwnd;
		msg.message = message;
		msg.wParam = keycode;
		msg.lParam = keyflag;
		msg.time = ::GetTickCount();
		pg->msgkey_queue->push(msg);
	}
}

/*private function*/
static
void
push_mouse_msg(struct _graph_setting * pg, UINT message, WPARAM wparam, LPARAM lparam) {
	EGEMSG msg ={0};
	msg.hwnd = pg->hwnd;
	msg.message = message;
	msg.wParam = wparam;
	msg.lParam = lparam;
	msg.mousekey = (pg->mouse_state_m << 2) | (pg->mouse_state_r << 1) | (pg->mouse_state_l << 0);
	msg.time = ::GetTickCount();
	pg->msgmouse_queue->push(msg);
}

/*private function*/
static
LRESULT
CALLBACK
wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	struct _graph_setting * pg_w = NULL;
	struct _graph_setting * pg = &graph_setting;
	//int wmId, wmEvent;

	pg_w = (struct _graph_setting *)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
	if (pg_w == NULL || pg->img_page[0] == NULL) {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	switch (message) {
	case WM_TIMER:
		if (pg == pg_w) {
			on_timer(pg, hWnd, (unsigned int)wParam);
		}
		break;
	case WM_PAINT:
		if (pg == pg_w) {
			on_paint(pg, hWnd);
		}
		break;
	case WM_CLOSE:
		if (pg == pg_w) {
			if (pg->callback_close) {
				pg->callback_close();
			} else {
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_DESTROY:
		if (pg == pg_w) {
			on_destroy(pg);
		}
		break;
	case WM_ERASEBKGND:
		if (pg == pg_w) {
			return TRUE;
		}
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		//if (hWnd == pg->hwnd)
		{
			on_key(pg, message, (unsigned long)wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		pg->mouse_lastclick_x = (short int)((UINT)lParam & 0xFFFF);
		pg->mouse_lastclick_y = (short int)((UINT)lParam >> 16);
		pg->keystatemap[VK_LBUTTON] = 1;
		SetCapture(hWnd);
		pg->mouse_state_l = 1;
		if (hWnd == pg->hwnd) push_mouse_msg(pg, message, wParam, lParam);
		break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
		pg->mouse_lastclick_x = (short int)((UINT)lParam & 0xFFFF);
		pg->mouse_lastclick_y = (short int)((UINT)lParam >> 16);
		pg->keystatemap[VK_MBUTTON] = 1;
		SetCapture(hWnd);
		pg->mouse_state_m = 1;
		if (hWnd == pg->hwnd) push_mouse_msg(pg, message, wParam, lParam);
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
		pg->mouse_lastclick_x = (short int)((UINT)lParam & 0xFFFF);
		pg->mouse_lastclick_y = (short int)((UINT)lParam >> 16);
		pg->keystatemap[VK_RBUTTON] = 1;
		SetCapture(hWnd);
		pg->mouse_state_r = 1;
		if (hWnd == pg->hwnd) push_mouse_msg(pg, message, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		pg->mouse_lastup_x = (short int)((UINT)lParam & 0xFFFF);
		pg->mouse_lastup_y = (short int)((UINT)lParam >> 16);
		pg->mouse_state_l = 0;
		pg->keystatemap[VK_LBUTTON] = 0;
		if (pg->mouse_state_l == 0 && pg->mouse_state_m == 0 && pg->mouse_state_r == 0) {
			ReleaseCapture();
		}
		if (hWnd == pg->hwnd) push_mouse_msg(pg, message, wParam, lParam);
		break;
	case WM_MBUTTONUP:
		pg->mouse_lastup_x = (short int)((UINT)lParam & 0xFFFF);
		pg->mouse_lastup_y = (short int)((UINT)lParam >> 16);
		pg->mouse_state_m = 0;
		pg->keystatemap[VK_MBUTTON] = 0;
		if (pg->mouse_state_l == 0 && pg->mouse_state_m == 0 && pg->mouse_state_r == 0) {
			ReleaseCapture();
		}
		if (hWnd == pg->hwnd) push_mouse_msg(pg, message, wParam, lParam);
		break;
	case WM_RBUTTONUP:
		pg->mouse_lastup_x = (short int)((UINT)lParam & 0xFFFF);
		pg->mouse_lastup_y = (short int)((UINT)lParam >> 16);
		pg->mouse_state_r = 0;
		pg->keystatemap[VK_RBUTTON] = 0;
		if (pg->mouse_state_l == 0 && pg->mouse_state_m == 0 && pg->mouse_state_r == 0) {
			ReleaseCapture();
		}
		if (hWnd == pg->hwnd) push_mouse_msg(pg, message, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		pg->mouse_last_x = (short int)((UINT)lParam & 0xFFFF);
		pg->mouse_last_y = (short int)((UINT)lParam >> 16);
		if (hWnd == pg->hwnd &&
			(pg->mouse_lastup_x != pg->mouse_last_x || pg->mouse_lastup_y != pg->mouse_last_y ))
				push_mouse_msg(pg, message, wParam, lParam);
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			pt.x = (short int)((UINT)lParam & 0xFFFF);
			pt.y = (short int)((UINT)lParam >> 16);
			ScreenToClient(pg->hwnd, &pt);
			pg->mouse_last_x = pt.x;
			pg->mouse_last_y = pt.y;
			lParam = ((unsigned short)(short int)pg->mouse_last_y << 16) | (unsigned short)(short int)pg->mouse_last_x;
		}
		if (hWnd == pg->hwnd) push_mouse_msg(pg, message, wParam, lParam);
		break;
	case WM_SETCURSOR:
		if (pg == pg_w) {
			on_setcursor(pg, hWnd);
			return TRUE;
		}
		break;
	case WM_IME_CONTROL:
		on_ime_control(pg, hWnd, message, wParam, lParam);
		break;
	case WM_USER + 1:
		windowmanager(pg, (wParam != 0), (struct msg_createwindow*)lParam);
		break;
	case WM_USER + 2:
		::SetFocus((HWND)lParam);
		break;
	case WM_CTLCOLOREDIT:
		{
			egeControlBase* ctl = (egeControlBase *)GetWindowLongPtrW((HWND)lParam, GWLP_USERDATA);
			return ctl->onMessage(message, wParam, lParam);
		}
		break;
	default:
		if (pg != pg_w) {
			return ((egeControlBase*)pg_w)->onMessage(message, wParam, lParam);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	if (pg != pg_w) {
		return ((egeControlBase*)pg_w)->onMessage(message, wParam, lParam);
	}
	return 0;
}

PVOID
getProcfunc() {
	return (PVOID)wndproc;
}

/*private function*/
static
ATOM
register_class(struct _graph_setting * pg, HINSTANCE hInstance) {
	WNDCLASSEX wcex ={0};
	HICON hico = NULL;

	wcex.cbSize = sizeof(wcex);

	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = (WNDPROC)getProcfunc();
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName  = pg->window_class_name;

	EnumResourceNames(hInstance, RT_ANIICON, EnumResNameProc, (LONG_PTR)&hico);
	if (hico) {
		wcex.hIcon = hico;
		goto END_LOAD_ICON;
	}
	EnumResourceNames(hInstance, RT_GROUP_ICON, EnumResNameProc, (LONG_PTR)&hico);
	if (hico) {
		wcex.hIcon = hico;
		goto END_LOAD_ICON;
	}
	EnumResourceNames(hInstance, RT_ICON, EnumResNameProc, (LONG_PTR)&hico);
	if (hico) {
		wcex.hIcon = hico;
		goto END_LOAD_ICON;
	}

END_LOAD_ICON:

	return RegisterClassEx(&wcex);
}

/* private function */
int
graph_init(_graph_setting * pg) {
	HDC hDC = GetDC(pg->hwnd);
	pg->dc = hDC;
	pg->window_dc = hDC;
	pg->img_timer_update = newimage();
	pg->msgkey_queue = new thread_queue<EGEMSG>;
	pg->msgmouse_queue = new thread_queue<EGEMSG>;
	setactivepage(0);
	settarget(NULL);
	setvisualpage(0);
	window_setviewport(0, 0, pg->dc_w, pg->dc_h);
	//ReleaseDC(pg->hwnd, hDC);
	return 0;
}

void logoscene() {
	int nsize = getlogodatasize();
	PIMAGE pimg = newimage();
	int alpha = 0, b_nobreak = 1, n;
	pimg->getimage((void*)getlogodata(), nsize);
	if (getwidth() <= getwidth(pimg)) {
		PIMAGE pimg1 = newimage(getwidth(pimg)/2, getheight(pimg)/2);
		putimage(pimg1, 0, 0, getwidth(pimg)/2, getheight(pimg)/2,
			pimg, 0, 0, getwidth(pimg), getheight(pimg));
		delimage( pimg );
		pimg = pimg1;
	}
	setrendermode(RENDER_MANUAL);
	for (n=0; n<20*1 && b_nobreak; n++, delay_fps(60)) {
		cleardevice();
	}
	for (alpha=0; alpha<=0xFF && b_nobreak; alpha+=8, delay_fps(60)) {
		setbkcolor_f(EGERGB(alpha, alpha, alpha));
		cleardevice();
		while (kbhit()) {
			getkey();
		}
	}
	setbkcolor_f(0xFFFFFF);
	for (alpha=0; alpha<=0xFF && b_nobreak; alpha+=8, delay_fps(60)) {
		cleardevice();
		putimage_alphablend(NULL, pimg, (getwidth() - pimg->getwidth()) / 2, (getheight() - pimg->getheight()) / 2, (UCHAR)alpha);
		while (kbhit()) {
			getkey();
			b_nobreak = 0;
		}
	}
	setbkcolor_f(0xFFFFFF);
	for (n=0; n<60*1 && b_nobreak; n++, delay_fps(60)) {
		cleardevice();
		putimage((getwidth() - pimg->getwidth()) / 2, (getheight() - pimg->getheight()) / 2, pimg);
		while (kbhit()) {
			getkey();
			b_nobreak = 0;
		}
	}
	setbkcolor_f(0xFFFFFF);
	for ((alpha>0xFF) && (alpha-=4); alpha>=0; alpha-=8, delay_fps(60)) {
		cleardevice();
		putimage_alphablend(NULL, pimg, (getwidth() - pimg->getwidth()) / 2, (getheight() - pimg->getheight()) / 2, (UCHAR)alpha);
		while (kbhit()) {
			getkey();
			b_nobreak = 0;
		}
	}
	cleardevice();
	for (alpha=0xFF; alpha>=0; alpha-=8, delay_fps(60)) {
		setbkcolor_f(EGERGB(alpha, alpha, alpha));
		cleardevice();
		while (kbhit()) {
			getkey();
			b_nobreak = 0;
		}
	}
	delimage( pimg );
	setbkcolor_f(BLACK);
	cleardevice();
	setrendermode(RENDER_AUTO);
}

void
init_img_page(struct _graph_setting * pg) {
	if (g_has_init) {
		for (int page = 0; page < BITMAP_PAGE_SIZE; ++page) {
			if (pg->img_page[page] == NULL) {
				;
			} else {
				pg->img_page[page]->createimage(pg->dc_w, pg->dc_h);
			}
		}
		graph_init(pg);
		ShowWindow(pg->hwnd, SW_SHOW);
	} else {
#ifdef EGE_GDIPLUS
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&pg->g_gdiplusToken, &gdiplusStartupInput, NULL);
#endif
	}
	g_has_init = true;
}

void
initgraph(int *gdriver, int *gmode, char *path) {
	struct _graph_setting * pg = &graph_setting;

	g_initcall = 0;
	if (!g_has_init) {
		memset(pg, 0, sizeof(_graph_setting));
		pg->exit_flag = 0;
		pg->exit_window = 0;
		init_img_page(pg);
	} else {
		pg->exit_flag = 0;
		pg->exit_window = 0;
		setmode(*gdriver, *gmode);
		init_img_page(pg);
		return ;
	}

	setmode(*gdriver, *gmode);
	pg->instance = GetModuleHandle(NULL);
	lstrcpy(pg->window_class_name, TEXT("Easy Graphics Engine"));
	lstrcpy(pg->window_caption, EGE_TITLE);

	{
		//SECURITY_ATTRIBUTES sa = {0};
		DWORD pid;
		pg->init_finish = false;
		pg->threadui_handle = CreateThread(NULL, 0, messageloopthread, pg, CREATE_SUSPENDED, &pid);
		ResumeThread(pg->threadui_handle);
		while (!pg->init_finish) {
			Sleep(1);
		}
	}

	UpdateWindow(pg->hwnd);

	{   //初始化鼠标位置数据
		pg->mouse_last_x = pg->dc_w / 2;
		pg->mouse_last_y = pg->dc_h / 2;
	}
	static egeControlBase _egeControlBase;

	if (g_initoption & INIT_WITHLOGO) {
		logoscene();
	}
	if (g_initoption & INIT_RENDERMANUAL) {
		setrendermode(RENDER_MANUAL);
	}
	pg->mouse_show = 1;
}

void
initgraph(int Width, int Height, int Flag) {
	int g = TRUECOLORSIZE, m = (Width) | (Height<<16);
	if (g_initcall == 0) setinitmode(Flag);
	initgraph(&g, &m, (char*)"");
	//using flag;
}

void
detectgraph(int *gdriver, int *gmode) {
	*gdriver = VGA;
	*gmode = VGAHI;
}

void
closegraph() {
	struct _graph_setting * pg = &graph_setting;
	ShowWindow(pg->hwnd, SW_HIDE);
}

int attachHWND(HWND hWnd) {
	g_attach_hwnd = hWnd;
	return 0;
}

/*private function*/
DWORD
WINAPI
messageloopthread(LPVOID lpParameter) {
	_graph_setting* pg = (_graph_setting*)lpParameter;
	MSG msg;
	{
		int nCmdShow = SW_SHOW;
		register_class(pg, pg->instance);

		/* 执行应用程序初始化: */
		if (!init_instance(pg->instance, nCmdShow)) {
			return 0xFFFFFFFF;
		}

		//图形初始化
		graph_init(pg);

		{
			pg->mouse_show = 0;
			pg->exit_flag = 0;
			pg->use_force_exit = (g_initoption & INIT_NOFORCEEXIT ? false : true);
			if (g_initoption & INIT_NOFORCEEXIT) {
				SetCloseHandler(DefCloseHandler);
			}
			pg->close_manually = true;
		}
		{
			pg->skip_timer_mark = false;
			SetTimer(pg->hwnd, RENDER_TIMER_ID, 50, NULL);
		}
	}
	pg->init_finish = true;

	while (!pg->exit_window) {
		if (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			Sleep(1);
		}
	}
	return 0;
}


void
setinitmode(int mode, int x, int y) {
	g_initcall = 1;
	g_initoption = mode;
	if (mode & INIT_NOBORDER) {
		if (mode & INIT_CHILD) {
			g_windowstyle = WS_CHILDWINDOW|WS_CLIPCHILDREN|WS_VISIBLE;
		} else {
			g_windowstyle = WS_POPUP|WS_CLIPCHILDREN|WS_VISIBLE;
		}
		g_windowexstyle = WS_EX_LEFT|WS_EX_LTRREADING;
	} else {
		g_windowstyle = WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_CLIPCHILDREN|WS_VISIBLE;
		g_windowexstyle = WS_EX_LEFT|WS_EX_LTRREADING;
	}
	if (mode & INIT_TOPMOST) {
		g_windowexstyle |= WS_EX_TOPMOST;
	}
	g_windowpos_x = x;
	g_windowpos_y = y;
}

} // namespace ege
