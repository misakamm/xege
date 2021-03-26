/*
* EGE (Easy Graphics Engine)
* filename  egegapi.cpp

本文件汇集较独立的函数接口
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#include "ege_head.h"

#include <cmath>
#include <cstdarg>
#include <cstdio>

#include "lpng/zlib.h"
#include "ege/sys_edit.h"
#include "ege_common.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <wincon.h>


// macro to check whether hConsole is valid
#define CHECK_CONSOLE_HANDLE(hHandle) { if(hHandle == NULL) return FALSE; }

namespace ege {

//We don't support vc6
#ifndef EGE_COMPILERINFO_VC6

static HANDLE hInput=NULL;
static HANDLE hOutput=NULL;
static HWND hConsoleWnd=NULL;
static FILE fOldIn;
static FILE fOldOut;

BOOL init_console(){
	HMENU hMenu;
	int hCrt;
	FILE *hf;
	if (hInput!=NULL) {
		return FALSE;
	}
	if (GetConsoleWindow()!=NULL) {
		return FALSE;
	}
	if (!AllocConsole()) {
		return FALSE;
	}
	hOutput=GetStdHandle(STD_OUTPUT_HANDLE);

	if(INVALID_HANDLE_VALUE == hOutput) {
		return FALSE;
	}
	hInput=
		GetStdHandle(STD_INPUT_HANDLE);
	if(INVALID_HANDLE_VALUE == hInput) {
		return FALSE;
	}
	SetConsoleTitle("EGE CONSOLE");
	hConsoleWnd=GetConsoleWindow();
 	if(INVALID_HANDLE_VALUE == hConsoleWnd) {
		return FALSE;
	}
	hMenu = GetSystemMenu(hConsoleWnd,0);
	if(hMenu != NULL)
	{
		DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
		DrawMenuBar(hConsoleWnd);
	}

	hCrt=_open_osfhandle((intptr_t)hOutput,_O_TEXT);
	hf=_fdopen( hCrt, "w" );
	fOldOut=*stdout;
	*stdout=*hf;
	setvbuf( stdout, NULL, _IONBF, 0 );

	hCrt=_open_osfhandle((intptr_t)hInput,_O_TEXT);
	hf=_fdopen(hCrt, "r");
	fOldIn=*stdin;
	*stdin=*hf;
	//setvbuf( stdin, NULL, _IONBF, 0 );
  // test code


	//SetConsoleTextAttribute(hOutput, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED ); // white text on black bg
	clear_console();
	ShowWindow(hConsoleWnd,SW_SHOW);
	return TRUE;
}

void clear_console(){
	/***************************************/
	// This code is from one of Microsoft's
	// knowledge base articles, you can find it at
	// http://support.microsoft.com/default.aspx?scid=KB;EN-US;q99261&
	/***************************************/

	COORD coordScreen = { 0, 0 };

	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
	DWORD dwConSize;

	if(hOutput == NULL) return; 

	/* get the number of character cells in the current buffer */

	bSuccess = GetConsoleScreenBufferInfo( hOutput, &csbi );
	
	if(!bSuccess) return;
	
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	/* fill the entire screen with blanks */

	bSuccess = FillConsoleOutputCharacter( hOutput, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );
	if(!bSuccess) return;
		

	/* get the current text attribute */

	bSuccess = GetConsoleScreenBufferInfo( hOutput, &csbi );
	if(!bSuccess) return;
		

	/* now set the buffer's attributes accordingly */

	bSuccess = FillConsoleOutputAttribute( hOutput, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
	if(!bSuccess) return;
		

	/* put the cursor at (0, 0) */

	bSuccess = SetConsoleCursorPosition( hOutput, coordScreen );
	if(!bSuccess) return;
		
}

BOOL show_console(){
	CHECK_CONSOLE_HANDLE(hConsoleWnd);
	return ShowWindow(hConsoleWnd,SW_SHOW);
}

BOOL hide_console(){
	CHECK_CONSOLE_HANDLE(hConsoleWnd);
	return ShowWindow(hConsoleWnd,SW_HIDE);
}

BOOL close_console(){
	if (!FreeConsole()){
		return FALSE;		
	}
	hOutput=NULL;
	hInput=NULL;
	hConsoleWnd=NULL;
	*stdout=fOldOut;
	*stdin=fOldIn;
	return TRUE;
};

#endif

	

int dealmessage(_graph_setting* pg, bool force_update);
void guiupdate(_graph_setting* pg, egeControlBase* &root);
float EGE_PRIVATE_GetFPS(int add);
int getflush();

std::string w2mb(const wchar_t wStr[]) {
	int bufsize = WideCharToMultiByte(CP_ACP, 0, wStr, -1, NULL, 0, 0, 0);
	std::string mbStr(bufsize, '\0');
	WideCharToMultiByte(CP_ACP, 0, wStr, -1, &mbStr[0], bufsize, 0, 0);
	return mbStr;
}

std::wstring mb2w(const char mbStr[]) {
	int bufsize = MultiByteToWideChar(CP_ACP, 0, mbStr, -1, NULL, 0);
	std::wstring wStr(bufsize, L'\0');
	MultiByteToWideChar(CP_ACP, 0, mbStr, -1, &wStr[0], bufsize);
	return wStr;
}

void internal_panic(LPCWSTR errmsg) {
	MessageBoxW(graph_setting.hwnd, errmsg, L"EGE INTERNAL ERROR", MB_ICONSTOP);
	ExitProcess((UINT)grError);
}

double
get_highfeq_time_ls(struct _graph_setting * pg) {
	static LARGE_INTEGER llFeq = {{0}}; /* 此实为常数 */
	LARGE_INTEGER llNow = {{0}};

	if (pg->get_highfeq_time_start.QuadPart == 0) {
		if (1)
		{
			SetThreadAffinityMask(::GetCurrentThread(), 0);
			QueryPerformanceCounter(&pg->get_highfeq_time_start);
			QueryPerformanceFrequency(&llFeq);
		}
		else if (0)
		{
			::timeBeginPeriod(1);
			pg->get_highfeq_time_start.QuadPart = ::timeGetTime();
			::timeEndPeriod(1);
			llFeq.QuadPart = 1000;
		}
		else if (1)
		{
			pg->get_highfeq_time_start.QuadPart = ::GetTickCount();
			llFeq.QuadPart = 1000;
		}
		else if (0)
		{
			::GetSystemTimeAsFileTime((LPFILETIME)&pg->get_highfeq_time_start);
			llFeq.QuadPart = 10000000;
		}
		return 0;
	} else {
		if (1)
		{
			QueryPerformanceCounter(&llNow);
		}
		else if (0)
		{
			::timeBeginPeriod(1);
			llNow.QuadPart = ::timeGetTime();
			::timeEndPeriod(1);
		}
		else if (1)
		{
			llNow.QuadPart = ::GetTickCount();
		}
		else if (0)
		{
			::GetSystemTimeAsFileTime((LPFILETIME)&llNow);
		}
		llNow.QuadPart -= pg->get_highfeq_time_start.QuadPart;
		return (double)llNow.QuadPart / llFeq.QuadPart;
	}
}

bool
is_run() {
	struct _graph_setting * pg = &graph_setting;
	if (pg->exit_window || pg->exit_flag) return false;
	return true;
}

void setcaption(LPCSTR  caption) {
	const std::wstring& new_caption = mb2w(caption);
	setcaption(new_caption.c_str());
}

void setcaption(LPCWSTR caption) {
	struct _graph_setting * pg = &graph_setting;
	if (pg->has_init) {
		::SetWindowTextW(getHWnd(), caption);
		::UpdateWindow(getHWnd()); // for vc6
	}

	pg->window_caption = caption;
}


void seticon(int icon_id) {
	struct _graph_setting * pg = &graph_setting;
	HICON hIcon = NULL;
	HINSTANCE instance = GetModuleHandle(NULL);

	if (icon_id == 0) {
		hIcon = ::LoadIconW(NULL, (LPCWSTR)IDI_APPLICATION);
	} else {
		hIcon = ::LoadIconW(instance, MAKEINTRESOURCEW(icon_id));
	}
	if (hIcon) {
		pg->window_hicon = hIcon;
		if (pg->has_init){
#ifdef _WIN64
			::SetClassLongPtrW(getHWnd(), GCLP_HICON, (LONG_PTR)hIcon);
#else
			::SetClassLongW(getHWnd(), GCL_HICON, (LONG)hIcon);
#endif
		}
	}
}

void movewindow(int x, int y, bool redraw) {
	::MoveWindow(getHWnd(), x, y, getwidth(), getheight(), redraw);
}


void
api_sleep(long dwMilliseconds) {
	if (dwMilliseconds >= 0)
		::Sleep(dwMilliseconds);
}

void
ege_sleep(long ms) {
	if (ms <= 0) return;
	if (0) { // 经济模式，占CPU极少
		::Sleep(ms);
	} else if (0) { //精确模式，占CPU略高
		static HANDLE hTimer = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		static MMRESULT resTimer = 0;
		::ResetEvent(hTimer);
		if (resTimer) ::timeKillEvent(resTimer);
		resTimer = ::timeSetEvent(ms, 1, (LPTIMECALLBACK)hTimer, 0, TIME_ONESHOT|TIME_CALLBACK_EVENT_SET);
		if (resTimer) {
			::WaitForSingleObject(hTimer, INFINITE);
		} else {
			::Sleep(1);
		}
		//::CloseHandle(hTimer);
	} else if (1) { //高精模式，占CPU更高
		static HANDLE hTimer = ::CreateWaitableTimer(NULL, TRUE, NULL);
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = ms * (LONGLONG)-10000;

		if (hTimer) {
			if (::SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, FALSE)) {
				::WaitForSingleObject(hTimer, INFINITE); // != WAIT_OBJECT_0;
			}
			//::CloseHandle(hTimer);
		} else {
			::Sleep(ms);
		}
	}
}

void
delay(long ms) {
	ege_sleep(ms);
}

