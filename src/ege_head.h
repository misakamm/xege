/*
* EGE (Easy Graphics Engine)
* filename  ege_head.cpp

公共声明头文件（私有，不对外使用）
*/

#ifndef _EGE_HEAD_H_
#define _EGE_HEAD_H_

#ifndef _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#endif
#ifndef _ALLOW_RUNTIME_LIBRARY_MISMATCH
#define _ALLOW_RUNTIME_LIBRARY_MISMATCH
#endif


#define EGE_TOSTR_(x)  #x
#define EGE_TOSTR(x)   EGE_TOSTR_(x)

#define EGE_L_(str)    L##str
#define EGE_L(str)     EGE_L_(str)

//编译器版本，目前仅支持 MSVC/MinGW
#ifdef _WIN64
#	define SYSBITS "x64"
#else
#	define SYSBITS "x86"
#endif

#define SYSBITS_W  EGE_L(SYSBITS)

#ifdef _MSC_VER
#	if (_MSC_VER >= 1930)
#		define MSVC_VER "MSVC"
#	elif (_MSC_VER >= 1920)
#		define MSVC_VER "VC2019"
#	elif (_MSC_VER >= 1910)
#		define MSVC_VER "VC2017"
#	elif (_MSC_VER >= 1900)
#		define MSVC_VER "VC2015"
#	elif (_MSC_VER >= 1800)
#		define MSVC_VER "VC2013"
#	elif (_MSC_VER >= 1700)
#		define MSVC_VER "VC2012"
#	elif (_MSC_VER >= 1600)
#		define MSVC_VER "VC2010"
#	elif (_MSC_VER >= 1500)
#		define MSVC_VER "VC2008"
#	elif (_MSC_VER > 1200)
#		define MSVC_VER "VC2005"
#	else
#		define MSVC_VER "VC6"
#	endif
#	define COMPILER_VER     MSVC_VER SYSBITS
#	define COMPILER_VER_W   EGE_L(MSVC_VER) SYSBITS_W
#else
#	define GCC_VER          EGE_TOSTR(__GNUC__) "." EGE_TOSTR(__GNUC_MINOR__)
#	define GCC_VER_W        EGE_L(EGE_TOSTR(__GNUC__)) L"." EGE_L(EGE_TOSTR(__GNUC_MINOR__))
#	define COMPILER_VER     "GCC"   GCC_VER    SYSBITS
#	define COMPILER_VER_W   L"GCC"  GCC_VER_W  SYSBITS_W
#endif

#define EGE_VERSION_YEAR  20
#define EGE_VERSION_MONTH 05

#define EGE_VERSION_INT EGE_VERSION_YEAR * 100 + EGE_VERSION_MONTH
#define EGE_VERSION     EGE_TOSTR(EGE_VERSION_YEAR) "." EGE_TOSTR(EGE_VERSION_MONTH)
#define EGE_VERSION_W   EGE_L(EGE_TOSTR(EGE_VERSION_YEAR)) L"." EGE_L(EGE_TOSTR(EGE_VERSION_MONTH))
#define EGE_TITLE       "EGE"  EGE_VERSION   " "  COMPILER_VER
#define EGE_TITLE_W     L"EGE" EGE_VERSION_W L" " COMPILER_VER_W

#define EGE_WNDCLSNAME    "Easy Graphics Engine"
#define EGE_WNDCLSNAME_W  EGE_L(EGE_WNDCLSNAME)

// MSVC 从 10.0（VS2010）开始有 stdint.h
// GCC 从 4.5 开始有 stdint.h
#if _MSC_VER >= 1600 || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#  include <stdint.h>
#elif !defined(_MSC_VER) || _MSC_VER > 1300
#  include "stdint.h"
#endif

#include <string>

#define _GRAPH_LIB_BUILD_
#include "ege.h"
#include "ege/egecontrolbase.h"
#include "thread_queue.h"

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#endif

#ifndef ASSERT_TRUE
#ifdef _DEBUG
#include <cassert>
#define ASSERT_TRUE(e) assert((e) != MUSIC_ERROR)
#else
#define ASSERT_TRUE(e) (void(0))
#endif
#endif

