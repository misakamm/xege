/*********************************************************
* EGE (Easy Graphics Engine)  20.08
* FileName      ege.h
* HomePage1     http://misakamm.github.com/xege
* HomePage2     http://misakamm.bitbucket.org/index.htm
* teiba1        http://tieba.baidu.com/f?kw=ege
* teiba2        http://tieba.baidu.com/f?kw=ege%C4%EF
* Blog:         http://misakamm.com
* E-Mail:       mailto:misakamm[at gmail com]
*
* FileName: ege.h
* �� VC ��ģ�� Borland BGI ��ͼ�⣬ʵ�ּ򵥵Ļ�ͼ֮�࣬��չ�˽ϸ��ӵĻ�ͼ����
*
* ������ʹ�ñ���ʱ����Ҫ����conio.hͷ�ļ�
* ��Щͷ�ļ���Ӧ���棬������ܻ�������
* ����getch��conio.h�ڵĸ��ǣ��ɰ����������������ע��
* ���蹲�棬��ʹ�ö��ļ��ֿ�������ģʽʹ�ã�
* ������һ��cppͬʱ�����������Էֿ�����
* ʹ�ñ��⣬������C++���룬��֧�ֵı�������
* VC6/VC2008/VC2010/VC2012/VC2013/MinGW3.4.5/MinGW4.7.1/MinGW4.8.1
*********************************************************/

/****************************************************************************
** ע�����
* �������Ҫ��ʾ����̨���ڣ����ڰ������ļ���ǰ���һ��define SHOW_CONSOLE
* �����Sleep���APIʱ�����ߵ���delay��ʵ�ʾ���ת��Ϊ����delay_ms����������API��ʹ��api_sleep
* ��delay_ms(0)�������ж���û�и��µı�Ҫ��������ε��������ĵ��ò��������֡�ʵ�Ӱ��
* �����delay_ms, delay_fps, getch, getkey, getmouse ʱ���������ݿ��ܻ���£���Щ�����൱��������delay_ms(0)��
*   �����ֻ��Ҫ���´��ڣ�������ȴ���������delay_ms(0)��ע��delayֻ��ʱ�������´���
* ������ʹ��delay_ms/delay_fps���������Լ�����ĳ���ռ�õ�CPU������һ����û�е���ͬʱҲû��getch/getmouse�Ļ�������ռ��һ��CPU��ʱ��
****************************************************************************/

#ifndef EGE_H
#define EGE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

// Easy Graphics Engine Version
// Calendar Versioning, format: YY.0M.PatchNumber (If the PatchNumber equals 0, the YY.0M format is used.)
#define EGE_VERSION        "20.08"
#define EGE_VERSION_MAJOR  20
#define EGE_VERSION_MINOR  8
#define EGE_VERSION_PATCH  0
#define EGE_MAKE_VERSION_NUMBER(major, minor, patch)    ((major) * 10000L + (minor) * 100L + (patch))
#define EGE_VERSION_NUMBER    EGE_MAKE_VERSION_NUMBER(EGE_VERSION_MAJOR, EGE_VERSION_MINOR, EGE_VERSION_PATCH)

#ifndef __cplusplus
#error You must use C++ compiler, or you need filename with '.cpp' suffix
#endif

#if defined(_INC_CONIO) || defined(_CONIO_H_)
#error can not include "conio.h" before "graphics.h"
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
#           pragma comment(lib,"graphics64.lib")
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
#           define EGE_DEPRECATE(text) _CRT_DEPRECATE_TEXT("This function is deprecated, more info visit http://tcgraphics.sourceforge.net/")
#       else
#           define EGE_DEPRECATE(text)
#       endif
#   else
#       define EGE_DEPRECATE(text) __attribute__((deprecated))
#   endif
#endif

#define EGE_GDIPLUS // ����GDIPLUS

#define SHOWCONSOLE          1 // ����ͼ��ģʽʱ����������̨����ʾ
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
//#define NAMESPACE_EGE_L    namespace ege {
//#define NAMESPACE_EGE_R    }

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

/* graphresult error return codes */
enum graphics_errors
{
    grOk                 = 0,
    grNoInitGraph        = -1,
    grNotDetected        = -2,
    grFileNotFound       = -3,
    grInvalidDriver      = -4,
    grNoLoadMem          = -5,
    grNoScanMem          = -6,
    grNoFloodMem         = -7,
    grFontNotFound       = -8,
    grNoFontMem          = -9,
    grInvalidMode        = -10,
    grError              = -11, /* generic error */
    grIOerror            = -12,
    grInvalidFont        = -13,
    grInvalidFontNum     = -14,
    grInvalidVersion     = -18,
    grException          = 0x10, /* ege error */
    grParamError         = 0x11,
    grInvalidRegion      = 0x12,
    grOutOfMemory        = 0x13,
    grNullPointer        = 0x14,
    grAllocError         = 0x15,
    grInvalidMemory      = 0xCDCDCDCD,
};

enum message_event
{
    MSG_EVENT_UP         = 0x00,
    MSG_EVENT_DOWN       = 0x01,
    MSG_EVENT_CLICK      = 0x01,
    MSG_EVENT_DBCLICK    = 0x02,
    MSG_EVENT_MOVE       = 0x04,
    MSG_EVENT_WHEEL      = 0x10,
};

enum message_mouse
{
    MSG_MOUSE_LEFT  	 = 0x01,
    MSG_MOUSE_RIGHT 	 = 0x02,
    MSG_MOUSE_MID   	 = 0x04,
};

typedef DWORD color_t;

// ��ɫ
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
    YELLOWGREEN          = EGERGB(0x9A, 0xCD, 0x32),
};

/* Fill patterns for get/set fillstyle */
enum fill_patterns
{
    EMPTY_FILL,      /* fills area in background color */
    SOLID_FILL,      /* fills area in solid fill color */
    LINE_FILL,       /* --- fill */
    LTSLASH_FILL,    /* /// fill */
    SLASH_FILL,      /* /// fill with thick lines */
    BKSLASH_FILL,    /* \\\ fill with thick lines */
    LTBKSLASH_FILL,  /* \\\ fill */
    HATCH_FILL,      /* light hatch fill */
    XHATCH_FILL,     /* heavy cross hatch fill */
    INTERLEAVE_FILL, /* interleaving line fill */
    WIDE_DOT_FILL,   /* Widely spaced dot fill */
    CLOSE_DOT_FILL,  /* Closely spaced dot fill */
    USER_FILL        /* user defined fill */
};

/* Horizontal and vertical justification for settextjustify */
enum text_just
{
    LEFT_TEXT            = 0,
    CENTER_TEXT          = 1,
    RIGHT_TEXT           = 2,

    BOTTOM_TEXT          = 0,
    /* CENTER_TEXT       = 1,  already defined above */
    TOP_TEXT             = 2
};

/* Line styles for get/setlinestyle */
enum line_styles
{
    SOLID_LINE           = PS_SOLID,
    CENTER_LINE          = PS_DASH,
    DOTTED_LINE          = PS_DOT,
    DASHED_LINE          = PS_DASHDOT,
    NULL_LINE            = PS_NULL,
    USERBIT_LINE         = PS_USERSTYLE, /* User defined line style */
};