void
delay_ms(long ms) {
	struct _graph_setting * pg = &graph_setting;
	egeControlBase* &root = pg->egectrl_root;
	pg->skip_timer_mark = true;
	if (ms == 0) {
		if (pg->update_mark_count < UPDATE_MAX_CALL) {
			ege_sleep(1);
			root->draw(NULL);
			dealmessage(pg, FORCE_UPDATE);
			root->update();
			{
				int l,t,r,b,c;
				getviewport(&l, &t, &r, &b, &c);
				setviewport(l, t, r, b, c);
			}
		}
		pg->delay_ms_dwLast = get_highfeq_time_ls(pg) * 1000.0;
		pg->skip_timer_mark = false;
		return;
	}
	{
		double delay_time = ms;
		double dw = get_highfeq_time_ls(pg) * 1000.0;
		int f = 100;

		if (ms >= 50) {
			f = 0;
		}
		pg->delay_ms_dwLast = 0;
		if (pg->delay_ms_dwLast == 0) {
			pg->delay_ms_dwLast = get_highfeq_time_ls(pg) * 1000.0;
		}
		if (pg->delay_ms_dwLast + 200.0 > dw) {
			dw = pg->delay_ms_dwLast;
		}

		//ege_sleep(1);
		root->draw(NULL);
		while (dw + delay_time >= get_highfeq_time_ls(pg) * 1000.0) {
			if ( f <= 0 || pg->update_mark_count < UPDATE_MAX_CALL) {
				dealmessage(pg, FORCE_UPDATE);
				f = 256;
			} else {
				ege_sleep((int)(dw + delay_time - get_highfeq_time_ls(pg) * 1000.0));
			}
			f -= 1;
		}
		dealmessage(pg, FORCE_UPDATE);
		dw = get_highfeq_time_ls(pg) * 1000.0;
		guiupdate(pg, root);
		if (pg->delay_ms_dwLast + 200.0 <= dw || pg->delay_ms_dwLast > dw) {
			pg->delay_ms_dwLast = dw;
		} else {
			pg->delay_ms_dwLast += delay_time;
		}
	}
	pg->skip_timer_mark = false;
}

/*
延迟1/fps的时间，调用间隔不大于200ms时能保证每秒能返回fps次
*/
void
delay_fps(int fps) {
	delay_fps((double)fps);
}
void
delay_fps(long fps) {
	delay_fps((double)fps);
}
void
delay_fps(double fps) {
	struct _graph_setting * pg = &graph_setting;
	egeControlBase* &root = pg->egectrl_root;
	pg->skip_timer_mark = true;
	double delay_time = 1000.0 / fps;
	double avg_max_time = delay_time * 10.0; // 误差时间在这个数值以内做平衡
	double dw = get_highfeq_time_ls(pg) * 1000.0;
	int nloop = 0;

	if (pg->delay_fps_dwLast == 0) {
		pg->delay_fps_dwLast = get_highfeq_time_ls(pg) * 1000.0;
	}
	if (pg->delay_fps_dwLast + delay_time + avg_max_time > dw) {
		dw = pg->delay_fps_dwLast;
	}
	root->draw(NULL);
	for (; nloop>=0; --nloop) {
		if ((dw + delay_time + (100.0) >= get_highfeq_time_ls(pg) * 1000.0)) {
			do {
				ege_sleep((int)(dw + delay_time - get_highfeq_time_ls(pg) * 1000.0));
			} while (dw + delay_time >= get_highfeq_time_ls(pg) * 1000.0);
		}
		dealmessage(pg, FORCE_UPDATE);
		dw = get_highfeq_time_ls(pg) * 1000.0;
		guiupdate(pg, root);
		if (pg->delay_fps_dwLast + delay_time + avg_max_time <= dw || pg->delay_fps_dwLast > dw) {
			pg->delay_fps_dwLast = dw;
		} else {
			pg->delay_fps_dwLast += delay_time;
		}
	}
	pg->skip_timer_mark = false;
}

/*
延迟1/fps的时间，调用间隔不大于200ms时能保证每秒能返回fps次
*/
void
delay_jfps(int fps) {
	delay_jfps((double)fps);
}
void
delay_jfps(long fps) {
	delay_jfps((double)fps);
}
void
delay_jfps(double fps) {
	struct _graph_setting * pg = &graph_setting;
	egeControlBase* &root = pg->egectrl_root;
	pg->skip_timer_mark = true;
	double delay_time = 1000.0/fps;
	double avg_max_time = delay_time * 10.0;
	double dw = get_highfeq_time_ls(pg) * 1000.0;
	int nloop = 0;

	if (pg->delay_fps_dwLast == 0) {
		pg->delay_fps_dwLast = get_highfeq_time_ls(pg) * 1000.0;
	}
	if (pg->delay_fps_dwLast + delay_time + avg_max_time > dw) {
		dw = pg->delay_fps_dwLast;
	}
	root->draw(NULL);
	for (; nloop>=0; --nloop) {
		int bSleep = 0;
		while (dw + delay_time >= get_highfeq_time_ls(pg) * 1000.0) {
			ege_sleep((int)(dw + delay_time - get_highfeq_time_ls(pg) * 1000.0));
			bSleep = 1;
		}
		if (bSleep) {
			dealmessage(pg, FORCE_UPDATE);
		} else {
			EGE_PRIVATE_GetFPS(-0x100);
		}
		dw = get_highfeq_time_ls(pg) * 1000.0;
		guiupdate(pg, root);
		if (pg->delay_fps_dwLast + delay_time + avg_max_time <= dw || pg->delay_fps_dwLast > dw) {
			pg->delay_fps_dwLast = dw;
		} else {
			pg->delay_fps_dwLast += delay_time;
		}
	}
	pg->skip_timer_mark = false;
}

int showmouse(int bShow) {
	struct _graph_setting * pg = &graph_setting;
	int ret = pg->mouse_show;
	pg->mouse_show = bShow;
	return ret;
}

int
mousepos(int *x, int *y) {
	struct _graph_setting * pg = &graph_setting;
	*x = pg->mouse_last_x;
	*y = pg->mouse_last_y;
	return 0;
}

void
setwritemode(int mode, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	SetROP2(img->m_hDC, mode);
	CONVERT_IMAGE_END;
}

static inline bool in_rect(int x, int y, int w, int h) {
	return !((x < 0) || (y < 0) || (x >= w) || (y >= h));
}

color_t
getpixel(int x, int y, PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	CONVERT_IMAGE_END;
	x += img->m_vpt.left;
	y += img->m_vpt.top;
	if (in_rect(x, y, img->m_width, img->m_height)) {
		return img->m_pBuffer[y * img->m_width + x];
	}
	// else
	return 0;
}

void
putpixel(int x, int y, color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	x += img->m_vpt.left;
	y += img->m_vpt.top;
	if (in_rect(x, y, img->m_vpt.right, img->m_vpt.bottom)) {
		img->m_pBuffer[y * img->m_width + x] = color;
	}
	CONVERT_IMAGE_END;
}

void
putpixels(int nPoint, int* pPoints, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	int x, y, c;
	PDWORD pb = &img->m_pBuffer[img->m_vpt.top * img->m_width + img->m_vpt.left];
	int w = img->m_vpt.right - img->m_vpt.left, h = img->m_vpt.bottom - img->m_vpt.top;
	int tw = img->m_width;
	for (int n=0; n<nPoint; ++n, pPoints += 3) {
		x = pPoints[0], y = pPoints[1], c = pPoints[2];
		if (in_rect(x, y, w, h)) {
			pb[y * tw + x] = c;
		}
	}
	CONVERT_IMAGE_END;
}

void
putpixels_f(int nPoint, int* pPoints, PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE(pimg);
	int x, y, c;
	int tw = img->m_width;
	int th = img->m_height;
	for (int n=0; n<nPoint; ++n, pPoints += 3) {
		x = pPoints[0], y = pPoints[1], c = pPoints[2];
		if (in_rect(x, y, tw, th)) {
			img->m_pBuffer[y * tw + x] = c;
		}
	}
	CONVERT_IMAGE_END;
}

color_t
getpixel_f(int x, int y, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_F_CONST(pimg);
	if (in_rect(x, y, img->m_width, img->m_height)) {
		return img->m_pBuffer[y * img->m_width + x];
	}
	return 0;
}

void
putpixel_f(int x, int y, color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_F(pimg);
	if (in_rect(x, y, img->m_width, img->m_height)) {
		img->m_pBuffer[y * img->m_width + x] = color;
	}
}

void putpixel_withalpha(int x, int y, color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	x += img->m_vpt.left;
	y += img->m_vpt.top;
	if (in_rect(x, y, img->m_vpt.right, img->m_vpt.bottom)) {
		color_t& dst_color = img->m_pBuffer[y * img->m_width + x];
		dst_color = alphablend_inline(dst_color, color, EGEGET_A(color));
	}
	CONVERT_IMAGE_END;
}

void putpixel_withalpha_f(int x, int y, color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_F(pimg);
	if (in_rect(x, y, img->m_width, img->m_height)) {
		color_t& dst_color = img->m_pBuffer[y * img->m_width + x];
		dst_color = alphablend_inline(dst_color, color, EGEGET_A(color));
	}
	CONVERT_IMAGE_END;
}

void putpixel_savealpha(int x, int y, color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	x += img->m_vpt.left;
	y += img->m_vpt.top;
	if (in_rect(x, y, img->m_vpt.right, img->m_vpt.bottom)) {
		color_t& dst_color = img->m_pBuffer[y * img->m_width + x];
		dst_color = EGECOLORA(color, EGEGET_A(dst_color));
	}
	CONVERT_IMAGE_END;
}

void putpixel_savealpha_f(int x, int y, color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_F(pimg);
	if (in_rect(x, y, img->m_width, img->m_height)) {
		color_t& dst_color = img->m_pBuffer[y * img->m_width + x];
		dst_color = EGECOLORA(color, EGEGET_A(dst_color));
	}
	CONVERT_IMAGE_END;
}

void
moveto(int x, int y, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	MoveToEx(img->m_hDC, x, y, NULL);
	CONVERT_IMAGE_END;
}

void
moverel(int dx, int dy, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	POINT pt;
	GetCurrentPositionEx(img->m_hDC, &pt);
	dx += pt.x;
	dy += pt.y;
	MoveToEx(img->m_hDC, dx, dy, NULL);
	CONVERT_IMAGE_END;
}

void
line(int x1, int y1, int x2, int y2, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	MoveToEx(img->m_hDC, x1, y1, NULL);
	LineTo(img->m_hDC, x2, y2);
	CONVERT_IMAGE_END;
}

void
linerel(int dx, int dy, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	POINT pt;
	GetCurrentPositionEx(img->m_hDC, &pt);
	dx += pt.x;
	dy += pt.y;
	LineTo(img->m_hDC, dx, dy);
	CONVERT_IMAGE_END;
}

void
lineto(int x, int y, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	LineTo(img->m_hDC, x, y);
	CONVERT_IMAGE_END;
}