#define EGE_GDIPLUS // 使用gdi+函数扩展

#ifdef EGE_GDIPLUS
#	include <gdiplus.h>
#endif

#define QUEUE_LEN               1024
#define BITMAP_PAGE_SIZE	    4
#define BITMAP_PAGE_MIN_SIZE    1
#define UPDATE_MAX_CALL         0xFF
#define RENDER_TIMER_ID         916
#define IMAGE_INIT_FLAG         0x20100916
#define MAX_KEY_VCODE           256
#define FLOAT_EPS               1e-3f

#ifndef SWAP
#define SWAP(_a, _b, _t) {_t = _a; _a = _b; _b = _t; }
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define IFATODOB(A, B)  ( (A) && (B, 0) )
#define IFNATODOB(A, B) ( (A) || (B, 0) )

#define CONVERT_IMAGE(pimg) ( ((size_t)(pimg)<0x20 ?\
		((pimg) ?\
			(graph_setting.img_page[(size_t)(pimg) & 0xF])\
		: (--graph_setting.update_mark_count, graph_setting.imgtarget))\
	: pimg) )

#define CONVERT_IMAGE_CONST(pimg) ( (size_t)(pimg)<0x20 ?\
		((pimg) ?\
			(graph_setting.img_page[(size_t)(pimg) & 0xF])\
		: graph_setting.imgtarget)\
	: pimg)

#define CONVERT_IMAGE_F(pimg) CONVERT_IMAGE(pimg)

#define CONVERT_IMAGE_F_CONST(pimg) CONVERT_IMAGE_CONST(pimg)

#define CONVERT_IMAGE_END

#ifndef DEFAULT_CHARSET
#define DEFAULT_CHARSET ANSI_CHARSET
#endif

#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif

#ifndef __int3264
#if defined(_WIN64)
typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

#define __int3264   __int64

#else
typedef _W64 long LONG_PTR, *PLONG_PTR;
typedef _W64 unsigned long ULONG_PTR, *PULONG_PTR;

#define __int3264   __int32

#endif
#endif

typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

typedef unsigned int uint32;

#if !defined(_MSC_VER) || _MSC_VER > 1200
typedef intptr_t POINTER_SIZE;
#else
typedef long POINTER_SIZE;
#endif

#ifndef _MSC_VER
#define GRADIENT_FILL_RECT_H    0x00000000
#define GRADIENT_FILL_RECT_V    0x00000001
#define GRADIENT_FILL_TRIANGLE  0x00000002
#define GRADIENT_FILL_OP_FLAG   0x000000ff

extern "C" {
WINGDIAPI
BOOL
WINAPI
GradientFill(
	HDC hdc,
	PTRIVERTEX pVertex,
	ULONG nVertex,
	PVOID pMesh,
	ULONG nMesh,
	ULONG ulMode
	);
}
#endif

namespace ege {

enum dealmessage_update {
	NORMAL_UPDATE = false,
	FORCE_UPDATE = true,
};

struct EGEMSG {
	HWND        hwnd;
	UINT        message;
	WPARAM      wParam;
	LPARAM      lParam;
	DWORD       time;
	UINT        mousekey;
	UINT        flag;
};

// 定义图像对象
class IMAGE
{
	int     m_initflag;
public:
	HDC         m_hDC;
	HBITMAP     m_hBmp;
	int         m_width;
	int         m_height;
	PDWORD      m_pBuffer;
	color_t     m_color;
	color_t     m_fillcolor;
	bool        m_aa;
private:
	int  newimage(HDC hdc, int width, int height);
	int  deleteimage();
	void reset();
public:
	viewporttype        m_vpt;
	textsettingstype    m_texttype;
	linestyletype       m_linestyle;
	float               m_linewidth;
	color_t             m_bk_color;
	void*               m_pattern_obj;
	int                 m_pattern_type;
	void*               m_texture;
private:
	void inittest(const WCHAR* strCallFunction = NULL) const;
public:
	IMAGE();
	IMAGE(int width, int height);
	IMAGE(IMAGE &img);              // 拷贝构造函数
	IMAGE& operator = (const IMAGE &img); // 赋值运算符重载函数
	~IMAGE();
	void set_pattern(void* obj, int type);
	void delete_pattern();
	void gentexture(bool gen);
public:
	HDC getdc()        const {return m_hDC;}
	int getwidth()     const {return m_width; }
	int getheight()    const {return m_height;}
	color_t* getbuffer() const {return (color_t*)m_pBuffer;}

