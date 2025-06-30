/*********************************************************
 * EGE (Easy Graphics Engine)  24.04
 * FileName:    ege.h
 * Website:     https://xege.org
 * Community:   https://club.xege.org
 * GitHub:      https://github.com/wysaid/xege
 * GitHub:      https://github.com/Easy-Graphics-Engine
 * Gitee:       https://gitee.com/xege/xege
 * Blog:        https://blog.csdn.net/qq_39151563/article/details/125688290
 * E-Mail:      this@xege.org
 *
 *********************************************************/

#ifndef EGE_H
#define EGE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

// Easy Graphics Engine Version
// Calendar Versioning, format: YY.0M.PatchNumber (If the PatchNumber equals 0, the YY.0M format is used.)
#define EGE_VERSION        "24.04"
#define EGE_VERSION_MAJOR  24
#define EGE_VERSION_MINOR  4
#define EGE_VERSION_PATCH  0
#define EGE_MAKE_VERSION_NUMBER(major, minor, patch)    ((major) * 10000L + (minor) * 100L + (patch))
#define EGE_VERSION_NUMBER    EGE_MAKE_VERSION_NUMBER(EGE_VERSION_MAJOR, EGE_VERSION_MINOR, EGE_VERSION_PATCH)

#ifndef __cplusplus
#error You must use a C++ compiler and ensure that your source files is named with the '.cpp' suffix.
#endif

#if defined(_INC_CONIO) || defined(_CONIO_H_)
#error You cannot include "conio.h" before "graphics.h".
#endif

#if defined(_MSC_VER)
#   pragma warning(disable: 4355)
#   ifndef _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#       define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#   endif
#   ifndef _ALLOW_RUNTIME_LIBRARY_MISMATCH
#       define _ALLOW_RUNTIME_LIBRARY_MISMATCH
#   endif
#endif

#if !defined(EGE_GRAPH_LIB_BUILD) && !defined(EGE_GRAPH_NO_LIB)
#   ifdef _MSC_VER
#       pragma comment(lib,"gdiplus.lib")
#       ifdef _WIN64 // 64 bit libs
#           pragma comment(lib,"graphics.lib")
#       else   // 32 bit libs
#           pragma comment(lib,"graphics.lib")
#       endif
#   endif
#endif


#if !defined(EGE_GRAPH_LIB_BUILD) && !defined(EGE_GRAPH_NO_LIB)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif
#endif

#include "ege/stdint.h"

#if defined(EGE_FOR_AUTO_CODE_COMPLETETION_ONLY)
#include <windef.h>
#include <winuser.h>
#include <wingdi.h>
#else
#include <windows.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1300)
#define EGE_COMPILERINFO_VC6
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1200 && !defined(SetWindowLongPtr)
#   define SetWindowLongPtrW   SetWindowLongW
#   define GetWindowLongPtrW   GetWindowLongW
#   define GWLP_USERDATA       GWL_USERDATA
#   define GWLP_WNDPROC        GWL_WNDPROC
#endif

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

#ifndef EGE_CDECL
#   if __STDC__
#       define EGE_CDECL  __cdecl
#   else
#       define EGE_CDECL  __cdecl
#   endif
#endif

#ifdef _MSC_VER
#   if defined(_WIN64)
#       define EGEAPI
#   else
#       define EGEAPI EGE_CDECL
#   endif
#else
#   if defined(__WORDSIZE)
#       if __WORDSIZE > 32
#           define EGEAPI
#       else
#           define EGEAPI EGE_CDECL
#       endif
#   else
#       define EGEAPI
#   endif
#endif

#ifndef EGE_DEPRECATE
#   ifdef _MSC_VER
#       ifdef _CRT_DEPRECATE_TEXT
#           define EGE_DEPRECATE(function, msg) _CRT_DEPRECATE_TEXT("This function is deprecated. " msg " For more information, visit https://xege.org .")
#       else
#           define EGE_DEPRECATE(function, msg)
#       endif
#   elif ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)))
#       define EGE_DEPRECATE(function, msg) __attribute__((deprecated(msg " For more information, visit https://xege.org .")))
#   else
#       define EGE_DEPRECATE(function, msg) __attribute__((deprecated))
#   endif
#endif

#define EGE_GDIPLUS

#define EGERGBA(r, g, b, a)  ((::ege::color_t)(((r) << 16) | ((g) << 8) | (b) | ((a) << 24)))
#define EGERGB(r, g, b)      EGERGBA(r, g, b, 0xFF)
#define EGEARGB(a, r, g, b)  EGERGBA(r, g, b, a)
#define EGEACOLOR(a, color)  ((::ege::color_t)(((color) & 0xFFFFFF) | ((a) << 24)))
#define EGECOLORA(color, a)  EGEACOLOR(a, color)
#define EGEGET_R(c)          (((c) >> 16) & 0xFF)
#define EGEGET_G(c)          (((c) >> 8) & 0xFF)
#define EGEGET_B(c)          (((c)) & 0xFF)
#define EGEGET_A(c)          (((c) >> 24) & 0xFF)
#define EGEGRAY(gray)        EGERGB(gray, gray, gray)
#define EGEGRAYA(gray, a)    EGERGBA(gray, gray, gray, a)
#define EGEAGRAY(a, gray)    EGEGRAYA(gray, a)

/* you can also use 932 as shift-jis, 950 as big5 ... */
/* see https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers */
#define EGE_CODEPAGE_GB2312    936
#define EGE_CODEPAGE_UTF8      65001
#define EGE_CODEPAGE_ANSI      0

namespace ege
{

const double PI = 3.1415926535897932384626;

/* define graphics drivers */
enum graphics_drivers
{
    DETECT = 0,         /* requests autodetection */
    CGA, MCGA, EGA, EGA64, EGAMONO, IBM8514,/* 1 - 6 */
    HERCMONO, ATT400, VGA, PC3270,          /* 7 - 10 */
    TRUECOLOR, TRUECOLORSIZE,
    CURRENT_DRIVER = -1
};

/* graphics modes for each driver */
enum graphics_modes
{
    CGAC0      = 0, /* 320x200 palette 0; 1 page  */
    CGAC1      = 1, /* 320x200 palette 1; 1 page  */
    CGAC2      = 2, /* 320x200 palette 2: 1 page  */
    CGAC3      = 3, /* 320x200 palette 3; 1 page  */
    CGAHI      = 4, /* 640x200 1 page             */
    MCGAC0     = 0, /* 320x200 palette 0; 1 page  */
    MCGAC1     = 1, /* 320x200 palette 1; 1 page  */
    MCGAC2     = 2, /* 320x200 palette 2; 1 page  */
    MCGAC3     = 3, /* 320x200 palette 3; 1 page  */
    MCGAMED    = 4, /* 640x200 1 page             */
    MCGAHI     = 5, /* 640x480 1 page             */
    EGALO      = 0, /* 640x200 16 color 4 pages   */
    EGAHI      = 1, /* 640x350 16 color 2 pages   */
    EGA64LO    = 0, /* 640x200 16 color 1 page    */
    EGA64HI    = 1, /* 640x350 4 color  1 page    */
    EGAMONOHI  = 0, /* 640x350 64K on card, 1 page - 256K on card, 4 pages */
    HERCMONOHI = 0, /* 720x348 2 pages            */
    ATT400C0   = 0, /* 320x200 palette 0; 1 page  */
    ATT400C1   = 1, /* 320x200 palette 1; 1 page  */
    ATT400C2   = 2, /* 320x200 palette 2; 1 page  */
    ATT400C3   = 3, /* 320x200 palette 3; 1 page  */
    ATT400MED  = 4, /* 640x200 1 page             */
    ATT400HI   = 5, /* 640x400 1 page             */
    VGALO      = 0, /* 640x200 16 color 4 pages   */
    VGAMED     = 1, /* 640x350 16 color 2 pages   */
    VGAHI      = 2, /* 640x480 16 color 1 page    */
    PC3270HI   = 0, /* 720x350 1 page             */
    IBM8514LO  = 0, /* 640x480 256 colors         */
    IBM8514HI  = 1  /*1024x768 256 colors         */
};

enum initmode_flag
{
    INIT_DEFAULT         = 0x0,   ///< 默认模式
    INIT_NOBORDER        = 0x1,   ///< 无边框窗口
    INIT_CHILD           = 0x2,   ///< 子窗口模式
    INIT_TOPMOST         = 0x4,   ///< 置顶窗口
    INIT_RENDERMANUAL    = 0x8,   ///< 手动渲染模式
    INIT_NOFORCEEXIT     = 0x10,  ///< 关闭窗口时不强制退出程序，只设置内部标志位，is_run() 可以获取标志位
    INIT_UNICODE         = 0x20,  ///< Unicode字符消息 (等同于setunicodecharmessage(true))
    INIT_HIDE            = 0x40,  ///< 隐藏窗口
    INIT_WITHLOGO        = 0x100, ///< 启动时显示EGE Logo 动画 (Debug版本下默认不显示)
    INIT_ANIMATION       = INIT_DEFAULT | INIT_RENDERMANUAL | INIT_NOFORCEEXIT ///< 动画模式
};

/**
 * @enum rendermode_e
 * @brief 渲染模式
 */
enum rendermode_e
{
    RENDER_AUTO,   ///< 自动渲染
    RENDER_MANUAL  ///< 手动渲染
};

/**
 * @enum graphics_errors
 * @brief 图形操作错误码
 * 
 * 定义了图形操作可能返回的各种错误代码
 */
enum graphics_errors
{
    grOk                 = 0,           ///< 操作成功
    grNoInitGraph        = -1,          ///< 图形系统未初始化
    grNotDetected        = -2,          ///< 未检测到图形设备
    grFileNotFound       = -3,          ///< 文件未找到
    grInvalidDriver      = -4,          ///< 无效的驱动
    grNoLoadMem          = -5,          ///< 内存加载失败
    grNoScanMem          = -6,          ///< 扫描内存失败
    grNoFloodMem         = -7,          ///< 填充内存失败
    grFontNotFound       = -8,          ///< 字体未找到
    grNoFontMem          = -9,          ///< 字体内存不足
    grInvalidMode        = -10,         ///< 无效模式
    grError              = -11,         ///< 通用错误
    grIOerror            = -12,         ///< I/O错误
    grInvalidFont        = -13,         ///< 无效字体
    grInvalidFontNum     = -14,         ///< 无效字体编号
    grInvalidVersion     = -18,         ///< 版本不兼容

    grException          = 0x10,        ///< EGE异常
    grParamError         = 0x11,        ///< 参数错误
    grInvalidRegion      = 0x12,        ///< 无效区域
    grOutOfMemory        = 0x13,        ///< 内存不足
    grNullPointer        = 0x14,        ///< 空指针
    grAllocError         = 0x15,        ///< 分配错误
    grInvalidMemory      = 0xCDCDCDCD   ///< 无效内存
};

/**
 * @enum message_event
 * @brief 消息事件类型
 * 
 * 定义了鼠标和键盘消息的事件类型，用于消息处理
 */
enum message_event
{
    MSG_EVENT_UP         = 0x00,    ///< 按键/鼠标按钮释放事件
    MSG_EVENT_DOWN       = 0x01,    ///< 按键/鼠标按钮按下事件
    MSG_EVENT_CLICK      = 0x01,    ///< 鼠标单击事件（等同于DOWN）
    MSG_EVENT_DBCLICK    = 0x02,    ///< 鼠标双击事件
    MSG_EVENT_MOVE       = 0x04,    ///< 鼠标移动事件
    MSG_EVENT_WHEEL      = 0x10     ///< 鼠标滚轮事件
};

/**
 * @enum message_mouse
 * @brief 鼠标按钮标识
 * 
 * 定义了不同的鼠标按钮，可以通过位或操作组合使用
 */
enum message_mouse
{
    MSG_MOUSE_LEFT  	 = 0x01,    ///< 鼠标左键
    MSG_MOUSE_RIGHT 	 = 0x02,    ///< 鼠标右键
    MSG_MOUSE_MID   	 = 0x04     ///< 鼠标中键（滚轮按键）
};


#ifndef EGE_COLOR_T_TYPEDEF
#define EGE_COLOR_T_TYPEDEF
/// @brief 颜色类型定义，使用32位无符号整数表示ARGB颜色
typedef uint32_t color_t;
#endif

/**
 * @enum alpha_type
 * @brief Alpha通道类型
 * 
 * 定义了图像Alpha通道的不同处理方式
 */
enum alpha_type
{
    ALPHATYPE_STRAIGHT      = 0,    ///< 直接Alpha（非预乘Alpha）
    ALPHATYPE_PREMULTIPLIED = 1     ///< 预乘Alpha
};

/**
 * @struct ege_point
 * @brief 浮点坐标点结构
 * 
 * 用于表示二维空间中的一个点，坐标使用浮点数
 */
struct ege_point
{
    float x;    ///< x坐标
    float y;    ///< y坐标
};

/**
 * @struct ege_rect
 * @brief 矩形区域结构
 * 
 * 用于表示矩形区域，包含位置和尺寸信息
 */
struct ege_rect
{
    float x;    ///< 矩形左上角x坐标
    float y;    ///< 矩形左上角y坐标
    float w;    ///< 矩形宽度
    float h;    ///< 矩形高度
};

/**
 * @struct ege_colpoint
 * @brief 带颜色的坐标点结构
 * 
 * 用于表示带有颜色信息的二维坐标点，常用于渐变效果
 */
struct ege_colpoint
{
    float   x;      ///< x坐标
    float   y;      ///< y坐标
    color_t color;  ///< 该点的颜色值
};

/**
 * @enum COLORS
 * @brief 预定义颜色常量
 * 
 * 提供了常用的颜色常量，基于Web安全色彩标准定义
 * 颜色值使用RGB格式，可以直接用于绘图函数
 */
enum COLORS
{
    ALICEBLUE            = EGERGB(0xF0, 0xF8, 0xFF),
    ANTIQUEWHITE         = EGERGB(0xFA, 0xEB, 0xD7),
    AQUA                 = EGERGB(0x00, 0xFF, 0xFF),
    AQUAMARINE           = EGERGB(0x7F, 0xFF, 0xD4),
    AZURE                = EGERGB(0xF0, 0xFF, 0xFF),
    BEIGE                = EGERGB(0xF5, 0xF5, 0xDC),
    BISQUE               = EGERGB(0xFF, 0xE4, 0xC4),
    BLACK                = EGERGB(0x00, 0x00, 0x00),
    BLANCHEDALMOND       = EGERGB(0xFF, 0xEB, 0xCD),
    BLUE                 = EGERGB(0x00, 0x00, 0xFF),
    BLUEVIOLET           = EGERGB(0x8A, 0x2B, 0xE2),
    BROWN                = EGERGB(0xA5, 0x2A, 0x2A),
    BURLYWOOD            = EGERGB(0xDE, 0xB8, 0x87),
    CADETBLUE            = EGERGB(0x5F, 0x9E, 0xA0),
    CHARTREUSE           = EGERGB(0x7F, 0xFF, 0x00),
    CHOCOLATE            = EGERGB(0xD2, 0x69, 0x1E),
    CORAL                = EGERGB(0xFF, 0x7F, 0x50),
    CORNFLOWERBLUE       = EGERGB(0x64, 0x95, 0xED),
    CORNSILK             = EGERGB(0xFF, 0xF8, 0xDC),
    CRIMSON              = EGERGB(0xDC, 0x14, 0x3C),
    CYAN                 = EGERGB(0x00, 0xFF, 0xFF),
    DARKBLUE             = EGERGB(0x00, 0x00, 0x8B),
    DARKCYAN             = EGERGB(0x00, 0x8B, 0x8B),
    DARKGOLDENROD        = EGERGB(0xB8, 0x86, 0x0B),
    DARKGRAY             = EGERGB(0xA9, 0xA9, 0xA9),
    DARKGREEN            = EGERGB(0x00, 0x64, 0x00),
    DARKKHAKI            = EGERGB(0xBD, 0xB7, 0x6B),
    DARKMAGENTA          = EGERGB(0x8B, 0x00, 0x8B),
    DARKOLIVEGREEN       = EGERGB(0x55, 0x6B, 0x2F),
    DARKORANGE           = EGERGB(0xFF, 0x8C, 0x00),
    DARKORCHID           = EGERGB(0x99, 0x32, 0xCC),
    DARKRED              = EGERGB(0x8B, 0x00, 0x00),
    DARKSALMON           = EGERGB(0xE9, 0x96, 0x7A),
    DARKSEAGREEN         = EGERGB(0x8F, 0xBC, 0x8F),
    DARKSLATEBLUE        = EGERGB(0x48, 0x3D, 0x8B),
    DARKSLATEGRAY        = EGERGB(0x2F, 0x4F, 0x4F),
    DARKTURQUOISE        = EGERGB(0x00, 0xCE, 0xD1),
    DARKVIOLET           = EGERGB(0x94, 0x00, 0xD3),
    DEEPPINK             = EGERGB(0xFF, 0x14, 0x93),
    DEEPSKYBLUE          = EGERGB(0x00, 0xBF, 0xFF),
    DIMGRAY              = EGERGB(0x69, 0x69, 0x69),
    DODGERBLUE           = EGERGB(0x1E, 0x90, 0xFF),
    FIREBRICK            = EGERGB(0xB2, 0x22, 0x22),
    FLORALWHITE          = EGERGB(0xFF, 0xFA, 0xF0),
    FORESTGREEN          = EGERGB(0x22, 0x8B, 0x22),
    FUCHSIA              = EGERGB(0xFF, 0x00, 0xFF),
    GAINSBORO            = EGERGB(0xDC, 0xDC, 0xDC),
    GHOSTWHITE           = EGERGB(0xF8, 0xF8, 0xFF),
    GOLD                 = EGERGB(0xFF, 0xD7, 0x00),
    GOLDENROD            = EGERGB(0xDA, 0xA5, 0x20),
    GRAY                 = EGERGB(0x80, 0x80, 0x80),
    GREEN                = EGERGB(0x00, 0x80, 0x00),
    GREENYELLOW          = EGERGB(0xAD, 0xFF, 0x2F),
    HONEYDEW             = EGERGB(0xF0, 0xFF, 0xF0),
    HOTPINK              = EGERGB(0xFF, 0x69, 0xB4),
    INDIANRED            = EGERGB(0xCD, 0x5C, 0x5C),
    INDIGO               = EGERGB(0x4B, 0x00, 0x82),
    IVORY                = EGERGB(0xFF, 0xFF, 0xF0),
    KHAKI                = EGERGB(0xF0, 0xE6, 0x8C),
    LAVENDER             = EGERGB(0xE6, 0xE6, 0xFA),
    LAVENDERBLUSH        = EGERGB(0xFF, 0xF0, 0xF5),
    LAWNGREEN            = EGERGB(0x7C, 0xFC, 0x00),
    LEMONCHIFFON         = EGERGB(0xFF, 0xFA, 0xCD),
    LIGHTBLUE            = EGERGB(0xAD, 0xD8, 0xE6),
    LIGHTCORAL           = EGERGB(0xF0, 0x80, 0x80),
    LIGHTCYAN            = EGERGB(0xE0, 0xFF, 0xFF),
    LIGHTGOLDENRODYELLOW = EGERGB(0xFA, 0xFA, 0xD2),
    LIGHTGRAY            = EGERGB(0xD3, 0xD3, 0xD3),
    LIGHTGREEN           = EGERGB(0x90, 0xEE, 0x90),
    LIGHTPINK            = EGERGB(0xFF, 0xB6, 0xC1),
    LIGHTSALMON          = EGERGB(0xFF, 0xA0, 0x7A),
    LIGHTSEAGREEN        = EGERGB(0x20, 0xB2, 0xAA),
    LIGHTSKYBLUE         = EGERGB(0x87, 0xCE, 0xFA),
    LIGHTSLATEGRAY       = EGERGB(0x77, 0x88, 0x99),
    LIGHTSTEELBLUE       = EGERGB(0xB0, 0xC4, 0xDE),
    LIGHTYELLOW          = EGERGB(0xFF, 0xFF, 0xE0),
    LIGHTRED             = EGERGB(0xFC, 0x54, 0x54),
    LIGHTMAGENTA         = EGERGB(0xFC, 0x54, 0xFC),
    LIME                 = EGERGB(0x00, 0xFF, 0x00),
    LIMEGREEN            = EGERGB(0x32, 0xCD, 0x32),
    LINEN                = EGERGB(0xFA, 0xF0, 0xE6),
    MAGENTA              = EGERGB(0xFF, 0x00, 0xFF),
    MAROON               = EGERGB(0x80, 0x00, 0x00),
    MEDIUMAQUAMARINE     = EGERGB(0x66, 0xCD, 0xAA),
    MEDIUMBLUE           = EGERGB(0x00, 0x00, 0xCD),
    MEDIUMORCHID         = EGERGB(0xBA, 0x55, 0xD3),
    MEDIUMPURPLE         = EGERGB(0x93, 0x70, 0xDB),
    MEDIUMSEAGREEN       = EGERGB(0x3C, 0xB3, 0x71),
    MEDIUMSLATEBLUE      = EGERGB(0x7B, 0x68, 0xEE),
    MEDIUMSPRINGGREEN    = EGERGB(0x00, 0xFA, 0x9A),
    MEDIUMTURQUOISE      = EGERGB(0x48, 0xD1, 0xCC),
    MEDIUMVIOLETRED      = EGERGB(0xC7, 0x15, 0x85),
    MIDNIGHTBLUE         = EGERGB(0x19, 0x19, 0x70),
    MINTCREAM            = EGERGB(0xF5, 0xFF, 0xFA),
    MISTYROSE            = EGERGB(0xFF, 0xE4, 0xE1),
    MOCCASIN             = EGERGB(0xFF, 0xE4, 0xB5),
    NAVAJOWHITE          = EGERGB(0xFF, 0xDE, 0xAD),
    NAVY                 = EGERGB(0x00, 0x00, 0x80),
    OLDLACE              = EGERGB(0xFD, 0xF5, 0xE6),
    OLIVE                = EGERGB(0x80, 0x80, 0x00),
    OLIVEDRAB            = EGERGB(0x6B, 0x8E, 0x23),
    ORANGE               = EGERGB(0xFF, 0xA5, 0x00),
    ORANGERED            = EGERGB(0xFF, 0x45, 0x00),
    ORCHID               = EGERGB(0xDA, 0x70, 0xD6),
    PALEGOLDENROD        = EGERGB(0xEE, 0xE8, 0xAA),
    PALEGREEN            = EGERGB(0x98, 0xFB, 0x98),
    PALETURQUOISE        = EGERGB(0xAF, 0xEE, 0xEE),
    PALEVIOLETRED        = EGERGB(0xDB, 0x70, 0x93),
    PAPAYAWHIP           = EGERGB(0xFF, 0xEF, 0xD5),
    PEACHPUFF            = EGERGB(0xFF, 0xDA, 0xB9),
    PERU                 = EGERGB(0xCD, 0x85, 0x3F),
    PINK                 = EGERGB(0xFF, 0xC0, 0xCB),
    PLUM                 = EGERGB(0xDD, 0xA0, 0xDD),
    POWDERBLUE           = EGERGB(0xB0, 0xE0, 0xE6),
    PURPLE               = EGERGB(0x80, 0x00, 0x80),
    RED                  = EGERGB(0xFF, 0x00, 0x00),
    ROSYBROWN            = EGERGB(0xBC, 0x8F, 0x8F),
    ROYALBLUE            = EGERGB(0x41, 0x69, 0xE1),
    SADDLEBROWN          = EGERGB(0x8B, 0x45, 0x13),
    SALMON               = EGERGB(0xFA, 0x80, 0x72),
    SANDYBROWN           = EGERGB(0xF4, 0xA4, 0x60),
    SEAGREEN             = EGERGB(0x2E, 0x8B, 0x57),
    SEASHELL             = EGERGB(0xFF, 0xF5, 0xEE),
    SIENNA               = EGERGB(0xA0, 0x52, 0x2D),
    SILVER               = EGERGB(0xC0, 0xC0, 0xC0),
    SKYBLUE              = EGERGB(0x87, 0xCE, 0xEB),
    SLATEBLUE            = EGERGB(0x6A, 0x5A, 0xCD),
    SLATEGRAY            = EGERGB(0x70, 0x80, 0x90),
    SNOW                 = EGERGB(0xFF, 0xFA, 0xFA),
    SPRINGGREEN          = EGERGB(0x00, 0xFF, 0x7F),
    STEELBLUE            = EGERGB(0x46, 0x82, 0xB4),
    TAN                  = EGERGB(0xD2, 0xB4, 0x8C),
    TEAL                 = EGERGB(0x00, 0x80, 0x80),
    THISTLE              = EGERGB(0xD8, 0xBF, 0xD8),
    TOMATO               = EGERGB(0xFF, 0x63, 0x47),
    TURQUOISE            = EGERGB(0x40, 0xE0, 0xD0),
    VIOLET               = EGERGB(0xEE, 0x82, 0xEE),
    WHEAT                = EGERGB(0xF5, 0xDE, 0xB3),
    WHITE                = EGERGB(0xFF, 0xFF, 0xFF),
    WHITESMOKE           = EGERGB(0xF5, 0xF5, 0xF5),
    YELLOW               = EGERGB(0xFF, 0xFF, 0x00),
    YELLOWGREEN          = EGERGB(0x9A, 0xCD, 0x32)
};

/**
 * @enum line_styles
 * @brief 线条样式
 * 
 * 定义了绘制线条时可以使用的不同样式
 */
enum line_styles
{
    SOLID_LINE           = PS_SOLID,        ///< 实线
    CENTER_LINE          = PS_DASH,         ///< 中心线（虚线）
    DOTTED_LINE          = PS_DOT,          ///< 点线
    DASHED_LINE          = PS_DASHDOT,      ///< 点划线
    NULL_LINE            = PS_NULL,         ///< 空线（不绘制）
    USERBIT_LINE         = PS_USERSTYLE     ///< 用户自定义线条样式
};

/**
 * @struct line_style_type
 * @brief 线条样式结构
 * 
 * 描述线条的详细样式属性
 */
struct line_style_type
{
    int             linestyle;  ///< 线条样式
    unsigned short  upattern;   ///< 用户定义的线条模式
    int             thickness;  ///< 线条粗细
};

/**
 * @enum line_cap_type
 * @brief 线条端点样式
 * 
 * 定义了线条两端的绘制样式
 */
enum line_cap_type
{
    LINECAP_FLAT   = 0,     ///< 平直端点
    LINECAP_SQUARE,         ///< 方形端点  
    LINECAP_ROUND           ///< 圆形端点
};

/**
 * @enum line_join_type
 * @brief 线条连接样式
 * 
 * 定义了多条线连接处的绘制样式
 */
enum line_join_type
{
    LINEJOIN_MITER = 0,     ///< 尖角连接
    LINEJOIN_BEVEL,         ///< 斜角连接
    LINEJOIN_ROUND          ///< 圆角连接
};

/**
 * @enum fill_patterns
 * @brief 填充图案
 * 
 * 定义了几何图形填充时可以使用的不同图案样式
 */
enum fill_patterns
{
    EMPTY_FILL,         ///< 无填充
    SOLID_FILL,         ///< 实心填充（使用填充色填充）
    LINE_FILL,          ///< 水平线填充 ---
    LTSLASH_FILL,       ///< 细斜线填充 "///"
    SLASH_FILL,         ///< 粗斜线填充 "///"
    BKSLASH_FILL,       ///< 粗反斜线填充 "\\\"
    LTBKSLASH_FILL,     ///< 细反斜线填充 "\\\"
    HATCH_FILL,         ///< 浅网格填充
    XHATCH_FILL,        ///< 深交叉网格填充
    INTERLEAVE_FILL,    ///< 交错线填充
    WIDE_DOT_FILL,      ///< 稀疏点填充
    CLOSE_DOT_FILL,     ///< 密集点填充
    USER_FILL           ///< 用户自定义填充
};

/**
 * @enum fill_mode
 * @brief 填充模式
 * 
 * 定义了复杂图形的填充算法
 */
enum fill_mode
{
    FILLMODE_DEFAULT   = 0,     ///< 默认填充模式
    FILLMODE_ALTERNATE = 1,     ///< 交替填充模式
    FILLMODE_WINDING   = 2      ///< 回卷填充模式
};

/**
 * @enum text_just
 * @brief 文本对齐方式
 * 
 * 定义了文本的水平和垂直对齐方式
 */
enum text_just
{
    LEFT_TEXT            = 0,   ///< 左对齐
    CENTER_TEXT          = 1,   ///< 居中对齐
    RIGHT_TEXT           = 2,   ///< 右对齐