/* private function */
static
void
line_base(float x1, float y1, float x2, float y2, PIMAGE img) {
	int bswap = 2;
	color_t col = getcolor(img);
	color_t endp = 0;
	color_t* pBuffer = (color_t*)img->m_pBuffer;
	int rw = img->m_width;
	if (x1 > x2) {
		float ft;
		SWAP(x1, x2, ft);
		SWAP(y1, y2, ft);
		if (bswap) bswap ^= 3;
	}
	if (x2 < img->m_vpt.left) return ;
	if (x1 > img->m_vpt.right) return ;
	if (x1 < img->m_vpt.left) {
		if (x2 - x1 < FLOAT_EPS) return;
		float d = (x2 - img->m_vpt.left) / (x2 - x1);
		y1 = (y1 - y2) * d + y2;
		x1 = (float)img->m_vpt.left;
		if (bswap == 1) bswap = 0;
	}
	if (x2 > img->m_vpt.right) {
		if (x2 - x1 < FLOAT_EPS) return;
		float d = (img->m_vpt.right - x1) / (x2 - x1);
		y2 = (y2 - y1) * d + y1;
		x2 = (float)img->m_vpt.right;
		if (bswap == 2) bswap = 0;
	}
	if (y1 > y2) {
		float ft;
		SWAP(x1, x2, ft);
		SWAP(y1, y2, ft);
		if (bswap) bswap ^= 3;
	}
	if (y2 < img->m_vpt.top) return ;
	if (y1 > img->m_vpt.bottom) return ;
	if (y1 < img->m_vpt.top) {
		if (y2 - y1 < FLOAT_EPS) return;
		float d = (y2 - img->m_vpt.top) / (y2 - y1);
		x1 = (x1 - x2) * d + x2;
		y1 = (float)img->m_vpt.top;
		if (bswap == 1) bswap = 0;
	}
	if (y2 > img->m_vpt.bottom) {
		if (y2 - y1 < FLOAT_EPS) return;
		float d = (img->m_vpt.bottom - y1) / (y2 - y1);
		x2 = (x2 - x1) * d + x1;
		y2 = (float)img->m_vpt.bottom;
		if (bswap == 2) bswap = 0;
	}
	if (bswap) {
		if (bswap == 1)     {
			endp = pBuffer[(int)y1 * rw + (int)x1];
		} else {
			endp = pBuffer[(int)y2 * rw + (int)x2];
		}
	}
	if (y2 - y1 > fabs(x2 - x1)) {
		int y = (int)(y1 + 0.9f);
		int ye = (int)(y2);
		float x, dx;
		if (y < y1) ++y;
		dx = (x2 - x1) / (y2 - y1);
		x = (y - y1) * dx + x1 + 0.5f;
		if (ye >= img->m_vpt.bottom) ye = img->m_vpt.bottom - 1;
		if (ye < y2) bswap = 0;
		for (; y <= ye; ++y, x += dx) {
			pBuffer[y * rw + (int)x] = col;
		}
	} else {
		if (x1 > x2) {
			float ft;
			SWAP(x1, x2, ft);
			SWAP(y1, y2, ft);
			if (bswap) bswap ^= 3;
		}
		int x = (int)(x1 + 0.9f);
		int xe = (int)(x2);
		float y, dy;
		if (x < x1) ++x;
		dy = (y2 - y1) / (x2 - x1);
		y = (x - x1) * dy + y1 + 0.5f;
		if (xe >= img->m_vpt.right) xe = img->m_vpt.right - 1;
		if (xe < x2) bswap = 0;
		for (; x <= xe; ++x, y += dy) {
			pBuffer[(int)y * rw + x] = col;
		}
	}
	if (bswap) {
		if (bswap == 1) {
			pBuffer[(int)y1 * rw + (int)x1] = endp;
		} else {
			pBuffer[(int)y2 * rw + (int)x2] = endp;
		}
	}
}

void
lineto_f(float x, float y, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	POINT pt;
	GetCurrentPositionEx(img->m_hDC, &pt);
	line_base((float)pt.x, (float)pt.y, x, y, img);
	CONVERT_IMAGE_END;
}

void
linerel_f(float dx, float dy, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	POINT pt;
	GetCurrentPositionEx(img->m_hDC, &pt);
	line_base((float)pt.x, (float)pt.y, (float)pt.x + dx, (float)pt.y + dy, img);
	CONVERT_IMAGE_END;
}

void
line_f(float x1, float y1, float x2, float y2, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	line_base(x1, y1, x2, y2, img);
	CONVERT_IMAGE_END;
}

/*private function*/
static
int
saveBrush(PIMAGE img, int save) { //此函数调用前，已经有Lock
	struct _graph_setting * pg = &graph_setting;
	if (save) {
		LOGBRUSH lbr = {0};

		lbr.lbColor = 0;
		lbr.lbStyle = BS_NULL;
		pg->savebrush_hbr = CreateBrushIndirect(&lbr);
		if (pg->savebrush_hbr) {
			pg->savebrush_hbr = (HBRUSH)SelectObject(img->m_hDC, pg->savebrush_hbr);
			return 1;
		}
	} else {
		if (pg->savebrush_hbr) {
			pg->savebrush_hbr = (HBRUSH)SelectObject(img->m_hDC, pg->savebrush_hbr);
			DeleteObject(pg->savebrush_hbr);
			pg->savebrush_hbr = NULL;
		}
	}
	return 0;
}

void rectangle(int left, int top, int right, int bottom, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (saveBrush(img, 1)) {
		Rectangle(img->m_hDC, left, top, right, bottom);
		saveBrush(img, 0);
	}
	CONVERT_IMAGE_END;
}

color_t
getcolor(PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);

	if (img && img->m_hDC) {
		CONVERT_IMAGE_END;
		return img->m_color;
		/*
		HPEN hpen_c = (HPEN)GetCurrentObject(img->m_hDC, OBJ_PEN);
		LOGPEN logPen;
		GetObject(hpen_c, sizeof(logPen), &logPen);
		CONVERT_IMAGE_END;
		return logPen.lopnColor;
		// */
	}
	CONVERT_IMAGE_END;
	return 0xFFFFFFFF;
}

// 将描述线形的位模式转换为 style 数组
// 用于 ExtCreatePen 中。
// 返回值为用到的数组元素数。
static int upattern2array(unsigned short upattern, DWORD style[]) {
	int n, segments = 0, segmentLength = 1;
	int state = !!(upattern & 1);
	for (n = 1; n < 16; n++) {
		int currentBit = !!(upattern & (1<<n));
		if (state == currentBit) {
			segmentLength += 1;
		} else {
			state = currentBit;
			style[segments] = segmentLength;
			segments += 1;
			segmentLength = 1;
		}
	}
	style[segments] = segmentLength;
	segments += 1;

	// 若 upattern 以 0 开头且为偶数段
	if (!(upattern & 1) && segments % 2 == 0) {
		DWORD p0 = style[0];
		for (int i = 0; i < segments - 1; ++i) {
			style[i] = style[i + 1];
		}
		style[segments - 1] = p0;
	}

	return segments;
}

static void update_pen(PIMAGE img) {
	LOGBRUSH lbr;
	lbr.lbColor = ARGBTOZBGR(img->m_color);
	lbr.lbStyle = BS_SOLID;
	lbr.lbHatch = 0;

	const int linestyle = img->m_linestyle.linestyle;
	const unsigned short upattern = img->m_linestyle.upattern;
	const int thickness = img->m_linestyle.thickness;

	// 添加这些属性以获得正确的显示效果
	int ls = linestyle|PS_GEOMETRIC|PS_ENDCAP_ROUND|PS_JOIN_ROUND;

	HPEN hpen;
	if (linestyle == USERBIT_LINE) {
		DWORD style[20] = {0};
		int bn = upattern2array(upattern, style);
		hpen = ExtCreatePen(ls, thickness, &lbr, bn, style);
	} else {
		hpen = ExtCreatePen(ls, thickness, &lbr, 0, NULL);
	}
	if (hpen) {
		DeleteObject(SelectObject(img->m_hDC, hpen));
	}

	// why update pen not in IMAGE???
#ifdef EGE_GDIPLUS
	Gdiplus::Pen* pen=img->getPen();
	pen->SetColor(img->m_color);
	pen->SetWidth(img->m_linewidth);
	pen->SetDashStyle(linestyle_to_dashstyle(img->m_linestyle.linestyle));
#endif

}

void
setcolor(color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img && img->m_hDC) {
		img->m_color = color;

		update_pen(img);
		SetTextColor(img->m_hDC, ARGBTOZBGR(color));
	}
	CONVERT_IMAGE_END;
}

void
setfillcolor(color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	LOGBRUSH lbr = {0};
	img->m_fillcolor = color;
	lbr.lbColor = ARGBTOZBGR(color);
	lbr.lbHatch = BS_SOLID;
	HBRUSH hbr = CreateBrushIndirect(&lbr);
	if (hbr) {
		DeleteObject(SelectObject(img->m_hDC, hbr));
	}
#ifdef EGE_GDIPLUS
	img->set_pattern(NULL);
#endif
	CONVERT_IMAGE_END;
}

color_t
getfillcolor(PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);

	if (img && img->m_hDC) {
		CONVERT_IMAGE_END;
		return img->m_fillcolor;
	}
	CONVERT_IMAGE_END;
	return 0xFFFFFFFF;
}

color_t
getbkcolor(PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);

	CONVERT_IMAGE_END;
	if (img) {
		return img->m_bk_color;
	}
	return 0xFFFFFFFF;
}

void
setbkcolor(color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img && img->m_hDC) {
		PDWORD p = img->m_pBuffer;
		int size = img->m_width * img->m_height;
		color_t col = img->m_bk_color;
		img->m_bk_color = color;
		SetBkColor(img->m_hDC, ARGBTOZBGR(color));
		for (int n = 0; n < size; n++, p++) {
			if (*p == col) {
				*p = color;
			}
		}
	}
	CONVERT_IMAGE_END;
}
void
setbkcolor_f(color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img && img->m_hDC) {
		img->m_bk_color = color;
		SetBkColor(img->m_hDC, ARGBTOZBGR(color));
	}
	CONVERT_IMAGE_END;
}

void setfontbkcolor(color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img && img->m_hDC) {
		SetBkColor(img->m_hDC, ARGBTOZBGR(color));
	}
	CONVERT_IMAGE_END;
}

void
setbkmode(int iBkMode, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img && img->m_hDC) {
		SetBkMode(img->m_hDC, iBkMode);
	}
	CONVERT_IMAGE_END;
}

PIMAGE gettarget() {
	struct _graph_setting * pg = &graph_setting;
	return pg->imgtarget_set;
}
int settarget(PIMAGE pbuf) {
	struct _graph_setting * pg = &graph_setting;
	pg->imgtarget_set = pbuf;
	if (pbuf == NULL) {
		pg->imgtarget = pg->img_page[graph_setting.active_page];
	} else {
		pg->imgtarget = pbuf;
	}
	return 0;
}

void
cleardevice(PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img && img->m_hDC) {
		color_t c = getbkcolor(img);
		for (color_t *p = (color_t*)img->getbuffer(),
			*e = (color_t*)&img->getbuffer()[img->m_width * img->m_height];
			p != e;
			++p) {
				*p = c;
		}
	}
	CONVERT_IMAGE_END;
}

void
arc(int x, int y, int stangle, int endangle, int radius, PIMAGE pimg) {
	ellipse(x, y, stangle, endangle, radius, radius, pimg);
}

void
arcf(float x, float y, float stangle, float endangle, float radius, PIMAGE pimg) {
	ellipsef(x, y, stangle, endangle, radius, radius, pimg);
}

void
circle(int x, int y, int radius, PIMAGE pimg) {
	ellipse(x, y, 0, 360, radius, radius, pimg);
}

