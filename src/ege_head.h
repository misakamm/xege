/*
* EGE (Easy Graphics Engine)
* filename  ege_head.cpp

��������ͷ�ļ���˽�У�������ʹ�ã�
*/

#ifndef _EGE_HEAD_H_
#define _EGE_HEAD_H_

#ifndef _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#endif
#ifndef _ALLOW_RUNTIME_LIBRARY_MISMATCH
#define _ALLOW_RUNTIME_LIBRARY_MISMATCH
#endif

#if !defined(_MSC_VER) || _MSC_VER > 1300
#include "stdint.h"
#endif

#define _GRAPH_LIB_BUILD_
#include "ege.h"
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

#define EGE_GDIPLUS // ʹ��gdi+������չ

#ifdef EGE_GDIPLUS
#ifdef _MSC_VER
#	include <gdiplus.h>
#else
#	if __GNUC__ == 3
#		include "gdiplus3/gdiplus.h"
#	elif __GNUC__ >= 4
#		include "gdiplus4/gdiplus.h"
#	endif
#endif
#endif

#define QUEUE_LEN           1024
#define BITMAP_PAGE_SIZE    4
#define UPDATE_MAX_CALL     0xFF
#define RENDER_TIMER_ID     916
#define IMAGE_INIT_FLAG     0x20100916
#define MAX_KEY_VCODE       256
#define FLOAT_EPS           1e-3f


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

// ����ͼ�����
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
	IMAGE(IMAGE &img);              // �������캯��
	IMAGE& operator = (const IMAGE &img); // ��ֵ��������غ���
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
};

// ����egeȫ��״̬����
struct _graph_setting {
	struct _graph {
		int width;
		int height;
	}graph;
	struct _aspectratio {
		float xasp, yasp;
	}aspectratio;

	int writemode;

	HDC dc;
	HDC window_dc;
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
	TCHAR   window_class_name[32];
	TCHAR   window_caption[128];
	int     exit_flag;
	int     exit_window;
	int     update_mark_count; //���±��
	bool    close_manually;
	bool    use_force_exit; //ǿ�ƹرս��̱��
	bool    lock_window;
	bool    init_finish;
	bool    timer_stop_mark;
	bool    skip_timer_mark;

	thread_queue<EGEMSG> *msgkey_queue, *msgmouse_queue;

	HANDLE threadui_handle;


	/*���״̬��¼*/
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

	/* ����״̬��¼ */
	int keystatemap[MAX_KEY_VCODE];

	/* egeControlBase */
	egeControlBase* egectrl_root;
	egeControlBase* egectrl_focus;

	/* ˽��ȫ�ֱ��� */
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

	/* ��������ʱ������ */
	DWORD g_t_buff[1024 * 8];
};

extern struct _graph_setting& graph_setting;

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

} // namespace ege

#endif /*_EGE_HEAD_H_*/