    TOP_TEXT             = 0,   ///< 上对齐
/*  CENTER_TEXT          = 1,     已在上面定义 */
    BOTTOM_TEXT          = 2    ///< 下对齐
};

/**
 * @struct textsettingstype
 * @brief 文本设置结构
 * 
 * 包含文本的字体、方向、大小和对齐方式等设置
 */
struct textsettingstype
{
    int font;       ///< 字体类型
    int direction;  ///< 文字方向
    int charsize;   ///< 字符大小
    int horiz;      ///< 水平对齐方式
    int vert;       ///< 垂直对齐方式
};

/**
 * @enum font_styles
 * @brief 字体样式
 * 
 * 定义了字体的各种样式，可以通过位或操作组合使用
 */
enum font_styles
{
    FONTSTYLE_BOLD       = 1,   ///< 粗体
    FONTSTYLE_ITALIC     = 2,   ///< 斜体  
    FONTSTYLE_UNDERLINE  = 4,   ///< 下划线
    FONTSTYLE_STRIKEOUT  = 8    ///< 删除线
};

/**
 * @enum music_state_flag
 * @brief 音乐播放状态标志
 * 
 * 定义了音乐播放器的各种状态
 */
enum music_state_flag
{
    MUSIC_MODE_NOT_OPEN  = 0x0,     ///< 未打开状态
    MUSIC_MODE_NOT_READY = 0x20C,   ///< 未就绪状态
    MUSIC_MODE_PAUSE     = 0x211,   ///< 暂停状态
    MUSIC_MODE_PLAY      = 0x20E,   ///< 播放状态
    MUSIC_MODE_STOP      = 0x20D,   ///< 停止状态
    MUSIC_MODE_OPEN      = 0x212,   ///< 已打开状态
    MUSIC_MODE_SEEK      = 0x210    ///< 定位状态
};

/// @brief 音乐操作错误代码
#define MUSIC_ERROR  0xFFFFFFFF

/**
 * @enum key_msg_flag
 * @brief 按键消息标志
 * 
 * 定义了按键消息的类型和状态标志
 */
enum key_msg_flag
{
    KEYMSG_CHAR_FLAG     = 2,       ///< 字符消息标志
    KEYMSG_DOWN_FLAG     = 1,       ///< 按下消息标志
    KEYMSG_UP_FLAG       = 1,       ///< 释放消息标志

    KEYMSG_CHAR          = 0x40000, ///< 字符消息
    KEYMSG_DOWN          = 0x10000, ///< 按键按下消息
    KEYMSG_UP            = 0x20000, ///< 按键释放消息
    KEYMSG_FIRSTDOWN     = 0x80000  ///< 首次按下消息
};

/**
 * @enum key_code_e
 * @brief 键盘和鼠标键码
 * 
 * 定义了所有可以检测的键盘按键和鼠标按钮的键码值
 * 键码值基于Windows虚拟键码(Virtual Key Codes)
 */
enum key_code_e
{
    // 鼠标按钮
    key_mouse_l         = 0x01,     ///< 鼠标左键
    key_mouse_r         = 0x02,     ///< 鼠标右键
    key_mouse_m         = 0x04,     ///< 鼠标中键
    key_mouse_x1        = 0x05,     ///< 鼠标X1键
    key_mouse_x2        = 0x06,     ///< 鼠标X2键
    
    // 特殊功能键
    key_back            = 0x08,     ///< 退格键 (Backspace)
    key_tab             = 0x09,     ///< 制表键 (Tab)
    key_enter           = 0x0d,     ///< 回车键 (Enter)
    key_shift           = 0x10,     ///< Shift键
    key_control         = 0x11,     ///< Ctrl键
    key_menu            = 0x12,     ///< Alt键
    key_pause           = 0x13,     ///< 暂停键 (Pause)
    key_capslock        = 0x14,     ///< 大写锁定键 (Caps Lock)
    key_esc             = 0x1b,     ///< 逃逸键 (Escape)
    key_space           = 0x20,     ///< 空格键 (Space)

    // 导航键
    key_pageup          = 0x21,     ///< 向上翻页键 (Page Up)
    key_pagedown        = 0x22,     ///< 向下翻页键 (Page Down)
    key_end             = 0x23,     ///< 结束键 (End)
    key_home            = 0x24,     ///< 主页键 (Home)

    // 方向键
    key_left            = 0x25,     ///< 左方向键
    key_up              = 0x26,     ///< 上方向键
    key_right           = 0x27,     ///< 右方向键
    key_down            = 0x28,     ///< 下方向键

    // 编辑键
    key_print           = 0x2a,     ///< 打印键 (Print)
    key_snapshot        = 0x2c,     ///< 截图键 (Print Screen)
    key_insert          = 0x2d,     ///< 插入键 (Insert)
    key_delete          = 0x2e,     ///< 删除键 (Delete)

    // 数字键 (主键盘区)
    key_0               = 0x30,     ///< 数字键0
    key_1               = 0x31,     ///< 数字键1
    key_2               = 0x32,     ///< 数字键2
    key_3               = 0x33,     ///< 数字键3
    key_4               = 0x34,     ///< 数字键4
    key_5               = 0x35,     ///< 数字键5
    key_6               = 0x36,     ///< 数字键6
    key_7               = 0x37,     ///< 数字键7
    key_8               = 0x38,     ///< 数字键8
    key_9               = 0x39,     ///< 数字键9

    // 字母键
    key_A               = 0x41,     ///< 字母键A
    key_B               = 0x42,     ///< 字母键B
    key_C               = 0x43,     ///< 字母键C
    key_D               = 0x44,     ///< 字母键D
    key_E               = 0x45,     ///< 字母键E
    key_F               = 0x46,     ///< 字母键F
    key_G               = 0x47,     ///< 字母键G
    key_H               = 0x48,     ///< 字母键H
    key_I               = 0x49,     ///< 字母键I
    key_J               = 0x4a,     ///< 字母键J
    key_K               = 0x4b,     ///< 字母键K
    key_L               = 0x4c,     ///< 字母键L
    key_M               = 0x4d,     ///< 字母键M
    key_N               = 0x4e,     ///< 字母键N
    key_O               = 0x4f,     ///< 字母键O
    key_P               = 0x50,     ///< 字母键P
    key_Q               = 0x51,     ///< 字母键Q
    key_R               = 0x52,     ///< 字母键R
    key_S               = 0x53,     ///< 字母键S
    key_T               = 0x54,     ///< 字母键T
    key_U               = 0x55,     ///< 字母键U
    key_V               = 0x56,     ///< 字母键V
    key_W               = 0x57,     ///< 字母键W
    key_X               = 0x58,     ///< 字母键X
    key_Y               = 0x59,     ///< 字母键Y
    key_Z               = 0x5a,     ///< 字母键Z
    
    // Windows键
    key_win_l           = 0x5b,     ///< 左Windows键
    key_win_r           = 0x5c,     ///< 右Windows键

    key_sleep           = 0x5f,     ///< 休眠键

    // 数字键盘
    key_num0            = 0x60,     ///< 数字键盘0
    key_num1            = 0x61,     ///< 数字键盘1
    key_num2            = 0x62,     ///< 数字键盘2
    key_num3            = 0x63,     ///< 数字键盘3
    key_num4            = 0x64,     ///< 数字键盘4
    key_num5            = 0x65,     ///< 数字键盘5
    key_num6            = 0x66,     ///< 数字键盘6
    key_num7            = 0x67,     ///< 数字键盘7
    key_num8            = 0x68,     ///< 数字键盘8
    key_num9            = 0x69,     ///< 数字键盘9

    // 数字键盘运算符
    key_multiply        = 0x6a,     ///< 数字键盘乘号 (*)
    key_add             = 0x6b,     ///< 数字键盘加号 (+)
    key_separator       = 0x6c,     ///< 数字键盘分隔符
    key_subtract        = 0x6d,     ///< 数字键盘减号 (-)
    key_decimal         = 0x6e,     ///< 数字键盘小数点 (.)
    key_divide          = 0x6f,     ///< 数字键盘除号 (/)

    // 功能键
    key_f1              = 0x70,     ///< F1功能键
    key_f2              = 0x71,     ///< F2功能键
    key_f3              = 0x72,     ///< F3功能键
    key_f4              = 0x73,     ///< F4功能键
    key_f5              = 0x74,     ///< F5功能键
    key_f6              = 0x75,     ///< F6功能键
    key_f7              = 0x76,     ///< F7功能键
    key_f8              = 0x77,     ///< F8功能键
    key_f9              = 0x78,     ///< F9功能键
    key_f10             = 0x79,     ///< F10功能键
    key_f11             = 0x7a,     ///< F11功能键
    key_f12             = 0x7b,     ///< F12功能键

    // 锁定键
    key_numlock         = 0x90,     ///< 数字锁定键 (Num Lock)
    key_scrolllock      = 0x91,     ///< 滚动锁定键 (Scroll Lock)

    // 左右区分的修饰键
    key_shift_l         = 0xa0,     ///< 左Shift键
    key_shift_r         = 0xa1,     ///< 右Shift键
    key_control_l       = 0xa2,     ///< 左Ctrl键
    key_control_r       = 0xa3,     ///< 右Ctrl键
    key_menu_l          = 0xa4,     ///< 左Alt键
    key_menu_r          = 0xa5,     ///< 右Alt键

    // 标点符号键
    key_semicolon       = 0xba,     ///< 分号键 (;)
    key_plus            = 0xbb,     ///< 等号/加号键 (=)
    key_comma           = 0xbc,     ///< 逗号键 (,)
    key_minus           = 0xbd,     ///< 减号/下划线键 (-)
    key_period          = 0xbe,     ///< 句号键 (.)
    key_slash           = 0xbf,     ///< 斜杠键 (/)
    key_tilde           = 0xc0,     ///< 波浪号键 (~)
    key_lbrace          = 0xdb,     ///< 左方括号键 ([)
    key_backslash       = 0xdc,     ///< 反斜杠键 (\)
    key_rbrace          = 0xdd,     ///< 右方括号键 (])
    key_quote           = 0xde,     ///< 引号键 (')

    key_ime_process     = 0xe5      ///< 输入法处理键
};

/**
 * @enum key_msg_e
 * @brief 按键消息类型
 * 
 * 定义了按键事件的具体类型
 */
enum key_msg_e
{
    key_msg_down        = 1,    ///< 按键按下消息
    key_msg_up          = 2,    ///< 按键释放消息
    key_msg_char        = 4     ///< 字符输入消息
};

/**
 * @enum key_flag_e
 * @brief 按键状态标志
 * 
 * 定义了按键事件的修饰键状态和特殊标志
 */
enum key_flag_e
{
    key_flag_shift      = 0x100,    ///< Shift键被按下
    key_flag_ctrl       = 0x200,    ///< Ctrl键被按下
    key_flag_first_down = 0x80000   ///< 首次按下标志
};

/**
 * @struct key_msg
 * @brief 按键消息结构
 * 
 * 包含完整的按键事件信息
 */
struct key_msg
{
    int             key;    ///< 按键键码
    key_msg_e       msg;    ///< 消息类型
    unsigned int    flags;  ///< 状态标志
};

/**
 * @enum mouse_msg_e
 * @brief 鼠标消息类型
 * 
 * 定义了鼠标事件的具体类型
 */
enum mouse_msg_e
{
    mouse_msg_down      = 0x10,     ///< 鼠标按钮按下消息
    mouse_msg_up        = 0x20,     ///< 鼠标按钮释放消息
    mouse_msg_move      = 0x40,     ///< 鼠标移动消息
    mouse_msg_wheel     = 0x80      ///< 鼠标滚轮消息
};

/**
 * @enum mouse_flag_e
 * @brief 鼠标状态标志
 * 
 * 定义了鼠标事件中各按钮和修饰键的状态
 */
enum mouse_flag_e
{
    mouse_flag_left     = 0x001,    ///< 鼠标左键被按下
    mouse_flag_right    = 0x002,    ///< 鼠标右键被按下
    mouse_flag_mid      = 0x004,    ///< 鼠标中键被按下
    mouse_flag_x1       = 0x008,    ///< 鼠标X1键被按下
    mouse_flag_x2       = 0x010,    ///< 鼠标X2键被按下
    mouse_flag_shift    = 0x100,    ///< Shift键被按下
    mouse_flag_ctrl     = 0x200     ///< Ctrl键被按下
};

/**
 * @struct mouse_msg
 * @brief 鼠标消息结构
 * 
 * 包含完整的鼠标事件信息，提供了便捷的状态查询方法
 */
struct mouse_msg
{
    int             x;      ///< 鼠标x坐标
    int             y;      ///< 鼠标y坐标
    mouse_msg_e     msg;    ///< 消息类型
    unsigned int    flags;  ///< 状态标志
    int             wheel;  ///< 滚轮滚动增量

    /// @brief 检查是否为鼠标左键事件
    bool is_left()  const {return (flags & mouse_flag_left)  != 0;}
    /// @brief 检查是否为鼠标右键事件
    bool is_right() const {return (flags & mouse_flag_right) != 0;}
    /// @brief 检查是否为鼠标中键事件
    bool is_mid()   const {return (flags & mouse_flag_mid)   != 0;}
    /// @brief 检查是否为鼠标X1键事件
    bool is_x1()    const {return (flags & mouse_flag_x1)    != 0;}
    /// @brief 检查是否为鼠标X2键事件
    bool is_x2()    const {return (flags & mouse_flag_x2)    != 0;}