void
circlef(float x, float y, float radius, PIMAGE pimg) {
	ellipsef(x, y, 0.0f, 360.0f, radius, radius, pimg);
}

void
ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	double sr = stangle/180.0*PI, er = endangle/180.0*PI;

	if (img) {
		Arc(img->m_hDC,
		x-xradius, y-yradius,
		x+xradius, y+yradius,
		(int)(x + xradius*cos(sr)),
		(int)(y - yradius*sin(sr)),
		(int)(x + xradius*cos(er)),
		(int)(y - yradius*sin(er))
		);
	}
	CONVERT_IMAGE_END;
}

void
ellipsef(float x, float y, float stangle, float endangle, float xradius, float yradius, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	double sr = stangle/180.0*PI, er = endangle/180.0*PI;

	if (img) {
		Arc(img->m_hDC,
		(int)(x-xradius), (int)(y-yradius),
		(int)(x+xradius), (int)(y+yradius),
		(int)(x + xradius*cos(sr)),
		(int)(y - yradius*sin(sr)),
		(int)(x + xradius*cos(er)),
		(int)(y - yradius*sin(er))
		);
	}
	CONVERT_IMAGE_END;
}

void
pieslice(int x, int y, int stangle, int endangle, int radius, PIMAGE pimg) {
	sector(x, y, stangle, endangle, radius, radius, pimg);
}

void
pieslicef(float x, float y, float stangle, float endangle, float radius, PIMAGE pimg) {
	sectorf(x, y, stangle, endangle, radius, radius, pimg);
}

void
sector(int x, int y, int stangle, int endangle, int xradius, int yradius, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	double sr = stangle/180.0*PI, er = endangle/180.0*PI;
	if (img) {
		Pie(img->m_hDC,
			x-xradius, y-yradius,
			x+xradius, y+yradius,
			(int)ege_round(x + xradius*cos(sr)), (int)ege_round(y - yradius*sin(sr)),
			(int)ege_round(x + xradius*cos(er)), (int)ege_round(y - yradius*sin(er))
			);
	}
	CONVERT_IMAGE_END;
}

void
sectorf(float x, float y, float stangle, float endangle, float xradius, float yradius, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	double sr = stangle/180.0*PI, er = endangle/180.0*PI;
	if (img) {
		Pie(img->m_hDC,
			(int)(x-xradius), (int)(y-yradius),
			(int)(x+xradius), (int)(y+yradius),
			(int)ege_round(x + xradius*cos(sr)), (int)ege_round(y - yradius*sin(sr)),
			(int)ege_round(x + xradius*cos(er)), (int)ege_round(y - yradius*sin(er))
			);
	}
	CONVERT_IMAGE_END;
}

void
fillellipse(int x, int y, int xradius, int yradius, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Ellipse(
			img->m_hDC,
			x-xradius, y-yradius,
			x+xradius, y+yradius
			);
	}
	CONVERT_IMAGE_END;
}

void
fillellipsef(float x, float y, float xradius, float yradius, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Ellipse(
			img->m_hDC,
			(int)(x-xradius), (int)(y-yradius),
			(int)(x+xradius), (int)(y+yradius)
			);
	}
	CONVERT_IMAGE_END;
}

void
bar(int left, int top, int right, int bottom, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	RECT rect = {left, top, right, bottom};
	HBRUSH hbr_last = (HBRUSH)GetCurrentObject(img->m_hDC, OBJ_BRUSH); //(HBRUSH)SelectObject(pg->g_hdc, hbr);

	if (img) {
		FillRect(img->m_hDC, &rect, hbr_last);
	}
	CONVERT_IMAGE_END;
}

void
fillroundrect(int left, int top, int right, int bottom, int xradius, int yradius, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		RoundRect(img->m_hDC,left,top,right,bottom,xradius*2,yradius*2); 
	}
	CONVERT_IMAGE_END;
}

void
roundrect(int left, int top, int right, int bottom, int xradius, int yradius, PIMAGE pimg){
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		LOGBRUSH lbr;
		HBRUSH hbr;
		HGDIOBJ hOld;
		lbr.lbStyle = BS_NULL;
		lbr.lbHatch = BS_NULL; //ignored
		lbr.lbColor = WHITE; // ignored
		hbr = CreateBrushIndirect(&lbr);
		hOld=SelectObject(img->m_hDC,hbr);
		RoundRect(img->m_hDC,left,top,right,bottom,xradius*2,yradius*2); 
		SelectObject(img->m_hDC,hOld); 
		DeleteObject(hbr);		
	}
	CONVERT_IMAGE_END;
}


void
fillrect(int left, int top, int right, int bottom, PIMAGE pimg){
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Rectangle(img->m_hDC,left,top,right,bottom); 
	}
	CONVERT_IMAGE_END;
}

void
bar3d(int x1, int y1, int x2, int y2, int depth, int topflag, PIMAGE pimg) {
	--x2; --y2;
	{
		int pt[20] = {
			x2, y2,
			x2, y1,
			x1, y1,
			x1, y2,
			x2, y2,
			x2+depth, y2-depth,
			x2+depth, y1-depth,
			x1+depth, y1-depth,
			x1, y1,
		};

		bar(x1, y1, x2, y2, pimg);
		if (topflag) {
			drawpoly(9, pt, pimg);
			line(x2, y1, x2+depth, y1-depth, pimg);
		} else {
			drawpoly(7, pt, pimg);
		}
	}
}

void
drawpoly(int numpoints, const int *polypoints, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Polyline(img->m_hDC, (POINT*)polypoints, numpoints);
	}
	CONVERT_IMAGE_END;
}

void
drawbezier(int numpoints, const int *polypoints, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (numpoints % 3 != 1) {
			numpoints = numpoints - (numpoints + 2) % 3;
		}
		PolyBezier(img->m_hDC, (POINT*)polypoints, numpoints);
	}
	CONVERT_IMAGE_END;
}

void
drawlines(int numlines, const int *polypoints, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		DWORD* pl = (DWORD*) malloc(sizeof(DWORD) * numlines);
		for (int i = 0; i < numlines; ++i) pl[i] = 2;
		PolyPolyline(img->m_hDC, (POINT*)polypoints, pl, numlines);
		free(pl);
	}
	CONVERT_IMAGE_END;
}

void
fillpoly(int numpoints, const int *polypoints, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img) {
		Polygon(img->m_hDC, (POINT*)polypoints, numpoints);
	}
	CONVERT_IMAGE_END;
}

void
fillpoly_gradient(int numpoints, const ege_colpoint* polypoints, PIMAGE pimg) {
	if (numpoints < 3) return;
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		TRIVERTEX* vert = (TRIVERTEX*)malloc(sizeof(TRIVERTEX) * numpoints);
		if (vert) {
			GRADIENT_TRIANGLE* tri = (GRADIENT_TRIANGLE*)malloc(sizeof(GRADIENT_TRIANGLE) * (numpoints - 2));
			if (tri) {
				for (int i = 0; i < numpoints; ++i) {
					vert[i].x = (long)polypoints[i].x;
					vert[i].y = (long)polypoints[i].y;
					vert[i].Red     = EGEGET_R(polypoints[i].color) << 8;
					vert[i].Green   = EGEGET_G(polypoints[i].color) << 8;
					vert[i].Blue    = EGEGET_B(polypoints[i].color) << 8;
					//vert[i].Alpha   = EGEGET_A(polypoints[i].color) << 8;
					vert[i].Alpha   = 0;
				}
				for (int j = 0; j < numpoints - 2; ++j) {
					tri[j].Vertex1 = j;
					tri[j].Vertex2 = j + 1;
					tri[j].Vertex3 = j + 2;
				}
				::GradientFill(img->getdc(), vert, numpoints, tri, numpoints - 2, GRADIENT_FILL_TRIANGLE);
				free(tri);
			}
			free(vert);
		}
	}
	CONVERT_IMAGE_END;
}

void
floodfill(int x, int y, int border, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		FloodFill(img->m_hDC, x, y, ARGBTOZBGR(border));
	}
	CONVERT_IMAGE_END;
}

void
floodfillsurface(int x, int y, color_t areacolor, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		ExtFloodFill(img->m_hDC, x, y, ARGBTOZBGR(areacolor), FLOODFILLSURFACE);
	}
	CONVERT_IMAGE_END;
}

/* private function */
static
unsigned int
private_gettextmode(PIMAGE img) {
	UINT fMode = TA_NOUPDATECP; //TA_UPDATECP;
	if (img->m_texttype.horiz == RIGHT_TEXT) {
		fMode |= TA_RIGHT;
	} else if (img->m_texttype.horiz == CENTER_TEXT) {
		fMode |= TA_CENTER;
	} else {
		fMode |= TA_LEFT;
	}
	if (img->m_texttype.vert == BOTTOM_TEXT) {
		fMode |= TA_BOTTOM;
	} else {
		fMode |= TA_TOP;
	}
	return fMode;
}

/* private function */
static
void
private_textout (PIMAGE img, LPCSTR textstring, int x, int y, int horiz, int vert) {
	if (horiz >= 0 && vert >= 0) {
		UINT fMode = TA_NOUPDATECP; //TA_UPDATECP;
		img->m_texttype.horiz = horiz;
		img->m_texttype.vert = vert;
		if (img->m_texttype.horiz == RIGHT_TEXT) {
			fMode |= TA_RIGHT;
		} else if (img->m_texttype.horiz == CENTER_TEXT) {
			fMode |= TA_CENTER;
		} else {
			fMode |= TA_LEFT;
		}
		if (img->m_texttype.vert == BOTTOM_TEXT) {
			fMode |= TA_BOTTOM;
		} else {
			fMode |= TA_TOP;
		}
		SetTextAlign(img->m_hDC, fMode);
	} else {
		SetTextAlign(img->m_hDC, private_gettextmode(img));
	}
	if (textstring) {
		if (img->m_texttype.vert == CENTER_TEXT) {
			y -= textheight(textstring, img)/2;
		}
		TextOutA(img->m_hDC, x, y, textstring, (int)strlen(textstring));
	}
}

/* private function */
static
void
private_textout (PIMAGE img, LPCWSTR textstring, int x, int y, int horiz, int vert) {
	if (horiz >= 0 && vert >= 0) {
		UINT fMode = TA_NOUPDATECP; //TA_UPDATECP;
		img->m_texttype.horiz = horiz;
		img->m_texttype.vert = vert;
		if (img->m_texttype.horiz == RIGHT_TEXT) {
			fMode |= TA_RIGHT;
		} else if (img->m_texttype.horiz == CENTER_TEXT) {
			fMode |= TA_CENTER;
		} else {
			fMode |= TA_LEFT;
		}
		if (img->m_texttype.vert == BOTTOM_TEXT) {
			fMode |= TA_BOTTOM;
		} else {
			fMode |= TA_TOP;
		}
		SetTextAlign(img->m_hDC, fMode);
	} else {
		SetTextAlign(img->m_hDC, private_gettextmode(img));
	}
	if (textstring) {
		if (img->m_texttype.vert == CENTER_TEXT) {
			y -= textheight(textstring, img)/2;
		}
		TextOutW(img->m_hDC, x, y, textstring, (int)lstrlenW(textstring));
	}
}