enum key_msg_flag
{
    KEYMSG_CHAR_FLAG     = 2,
    KEYMSG_DOWN_FLAG     = 1,
    KEYMSG_UP_FLAG       = 1,

    KEYMSG_CHAR          = 0x40000,
    KEYMSG_DOWN          = 0x10000,
    KEYMSG_UP            = 0x20000,
    KEYMSG_FIRSTDOWN     = 0x80000,
};

enum music_state_flag
{
    MUSIC_MODE_NOT_OPEN  = 0x0,
    MUSIC_MODE_NOT_READY = 0x20C,
    MUSIC_MODE_PAUSE     = 0x211,
    MUSIC_MODE_PLAY      = 0x20E,
    MUSIC_MODE_STOP      = 0x20D,
    MUSIC_MODE_OPEN      = 0x212,
    MUSIC_MODE_SEEK      = 0x210,
};

enum initmode_flag
{
    INIT_DEFAULT         = 0x0,
    INIT_NOBORDER        = 0x1,
    INIT_CHILD           = 0x2,
    INIT_TOPMOST         = 0x4,
    INIT_RENDERMANUAL    = 0x8,
    INIT_NOFORCEEXIT     = 0x10,
    INIT_UNICODE         = 0x20,
    INIT_HIDE            = 0x40,
    INIT_WITHLOGO        = 0x100,
    INIT_ANIMATION       = INIT_DEFAULT | INIT_RENDERMANUAL | INIT_NOFORCEEXIT,
};

enum rendermode_e
{
    RENDER_AUTO,
    RENDER_MANUAL,
};

typedef enum key_code_e
{
    key_mouse_l         = 0x01,
    key_mouse_r         = 0x02,
    key_mouse_m         = 0x04,
    key_back            = 0x08,
    key_tab             = 0x09,
    key_enter           = 0x0d,
    key_shift           = 0x10,
    key_control         = 0x11,
    key_menu            = 0x12,
    key_pause           = 0x13,
    key_capslock        = 0x14,
    key_esc             = 0x1b,
    key_space           = 0x20,

    key_pageup          = 0x21,
    key_pagedown        = 0x22,
    key_home            = 0x23,
    key_end             = 0x24,

    key_left            = 0x25,
    key_up              = 0x26,
    key_right           = 0x27,
    key_down            = 0x28,

    key_print           = 0x2a,
    key_snapshot        = 0x2c,
    key_insert          = 0x2d,
    key_delete          = 0x2e,

    key_0               = 0x30,
    key_1               = 0x31,
    key_2               = 0x32,
    key_3               = 0x33,
    key_4               = 0x34,
    key_5               = 0x35,
    key_6               = 0x36,
    key_7               = 0x37,
    key_8               = 0x38,
    key_9               = 0x39,

    key_A               = 0x41,
    key_B               = 0x42,
    key_C               = 0x43,
    key_D               = 0x44,
    key_E               = 0x45,
    key_F               = 0x46,
    key_G               = 0x47,
    key_H               = 0x48,
    key_I               = 0x49,
    key_J               = 0x4a,
    key_K               = 0x4b,
    key_L               = 0x4c,
    key_M               = 0x4d,
    key_N               = 0x4e,
    key_O               = 0x4f,
    key_P               = 0x50,
    key_Q               = 0x51,
    key_R               = 0x52,
    key_S               = 0x53,
    key_T               = 0x54,
    key_U               = 0x55,
    key_V               = 0x56,
    key_W               = 0x57,
    key_X               = 0x58,
    key_Y               = 0x59,
    key_Z               = 0x5a,
    key_win_l           = 0x5b,
    key_win_r           = 0x5c,

    key_sleep           = 0x5f,

    key_num0            = 0x60,
    key_num1            = 0x61,
    key_num2            = 0x62,
    key_num3            = 0x63,
    key_num4            = 0x64,
    key_num5            = 0x65,
    key_num6            = 0x66,
    key_num7            = 0x67,
    key_num8            = 0x68,
    key_num9            = 0x69,

    key_multiply        = 0x6a,
    key_add             = 0x6b,
    key_separator       = 0x6c,
    key_subtract        = 0x6d,
    key_decimal         = 0x6e,
    key_divide          = 0x6f,

    key_f1              = 0x70,
    key_f2              = 0x71,
    key_f3              = 0x72,
    key_f4              = 0x73,
    key_f5              = 0x74,
    key_f6              = 0x75,
    key_f7              = 0x76,
    key_f8              = 0x77,
    key_f9              = 0x78,
    key_f10             = 0x79,
    key_f11             = 0x7a,
    key_f12             = 0x7b,

    key_numlock         = 0x90,
    key_scrolllock      = 0x91,

    key_shift_l         = 0xa0,
    key_shift_r         = 0xa1,
    key_control_l       = 0xa2,
    key_control_r       = 0xa3,
    key_menu_l          = 0xa4,
    key_menu_r          = 0xa5,

    key_semicolon       = 0xba,
    key_plus            = 0xbb,
    key_comma           = 0xbc,
    key_minus           = 0xbd,
    key_period          = 0xbe,
    key_slash           = 0xbf,
    key_tilde           = 0xc0,
    key_lbrace          = 0xdb,
    key_backslash       = 0xdc,
    key_rbrace          = 0xdd,
    key_quote           = 0xde,

    key_ime_process     = 0xe5,
} key_code_e;

typedef enum key_msg_e
{
    key_msg_down        = 1,
    key_msg_up          = 2,
    key_msg_char        = 4,
} key_msg_e;

typedef enum key_flag_e
{
    key_flag_shift      = 0x100,
    key_flag_ctrl       = 0x200,
    key_flag_first_down = 0x80000,
} key_flag_e;

typedef enum mouse_msg_e
{
    mouse_msg_down      = 0x10,
    mouse_msg_up        = 0x20,
    mouse_msg_move      = 0x40,
    mouse_msg_wheel     = 0x80,
} mouse_msg_e;

typedef enum mouse_flag_e
{
    mouse_flag_left     = 1,
    mouse_flag_right    = 2,
    mouse_flag_mid      = 4,
    mouse_flag_shift    = 0x100,
    mouse_flag_ctrl     = 0x200,
} mouse_flag_e;

struct viewporttype
{
    int left;
    int top;
    int right;
    int bottom;
    int clipflag;
};

struct textsettingstype
{
    int font;
    int direction;
    int charsize;
    int horiz;
    int vert;
};

struct linestyletype
{
    int             linestyle;
    unsigned short  upattern;
    int             thickness;
};

typedef struct key_msg
{
    int             key;
    key_msg_e       msg;
    unsigned int    flags;
} key_msg;

typedef struct mouse_msg
{
    int             x;
    int             y;
    mouse_msg_e     msg;
    unsigned int    flags;
    int             wheel;
    bool is_left()  {return (flags & mouse_flag_left)  != 0;}
    bool is_right() {return (flags & mouse_flag_right) != 0;}
    bool is_mid()   {return (flags & mouse_flag_mid)   != 0;}
    bool is_down()  {return msg == mouse_msg_down;}
    bool is_up()    {return msg == mouse_msg_up;}
    bool is_move()  {return msg == mouse_msg_move;}
    bool is_wheel() {return msg == mouse_msg_wheel;}
} mouse_msg;