    /// @brief 检查是否为按钮按下事件
    bool is_down()  const {return msg == mouse_msg_down; }
    /// @brief 检查是否为按钮释放事件
    bool is_up()    const {return msg == mouse_msg_up;   }
    /// @brief 检查是否为鼠标移动事件
    bool is_move()  const {return msg == mouse_msg_move; }
    /// @brief 检查是否为滚轮事件
    bool is_wheel() const {return msg == mouse_msg_wheel;}
};

/**
 * @struct MOUSEMSG
 * @brief 传统鼠标消息结构（兼容性）
 * 
 * 提供与旧版本兼容的鼠标消息格式
 */
struct MOUSEMSG
{
    UINT  uMsg;         ///< Windows消息ID
    bool  mkCtrl;       ///< Ctrl键状态
    bool  mkShift;      ///< Shift键状态
    bool  mkLButton;    ///< 左键状态
    bool  mkMButton;    ///< 中键状态
    bool  mkRButton;    ///< 右键状态
    bool  mkXButton1;   ///< X1键状态
    bool  mkXButton2;   ///< X2键状态
    short x;            ///< x坐标
    short y;            ///< y坐标
    short wheel;        ///< 滚轮增量
};

/**
 * @struct viewporttype
 * @brief 视口类型结构
 * 
 * 定义了绘图视口的边界矩形
 */
/**
 * @struct viewporttype
 * @brief 视口类型结构
 * 
 * 定义了绘图视口的边界矩形
 */
struct viewporttype
{
    int left;       ///< 左边界
    int top;        ///< 上边界
    int right;      ///< 右边界
    int bottom;     ///< 下边界
    int clipflag;   ///< 裁剪标志
};

/**
 * @struct ege_transform_matrix
 * @brief 2D变换矩阵
 * 
 * 用于2D图形变换的3x2矩阵，支持平移、旋转、缩放等变换
 */
struct ege_transform_matrix
{
    float m11, m12;     ///< 第一行：[m11, m12]
    float m21, m22;     ///< 第二行：[m21, m22]
    float m31, m32;     ///< 第三行：[m31, m32] 平移分量
};

/**
 * @struct ege_path
 * @brief 图形路径
 * 
 * 用于定义复杂的图形路径，支持直线、曲线等图形元素的组合
 */
struct ege_path
{
private:
    void* m_data;       ///< 内部数据指针

public:
    /// @brief 默认构造函数
    ege_path();
    
    /// @brief 从点数组和类型数组构造路径
    /// @param points 点数组
    /// @param types 路径类型数组
    /// @param count 点的数量
    ege_path(const ege_point* points, const unsigned char* types, int count);
    
    /// @brief 拷贝构造函数
    /// @param path 要拷贝的路径
    ege_path(const ege_path& path);
    
    /// @brief 析构函数
    virtual ~ege_path();

    /// @brief 获取只读数据指针
    /// @return 常量数据指针
    const void* data() const;
    
    /// @brief 获取可写数据指针
    /// @return 数据指针
    void* data();
    
    /// @brief 赋值操作符
    /// @param path 要赋值的路径
    /// @return 路径引用
    ege_path& operator=(const ege_path& path);
};

/**
 * @struct msg_createwindow
 * @brief 创建窗口消息结构
 * 
 * 用于窗口创建时传递参数的消息结构
 */
struct msg_createwindow
{
    HANDLE  hEvent;         ///< 事件句柄
    HWND    hwnd;           ///< 窗口句柄
    const wchar_t* classname; ///< 窗口类名
    DWORD   style;          ///< 窗口样式
    DWORD   exstyle;        ///< 扩展窗口样式
    size_t  id;             ///< 窗口ID
    LPVOID  param;          ///< 参数指针
};

/// @brief 通用回调函数类型
typedef void (CALLBACK_PROC)();

/// @brief 键盘消息处理回调函数类型
/// @param userdata 用户数据指针
/// @param message 消息类型
/// @param key 键码
/// @return 处理结果
typedef int (__stdcall MSG_KEY_PROC  )(void*, unsigned, int);

/// @brief 鼠标消息处理回调函数类型
/// @param userdata 用户数据指针
/// @param message 消息类型
/// @param x x坐标
/// @param y y坐标
/// @param flags 标志位
/// @return 处理结果
typedef int (__stdcall MSG_MOUSE_PROC)(void*, unsigned, int, int, int);

/// @brief 回调函数指针类型
typedef CALLBACK_PROC       * LPCALLBACK_PROC;
/// @brief 键盘消息处理函数指针类型
typedef MSG_KEY_PROC        * LPMSG_KEY_PROC;
/// @brief 鼠标消息处理函数指针类型
typedef MSG_MOUSE_PROC      * LPMSG_MOUSE_PROC;

struct VECTOR3D;

/// @brief 绕X轴旋转3D点
/// @param point 要旋转的3D点指针
/// @param rad 旋转角度（弧度）
void EGEAPI rotate_point3d_x(VECTOR3D* point, float rad);

/// @brief 绕Y轴旋转3D点
/// @param point 要旋转的3D点指针
/// @param rad 旋转角度（弧度）
void EGEAPI rotate_point3d_y(VECTOR3D* point, float rad);

/// @brief 绕Z轴旋转3D点
/// @param point 要旋转的3D点指针
/// @param rad 旋转角度（弧度）
void EGEAPI rotate_point3d_z(VECTOR3D* point, float rad);

/**
 * @struct VECTOR3D
 * @brief 3D向量结构
 * 
 * 表示三维空间中的向量或点，提供了基本的3D图形运算功能
 */
struct VECTOR3D
{
    float x, y, z;      ///< 三维坐标分量

    /// @brief 默认构造函数，初始化为原点
    VECTOR3D() : x(0.0f), y(0.0f), z(0.0f) {}
    
    /// @brief 构造函数
    /// @param x x坐标
    /// @param y y坐标
    /// @param z z坐标（默认为0）
    VECTOR3D(float x, float y, float z = 0.0f) : x(x), y(y), z(z) {}    /// @brief 赋值操作符
    /// @param vector 要赋值的向量
    /// @return 向量引用
    VECTOR3D& operator=(const VECTOR3D& vector)
    {
        x = vector.x;
        y = vector.y;
        z = vector.z;
        return *this;
    }

    /// @brief 向量加法赋值操作符
    VECTOR3D& operator+=(const VECTOR3D& vector);
    /// @brief 向量减法赋值操作符
    VECTOR3D& operator-=(const VECTOR3D& vector);
    /// @brief 向量加法操作符
    VECTOR3D  operator+ (const VECTOR3D& vector) const;
    /// @brief 向量减法操作符
    VECTOR3D  operator- (const VECTOR3D& vector) const;
    /// @brief 标量乘法赋值操作符
    VECTOR3D& operator*=(float scale);
    /// @brief 标量乘法操作符
    VECTOR3D  operator* (float scale) const;
    /// @brief 向量点积操作符
    float     operator* (const VECTOR3D& vector) const;
    /// @brief 向量叉积操作符
    VECTOR3D  operator& (const VECTOR3D& vector) const;
    /// @brief 向量叉积赋值操作符
    VECTOR3D& operator&=(const VECTOR3D& vector);
    
    /// @brief 获取向量模长
    /// @return 向量的模长
    float     GetModule() const;

    /// @brief 获取向量模长的平方
    /// @return 向量模长的平方
    float GetSqrModule() const { return float(x * x + y * y + z * z); }

    /// @brief 设置向量模长
    /// @param m 新的模长
    /// @return 向量引用
    VECTOR3D& SetModule(float m)
    {
        float t  = m / GetModule();
        *this   *= t;
        return *this;
    }

    /// @brief 绕指定轴旋转向量
    /// @param rad 旋转角度（弧度）
    /// @param vector 旋转轴向量
    /// @return 向量引用
    VECTOR3D& Rotate(float rad, const VECTOR3D& vector);

    /// @brief 绕指定轴旋转向量
    /// @param rad 旋转角度（弧度）
    /// @param x 旋转轴x分量
    /// @param y 旋转轴y分量
    /// @param z 旋转轴z分量
    /// @return 向量引用
    VECTOR3D& Rotate(float rad, float x, float y, float z)
    {
        VECTOR3D v(x, y, z);
        return Rotate(rad, v);
    }

    /// @brief 从起始向量旋转到目标向量
    /// @param e 目标向量
    /// @param s 起始向量（默认为z轴正方向）
    /// @return 向量引用
    VECTOR3D&    Rotate  (const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f));
    
    /// @brief 计算两个向量之间的夹角
    /// @param e 第一个向量
    /// @param s 第二个向量（默认为z轴正方向）
    /// @return 夹角（弧度）
    static float GetAngle(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f));
};

/// @brief 图像对象前置声明
class IMAGE;
/// @brief 图像对象指针类型
typedef IMAGE *PIMAGE;
/// @brief 常量图像对象指针类型
typedef const IMAGE *PCIMAGE;

/**
 * @brief 设置代码页
 * 
 * 设置字符编码，影响文本处理和显示
 * @param codepage 代码页，应使用 EGE_CODEPAGE_XXX 常量，默认为 EGE_CODEPAGE_ANSI
 */
void EGEAPI setcodepage(unsigned int codepage);

/**
 * @brief 获取当前代码页
 * 
 * @return 当前设置的代码页
 */
unsigned int EGEAPI getcodepage();

/**
 * @brief 设置是否启用Unicode字符消息
 * 
 * 控制 getkey() 函数是否使用UTF-16编码的字符消息
 * @param enable true启用UTF-16，false使用ANSI
 */
void EGEAPI setunicodecharmessage(bool enable);

/**
 * @brief 获取Unicode字符消息设置状态
 * 
 * @return true表示启用UTF-16，false表示使用ANSI
 */
bool EGEAPI getunicodecharmessage();

/**
 * @brief 设置初始化模式
 * 
 * 设置窗口创建时的默认参数
 * @param mode 初始化模式标志
 * @param x 窗口初始x坐标（默认为CW_USEDEFAULT）
 * @param y 窗口初始y坐标（默认为CW_USEDEFAULT）
 */
void EGEAPI setinitmode(initmode_flag mode, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT);
inline void setinitmode(int mode, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT)
{
    setinitmode(static_cast<initmode_flag>(mode), x, y);
}

/**
 * @brief 获取当前初始化模式
 * 
 * @return 当前设置的初始化模式标志
 */
initmode_flag  EGEAPI getinitmode();

/**
 * @brief 创建 EGE 图形化窗口，并进行环境初始化
 * 
 * 这是EGE图形库的主要初始化函数，执行后会创建并显示图形窗口
 * 
 * @param width 窗口宽度（像素）
 * @param height 窗口高度（像素）
 * @param mode 初始化模式标志，控制窗口的各种属性
 * 
 * @code
 * // 创建一个800x600的默认窗口
 * initgraph(800, 600, INIT_DEFAULT);
 * 
 * // 创建一个无边框的置顶窗口
 * initgraph(640, 480, INIT_NOBORDER | INIT_TOPMOST);
 * @endcode
 */
void EGEAPI initgraph(int width, int height, initmode_flag mode);
inline void initgraph(int width, int height, int mode)
{
    initgraph(width, height, static_cast<initmode_flag>(mode));
}

/**
 * @brief 创建 EGE 图形化窗口（简化版本）
 * 
 * 使用当前设置的初始化模式创建图形窗口
 * 在调试版本中使用默认模式，在发布版本中显示EGE标志
 * 
 * @param width 窗口宽度（像素）
 * @param height 窗口高度（像素）
 */
inline void EGEAPI initgraph(int width, int height)
{
#if !defined(NDEBUG) || defined(DEBUG) || defined(_DEBUG)
    initgraph(width, height, getinitmode());
#else
    initgraph(width, height, getinitmode() | INIT_WITHLOGO);
#endif
}

/**
 * @brief 初始化图形系统（BGI兼容版本）
 * 
 * 提供与传统BGI图形库的兼容接口
 * 
 * @param graphdriver 图形驱动类型指针，通常传入DETECT以自动检测
 * @param graphmode 图形模式指针，通常传入0以自动选择
 * @param pathtodriver BGI驱动文件路径，若驱动文件在当前目录可传空字符串""
 */
void initgraph(int *graphdriver, int *graphmode, const char *pathtodriver);

/**
 * @brief 关闭图形系统
 * 
 * 逻辑上关闭图形系统
 * 执行后EGE窗口会隐藏，但资源不会被全部释放，is_run()函数仍然返回true (需注意)
 * 如果需要释放 IMAGE 对象资源，仍需调用 delimage 函数
 */
void EGEAPI closegraph();

/**
 * @brief 检查图形环境是否正在运行
 * 
 * @return true 当EGE图形环境存在且窗口未被关闭时
 * @return false 当EGE图形环境不存在或用户点击关闭按钮后
 */
bool EGEAPI is_run();

/**
 * @brief 设置窗口标题
 * @param caption 窗口标题字符串
 */
void EGEAPI setcaption(const char* caption);

/**
 * @brief 设置窗口标题(Unicode版本)
 * @param caption 窗口标题宽字符串
 */
void EGEAPI setcaption(const wchar_t* caption);

/**
 * @brief 设置窗口图标
 * @param icon_id 图标资源ID
 */
void EGEAPI seticon(int icon_id);

/**
 * @brief 附加到已有的窗口句柄
 * @param hWnd 要附加的窗口句柄
 * @return 操作结果代码
 */
int  EGEAPI attachHWND(HWND hWnd);

/**
 * @brief 显示窗口
 */
void EGEAPI showwindow();

/**
 * @brief 隐藏窗口
 */
void EGEAPI hidewindow();

/**
 * @brief 移动窗口到指定位置
 * @param x 新的x坐标
 * @param y 新的y坐标
 * @param redraw 是否重绘窗口（默认为true）
 */
void EGEAPI movewindow(int x, int y, bool redraw = true);

/**
 * @brief 调整窗口大小
 * @param width 新的窗口宽度
 * @param height 新的窗口高度
 */
void EGEAPI resizewindow(int width, int height);

/**
 * @brief 刷新窗口显示
 */
void EGEAPI flushwindow();

/**
 * @brief 设置渲染模式
 * @param mode 渲染模式（自动或手动）
 */
void EGEAPI setrendermode(rendermode_e mode);

/**
 * @brief 获取当前绘图目标
 * @return 当前绘图目标图像指针，NULL表示屏幕
 */
PIMAGE      gettarget();

/**
 * @brief 设置绘图目标
 * @param pbuf 目标图像指针，NULL表示屏幕
 * @return 设置结果代码
 */
int         settarget(PIMAGE pbuf);

/**
 * @brief 清空设备（清屏）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI cleardevice(PIMAGE pimg = NULL);

/**
 * @brief 获取视口设置
 * @param left 左边界输出指针
 * @param top 上边界输出指针
 * @param right 右边界输出指针
 * @param bottom 下边界输出指针
 * @param clip 裁剪标志输出指针（可选）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI getviewport(int* left, int* top, int* right, int* bottom, int* clip = 0, PCIMAGE pimg = NULL);

/**
 * @brief 设置视口
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param clip 是否启用裁剪（默认为1）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setviewport(int  left, int  top, int  right, int  bottom, int  clip = 1, PIMAGE  pimg = NULL);

/**
 * @brief 清空视口区域
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI clearviewport(PIMAGE pimg = NULL);

/**
 * @deprecated 已弃用，请使用图像函数替代
 * @brief 设置活动页面
 * @param page 页面编号
 */
EGE_DEPRECATE(setactivepage, "Please use the image function instead.")
void EGEAPI setactivepage(int page);

/**
 * @deprecated 已弃用，请使用图像函数替代
 * @brief 设置可视页面
 * @param page 页面编号
 */
EGE_DEPRECATE(setvisualpage, "Please use the image function instead.")
void EGEAPI setvisualpage(int page);

/**
 * @deprecated 已弃用，请使用图像函数替代
 * @brief 交换页面
 */
EGE_DEPRECATE(swappage, "Please use the image function instead.")
void EGEAPI swappage();

/**
 * @brief 获取窗口视口设置
 * @param viewport 视口结构指针
 */
void EGEAPI window_getviewport(viewporttype * viewport);

/**
 * @brief 获取窗口视口设置
 * @param left 左边界输出指针
 * @param top 上边界输出指针
 * @param right 右边界输出指针
 * @param bottom 下边界输出指针
 */
void EGEAPI window_getviewport(int* left, int* top, int* right, int* bottom);

/**
 * @brief 设置窗口视口
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 */
void EGEAPI window_setviewport(int  left, int  top, int  right, int  bottom);

/**
 * @brief 设置线条宽度
 * @param width 线条宽度（像素）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setlinewidth(float width, PIMAGE pimg = NULL);

/**
 * @brief 获取线条样式
 * @param linestyle 线条样式输出指针
 * @param pattern 线条图案输出指针（可选）
 * @param thickness 线条粗细输出指针（可选）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI getlinestyle(int* linestyle, unsigned short* pattern = NULL, int* thickness = NULL, PCIMAGE pimg = NULL);

/**
 * @brief 设置线条样式
 * @param linestyle 线条样式
 * @param pattern 线条图案（默认为0）
 * @param thickness 线条粗细（默认为1）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setlinestyle(int  linestyle, unsigned short  pattern = 0,    int  thickness = 1,    PIMAGE  pimg = NULL);

/**
 * @brief 设置线条端点样式
 * @param linecap 端点样式
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setlinecap(line_cap_type linecap, PIMAGE pimg = NULL);

/**
 * @brief 设置线条端点样式（分别设置起始和结束端点）
 * @param startCap 起始端点样式
 * @param endCap 结束端点样式
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setlinecap(line_cap_type  startCap, line_cap_type  endCap, PIMAGE  pimg = NULL);

/**
 * @brief 获取线条端点样式
 * @param startCap 起始端点样式输出指针
 * @param endCap 结束端点样式输出指针
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI getlinecap(line_cap_type* startCap, line_cap_type* endCap, PCIMAGE pimg = NULL);

/**
 * @brief 获取线条端点样式
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 端点样式
 */
line_cap_type EGEAPI getlinecap(PCIMAGE pimg = NULL);

/**
 * @brief 设置线条连接样式
 * @param linejoin 连接样式
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setlinejoin(line_join_type  linejoin, PIMAGE pimg = NULL);

/**
 * @brief 设置线条连接样式（带斜接限制）
 * @param linejoin 连接样式
 * @param miterLimit 斜接限制值
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setlinejoin(line_join_type  linejoin, float  miterLimit, PIMAGE pimg = NULL);

/**
 * @brief 获取线条连接样式
 * @param linejoin 连接样式输出指针
 * @param miterLimit 斜接限制值输出指针
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI getlinejoin(line_join_type* linejoin, float* miterLimit, PCIMAGE pimg = NULL);

/**
 * @brief 获取线条连接样式
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 连接样式
 */
line_join_type EGEAPI getlinejoin(PCIMAGE pimg = NULL);

/**
 * @brief 设置填充样式
 * @param pattern 填充图案类型
 * @param color 填充颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setfillstyle(int pattern, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置写入模式
 * @param mode 写入模式
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setwritemode(int mode, PIMAGE pimg = NULL);

/**
 * @brief 获取绘图颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 当前绘图颜色
 */
color_t EGEAPI getcolor      (PCIMAGE pimg = NULL);

/**
 * @brief 获取线条颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 当前线条颜色
 */
color_t EGEAPI getlinecolor  (PCIMAGE pimg = NULL);

/**
 * @brief 获取填充颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 当前填充颜色
 */
color_t EGEAPI getfillcolor  (PCIMAGE pimg = NULL);

/**
 * @brief 获取背景颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 当前背景颜色
 */
color_t EGEAPI getbkcolor    (PCIMAGE pimg = NULL);

/**
 * @brief 获取文本颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 当前文本颜色
 */
color_t EGEAPI gettextcolor  (PCIMAGE pimg = NULL);

/**
 * @brief 设置绘图颜色
 * @param color 绘图颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI setcolor      (color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置线条颜色
 * @param color 线条颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI setlinecolor  (color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置填充颜色
 * @param color 填充颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI setfillcolor  (color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置背景颜色, 会主动将旧背景色像素替换为新背景色像素
 * @param color 背景颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI setbkcolor    (color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置背景颜色, 但不会将旧背景色像素替换为新背景色像素
 * @param color 背景颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI setbkcolor_f  (color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置文本颜色
 * @param color 文本颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI settextcolor  (color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置字体背景颜色
 * @param color 字体背景颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI setfontbkcolor(color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置背景混合模式
 * @param bkMode 背景模式（TRANSPARENT或OPAQUE）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI setbkmode(int bkMode, PIMAGE pimg = NULL);

/// @defgroup ColorConversion 颜色转换宏定义
/// @{
#define RGBtoGRAY   rgb2gray    ///< RGB转灰度
#define RGBtoHSL    rgb2hsl     ///< RGB转HSL
#define RGBtoHSV    rgb2hsv     ///< RGB转HSV
#define HSLtoRGB    hsl2rgb     ///< HSL转RGB
#define HSVtoRGB    hsv2rgb     ///< HSV转RGB
/// @}

/**
 * @brief RGB颜色转换为灰度
 * @param rgb RGB颜色值
 * @return 灰度颜色值
 */
color_t EGEAPI rgb2gray(color_t rgb);

/**
 * @brief RGB颜色转换为HSL
 * @param rgb RGB颜色值
 * @param H 色调输出指针（0-360度）
 * @param S 饱和度输出指针（0-1）
 * @param L 亮度输出指针（0-1）
 */
void    EGEAPI rgb2hsl(color_t rgb, float* H, float* S, float* L);

/**
 * @brief RGB颜色转换为HSV
 * @param rgb RGB颜色值
 * @param H 色调输出指针（0-360度）
 * @param S 饱和度输出指针（0-1）
 * @param V 明度输出指针（0-1）
 */
void    EGEAPI rgb2hsv(color_t rgb, float* H, float* S, float* V);

/**
 * @brief HSL颜色转换为RGB
 * @param H 色调（0-360度）
 * @param S 饱和度（0-1）
 * @param L 亮度（0-1）
 * @return RGB颜色值
 */
color_t EGEAPI hsl2rgb(float H, float S, float L);

/**
 * @brief HSV颜色转换为RGB
 * @param H 色调（0-360度）
 * @param S 饱和度（0-1）
 * @param V 明度（0-1）
 * @return RGB颜色值
 */
color_t EGEAPI hsv2rgb(float H, float S, float V);

/**
 * @brief 颜色混合
 * @param dst 目标颜色
 * @param src 源颜色
 * @param alpha Alpha透明度（0-255）
 * @return 混合后的颜色
 */
color_t EGEAPI colorblend  (color_t dst, color_t src, unsigned char alpha);