void
outtext(LPCSTR textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img) {
		POINT pt;
		GetCurrentPositionEx(img->m_hDC, &pt);
		private_textout(img, textstring, pt.x, pt.y, -1, -1);
	}
	CONVERT_IMAGE_END;
}

void
outtext(LPCWSTR textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img) {
		POINT pt;
		GetCurrentPositionEx(img->m_hDC, &pt);
		private_textout(img, textstring, pt.x, pt.y, -1, -1);
	}
	CONVERT_IMAGE_END;
}

void
outtext(CHAR c, PIMAGE pimg) {
	CHAR str[10] = {c};
	outtext(str, pimg);
}

void
outtext(WCHAR c, PIMAGE pimg) {
	WCHAR str[10] = {c};
	outtext(str, pimg);
}

void
outtextxy(int x, int y, LPCSTR textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img) {
		private_textout(img, textstring, x, y, -1, -1);
	}
	CONVERT_IMAGE_END;
}

void
outtextxy(int x, int y, LPCWSTR textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img) {
		private_textout(img, textstring, x, y, -1, -1);
	}
	CONVERT_IMAGE_END;
}

void
outtextxy(int x, int y, CHAR c, PIMAGE pimg) {
	CHAR str[10] = {c};
	outtextxy(x, y, str, pimg);
}

void
outtextxy(int x, int y, WCHAR c, PIMAGE pimg) {
	WCHAR str[10] = {c};
	outtextxy(x, y, str, pimg);
}

void
outtextrect(int x, int y, int w, int h, LPCSTR  textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img) {
		unsigned int fmode = private_gettextmode(img);
		RECT rect = {x, y, x+w, y+h};
		DrawTextA(img->m_hDC, textstring, -1, &rect, fmode|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL|DT_EXPANDTABS);
	}

	CONVERT_IMAGE_END;
}

void
outtextrect(int x, int y, int w, int h, LPCWSTR textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img) {
		unsigned int fmode = private_gettextmode(img);
		RECT rect = {x, y, x+w, y+h};
		DrawTextW(img->m_hDC, textstring, -1, &rect, fmode|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL|DT_EXPANDTABS);
	}

	CONVERT_IMAGE_END;
}

void
xyprintf(int x, int y, LPCSTR  fmt, ...) {
	va_list v;
	va_start(v, fmt);
	{
		struct _graph_setting * pg = &graph_setting;
		char* buff = (char*)pg->g_t_buff;
		vsprintf(buff, fmt, v);
		outtextxy(x, y, buff);
	}
	va_end(v);
}

void
xyprintf(int x, int y, LPCWSTR fmt, ...) {
	va_list v;
	va_start(v, fmt);
	{
		struct _graph_setting * pg = &graph_setting;
		wchar_t* buff = (wchar_t*)pg->g_t_buff;
		vswprintf(buff, fmt, v);
		outtextxy(x, y, buff);
	}
	va_end(v);
}

void
rectprintf(int x, int y, int w, int h, LPCSTR  fmt, ...) {
	va_list v;
	va_start(v, fmt);
	{
		struct _graph_setting * pg = &graph_setting;
		char* buff = (char*)pg->g_t_buff;
		vsprintf(buff, fmt, v);
		outtextrect(x, y, w, h, buff);
	}
	va_end(v);
}

void
rectprintf(int x, int y, int w, int h, LPCWSTR fmt, ...) {
	va_list v;
	va_start(v, fmt);
	{
		struct _graph_setting * pg = &graph_setting;
		wchar_t* buff = (wchar_t*)pg->g_t_buff;
		vswprintf(buff, fmt, v);
		outtextrect(x, y, w, h, buff);
	}
	va_end(v);
}

int
textwidth(LPCSTR textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		SIZE sz;
		GetTextExtentPoint32A(img->m_hDC, textstring, (int)strlen(textstring), &sz);
		CONVERT_IMAGE_END;
		return sz.cx;
	}
	CONVERT_IMAGE_END;
	return 0;
}

int
textwidth(LPCWSTR textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		SIZE sz;
		GetTextExtentPoint32W(img->m_hDC, textstring, (int)lstrlenW(textstring), &sz);
		CONVERT_IMAGE_END;
		return sz.cx;
	}
	CONVERT_IMAGE_END;
	return 0;
}

int
textwidth(CHAR c, PIMAGE pimg) {
	CHAR str[2] = {c};
	return textwidth(str, pimg);
}

int
textwidth(WCHAR c, PIMAGE pimg) {
	WCHAR str[2] = {c};
	return textwidth(str, pimg);
}

int
textheight(LPCSTR textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		SIZE sz;
		GetTextExtentPoint32A(img->m_hDC, textstring, (int)strlen(textstring), &sz);
		CONVERT_IMAGE_END;
		return sz.cy;
	}
	CONVERT_IMAGE_END;
	return 0;
}

int
textheight(LPCWSTR textstring, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		SIZE sz;
		GetTextExtentPoint32W(img->m_hDC, textstring, (int)lstrlenW(textstring), &sz);
		CONVERT_IMAGE_END;
		return sz.cy;
	}
	CONVERT_IMAGE_END;
	return 0;
}

int
textheight(CHAR c, PIMAGE pimg) {
	CHAR str[2] = {c};
	return textheight(str, pimg);
}

int
textheight(WCHAR c, PIMAGE pimg) {
	WCHAR str[2] = {c};
	return textheight(str, pimg);
}

void
settextjustify(int horiz, int vert, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		img->m_texttype.horiz = horiz;
		img->m_texttype.vert = vert;
	}
	CONVERT_IMAGE_END;
}

void
getlinestyle(int *plinestyle, unsigned short *pupattern, int *pthickness, PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (plinestyle) {
		*plinestyle = img->m_linestyle.linestyle;
	}
	if (pupattern) {
		*pupattern = img->m_linestyle.upattern;
	}
	if (pthickness) {
		*pthickness = img->m_linestyle.thickness;
	}
	CONVERT_IMAGE_END;
}

void
setlinestyle(int linestyle, unsigned short upattern, int thickness, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);

	if (!(img && img->m_hDC)) {
		CONVERT_IMAGE_END;
		return;
	}

	img->m_linestyle.thickness = thickness;
	img->m_linewidth = (float)thickness;
	img->m_linestyle.linestyle = linestyle;
	img->m_linestyle.upattern = upattern;

	update_pen(img);

	CONVERT_IMAGE_END;
}

void
setlinewidth(float width, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);

	if (img && img->m_hDC) {
		img->m_linestyle.thickness = (int)width;
		img->m_linewidth = width;

		update_pen(img);
	}
	CONVERT_IMAGE_END;
}

void
setfillstyle(int pattern, color_t color, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	LOGBRUSH lbr = {0};
	img->m_fillcolor = color;
	lbr.lbColor = ARGBTOZBGR(color);
	//SetBkColor(img->m_hDC, color);
	if (pattern < SOLID_FILL) {
		lbr.lbHatch = BS_NULL;
	} else if (pattern == SOLID_FILL) {
		lbr.lbHatch = BS_SOLID;
	} else if (pattern < USER_FILL) { // dose not finish
		int hatchmap[] = {
			HS_VERTICAL,
			HS_BDIAGONAL,
			HS_BDIAGONAL,
			HS_FDIAGONAL,
			HS_FDIAGONAL,
			HS_CROSS,
			HS_DIAGCROSS,
			HS_VERTICAL,
			HS_DIAGCROSS,
			HS_DIAGCROSS
		};
		lbr.lbStyle = BS_HATCHED;
		lbr.lbHatch = hatchmap[pattern - 2];
	} else {
		lbr.lbHatch = BS_SOLID;
	}
	HBRUSH hbr = CreateBrushIndirect(&lbr);
	if (hbr) {
		DeleteObject(SelectObject(img->m_hDC, hbr));
	}
	#ifdef EGE_GDIPLUS
		img->set_pattern(NULL);
	#endif
	CONVERT_IMAGE_END;
}

void
setfont(
		int nHeight,
		int nWidth,
		LPCSTR lpszFace,
		int nEscapement,
		int nOrientation,
		int nWeight,
		int bItalic,
		int bUnderline,
		int bStrikeOut,
		BYTE fbCharSet,
		BYTE fbOutPrecision,
		BYTE fbClipPrecision,
		BYTE fbQuality,
		BYTE fbPitchAndFamily,
		PIMAGE pimg)
{
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		LOGFONTA lf = {0};
		lf.lfHeight         = nHeight;
		lf.lfWidth          = nWidth;
		lf.lfEscapement     = nEscapement;
		lf.lfOrientation    = nOrientation;
		lf.lfWeight         = nWeight;
		lf.lfItalic         = (bItalic != 0);
		lf.lfUnderline      = (bUnderline != 0);
		lf.lfStrikeOut      = (bStrikeOut != 0);
		lf.lfCharSet        = fbCharSet;
		lf.lfOutPrecision   = fbOutPrecision;
		lf.lfClipPrecision  = fbClipPrecision;
		lf.lfQuality        = fbQuality;
		lf.lfPitchAndFamily = fbPitchAndFamily;
		lstrcpyA(lf.lfFaceName, lpszFace);
		HFONT hfont = CreateFontIndirectA(&lf);
		DeleteObject(SelectObject(img->m_hDC, hfont));
	}
	CONVERT_IMAGE_END;
}

void
setfont(
		int nHeight,
		int nWidth,
		LPCWSTR lpszFace,
		int nEscapement,
		int nOrientation,
		int nWeight,
		int bItalic,
		int bUnderline,
		int bStrikeOut,
		BYTE fbCharSet,
		BYTE fbOutPrecision,
		BYTE fbClipPrecision,
		BYTE fbQuality,
		BYTE fbPitchAndFamily,
		PIMAGE pimg)
{
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		LOGFONTW lf = {0};
		lf.lfHeight         = nHeight;
		lf.lfWidth          = nWidth;
		lf.lfEscapement     = nEscapement;
		lf.lfOrientation    = nOrientation;
		lf.lfWeight         = nWeight;
		lf.lfItalic         = (bItalic != 0);
		lf.lfUnderline      = (bUnderline != 0);
		lf.lfStrikeOut      = (bStrikeOut != 0);
		lf.lfCharSet        = fbCharSet;
		lf.lfOutPrecision   = fbOutPrecision;
		lf.lfClipPrecision  = fbClipPrecision;
		lf.lfQuality        = fbQuality;
		lf.lfPitchAndFamily = fbPitchAndFamily;
		lstrcpyW(lf.lfFaceName, lpszFace);
		HFONT hfont = CreateFontIndirectW(&lf);
		DeleteObject(SelectObject(img->m_hDC, hfont));
	}
	CONVERT_IMAGE_END;
}