	int  createimage(int width, int height);
	int  resize(int width, int height);
	void copyimage(const PIMAGE pSrcImg);
	void getimage(int srcX, int srcY, int srcWidth, int srcHeight);
	void getimage(const PIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);
	int  getimage(LPCSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);
	int  getimage(LPCWSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);
	int  getimage(LPCSTR pResType, LPCSTR pResName, int zoomWidth = 0, int zoomHeight = 0);
	int  getimage(LPCWSTR pResType, LPCWSTR pResName, int zoomWidth = 0, int zoomHeight = 0);
	int  getimage(void* pMem, long size);
	void putimage(int dstX, int dstY, DWORD dwRop = SRCCOPY) const;
	void putimage(int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;
	void putimage(PIMAGE pDstImg, int dstX, int dstY, DWORD dwRop = SRCCOPY) const;
	void putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;
	void putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY) const;
	int  saveimage(LPCSTR  filename);
	int  saveimage(LPCWSTR filename);
	int  getpngimg(FILE* fp);
	int  savepngimg(FILE* fp, int bAlpha);
	int
	putimage_transparent(
		PIMAGE imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		color_t crTransparent,  // color to make transparent
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);
	int
	putimage_alphablend(
		PIMAGE imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		unsigned char alpha,    // alpha
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);
	int
	putimage_alphatransparent(
		PIMAGE imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		color_t crTransparent,  // color to make transparent
		unsigned char alpha,    // alpha
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);
	int
	putimage_withalpha(
		PIMAGE imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);

	int
	putimage_alphafilter(
		PIMAGE imgdest,         // handle to dest
		int nXOriginDest,       // x-coord of destination upper-left corner
		int nYOriginDest,       // y-coord of destination upper-left corner
		PIMAGE imgalpha,        // alpha
		int nXOriginSrc = 0,    // x-coord of source upper-left corner
		int nYOriginSrc = 0,    // y-coord of source upper-left corner
		int nWidthSrc = 0,      // width of source rectangle
		int nHeightSrc = 0      // height of source rectangle
	);
	int
	imagefilter_blurring_4 (
		int intensity,
		int alpha,
		int nXOriginDest,
		int nYOriginDest,
		int nWidthDest,
		int nHeightDest
	);
	int
	imagefilter_blurring_8 (
		int intensity,
		int alpha,
		int nXOriginDest,
		int nYOriginDest,
		int nWidthDest,
		int nHeightDest
	);
	int
	imagefilter_blurring (
		int intensity,
		int alpha,
		int nXOriginDest = 0,
		int nYOriginDest = 0,
		int nWidthDest = 0,
		int nHeightDest = 0
	);
	int putimage_rotate(
		PIMAGE imgtexture,
		int nXOriginDest,
		int nYOriginDest,
		float centerx,
		float centery,
		float radian,
		int btransparent = 0,           // transparent (1) or not (0)
		int alpha = -1,                  // in range[0, 256], alpha== -1 means no alpha
		int smooth = 0
	);

	int putimage_rotatezoom(
		PIMAGE imgtexture,
		int nXOriginDest,
		int nYOriginDest,
		float centerx,
		float centery,
		float radian,
		float zoom,
		int btransparent = 0,           // transparent (1) or not (0)
		int alpha = -1,                  // in range[0, 256], alpha== -1 means no alpha
		int smooth = 0
	);

	friend void getimage_from_png_struct(PIMAGE, void*, void*);
};

// 定义ege全局状态对象
struct _graph_setting {
	bool has_init;
	bool is_unicode;

	struct _graph {
		int width;
		int height;
	}graph;
	struct _aspectratio {
		float xasp, yasp;
	}aspectratio;

	int writemode;