typedef struct ege_point
{
    float x;
    float y;
} ege_point;

typedef struct ege_rect
{
    float x;
    float y;
    float w;
    float h;
} ege_rect;

typedef struct ege_colpoint
{
    float   x;
    float   y;
    color_t color;
} ege_colpoint;

// �����Ϣ
struct MOUSEMSG
{
    UINT  uMsg;      // ��ǰ�����Ϣ
    bool  mkCtrl;    // Ctrl ���Ƿ���
    bool  mkShift;   // Shift ���Ƿ���
    bool  mkLButton; // �������Ƿ���
    bool  mkMButton; // ����м��Ƿ���
    bool  mkRButton; // ����Ҽ��Ƿ���
    short x;         // ��ǰ��� x ����
    short y;         // ��ǰ��� y ����
    short wheel;     // �����ֹ���ֵ(120Ϊ����)
};

struct msg_createwindow
{
    HANDLE  hEvent;
    HWND    hwnd;
    LPCWSTR classname;
    DWORD   style;
    DWORD   exstyle;
    size_t  id;
    LPVOID  param;
};


//�������
#define MUSIC_ERROR  0xFFFFFFFF


typedef void (CALLBACK_PROC)();
typedef int (__stdcall MSG_KEY_PROC  )(void*, unsigned, int);
typedef int (__stdcall MSG_MOUSE_PROC)(void*, unsigned, int, int, int);
typedef CALLBACK_PROC       * LPCALLBACK_PROC;
typedef MSG_KEY_PROC        * LPMSG_KEY_PROC;
typedef MSG_MOUSE_PROC      * LPMSG_MOUSE_PROC;

/*
ע�⣺���º�����ע�ͺ��'###'�ĺ�����ʾδʵ��
*/

struct VECTOR3D;

// 3d ���㸨������
void EGEAPI rotate_point3d_x(VECTOR3D* pt, float r); // ���ȣ����ֶ���
void EGEAPI rotate_point3d_y(VECTOR3D* pt, float r);
void EGEAPI rotate_point3d_z(VECTOR3D* pt, float r);

struct VECTOR3D
{
    float x, y, z;

    VECTOR3D()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    VECTOR3D(float _x, float _y)
    {
        x = _x;
        y = _y;
        z = 0;
    }

    VECTOR3D(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    VECTOR3D& operator=(const VECTOR3D& _fp)
    {
        x = _fp.x;
        y = _fp.y;
        z = _fp.z;
        return *this;
    }

    VECTOR3D& operator+=(const VECTOR3D& _fp);
    VECTOR3D& operator-=(const VECTOR3D& _fp);
    VECTOR3D  operator+ (const VECTOR3D& _fp) const;
    VECTOR3D  operator- (const VECTOR3D& _fp) const;
    VECTOR3D& operator*=(float f);                   // ����
    VECTOR3D  operator* (float f) const;             // ����
    float     operator* (const VECTOR3D& _fp) const; // ���
    VECTOR3D  operator& (const VECTOR3D& _fp) const; // ���
    VECTOR3D& operator&=(const VECTOR3D& _fp);       // ���
    float     GetModule() const;

    float GetSqrModule() const { return float(x * x + y * y + z * z); }

    VECTOR3D& SetModule(float m)
    {
        float t  = m / GetModule();
        *this   *= t;
        return *this;
    }

    VECTOR3D& Rotate(float rad, const VECTOR3D& v); // ����������ת�����ֶ���radΪ����

    VECTOR3D& Rotate(float rad, float x, float y, float z)
    {
        VECTOR3D v(x, y, z);
        return Rotate(rad, v);
    }
    // ��s��e֮��ļн�ȷ����ת
    VECTOR3D&    Rotate  (const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f));
    static float GetAngel(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f));
};

class IMAGE;
typedef IMAGE *PIMAGE;
typedef const IMAGE *PCIMAGE;

// ��ͼ������غ���

// ���ó�ʼ��ģʽ��mode=0Ϊ��ͨ��1Ϊ�ޱ߿򴰿ڣ�xy�ǳ�ʼ��������
void EGEAPI setinitmode(int mode, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT);
int  EGEAPI getinitmode();
void EGEAPI initgraph(int Width, int Height, int Flag); // ��ʼ��ͼ�λ���
// Debug ������Ĭ�ϲ���ʾ LOGO��Release ģʽ��Ĭ����ʾ��
#if !defined(NDEBUG) || defined(DEBUG) || defined(_DEBUG)
inline void EGEAPI initgraph(int Width, int Height)
{
    initgraph(Width, Height, getinitmode());
}
#else
inline void EGEAPI initgraph(int Width, int Height)
{
    initgraph(Width, Height, getinitmode() | INIT_WITHLOGO);
}
#endif
void EGEAPI initgraph(int* gdriver, int* gmode, const char* path); // ���� Borland C++ 3.1 �����أ�ֻʹ�� 640x480x24bit
void EGEAPI closegraph();                                          // �ر�ͼ�λ���
bool EGEAPI is_run();                                              // �ж�UI�Ƿ��˳�
void EGEAPI setcaption(LPCSTR caption);
void EGEAPI setcaption(LPCWSTR caption);
void EGEAPI seticon(int icon_id);
int  EGEAPI attachHWND(HWND hWnd);

void EGEAPI movewindow(int x, int y, bool redraw = true); // �ƶ�����
void EGEAPI resizewindow(int width, int height);          // ���贰�ڳߴ�

void EGEAPI setrendermode(rendermode_e mode);

// ��ͼ��������
PIMAGE      gettarget();
int         settarget(PIMAGE pbuf);             // �� NULL ���ô���Ϊ��ͼĿ��

void EGEAPI cleardevice(PIMAGE pimg = NULL);    // ����

void EGEAPI getviewport(int *pleft, int *ptop, int *pright, int *pbottom, int *pclip = 0, PCIMAGE pimg = NULL); // ��ȡ��ͼ��Ϣ
void EGEAPI setviewport(int left, int top, int right, int bottom, int clip = 1, PIMAGE pimg = NULL);           // ������ͼ
void EGEAPI clearviewport(PIMAGE pimg = NULL);                                                                 // �����ͼ

EGE_DEPRECATE(setactivepage)
void EGEAPI setactivepage(int page); // ���õ�ǰ��ͼҳ������ͼ����Ĭ�ϵ�������壬��Χ0-1��Ĭ��Ϊ0
EGE_DEPRECATE(setvisualpage)
void EGEAPI setvisualpage(int page); // ���õ�ǰ��ʾҳ������������ʾ�������ϵ�ҳ����Χ0-1��Ĭ��Ϊ0
EGE_DEPRECATE(swappage)
void EGEAPI swappage();
void EGEAPI window_getviewport(struct viewporttype * viewport);
void EGEAPI window_getviewport(int* left, int* top, int* right, int* bottom);
void EGEAPI window_setviewport(int  left, int  top, int  right, int  bottom);