void
setfont(
		int nHeight,
		int nWidth,
		LPCSTR lpszFace,
		int nEscapement,
		int nOrientation,
		int nWeight,
		int bItalic,
		int bUnderline,
		int bStrikeOut,
		PIMAGE pimg)
{
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		LOGFONTA lf = {0};
		lf.lfHeight         = nHeight;
		lf.lfWidth          = nWidth;
		lf.lfEscapement     = nEscapement;
		lf.lfOrientation    = nOrientation;
		lf.lfWeight         = nWeight;
		lf.lfItalic         = (bItalic != 0);
		lf.lfUnderline      = (bUnderline != 0);
		lf.lfStrikeOut      = (bStrikeOut != 0);
		lf.lfCharSet        = DEFAULT_CHARSET;
		lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
		lf.lfQuality        = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH;
		lstrcpyA(lf.lfFaceName, lpszFace);
		HFONT hfont = CreateFontIndirectA(&lf);
		DeleteObject(SelectObject(img->m_hDC, hfont));
	}
	CONVERT_IMAGE_END;
}

void
setfont(
		int nHeight,
		int nWidth,
		LPCWSTR lpszFace,
		int nEscapement,
		int nOrientation,
		int nWeight,
		int bItalic,
		int bUnderline,
		int bStrikeOut,
		PIMAGE pimg)
{
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		LOGFONTW lf = {0};
		lf.lfHeight         = nHeight;
		lf.lfWidth          = nWidth;
		lf.lfEscapement     = nEscapement;
		lf.lfOrientation    = nOrientation;
		lf.lfWeight         = nWeight;
		lf.lfItalic         = (bItalic != 0);
		lf.lfUnderline      = (bUnderline != 0);
		lf.lfStrikeOut      = (bStrikeOut != 0);
		lf.lfCharSet        = DEFAULT_CHARSET;
		lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
		lf.lfQuality        = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH;
		lstrcpyW(lf.lfFaceName, lpszFace);
		HFONT hfont = CreateFontIndirectW(&lf);
		DeleteObject(SelectObject(img->m_hDC, hfont));
	}
	CONVERT_IMAGE_END;
}

void
setfont(int nHeight, int nWidth, LPCSTR lpszFace, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		LOGFONTA lf = {0};
		lf.lfHeight         = nHeight;
		lf.lfWidth          = nWidth;
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
		lstrcpyA(lf.lfFaceName, lpszFace);
		HFONT hfont = CreateFontIndirectA(&lf);
		DeleteObject(SelectObject(img->m_hDC, hfont));
	}
	CONVERT_IMAGE_END;
}

void
setfont(int nHeight, int nWidth, LPCWSTR lpszFace, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		LOGFONTW lf = {0};
		lf.lfHeight         = nHeight;
		lf.lfWidth          = nWidth;
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
		lstrcpyW(lf.lfFaceName, lpszFace);
		HFONT hfont = CreateFontIndirectW(&lf);
		DeleteObject(SelectObject(img->m_hDC, hfont));
	}
	CONVERT_IMAGE_END;
}

void
setfont(const LOGFONTA *font, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		HFONT hfont = CreateFontIndirectA(font);
		DeleteObject(SelectObject(img->m_hDC, hfont));
	}
	CONVERT_IMAGE_END;
}

void
setfont(const LOGFONTW *font, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		HFONT hfont = CreateFontIndirectW(font);
		DeleteObject(SelectObject(img->m_hDC, hfont));
	}
	CONVERT_IMAGE_END;
}

void
getfont(LOGFONTA *font, PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		HFONT hf = (HFONT)GetCurrentObject(img->m_hDC, OBJ_FONT);
		GetObjectA(hf, sizeof(LOGFONTA), font);
	}
	CONVERT_IMAGE_END;
}

void
getfont(LOGFONTW *font, PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		HFONT hf = (HFONT)GetCurrentObject(img->m_hDC, OBJ_FONT);
		GetObjectW(hf, sizeof(LOGFONTW), font);
	}
	CONVERT_IMAGE_END;
}

void setrendermode(rendermode_e mode) {
	if (mode == RENDER_MANUAL) {
		struct _graph_setting * pg = &graph_setting;
		if (pg->lock_window) {
			;
		} else {
			KillTimer(pg->hwnd, RENDER_TIMER_ID);
			pg->timer_stop_mark = true;
			PostMessageW(pg->hwnd, WM_TIMER, RENDER_TIMER_ID, 0);
			pg->lock_window = true;
			while (pg->timer_stop_mark) {
				::Sleep(1);
			}
		}
	} else {
		struct _graph_setting * pg = &graph_setting;
		delay_ms(0);
		SetTimer(pg->hwnd, RENDER_TIMER_ID, 50, NULL);
		pg->skip_timer_mark = false;
		pg->lock_window = false;
	}
}

void
setactivepage(int page) {
	struct _graph_setting * pg = &graph_setting;
	if (0 <= page && page < BITMAP_PAGE_SIZE) {
		pg->active_page = page;
		
		if (pg->img_page[page] == NULL) {
			pg->img_page[page] = new IMAGE(pg->dc_w, pg->dc_h);
		}

		pg->imgtarget = pg->img_page[page];
		pg->dc = pg->img_page[page]->m_hDC;
	}
}

void
setvisualpage(int page) {
	struct _graph_setting * pg = &graph_setting;
	if (0 <= page && page < BITMAP_PAGE_SIZE) {
		pg->visual_page = page;
		if (pg->img_page[page] == NULL) {
			pg->img_page[page] = new IMAGE(pg->dc_w, pg->dc_h);
		}
		pg->update_mark_count = 0;
	}
}

void
swappage() {
	struct _graph_setting * pg = &graph_setting;
	setvisualpage(pg->active_page);
	setactivepage(1 - pg->active_page);
}

void
window_getviewport(struct viewporttype * viewport) {
	struct _graph_setting * pg = &graph_setting;
	viewport->left   = pg->base_x;
	viewport->top    = pg->base_y;
	viewport->right  = pg->base_w + pg->base_x;
	viewport->bottom = pg->base_h + pg->base_y;
}

void
window_getviewport(int* left, int* top, int* right, int* bottom) {
	struct _graph_setting * pg = &graph_setting;
	if (left)   *left   = pg->base_x;
	if (top)    *top    = pg->base_y;
	if (right)  *right  = pg->base_w + pg->base_x;
	if (bottom) *bottom = pg->base_h + pg->base_y;
}

void
window_setviewport(int left, int top, int right, int bottom) {
	struct _graph_setting * pg = &graph_setting;
	int same_xy = 0, same_wh = 0;
	if (pg->base_x == left && pg->base_y == top) {
		same_xy = 1;
	}
	if (pg->base_w == bottom - top && pg->base_h == right - left) {
		same_wh = 1;
	}
	pg->base_x = left;
	pg->base_y = top;
	pg->base_w = right - left;
	pg->base_h = bottom - top;
	if (same_xy == 0 || same_wh == 0) {
		graph_setting.update_mark_count -= 1;
	}
	/*修正窗口大小*/
	if (same_wh == 0) {
		RECT rect, crect;
		int dw, dh;
		GetClientRect(pg->hwnd, &crect);
		GetWindowRect(pg->hwnd, &rect);
		dw = pg->base_w - crect.right;
		dh = pg->base_h - crect.bottom;
		{
			HWND hwnd = GetParent(pg->hwnd);
			if (hwnd) {
				POINT pt = {0, 0};
				ClientToScreen(hwnd, &pt);
				rect.left   -= pt.x;
				rect.top    -= pt.y;
				rect.right  -= pt.x;
				rect.bottom -= pt.y;
			}

			MoveWindow(
				pg->hwnd,
				rect.left,
				rect.top,
				rect.right+dw-rect.left,
				rect.bottom+dh-rect.top,
				TRUE);
		}
	}
}

void
getviewport(int *pleft, int *ptop, int *pright, int *pbottom, int *pclip, PCIMAGE pimg) {
	PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if(pleft)   *pleft  = img->m_vpt.left;
	if (ptop)   *ptop   = img->m_vpt.top;
	if (pright) *pright = img->m_vpt.right;
	if (pbottom)*pbottom= img->m_vpt.bottom;
	if (pclip)  *pclip  = img->m_vpt.clipflag;
	CONVERT_IMAGE_END;
}

void
setviewport(int left, int top, int right, int bottom, int clip, PIMAGE pimg) {
	//struct _graph_setting * pg = &graph_setting;

	PIMAGE img = CONVERT_IMAGE(pimg);

	SetViewportOrgEx(img->m_hDC, 0, 0, NULL);

	img->m_vpt.left     = left;
	img->m_vpt.top      = top;
	img->m_vpt.right    = right;
	img->m_vpt.bottom   = bottom;
	img->m_vpt.clipflag = clip;

	if (img->m_vpt.left < 0) {
		img->m_vpt.left = 0;
	}
	if (img->m_vpt.top < 0) {
		img->m_vpt.top = 0;
	}
	if (img->m_vpt.right > img->m_width) {
		img->m_vpt.right = img->m_width;
	}
	if (img->m_vpt.bottom > img->m_height) {
		img->m_vpt.bottom = img->m_height;
	}

	HRGN rgn = NULL;
	if (img->m_vpt.clipflag) {
		rgn = CreateRectRgn(
			img->m_vpt.left,
			img->m_vpt.top,
			img->m_vpt.right,
			img->m_vpt.bottom
			);
	} else {
		rgn = CreateRectRgn(
			0,
			0,
			img->m_width,
			img->m_height
			);
	}
	SelectClipRgn(img->m_hDC, rgn);
	DeleteObject(rgn);

	//OffsetViewportOrgEx(img->m_hDC, img->m_vpt.left, img->m_vpt.top, NULL);
	SetViewportOrgEx(img->m_hDC, img->m_vpt.left, img->m_vpt.top, NULL);

	CONVERT_IMAGE_END;
}

void
clearviewport(PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);

	if (img && img->m_hDC) {
		RECT rect = {
			0,
			0,
			img->m_vpt.right - img->m_vpt.left,
			img->m_vpt.bottom - img->m_vpt.top
		};
		HBRUSH hbr = CreateSolidBrush(GetBkColor(img->m_hDC));
		FillRect(img->m_hDC, &rect, hbr);
		DeleteObject(hbr);
	}
	CONVERT_IMAGE_END;
}

#ifdef EGE_GDIPLUS
Gdiplus::DashStyle linestyle_to_dashstyle(int linestyle){
	switch(linestyle){
	case SOLID_LINE:
		return Gdiplus::DashStyleSolid;
	case PS_DASH:
		return Gdiplus::DashStyleDash;
	case PS_DOT:
		return Gdiplus::DashStyleDot;
	case PS_DASHDOT:
		return Gdiplus::DashStyleDashDot;
	}
	return Gdiplus::DashStyleSolid;
}