/**
 * @brief 颜色混合（fast版本, 精度较低）
 * @param dst 目标颜色
 * @param src 源颜色
 * @param alpha Alpha透明度（0-255）
 * @return 混合后的颜色
 */
color_t EGEAPI colorblend_f(color_t dst, color_t src, unsigned char alpha);

/**
 * @brief Alpha混合
 * @param dst 目标颜色
 * @param src 源颜色（包含Alpha通道）
 * @return 混合后的颜色
 */
color_t EGEAPI alphablend  (color_t dst, color_t src);

/**
 * @brief Alpha混合（带透明度因子）
 * @param dst 目标颜色
 * @param src 源颜色
 * @param srcAlphaFactor 源颜色透明度因子（0-255）
 * @return 混合后的颜色
 */
color_t EGEAPI alphablend  (color_t dst, color_t src, unsigned char srcAlphaFactor);

/**
 * @brief Alpha混合（预乘Alpha）
 * @param dst 目标颜色
 * @param src 源颜色（预乘Alpha）
 * @return 混合后的颜色
 */
color_t EGEAPI alphablend_premultiplied(color_t dst, color_t src);

/**
 * @brief Alpha混合（预乘Alpha，带透明度因子）
 * @param dst 目标颜色
 * @param src 源颜色（预乘Alpha）
 * @param srcAlphaFactor 源颜色透明度因子（0-255）
 * @return 混合后的颜色
 */
color_t EGEAPI alphablend_premultiplied(color_t dst, color_t src, unsigned char srcAlphaFactor);

/**
 * @brief 获取像素颜色
 * @param x x坐标
 * @param y y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 像素颜色值
 */
color_t EGEAPI getpixel   (int x, int y, PCIMAGE pimg = NULL);

/**
 * @brief 设置像素颜色
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel   (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 获取像素颜色（快速版本, 无边界检查，无视viewport）
 * @param x x坐标
 * @param y y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 像素颜色值
 */
color_t EGEAPI getpixel_f (int x, int y, PCIMAGE pimg = NULL);

/**
 * @brief 设置像素颜色（快速版本, 无边界检查，无视viewport）
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel_f (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 批量设置像素
 * @param numOfPoints 点的数量
 * @param points 点坐标数组，格式为[x1,y1,x2,y2,...]
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixels  (int numOfPoints, const int* points, PIMAGE pimg = NULL);

/**
 * @brief 批量设置像素（快速版本, 无边界检查）
 * @param numOfPoints 点的数量
 * @param points 点坐标数组，格式为[x1,y1,x2,y2,...]
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixels_f(int numOfPoints, const int* points, PIMAGE pimg = NULL);

/**
 * @brief 设置像素(RGB 通道设置为与 ARGB 颜色混合的结果，alpha 通道保持不变)
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值（包含Alpha通道）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel_withalpha   (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置像素（保留Alpha通道，快速版本, 无边界检查）
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值（包含Alpha通道）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel_withalpha_f (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置像素 (RGB 通道替换为指定的颜色值，alpha 通道保持不变)
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel_savealpha   (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置像素（保存Alpha通道，快速版本, 无边界检查）
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel_savealpha_f (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置像素（Alpha混合）
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel_alphablend  (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置像素（Alpha混合，快速版本, 无边界检查）
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel_alphablend_f(int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置像素（Alpha混合，带透明度因子）
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值
 * @param alphaFactor 透明度因子（0-255）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel_alphablend  (int x, int y, color_t color, unsigned char alphaFactor, PIMAGE pimg = NULL);

/**
 * @brief 设置像素（Alpha混合，带透明度因子，快速版本, 无边界检查）
 * @param x x坐标
 * @param y y坐标
 * @param color 颜色值
 * @param alphaFactor 透明度因子（0-255）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI putpixel_alphablend_f(int x, int y, color_t color, unsigned char alphaFactor, PIMAGE pimg = NULL);

/**
 * @brief 移动当前绘图位置
 * @param x 新的x坐标
 * @param y 新的y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI moveto (int x,  int y,  PIMAGE pimg = NULL);

/**
 * @brief 相对移动当前绘图位置
 * @param dx x方向偏移量
 * @param dy y方向偏移量
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI moverel(int dx, int dy, PIMAGE pimg = NULL);

/**
 * @brief 绘制直线
 * @param x1 起点x坐标
 * @param y1 起点y坐标
 * @param x2 终点x坐标
 * @param y2 终点y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI line     (int   x1, int   y1, int   x2, int   y2, PIMAGE pimg = NULL);

/**
 * @brief 绘制直线（浮点版本）
 * @param x1 起点x坐标
 * @param y1 起点y坐标
 * @param x2 终点x坐标
 * @param y2 终点y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI line_f   (float x1, float y1, float x2, float y2, PIMAGE pimg = NULL);

/**
 * @brief 从当前位置绘制直线到指定点
 * @param x 终点x坐标
 * @param y 终点y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI lineto   (int   x,  int   y,  PIMAGE pimg = NULL);

/**
 * @brief 从当前位置绘制直线到指定点（浮点版本）
 * @param x 终点x坐标
 * @param y 终点y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI lineto_f (float x,  float y,  PIMAGE pimg = NULL);

/**
 * @brief 从当前位置绘制相对直线
 * @param dx x方向偏移量
 * @param dy y方向偏移量
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI linerel  (int   dx, int   dy, PIMAGE pimg = NULL);

/**
 * @brief 从当前位置绘制相对直线（浮点版本）
 * @param dx x方向偏移量
 * @param dy y方向偏移量
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void    EGEAPI linerel_f(float dx, float dy, PIMAGE pimg = NULL);

//void EGEAPI getarccoords(int *px, int *py, int *pxstart, int *pystart, int *pxend, int *pyend, PIMAGE pimg = NULL);    // ###

/**
 * @brief 绘制椭圆弧
 * @param x 椭圆中心x坐标
 * @param y 椭圆中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ellipse      (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制椭圆弧（浮点版本）
 * @param x 椭圆中心x坐标
 * @param y 椭圆中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ellipsef     (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制扇形（仅边框）
 * @param x 扇形中心x坐标
 * @param y 扇形中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI sector       (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制扇形（仅边框，浮点版本）
 * @param x 扇形中心x坐标
 * @param y 扇形中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI sectorf      (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制饼图（仅边框）
 * @param x 饼图中心x坐标
 * @param y 饼图中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI pie          (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制饼图（仅边框，浮点版本）
 * @param x 饼图中心x坐标
 * @param y 饼图中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI pief         (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充饼图
 * @param x 饼图中心x坐标
 * @param y 饼图中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillpie      (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充饼图（浮点版本）
 * @param x 饼图中心x坐标
 * @param y 饼图中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillpief     (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制实心饼图
 * @param x 饼图中心x坐标
 * @param y 饼图中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidpie     (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制实心饼图（浮点版本）
 * @param x 饼图中心x坐标
 * @param y 饼图中心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidpief    (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制圆弧
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI arc          (int   x, int   y, int   startAngle, int   endAngle, int   radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制圆弧（浮点版本）
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI arcf         (float x, float y, float startAngle, float endAngle, float radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制圆扇形
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI pieslice     (int   x, int   y, int   startAngle, int   endAngle, int   radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制圆扇形（浮点版本）
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param startAngle 起始角度（度）
 * @param endAngle 结束角度（度）
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI pieslicef    (float x, float y, float startAngle, float endAngle, float radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充椭圆
 * @param x 椭圆中心x坐标
 * @param y 椭圆中心y坐标
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillellipse  (int   x, int   y, int   xRadius,    int   yRadius,  PIMAGE pimg = NULL);

/**
 * @brief 绘制填充椭圆（浮点版本）
 * @param x 椭圆中心x坐标
 * @param y 椭圆中心y坐标
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillellipsef (float x, float y, float xRadius,    float yRadius,  PIMAGE pimg = NULL);

/**
 * @brief 绘制实心椭圆
 * @param x 椭圆中心x坐标
 * @param y 椭圆中心y坐标
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidellipse (int   x, int   y, int   xRadius,    int   yRadius,  PIMAGE pimg = NULL);

/**
 * @brief 绘制实心椭圆（浮点版本）
 * @param x 椭圆中心x坐标
 * @param y 椭圆中心y坐标
 * @param xRadius x轴半径
 * @param yRadius y轴半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidellipsef(float x, float y, float xRadius,    float yRadius,  PIMAGE pimg = NULL);

/**
 * @brief 绘制圆形边框
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI circle       (int   x, int   y, int   radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制圆形边框（浮点版本）
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI circlef      (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充圆形
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillcircle   (int   x, int   y, int   radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充圆形（浮点版本）
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillcirclef  (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制实心圆形
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidcircle  (int   x, int   y, int   radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制实心圆形（浮点版本）
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidcirclef (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制3D柱状图
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param depth 深度
 * @param topFlag 是否绘制顶面
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI bar3d        (int left, int top, int right, int bottom, int depth,   int topFlag, PIMAGE pimg = NULL);

/**
 * @brief 绘制柱状图（填充矩形）
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI bar          (int left, int top, int right, int bottom, PIMAGE pimg = NULL);

/**
 * @brief 绘制矩形边框
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI rectangle    (int left, int top, int right, int bottom, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充矩形
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillrect     (int left, int top, int right, int bottom, PIMAGE pimg = NULL);

/**
 * @brief 绘制实心矩形
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidrect    (int left, int top, int right, int bottom, PIMAGE pimg = NULL);

/**
 * @brief 绘制圆角矩形边框
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param radius 圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI roundrect     (int left, int top, int right, int bottom, int radius,  PIMAGE pimg = NULL);

/**
 * @brief 绘制填充圆角矩形
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param radius 圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillroundrect (int left, int top, int right, int bottom, int radius,  PIMAGE pimg = NULL);

/**
 * @brief 绘制实心圆角矩形
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param radius 圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidroundrect(int left, int top, int right, int bottom, int radius,  PIMAGE pimg = NULL);

/**
 * @brief 绘制圆角矩形边框（不同圆角半径）
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param xRadius x轴圆角半径
 * @param yRadius y轴圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI roundrect     (int left, int top, int right, int bottom, int xRadius, int yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充圆角矩形（不同圆角半径）
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param xRadius x轴圆角半径
 * @param yRadius y轴圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillroundrect (int left, int top, int right, int bottom, int xRadius, int yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制实心圆角矩形（不同圆角半径）
 * @param left 左边界
 * @param top 上边界
 * @param right 右边界
 * @param bottom 下边界
 * @param xRadius x轴圆角半径
 * @param yRadius y轴圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidroundrect(int left, int top, int right, int bottom, int xRadius, int yRadius, PIMAGE pimg = NULL);

/**
 * @brief 绘制多边形
 * @param numOfPoints 点的数量
 * @param points 点坐标数组，格式为[x1,y1,x2,y2,...]
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI drawpoly      (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief 绘制多条线段
 * @param numOfPoints 点的数量
 * @param points 点坐标数组，格式为[x1,y1,x2,y2,...]
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI polyline      (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief 绘制多边形边框
 * @param numOfPoints 点的数量
 * @param points 点坐标数组，格式为[x1,y1,x2,y2,...]
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI polygon       (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充多边形
 * @param numOfPoints 点的数量
 * @param points 点坐标数组，格式为[x1,y1,x2,y2,...]
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillpoly      (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief 绘制实心多边形
 * @param numOfPoints 点的数量
 * @param points 点坐标数组，格式为[x1,y1,x2,y2,...]
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI solidpoly     (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief 绘制渐变填充多边形
 * @param numOfPoints 点的数量
 * @param points 带颜色的点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI fillpoly_gradient(int numOfPoints, const ege_colpoint* points, PIMAGE pimg = NULL);

/**
 * @brief 绘制多条线段
 * @param numOfLines 线段数量
 * @param points 线段端点坐标数组，每两个点构成一条线段
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI drawlines     (int numOfLines,  const int *points, PIMAGE pimg = NULL);

/**
 * @brief 绘制贝塞尔曲线
 * @param numOfPoints 控制点数量
 * @param points 控制点坐标数组，格式为[x1,y1,x2,y2,...]
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI drawbezier    (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief 漫水填充（根据边界颜色）
 * @param x 填充起始点x坐标
 * @param y 填充起始点y坐标
 * @param borderColor 边界颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI floodfill     (int x, int y, int borderColor, PIMAGE pimg = NULL);

/**
 * @brief 区域填充（根据区域颜色）
 * @param x 填充起始点x坐标
 * @param y 填充起始点y坐标
 * @param areaColor 要替换的区域颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI floodfillsurface (int x, int y, color_t areaColor, PIMAGE pimg = NULL);

#ifdef EGE_GDIPLUS
/// @defgroup EGEGDIPlus EGE GDI+增强函数
/// 需要定义EGE_GDIPLUS宏才能使用的增强绘图功能
/// @{

/**
 * @brief 启用或禁用抗锯齿
 * @param enable true启用抗锯齿，false禁用
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_enable_aa(bool enable, PIMAGE pimg = NULL);

/**
 * @brief 设置全局Alpha透明度
 * @param alpha Alpha值（0-255，0完全透明，255完全不透明）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_setalpha(int alpha, PIMAGE pimg = NULL);

/**
 * @brief 绘制直线（GDI+增强版本）
 * @param x1 起点x坐标
 * @param y1 起点y坐标
 * @param x2 终点x坐标
 * @param y2 终点y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_line(float x1, float y1, float x2, float y2, PIMAGE pimg = NULL);

/**
 * @brief 绘制多边形（GDI+增强版本）
 * @param numOfPoints 点的数量
 * @param points 点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawpoly       (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief 绘制多条线段（GDI+增强版本）
 * @param numOfPoints 点的数量
 * @param points 点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_polyline       (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief 绘制多边形边框（GDI+增强版本）
 * @param numOfPoints 点的数量
 * @param points 点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_polygon        (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充多边形（GDI+增强版本）
 * @param numOfPoints 点的数量
 * @param points 点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillpoly       (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief 绘制贝塞尔曲线（GDI+增强版本）
 * @param numOfPoints 控制点数量
 * @param points 控制点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_bezier         (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief 绘制贝塞尔曲线（同ege_bezier）
 * @param numOfPoints 控制点数量
 * @param points 控制点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawbezier     (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);  // Same as ege_bezier

/**
 * @brief 绘制基数样条曲线
 * @param numOfPoints 控制点数量
 * @param points 控制点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawcurve      (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief 绘制闭合基数样条曲线
 * @param numOfPoints 控制点数量
 * @param points 控制点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawclosedcurve(int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充闭合基数样条曲线
 * @param numOfPoints 控制点数量
 * @param points 控制点坐标数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillclosedcurve(int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief 绘制基数样条曲线（带张力参数）
 * @param numOfPoints 控制点数量
 * @param points 控制点坐标数组
 * @param tension 张力参数（0.0为直线，值越大曲线越弯曲）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawcurve      (int numOfPoints, const ege_point* points, float tension, PIMAGE pimg = NULL);

/**
 * @brief 绘制闭合基数样条曲线（带张力参数）
 * @param numOfPoints 控制点数量
 * @param points 控制点坐标数组
 * @param tension 张力参数（0.0为直线，值越大曲线越弯曲）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawclosedcurve(int numOfPoints, const ege_point* points, float tension, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充闭合基数样条曲线（带张力参数）
 * @param numOfPoints 控制点数量
 * @param points 控制点坐标数组
 * @param tension 张力参数（0.0为直线，值越大曲线越弯曲）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillclosedcurve(int numOfPoints, const ege_point* points, float tension, PIMAGE pimg = NULL);

/**
 * @brief 绘制矩形边框（GDI+增强版本）
 * @param x 矩形左上角x坐标
 * @param y 矩形左上角y坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_rectangle    (float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充矩形（GDI+增强版本）
 * @param x 矩形左上角x坐标
 * @param y 矩形左上角y坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillrect     (float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief 绘制圆形边框（GDI+增强版本）
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_circle       (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充圆形（GDI+增强版本）
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param radius 半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillcircle   (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制椭圆边框（GDI+增强版本）
 * @param x 椭圆边界矩形左上角x坐标
 * @param y 椭圆边界矩形左上角y坐标
 * @param w 椭圆边界矩形宽度
 * @param h 椭圆边界矩形高度
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_ellipse      (float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充椭圆（GDI+增强版本）
 * @param x 椭圆边界矩形左上角x坐标
 * @param y 椭圆边界矩形左上角y坐标
 * @param w 椭圆边界矩形宽度
 * @param h 椭圆边界矩形高度
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillellipse  (float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief 绘制椭圆弧（GDI+增强版本）
 * @param x 椭圆边界矩形左上角x坐标
 * @param y 椭圆边界矩形左上角y坐标
 * @param w 椭圆边界矩形宽度
 * @param h 椭圆边界矩形高度
 * @param startAngle 起始角度（度）
 * @param sweepAngle 扫描角度（度）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_arc          (float x, float y, float w, float h, float startAngle, float sweepAngle, PIMAGE pimg = NULL);

/**
 * @brief 绘制饼图边框（GDI+增强版本）
 * @param x 椭圆边界矩形左上角x坐标
 * @param y 椭圆边界矩形左上角y坐标
 * @param w 椭圆边界矩形宽度
 * @param h 椭圆边界矩形高度
 * @param startAngle 起始角度（度）
 * @param sweepAngle 扫描角度（度）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_pie          (float x, float y, float w, float h, float startAngle, float sweepAngle, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充饼图（GDI+增强版本）
 * @param x 椭圆边界矩形左上角x坐标
 * @param y 椭圆边界矩形左上角y坐标
 * @param w 椭圆边界矩形宽度
 * @param h 椭圆边界矩形高度
 * @param startAngle 起始角度（度）
 * @param sweepAngle 扫描角度（度）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillpie      (float x, float y, float w, float h, float startAngle, float sweepAngle, PIMAGE pimg = NULL);

/**
 * @brief 绘制圆角矩形边框（GDI+增强版本）
 * @param x 矩形左上角x坐标
 * @param y 矩形左上角y坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param radius 圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_roundrect    (float x, float y, float w, float h,  float radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充圆角矩形（GDI+增强版本）
 * @param x 矩形左上角x坐标
 * @param y 矩形左上角y坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param radius 圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillroundrect(float x, float y, float w, float h,  float radius, PIMAGE pimg = NULL);

/**
 * @brief 绘制圆角矩形边框（各角不同半径）
 * @param x 矩形左上角x坐标
 * @param y 矩形左上角y坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param radius1 左上角圆角半径
 * @param radius2 右上角圆角半径
 * @param radius3 右下角圆角半径
 * @param radius4 左下角圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_roundrect    (float x, float y, float w, float h,  float radius1, float radius2, float radius3, float radius4, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充圆角矩形（各角不同半径）
 * @param x 矩形左上角x坐标
 * @param y 矩形左上角y坐标
 * @param w 矩形宽度
 * @param h 矩形高度
 * @param radius1 左上角圆角半径
 * @param radius2 右上角圆角半径
 * @param radius3 右下角圆角半径
 * @param radius4 左下角圆角半径
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillroundrect(float x, float y, float w, float h,  float radius1, float radius2, float radius3, float radius4, PIMAGE pimg = NULL);

/**
 * @brief 清除填充图案（设为无图案）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_setpattern_none(PIMAGE pimg = NULL);

/**
 * @brief 设置线性渐变填充图案
 * @param x1 起始点x坐标
 * @param y1 起始点y坐标
 * @param c1 起始点颜色
 * @param x2 结束点x坐标
 * @param y2 结束点y坐标
 * @param c2 结束点颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_setpattern_lineargradient(float x1, float y1, color_t c1, float x2, float y2, color_t c2, PIMAGE pimg = NULL);

/**
 * @brief 设置路径渐变填充图案
 * @param center 中心点
 * @param centerColor 中心颜色
 * @param count 边界点数量
 * @param points 边界点数组
 * @param colorCount 边界颜色数量
 * @param pointColors 边界颜色数组
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_setpattern_pathgradient(ege_point center, color_t centerColor, int count, const ege_point* points, int colorCount, const color_t* pointColors, PIMAGE pimg = NULL);

/**
 * @brief 设置椭圆渐变填充图案
 * @param center 中心点
 * @param centerColor 中心颜色
 * @param x 椭圆左上角x坐标
 * @param y 椭圆左上角y坐标
 * @param w 椭圆宽度
 * @param h 椭圆高度
 * @param color 边界颜色
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_setpattern_ellipsegradient(ege_point center, color_t centerColor, float x, float y, float w, float h, color_t color, PIMAGE pimg = NULL);

/**
 * @brief 设置纹理填充图案
 * @param imgSrc 源纹理图像
 * @param x 纹理区域左上角x坐标
 * @param y 纹理区域左上角y坐标
 * @param w 纹理区域宽度
 * @param h 纹理区域高度
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_setpattern_texture(PIMAGE imgSrc, float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief 绘制文本（GDI+增强版本）
 * @param text 要绘制的文本
 * @param x 文本左上角x坐标
 * @param y 文本左上角y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawtext(const char*    text, float x, float y, PIMAGE pimg = NULL);

/**
 * @brief 绘制文本（GDI+增强版本，Unicode）
 * @param text 要绘制的文本
 * @param x 文本左上角x坐标
 * @param y 文本左上角y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawtext(const wchar_t* text, float x, float y, PIMAGE pimg = NULL);

/**
 * @brief 生成纹理
 * @param generate 是否生成纹理
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_gentexture(bool generate, PIMAGE pimg = NULL);

/**
 * @brief 绘制纹理图像
 * @param imgSrc 源纹理图像
 * @param x 目标区域左上角x坐标
 * @param y 目标区域左上角y坐标
 * @param w 目标区域宽度
 * @param h 目标区域高度
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_puttexture(PCIMAGE imgSrc, float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief 绘制纹理图像（指定目标矩形）
 * @param imgSrc 源纹理图像
 * @param dest 目标矩形
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_puttexture(PCIMAGE imgSrc, ege_rect dest, PIMAGE pimg = NULL);

/**
 * @brief 绘制纹理图像（指定源和目标矩形）
 * @param imgSrc 源纹理图像
 * @param dest 目标矩形
 * @param src 源矩形
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_puttexture(PCIMAGE imgSrc, ege_rect dest, ege_rect src, PIMAGE pimg = NULL);

//draw image
/**
 * @brief 绘制图像（GDI+增强版本）
 * @param imgSrc 源图像
 * @param xDest 目标x坐标
 * @param yDest 目标y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawimage(PCIMAGE imgSrc,int xDest, int yDest, PIMAGE pimg = NULL);

/**
 * @brief 绘制图像（GDI+增强版本，指定源和目标区域）
 * @param imgSrc 源图像
 * @param xDest 目标区域左上角x坐标
 * @param yDest 目标区域左上角y坐标
 * @param widthDest 目标区域宽度
 * @param heightDest 目标区域高度
 * @param xSrc 源区域左上角x坐标
 * @param ySrc 源区域左上角y坐标
 * @param widthSrc 源区域宽度
 * @param heightSrc 源区域高度
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawimage(PCIMAGE imgSrc,int xDest, int yDest, int widthDest, int heightDest, int xSrc, int ySrc, int widthSrc, int heightSrc,PIMAGE pimg = NULL);

/**
 * @brief 绘制路径边框
 * @param path 路径对象指针
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawpath(const ege_path* path, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充路径
 * @param path 路径对象指针
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillpath(const ege_path* path, PIMAGE pimg = NULL);

/**
 * @brief 绘制路径边框（指定偏移）
 * @param path 路径对象指针
 * @param x x方向偏移
 * @param y y方向偏移
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_drawpath(const ege_path* path, float x, float y, PIMAGE pimg = NULL);

/**
 * @brief 绘制填充路径（指定偏移）
 * @param path 路径对象指针
 * @param x x方向偏移
 * @param y y方向偏移
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_fillpath(const ege_path* path, float x, float y, PIMAGE pimg = NULL);

/// @defgroup PathManagement 路径管理函数
/// @{

/**
 * @brief 创建新的路径对象
 * @return 路径对象指针
 */