// ��ͼ����
EGE_DEPRECATE(getlinestyle)
void EGEAPI getlinestyle(int *plinestyle, unsigned short *pupattern = NULL, int *pthickness = NULL, PCIMAGE pimg = NULL); // ��ȡ��ǰ����
void EGEAPI setlinestyle(int linestyle, unsigned short upattern = 0, int thickness = 1, PIMAGE pimg = NULL);          // ���õ�ǰ����
void EGEAPI setlinewidth(float width, PIMAGE pimg = NULL);          // ���õ�ǰ�߿�
//EGE_DEPRECATE(getfillstyle)
//void getfillstyle(color_t *pcolor, int *ppattern = NULL, PIMAGE pimg = NULL);           // ��ȡ������� ###
EGE_DEPRECATE(setfillstyle)
void EGEAPI setfillstyle(int pattern, color_t color, PIMAGE pimg = NULL);  // �����������

void EGEAPI setwritemode(int mode, PIMAGE pimg = NULL);         // ���û�ͼλ����ģʽ

//void EGEAPI graphdefaults(PIMAGE pimg = NULL);                  // �������л�ͼ����ΪĬ��ֵ ###

// ɫ�ʺ���
color_t EGEAPI getcolor(PCIMAGE pimg = NULL);                     // ��ȡ��ǰ��ͼǰ��ɫ
color_t EGEAPI getfillcolor(PCIMAGE pimg = NULL);                 // ��ȡ��ǰ��ͼ���ɫ
color_t EGEAPI getbkcolor(PCIMAGE pimg = NULL);                   // ��ȡ��ǰ��ͼ����ɫ
void    EGEAPI setcolor(color_t color, PIMAGE pimg = NULL);       // ���õ�ǰ��ͼǰ��ɫ
void    EGEAPI setfillcolor(color_t color, PIMAGE pimg = NULL);   // ���õ�ǰ��ͼ���ɫ
void    EGEAPI setbkcolor(color_t color, PIMAGE pimg = NULL);     // ���õ�ǰ��ͼ����ɫ�����ò�������ɫ�����滻��
void    EGEAPI setbkcolor_f(color_t color, PIMAGE pimg = NULL);   // �������õ�ǰ��ͼ����ɫ��ֻ���ò��滭��
void    EGEAPI setfontbkcolor(color_t color, PIMAGE pimg = NULL); // ���õ�ǰ���ֱ���ɫ
void    EGEAPI setbkmode(int iBkMode, PIMAGE pimg = NULL);        // ���ñ������ģʽ(0=OPAQUE, 1=TRANSPARENT)

// ���ݺ�
#define RGBtoGRAY   rgb2gray
#define RGBtoHSL    rgb2hsl
#define RGBtoHSV    rgb2hsv
#define HSLtoRGB    hsl2rgb
#define HSVtoRGB    hsv2rgb

// ��ɫģ��ת������
color_t     EGEAPI rgb2gray(color_t rgb);
void        EGEAPI rgb2hsl(color_t rgb, float *H, float *S, float *L);
void        EGEAPI rgb2hsv(color_t rgb, float *H, float *S, float *V);
color_t     EGEAPI hsl2rgb(float H, float S, float L);
color_t     EGEAPI hsv2rgb(float H, float S, float V);

// �� Alpha ͨ�������ɫ���� src ��Ϊ����ɫ��dst ��Ϊǰ��ɫ���л��
color_t     EGEAPI alphablend(color_t dst, color_t src); // ʹ�� EGEGET_A(src) ��Ϊ Alpha ֵ
color_t     EGEAPI alphablend(color_t dst, color_t src, unsigned char alpha);


// ������ͼ����

color_t EGEAPI getpixel  (int x, int y, PCIMAGE pimg = NULL);               // ��ȡ�����ɫ
void    EGEAPI putpixel  (int x, int y, color_t color, PIMAGE pimg = NULL); // ����
color_t EGEAPI getpixel_f(int x, int y, PCIMAGE pimg = NULL);               // ��ȡ�����ɫ
void    EGEAPI putpixel_f(int x, int y, color_t color, PIMAGE pimg = NULL); // �������껭��
void    EGEAPI putpixels  (int nPoint, int* pPoints, PIMAGE pimg = NULL);   // ��������
void    EGEAPI putpixels_f(int nPoint, int* pPoints, PIMAGE pimg = NULL);   // ��������

void    EGEAPI putpixel_withalpha  (int x, int y, color_t color, PIMAGE pimg = NULL); // ��͸���Ȼ���
void    EGEAPI putpixel_withalpha_f(int x, int y, color_t color, PIMAGE pimg = NULL); // ��͸���Ⱦ������껭��
void    EGEAPI putpixel_savealpha  (int x, int y, color_t color, PIMAGE pimg = NULL); // �������ص����ɫ��ͬʱ����ԭ��alphaֵ��
void    EGEAPI putpixel_savealpha_f(int x, int y, color_t color, PIMAGE pimg = NULL); // �������ص����ɫ��ͬʱ����ԭ��alphaֵ��ʹ�þ������꣩

void    EGEAPI moveto(int x, int y, PIMAGE pimg = NULL);                      // �ƶ���ǰ��(��������)
void    EGEAPI moverel(int dx, int dy, PIMAGE pimg = NULL);                   // �ƶ���ǰ��(�������)

void    EGEAPI line(int x1, int y1, int x2, int y2, PIMAGE pimg = NULL);      // ����
void    EGEAPI linerel(int dx, int dy, PIMAGE pimg = NULL);                   // ����(���������)
void    EGEAPI lineto(int x, int y, PIMAGE pimg = NULL);                      // ����(����������)
void    EGEAPI line_f(float x1, float y1, float x2, float y2, PIMAGE pimg = NULL);  // ����
void    EGEAPI linerel_f(float dx, float dy, PIMAGE pimg = NULL);                   // ����(���������)
void    EGEAPI lineto_f(float x, float y, PIMAGE pimg = NULL);                      // ����(����������)


void EGEAPI rectangle(int left, int top, int right, int bottom, PIMAGE pimg = NULL);   // ������

//void EGEAPI getarccoords(int *px, int *py, int *pxstart, int *pystart, int *pxend, int *pyend, PIMAGE pimg = NULL);    // ��ȡԲ��������Ϣ ###
void EGEAPI arc(int x, int y, int stangle, int endangle, int radius, PIMAGE pimg = NULL);                  // ��Բ��
void EGEAPI circle(int x, int y, int radius, PIMAGE pimg = NULL);                                          // ��Բ
void EGEAPI pieslice(int x, int y, int stangle, int endangle, int radius, PIMAGE pimg = NULL);             // �����Բ����
void EGEAPI ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius, PIMAGE pimg = NULL);// ����Բ����
void EGEAPI fillellipse(int x, int y, int xradius, int yradius, PIMAGE pimg = NULL);                       // �������Բ
void EGEAPI sector(int x, int y, int stangle, int endangle, int xradius, int yradius, PIMAGE pimg = NULL); // �������Բ����
void EGEAPI roundrect(int left, int top, int right, int bottom, int xradius, int yradius, PIMAGE pimg = NULL); //��Բ�Ǿ���
void EGEAPI arcf(float x, float y, float stangle, float endangle, float radius, PIMAGE pimg = NULL);                    // ��Բ��
void EGEAPI circlef(float x, float y, float radius, PIMAGE pimg = NULL);                                                // ��Բ
void EGEAPI pieslicef(float x, float y, float stangle, float endangle, float radius, PIMAGE pimg = NULL);               // �����Բ����
void EGEAPI ellipsef(float x, float y, float stangle, float endangle, float xradius, float yradius, PIMAGE pimg = NULL);// ����Բ����
void EGEAPI fillellipsef(float x, float y, float xradius, float yradius, PIMAGE pimg = NULL);                           // �������Բ
void EGEAPI sectorf(float x, float y, float stangle, float endangle, float xradius, float yradius, PIMAGE pimg = NULL); // �������Բ����

//�����Բ
inline void EGEAPI fillcircle(int x, int y, int radius, PIMAGE pimg = NULL)
{
    fillellipse(x,y,radius,radius,pimg);
}
//�����Բ
inline void EGEAPI fillcirclef(float x, float y, float radius, PIMAGE pimg = NULL)
{
    fillellipsef(x,y,radius,radius,pimg);
}

void EGEAPI bar(int left, int top, int right, int bottom, PIMAGE pimg = NULL);                             // ���ޱ߿�������
void EGEAPI bar3d(int left, int top, int right, int bottom, int depth, int topflag, PIMAGE pimg = NULL);   // ���б߿���ά������

void EGEAPI fillrect(int left, int top, int right, int bottom, PIMAGE pimg = NULL);                      //��������
void EGEAPI fillroundrect(int left, int top, int right, int bottom, int xradius, int yradius, PIMAGE pimg = NULL); //�����Բ�Ǿ���

void EGEAPI drawpoly(int numpoints, const int *polypoints, PIMAGE pimg = NULL);     // �������
void EGEAPI drawlines(int numlines, const int *polypoints, PIMAGE pimg = NULL);     // �������������ߣ���չ������
void EGEAPI drawbezier(int numpoints, const int *polypoints, PIMAGE pimg = NULL);   // ��bezier���ߣ���չ������
void EGEAPI fillpoly(int numpoints, const int *polypoints, PIMAGE pimg = NULL);     // �����Ķ����
void EGEAPI fillpoly_gradient(int numpoints, const ege_colpoint* polypoints, PIMAGE pimg = NULL); // ���������Ķ����
void EGEAPI floodfill(int x, int y, int border, PIMAGE pimg = NULL);                // ���߽���ɫ�������
void EGEAPI floodfillsurface(int x, int y, color_t areacolor, PIMAGE pimg = NULL);  // ����ʼ����ɫ�������

#ifdef EGE_GDIPLUS
// �߼���ͼ��������AA��
// ege new_api
void EGEAPI ege_enable_aa(bool enable, PIMAGE pimg = NULL);