void
ege_line(float x1, float y1, float x2, float y2, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (img->m_linestyle.linestyle == PS_NULL) 
			return;
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Pen* pen=img->getPen();
		graphics->DrawLine(pen, x1, y1, x2, y2);
	}
	CONVERT_IMAGE_END;
}

void
ege_drawpoly(int numpoints, ege_point* polypoints, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (img->m_linestyle.linestyle == PS_NULL)
			return;
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Pen* pen=img->getPen();
		graphics->DrawLines(pen, (Gdiplus::PointF*)polypoints, numpoints);
	}
	CONVERT_IMAGE_END;
}

void
ege_drawcurve(int numpoints, ege_point* polypoints, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (img->m_linestyle.linestyle == PS_NULL)
			return;
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Pen* pen=img->getPen();
		graphics->DrawCurve(pen, (Gdiplus::PointF*)polypoints, numpoints);
	}
	CONVERT_IMAGE_END;
}

void
ege_rectangle(float x, float y, float w, float h, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (img->m_linestyle.linestyle == PS_NULL)
			return;
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Pen* pen=img->getPen();
		graphics->DrawRectangle(pen, x, y, w, h);
	}
	CONVERT_IMAGE_END;
}

void
ege_ellipse(float x, float y, float w, float h, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (img->m_linestyle.linestyle == PS_NULL)
			return;
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Pen* pen=img->getPen();
		graphics->DrawEllipse(pen, x, y, w, h);
	}
	CONVERT_IMAGE_END;
}

void
ege_pie(float x, float y, float w, float h, float stangle, float sweepAngle, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (img->m_linestyle.linestyle == PS_NULL)
			return;
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Pen* pen=img->getPen();
		graphics->DrawPie(pen, x, y, w, h, stangle, sweepAngle);
	}
	CONVERT_IMAGE_END;
}

void
ege_arc(float x, float y, float w, float h, float stangle, float sweepAngle, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (img->m_linestyle.linestyle == PS_NULL)
			return;
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Pen* pen=img->getPen();
		graphics->DrawArc(pen, x, y, w, h, stangle, sweepAngle);
	}
	CONVERT_IMAGE_END;
}

void
ege_bezier(int numpoints, ege_point* polypoints, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (img->m_linestyle.linestyle == PS_NULL)
			return;
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Pen* pen=img->getPen();
		graphics->DrawBeziers(pen, (Gdiplus::PointF*)polypoints, numpoints);
	}
	CONVERT_IMAGE_END;
}

void
ege_setpattern_none(PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	img->set_pattern(NULL);
	CONVERT_IMAGE_END;
}

void
ege_setpattern_lineargradient(float x1, float y1, color_t c1, float x2, float y2, color_t c2, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::LinearGradientBrush* pbrush = new Gdiplus::LinearGradientBrush(
			Gdiplus::PointF(x1, y1),
			Gdiplus::PointF(x2, y2),
			Gdiplus::Color(c1),
			Gdiplus::Color(c2)
			);
		img->set_pattern(pbrush);
	}
	CONVERT_IMAGE_END;
}

void
ege_setpattern_pathgradient(ege_point center, color_t centercolor,
	int count, ege_point* points, int colcount, color_t* pointscolor, PIMAGE pimg)
{
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::PathGradientBrush* pbrush = new Gdiplus::PathGradientBrush(
			(Gdiplus::PointF*)points,
			count,
			Gdiplus::WrapModeTile
			);
		pbrush->SetCenterColor(Gdiplus::Color(centercolor));
		pbrush->SetCenterPoint(Gdiplus::PointF(center.x, center.y));
		pbrush->SetSurroundColors((Gdiplus::Color*)pointscolor, &colcount);
		img->set_pattern(pbrush);
	}
	CONVERT_IMAGE_END;
}

void
ege_setpattern_ellipsegradient(ege_point center, color_t centercolor,
	float x, float y, float w, float h, color_t color, PIMAGE pimg)
{
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::GraphicsPath path;
		path.AddEllipse(x, y, w, h);
		Gdiplus::PathGradientBrush* pbrush = new Gdiplus::PathGradientBrush(
			&path
			);
		int count = 1;
		pbrush->SetCenterColor(Gdiplus::Color(centercolor));
		pbrush->SetCenterPoint(Gdiplus::PointF(center.x, center.y));
		pbrush->SetSurroundColors((Gdiplus::Color*)&color, &count);
		img->set_pattern(pbrush);
	}
	CONVERT_IMAGE_END;
}

void
ege_setpattern_texture(PIMAGE srcimg, float x, float y, float w, float h, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (srcimg->m_texture) {
			Gdiplus::TextureBrush* pbrush = new Gdiplus::TextureBrush(
				(Gdiplus::Image*)srcimg->m_texture,
				Gdiplus::WrapModeTile,
				x, y, w, h);
			img->set_pattern(pbrush);
		}
	}
	CONVERT_IMAGE_END;
}

void
ege_fillpoly(int numpoints, ege_point* polypoints, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Brush* brush=img->getBrush();
		graphics->FillPolygon(brush, (Gdiplus::PointF*)polypoints, numpoints);
	}
	CONVERT_IMAGE_END;
}

void
ege_fillrect(float x, float y, float w, float h, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Brush* brush=img->getBrush();
		graphics->FillRectangle(brush, x, y, w, h);
	}
	CONVERT_IMAGE_END;
}

void
ege_fillellipse(float x, float y, float w, float h, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Brush* brush=img->getBrush();
		graphics->FillEllipse(brush, x, y, w, h);
	}
	CONVERT_IMAGE_END;
}

void
ege_fillpie(float x, float y, float w, float h, float stangle, float sweepAngle, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Brush* brush=img->getBrush();
		graphics->FillPie(brush, x, y, w, h, stangle, sweepAngle);
	}
	CONVERT_IMAGE_END;
}

void
ege_setalpha(int alpha, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img && img->m_hDC) {
		int a = alpha << 24;
		int len = img->m_width * img->m_height;
		for (int i = 0; i < len; ++i)
		{
			DWORD c = img->m_pBuffer[i];
			img->m_pBuffer[i] = a | (c & 0xFFFFFF);
		}
	}
	CONVERT_IMAGE_END;
}

void
ege_gentexture(bool gen, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		img->gentexture(gen);
	}
	CONVERT_IMAGE_END;
}

void
ege_puttexture(PCIMAGE srcimg, float x, float y, float w, float h, PIMAGE pimg) {
	ege_rect dest = {x, y, w, h};
	ege_puttexture(srcimg, dest, pimg);
}

void
ege_puttexture(PCIMAGE srcimg, ege_rect dest, PIMAGE pimg) {
	ege_rect src;
	PIMAGE img = CONVERT_IMAGE_CONST(pimg);
	if (img) {
		src.x = 0;
		src.y = 0;
		src.w = (float)srcimg->getwidth();
		src.h = (float)srcimg->getheight();
		ege_puttexture(srcimg, dest, src, img);
	}
	CONVERT_IMAGE_END;
}

void
ege_puttexture(PCIMAGE srcimg, ege_rect dest, ege_rect src, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		if (srcimg->m_texture) {
			Gdiplus::Graphics* graphics=img->getGraphics();
			/*
			Gdiplus::ImageAttributes ia;
			Gdiplus::ColorMatrix mx = {
				{
					{1.0, 0.0, 0.0, 0.0, 0.0},
					{0.0, 1.0, 0.0, 0.0, 0.0},
					{0.0, 0.0, 1.0, 0.0, 0.0},
					{0.0, 0.0, 0.0, 1.0, 0.0},
					{0.0, 0.0, 0.0, 0.0, 1.0},
				}
			};
			ia.SetColorMatrix(&mx);
			// */
			//graphics.SetTransform();
			graphics->DrawImage(
				(Gdiplus::Image*)srcimg->m_texture,
				Gdiplus::RectF(dest.x, dest.y, dest.w, dest.h),
				src.x,
				src.y,
				src.w,
				src.h,
				Gdiplus::UnitPixel,
				NULL
				);
		}
	}
	CONVERT_IMAGE_END;
}

// TODO: 错误处理
static void ege_drawtext_p(LPCWSTR textstring, float x, float y, PIMAGE img) {
	Gdiplus::Graphics* graphics=img->getGraphics();

	HFONT hf = (HFONT)GetCurrentObject(img->m_hDC, OBJ_FONT);
	LOGFONT lf;
	GetObject(hf, sizeof(LOGFONT), &lf);
	if (strcmp(lf.lfFaceName, "System") == 0) {
		hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	}

	Gdiplus::Font font(img->m_hDC, hf);
	// if (!font.IsAvailable()) {
	// 	fprintf(stderr, "!font.IsAvailable(), hf: %p\n", hf);
	// }
	Gdiplus::PointF origin(x, y);
	Gdiplus::SolidBrush brush(img->m_color);
	graphics->DrawString(textstring, -1, &font, origin, &brush);
	// int err;
	// if (err = graphics.DrawString(textstring, -1, &font, origin, &brush)) {
	// 	fprintf(stderr, "DrawString Err: %d\n", err);
	// }
}

void EGEAPI ege_drawtext(LPCSTR  textstring, float x, float y, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img && img->m_hDC) {
		int bufferSize = MultiByteToWideChar(CP_ACP, 0, textstring, -1, NULL, 0);
		if (bufferSize < 128) {
			WCHAR wStr[128];
			MultiByteToWideChar(CP_ACP, 0, textstring, -1, wStr, 128);
			ege_drawtext_p(wStr, x, y, img);
		} else {
			const std::wstring& wStr = mb2w(textstring);
			ege_drawtext_p(wStr.c_str(), x, y, img);
		}
	}
	CONVERT_IMAGE_END;
}


void EGEAPI ege_drawtext(LPCWSTR textstring, float x, float y, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img && img->m_hDC) {
		ege_drawtext_p(textstring, x, y, img);
	}
	CONVERT_IMAGE_END;
}

void EGEAPI ege_drawimage(PCIMAGE srcimg,int dstX, int dstY,PIMAGE pimg){
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img && srcimg) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Bitmap bitmap(srcimg->getwidth(),srcimg->getheight(),4*srcimg->getwidth(),
			PixelFormat32bppARGB,(BYTE *)(srcimg->m_pBuffer));
		Gdiplus::Point p(dstX,dstY); 
		graphics->DrawImage(&bitmap,p);
	}
	CONVERT_IMAGE_END;	
} 