ege_path* EGEAPI ege_path_create     ();

/**
 * @brief 从点数组和类型数组创建路径
 * @param points 点坐标数组
 * @param types 点类型数组
 * @param count 点的数量
 * @return 路径对象指针
 */
ege_path* EGEAPI ege_path_createfrom (const ege_point* points, const unsigned char* types, int count);

/**
 * @brief 克隆路径
 * @param path 源路径对象指针
 * @return 新的路径对象指针
 */
ege_path* EGEAPI ege_path_clone      (const ege_path* path);

/**
 * @brief 销毁路径对象
 * @param path 路径对象指针
 */
void      EGEAPI ege_path_destroy    (const ege_path* path);

/**
 * @brief 开始新的子路径
 * @param path 路径对象指针
 */
void      EGEAPI ege_path_start      (ege_path* path);

/**
 * @brief 关闭当前子路径
 * @param path 路径对象指针
 */
void      EGEAPI ege_path_close      (ege_path* path);

/**
 * @brief 关闭所有打开的子路径
 * @param path 路径对象指针
 */
void      EGEAPI ege_path_closeall   (ege_path* path);

/**
 * @brief 设置路径填充模式
 * @param path 路径对象指针
 * @param mode 填充模式
 */
void      EGEAPI ege_path_setfillmode(ege_path* path, fill_mode mode);

/**
 * @brief 重置路径（清空所有子路径）
 * @param path 路径对象指针
 */
void      EGEAPI ege_path_reset      (ege_path* path);

/**
 * @brief 反转路径方向
 * @param path 路径对象指针
 */
void      EGEAPI ege_path_reverse    (ege_path* path);

/**
 * @brief 扩展路径（生成轮廓）
 * @param path 路径对象指针
 * @param lineWidth 线条宽度
 * @param matrix 变换矩阵（可选）
 */
void      EGEAPI ege_path_widen      (ege_path* path, float lineWidth, const ege_transform_matrix* matrix = NULL);

/**
 * @brief 扩展路径（生成轮廓，指定平坦度）
 * @param path 路径对象指针
 * @param lineWidth 线条宽度
 * @param matrix 变换矩阵
 * @param flatness 平坦度
 */
void      EGEAPI ege_path_widen      (ege_path* path, float lineWidth, const ege_transform_matrix* matrix,  float flatness);

/**
 * @brief 平坦化路径（将曲线转换为直线段）
 * @param path 路径对象指针
 * @param matrix 变换矩阵（可选）
 */
void      EGEAPI ege_path_flatten    (ege_path* path, const ege_transform_matrix* matrix = NULL);

/**
 * @brief 平坦化路径（指定平坦度）
 * @param path 路径对象指针
 * @param matrix 变换矩阵
 * @param flatness 平坦度
 */
void      EGEAPI ege_path_flatten    (ege_path* path, const ege_transform_matrix* matrix, float flatness);

/**
 * @brief 扭曲路径
 * @param path 路径对象指针
 * @param points 扭曲控制点数组
 * @param count 控制点数量
 * @param rect 扭曲矩形区域
 * @param matrix 变换矩阵（可选）
 */
void      EGEAPI ege_path_warp       (ege_path* path, const ege_point* points, int count, const ege_rect* rect, const ege_transform_matrix* matrix = NULL);

/**
 * @brief 扭曲路径（指定平坦度）
 * @param path 路径对象指针
 * @param points 扭曲控制点数组
 * @param count 控制点数量
 * @param rect 扭曲矩形区域
 * @param matrix 变换矩阵
 * @param flatness 平坦度
 */
void      EGEAPI ege_path_warp       (ege_path* path, const ege_point* points, int count, const ege_rect* rect, const ege_transform_matrix* matrix, float flatness);

/**
 * @brief 生成路径轮廓
 * @param path 路径对象指针
 * @param matrix 变换矩阵（可选）
 */
void      EGEAPI ege_path_outline    (ege_path* path, const ege_transform_matrix* matrix = NULL);

/**
 * @brief 生成路径轮廓（指定平坦度）
 * @param path 路径对象指针
 * @param matrix 变换矩阵
 * @param flatness 平坦度
 */
void      EGEAPI ege_path_outline    (ege_path* path, const ege_transform_matrix* matrix, float flatness);

/**
 * @brief 检测点是否在路径内部
 * @param path 路径对象指针
 * @param x 检测点x坐标
 * @param y 检测点y坐标
 * @return 如果点在路径内部返回true，否则返回false
 */
bool      EGEAPI ege_path_inpath     (const ege_path* path, float x, float y);

/**
 * @brief 检测点是否在路径内部（指定图像上下文）
 * @param path 路径对象指针
 * @param x 检测点x坐标
 * @param y 检测点y坐标
 * @param pimg 图像上下文指针
 * @return 如果点在路径内部返回true，否则返回false
 */
bool      EGEAPI ege_path_inpath     (const ege_path* path, float x, float y, PCIMAGE pimg);

/**
 * @brief 检测点是否在路径边框上
 * @param path 路径对象指针
 * @param x 检测点x坐标
 * @param y 检测点y坐标
 * @return 如果点在路径边框上返回true，否则返回false
 */
bool      EGEAPI ege_path_instroke   (const ege_path* path, float x, float y);

/**
 * @brief 检测点是否在路径边框上（指定图像上下文）
 * @param path 路径对象指针
 * @param x 检测点x坐标
 * @param y 检测点y坐标
 * @param pimg 图像上下文指针
 * @return 如果点在路径边框上返回true，否则返回false
 */
bool      EGEAPI ege_path_instroke   (const ege_path* path, float x, float y, PCIMAGE pimg);

/**
 * @brief 获取路径最后一个点的坐标
 * @param path 路径对象指针
 * @return 最后一个点的坐标
 */
ege_point      EGEAPI ege_path_lastpoint    (const ege_path* path);

/**
 * @brief 获取路径中点的数量
 * @param path 路径对象指针
 * @return 路径中点的数量
 */
int            EGEAPI ege_path_pointcount   (const ege_path* path);

/**
 * @brief 获取路径边界矩形
 * @param path 路径对象指针
 * @param matrix 变换矩阵，NULL表示不进行变换
 * @return 路径的边界矩形
 */
ege_rect       EGEAPI ege_path_getbounds    (const ege_path* path, const ege_transform_matrix* matrix = NULL);

/**
 * @brief 获取路径边界矩形（指定图像上下文）
 * @param path 路径对象指针
 * @param matrix 变换矩阵
 * @param pimg 图像上下文指针
 * @return 路径的边界矩形
 */
ege_rect       EGEAPI ege_path_getbounds    (const ege_path* path, const ege_transform_matrix* matrix, PCIMAGE pimg);

/**
 * @brief 获取路径中的所有点
 * @param path 路径对象指针
 * @param points 点数组缓冲区，NULL表示自动分配
 * @return 指向点数组的指针
 * @note 如果points为NULL，函数会分配内存，调用者需要负责释放
 */
ege_point*     EGEAPI ege_path_getpathpoints(const ege_path* path, ege_point* points = NULL);

/**
 * @brief 获取路径中的所有点类型
 * @param path 路径对象指针
 * @param types 类型数组缓冲区，NULL表示自动分配
 * @return 指向类型数组的指针
 * @note 如果types为NULL，函数会分配内存，调用者需要负责释放
 */
unsigned char* EGEAPI ege_path_getpathtypes (const ege_path* path, unsigned char* types = NULL);

/**
 * @brief 变换路径
 * @param path 路径对象指针
 * @param matrix 变换矩阵
 */
void EGEAPI ege_path_transform     (ege_path* path, const ege_transform_matrix* matrix);

/// @defgroup PathAdd 路径添加函数
/// @{

/**
 * @brief 向路径添加另一个路径
 * @param dstPath 目标路径对象指针
 * @param srcPath 源路径对象指针
 * @param connect 是否连接到当前路径的最后一点
 */
void EGEAPI ege_path_addpath       (ege_path* dstPath, const ege_path* srcPath, bool connect);

/**
 * @brief 向路径添加直线段
 * @param path 路径对象指针
 * @param x1 起点x坐标
 * @param y1 起点y坐标
 * @param x2 终点x坐标
 * @param y2 终点y坐标
 */
void EGEAPI ege_path_addline       (ege_path* path, float x1, float y1, float x2, float y2);

/**
 * @brief 向路径添加弧形
 * @param path 路径对象指针
 * @param x 椭圆左上角x坐标
 * @param y 椭圆左上角y坐标
 * @param width 椭圆宽度
 * @param height 椭圆高度
 * @param startAngle 起始角度（度）
 * @param sweepAngle 扫描角度（度）
 */
void EGEAPI ege_path_addarc        (ege_path* path, float x, float y, float width, float height, float startAngle, float sweepAngle);

/**
 * @brief 向路径添加折线
 * @param path 路径对象指针
 * @param numOfPoints 点的数量
 * @param points 点数组
 */