void EGEAPI ege_line(float x1, float y1, float x2, float y2, PIMAGE pimg = NULL);
void EGEAPI ege_drawpoly(int numpoints, ege_point* polypoints, PIMAGE pimg = NULL);
void EGEAPI ege_drawcurve(int numpoints, ege_point* polypoints, PIMAGE pimg = NULL);
void EGEAPI ege_rectangle(float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_ellipse(float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_pie(float x, float y, float w, float h, float stangle, float sweepAngle, PIMAGE pimg = NULL);

void EGEAPI ege_arc(float x, float y, float w, float h, float stangle, float sweepAngle, PIMAGE pimg = NULL);
void EGEAPI ege_bezier(int numpoints, ege_point* polypoints, PIMAGE pimg = NULL);

void EGEAPI ege_fillpoly(int numpoints, ege_point* polypoints, PIMAGE pimg = NULL);
void EGEAPI ege_fillrect(float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_fillellipse(float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_fillpie(float x, float y, float w, float h, float stangle, float sweepAngle, PIMAGE pimg = NULL);

void EGEAPI ege_setpattern_none(PIMAGE pimg = NULL);
void EGEAPI ege_setpattern_lineargradient(float x1, float y1, color_t c1, float x2, float y2, color_t c2, PIMAGE pimg = NULL);
void EGEAPI ege_setpattern_pathgradient(ege_point center, color_t centercolor,
    int count, ege_point* points, int colcount, color_t* pointscolor, PIMAGE pimg = NULL);
void EGEAPI ege_setpattern_ellipsegradient(ege_point center, color_t centercolor,
    float x, float y, float w, float h, color_t color, PIMAGE pimg = NULL);
void EGEAPI ege_setpattern_texture(PIMAGE srcimg, float x, float y, float w, float h, PIMAGE pimg = NULL);

void EGEAPI ege_drawtext(LPCSTR  textstring, float x, float y, PIMAGE pimg = NULL);
void EGEAPI ege_drawtext(LPCWSTR textstring, float x, float y, PIMAGE pimg = NULL);

void EGEAPI ege_setalpha(int alpha, PIMAGE pimg = NULL);
void EGEAPI ege_gentexture(bool gen, PIMAGE pimg = NULL);
void EGEAPI ege_puttexture(PCIMAGE srcimg, float x, float y, float w, float h, PIMAGE pimg = NULL);
void EGEAPI ege_puttexture(PCIMAGE srcimg, ege_rect dest, PIMAGE pimg = NULL);
void EGEAPI ege_puttexture(PCIMAGE srcimg, ege_rect dest, ege_rect src, PIMAGE pimg = NULL);

//draw image
void EGEAPI ege_drawimage(PCIMAGE srcimg,int dstX, int dstY,PIMAGE pimg = NULL);
void EGEAPI ege_drawimage(PCIMAGE srcimg,int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, int srcWidth, int srcHeight,PIMAGE pimg = NULL);

// matrix for transformation
typedef struct ege_transform_matrix
{
    float m11;
    float m12;
    float m21;
    float m22;
    float m31;
    float m32;
} ege_transform_matrix;

//transforms
void EGEAPI ege_transform_rotate(float angle,PIMAGE pimg = NULL);
void EGEAPI ege_transform_translate(float x,float y,PIMAGE pimg = NULL);
void EGEAPI ege_transform_scale(float scale_x, float scale_y,PIMAGE pimg = NULL);
void EGEAPI ege_transform_reset(PIMAGE pimg = NULL);
void EGEAPI ege_get_transform(ege_transform_matrix* pmatrix, PIMAGE pimg = NULL);
void EGEAPI ege_set_transform(ege_transform_matrix* const pmatrix, PIMAGE pimg = NULL);
ege_point EGEAPI ege_transform_calc(ege_point p, PIMAGE pimg = NULL); // Calculate transformed coordination of p;
ege_point EGEAPI ege_transform_calc(float x, float y, PIMAGE pimg = NULL); // Calculate transformed coordination of point(x,y);

//
#endif

//We don't support VC 6
#ifndef EGE_COMPILERINFO_VC6
//Console
BOOL init_console(); // Initialize the console
void clear_console(); // clear the console
BOOL show_console(); //Show the Console
BOOL hide_console(); // hide the console
BOOL close_console(); //close the console and restore the old STD I/O
#endif

//int  EGEAPI Begin2d();
//void EGEAPI EndRender();

//ʱ�亯�������º��������ڶ��߳���ʹ�ã�ֻ�ܸ���ͼ���̵߳��ã�
void EGEAPI ege_sleep(long ms);     // �����ӳ�ms����
void EGEAPI delay(long ms);         // �����ӳ�ms����
void EGEAPI delay_ms(long ms);      // ƽ���ӳ�ms����
void EGEAPI delay_fps(int fps);    // ƽ���ӳ�1000/fps���룬�����ȶ�֡�ʿ���
void EGEAPI delay_fps(long fps);    // ƽ���ӳ�1000/fps���룬�����ȶ�֡�ʿ���
void EGEAPI delay_fps(double fps);  // ƽ���ӳ�1000/fps���룬�����ȶ�֡�ʿ���
void EGEAPI delay_jfps(int fps);   // ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡
void EGEAPI delay_jfps(long fps);   // ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡
void EGEAPI delay_jfps(double fps); // ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡
// ���º������Զ��߳���ʹ�ã���ͼ��(worker)�̵߳�sleepʹ�����
void EGEAPI api_sleep(long dwMilliseconds);
double EGEAPI fclock(); // ��ȡ����Ϊ��λ�ĸ���ʱ�䣬ֻ���ڼ�ʱ�ã�����0.01�룬

// ������غ���
void EGEAPI outtext(LPCSTR  textstring, PIMAGE pimg = NULL);                   // �ڵ�ǰλ���������
void EGEAPI outtext(LPCWSTR textstring, PIMAGE pimg = NULL);                   // �ڵ�ǰλ���������
void EGEAPI outtext(CHAR  c, PIMAGE pimg = NULL);                              // �ڵ�ǰλ������ַ�
void EGEAPI outtext(WCHAR c, PIMAGE pimg = NULL);                              // �ڵ�ǰλ������ַ�
void EGEAPI outtextxy(int x, int y, LPCSTR  textstring, PIMAGE pimg = NULL);   // ��ָ��λ���������
void EGEAPI outtextxy(int x, int y, LPCWSTR textstring, PIMAGE pimg = NULL);   // ��ָ��λ���������
void EGEAPI outtextxy(int x, int y, CHAR c, PIMAGE pimg = NULL);               // ��ָ��λ������ַ�
void EGEAPI outtextxy(int x, int y, WCHAR c, PIMAGE pimg = NULL);              // ��ָ��λ������ַ�
void EGEAPI outtextrect(int x, int y, int w, int h, LPCSTR  textstring, PIMAGE pimg = NULL); // ��ָ�����η�Χ�������
void EGEAPI outtextrect(int x, int y, int w, int h, LPCWSTR textstring, PIMAGE pimg = NULL); // ��ָ�����η�Χ�������
void EGEAPI xyprintf(int x, int y, LPCSTR  fmt, ...); // ��ָ��λ�������ʽ���ַ�����ָ����ͼĿ�����settarget
void EGEAPI xyprintf(int x, int y, LPCWSTR fmt, ...); // ��ָ��λ�������ʽ���ַ�����ָ����ͼĿ�����settarget
void EGEAPI rectprintf(int x, int y, int w, int h, LPCSTR  fmt, ...); // ��ָ�����������ʽ���ַ�����ָ����ͼĿ�����settarget
void EGEAPI rectprintf(int x, int y, int w, int h, LPCWSTR fmt, ...); // ��ָ�����������ʽ���ַ�����ָ����ͼĿ�����settarget
int  EGEAPI textwidth(LPCSTR  textstring, PIMAGE pimg = NULL);                 // ��ȡ�ַ���ռ�õ����ؿ�
int  EGEAPI textwidth(LPCWSTR textstring, PIMAGE pimg = NULL);                 // ��ȡ�ַ���ռ�õ����ؿ�
int  EGEAPI textwidth(CHAR  c, PIMAGE pimg = NULL);
int  EGEAPI textwidth(WCHAR c, PIMAGE pimg = NULL);
int  EGEAPI textheight(LPCSTR  textstring, PIMAGE pimg = NULL);                // ��ȡ�ַ���ռ�õ����ظ�
int  EGEAPI textheight(LPCWSTR textstring, PIMAGE pimg = NULL);                // ��ȡ�ַ���ռ�õ����ظ�
int  EGEAPI textheight(CHAR  c, PIMAGE pimg = NULL);
int  EGEAPI textheight(WCHAR c, PIMAGE pimg = NULL);
void EGEAPI settextjustify(int horiz, int vert, PIMAGE pimg = NULL);

// ���õ�ǰ������ʽ(�������)
//      nHeight: �ַ���ƽ���߶ȣ�
//      nWidth: �ַ���ƽ�����(0 ��ʾ����Ӧ)��
//      lpszFace: �������ƣ�
//      nEscapement: �ַ�������д�Ƕ�(��λ 0.1 ��)��
//      nOrientation: ÿ���ַ�����д�Ƕ�(��λ 0.1 ��)��
//      nWeight: �ַ��ıʻ���ϸ(0 ��ʾĬ�ϴ�ϸ)��
//      bItalic: �Ƿ�б�壻
//      bUnderline: �Ƿ��»��ߣ�
//      bStrikeOut: �Ƿ�ɾ���ߣ�
//      fbCharSet: ָ���ַ�����
//      fbOutPrecision: ָ�����ֵ�������ȣ�
//      fbClipPrecision: ָ�����ֵļ������ȣ�
//      fbQuality: ָ�����ֵ����������
//      fbPitchAndFamily: ָ���Գ��淽ʽ�������������ϵ�С�
void EGEAPI setfont(int nHeight, int nWidth, LPCSTR lpszFace,  PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCWSTR lpszFace, PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation,
                    int nWeight, int bItalic, int bUnderline, int bStrikeOut, PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation,
                    int nWeight, int bItalic, int bUnderline, int bStrikeOut, PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCSTR lpszFace,  int nEscapement, int nOrientation,
                    int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet,
                    BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily, PIMAGE pimg = NULL);
void EGEAPI setfont(int nHeight, int nWidth, LPCWSTR lpszFace, int nEscapement, int nOrientation,
                    int nWeight, int bItalic, int bUnderline, int bStrikeOut, BYTE fbCharSet,
                    BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily, PIMAGE pimg = NULL);
void EGEAPI setfont(const LOGFONTA *font, PIMAGE pimg = NULL); // ���õ�ǰ������ʽ
void EGEAPI setfont(const LOGFONTW *font, PIMAGE pimg = NULL); // ���õ�ǰ������ʽ
void EGEAPI getfont(LOGFONTA *font, PCIMAGE pimg = NULL);      // ��ȡ��ǰ������ʽ
void EGEAPI getfont(LOGFONTW *font, PCIMAGE pimg = NULL);      // ��ȡ��ǰ������ʽ


//ͼƬ����
#define getmaxx getwidth
#define getmaxy getheight

int EGEAPI getwidth(PCIMAGE pimg = NULL);          // ��ȡͼƬ���
int EGEAPI getheight(PCIMAGE pimg = NULL);         // ��ȡͼƬ�߶�
int EGEAPI getx(PCIMAGE pimg = NULL);              // ��ȡ��ǰ x ����
int EGEAPI gety(PCIMAGE pimg = NULL);              // ��ȡ��ǰ y ����

PIMAGE         EGEAPI newimage();                      // ���� PIMAGE
PIMAGE         EGEAPI newimage(int width, int height); // ���� PIMAGE
void           EGEAPI delimage(PCIMAGE pImg);          // ɾ�� PIMAGE
color_t*       EGEAPI getbuffer(PIMAGE pImg);
const color_t* EGEAPI getbuffer(PCIMAGE pImg);

int  EGEAPI resize_f(PIMAGE pDstImg, int width, int height);  //����ߴ磬������䱳��ɫ
int  EGEAPI resize(PIMAGE pDstImg, int width, int height); //����ߴ磬����䱳��ɫ
int  EGEAPI getimage(PIMAGE pDstImg, int srcX, int srcY, int srcWidth, int srcHeight);                             // ����Ļ��ȡͼ��
int  EGEAPI getimage(PIMAGE pDstImg, PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);            // ����һ�� PIMAGE �����л�ȡͼ��
int  EGEAPI getimage(PIMAGE pDstImg, LPCSTR  pImgFile, int zoomWidth = 0, int zoomHeight = 0);                     // ��ͼƬ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
int  EGEAPI getimage(PIMAGE pDstImg, LPCWSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);                     // ��ͼƬ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
int  EGEAPI getimage(PIMAGE pDstImg, LPCSTR  pResType, LPCSTR  pResName, int zoomWidth = 0, int zoomHeight = 0);   // ����Դ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
int  EGEAPI getimage(PIMAGE pDstImg, LPCWSTR pResType, LPCWSTR pResName, int zoomWidth = 0, int zoomHeight = 0);   // ����Դ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
void EGEAPI putimage(int dstX, int dstY, PCIMAGE pSrcImg, DWORD dwRop = SRCCOPY);                                  // ����ͼ����Ļ
void EGEAPI putimage(int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);                                // ����ͼ����Ļ(ָ�����)
void EGEAPI putimage(int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY);   // ����ͼ����Ļ(ָ��Դ��ߺ�Ŀ���߽�������)
void EGEAPI putimage(PIMAGE pDstImg, int dstX, int dstY, PCIMAGE pSrcImg, DWORD dwRop = SRCCOPY);                                                                 // ����ͼ����һͼ����
void EGEAPI putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);                // ����ͼ����һͼ����(ָ�����)
void EGEAPI putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY);   // ����ͼ����һͼ����(ָ��Դ��ߺ�Ŀ���߽�������)
int  EGEAPI saveimage(PCIMAGE pimg, LPCSTR  filename);
int  EGEAPI saveimage(PCIMAGE pimg, LPCWSTR filename);
int  EGEAPI savepng(PCIMAGE pimg, LPCSTR  filename, int bAlpha = 0);
int  EGEAPI savepng(PCIMAGE pimg, LPCWSTR filename, int bAlpha = 0);
int  EGEAPI getimage_pngfile(PIMAGE pimg, LPCSTR  filename);
int  EGEAPI getimage_pngfile(PIMAGE pimg, LPCWSTR filename);

int EGEAPI putimage_transparent(
    PIMAGE imgdest,         // handle to dest
    PCIMAGE imgsrc,         // handle to source
    int nXOriginDest,       // x-coord of destination upper-left corner
    int nYOriginDest,       // y-coord of destination upper-left corner
    color_t crTransparent,  // color to make transparent
    int nXOriginSrc = 0,    // x-coord of source upper-left corner
    int nYOriginSrc = 0,    // y-coord of source upper-left corner
    int nWidthSrc = 0,      // width of source rectangle
    int nHeightSrc = 0      // height of source rectangle
);
int EGEAPI putimage_alphablend(
    PIMAGE imgdest,         // handle to dest
    PCIMAGE imgsrc,         // handle to source
    int nXOriginDest,       // x-coord of destination upper-left corner
    int nYOriginDest,       // y-coord of destination upper-left corner
    unsigned char alpha,    // alpha
    int nXOriginSrc = 0,    // x-coord of source upper-left corner
    int nYOriginSrc = 0,    // y-coord of source upper-left corner
    int nWidthSrc = 0,      // width of source rectangle
    int nHeightSrc = 0      // height of source rectangle
);
int EGEAPI putimage_alphatransparent(
    PIMAGE imgdest,         // handle to dest
    PCIMAGE imgsrc,         // handle to source
    int nXOriginDest,       // x-coord of destination upper-left corner
    int nYOriginDest,       // y-coord of destination upper-left corner
    color_t crTransparent,  // color to make transparent
    unsigned char alpha,    // alpha
    int nXOriginSrc = 0,    // x-coord of source upper-left corner
    int nYOriginSrc = 0,    // y-coord of source upper-left corner
    int nWidthSrc = 0,      // width of source rectangle
    int nHeightSrc = 0      // height of source rectangle
);
int EGEAPI putimage_withalpha(
    PIMAGE imgdest,         // handle to dest
    PCIMAGE imgsrc,         // handle to source
    int nXOriginDest,       // x-coord of destination upper-left corner
    int nYOriginDest,       // y-coord of destination upper-left corner
    int nXOriginSrc = 0,    // x-coord of source upper-left corner
    int nYOriginSrc = 0,    // y-coord of source upper-left corner
    int nWidthSrc = 0,      // width of source rectangle
    int nHeightSrc = 0      // height of source rectangle
);
int EGEAPI putimage_withalpha(
    PIMAGE imgdest,         // handle to dest
    PCIMAGE imgsrc,         // handle to source
    int nXOriginDest,       // x-coord of destination upper-left corner
    int nYOriginDest,       // y-coord of destination upper-left corner
    int nWidthDest,         // width of destination rectangle
    int nHeightDest,        // height of destination rectangle
    int nXOriginSrc,        // x-coord of source upper-left corner
    int nYOriginSrc,        // y-coord of source upper-left corner
    int nWidthSrc,          // width of source rectangle
    int nHeightSrc          // height of source rectangle
);
int EGEAPI putimage_alphafilter(
    PIMAGE imgdest,         // handle to dest
    PCIMAGE imgsrc,         // handle to source
    int nXOriginDest,       // x-coord of destination upper-left corner
    int nYOriginDest,       // y-coord of destination upper-left corner
    PCIMAGE imgalpha,       // alpha
    int nXOriginSrc,        // x-coord of source upper-left corner
    int nYOriginSrc,        // y-coord of source upper-left corner
    int nWidthSrc,          // width of source rectangle
    int nHeightSrc          // height of source rectangle
);
int EGEAPI imagefilter_blurring (
    PIMAGE imgdest,         // handle to dest
    int intensity,
    int alpha,
    int nXOriginDest = 0,
    int nYOriginDest = 0,
    int nWidthDest = 0,
    int nHeightDest = 0
);
int EGEAPI putimage_rotate(
    PIMAGE imgdest,
    PCIMAGE imgtexture,
    int nXOriginDest,
    int nYOriginDest,
    float centerx,
    float centery,
    float radian,
    int btransparent = 0,            // transparent (1) or not (0)
    int alpha = -1,                  // in range[0, 256], alpha== -1 means no alpha
    int smooth = 0
);

int EGEAPI putimage_rotatezoom(
    PIMAGE imgdest,
    PCIMAGE imgtexture,
    int nXOriginDest,
    int nYOriginDest,
    float centerx,
    float centery,
    float radian,
    float zoom,
    int btransparent = 0,            // transparent (1) or not (0)
    int alpha = -1,                  // in range[0, 256], alpha== -1 means no alpha
    int smooth = 0
);

int EGEAPI putimage_rotatetransparent(
    PIMAGE imgdest,/* handle to dest, NULL means the SCREEN  */
    PCIMAGE imgsrc,/* handle to source */
    int xCenterDest,/* x-coord of rotation center in dest */
    int yCenterDest,/* y-coord of rotation center in dest */
    int xCenterSrc, /* x-coord of rotation center in source */
    int yCenterSrc,/* y-coord of rotation center in source */
    color_t crTransparent, /* color to make transparent */
    float radian, /* rotation angle (clockwise, in radian) */
    float zoom=1.0 /* zoom factor */
);

int EGEAPI putimage_rotatetransparent(
    PIMAGE imgdest,/* handle to dest, NULL means the SCREEN */
    PCIMAGE imgsrc,/* handle to source */
    int xCenterDest,/* x-coord of rotation center in dest */
    int yCenterDest,/* y-coord of rotation center in dest */
    int xOriginSrc, /* x-coord of source upper-left corner */
    int yOriginSrc, /* y-coord of source upper-left corner */
    int widthSrc, /* width of source rectangle */
    int heightSrc, /* height of source rectangle */
    int xCenterSrc, /* x-coord of rotation center in source */
    int yCenterSrc,/* y-coord of rotation center in source */
    color_t crTransparent, /* color to make transparent */
    float radian, /* rotation angle (clockwise, in radian) */
    float zoom=1.0 /* zoom factor */
);

// ��������

HWND        EGEAPI getHWnd();         // ��ȡ��ͼ���ھ��
HINSTANCE   EGEAPI getHInstance();
HDC         EGEAPI getHDC(PCIMAGE pImg = NULL);

PVOID       EGEAPI getProcfunc();
long        EGEAPI getGraphicsVer();   // ��ȡ��ǰ�汾
float       EGEAPI getfps();           // ��ȡ��ǰ֡��

//�������
void            EGEAPI randomize();
unsigned int    EGEAPI random(unsigned int n);
double          EGEAPI randomf();

//�߼����뺯��
// title �Ի�����⣬ text �Ի�����ʾ���֣� buf�����������ݵ��ַ���ָ�룬 lenָ��buf����󳤶ȣ�Ҳͬʱ�������������ݳ���
int EGEAPI inputbox_getline(LPCSTR  title, LPCSTR  text, LPSTR  buf, int len);  //�����Ի������û����룬��ǰ����������ͣ�����ط�0��ʾ������Ч��0Ϊ��Ч
int EGEAPI inputbox_getline(LPCWSTR title, LPCWSTR text, LPWSTR buf, int len);  //�����Ի������û����룬��ǰ����������ͣ�����ط�0��ʾ������Ч��0Ϊ��Ч


//���̴�����
int     EGEAPI kbmsg();
key_msg EGEAPI getkey();
EGE_DEPRECATE(getchEx)
int     EGEAPI getchEx(int flag);
EGE_DEPRECATE(kbhitEx)
int     EGEAPI kbhitEx(int flag);
int     EGEAPI keystate(int key);       // ��ü���Ϊkey�ļ�����key_code_e���Ƿ��£����keyʹ����갴���ļ��룬���õ�������״̬
void    EGEAPI flushkey();              // ��ռ�����Ϣ������

#if !defined(_INC_CONIO) && !defined(_CONIO_H_)
#define _INC_CONIO
#define _CONIO_H_
int EGEAPI getch();
int EGEAPI kbhit();
#else
#define getch getchEx
#define kbhit kbhitEx
#endif

//��괦����
int         EGEAPI mousemsg();                  // ����Ƿ���������Ϣ
mouse_msg   EGEAPI getmouse();                  // ��ȡһ�������Ϣ�����û�У��͵ȴ�
EGE_DEPRECATE(GetMouseMsg)
MOUSEMSG    EGEAPI GetMouseMsg();               // �����Ƽ�ʹ�õĺ�������ȡһ�������Ϣ�����û�У��͵ȴ�

void        EGEAPI flushmouse();                // ��������Ϣ������
int         EGEAPI showmouse(int bShow);        // �����Ƿ���ʾ���
int         EGEAPI mousepos(int *x, int *y); // ��ȡ��ǰ���λ��

/*
callback function define as:
int __stdcall on_msg_key(void* param, unsigned msg, int key);
msg: see 'enum message_event'
key: keycode
return zero means process this message, otherwise means pass it and then process with 'getkey' function
*/
//int message_addkeyhandler(void* param, LPMSG_KEY_PROC func);        //���ü��̻ص�����
/*
callback function define as:
int __stdcall on_msg_mouse(void* param, unsigned msg, int key, int x, int y);
msg: see 'enum message_event'
key: see 'enum message_mouse', if msg==MSG_EVENT_WHELL, key is a int number that indicates the distance the wheel is rotated, expressed in multiples or divisions of WHEEL_DELTA, which is 120.
x,y: current mouse (x, y)
return zero means process this message, otherwise means pass it and then process with 'GetMouseMsg' function
*/
//int message_addmousehandler(void* param, LPMSG_MOUSE_PROC func);    //�������ص�����
int EGEAPI SetCloseHandler(LPCALLBACK_PROC func);

class MUSIC
{
public:
    MUSIC();
    virtual ~MUSIC();

    operator HWND() const { return (HWND)m_dwCallBack; }

public:
    int IsOpen() { return (m_DID != MUSIC_ERROR) ? 1 : 0; }

    DWORD OpenFile(LPCSTR filepath);
    DWORD OpenFile(LPCWSTR filepath);
    DWORD Play(DWORD dwFrom = MUSIC_ERROR, DWORD dwTo = MUSIC_ERROR);
    DWORD Pause();
    DWORD Seek(DWORD dwTo); // ����λ�ö�λ����λΪms
    DWORD SetVolume(float value);
    DWORD Close();
    DWORD Stop();
    DWORD GetPosition();
    DWORD GetLength();
    // ���º���GetPlayStatus�ķ���ֵΪ����֮һ�����忴��׺����
    // MUSIC_MODE_NOT_OPEN   //û����ȷ��
    // MUSIC_MODE_NOT_READY  //�豸û׼���� ������ʹ�ã�
    // MUSIC_MODE_PAUSE  //��ͣ��
    // MUSIC_MODE_PLAY   //���ڲ���
    // MUSIC_MODE_STOP   //�ɹ��򿪺󣬻��߲����������״̬
    // MUSIC_MODE_OPEN   //���� ������ʹ�ã�
    // MUSIC_MODE_SEEK   //��λ�� ������ʹ�ã�
    DWORD GetPlayStatus();

private:
    DWORD m_DID;
    PVOID m_dwCallBack;
};

/* ѹ������ */
/* ѹ��ʱdest������Ҫ��֤��С��СΪsourceLen * 1.001 + 16 */
/* ����compress/compress2ǰ��*destLen������ֵ����ʾdest������������С������ʱ���ֵ��ʾʵ�ʴ�С */
/* compress2 ��level ��0-9��0��ѹ����9���ѹ����compress����ʹ��Ĭ��ֵ6 */
int           EGEAPI ege_compress  (void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen);
int           EGEAPI ege_compress2 (void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen, int level);
int           EGEAPI ege_uncompress(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen);
unsigned long EGEAPI ege_uncompress_size(const void *source, unsigned long sourceLen); /* ����0��ʾ����������ʾ��С */

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