void EGEAPI ege_drawimage(PCIMAGE srcimg,int dstX, int dstY, int dstWidth, int dstHeight,
		int srcX, int srcY, int srcWidth, int srcHeight,PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img && srcimg) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Bitmap bitmap(srcimg->getwidth(),srcimg->getheight(),4*srcimg->getwidth(),
			PixelFormat32bppARGB,(BYTE *)(srcimg->m_pBuffer));
		Gdiplus::Point destPoints[3] = {
   			Gdiplus::Point(dstX, dstY),
   			Gdiplus::Point(dstX+dstWidth, dstY),
   			Gdiplus::Point(dstX, dstY+dstHeight)};
		graphics->DrawImage(&bitmap,destPoints,3,srcX,srcY,srcWidth,srcHeight,Gdiplus::UnitPixel,NULL,NULL,NULL);
	}
	CONVERT_IMAGE_END;			
}

void EGEAPI ege_transform_rotate(float angle,PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		graphics->RotateTransform(angle);		
	}
	CONVERT_IMAGE_END;	
}

void EGEAPI ege_transform_translate(float x,float y,PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		graphics->TranslateTransform(x,y);		
	}
	CONVERT_IMAGE_END;	
}

void EGEAPI ege_transform_scale(float scale_x, float scale_y,PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		graphics->ScaleTransform(scale_x,scale_y);		
	}
	CONVERT_IMAGE_END;	
}

void EGEAPI ege_transform_reset(PIMAGE pimg){
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		graphics->ResetTransform();
	}
	CONVERT_IMAGE_END;	
}
void EGEAPI ege_get_transform(ege_transform_matrix* pmatrix, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Matrix m;
		Gdiplus::REAL elements[6];
		graphics->GetTransform(&m);
		m.GetElements(elements);
		pmatrix->m11 = elements[0];
		pmatrix->m12 = elements[1];
		pmatrix->m21 = elements[2];
		pmatrix->m22 = elements[3];
		pmatrix->m31 = elements[4];
		pmatrix->m32 = elements[5];		
	}
	CONVERT_IMAGE_END;	
}

void EGEAPI ege_set_transform(ege_transform_matrix* const pmatrix, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Matrix m(pmatrix->m11,pmatrix->m12,pmatrix->m21,pmatrix->m22,pmatrix->m31,pmatrix->m32);
		graphics->SetTransform(&m);
	}
	CONVERT_IMAGE_END;		
}

ege_point EGEAPI ege_transform_calc(ege_point p, PIMAGE pimg) {
	return ege_transform_calc(p.x,p.y,pimg);		
}

ege_point EGEAPI ege_transform_calc(float x, float y, PIMAGE pimg) {
	PIMAGE img = CONVERT_IMAGE(pimg);
	ege_point p;
	if (img) {
		Gdiplus::Graphics* graphics=img->getGraphics();
		Gdiplus::Matrix m;
		Gdiplus::REAL elements[6],m11,m12,m21,m22,m31,m32;
		graphics->GetTransform(&m);
		m.GetElements(elements);
		m11 = elements[0];
		m12 = elements[1];
		m21 = elements[2];
		m22 = elements[3];
		m31 = elements[4];
		m32 = elements[5];	
		p.x = x*m11+y*m21+m31;
		p.y = x*m12+y*m22+m32;
	} else {
		p.x = 0;
		p.y = 0;
	}
	CONVERT_IMAGE_END;
	return p;
}


#endif //EGEGDIPLUS

HWND
getHWnd() {
	struct _graph_setting * pg = &graph_setting;
	return pg->hwnd;
}

HINSTANCE
getHInstance() {
	struct _graph_setting * pg = &graph_setting;
	return pg->instance;
}

int
message_addkeyhandler(void* param, LPMSG_KEY_PROC func) {
	struct _graph_setting * pg = &graph_setting;
	pg->callback_key = func;
	pg->callback_key_param = param;
	return grOk;
}

int
message_addmousehandler(void* param, LPMSG_MOUSE_PROC func) {
	struct _graph_setting * pg = &graph_setting;
	pg->callback_mouse = func;
	pg->callback_mouse_param = param;
	return grOk;
}

int
SetCloseHandler(LPCALLBACK_PROC func) {
	struct _graph_setting * pg = &graph_setting;
	pg->callback_close = func;
	return grOk;
}

/* private funcion */
static
void
draw_frame(PIMAGE img, int l, int t, int r, int b, color_t lc, color_t dc) {
	setcolor(lc, img);
	line(l, b, l, t, img);
	lineto(r, t, img);
	setcolor(dc, img);
	lineto(r, b, img);
	lineto(l, b, img);
}

int
inputbox_getline(LPCSTR title, LPCSTR text, LPSTR buf, int len) {
	const std::wstring& title_w = mb2w(title);
	const std::wstring& text_w = mb2w(text);
	std::wstring buf_w(len, L'\0');
	int ret = inputbox_getline(title_w.c_str(), text_w.c_str(), &buf_w[0], len);
	if (ret) {
		WideCharToMultiByte(CP_ACP, 0, buf_w.c_str(), -1, buf, len, 0, 0);
	}
	return ret;
}

int
inputbox_getline(LPCWSTR title, LPCWSTR text, LPWSTR buf, int len) {
	struct _graph_setting * pg = &graph_setting;
	IMAGE bg;
	IMAGE window;
	int w = 400, h = 300, x = (getwidth() - w) / 2, y = (getheight() - h) / 2;
	int ret = 0;

	bg.getimage(0, 0, getwidth(), getheight());
	window.resize(w, h);
	buf[0] = 0;

	sys_edit edit(true);
	edit.create(true);
	edit.move(x+30+1, y+192+1);
	edit.size(w-(30+1)*2, h-40-192-2);
	edit.setmaxlen(len);
	edit.visible(true);
	edit.setfocus();
	
	setbkcolor(EGERGB(0x80, 0xA0, 0x80), &window);
	draw_frame(&window, 0, 0, w-1, h-1, EGERGB(0xA0, 0xC0, 0xA0), EGERGB(0x50, 0x70, 0x50));
	setfillcolor(EGERGB(0, 0, 0xA0), &window);
	for (int dy = 1; dy<24; dy++) {
		setcolor(HSLtoRGB(240.0f, 1.0f, 0.5f + float(dy/24.0*0.3)), &window);
		line(1, dy, w-1, dy, &window);
	}
	setcolor(0xFFFFFF, &window);
	setbkmode(TRANSPARENT, &window);
	setfont(18, 0, L"Tahoma", &window);
	outtextxy(3, 3, title, &window);
	setcolor(0x0, &window);
	{
		RECT rect = {30, 32, w-30, 128-3};
		DrawTextW(window.m_hDC, text, -1, &rect, DT_NOPREFIX|DT_LEFT|DT_TOP|TA_NOUPDATECP|DT_WORDBREAK|DT_EDITCONTROL|DT_EXPANDTABS);
	}	
	putimage(0, 0, &bg);
	putimage(x, y, &window);
	delay_ms(0);
	while(is_run()) {
		key_msg msg	= getkey();
		if (msg.key == key_enter && msg.msg == key_msg_up) {
			break;
		}
	}
	edit.gettext(len, buf);
	len = lstrlenW(buf);
	while (len > 0 && (buf[len - 1] == '\r' || buf[len - 1] == '\n')) {
		buf[--len] = 0;
	}
	ret = len;
	putimage(0, 0, &bg);
	delay_ms(0);
	getflush();
	return ret;
}

float
EGE_PRIVATE_GetFPS(int add) {//获取帧数
	static int      fps = 0;
	static int      fps_inv = 0;
	static double   time = 0;
	static float    flret = 0;
	static float    fret = 0;
	static float    fret_inv = 0;

	struct _graph_setting * pg = &graph_setting;
	double cur = get_highfeq_time_ls(pg);
	if (add == 0x100) {
		fps += 1;
	} else if (add == -0x100) {
		fps += 1;
		fps_inv += 1;
	}
	if (cur - time >= 0.5) {
		flret = fret;
		fret = (float)(fps / (cur - time));
		fret_inv = (float)((fps - fps_inv) / (cur - time));
		fps = 0;
		fps_inv = 0;
		time = cur;
	}
	if (add >0) {
		return (fret + flret) / 2;
	} else {
		return fret_inv;
	}
}

float
getfps() {
	return EGE_PRIVATE_GetFPS(0);
}

double
fclock() {
	struct _graph_setting * pg = &graph_setting;
	if (pg->fclock_start == 0) {
		pg->fclock_start = ::GetTickCount();
	}
	return (::GetTickCount() - pg->fclock_start) / 1000.0; //get_highfeq_time_ls(pg);
}

int
ege_compress(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen) {
	if (sourceLen == 0) {
		return -1;
	}
	{
		int ret = compress(
			(Bytef*)dest + sizeof(unsigned long),
			(uLongf*)destLen,
			(Bytef*)source,
			(uLong)sourceLen
			);
		((unsigned long *)dest)[0] = sourceLen;
		*destLen += 4;
		return ret;
	}
}

int
ege_compress2(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen, int level) {
	if (sourceLen == 0) {
		return -1;
	}
	{
		int ret = compress2(
			(Bytef*)dest + sizeof(unsigned long),
			(uLongf*)destLen,
			(Bytef*)source,
			(uLong)sourceLen,
			level
			);
		*(unsigned long *)dest = sourceLen;
		*destLen += sizeof(unsigned long);
		return ret;
	}
}

unsigned long
ege_uncompress_size(const void *source, unsigned long sourceLen) {
	if (sourceLen > sizeof(unsigned long)) {
		return ((unsigned long *)source)[0];
	} else {
		return 0;
	}
}

int
ege_uncompress(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen) {
	*(uLongf*)destLen = ege_uncompress_size(source, sourceLen);
	if (*(uLongf*)destLen > 0) {
		int ret = uncompress(
			(Bytef*)dest,
			(uLongf*)destLen,
			(Bytef*)source + sizeof(unsigned long),
			(uLong)sourceLen - sizeof(unsigned long)
			);
		return ret;
	} else {
		return -1;
	}
}

LRESULT sys_edit::onMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_CTLCOLOREDIT: 
			{
				HDC dc = (HDC)wParam;
				HBRUSH br = ::CreateSolidBrush(ARGBTOZBGR(m_bgcolor));
	
				::SetBkColor(dc, ARGBTOZBGR(m_bgcolor));
				::SetTextColor(dc, ARGBTOZBGR(m_color));
				::DeleteObject(m_hBrush);
				m_hBrush = br;
				return (LRESULT)br;			
			}
			break;
		case WM_SETFOCUS:
			m_bInputFocus = 1;
			// call textbox's own message process to show caret
			return ((LRESULT (CALLBACK *)(HWND, UINT, WPARAM, LPARAM))m_callback)(m_hwnd, message, wParam, lParam);		
		case WM_KILLFOCUS:
			m_bInputFocus = 0;
			// call textbox's own message process to hide caret
			return ((LRESULT (CALLBACK *)(HWND, UINT, WPARAM, LPARAM))m_callback)(m_hwnd, message, wParam, lParam);
		default:
			return ((LRESULT (CALLBACK *)(HWND, UINT, WPARAM, LPARAM))m_callback)(m_hwnd, message, wParam, lParam);
	}
}

} // namespace ege