	HDC dc;
	int dc_w, dc_h;
	PIMAGE img_page[BITMAP_PAGE_SIZE];
	int base_x, base_y, base_w, base_h;

	int     visual_page;
	int     active_page;
	PIMAGE  imgtarget;
	PIMAGE  imgtarget_set;
	PIMAGE  img_timer_update;

	HINSTANCE instance;
	HWND    hwnd;
	std::wstring window_caption;
	HICON   window_hicon;
	int     exit_flag;
	int     exit_window;
	int     update_mark_count; //更新标记
	bool    close_manually;
	bool    use_force_exit; //强制关闭进程标记
	bool    lock_window;
	bool    timer_stop_mark;
	bool    skip_timer_mark;

	thread_queue<EGEMSG> *msgkey_queue, *msgmouse_queue;

	HANDLE threadui_handle;


	/*鼠标状态记录*/
	int mouse_state_l, mouse_state_m, mouse_state_r;
	int mouse_last_x, mouse_last_y;
	int mouse_lastclick_x, mouse_lastclick_y;
	int mouse_lastup_x, mouse_lastup_y;
	int mouse_show;

	LPMSG_KEY_PROC callback_key;
	void* callback_key_param;
	LPMSG_MOUSE_PROC callback_mouse;
	void* callback_mouse_param;
	LPCALLBACK_PROC callback_close;

	/* 键盘状态记录 */
	int keystatemap[MAX_KEY_VCODE];

	/* egeControlBase */
	egeControlBase* egectrl_root;
	egeControlBase* egectrl_focus;

	/* 私用全局变量 */
#ifdef EGE_GDIPLUS
	ULONG_PTR g_gdiplusToken;
#endif
	LARGE_INTEGER get_highfeq_time_start;
	DWORD         fclock_start;
	//double delay_dwLast;
	double delay_ms_dwLast;
	double delay_fps_dwLast;
	int getch_last_key;

	HBRUSH savebrush_hbr;

	/* 函数用临时缓冲区 */
	DWORD g_t_buff[1024 * 8];

	_graph_setting() {
		window_caption = EGE_TITLE_W;
	}
};

extern struct _graph_setting graph_setting;

template<typename T>
struct count_ptr {
	explicit count_ptr( T* p ) {
		//m_mutex = new Mutex;
		m_cnt = new long( 1 );
		m_p = p;
	}
	~count_ptr() {
		//m_mutex->Lock();
		--*m_cnt;
		if ( *m_cnt == 0 ) {
			delete m_p;
			m_p = static_cast<T*>( 0 );
			delete m_cnt;
			m_cnt = static_cast<long*>( 0 );
		}
		//Mutex* mutex = m_mutex;
		//m_mutex = static_cast<Mutex*> ( 0 );
		//mutex->UnLock();
	}
	count_ptr( const count_ptr<T>& count_ptr_ ) {
		//m_mutex = count_ptr_.m_mutex;
		//m_mutex->Lock();
		m_p = count_ptr_.m_p;
		m_cnt = count_ptr_.m_cnt;
		++*m_cnt;
		//m_mutex->UnLock();
	}
	count_ptr<T>& operator= ( const count_ptr<T>& count_ptr_ ) {
		//m_mutex->Lock();
		--*m_cnt;
		if ( *m_cnt == 0 ) {
			delete m_p;
			m_p = static_cast<T*>( 0 );
			delete m_cnt;
			m_cnt = static_cast<long*>( 0 );
		}
		//Mutex* mutex = m_mutex;
		//m_mutex = count_ptr_.m_mutex;
		//mutex->UnLock();

		//m_mutex->Lock();
		m_p = count_ptr_.m_p;
		m_cnt = static_cast<long*>( count_ptr_.m_cnt );
		++*m_cnt;
		//m_mutex->UnLock();
		return *this;
	}
	operator T* () const {
		return m_p;
	}
	T& operator * () const {
		return *m_p;
	}
	T* operator -> () const {
		return m_p;
	}
private:
	T* m_p;
	long* m_cnt;
	//Mutex* m_mutex;
};

// convert wide string to multibyte string
std::string w2mb(LPCWSTR wStr);

} // namespace ege

#endif /*_EGE_HEAD_H_*/