void EGEAPI ege_path_addpolyline   (ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief 向路径添加贝塞尔曲线（点数组版本）
 * @param path 路径对象指针
 * @param numOfPoints 控制点的数量
 * @param points 控制点数组
 */
void EGEAPI ege_path_addbezier     (ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief 向路径添加贝塞尔曲线（坐标版本）
 * @param path 路径对象指针
 * @param x1 起点x坐标
 * @param y1 起点y坐标
 * @param x2 第一个控制点x坐标
 * @param y2 第一个控制点y坐标
 * @param x3 第二个控制点x坐标
 * @param y3 第二个控制点y坐标
 * @param x4 终点x坐标
 * @param y4 终点y坐标
 */
void EGEAPI ege_path_addbezier     (ege_path* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

/**
 * @brief 向路径添加基数样条曲线
 * @param path 路径对象指针
 * @param numOfPoints 控制点的数量
 * @param points 控制点数组
 */
void EGEAPI ege_path_addcurve      (ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief 向路径添加基数样条曲线（指定张力）
 * @param path 路径对象指针
 * @param numOfPoints 控制点的数量
 * @param points 控制点数组
 * @param tension 张力值（0.0-1.0）
 */
void EGEAPI ege_path_addcurve      (ege_path* path, int numOfPoints, const ege_point* points, float tension);

/**
 * @brief 向路径添加圆形
 * @param path 路径对象指针
 * @param x 圆心x坐标
 * @param y 圆心y坐标
 * @param radius 半径
 */
void EGEAPI ege_path_addcircle     (ege_path* path, float x, float y, float radius);

/**
 * @brief 向路径添加矩形
 * @param path 路径对象指针
 * @param x 矩形左上角x坐标
 * @param y 矩形左上角y坐标
 * @param width 矩形宽度
 * @param height 矩形高度
 */
void EGEAPI ege_path_addrect       (ege_path* path, float x, float y, float width, float height);

/**
 * @brief 向路径添加椭圆
 * @param path 路径对象指针
 * @param x 椭圆边界左上角x坐标
 * @param y 椭圆边界左上角y坐标
 * @param width 椭圆宽度
 * @param height 椭圆高度
 */
void EGEAPI ege_path_addellipse    (ege_path* path, float x, float y, float width, float height);

/**
 * @brief 向路径添加饼图
 * @param path 路径对象指针
 * @param x 椭圆边界左上角x坐标
 * @param y 椭圆边界左上角y坐标
 * @param width 椭圆宽度
 * @param height 椭圆高度
 * @param startAngle 起始角度（度）
 * @param sweepAngle 扫描角度（度）
 */
void EGEAPI ege_path_addpie        (ege_path* path, float x, float y, float width, float height, float startAngle, float sweepAngle);

/**
 * @brief 向路径添加文本（ANSI版本）
 * @param path 路径对象指针
 * @param x 文本起始x坐标
 * @param y 文本起始y坐标
 * @param text 文本内容
 * @param height 文本高度
 * @param length 文本长度，-1表示自动计算
 * @param typeface 字体名称，NULL表示使用默认字体
 * @param fontStyle 字体样式
 */
void EGEAPI ege_path_addtext       (ege_path* path, float x, float y, const char*    text, float height, int length = -1, const char*    typeface = NULL, int fontStyle = 0);

/**
 * @brief 向路径添加文本（Unicode版本）
 * @param path 路径对象指针
 * @param x 文本起始x坐标
 * @param y 文本起始y坐标
 * @param text 文本内容
 * @param height 文本高度
 * @param length 文本长度，-1表示自动计算
 * @param typeface 字体名称，NULL表示使用默认字体
 * @param fontStyle 字体样式
 */
void EGEAPI ege_path_addtext       (ege_path* path, float x, float y, const wchar_t* text, float height, int length = -1, const wchar_t* typeface = NULL, int fontStyle = 0);

/**
 * @brief 向路径添加多边形
 * @param path 路径对象指针
 * @param numOfPoints 顶点数量
 * @param points 顶点数组
 */
void EGEAPI ege_path_addpolygon    (ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief 向路径添加闭合基数样条曲线
 * @param path 路径对象指针
 * @param numOfPoints 控制点的数量
 * @param points 控制点数组
 */
void EGEAPI ege_path_addclosedcurve(ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief 向路径添加闭合基数样条曲线（指定张力）
 * @param path 路径对象指针
 * @param numOfPoints 控制点的数量
 * @param points 控制点数组
 * @param tension 张力值（0.0-1.0）
 */
void EGEAPI ege_path_addclosedcurve(ege_path* path, int numOfPoints, const ege_point* points, float tension);

/// @}


/// @defgroup Transform 变换函数
/// @{

/**
 * @brief 旋转变换
 * @param angle 旋转角度（度）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_transform_rotate(float angle, PIMAGE pimg = NULL);

/**
 * @brief 平移变换
 * @param x x方向平移量
 * @param y y方向平移量
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_transform_translate(float x, float y, PIMAGE pimg = NULL);

/**
 * @brief 缩放变换
 * @param xScale x方向缩放比例
 * @param yScale y方向缩放比例
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_transform_scale(float xScale, float yScale, PIMAGE pimg = NULL);

/**
 * @brief 重置变换矩阵为单位矩阵
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_transform_reset(PIMAGE pimg = NULL);

/**
 * @brief 获取当前变换矩阵
 * @param matrix 输出变换矩阵的指针
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_get_transform(ege_transform_matrix* matrix, PIMAGE pimg = NULL);

/**
 * @brief 设置变换矩阵
 * @param matrix 要设置的变换矩阵
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI ege_set_transform(const ege_transform_matrix* matrix, PIMAGE pimg = NULL);

/**
 * @brief 计算点经过变换后的坐标
 * @param p 原始点坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 变换后的点坐标
 */
ege_point EGEAPI ege_transform_calc(ege_point p, PIMAGE pimg = NULL);

/// @}

/**
 * @brief 计算点经过变换后的坐标（坐标版本）
 * @param x 原始点x坐标
 * @param y 原始点y坐标
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 变换后的点坐标
 */
ege_point EGEAPI ege_transform_calc(float x, float y, PIMAGE pimg = NULL);


#endif

// It is not supported in VC 6.0.
#ifndef EGE_COMPILERINFO_VC6

/// @defgroup Console 控制台函数
/// @{

/**
 * @brief 初始化控制台
 * @return 成功返回true，失败返回false
 * @note 创建一个控制台窗口，用于标准输入输出
 */
bool EGEAPI init_console();

/**
 * @brief 清空控制台
 * @return 成功返回true，失败返回false
 */
bool EGEAPI clear_console();

/**
 * @brief 显示控制台窗口
 * @return 成功返回true，失败返回false
 */
bool EGEAPI show_console();

/**
 * @brief 隐藏控制台窗口
 * @return 成功返回true，失败返回false
 */
bool EGEAPI hide_console();

/**
 * @brief 关闭控制台并恢复标准输入输出
 * @return 成功返回true，失败返回false
 */
bool EGEAPI close_console();
#endif

/**
 * @brief 从控制台获取字符（替代<conio.h>中的getch函数）
 * @return 获取到的字符码
 */
int  EGEAPI getch_console();

/**
 * @brief 检测控制台是否有按键（替代<conio.h>中的kbhit函数）
 * @return 有按键返回非零值，无按键返回0
 */
int  EGEAPI kbhit_console();

/// @}

/// @defgroup Time 时间相关函数
/// @{

/**
 * @brief EGE库延时函数
 * @param ms 延时时间（毫秒）
 * @note 精确的延时函数，不会被系统调度影响
 */
void EGEAPI ege_sleep (long ms);

/**
 * @brief 延时函数
 * @param ms 延时时间（毫秒）
 * @note 标准延时函数
 */
void EGEAPI delay     (long ms);

/**
 * @brief 毫秒延时函数
 * @param ms 延时时间（毫秒）
 */
void EGEAPI delay_ms  (long ms);

/**
 * @brief API延时函数
 * @param ms 延时时间（毫秒）
 * @note 使用系统API实现的延时
 */
void EGEAPI api_sleep (long ms);

/**
 * @brief 按帧率延时（整数版本）
 * @param fps 目标帧率
 * @note 根据帧率自动计算延时时间
 */
void EGEAPI delay_fps (int    fps);

/**
 * @brief 按帧率延时（长整数版本）
 * @param fps 目标帧率
 */
void EGEAPI delay_fps (long   fps);

/**
 * @brief 按帧率延时（双精度浮点版本）
 * @param fps 目标帧率
 */
void EGEAPI delay_fps (double fps);

/**
 * @brief 带跳帧的按帧率延时
 * @param fps 目标帧率
 * @note 更精确的帧率控制
 */
void EGEAPI delay_jfps(int    fps);

/**
 * @brief 精确按帧率延时（长整数版本）
 * @param fps 目标帧率
 */
void EGEAPI delay_jfps(long   fps);

/**
 * @brief 精确按帧率延时（双精度浮点版本）
 * @param fps 目标帧率
 */
void EGEAPI delay_jfps(double fps);

/**
 * @brief 获取高精度时钟
 * @return 当前时间（秒，双精度浮点）
 * @note 用于高精度计时和性能测量
 */
double EGEAPI fclock();

/// @}

/**
 * @defgroup TextOutput 文本输出
 * @brief 文本输出和显示相关函数
 * @{
 */

/**
 * @brief 在当前位置输出文本字符串
 * @param text 要输出的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 文本输出位置由当前绘图位置决定，使用 moveto() 设置
 */
void EGEAPI outtext(const char*    text, PIMAGE pimg = NULL);

/**
 * @brief 在当前位置输出文本字符串（Unicode版本）
 * @param text 要输出的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 文本输出位置由当前绘图位置决定，使用 moveto() 设置
 */
void EGEAPI outtext(const wchar_t* text, PIMAGE pimg = NULL);

/**
 * @brief 在当前位置输出单个字符
 * @param c 要输出的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 文本输出位置由当前绘图位置决定，使用 moveto() 设置
 */
void EGEAPI outtext(char    c, PIMAGE pimg = NULL);

/**
 * @brief 在当前位置输出单个字符（Unicode版本）
 * @param c 要输出的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 文本输出位置由当前绘图位置决定，使用 moveto() 设置
 */
void EGEAPI outtext(wchar_t c, PIMAGE pimg = NULL);

/**
 * @brief 在指定位置输出文本字符串
 * @param x 输出位置的x坐标
 * @param y 输出位置的y坐标
 * @param text 要输出的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI outtextxy(int x, int y, const char*    text, PIMAGE pimg = NULL);

/**
 * @brief 在指定位置输出文本字符串（Unicode版本）
 * @param x 输出位置的x坐标
 * @param y 输出位置的y坐标
 * @param text 要输出的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI outtextxy(int x, int y, const wchar_t* text, PIMAGE pimg = NULL);

/**
 * @brief 在指定位置输出单个字符
 * @param x 输出位置的x坐标
 * @param y 输出位置的y坐标
 * @param c 要输出的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI outtextxy(int x, int y, char    c, PIMAGE pimg = NULL);

/**
 * @brief 在指定位置输出单个字符（Unicode版本）
 * @param x 输出位置的x坐标
 * @param y 输出位置的y坐标
 * @param c 要输出的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI outtextxy(int x, int y, wchar_t c, PIMAGE pimg = NULL);

/**
 * @brief 在指定位置格式化输出文本
 * @param x 输出位置的x坐标
 * @param y 输出位置的y坐标
 * @param format 格式化字符串（类似printf）
 * @param ... 可变参数列表
 * @note 实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI xyprintf (int x, int y, const char*    format, ...);

/**
 * @brief 在指定位置格式化输出文本（Unicode版本）
 * @param x 输出位置的x坐标
 * @param y 输出位置的y坐标
 * @param format 格式化字符串（类似printf）
 * @param ... 可变参数列表
 * @note 实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI xyprintf (int x, int y, const wchar_t* format, ...);

/**
 * @brief 在指定矩形区域内输出文本
 * @param x 矩形区域左上角x坐标
 * @param y 矩形区域左上角y坐标
 * @param w 矩形区域宽度
 * @param h 矩形区域高度
 * @param text 要输出的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 文本会在指定矩形范围内自动换行，支持文本对齐设置
 */
void EGEAPI outtextrect(int x, int y, int w, int h, const char*    text, PIMAGE pimg = NULL);

/**
 * @brief 在指定矩形区域内输出文本（Unicode版本）
 * @param x 矩形区域左上角x坐标
 * @param y 矩形区域左上角y坐标
 * @param w 矩形区域宽度
 * @param h 矩形区域高度
 * @param text 要输出的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 文本会在指定矩形范围内自动换行，支持文本对齐设置
 */
void EGEAPI outtextrect(int x, int y, int w, int h, const wchar_t* text, PIMAGE pimg = NULL);

/**
 * @brief 在指定矩形区域内格式化输出文本
 * @param x 矩形区域左上角x坐标
 * @param y 矩形区域左上角y坐标
 * @param w 矩形区域宽度
 * @param h 矩形区域高度
 * @param format 格式化字符串（类似printf）
 * @param ... 可变参数列表
 * @note 文本会在指定矩形范围内自动换行，支持文本对齐设置
 */
void EGEAPI rectprintf (int x, int y, int w, int h, const char*    format, ...);

/**
 * @brief 在指定矩形区域内格式化输出文本（Unicode版本）
 * @param x 矩形区域左上角x坐标
 * @param y 矩形区域左上角y坐标
 * @param w 矩形区域宽度
 * @param h 矩形区域高度
 * @param format 格式化字符串（类似printf）
 * @param ... 可变参数列表
 * @note 文本会在指定矩形范围内自动换行，支持文本对齐设置
 */
void EGEAPI rectprintf (int x, int y, int w, int h, const wchar_t* format, ...);

/**
 * @brief 获取文本字符串的显示宽度
 * @param text 要测量的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 文本显示宽度（像素）
 * @note 返回值受当前字体设置影响
 */
int  EGEAPI textwidth(const char*    text, PCIMAGE pimg = NULL);

/**
 * @brief 获取文本字符串的显示宽度（Unicode版本）
 * @param text 要测量的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 文本显示宽度（像素）
 * @note 返回值受当前字体设置影响
 */
int  EGEAPI textwidth(const wchar_t* text, PCIMAGE pimg = NULL);

/**
 * @brief 获取单个字符的显示宽度
 * @param c 要测量的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 字符显示宽度（像素）
 * @note 返回值受当前字体设置影响
 */
int  EGEAPI textwidth(char    c, PCIMAGE pimg = NULL);

/**
 * @brief 获取单个字符的显示宽度（Unicode版本）
 * @param c 要测量的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 字符显示宽度（像素）
 * @note 返回值受当前字体设置影响
 */
int  EGEAPI textwidth(wchar_t c, PCIMAGE pimg = NULL);

/**
 * @brief 获取文本字符串的显示高度
 * @param text 要测量的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 文本显示高度（像素）
 * @note 返回值受当前字体设置影响
 */
int  EGEAPI textheight(const char*    text, PCIMAGE pimg = NULL);

/**
 * @brief 获取文本字符串的显示高度（Unicode版本）
 * @param text 要测量的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 文本显示高度（像素）
 * @note 返回值受当前字体设置影响
 */
int  EGEAPI textheight(const wchar_t* text, PCIMAGE pimg = NULL);

/**
 * @brief 获取单个字符的显示高度
 * @param c 要测量的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 字符显示高度（像素）
 * @note 返回值受当前字体设置影响
 */
int  EGEAPI textheight(char    c, PCIMAGE pimg = NULL);

/**
 * @brief 获取单个字符的显示高度（Unicode版本）
 * @param c 要测量的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @return 字符显示高度（像素）
 * @note 返回值受当前字体设置影响
 */
int  EGEAPI textheight(wchar_t c, PCIMAGE pimg = NULL);

/**
 * @brief 在指定位置输出文本（支持浮点坐标和ARGB颜色）
 * @param x 输出位置的x坐标（浮点数）
 * @param y 输出位置的y坐标（浮点数）
 * @param text 要输出的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 使用GDI+渲染，支持ARGB颜色和抗锯齿，实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI ege_outtextxy(float x, float y, const char*    text, PIMAGE pimg = NULL);

/**
 * @brief 在指定位置输出文本（支持浮点坐标和ARGB颜色，Unicode版本）
 * @param x 输出位置的x坐标（浮点数）
 * @param y 输出位置的y坐标（浮点数）
 * @param text 要输出的文本字符串
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 使用GDI+渲染，支持ARGB颜色和抗锯齿，实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI ege_outtextxy(float x, float y, const wchar_t* text, PIMAGE pimg = NULL);

/**
 * @brief 在指定位置输出单个字符（支持浮点坐标和ARGB颜色）
 * @param x 输出位置的x坐标（浮点数）
 * @param y 输出位置的y坐标（浮点数）
 * @param c 要输出的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 使用GDI+渲染，支持ARGB颜色和抗锯齿，实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI ege_outtextxy(float x, float y, char    c, PIMAGE pimg = NULL);

/**
 * @brief 在指定位置输出单个字符（支持浮点坐标和ARGB颜色，Unicode版本）
 * @param x 输出位置的x坐标（浮点数）
 * @param y 输出位置的y坐标（浮点数）
 * @param c 要输出的字符
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 使用GDI+渲染，支持ARGB颜色和抗锯齿，实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI ege_outtextxy(float x, float y, wchar_t c, PIMAGE pimg = NULL);

/**
 * @brief 在指定位置格式化输出文本（支持浮点坐标和ARGB颜色）
 * @param x 输出位置的x坐标（浮点数）
 * @param y 输出位置的y坐标（浮点数）
 * @param format 格式化字符串（类似printf）
 * @param ... 可变参数列表
 * @note 使用GDI+渲染，支持ARGB颜色和抗锯齿，实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI ege_xyprintf (float x, float y, const char*    format, ...);

/**
 * @brief 在指定位置格式化输出文本（支持浮点坐标和ARGB颜色，Unicode版本）
 * @param x 输出位置的x坐标（浮点数）
 * @param y 输出位置的y坐标（浮点数）
 * @param format 格式化字符串（类似printf）
 * @param ... 可变参数列表
 * @note 使用GDI+渲染，支持ARGB颜色和抗锯齿，实际输出位置受文本对齐方式和字体倾斜角度影响
 */
void EGEAPI ege_xyprintf (float x, float y, const wchar_t* format, ...);

/// @}

/**
 * @defgroup FontSettings 字体设置
 * @brief 字体和文本对齐相关设置函数
 * @{
 */

/**
 * @brief 设置文本对齐方式
 * @param horiz 水平对齐方式（LEFT_TEXT、CENTER_TEXT、RIGHT_TEXT）
 * @param vert 垂直对齐方式（TOP_TEXT、CENTER_TEXT、BOTTOM_TEXT）
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @note 影响后续所有文本输出函数的对齐效果
 */
void EGEAPI settextjustify(int horiz, int vert, PIMAGE pimg = NULL);

/**
 * @brief 设置字体（简化版本）
 * @param height 字体高度（像素）
 * @param width 字体宽度（像素），0表示自动
 * @param typeface 字体名称
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setfont(int height, int width, const char* typeface,  PIMAGE pimg = NULL);

/**
 * @brief 设置字体（简化版本，Unicode）
 * @param height 字体高度（像素）
 * @param width 字体宽度（像素），0表示自动
 * @param typeface 字体名称
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setfont(int height, int width, const wchar_t* typeface, PIMAGE pimg = NULL);

/**
 * @brief 设置字体（完整版本）
 * @param height 字体高度（像素）
 * @param width 字体宽度（像素），0表示自动
 * @param typeface 字体名称
 * @param escapement 字体倾斜角度（十分之一度为单位）
 * @param orientation 字符倾斜角度（十分之一度为单位）
 * @param weight 字体粗细（100-900，400为正常，700为粗体）
 * @param italic 是否斜体
 * @param underline 是否下划线
 * @param strikeOut 是否删除线
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setfont(int height, int width, const char* typeface,  int escapement, int orientation,
                    int weight, bool italic, bool underline, bool strikeOut, PIMAGE pimg = NULL);

/**
 * @brief 设置字体（完整版本，Unicode）
 * @param height 字体高度（像素）
 * @param width 字体宽度（像素），0表示自动
 * @param typeface 字体名称
 * @param escapement 字体倾斜角度（十分之一度为单位）
 * @param orientation 字符倾斜角度（十分之一度为单位）
 * @param weight 字体粗细（100-900，400为正常，700为粗体）
 * @param italic 是否斜体
 * @param underline 是否下划线
 * @param strikeOut 是否删除线
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setfont(int height, int width, const wchar_t* typeface, int escapement, int orientation,
                    int weight, bool italic, bool underline, bool strikeOut, PIMAGE pimg = NULL);

/**
 * @brief 设置字体（高级版本）
 * @param height 字体高度（像素）
 * @param width 字体宽度（像素），0表示自动
 * @param typeface 字体名称
 * @param escapement 字体倾斜角度（十分之一度为单位）
 * @param orientation 字符倾斜角度（十分之一度为单位）
 * @param weight 字体粗细（100-900，400为正常，700为粗体）
 * @param italic 是否斜体
 * @param underline 是否下划线
 * @param strikeOut 是否删除线
 * @param charSet 字符集
 * @param outPrecision 输出精度
 * @param clipPrecision 裁剪精度
 * @param quality 输出质量
 * @param pitchAndFamily 字体间距和族信息
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setfont(int height, int width, const char* typeface,  int escapement, int orientation,
                    int weight, bool italic, bool underline, bool strikeOut, BYTE charSet,
                    BYTE outPrecision, BYTE clipPrecision, BYTE quality, BYTE pitchAndFamily, PIMAGE pimg = NULL);

/**
 * @brief 设置字体（高级版本，Unicode）
 * @param height 字体高度（像素）
 * @param width 字体宽度（像素），0表示自动
 * @param typeface 字体名称
 * @param escapement 字体倾斜角度（十分之一度为单位）
 * @param orientation 字符倾斜角度（十分之一度为单位）
 * @param weight 字体粗细（100-900，400为正常，700为粗体）
 * @param italic 是否斜体
 * @param underline 是否下划线
 * @param strikeOut 是否删除线
 * @param charSet 字符集
 * @param outPrecision 输出精度
 * @param clipPrecision 裁剪精度
 * @param quality 输出质量
 * @param pitchAndFamily 字体间距和族信息
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setfont(int height, int width, const wchar_t* typeface, int escapement, int orientation,
                    int weight, bool italic, bool underline, bool strikeOut, BYTE charSet,
                    BYTE outPrecision, BYTE clipPrecision, BYTE quality, BYTE pitchAndFamily, PIMAGE pimg = NULL);

/**
 * @brief 使用LOGFONTW结构设置字体
 * @param font 指向LOGFONTW结构的指针
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI setfont(const LOGFONTW *font, PIMAGE pimg = NULL);

/**
 * @brief 获取当前字体设置
 * @param font 用于接收字体信息的LOGFONTW结构指针
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 */
void EGEAPI getfont(LOGFONTW *font, PCIMAGE pimg = NULL);

/**
 * @brief 使用LOGFONTA结构设置字体
 * @param font 指向LOGFONTA结构的指针
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @deprecated 建议使用带LOGFONTW参数的setfont函数
 */
EGE_DEPRECATE(setfont, "Please use the 'getfont' function with the LOGFONTW* parameter instead.")
void EGEAPI setfont(const LOGFONTA *font, PIMAGE pimg = NULL);

/**
 * @brief 获取当前字体设置（ANSI版本）
 * @param font 用于接收字体信息的LOGFONTA结构指针
 * @param pimg 目标图像指针，NULL 表示当前ege窗口
 * @deprecated 建议使用带LOGFONTW参数的getfont函数
 */
EGE_DEPRECATE(getfont, "Please use the 'getfont' function with the LOGFONTW* parameter instead.")
void EGEAPI getfont(LOGFONTA *font, PCIMAGE pimg = NULL);

/// @}

#define getmaxx getwidth
#define getmaxy getheight

/**
 * @brief 获取窗口或图像的宽度
 * @param pimg 图像对象指针，NULL 表示获取当前绘图窗口的宽度
 * @return 返回窗口或图像的宽度（像素），如果图像对象无效则返回0
 * 
 * 这个函数用于获取指定图像或窗口的宽度。当pimg为NULL时，
 * 返回当前EGE绘图窗口的宽度；当pimg指向有效图像时，返回该图像的宽度。
 * 
 * @see getheight() 获取高度
 * @see getx() 获取当前x坐标
 * @see gety() 获取当前y坐标
 */
int EGEAPI getwidth(PCIMAGE pimg = NULL);

/**
 * @brief 获取窗口或图像的高度
 * @param pimg 图像对象指针，NULL 表示获取当前绘图窗口的高度
 * @return 返回窗口或图像的高度（像素），如果图像对象无效则返回0
 * 
 * 这个函数用于获取指定图像或窗口的高度。当pimg为NULL时，
 * 返回当前EGE绘图窗口的高度；当pimg指向有效图像时，返回该图像的高度。
 * 
 * @see getwidth() 获取宽度
 * @see getx() 获取当前x坐标
 * @see gety() 获取当前y坐标
 */
int EGEAPI getheight(PCIMAGE pimg = NULL);

/**
 * @brief 获取当前画笔位置的x坐标
 * @param pimg 图像对象指针，NULL 表示获取当前绘图窗口的画笔位置
 * @return 返回当前画笔位置的x坐标，如果图像对象无效则返回-1
 * 
 * 获取当前绘图位置的x坐标。画笔位置通常由moveto()、lineto()等函数设置，
 * 或者由绘图函数（如line()）的执行而改变。
 * 
 * @see gety() 获取当前y坐标
 * @see moveto() 移动画笔到指定位置
 * @see getwidth() 获取宽度
 * @see getheight() 获取高度
 */
int EGEAPI getx(PCIMAGE pimg = NULL);

/**
 * @brief 获取当前画笔位置的y坐标
 * @param pimg 图像对象指针，NULL 表示获取当前绘图窗口的画笔位置
 * @return 返回当前画笔位置的y坐标，如果图像对象无效则返回-1
 * 
 * 获取当前绘图位置的y坐标。画笔位置通常由moveto()、lineto()等函数设置，
 * 或者由绘图函数（如line()）的执行而改变。
 * 
 * @see getx() 获取当前x坐标
 * @see moveto() 移动画笔到指定位置
 * @see getwidth() 获取宽度
 * @see getheight() 获取高度
 */
int EGEAPI gety(PCIMAGE pimg = NULL);

/**
 * @brief 创建一个新的图像对象（1x1像素）
 * @return 返回新创建的图像对象指针，失败时返回NULL
 * 
 * 创建一个大小为1x1像素的图像对象，背景色为黑色。
 * 创建后的图像需要使用delimage()函数销毁以防止内存泄漏。
 * 
 * @note 这个函数创建的是最小尺寸的图像，通常用于后续调整大小或作为占位符
 * @warning 必须使用delimage()销毁创建的图像，否则会造成内存泄漏
 * 
 * @see newimage(int, int) 创建指定大小的图像
 * @see delimage() 销毁图像对象
 * @see resize() 调整图像大小
 */
PIMAGE         EGEAPI newimage();

/**
 * @brief 创建指定大小的新图像对象
 * @param width 图像宽度（像素），小于1时自动调整为1
 * @param height 图像高度（像素），小于1时自动调整为1
 * @return 返回新创建的图像对象指针，失败时返回NULL
 * 
 * 创建一个指定大小的图像对象，背景色为黑色。
 * 如果指定的宽度或高度小于1，会自动调整为1。
 * 创建后的图像需要使用delimage()函数销毁以防止内存泄漏。
 * 
 * @warning 必须使用delimage()销毁创建的图像，否则会造成内存泄漏
 * 
 * @see newimage() 创建1x1像素的图像
 * @see delimage() 销毁图像对象
 * @see resize() 调整图像大小
 */
PIMAGE         EGEAPI newimage(int width, int height);

/**
 * @brief 销毁图像对象并释放内存
 * @param pimg 要销毁的图像对象指针，如果为NULL则忽略
 * 
 * 销毁由newimage()函数创建的图像对象，释放相关的内存和系统资源。
 * 销毁后的图像指针不应再被使用。
 * 
 * @note 传入NULL指针是安全的，函数会忽略NULL参数
 * @warning 销毁图像后，不要再使用该图像指针，否则可能导致程序崩溃
 * 
 * @see newimage() 创建图像对象
 * @see newimage(int, int) 创建指定大小的图像对象
 */
void           EGEAPI delimage(PCIMAGE pimg);
//==================================================================================
// 图像管理和处理函数 - EGE图形库的图像操作功能
//==================================================================================
/**
 * @defgroup image_management 图像管理和处理函数
 * @brief EGE图形库的图像创建、加载、保存和处理功能
 * 
 * 图像管理模块提供了完整的图像操作能力，包括：
 * - 图像缓冲区操作：getbuffer() 获取像素缓冲区
 * - 图像尺寸调整：resize(), resize_f() 调整图像大小
 * - 图像获取：getimage() 系列函数从不同源获取图像数据
 * - 图像保存：saveimage(), savepng(), savebmp() 保存图像到文件
 * 
 * 支持的图像格式：PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * 支持从窗口、文件、资源、其他IMAGE对象获取图像
 * @{
 */

/**
 * @brief 获取图像像素缓冲区指针
 * @param pimg 要获取缓冲区的图像对象指针，默认为 NULL（表示窗口）
 * @return 图像缓冲区首地址，缓冲区为一维数组，大小为 图像宽度×图像高度
 * @note 坐标为(x, y)的像素对应缓冲区索引：buffer[y * width + x]
 * @note 返回的指针可以直接操作像素数据，修改后会立即生效
 */
color_t*       EGEAPI getbuffer(PIMAGE pimg);

/**
 * @brief 获取图像像素缓冲区指针（只读版本）
 * @param pimg 要获取缓冲区的图像对象指针，默认为 NULL（表示窗口）
 * @return 图像缓冲区首地址（只读），缓冲区为一维数组，大小为 图像宽度×图像高度
 * @note 坐标为(x, y)的像素对应缓冲区索引：buffer[y * width + x]
 * @note 返回的指针只能读取像素数据，不能修改
 */
const color_t* EGEAPI getbuffer(PCIMAGE pimg);

/**
 * @brief 调整图像尺寸（快速版本）
 * @param pimg 要调整大小的图像对象指针，不能为 NULL
 * @param width 图像新宽度
 * @param height 图像新高度
 * @return 成功返回 0，失败返回非 0 值
 * @note 调整后图像内容未定义，视口重置为初始状态
 * @note 此函数速度较快，但不保留原图像内容
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 */
int  EGEAPI resize_f(PIMAGE pimg, int width, int height);

/**
 * @brief 调整图像尺寸（标准版本）
 * @param pimg 要调整大小的图像对象指针，不能为 NULL
 * @param width 图像新宽度
 * @param height 图像新高度
 * @return 成功返回 0，失败返回非 0 值
 * @note 调整后图像用背景色填充，视口重置为初始状态
 * @note 此函数会清空图像内容并用背景色填充
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 */
int  EGEAPI resize  (PIMAGE pimg, int width, int height);

/**
 * @brief 从窗口获取图像
 * @param imgDest 保存图像的 IMAGE 对象指针
 * @param xSrc 要获取图像的区域左上角 x 坐标
 * @param ySrc 要获取图像的区域左上角 y 坐标
 * @param widthSrc 要获取图像的区域宽度
 * @param heightSrc 要获取图像的区域高度
 * @return 成功返回 grOk(0)，失败返回相应错误码
 * @note 从当前窗口的指定区域获取图像数据
 * @see getimage(PIMAGE, PCIMAGE, int, int, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, int xSrc, int ySrc, int widthSrc, int heightSrc);

/**
 * @brief 从另一个 IMAGE 对象获取图像
 * @param imgDest 保存图像的 IMAGE 对象指针
 * @param imgSrc 源图像 IMAGE 对象指针
 * @param xSrc 要获取图像的区域左上角 x 坐标
 * @param ySrc 要获取图像的区域左上角 y 坐标
 * @param widthSrc 要获取图像的区域宽度
 * @param heightSrc 要获取图像的区域高度
 * @return 成功返回 grOk(0)，失败返回相应错误码
 * @note 从源 IMAGE 对象的指定区域复制图像数据到目标 IMAGE 对象
 * @see getimage(PIMAGE, int, int, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, PCIMAGE imgSrc, int xSrc, int ySrc, int widthSrc, int heightSrc);

/**
 * @brief 从图片文件获取图像（char* 版本）
 * @param imgDest 保存图像的 IMAGE 对象指针
 * @param imageFile 图片文件名
 * @param zoomWidth 设定图像缩放至的宽度，0 表示使用原始宽度，不缩放
 * @param zoomHeight 设定图像缩放至的高度，0 表示使用原始高度，不缩放
 * @return 成功返回 grOk(0)，失败返回相应错误码（grAllocError/grFileNotFound/grNullPointer/grIOerror）
 * @note 支持格式：PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * @note 如果图像包含多帧，仅获取第一帧
 * @see getimage(PIMAGE, const wchar_t*, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, const char*  imageFile, int zoomWidth = 0, int zoomHeight = 0);

/**
 * @brief 从图片文件获取图像（wchar_t* 版本）
 * @param imgDest 保存图像的 IMAGE 对象指针
 * @param imageFile 图片文件名（宽字符版本）
 * @param zoomWidth 设定图像缩放至的宽度，0 表示使用原始宽度，不缩放
 * @param zoomHeight 设定图像缩放至的高度，0 表示使用原始高度，不缩放
 * @return 成功返回 grOk(0)，失败返回相应错误码（grAllocError/grFileNotFound/grNullPointer/grIOerror）
 * @note 支持格式：PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * @note 如果图像包含多帧，仅获取第一帧
 * @see getimage(PIMAGE, const char*, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, const wchar_t* imageFile, int zoomWidth = 0, int zoomHeight = 0);

/**
 * @brief 从资源文件获取图像（char* 版本）
 * @param imgDest 保存图像的 IMAGE 对象指针
 * @param resType 资源类型
 * @param resName 资源名称
 * @param zoomWidth 设定图像缩放至的宽度，0 表示使用原始宽度，不缩放
 * @param zoomHeight 设定图像缩放至的高度，0 表示使用原始高度，不缩放
 * @return 成功返回 grOk(0)，失败返回相应错误码（grAllocError/grFileNotFound/grNullPointer/grIOerror）
 * @note 支持格式：PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * @note 如果图像包含多帧，仅获取第一帧
 * @see getimage(PIMAGE, const wchar_t*, const wchar_t*, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, const char*  resType, const char*  resName, int zoomWidth = 0, int zoomHeight = 0);

/**
 * @brief 从资源文件获取图像（wchar_t* 版本）
 * @param imgDest 保存图像的 IMAGE 对象指针
 * @param resType 资源类型（宽字符版本）
 * @param resName 资源名称（宽字符版本）
 * @param zoomWidth 设定图像缩放至的宽度，0 表示使用原始宽度，不缩放
 * @param zoomHeight 设定图像缩放至的高度，0 表示使用原始高度，不缩放
 * @return 成功返回 grOk(0)，失败返回相应错误码（grAllocError/grFileNotFound/grNullPointer/grIOerror）
 * @note 支持格式：PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * @note 如果图像包含多帧，仅获取第一帧
 * @see getimage(PIMAGE, const char*, const char*, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, const wchar_t* resType, const wchar_t* resName, int zoomWidth = 0, int zoomHeight = 0);

/**
 * @brief 从 PNG 图片文件获取图像（char* 版本）
 * @param pimg 图像对象指针，需要先使用 newimage() 创建
 * @param filename 图像文件名
 * @return 成功返回 0，失败返回非 0 值
 * @note 获取后图像大小与原图大小一致，而不是图像原先的大小
 * @note 专门用于处理 PNG 格式图像
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 * @see getimage_pngfile(PIMAGE, const wchar_t*)
 */
int  EGEAPI getimage_pngfile(PIMAGE pimg, const char*  filename);

/**
 * @brief 从 PNG 图片文件获取图像（wchar_t* 版本）
 * @param pimg 图像对象指针，需要先使用 newimage() 创建
 * @param filename 图像文件名（宽字符版本）
 * @return 成功返回 0，失败返回非 0 值
 * @note 获取后图像大小与原图大小一致，而不是图像原先的大小
 * @note 专门用于处理 PNG 格式图像
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 * @see getimage_pngfile(PIMAGE, const char*)
 */
int  EGEAPI getimage_pngfile(PIMAGE pimg, const wchar_t* filename);

//==================================================================================
// putimage 系列函数 - EGE图形库的核心图像绘制功能
//==================================================================================
/**
 * @defgroup putimage_functions putimage系列函数
 * @brief EGE图形库的核心图像绘制功能
 * 
 * putimage系列函数提供了丰富的图像绘制和处理能力，包括：
 * - 基础图像绘制：putimage() 系列重载函数
 * - 透明效果：putimage_transparent(), putimage_alphablend(), putimage_withalpha()
 * - 混合效果：putimage_alphatransparent(), putimage_alphafilter()
 * - 变换效果：putimage_rotate(), putimage_rotatezoom(), putimage_rotatetransparent()
 * 
 * 这些函数支持多种绘制模式：
 * - 绘制到屏幕（imgDest = NULL）或另一个图像
 * - 支持区域裁剪、拉伸缩放、旋转变换
 * - 支持多种透明和混合模式
 * - 支持光栅操作码（SRCCOPY, SRCAND, SRCPAINT等）
 * - 支持平滑处理（抗锯齿）
 * @{
 */

/**
 * @brief 基础图像绘制函数 - 在指定位置绘制整个图像
 * @param x 绘制位置的 x 坐标
 * @param y 绘制位置的 y 坐标
 * @param pimg 要绘制的 IMAGE 对象指针
 * @param dwRop 三元光栅操作码，默认为 SRCCOPY（直接复制）
 */
void EGEAPI putimage(int x, int y, PCIMAGE pimg, DWORD dwRop = SRCCOPY);

/**
 * @brief 区域图像绘制函数 - 绘制图像的指定区域
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param widthDest 绘制的宽度
 * @param heightDest 绘制的高度
 * @param imgSrc 要绘制的 IMAGE 对象指针
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param dwRop 三元光栅操作码，默认为 SRCCOPY（直接复制）
 */
void EGEAPI putimage(int xDest, int yDest, int widthDest, int heightDest, PCIMAGE imgSrc, int xSrc, int ySrc, DWORD dwRop = SRCCOPY);

/**
 * @brief 拉伸图像绘制函数 - 将源图像区域拉伸到目标区域
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param widthDest 绘制的宽度
 * @param heightDest 绘制的高度
 * @param imgSrc 要绘制的 IMAGE 对象指针
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度
 * @param dwRop 三元光栅操作码，默认为 SRCCOPY（直接复制）
 */
void EGEAPI putimage(int xDest, int yDest, int widthDest, int heightDest, PCIMAGE imgSrc, int xSrc, int ySrc, int widthSrc, int heightSrc, DWORD dwRop = SRCCOPY);

/**
 * @brief 图像到图像绘制函数 - 在另一个图像上绘制图像
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param imgSrc 源 IMAGE 对象指针
 * @param dwRop 三元光栅操作码，默认为 SRCCOPY（直接复制）
 */
void EGEAPI putimage(PIMAGE imgDest, int xDest, int yDest, PCIMAGE imgSrc, DWORD dwRop = SRCCOPY);

/**
 * @brief 图像到图像区域绘制函数 - 在另一个图像上绘制图像的指定区域
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param widthDest 绘制的宽度
 * @param heightDest 绘制的高度
 * @param imgSrc 源 IMAGE 对象指针
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param dwRop 三元光栅操作码，默认为 SRCCOPY（直接复制）
 */
void EGEAPI putimage(PIMAGE imgDest, int xDest, int yDest, int widthDest, int heightDest, PCIMAGE imgSrc, int xSrc, int ySrc, DWORD dwRop = SRCCOPY);

/**
 * @brief 图像到图像拉伸绘制函数 - 在另一个图像上拉伸绘制图像
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param widthDest 绘制的宽度
 * @param heightDest 绘制的高度
 * @param imgSrc 源 IMAGE 对象指针
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度
 * @param dwRop 三元光栅操作码，默认为 SRCCOPY（直接复制）
 */
void EGEAPI putimage(PIMAGE imgDest, int xDest, int yDest, int widthDest, int heightDest, PCIMAGE imgSrc, int xSrc, int ySrc, int widthSrc, int heightSrc, DWORD dwRop = SRCCOPY);

/**
 * @brief 将图像保存到文件（char* 版本）
 * @param pimg 要保存的图像对象指针
 * @param filename 保存的图像文件名
 * @param withAlphaChannel 是否保存图像的透明通道，true: 保存, false: 不保存，默认为 false
 * @return 成功返回 grOk(0)，失败返回非 0 值
 * @note 目前支持 BMP 和 PNG 格式
 * @note 文件名以 .bmp 结尾保存为 BMP 格式，以 .png 结尾或无后缀名保存为 PNG 格式
 * @note 如果文件已存在，会覆盖原文件
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 * @see saveimage(PCIMAGE, const wchar_t*, bool)
 */
int  EGEAPI saveimage(PCIMAGE pimg, const char*  filename, bool withAlphaChannel = false);

/**
 * @brief 将图像保存到文件（wchar_t* 版本）
 * @param pimg 要保存的图像对象指针
 * @param filename 保存的图像文件名（宽字符版本）
 * @param withAlphaChannel 是否保存图像的透明通道，true: 保存, false: 不保存，默认为 false
 * @return 成功返回 grOk(0)，失败返回非 0 值
 * @note 目前支持 BMP 和 PNG 格式
 * @note 文件名以 .bmp 结尾保存为 BMP 格式，以 .png 结尾或无后缀名保存为 PNG 格式
 * @note 如果文件已存在，会覆盖原文件
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 * @see saveimage(PCIMAGE, const char*, bool)
 */
int  EGEAPI saveimage(PCIMAGE pimg, const wchar_t* filename, bool withAlphaChannel = false);

/**
 * @brief 将图像以 PNG 格式保存到文件（char* 版本）
 * @param pimg 要保存的图像对象指针
 * @param filename 保存的图像文件名
 * @param withAlphaChannel 是否保存图像的透明通道，true: 保存, false: 不保存，默认为 false
 * @return 成功返回 0，失败返回非 0 值
 * @note 强制以 PNG 格式保存，不受文件名后缀影响
 * @note 如果文件已存在，会覆盖原文件
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 * @see savepng(PCIMAGE, const wchar_t*, bool)
 */
int  EGEAPI savepng  (PCIMAGE pimg, const char*  filename, bool withAlphaChannel = false);

/**
 * @brief 将图像以 PNG 格式保存到文件（wchar_t* 版本）
 * @param pimg 要保存的图像对象指针
 * @param filename 保存的图像文件名（宽字符版本）
 * @param withAlphaChannel 是否保存图像的透明通道，true: 保存, false: 不保存，默认为 false
 * @return 成功返回 0，失败返回非 0 值
 * @note 强制以 PNG 格式保存，不受文件名后缀影响
 * @note 如果文件已存在，会覆盖原文件
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 * @see savepng(PCIMAGE, const char*, bool)
 */
int  EGEAPI savepng  (PCIMAGE pimg, const wchar_t* filename, bool withAlphaChannel = false);

/**
 * @brief 将图像以 BMP 格式保存到文件（char* 版本）
 * @param pimg 要保存的图像对象指针
 * @param filename 保存的图像文件名
 * @param withAlphaChannel 是否保存图像的透明通道，true: 保存, false: 不保存，默认为 false
 * @return 成功返回 0，失败返回非 0 值
 * @note 强制以 BMP 格式保存，不受文件名后缀影响
 * @note 如果文件已存在，会覆盖原文件
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 * @see savebmp(PCIMAGE, const wchar_t*, bool)
 */
int  EGEAPI savebmp  (PCIMAGE pimg, const char*  filename, bool withAlphaChannel = false);

/**
 * @brief 将图像以 BMP 格式保存到文件（wchar_t* 版本）
 * @param pimg 要保存的图像对象指针
 * @param filename 保存的图像文件名（宽字符版本）
 * @param withAlphaChannel 是否保存图像的透明通道，true: 保存, false: 不保存，默认为 false
 * @return 成功返回 0，失败返回非 0 值
 * @note 强制以 BMP 格式保存，不受文件名后缀影响
 * @note 如果文件已存在，会覆盖原文件
 * @warning 如果 pimg 为无效指针，会引发运行时异常
 * @see savebmp(PCIMAGE, const char*, bool)
 */
int  EGEAPI savebmp  (PCIMAGE pimg, const wchar_t* filename, bool withAlphaChannel = false);

/**
 * @brief 透明色绘制函数 - 指定颜色变为透明的图像绘制
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param transparentColor 要变为透明的像素颜色
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标，默认为 0
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标，默认为 0
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度，默认为 0（使用整个图像宽度）
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度，默认为 0（使用整个图像高度）
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_transparent(
    PIMAGE  imgDest,            // handle to dest
    PCIMAGE imgSrc,             // handle to source
    int xDest,                  // x-coord of destination upper-left corner
    int yDest,                  // y-coord of destination upper-left corner
    color_t transparentColor,   // color to make transparent
    int xSrc = 0,               // x-coord of source upper-left corner
    int ySrc = 0,               // y-coord of source upper-left corner
    int widthSrc = 0,           // width of source rectangle    
    int heightSrc = 0           // height of source rectangle
);

/**
 * @brief Alpha混合绘制函数 - 基础版本，指定整体透明度
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param alpha 图像整体透明度 (0-255)，0为完全透明，255为完全不透明
 * @param alphaType 源图像像素的 alpha 类型，默认为 ALPHATYPE_STRAIGHT
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_alphablend(
    PIMAGE  imgDest,
    PCIMAGE imgSrc,
    int xDest,
    int yDest,
    unsigned char alpha,
    alpha_type alphaType = ALPHATYPE_STRAIGHT
);

/**
 * @brief Alpha混合绘制函数 - 指定源图像起始位置
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param alpha 图像整体透明度 (0-255)，0为完全透明，255为完全不透明
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param alphaType 源图像像素的 alpha 类型，默认为 ALPHATYPE_STRAIGHT
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_alphablend(
    PIMAGE  imgDest,
    PCIMAGE imgSrc,
    int xDest,
    int yDest,
    unsigned char alpha,
    int xSrc,
    int ySrc,
    alpha_type alphaType = ALPHATYPE_STRAIGHT
);

/**
 * @brief Alpha混合绘制函数 - 指定源图像区域
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param alpha 图像整体透明度 (0-255)，0为完全透明，255为完全不透明
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度
 * @param alphaType 源图像像素的 alpha 类型，默认为 ALPHATYPE_STRAIGHT
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_alphablend(
    PIMAGE  imgDest,
    PCIMAGE imgSrc,
    int xDest,
    int yDest,
    unsigned char alpha,
    int xSrc,
    int ySrc,
    int widthSrc,
    int heightSrc,
    alpha_type alphaType = ALPHATYPE_STRAIGHT
);

/**
 * @brief Alpha混合绘制函数 - 完整版本，支持拉伸和平滑处理
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param widthDest 绘制的宽度
 * @param heightDest 绘制的高度
 * @param alpha 图像整体透明度 (0-255)，0为完全透明，255为完全不透明
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度
 * @param smooth 是否使用平滑处理（抗锯齿），默认为 false
 * @param alphaType 源图像像素的 alpha 类型，默认为 ALPHATYPE_STRAIGHT
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_alphablend(
    PIMAGE  imgDest,
    PCIMAGE imgSrc,
    int xDest,
    int yDest,
    int widthDest,
    int heightDest,
    unsigned char alpha,
    int xSrc,
    int ySrc,
    int widthSrc,
    int heightSrc,
    bool smooth = false,
    alpha_type alphaType = ALPHATYPE_STRAIGHT
);

/**
 * @brief Alpha透明色混合绘制函数 - 结合透明色和Alpha混合
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param transparentColor 要变为透明的像素颜色
 * @param alpha 图像整体透明度 (0-255)，0为完全透明，255为完全不透明
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标，默认为 0
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标，默认为 0
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度，默认为 0（使用整个图像宽度）
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度，默认为 0（使用整个图像高度）
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_alphatransparent(
    PIMAGE  imgDest,            // handle to dest
    PCIMAGE imgSrc,             // handle to source
    int xDest,                  // x-coord of destination upper-left corner
    int yDest,                  // y-coord of destination upper-left corner
    color_t transparentColor,   // color to make transparent
    unsigned char alpha,        // alpha
    int xSrc = 0,               // x-coord of source upper-left corner
    int ySrc = 0,               // y-coord of source upper-left corner
    int widthSrc = 0,           // width of source rectangle
    int heightSrc = 0           // height of source rectangle
);

/**
 * @brief Alpha通道绘制函数 - 使用图像自身的Alpha通道，基础版本
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针（必须包含Alpha通道数据）
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标，默认为 0
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标，默认为 0
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度，默认为 0（使用整个图像宽度）
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度，默认为 0（使用整个图像高度）
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_withalpha(
    PIMAGE  imgDest,            // handle to dest
    PCIMAGE imgSrc,             // handle to source
    int xDest,                  // x-coord of destination upper-left corner
    int yDest,                  // y-coord of destination upper-left corner
    int xSrc = 0,               // x-coord of source upper-left corner
    int ySrc = 0,               // y-coord of source upper-left corner
    int widthSrc = 0,           // width of source rectangle
    int heightSrc = 0           // height of source rectangle
);

/**
 * @brief Alpha通道绘制函数 - 使用图像自身的Alpha通道，支持拉伸和平滑处理
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针（必须包含Alpha通道数据）
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param widthDest 绘制的宽度
 * @param heightDest 绘制的高度
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度
 * @param smooth 是否使用平滑处理（抗锯齿），默认为 false
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_withalpha(
    PIMAGE  imgDest,            // handle to dest
    PCIMAGE imgSrc,             // handle to source
    int xDest,                  // x-coord of destination upper-left corner
    int yDest,                  // y-coord of destination upper-left corner
    int widthDest,              // width of destination rectangle
    int heightDest,             // height of destination rectangle
    int xSrc,                   // x-coord of source upper-left corner
    int ySrc,                   // y-coord of source upper-left corner
    int widthSrc,               // width of source rectangle
    int heightSrc,              // height of source rectangle
    bool smooth = false
);

/**
 * @brief Alpha滤镜绘制函数 - 使用另一图像作为Alpha遮罩
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param imgAlpha 用作Alpha遮罩的 IMAGE 对象指针
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_alphafilter(
    PIMAGE  imgDest,            // handle to dest
    PCIMAGE imgSrc,             // handle to source
    int xDest,                  // x-coord of destination upper-left corner
    int yDest,                  // y-coord of destination upper-left corner
    PCIMAGE imgAlpha,           // handle to alpha
    int xSrc,                   // x-coord of source upper-left corner
    int ySrc,                   // y-coord of source upper-left corner
    int widthSrc,               // width of source rectangle
    int heightSrc               // height of source rectangle
);

/**
 * @brief 图像模糊滤镜函数 - 对图像进行模糊处理
 * @param imgDest 目标 IMAGE 对象指针，要进行模糊处理的图像
 * @param intensity 模糊强度，值越大模糊效果越强
 * @param alpha 图像整体透明度 (0-255)，255为完全不透明
 * @param xDest 处理区域的左上角 x 坐标，默认为 0
 * @param yDest 处理区域的左上角 y 坐标，默认为 0
 * @param widthDest 处理区域的宽度，默认为 0（使用整个图像宽度）
 * @param heightDest 处理区域的高度，默认为 0（使用整个图像高度）
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI imagefilter_blurring (
    PIMAGE imgDest,
    int intensity,
    int alpha,
    int xDest = 0,
    int yDest = 0,
    int widthDest = 0,
    int heightDest = 0
);

/**
 * @brief 旋转绘制函数 - 围绕中心点旋转图像
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgTexture 源纹理 IMAGE 对象指针
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param xCenter 旋转中心点的 x 坐标（相对于源图像）
 * @param yCenter 旋转中心点的 y 坐标（相对于源图像）
 * @param radian 旋转角度（弧度制，顺时针方向）
 * @param transparent 是否使用图像的透明通道，默认为 false
 * @param alpha 图像整体透明度 (0-256)，-1表示不使用alpha，默认为 -1
 * @param smooth 是否使用平滑处理（抗锯齿），默认为 false
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_rotate(
    PIMAGE  imgDest,
    PCIMAGE imgTexture,
    int   xDest,
    int   yDest,
    float xCenter,
    float yCenter,
    float radian,
    bool  transparent = false,   // use the transparent channel of the image
    int   alpha = -1,            // in range[0, 256], alpha== -1 means no alpha
    bool  smooth = false
);

/**
 * @brief 旋转缩放绘制函数 - 同时进行旋转和缩放
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgTexture 源纹理 IMAGE 对象指针
 * @param xDest 绘制位置的 x 坐标
 * @param yDest 绘制位置的 y 坐标
 * @param xCenter 旋转中心点的 x 坐标（相对于源图像）
 * @param yCenter 旋转中心点的 y 坐标（相对于源图像）
 * @param radian 旋转角度（弧度制，顺时针方向）
 * @param zoom 缩放比例，1.0为原始大小
 * @param transparent 是否使用图像的透明通道，默认为 false
 * @param alpha 图像整体透明度 (0-256)，-1表示不使用alpha，默认为 -1
 * @param smooth 是否使用平滑处理（抗锯齿），默认为 false
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_rotatezoom(
    PIMAGE imgDest,
    PCIMAGE imgTexture,
    int xDest,
    int yDest,
    float xCenter,
    float yCenter,
    float radian,
    float zoom,
    bool transparent = false,   // use the transparent channel of the image
    int alpha = -1,             // in range[0, 256], alpha== -1 means no alpha
    bool smooth = false
);

/**
 * @brief 旋转透明绘制函数 - 旋转时指定透明色，基础版本
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针
 * @param xCenterDest 旋转中心点在目标图像中的 x 坐标
 * @param yCenterDest 旋转中心点在目标图像中的 y 坐标
 * @param xCenterSrc 旋转中心点在源图像中的 x 坐标
 * @param yCenterSrc 旋转中心点在源图像中的 y 坐标
 * @param transparentColor 要变为透明的像素颜色
 * @param radian 旋转角度（弧度制，顺时针方向）
 * @param zoom 缩放比例，1.0为原始大小，默认为 1.0f
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_rotatetransparent(
    PIMAGE imgDest,             /* handle to dest, NULL means the SCREEN  */
    PCIMAGE imgSrc,             /* handle to source */
    int xCenterDest,            /* x-coord of rotation center in dest */
    int yCenterDest,            /* y-coord of rotation center in dest */
    int xCenterSrc,             /* x-coord of rotation center in source */
    int yCenterSrc,             /* y-coord of rotation center in source */
    color_t transparentColor,   /* color to make transparent */
    float radian,               /* rotation angle (clockwise, in radian) */
    float zoom = 1.0f           /* zoom factor */
);

/**
 * @brief 旋转透明绘制函数 - 旋转时指定透明色，完整版本
 * @param imgDest 目标 IMAGE 对象指针，如果为 NULL 则绘制到屏幕
 * @param imgSrc 源 IMAGE 对象指针
 * @param xCenterDest 旋转中心点在目标图像中的 x 坐标
 * @param yCenterDest 旋转中心点在目标图像中的 y 坐标
 * @param xSrc 绘制内容在源 IMAGE 对象中的左上角 x 坐标
 * @param ySrc 绘制内容在源 IMAGE 对象中的左上角 y 坐标
 * @param widthSrc 绘制内容在源 IMAGE 对象中的宽度
 * @param heightSrc 绘制内容在源 IMAGE 对象中的高度
 * @param xCenterSrc 旋转中心点在源图像中的 x 坐标
 * @param yCenterSrc 旋转中心点在源图像中的 y 坐标
 * @param transparentColor 要变为透明的像素颜色
 * @param radian 旋转角度（弧度制，顺时针方向）
 * @param zoom 缩放比例，1.0为原始大小，默认为 1.0f
 * @return 成功返回 grOk，失败返回相应错误码
 */
int EGEAPI putimage_rotatetransparent(
    PIMAGE imgDest,             /* handle to dest, NULL means the SCREEN */
    PCIMAGE imgSrc,             /* handle to source */
    int xCenterDest,            /* x-coord of rotation center in dest */
    int yCenterDest,            /* y-coord of rotation center in dest */
    int xSrc,                   /* x-coord of source upper-left corner */
    int ySrc,                   /* y-coord of source upper-left corner */
    int widthSrc,               /* width of source rectangle */
    int heightSrc,              /* height of source rectangle */
    int xCenterSrc,             /* x-coord of rotation center in source */
    int yCenterSrc,             /* y-coord of rotation center in source */
    color_t transparentColor,   /* color to make transparent */
    float radian,               /* rotation angle (clockwise, in radian) */
    float zoom = 1.0f           /* zoom factor */
);

/** @} */ // 结束 putimage_functions 组

/**
 * @brief 获取绘图窗口的窗口句柄
 * @return 绘图窗口的窗口句柄(HWND)
 * @note 返回的是 Windows 系统的窗口句柄，可用于 Windows API 调用
 * @see getHInstance(), getHDC()
 */
HWND        EGEAPI getHWnd();

/**
 * @brief 获取绘图窗口的实例句柄
 * @return 应用程序实例句柄(HINSTANCE)
 * @note 返回的是 Windows 系统的应用程序实例句柄，可用于 Windows API 调用
 * @see getHWnd(), getHDC()
 */
HINSTANCE   EGEAPI getHInstance();

/**
 * @brief 获取绘图设备上下文
 * @param pimg 图像对象指针，如果为 NULL 则获取绘图窗口的设备上下文
 * @return 设备上下文句柄(HDC)
 * @note 返回的是 Windows 系统的设备上下文句柄，可用于 GDI 绘图操作
 * @warning 不要手动释放返回的 HDC，由 EGE 库自动管理
 * @see getHWnd(), getHInstance()
 */
HDC         EGEAPI getHDC(PCIMAGE pimg = NULL);

/**
 * @brief 获取过程函数指针
 * @return 过程函数指针
 * @note 内部使用函数，用于获取窗口过程函数指针
 */
PVOID       EGEAPI getProcfunc();

/**
 * @brief 获取 EGE 图形库版本号
 * @return EGE 图形库的版本号
 * @note 版本号格式为数字形式，可用于兼容性检查
 */
long        EGEAPI getGraphicsVer();

/**
 * @brief 获取当前帧率
 * @return 当前的帧率(fps)
 * @note 返回每秒帧数，用于性能监控和调试
 * @see delay_fps()
 */
float       EGEAPI getfps();

/**
 * @brief 初始化随机数生成器（使用当前时间作为种子）
 * @return 使用的随机数种子值
 * @note 使用当前时间戳作为种子初始化 Mersenne Twister 随机数生成器
 * @see randomize(unsigned int seed), random(), randomf()
 */
unsigned int    EGEAPI randomize();

/**
 * @brief 初始化随机数生成器（使用指定种子）
 * @param seed 随机数种子
 * @return 使用的随机数种子值
 * @note 使用指定种子初始化 Mersenne Twister 随机数生成器，相同种子产生相同序列
 * @see randomize(), random(), randomf()
 */
unsigned int    EGEAPI randomize(unsigned int seed);

/**
 * @brief 生成随机整数
 * @param n 随机数范围的上限，如果为 0 则生成完整范围的随机数
 * @return 生成的随机整数，范围为 [0, n) 或 [0, UINT_MAX]
 * @note 使用 Mersenne Twister 算法生成高质量随机数
 * @see randomize(), randomf()
 */
unsigned int    EGEAPI random(unsigned int n = 0);

/**
 * @brief 生成随机浮点数
 * @return 生成的随机浮点数，范围为 [0.0, 1.0)
 * @note 使用 Mersenne Twister 算法生成高质量随机浮点数
 * @see randomize(), random()
 */
double          EGEAPI randomf();

/**
 * @brief 显示输入对话框获取单行文本（ASCII 版本）
 * @param title 对话框标题
 * @param text 提示文本
 * @param buf 用于存储输入文本的缓冲区
 * @param len 缓冲区长度
 * @return 成功返回非零值，失败或取消返回 0
 * @note 显示一个模态对话框让用户输入单行文本
 * @warning 确保缓冲区足够大以避免溢出
 * @see inputbox_getline(const wchar_t*, const wchar_t*, LPWSTR, int)
 */
int EGEAPI inputbox_getline(const char*  title, const char*  text, LPSTR  buf, int len);

/**
 * @brief 显示输入对话框获取单行文本（Unicode 版本）
 * @param title 对话框标题
 * @param text 提示文本
 * @param buf 用于存储输入文本的缓冲区
 * @param len 缓冲区长度
 * @return 成功返回非零值，失败或取消返回 0
 * @note 显示一个模态对话框让用户输入单行文本，支持 Unicode 字符
 * @warning 确保缓冲区足够大以避免溢出
 * @see inputbox_getline(const char*, const char*, LPSTR, int)
 */
int EGEAPI inputbox_getline(const wchar_t* title, const wchar_t* text, LPWSTR buf, int len);



/// @defgroup InputHandling 输入处理
/// 键盘和鼠标输入处理相关函数
/// @{

/// @defgroup KeyboardInput 键盘输入
/// 键盘输入检测和消息处理函数
/// @{

/**
 * @brief 检测当前是否有键盘消息
 * @return 非零值表示有键盘消息，0表示没有，一般与getkey搭配使用
 * @note 用于非阻塞检测键盘输入，通常在主循环中与getkey()一起使用
 * @see getkey()
 */
int     EGEAPI kbmsg();

/**
 * @brief 获取键盘消息
 * @return 键盘消息结构体，包含按键代码、消息类型和标志位
 * @note 如果当前没有键盘消息则等待，支持按键按下、释放和字符消息
 * @see kbmsg(), key_msg, key_msg_e, key_flag_e
 */
key_msg EGEAPI getkey();

/**
 * @brief 获取键盘字符输入（扩展版本）
 * @param flag 消息标志位，控制接收的消息类型
 * @return 按键代码和消息类型组合
 * @deprecated 请使用getch()函数代替
 * @note 支持按键按下和释放事件的获取
 * @see getch()
 */
EGE_DEPRECATE(getchEx, "Please use the 'getch' function instead.")
int     EGEAPI getchEx(int flag);

/**
 * @brief 检测当前是否有键盘字符输入（扩展版本）
 * @param flag 消息标志位，控制检测的消息类型
 * @return 非零值表示有输入，0表示没有
 * @deprecated 请使用kbhit()函数代替
 * @note 支持按键按下和释放事件的检测
 * @see kbhit()
 */
EGE_DEPRECATE(kbhitEx, "Please use the 'kbhit' function instead.")
int     EGEAPI kbhitEx(int flag);

/**
 * @brief 判断键盘或鼠标上的某按键是否处于按下状态
 * @param key 要检测的按键代码，参见key_code_e枚举
 * @return 非零值表示按键按下，0表示未按下，-1表示参数错误
 * @note 实时检测按键状态，不消耗消息队列；支持键盘和鼠标按键
 * @see key_code_e
 */
int     EGEAPI keystate(int key);

/**
 * @brief 清空按键消息缓存区
 * @note 清除所有未处理的键盘消息，常用于忽略缓存的按键输入
 * @see flushmouse()
 */
void    EGEAPI flushkey();

/// @} // KeyboardInput

/// @defgroup CharacterInput 字符输入
/// 字符级键盘输入处理函数
/// @{

/**
 * @brief 获取键盘字符输入（EGE内部版本）
 * @return 字符的ASCII码，特殊键返回扩展码
 * @note EGE内部实现，用于避免与conio.h库冲突
 * @see getch()
 */
int     EGEAPI ege_getch();

/**
 * @brief 检测当前是否有键盘字符输入（EGE内部版本）
 * @return 非零值表示有字符输入，0表示没有
 * @note EGE内部实现，用于避免与conio.h库冲突
 * @see kbhit()
 */
int     EGEAPI ege_kbhit();

#if !defined(_INC_CONIO) && !defined(_CONIO_H_)
#define _INC_CONIO
#define _CONIO_H_

/**
 * @brief 获取键盘字符输入
 * @return 字符的ASCII码，特殊键返回扩展码
 * @note 如果当前没有字符输入则等待；等同于ege_getch()
 * @see ege_getch(), kbhit()
 */
int EGEAPI getch();  // Same as ege_getch()

/**
 * @brief 检测当前是否有键盘字符输入
 * @return 非零值表示有字符输入，0表示没有
 * @note 用于非阻塞检测字符输入，一般与getch搭配使用；等同于ege_kbhit()
 * @see ege_kbhit(), getch()
 */
int EGEAPI kbhit();  // Same as ege_kbhit()
#else
#define getch ege_getch
#define kbhit ege_kbhit
#endif

/// @} // CharacterInput

/// @defgroup MouseInput 鼠标输入
/// 鼠标输入检测和消息处理函数
/// @{

/**
 * @brief 检测当前是否有鼠标消息
 * @return 非零值表示有鼠标消息，0表示没有
 * @note 用于非阻塞检测鼠标输入，一般与getmouse搭配使用
 * @see getmouse()
 */
int         EGEAPI mousemsg();

/**
 * @brief 获取一个鼠标消息
 * @return 鼠标消息结构体，包含坐标、按键状态和消息类型
 * @note 如果当前鼠标消息队列为空则等待，直到有新消息产生
 * @see mousemsg(), mouse_msg, mouse_msg_e, mouse_flag_e
 */
mouse_msg   EGEAPI getmouse();

/**
 * @brief 获取一个鼠标消息（过时函数）
 * @return MOUSEMSG结构体，包含鼠标状态信息
 * @deprecated 请使用getmouse()函数代替
 * @note 兼容旧版本API，建议使用更现代的getmouse()函数
 * @see getmouse()
 */
EGE_DEPRECATE(GetMouseMsg, "Please use the 'getmouse' function instead.")
MOUSEMSG    EGEAPI GetMouseMsg();

/**
 * @brief 清空鼠标消息缓冲区
 * @note 清除所有未处理的鼠标消息，常用于忽略缓存的鼠标输入
 * @see flushkey()
 */
void        EGEAPI flushmouse();

/**
 * @brief 设置鼠标指针是否显示
 * @param bShow 非零值显示鼠标指针，0隐藏鼠标指针
 * @return 之前的显示状态
 * @note 用于控制鼠标光标在图形窗口中的可见性
 */
int         EGEAPI showmouse(int bShow);

/**
 * @brief 获取当前鼠标位置坐标
 * @param x 接收鼠标x坐标的指针
 * @param y 接收鼠标y坐标的指针
 * @return 函数执行状态
 * @note 获取鼠标在图形窗口中的实时坐标位置
 * @see keystate()
 */
int         EGEAPI mousepos(int *x, int *y);

/// @} // MouseInput
/// @} // InputHandling

/*
callback function define as:
int __stdcall on_msg_key(void* param, unsigned msg, int key);
msg: see 'enum message_event'
key: keycode
return zero means process this message, otherwise means pass it and then process with 'getkey' function
*/
//int message_addkeyhandler(void* param, LPMSG_KEY_PROC func);
/*
callback function define as:
int __stdcall on_msg_mouse(void* param, unsigned msg, int key, int x, int y);
msg: see 'enum message_event'
key: see 'enum message_mouse', if msg==MSG_EVENT_WHELL, key is a int number that indicates the distance the wheel is rotated, expressed in multiples or divisions of WHEEL_DELTA, which is 120.
x,y: current mouse (x, y)
return zero means process this message, otherwise means pass it and then process with 'GetMouseMsg' function
*/
//int message_addmousehandler(void* param, LPMSG_MOUSE_PROC func);
int EGEAPI SetCloseHandler(LPCALLBACK_PROC func);

/**
 * @brief 音乐播放类
 * 
 * MUSIC 类提供了基于 Windows Media Control Interface (MCI) 的音乐播放功能，
 * 支持播放 WAV、MP3、MIDI 等多种音频格式。
 * 
 * @note 该类基于 Windows MCI 实现，仅支持 Windows 平台
 * @note 支持的音频格式包括：WAV, MP3, MIDI 等
 * @see music_state_flag, MUSIC_ERROR
 */
class MUSIC
{
public:
    /**
     * @brief 构造函数
     * @note 初始化音乐播放器，设置初始状态为未打开
     */
    MUSIC();
    
    /**
     * @brief 析构函数
     * @note 自动关闭已打开的音乐文件并清理资源
     */
    virtual ~MUSIC();

    /**
     * @brief 类型转换操作符
     * @return 返回窗口句柄(HWND)
     * @note 用于与 Windows API 交互
     */
    operator HWND() const { return (HWND)m_dwCallBack; }

public:
    /**
     * @brief 检查是否已打开音乐文件
     * @return 1 表示已打开，0 表示未打开
     * @note 用于判断是否成功打开了音乐文件
     */
    int IsOpen() { return (m_DID != MUSIC_ERROR) ? 1 : 0; }

    /**
     * @brief 打开音乐文件（ASCII 版本）
     * @param filepath 音乐文件路径（包含文件名）
     * @return 操作成功返回 0，操作失败返回非 0
     * @note 支持 WAV、MP3、MIDI 等格式，成功打开后播放状态为 MUSIC_MODE_STOP
     * @note 如果已经打开了其他文件，会自动关闭原文件
     * @see OpenFile(const wchar_t*), Close()
     */
    DWORD OpenFile(const char* filepath);
    
    /**
     * @brief 打开音乐文件（Unicode 版本）
     * @param filepath 音乐文件路径（包含文件名）
     * @return 操作成功返回 0，操作失败返回非 0
     * @note 支持 WAV、MP3、MIDI 等格式，成功打开后播放状态为 MUSIC_MODE_STOP
     * @note 如果已经打开了其他文件，会自动关闭原文件
     * @see OpenFile(const char*), Close()
     */
    DWORD OpenFile(const wchar_t* filepath);
    
    /**
     * @brief 播放音乐
     * @param dwFrom 播放开始位置（毫秒），默认为 MUSIC_ERROR（继续播放）
     * @param dwTo 播放结束位置（毫秒），默认为 MUSIC_ERROR（播放到末尾）
     * @return 操作成功返回 0，操作失败返回非 0
     * @note Play() 为继续播放，Play(0) 为从头开始播放
     * @note 播放状态变为 MUSIC_MODE_PLAY
     * @see Pause(), Stop(), RepeatPlay()
     */
    DWORD Play(DWORD dwFrom = MUSIC_ERROR, DWORD dwTo = MUSIC_ERROR);
    
    /**
     * @brief 循环播放音乐
     * @param dwFrom 播放开始位置（毫秒），默认为 MUSIC_ERROR
     * @param dwTo 播放结束位置（毫秒），默认为 MUSIC_ERROR
     * @return 操作成功返回 0，操作失败返回非 0
     * @note 在指定的时间段内循环播放音乐
     * @see Play(), Pause(), Stop()
     */
    DWORD RepeatPlay(DWORD dwFrom = MUSIC_ERROR, DWORD dwTo = MUSIC_ERROR);
    
    /**
     * @brief 暂停播放
     * @return 操作成功返回 0，操作失败返回非 0
     * @note 播放状态变为 MUSIC_MODE_PAUSE，播放进度保持不变
     * @see Play(), Stop()
     */
    DWORD Pause();
    
    /**
     * @brief 定位播放位置
     * @param dwTo 目标播放位置（毫秒）
     * @return 操作成功返回 0，操作失败返回非 0
     * @note 目前此函数无效，建议使用 Play(dwTo) 代替
     * @deprecated 推荐使用 Play(dwTo) 实现定位播放
     * @see Play()
     */
    DWORD Seek(DWORD dwTo);
    
    /**
     * @brief 设置播放音量
     * @param value 音量大小，范围 0.0~1.0
     * @return 操作成功返回 0，操作失败返回非 0
     * @note 0.0 为静音，1.0 为最大音量
     */
    DWORD SetVolume(float value);
    
    /**
     * @brief 关闭音乐文件
     * @return 操作成功返回 0，操作失败返回非 0
     * @note 关闭当前打开的音乐文件并释放相关资源
     * @see OpenFile()
     */
    DWORD Close();
    
    /**
     * @brief 停止播放
     * @return 操作成功返回 0，操作失败返回非 0
     * @note 播放状态变为 MUSIC_MODE_STOP，播放进度保持不变
     * @see Play(), Pause()
     */
    DWORD Stop();
    
    /**
     * @brief 获取当前播放位置
     * @return 当前播放位置（毫秒）
     * @note 返回当前播放进度，单位为毫秒
     * @see GetLength(), GetPlayStatus()
     */
    DWORD GetPosition();
    
    /**
     * @brief 获取音乐总时长
     * @return 音乐总时长（毫秒）
     * @note 返回音乐文件的总长度，单位为毫秒
     * @see GetPosition(), GetPlayStatus()
     */
    DWORD GetLength();

    /**
     * @brief 获取播放状态
     * @return 当前播放状态，参见 music_state_flag 枚举
     * @note 返回值可能为：
     *       - MUSIC_MODE_NOT_OPEN: 未打开
     *       - MUSIC_MODE_NOT_READY: 设备未就绪
     *       - MUSIC_MODE_PAUSE: 暂停中
     *       - MUSIC_MODE_PLAY: 正在播放
     *       - MUSIC_MODE_STOP: 停止状态
     *       - MUSIC_MODE_OPEN: 打开中
     *       - MUSIC_MODE_SEEK: 定位中
     * @see music_state_flag, GetPosition(), GetLength()
     */
    DWORD GetPlayStatus();

private:
    DWORD m_DID;        ///< MCI 设备 ID
    PVOID m_dwCallBack; ///< 回调句柄
};

int           EGEAPI ege_compress  (void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen);
int           EGEAPI ege_compress2 (void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen, int level);
int           EGEAPI ege_uncompress(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen);
unsigned long EGEAPI ege_uncompress_size(const void *source, unsigned long sourceLen);

}

#ifndef EGE_GRAPH_LIB_BUILD
    #if defined(_MSC_VER) && (defined(HIDE_CONSOLE) || !defined(SHOW_CONSOLE))
        #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
    #endif

    #define Sleep(ms) delay_ms(ms)
#endif

#if !defined(_MSC_VER)
    #define WinMain(...) WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#elif defined(_CONSOLE)
    #if (_MSC_VER > 1200)
        #define WinMain(...) main(int argc, char* argv[])
    #else
        #define WinMain main
    #endif
#endif

#endif
