/*
* EGE (Easy Graphics Engine)
* filename  ege_head.cpp

公共声明头文件（私有，不对外使用）
*/

#ifndef EGE_HEAD_H
#define EGE_HEAD_H

#ifndef _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#endif
#ifndef _ALLOW_RUNTIME_LIBRARY_MISMATCH
#define _ALLOW_RUNTIME_LIBRARY_MISMATCH
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#define EGE_GRAPH_LIB_BUILD
#define EGE_DEPRECATE(function, msg)

#include "ege.h"
#include "ege/types.h"

#define EGE_TOSTR_(x) #x
#define EGE_TOSTR(x)  EGE_TOSTR_(x)

#define EGE_L_(str) L##str
#define EGE_L(str)  EGE_L_(str)

// 编译器版本，目前仅支持 MSVC/MinGW
#ifdef _WIN64
#define SYSBITS "x64"
#else
#define SYSBITS "x86"
#endif

#define SYSBITS_W EGE_L(SYSBITS)

#ifdef _MSC_VER
#   if (_MSC_VER >= 1930)
#       define MSVC_VER "VC2022"
#   elif (_MSC_VER >= 1920)
#       define MSVC_VER "VC2019"
#   elif (_MSC_VER >= 1910)
#       define MSVC_VER "VC2017"
#   elif (_MSC_VER >= 1900)
#       define MSVC_VER "VC2015"
#   elif (_MSC_VER >= 1800)
#       define MSVC_VER "VC2013"
#   elif (_MSC_VER >= 1700)
#       define MSVC_VER "VC2012"
#   elif (_MSC_VER >= 1600)
#       define MSVC_VER "VC2010"
#   elif (_MSC_VER >= 1500)
#       define MSVC_VER "VC2008"
#   elif (_MSC_VER > 1200)
#       define MSVC_VER "VC2005"
#   else
#       define MSVC_VER "VC6"
#   endif
#   define COMPILER_VER   MSVC_VER SYSBITS
#   define COMPILER_VER_W EGE_L(MSVC_VER) SYSBITS_W
#else
#   define GCC_VER        EGE_TOSTR(__GNUC__) "." EGE_TOSTR(__GNUC_MINOR__)
#   define GCC_VER_W      EGE_L(EGE_TOSTR(__GNUC__)) L"." EGE_L(EGE_TOSTR(__GNUC_MINOR__))
#   define COMPILER_VER   "GCC" GCC_VER SYSBITS
#   define COMPILER_VER_W L"GCC" GCC_VER_W SYSBITS_W
#endif

#define EGE_TITLE   "EGE" EGE_VERSION " " COMPILER_VER
#define EGE_TITLE_W L"EGE" EGE_L(EGE_VERSION) L" " COMPILER_VER_W

#define EGE_WNDCLSNAME   "Easy Graphics Engine"
#define EGE_WNDCLSNAME_W EGE_L(EGE_WNDCLSNAME)

#include <string>

#ifdef EGE_GDIPLUS
#   if defined(NOMINMAX) && defined(_MSC_VER)
#       define max(a, b) (((a) > (b)) ? (a) : (b))
#       define min(a, b) (((a) < (b)) ? (a) : (b))
#       include <gdiplus.h>
#       undef max
#       undef min
#   else
#       include <gdiplus.h>
#   endif
#endif

#include "thread_queue.h"

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#endif

#define QUEUE_LEN            1024
#define BITMAP_PAGE_SIZE     4
#define BITMAP_PAGE_MIN_SIZE 1
#define UPDATE_MAX_CALL      0xFF
#define RENDER_TIMER_ID      916
#define IMAGE_INIT_FLAG      0x20100916
#define MAX_KEY_VCODE        256
#define FLOAT_EPS            1e-3f



#define IFATODOB(A, B)  ((A) && (B, 0))
#define IFNATODOB(A, B) ((A) || (B, 0))

#define CONVERT_IMAGE(pimg)                                                                             \
    (((size_t)(pimg) < 0x20 ? ((pimg) ? (graph_setting.img_page[(size_t)(pimg) & 0xF]) :                \
                                        (--graph_setting.update_mark_count, graph_setting.imgtarget)) : \
                              pimg))

#define CONVERT_IMAGE_CONST(pimg) \
    ((size_t)(pimg) < 0x20 ? ((pimg) ? (graph_setting.img_page[(size_t)(pimg) & 0xF]) : graph_setting.imgtarget) : pimg)

#define CONVERT_IMAGE_F(pimg) CONVERT_IMAGE(pimg)

#define CONVERT_IMAGE_F_CONST(pimg) CONVERT_IMAGE_CONST(pimg)

#define CONVERT_IMAGE_END

#ifndef DEFAULT_CHARSET
#define DEFAULT_CHARSET ANSI_CHARSET
#endif


namespace ege
{

enum dealmessage_update
{
    NORMAL_UPDATE = false,
    FORCE_UPDATE  = true,
};

struct EGEMSG
{
    HWND   hwnd;
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD  time;
    UINT   mousekey;
    UINT   flag;
};

#ifdef EGE_GDIPLUS
Gdiplus::DashStyle linestyle_to_dashstyle(int linestyle);
#endif

class egeControlBase;   // egeControlBase 前置声明

// 定义ege全局状态对象
struct _graph_setting
{
    bool has_init;
    bool unicode_char_message;

    struct _graph
    {
        int width;
        int height;
    } graph;

    struct _aspectratio
    {
        float xasp, yasp;
    } aspectratio;

    int writemode;

    HDC    dc;
    int    dc_w, dc_h;
    PIMAGE img_page[BITMAP_PAGE_SIZE];
    int    base_x, base_y, base_w, base_h;

    int    visual_page;
    int    active_page;
    PIMAGE imgtarget;
    PIMAGE imgtarget_set;
    PIMAGE img_timer_update;

    HINSTANCE    instance;
    HWND         hwnd;
    std::wstring window_caption;
    HICON        window_hicon;
    color_t      window_initial_color;
    int          exit_flag;
    int          exit_window;
    int          update_mark_count; // 更新标记
    bool         close_manually;
    bool         use_force_exit; // 强制关闭进程标记
    bool         lock_window;
    bool         timer_stop_mark;
    bool         skip_timer_mark;
    bool         first_show;

    thread_queue<EGEMSG>*msgkey_queue, *msgmouse_queue;

    HANDLE threadui_handle;

    /* 鼠标状态记录 */
    Point mouse_pos;
    int mouse_show;

    LPMSG_KEY_PROC   callback_key;
    void*            callback_key_param;
    LPMSG_MOUSE_PROC callback_mouse;
    void*            callback_mouse_param;
    LPCALLBACK_PROC  callback_close;

    /* 按键状态记录 */
    bool keystatemap[MAX_KEY_VCODE];
    uint16_t key_press_count[MAX_KEY_VCODE];
    uint16_t key_release_count[MAX_KEY_VCODE];
    uint16_t key_repeat_count[MAX_KEY_VCODE];

    /* egeControlBase */
    egeControlBase* egectrl_root;
    egeControlBase* egectrl_focus;

    /* 私用全局变量 */
#ifdef EGE_GDIPLUS
    ULONG_PTR g_gdiplusToken;
#endif
    LARGE_INTEGER get_highfeq_time_start;
    DWORD         fclock_start;
    // double delay_dwLast;
    double delay_ms_dwLast;
    double delay_fps_dwLast;
    int    getch_last_key;
    unsigned int codepage;
    wchar_t wchar_message_low_surrogate_cache;

    HBRUSH savebrush_hbr;

    /* 函数用临时缓冲区 */
    DWORD g_t_buff[1024 * 8];

public:
    _graph_setting();
};

template <typename T> struct count_ptr
{
    explicit count_ptr(T* p)
    {
        // m_mutex = new Mutex;
        m_cnt = new long(1);
        m_p   = p;
    }

    ~count_ptr()
    {
        // m_mutex->Lock();
        --*m_cnt;
        if (*m_cnt == 0) {
            delete m_p;
            m_p = static_cast<T*>(0);
            delete m_cnt;
            m_cnt = static_cast<long*>(0);
        }
        // Mutex* mutex = m_mutex;
        // m_mutex = static_cast<Mutex*> ( 0 );
        // mutex->UnLock();
    }

    count_ptr(const count_ptr<T>& count_ptr_)
    {
        // m_mutex = count_ptr_.m_mutex;
        // m_mutex->Lock();
        m_p   = count_ptr_.m_p;
        m_cnt = count_ptr_.m_cnt;
        ++*m_cnt;
        // m_mutex->UnLock();
    }

    count_ptr<T>& operator=(const count_ptr<T>& count_ptr_)
    {
        // m_mutex->Lock();
        --*m_cnt;
        if (*m_cnt == 0) {
            delete m_p;
            m_p = static_cast<T*>(0);
            delete m_cnt;
            m_cnt = static_cast<long*>(0);
        }
        // Mutex* mutex = m_mutex;
        // m_mutex = count_ptr_.m_mutex;
        // mutex->UnLock();

        // m_mutex->Lock();
        m_p   = count_ptr_.m_p;
        m_cnt = static_cast<long*>(count_ptr_.m_cnt);
        ++*m_cnt;
        // m_mutex->UnLock();
        return *this;
    }

    operator T*() const { return m_p; }

    T& operator*() const { return *m_p; }
    T* operator->() const { return m_p; }

private:
    T*    m_p;
    long* m_cnt;
    // Mutex* m_mutex;
};

void internal_panic(const wchar_t* errmsg);

HBITMAP newbitmap(int width, int height, PDWORD* p_bmp_buf);



// FIXME: This seems to be VC6 SDK problem
#ifndef AC_SRC_ALPHA
#define AC_SRC_ALPHA 0x01
#endif

} // namespace ege

#endif /* EGE_HEAD_H */
