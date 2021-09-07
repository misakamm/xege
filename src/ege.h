/*********************************************************
* EGE (Easy Graphics Engine)
* FileName      ege.h
* HomePage1     http://misakamm.github.com/xege
* HomePage2     http://misakamm.bitbucket.org/index.htm
* teiba1        http://tieba.baidu.com/f?kw=ege
* teiba2        http://tieba.baidu.com/f?kw=ege%C4%EF
* Blog:         http://misakamm.com
* E-Mail:       mailto:misakamm[at gmail com]
*
* FileName: ege.h
* 在 VC 下模拟 Borland BGI 绘图库，实现简单的绘图之余，扩展了较复杂的绘图能力
*
* 包含并使用本库时，不要包含conio.h头文件
* 这些头文件不应共存，否则可能会编译错误，
* 或者getch被conio.h内的覆盖（由包含次序决定），请注意
* 如需共存，请使用多文件分开包含的模式使用，
* 即不能一个cpp同时包含，但可以分开包含
* 使用本库，必须用C++编译，可支持的编译器：
* VC6/VC2008/VC2010/VC2012/VC2013/MinGW3.4.5/MinGW4.7.1/MinGW4.8.1
*********************************************************/

/****************************************************************************
** 注意事项：
* ★如果需要显示控制台窗口，请在包含本文件的前面加一行define SHOW_CONSOLE
* ★调用Sleep这个API时，或者调用delay，实际均会转化为调用delay_ms，如必需调用API请使用api_sleep
* ★delay_ms(0)能自行判断有没有更新的必要，连续多次但不大量的调用并不会产生帧率的影响
* ★调用delay_ms, delay_fps, getch, getkey, getmouse 时，窗口内容可能会更新，这些函数相当于内置了delay_ms(0)，
*   如果你只需要更新窗口，而不想等待，可以用delay_ms(0)。注意delay只延时而不更新窗口
* ★合理地使用delay_ms/delay_fps函数，可以减少你的程序占用的CPU，否则一个都没有调用同时也没有getch/getmouse的话，程序将占满一个CPU的时间
****************************************************************************/

#ifndef EGE_H
#define EGE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#ifndef __cplusplus
#error You must use C++ compiler, or you need filename with '.cpp' suffix
#endif

#if defined(_INC_CONIO) || defined(_CONIO_H_)
#error can not include "conio.h" before "graphics.h"
#endif

#if defined(_MSC_VER)
#	pragma warning(disable: 4355)
#	ifndef _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#		define _ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH
#	endif
#	ifndef _ALLOW_RUNTIME_LIBRARY_MISMATCH
#		define _ALLOW_RUNTIME_LIBRARY_MISMATCH
#	endif
#endif

#if !defined(EGE_GRAPH_LIB_BUILD) && !defined(EGE_GRAPH_NO_LIB)
#	ifdef _MSC_VER
#		pragma comment(lib,"gdiplus.lib")
#		ifdef _WIN64 // 64 bit libs
#			pragma comment(lib,"graphics64.lib")
#		else   // 32 bit libs
#			pragma comment(lib,"graphics.lib")
#		endif
#	endif
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
#	define SetWindowLongPtrW   SetWindowLongW
#	define GetWindowLongPtrW   GetWindowLongW
#	define GWLP_USERDATA       GWL_USERDATA
#	define GWLP_WNDPROC        GWL_WNDPROC
#endif

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

#ifndef EGE_CDECL
#	if __STDC__
#		define EGE_CDECL  __cdecl
#	else
#		define EGE_CDECL  __cdecl
#	endif
#endif

#ifdef _MSC_VER
#	if defined(_WIN64)
#		define EGEAPI
#	else
#		define EGEAPI EGE_CDECL
#	endif
#else
#	if defined(__WORDSIZE)
#		if __WORDSIZE > 32
#			define EGEAPI
#		else
#			define EGEAPI EGE_CDECL
#		endif
#	else
#		define EGEAPI
#	endif
#endif

#ifndef EGE_DEPRECATE
#	ifdef _MSC_VER
#		ifdef _CRT_DEPRECATE_TEXT
#			define EGE_DEPRECATE(text) _CRT_DEPRECATE_TEXT("This function is deprecated, more info visit http://tcgraphics.sourceforge.net/")
#		else
#			define EGE_DEPRECATE(text)
#		endif
#	else
#		define EGE_DEPRECATE(text) __attribute__((deprecated))
#	endif
#endif

#define EGE_GDIPLUS     //启用GDIPLUS

#define SHOWCONSOLE             1       // 进入图形模式时，保留控制台的显示
#define EGERGBA(r, g, b, a)     ((::ege::color_t)( ((r)<<16) | ((g)<<8) | (b) | ((a)<<24) ))
#define EGERGB(r, g, b)         EGERGBA(r, g, b, 0xFF)
#define EGEARGB(a, r, g, b)     EGERGBA(r, g, b, a)
#define EGEACOLOR(a, color)     ((::ege::color_t)( ((color) & 0xFFFFFF) | ((a)<<24) ))
#define EGECOLORA(color, a)     EGEACOLOR(a, color)
#define EGEGET_R(c)             ( ((c)>>16) & 0xFF )
#define EGEGET_G(c)             ( ((c)>> 8) & 0xFF )
#define EGEGET_B(c)             ( ((c)) & 0xFF )
#define EGEGET_A(c)             ( ((c)>>24) & 0xFF )
#define EGEGRAY(gray)           EGERGB(gray, gray, gray)
#define EGEGRAYA(gray, a)       EGERGBA(gray, gray, gray, a)
#define EGEAGRAY(a, gray)       EGEGRAYA(gray, a)
//#define NAMESPACE_EGE_L         namespace ege {
//#define NAMESPACE_EGE_R         }

namespace ege {

const double PI = 3.1415926535897932384626;

enum graphics_drivers {     /* define graphics drivers */
	DETECT,         /* requests autodetection */
	CGA, MCGA, EGA, EGA64, EGAMONO, IBM8514,/* 1 - 6 */
	HERCMONO, ATT400, VGA, PC3270,          /* 7 - 10 */
	TRUECOLOR, TRUECOLORSIZE,
	CURRENT_DRIVER = -1
};

enum graphics_modes { /* graphics modes for each driver */
	CGAC0       = 0,  /* 320x200 palette 0; 1 page  */
	CGAC1       = 1,  /* 320x200 palette 1; 1 page  */
	CGAC2       = 2,  /* 320x200 palette 2: 1 page  */
	CGAC3       = 3,  /* 320x200 palette 3; 1 page  */
	CGAHI       = 4,  /* 640x200 1 page             */
	MCGAC0      = 0,  /* 320x200 palette 0; 1 page  */
	MCGAC1      = 1,  /* 320x200 palette 1; 1 page  */
	MCGAC2      = 2,  /* 320x200 palette 2; 1 page  */
	MCGAC3      = 3,  /* 320x200 palette 3; 1 page  */
	MCGAMED     = 4,  /* 640x200 1 page             */
	MCGAHI      = 5,  /* 640x480 1 page             */
	EGALO       = 0,  /* 640x200 16 color 4 pages   */
	EGAHI       = 1,  /* 640x350 16 color 2 pages   */
	EGA64LO     = 0,  /* 640x200 16 color 1 page    */
	EGA64HI     = 1,  /* 640x350 4 color  1 page    */
	EGAMONOHI   = 0,  /* 640x350 64K on card, 1 page - 256K on card, 4 pages */
	HERCMONOHI  = 0,  /* 720x348 2 pages            */
	ATT400C0    = 0,  /* 320x200 palette 0; 1 page  */
	ATT400C1    = 1,  /* 320x200 palette 1; 1 page  */
	ATT400C2    = 2,  /* 320x200 palette 2; 1 page  */
	ATT400C3    = 3,  /* 320x200 palette 3; 1 page  */
	ATT400MED   = 4,  /* 640x200 1 page             */
	ATT400HI    = 5,  /* 640x400 1 page             */
	VGALO       = 0,  /* 640x200 16 color 4 pages   */
	VGAMED      = 1,  /* 640x350 16 color 2 pages   */
	VGAHI       = 2,  /* 640x480 16 color 1 page    */
	PC3270HI    = 0,  /* 720x350 1 page             */
	IBM8514LO   = 0,  /* 640x480 256 colors         */
	IBM8514HI   = 1   /*1024x768 256 colors         */
};

enum graphics_errors {      /* graphresult error return codes */
	grOk                =   0,
	grNoInitGraph       =  -1,
	grNotDetected       =  -2,
	grFileNotFound      =  -3,
	grInvalidDriver     =  -4,
	grNoLoadMem         =  -5,
	grNoScanMem         =  -6,
	grNoFloodMem        =  -7,
	grFontNotFound      =  -8,
	grNoFontMem         =  -9,
	grInvalidMode       = -10,
	grError             = -11,   /* generic error */
	grIOerror           = -12,
	grInvalidFont       = -13,
	grInvalidFontNum    = -14,
	grInvalidVersion    = -18,
	grException         = 0x10,  /* ege error */
	grParamError        = 0x11,
	grInvalidRegion     = 0x12,
	grOutOfMemory       = 0x13,
	grNullPointer       = 0x14,
	grAllocError        = 0x15,
	grInvalidMemory     = 0xCDCDCDCD,
};

enum message_event {
	MSG_EVENT_UP            = 0x00,
	MSG_EVENT_DOWN          = 0x01,
	MSG_EVENT_CLICK         = 0x01,
	MSG_EVENT_DBCLICK       = 0x02,
	MSG_EVENT_MOVE          = 0x04,
	MSG_EVENT_WHEEL         = 0x10,
};

enum message_mouse {
	MSG_MOUSE_LEFT      = 0x01,
	MSG_MOUSE_RIGHT     = 0x02,
	MSG_MOUSE_MID       = 0x04,
};

typedef DWORD color_t;
// 颜色
enum COLORS {
	ALICEBLUE              = EGERGB(0xF0, 0xF8, 0xFF),
	ANTIQUEWHITE           = EGERGB(0xFA, 0xEB, 0xD7),
	AQUA                   = EGERGB(0x00, 0xFF, 0xFF),
	AQUAMARINE             = EGERGB(0x7F, 0xFF, 0xD4),
	AZURE                  = EGERGB(0xF0, 0xFF, 0xFF),
	BEIGE                  = EGERGB(0xF5, 0xF5, 0xDC),
	BISQUE                 = EGERGB(0xFF, 0xE4, 0xC4),
	BLACK                  = EGERGB(0x00, 0x00, 0x00),
	BLANCHEDALMOND         = EGERGB(0xFF, 0xEB, 0xCD),
	BLUE                   = EGERGB(0x00, 0x00, 0xFF),
	BLUEVIOLET             = EGERGB(0x8A, 0x2B, 0xE2),
	BROWN                  = EGERGB(0xA5, 0x2A, 0x2A),
	BURLYWOOD              = EGERGB(0xDE, 0xB8, 0x87),
	CADETBLUE              = EGERGB(0x5F, 0x9E, 0xA0),
	CHARTREUSE             = EGERGB(0x7F, 0xFF, 0x00),
	CHOCOLATE              = EGERGB(0xD2, 0x69, 0x1E),
	CORAL                  = EGERGB(0xFF, 0x7F, 0x50),
	CORNFLOWERBLUE         = EGERGB(0x64, 0x95, 0xED),
	CORNSILK               = EGERGB(0xFF, 0xF8, 0xDC),
	CRIMSON                = EGERGB(0xDC, 0x14, 0x3C),
	CYAN                   = EGERGB(0x00, 0xFF, 0xFF),
	DARKBLUE               = EGERGB(0x00, 0x00, 0x8B),
	DARKCYAN               = EGERGB(0x00, 0x8B, 0x8B),
	DARKGOLDENROD          = EGERGB(0xB8, 0x86, 0x0B),
	DARKGRAY               = EGERGB(0xA9, 0xA9, 0xA9),
	DARKGREEN              = EGERGB(0x00, 0x64, 0x00),
	DARKKHAKI              = EGERGB(0xBD, 0xB7, 0x6B),
	DARKMAGENTA            = EGERGB(0x8B, 0x00, 0x8B),
	DARKOLIVEGREEN         = EGERGB(0x55, 0x6B, 0x2F),
	DARKORANGE             = EGERGB(0xFF, 0x8C, 0x00),
	DARKORCHID             = EGERGB(0x99, 0x32, 0xCC),
	DARKRED                = EGERGB(0x8B, 0x00, 0x00),
	DARKSALMON             = EGERGB(0xE9, 0x96, 0x7A),
	DARKSEAGREEN           = EGERGB(0x8F, 0xBC, 0x8F),
	DARKSLATEBLUE          = EGERGB(0x48, 0x3D, 0x8B),
	DARKSLATEGRAY          = EGERGB(0x2F, 0x4F, 0x4F),
	DARKTURQUOISE          = EGERGB(0x00, 0xCE, 0xD1),
	DARKVIOLET             = EGERGB(0x94, 0x00, 0xD3),
	DEEPPINK               = EGERGB(0xFF, 0x14, 0x93),
	DEEPSKYBLUE            = EGERGB(0x00, 0xBF, 0xFF),
	DIMGRAY                = EGERGB(0x69, 0x69, 0x69),
	DODGERBLUE             = EGERGB(0x1E, 0x90, 0xFF),
	FIREBRICK              = EGERGB(0xB2, 0x22, 0x22),
	FLORALWHITE            = EGERGB(0xFF, 0xFA, 0xF0),
	FORESTGREEN            = EGERGB(0x22, 0x8B, 0x22),
	FUCHSIA                = EGERGB(0xFF, 0x00, 0xFF),
	GAINSBORO              = EGERGB(0xDC, 0xDC, 0xDC),
	GHOSTWHITE             = EGERGB(0xF8, 0xF8, 0xFF),
	GOLD                   = EGERGB(0xFF, 0xD7, 0x00),
	GOLDENROD              = EGERGB(0xDA, 0xA5, 0x20),
	GRAY                   = EGERGB(0x80, 0x80, 0x80),
	GREEN                  = EGERGB(0x00, 0x80, 0x00),
	GREENYELLOW            = EGERGB(0xAD, 0xFF, 0x2F),
	HONEYDEW               = EGERGB(0xF0, 0xFF, 0xF0),
	HOTPINK                = EGERGB(0xFF, 0x69, 0xB4),
	INDIANRED              = EGERGB(0xCD, 0x5C, 0x5C),
	INDIGO                 = EGERGB(0x4B, 0x00, 0x82),
	IVORY                  = EGERGB(0xFF, 0xFF, 0xF0),
	KHAKI                  = EGERGB(0xF0, 0xE6, 0x8C),
	LAVENDER               = EGERGB(0xE6, 0xE6, 0xFA),
	LAVENDERBLUSH          = EGERGB(0xFF, 0xF0, 0xF5),
	LAWNGREEN              = EGERGB(0x7C, 0xFC, 0x00),
	LEMONCHIFFON           = EGERGB(0xFF, 0xFA, 0xCD),
	LIGHTBLUE              = EGERGB(0xAD, 0xD8, 0xE6),
	LIGHTCORAL             = EGERGB(0xF0, 0x80, 0x80),
	LIGHTCYAN              = EGERGB(0xE0, 0xFF, 0xFF),
	LIGHTGOLDENRODYELLOW   = EGERGB(0xFA, 0xFA, 0xD2),
	LIGHTGRAY              = EGERGB(0xD3, 0xD3, 0xD3),
	LIGHTGREEN             = EGERGB(0x90, 0xEE, 0x90),
	LIGHTPINK              = EGERGB(0xFF, 0xB6, 0xC1),
	LIGHTSALMON            = EGERGB(0xFF, 0xA0, 0x7A),
	LIGHTSEAGREEN          = EGERGB(0x20, 0xB2, 0xAA),
	LIGHTSKYBLUE           = EGERGB(0x87, 0xCE, 0xFA),
	LIGHTSLATEGRAY         = EGERGB(0x77, 0x88, 0x99),
	LIGHTSTEELBLUE         = EGERGB(0xB0, 0xC4, 0xDE),
	LIGHTYELLOW            = EGERGB(0xFF, 0xFF, 0xE0),
	LIGHTRED               = EGERGB(0xFC, 0x54, 0x54),
	LIGHTMAGENTA           = EGERGB(0xFC, 0x54, 0xFC),
	LIME                   = EGERGB(0x00, 0xFF, 0x00),
	LIMEGREEN              = EGERGB(0x32, 0xCD, 0x32),
	LINEN                  = EGERGB(0xFA, 0xF0, 0xE6),
	MAGENTA                = EGERGB(0xFF, 0x00, 0xFF),
	MAROON                 = EGERGB(0x80, 0x00, 0x00),
	MEDIUMAQUAMARINE       = EGERGB(0x66, 0xCD, 0xAA),
	MEDIUMBLUE             = EGERGB(0x00, 0x00, 0xCD),
	MEDIUMORCHID           = EGERGB(0xBA, 0x55, 0xD3),
	MEDIUMPURPLE           = EGERGB(0x93, 0x70, 0xDB),
	MEDIUMSEAGREEN         = EGERGB(0x3C, 0xB3, 0x71),
	MEDIUMSLATEBLUE        = EGERGB(0x7B, 0x68, 0xEE),
	MEDIUMSPRINGGREEN      = EGERGB(0x00, 0xFA, 0x9A),
	MEDIUMTURQUOISE        = EGERGB(0x48, 0xD1, 0xCC),
	MEDIUMVIOLETRED        = EGERGB(0xC7, 0x15, 0x85),
	MIDNIGHTBLUE           = EGERGB(0x19, 0x19, 0x70),
	MINTCREAM              = EGERGB(0xF5, 0xFF, 0xFA),
	MISTYROSE              = EGERGB(0xFF, 0xE4, 0xE1),
	MOCCASIN               = EGERGB(0xFF, 0xE4, 0xB5),
	NAVAJOWHITE            = EGERGB(0xFF, 0xDE, 0xAD),
	NAVY                   = EGERGB(0x00, 0x00, 0x80),
	OLDLACE                = EGERGB(0xFD, 0xF5, 0xE6),
	OLIVE                  = EGERGB(0x80, 0x80, 0x00),
	OLIVEDRAB              = EGERGB(0x6B, 0x8E, 0x23),
	ORANGE                 = EGERGB(0xFF, 0xA5, 0x00),
	ORANGERED              = EGERGB(0xFF, 0x45, 0x00),
	ORCHID                 = EGERGB(0xDA, 0x70, 0xD6),
	PALEGOLDENROD          = EGERGB(0xEE, 0xE8, 0xAA),
	PALEGREEN              = EGERGB(0x98, 0xFB, 0x98),
	PALETURQUOISE          = EGERGB(0xAF, 0xEE, 0xEE),
	PALEVIOLETRED          = EGERGB(0xDB, 0x70, 0x93),
	PAPAYAWHIP             = EGERGB(0xFF, 0xEF, 0xD5),
	PEACHPUFF              = EGERGB(0xFF, 0xDA, 0xB9),
	PERU                   = EGERGB(0xCD, 0x85, 0x3F),
	PINK                   = EGERGB(0xFF, 0xC0, 0xCB),
	PLUM                   = EGERGB(0xDD, 0xA0, 0xDD),
	POWDERBLUE             = EGERGB(0xB0, 0xE0, 0xE6),
	PURPLE                 = EGERGB(0x80, 0x00, 0x80),
	RED                    = EGERGB(0xFF, 0x00, 0x00),
	ROSYBROWN              = EGERGB(0xBC, 0x8F, 0x8F),
	ROYALBLUE              = EGERGB(0x41, 0x69, 0xE1),
	SADDLEBROWN            = EGERGB(0x8B, 0x45, 0x13),
	SALMON                 = EGERGB(0xFA, 0x80, 0x72),
	SANDYBROWN             = EGERGB(0xF4, 0xA4, 0x60),
	SEAGREEN               = EGERGB(0x2E, 0x8B, 0x57),
	SEASHELL               = EGERGB(0xFF, 0xF5, 0xEE),
	SIENNA                 = EGERGB(0xA0, 0x52, 0x2D),
	SILVER                 = EGERGB(0xC0, 0xC0, 0xC0),
	SKYBLUE                = EGERGB(0x87, 0xCE, 0xEB),
	SLATEBLUE              = EGERGB(0x6A, 0x5A, 0xCD),
	SLATEGRAY              = EGERGB(0x70, 0x80, 0x90),
	SNOW                   = EGERGB(0xFF, 0xFA, 0xFA),
	SPRINGGREEN            = EGERGB(0x00, 0xFF, 0x7F),
	STEELBLUE              = EGERGB(0x46, 0x82, 0xB4),
	TAN                    = EGERGB(0xD2, 0xB4, 0x8C),
	TEAL                   = EGERGB(0x00, 0x80, 0x80),
	THISTLE                = EGERGB(0xD8, 0xBF, 0xD8),
	TOMATO                 = EGERGB(0xFF, 0x63, 0x47),
	TURQUOISE              = EGERGB(0x40, 0xE0, 0xD0),
	VIOLET                 = EGERGB(0xEE, 0x82, 0xEE),
	WHEAT                  = EGERGB(0xF5, 0xDE, 0xB3),
	WHITE                  = EGERGB(0xFF, 0xFF, 0xFF),
	WHITESMOKE             = EGERGB(0xF5, 0xF5, 0xF5),
	YELLOW                 = EGERGB(0xFF, 0xFF, 0x00),
	YELLOWGREEN            = EGERGB(0x9A, 0xCD, 0x32),
};

// 填充模式
enum fill_patterns {/* Fill patterns for get/setfillstyle */
	EMPTY_FILL,     /* fills area in background color */
	SOLID_FILL,     /* fills area in solid fill color */
	LINE_FILL,      /* --- fill */
	LTSLASH_FILL,   /* /// fill */
	SLASH_FILL,     /* /// fill with thick lines */
	BKSLASH_FILL,   /* \\\ fill with thick lines */
	LTBKSLASH_FILL, /* \\\ fill */
	HATCH_FILL,     /* light hatch fill */
	XHATCH_FILL,    /* heavy cross hatch fill */
	INTERLEAVE_FILL,/* interleaving line fill */
	WIDE_DOT_FILL,  /* Widely spaced dot fill */
	CLOSE_DOT_FILL, /* Closely spaced dot fill */
	USER_FILL       /* user defined fill */
};

enum text_just {        /* Horizontal and vertical justification
						for settextjustify */
	LEFT_TEXT   = 0,
	CENTER_TEXT = 1,
	RIGHT_TEXT  = 2,

	BOTTOM_TEXT = 0,
	/* CENTER_TEXT = 1,  already defined above */
	TOP_TEXT    = 2
};

enum line_styles {      /* Line styles for get/setlinestyle */
	SOLID_LINE      = PS_SOLID,
	CENTER_LINE     = PS_DASH,
	DOTTED_LINE     = PS_DOT,
	DASHED_LINE     = PS_DASHDOT,
	NULL_LINE       = PS_NULL,
	USERBIT_LINE    = PS_USERSTYLE,   /* User defined line style */
};

enum key_msg_flag {
	KEYMSG_CHAR_FLAG    = 2,
	KEYMSG_DOWN_FLAG    = 1,
	KEYMSG_UP_FLAG      = 1,

	KEYMSG_CHAR         = 0x40000,
	KEYMSG_DOWN         = 0x10000,
	KEYMSG_UP           = 0x20000,
	KEYMSG_FIRSTDOWN    = 0x80000,
};

enum music_state_flag {
	MUSIC_MODE_NOT_OPEN = 0x0,
	MUSIC_MODE_NOT_READY = 0x20C,
	MUSIC_MODE_PAUSE = 0x211,
	MUSIC_MODE_PLAY = 0x20E,
	MUSIC_MODE_STOP = 0x20D,
	MUSIC_MODE_OPEN = 0x212,
	MUSIC_MODE_SEEK = 0x210,
};

enum initmode_flag {
	INIT_DEFAULT        = 0x0,
	INIT_NOBORDER       = 0x1,
	INIT_CHILD          = 0x2,
	INIT_TOPMOST        = 0x4,
	INIT_RENDERMANUAL   = 0x8,
	INIT_NOFORCEEXIT    = 0x10,
	INIT_UNICODE        = 0x20,
	INIT_WITHLOGO       = 0x100,
	INIT_ANIMATION      = INIT_DEFAULT | INIT_RENDERMANUAL | INIT_NOFORCEEXIT,
};

enum rendermode_e {
	RENDER_AUTO,
	RENDER_MANUAL,
};

typedef enum key_code_e {
	key_mouse_l     = 0x01,
	key_mouse_r     = 0x02,
	key_mouse_m     = 0x04,
	key_back        = 0x08,
	key_tab         = 0x09,
	key_enter       = 0x0d,
	key_shift       = 0x10,
	key_control     = 0x11,
	key_menu        = 0x12,
	key_pause       = 0x13,
	key_capslock    = 0x14,
	key_esc         = 0x1b,
	key_space       = 0x20,

	key_pageup      = 0x21,
	key_pagedown    = 0x22,
	key_home        = 0x23,
	key_end         = 0x24,

	key_left        = 0x25,
	key_up          = 0x26,
	key_right       = 0x27,
	key_down        = 0x28,

	key_print       = 0x2a,
	key_snapshot    = 0x2c,
	key_insert      = 0x2d,
	key_delete      = 0x2e,

	key_0           = 0x30,
	key_1           = 0x31,
	key_2           = 0x32,
	key_3           = 0x33,
	key_4           = 0x34,
	key_5           = 0x35,
	key_6           = 0x36,
	key_7           = 0x37,
	key_8           = 0x38,
	key_9           = 0x39,

	key_A           = 0x41,
	key_B           = 0x42,
	key_C           = 0x43,
	key_D           = 0x44,
	key_E           = 0x45,
	key_F           = 0x46,
	key_G           = 0x47,
	key_H           = 0x48,
	key_I           = 0x49,
	key_J           = 0x4a,
	key_K           = 0x4b,
	key_L           = 0x4c,
	key_M           = 0x4d,
	key_N           = 0x4e,
	key_O           = 0x4f,
	key_P           = 0x50,
	key_Q           = 0x51,
	key_R           = 0x52,
	key_S           = 0x53,
	key_T           = 0x54,
	key_U           = 0x55,
	key_V           = 0x56,
	key_W           = 0x57,
	key_X           = 0x58,
	key_Y           = 0x59,
	key_Z           = 0x5a,
	key_win_l       = 0x5b,
	key_win_r       = 0x5c,

	key_sleep       = 0x5f,

	key_num0        = 0x60,
	key_num1        = 0x61,
	key_num2        = 0x62,
	key_num3        = 0x63,
	key_num4        = 0x64,
	key_num5        = 0x65,
	key_num6        = 0x66,
	key_num7        = 0x67,
	key_num8        = 0x68,
	key_num9        = 0x69,

	key_multiply    = 0x6a,
	key_add         = 0x6b,
	key_separator   = 0x6c,
	key_subtract    = 0x6d,
	key_decimal     = 0x6e,
	key_divide      = 0x6f,

	key_f1          = 0x70,
	key_f2          = 0x71,
	key_f3          = 0x72,
	key_f4          = 0x73,
	key_f5          = 0x74,
	key_f6          = 0x75,
	key_f7          = 0x76,
	key_f8          = 0x77,
	key_f9          = 0x78,
	key_f10         = 0x79,
	key_f11         = 0x7a,
	key_f12         = 0x7b,

	key_numlock     = 0x90,
	key_scrolllock  = 0x91,

	key_shift_l     = 0xa0,
	key_shift_r     = 0xa1,
	key_control_l   = 0xa2,
	key_control_r   = 0xa3,
	key_menu_l      = 0xa4,
	key_menu_r      = 0xa5,

	key_semicolon   = 0xba,
	key_plus        = 0xbb,
	key_comma       = 0xbc,
	key_minus       = 0xbd,
	key_period      = 0xbe,
	key_slash       = 0xbf,
	key_tilde       = 0xc0,
	key_lbrace      = 0xdb,
	key_backslash   = 0xdc,
	key_rbrace      = 0xdd,
	key_quote       = 0xde,

	key_ime_process = 0xe5,
}key_code_e;

typedef enum key_msg_e {
	key_msg_down    = 1,
	key_msg_up      = 2,
	key_msg_char    = 4,
}key_msg_e;
typedef enum key_flag_e {
	key_flag_shift  = 0x100,
	key_flag_ctrl   = 0x200,
}key_flag_e;

typedef enum mouse_msg_e {
	mouse_msg_down      = 0x10,
	mouse_msg_up        = 0x20,
	mouse_msg_move      = 0x40,
	mouse_msg_wheel     = 0x80,
}mouse_msg_e;

typedef enum mouse_flag_e {
	mouse_flag_left     = 1,
	mouse_flag_right    = 2,
	mouse_flag_mid      = 4,
	mouse_flag_shift    = 0x100,
	mouse_flag_ctrl     = 0x200,
}mouse_flag_e;

struct viewporttype {
	int left;
	int top;
	int right;
	int bottom;
	int clipflag;
};

struct textsettingstype {
	int font;
	int direction;
	int charsize;
	int horiz;
	int vert;
};

struct linestyletype {
	int linestyle;
	unsigned short upattern;
	int thickness;
};

typedef struct key_msg {
	int             key;
	key_msg_e       msg;
	unsigned int    flags;
}key_msg;

typedef struct mouse_msg {
	int             x;
	int             y;
	mouse_msg_e     msg;
	unsigned int    flags;
	int             wheel;
	bool is_left() { return (flags & mouse_flag_left) != 0; }
	bool is_right() { return (flags & mouse_flag_right) != 0; }
	bool is_mid() { return (flags & mouse_flag_mid) != 0; }
	bool is_down() { return msg == mouse_msg_down; }
	bool is_up() { return msg == mouse_msg_up; }
	bool is_move() { return msg == mouse_msg_move; }
	bool is_wheel() { return msg == mouse_msg_wheel; }
}mouse_msg;

typedef struct ege_point {
	float x;
	float y;
}ege_point;

typedef struct ege_rect {
	float x;
	float y;
	float w;
	float h;
}ege_rect;

typedef struct ege_colpoint {
	float   x;
	float   y;
	color_t color;
}ege_colpoint;

// 鼠标消息
struct MOUSEMSG {
	UINT uMsg;              // 当前鼠标消息
	bool mkCtrl;            // Ctrl 键是否按下
	bool mkShift;           // Shift 键是否按下
	bool mkLButton;         // 鼠标左键是否按下
	bool mkMButton;         // 鼠标中键是否按下
	bool mkRButton;         // 鼠标右键是否按下
	short x;                // 当前鼠标 x 坐标
	short y;                // 当前鼠标 y 坐标
	short wheel;            // 鼠标滚轮滚动值(120为基数)
};

struct msg_createwindow {
	HANDLE hEvent;
	HWND hwnd;
	LPCWSTR classname;
	DWORD style;
	DWORD exstyle;
	size_t id;
	LPVOID param;
};


//音乐类宏
#define MUSIC_ERROR  0xFFFFFFFF


typedef void (CALLBACK_PROC)();
typedef int (__stdcall MSG_KEY_PROC  )(void*, unsigned, int);
typedef int (__stdcall MSG_MOUSE_PROC)(void*, unsigned, int, int, int);
typedef CALLBACK_PROC       * LPCALLBACK_PROC;
typedef MSG_KEY_PROC        * LPMSG_KEY_PROC;
typedef MSG_MOUSE_PROC      * LPMSG_MOUSE_PROC;

/*
注意：以下函数的注释后带'###'的函数表示未实现
*/

struct VECTOR3D;

// 3d 计算辅助函数
void EGEAPI rotate_point3d_x(VECTOR3D  * pt, float r); //弧度，右手定则
void EGEAPI rotate_point3d_y(VECTOR3D  * pt, float r);
void EGEAPI rotate_point3d_z(VECTOR3D  * pt, float r);

struct VECTOR3D {
	float x, y, z;
	VECTOR3D() {
		x = 0; y = 0; z = 0;
	}
	VECTOR3D(float _x, float _y) {
		x = _x; y = _y; z = 0;
	}
	VECTOR3D(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}
	VECTOR3D& operator = (const VECTOR3D& _fp)  {
		x = _fp.x; y = _fp.y; z = _fp.z;
		return *this;
	}
	VECTOR3D& operator += (const VECTOR3D& _fp);
	VECTOR3D& operator -= (const VECTOR3D& _fp);
	VECTOR3D operator + (const VECTOR3D& _fp) const;
	VECTOR3D operator - (const VECTOR3D& _fp) const;
	VECTOR3D& operator *= (float f); //缩放
	VECTOR3D operator * (float f) const; //缩放
	float operator * (const VECTOR3D& _fp) const; //点乘
	VECTOR3D operator & (const VECTOR3D& _fp) const; //叉乘
	VECTOR3D& operator &= (const VECTOR3D& _fp); //叉乘
	float GetModule() const;
	float GetSqrModule() const {
		return float(x*x + y*y + z*z);
	}
	VECTOR3D& SetModule(float m) {
		float t = m / GetModule();
		*this *= t;
		return *this;
	}
	VECTOR3D& Rotate(float rad, const VECTOR3D& v); //绕任意轴旋转，右手定则，rad为弧度
	VECTOR3D& Rotate(float rad, float x, float y, float z) {
		VECTOR3D v(x, y, z);
		return Rotate(rad, v);
	}
	VECTOR3D& Rotate(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f)); //从s到e之间的夹角确定旋转
	static float GetAngel(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f));
};


class IMAGE;
typedef IMAGE *PIMAGE;
typedef const IMAGE *PCIMAGE;

// 绘图环境相关函数
void EGEAPI setinitmode(int mode, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT); //设置初始化模式，mode=0为普通，1为无边框窗口，xy是初始窗口坐标
int  EGEAPI getinitmode();
void EGEAPI initgraph(int Width, int Height, int Flag);        // 初始化图形环境
// Debug 配置下默认不显示 LOGO，Release 模式下默认显示。
#if !defined(NDEBUG) || defined(DEBUG) || defined(_DEBUG)
inline void EGEAPI initgraph(int Width, int Height) {
	initgraph(Width, Height, getinitmode());
}
#else
inline void EGEAPI initgraph(int Width, int Height) {
	initgraph(Width, Height, getinitmode()|INIT_WITHLOGO);
}
#endif
void EGEAPI initgraph(int* gdriver, int* gmode, const char* path);  // 兼容 Borland C++ 3.1 的重载，只使用 640x480x24bit
void EGEAPI closegraph();                                           // 关闭图形环境
bool EGEAPI is_run();                                               // 判断UI是否退出
void EGEAPI setcaption(LPCSTR  caption);
void EGEAPI setcaption(LPCWSTR caption);
void EGEAPI seticon(int icon_id);
int  EGEAPI attachHWND(HWND hWnd);

void EGEAPI movewindow(int x, int y, bool redraw = true);	//移动窗口
void EGEAPI resizewindow(int width, int height);			//重设窗口尺寸

void EGEAPI setrendermode(rendermode_e mode);

// 绘图环境设置
PIMAGE  gettarget();
int     settarget(PIMAGE pbuf); // 用 NULL 设置窗口为绘图目标

void EGEAPI cleardevice(PIMAGE pimg = NULL);                   // 清屏

void EGEAPI getviewport(int *pleft, int *ptop, int *pright, int *pbottom, int *pclip = 0, PCIMAGE pimg = NULL); // 获取视图信息
void EGEAPI setviewport(int left, int top, int right, int bottom, int clip = 1, PIMAGE pimg = NULL);           // 设置视图
void EGEAPI clearviewport(PIMAGE pimg = NULL);                                                                 // 清空视图

EGE_DEPRECATE(setactivepage)
void EGEAPI setactivepage(int page); // 设置当前绘图页，即绘图函数默认的输出缓冲，范围0-1，默认为0
EGE_DEPRECATE(setvisualpage)
void EGEAPI setvisualpage(int page); // 设置当前显示页，用于设置显示到窗口上的页，范围0-1，默认为0
EGE_DEPRECATE(swappage)
void EGEAPI swappage();
void EGEAPI window_getviewport(struct viewporttype * viewport);
void EGEAPI window_getviewport(int* left, int* top, int* right, int* bottom);
void EGEAPI window_setviewport(int  left, int  top, int  right, int  bottom);

// 绘图属性
EGE_DEPRECATE(getlinestyle)
void EGEAPI getlinestyle(int *plinestyle, unsigned short *pupattern = NULL, int *pthickness = NULL, PCIMAGE pimg = NULL); // 获取当前线形
void EGEAPI setlinestyle(int linestyle, unsigned short upattern = 0, int thickness = 1, PIMAGE pimg = NULL);          // 设置当前线形
void EGEAPI setlinewidth(float width, PIMAGE pimg = NULL);          // 设置当前线宽
//EGE_DEPRECATE(getfillstyle)
//void getfillstyle(color_t *pcolor, int *ppattern = NULL, PIMAGE pimg = NULL);           // 获取填充类型 ###
EGE_DEPRECATE(setfillstyle)
void EGEAPI setfillstyle(int pattern, color_t color, PIMAGE pimg = NULL);  // 设置填充类型

void EGEAPI setwritemode(int mode, PIMAGE pimg = NULL);         // 设置绘图位操作模式

//void EGEAPI graphdefaults(PIMAGE pimg = NULL);                  // 重置所有绘图设置为默认值 ###

// 色彩函数
color_t EGEAPI getcolor(PCIMAGE pimg = NULL);                   // 获取当前绘图前景色
color_t EGEAPI getfillcolor(PCIMAGE pimg = NULL);               // 获取当前绘图填充色
color_t EGEAPI getbkcolor(PCIMAGE pimg = NULL);                 // 获取当前绘图背景色
void EGEAPI setcolor(color_t color, PIMAGE pimg = NULL);        // 设置当前绘图前景色
void EGEAPI setfillcolor(color_t color, PIMAGE pimg = NULL);    // 设置当前绘图填充色
void EGEAPI setbkcolor(color_t color, PIMAGE pimg = NULL);      // 设置当前绘图背景色（设置并做背景色像素替换）
void EGEAPI setbkcolor_f(color_t color, PIMAGE pimg = NULL);    // 快速设置当前绘图背景色（只设置不绘画）
void EGEAPI setfontbkcolor(color_t color, PIMAGE pimg = NULL);  // 设置当前文字背景色
void EGEAPI setbkmode(int iBkMode, PIMAGE pimg = NULL);         // 设置背景混合模式(0=OPAQUE, 1=TRANSPARENT)

// 兼容宏
#define RGBtoGRAY   rgb2gray
#define RGBtoHSL    rgb2hsl
#define RGBtoHSV    rgb2hsv
#define HSLtoRGB    hsl2rgb
#define HSVtoRGB    hsv2rgb

// 颜色模型转换函数
color_t     EGEAPI rgb2gray(color_t rgb);
void        EGEAPI rgb2hsl(color_t rgb, float *H, float *S, float *L);
void        EGEAPI rgb2hsv(color_t rgb, float *H, float *S, float *V);
color_t     EGEAPI hsl2rgb(float H, float S, float L);
color_t     EGEAPI hsv2rgb(float H, float S, float V);

// 按 Alpha 通道混合颜色，将 src 作为背景色，dst 作为前景色进行混合
color_t     EGEAPI alphablend(color_t dst, color_t src); // 使用 EGEGET_A(src) 作为 Alpha 值
color_t     EGEAPI alphablend(color_t dst, color_t src, unsigned char alpha);


// 基本绘图函数

color_t     EGEAPI getpixel  (int x, int y, PCIMAGE pimg = NULL);               // 获取点的颜色
void        EGEAPI putpixel  (int x, int y, color_t color, PIMAGE pimg = NULL); // 画点
color_t     EGEAPI getpixel_f(int x, int y, PCIMAGE pimg = NULL);               // 获取点的颜色
void        EGEAPI putpixel_f(int x, int y, color_t color, PIMAGE pimg = NULL); // 绝对坐标画点
void        EGEAPI putpixels  (int nPoint, int* pPoints, PIMAGE pimg = NULL);   // 批量画点
void        EGEAPI putpixels_f(int nPoint, int* pPoints, PIMAGE pimg = NULL);   // 批量画点

void        EGEAPI putpixel_withalpha  (int x, int y, color_t color, PIMAGE pimg = NULL); // 带透明度画点
void        EGEAPI putpixel_withalpha_f(int x, int y, color_t color, PIMAGE pimg = NULL); // 带透明度绝对坐标画点
void        EGEAPI putpixel_savealpha  (int x, int y, color_t color, PIMAGE pimg = NULL); // 设置像素点的颜色（同时保留原有alpha值）
void        EGEAPI putpixel_savealpha_f(int x, int y, color_t color, PIMAGE pimg = NULL); // 设置像素点的颜色（同时保留原有alpha值，使用绝对坐标）

void EGEAPI moveto(int x, int y, PIMAGE pimg = NULL);                      // 移动当前点(绝对坐标)
void EGEAPI moverel(int dx, int dy, PIMAGE pimg = NULL);                   // 移动当前点(相对坐标)

void EGEAPI line(int x1, int y1, int x2, int y2, PIMAGE pimg = NULL);      // 画线
void EGEAPI linerel(int dx, int dy, PIMAGE pimg = NULL);                   // 画线(至相对坐标)
void EGEAPI lineto(int x, int y, PIMAGE pimg = NULL);                      // 画线(至绝对坐标)
void EGEAPI line_f(float x1, float y1, float x2, float y2, PIMAGE pimg = NULL);  // 画线
void EGEAPI linerel_f(float dx, float dy, PIMAGE pimg = NULL);                   // 画线(至相对坐标)
void EGEAPI lineto_f(float x, float y, PIMAGE pimg = NULL);                      // 画线(至绝对坐标)


void EGEAPI rectangle(int left, int top, int right, int bottom, PIMAGE pimg = NULL);   // 画矩形

//void EGEAPI getarccoords(int *px, int *py, int *pxstart, int *pystart, int *pxend, int *pyend, PIMAGE pimg = NULL);    // 获取圆弧坐标信息 ###
void EGEAPI arc(int x, int y, int stangle, int endangle, int radius, PIMAGE pimg = NULL);                  // 画圆弧
void EGEAPI circle(int x, int y, int radius, PIMAGE pimg = NULL);                                          // 画圆
void EGEAPI pieslice(int x, int y, int stangle, int endangle, int radius, PIMAGE pimg = NULL);             // 画填充圆扇形
void EGEAPI ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius, PIMAGE pimg = NULL);// 画椭圆弧线
void EGEAPI fillellipse(int x, int y, int xradius, int yradius, PIMAGE pimg = NULL);                       // 画填充椭圆
void EGEAPI sector(int x, int y, int stangle, int endangle, int xradius, int yradius, PIMAGE pimg = NULL); // 画填充椭圆扇形
void EGEAPI roundrect(int left, int top, int right, int bottom, int xradius, int yradius, PIMAGE pimg = NULL); //画圆角矩形 
void EGEAPI arcf(float x, float y, float stangle, float endangle, float radius, PIMAGE pimg = NULL);                    // 画圆弧
void EGEAPI circlef(float x, float y, float radius, PIMAGE pimg = NULL);                                                // 画圆
void EGEAPI pieslicef(float x, float y, float stangle, float endangle, float radius, PIMAGE pimg = NULL);               // 画填充圆扇形
void EGEAPI ellipsef(float x, float y, float stangle, float endangle, float xradius, float yradius, PIMAGE pimg = NULL);// 画椭圆弧线
void EGEAPI fillellipsef(float x, float y, float xradius, float yradius, PIMAGE pimg = NULL);                           // 画填充椭圆
void EGEAPI sectorf(float x, float y, float stangle, float endangle, float xradius, float yradius, PIMAGE pimg = NULL); // 画填充椭圆扇形

//画填充圆
inline void EGEAPI fillcircle(int x, int y, int radius, PIMAGE pimg = NULL) {
	fillellipse(x,y,radius,radius,pimg);
}
//画填充圆
inline void EGEAPI fillcirclef(float x, float y, float radius, PIMAGE pimg = NULL) {
	fillellipsef(x,y,radius,radius,pimg);
} 

void EGEAPI bar(int left, int top, int right, int bottom, PIMAGE pimg = NULL);                             // 画无边框填充矩形
void EGEAPI bar3d(int left, int top, int right, int bottom, int depth, int topflag, PIMAGE pimg = NULL);   // 画有边框三维填充矩形

void EGEAPI fillrect(int left, int top, int right, int bottom, PIMAGE pimg = NULL);                      //画填充矩形 
void EGEAPI fillroundrect(int left, int top, int right, int bottom, int xradius, int yradius, PIMAGE pimg = NULL); //画填充圆角矩形 

void EGEAPI drawpoly(int numpoints, const int *polypoints, PIMAGE pimg = NULL);     // 画多边形
void EGEAPI drawlines(int numlines, const int *polypoints, PIMAGE pimg = NULL);     // 画多条不连续线（扩展函数）
void EGEAPI drawbezier(int numpoints, const int *polypoints, PIMAGE pimg = NULL);   // 画bezier曲线（扩展函数）
void EGEAPI fillpoly(int numpoints, const int *polypoints, PIMAGE pimg = NULL);     // 画填充的多边形
void EGEAPI fillpoly_gradient(int numpoints, const ege_colpoint* polypoints, PIMAGE pimg = NULL); // 画渐变填充的多边形
void EGEAPI floodfill(int x, int y, int border, PIMAGE pimg = NULL);                // 按边界颜色填充区域
void EGEAPI floodfillsurface(int x, int y, color_t areacolor, PIMAGE pimg = NULL);  // 按起始点颜色填充区域

#ifdef EGE_GDIPLUS
// 高级绘图函数（带AA）
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
typedef struct ege_transform_matrix{
	float m11;
	float m12;
	float m21;
	float m22;
	float m31;
	float m32;	
}ege_transform_matrix;

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

//时间函数（以下函数不能在多线程下使用，只能给绘图主线程调用）
void EGEAPI ege_sleep(long ms);     // 至少延迟ms毫秒
void EGEAPI delay(long ms);         // 至少延迟ms毫秒
void EGEAPI delay_ms(long ms);      // 平均延迟ms毫秒
void EGEAPI delay_fps(int fps);    // 平均延迟1000/fps毫秒，用于稳定帧率控制
void EGEAPI delay_fps(long fps);    // 平均延迟1000/fps毫秒，用于稳定帧率控制
void EGEAPI delay_fps(double fps);  // 平均延迟1000/fps毫秒，用于稳定帧率控制
void EGEAPI delay_jfps(int fps);   // 平均延迟1000/fps毫秒，用于稳定逻辑帧率控制，绘图带跳帧
void EGEAPI delay_jfps(long fps);   // 平均延迟1000/fps毫秒，用于稳定逻辑帧率控制，绘图带跳帧
void EGEAPI delay_jfps(double fps); // 平均延迟1000/fps毫秒，用于稳定逻辑帧率控制，绘图带跳帧
// 以下函数可以多线程下使用，非图形(worker)线程的sleep使用这个
void EGEAPI api_sleep(long dwMilliseconds);
double EGEAPI fclock(); // 获取以秒为单位的浮点时间，只用于计时用，精度0.01秒，

// 文字相关函数
void EGEAPI outtext(LPCSTR  textstring, PIMAGE pimg = NULL);                   // 在当前位置输出文字
void EGEAPI outtext(LPCWSTR textstring, PIMAGE pimg = NULL);                   // 在当前位置输出文字
void EGEAPI outtext(CHAR  c, PIMAGE pimg = NULL);                              // 在当前位置输出字符
void EGEAPI outtext(WCHAR c, PIMAGE pimg = NULL);                              // 在当前位置输出字符
void EGEAPI outtextxy(int x, int y, LPCSTR  textstring, PIMAGE pimg = NULL);   // 在指定位置输出文字
void EGEAPI outtextxy(int x, int y, LPCWSTR textstring, PIMAGE pimg = NULL);   // 在指定位置输出文字
void EGEAPI outtextxy(int x, int y, CHAR c, PIMAGE pimg = NULL);               // 在指定位置输出字符
void EGEAPI outtextxy(int x, int y, WCHAR c, PIMAGE pimg = NULL);              // 在指定位置输出字符
void EGEAPI outtextrect(int x, int y, int w, int h, LPCSTR  textstring, PIMAGE pimg = NULL); // 在指定矩形范围输出文字
void EGEAPI outtextrect(int x, int y, int w, int h, LPCWSTR textstring, PIMAGE pimg = NULL); // 在指定矩形范围输出文字
void EGEAPI xyprintf(int x, int y, LPCSTR  fmt, ...); // 在指定位置输出格式化字符串，指定绘图目标调用settarget
void EGEAPI xyprintf(int x, int y, LPCWSTR fmt, ...); // 在指定位置输出格式化字符串，指定绘图目标调用settarget
void EGEAPI rectprintf(int x, int y, int w, int h, LPCSTR  fmt, ...); // 在指定矩形输出格式化字符串，指定绘图目标调用settarget
void EGEAPI rectprintf(int x, int y, int w, int h, LPCWSTR fmt, ...); // 在指定矩形输出格式化字符串，指定绘图目标调用settarget
int  EGEAPI textwidth(LPCSTR  textstring, PIMAGE pimg = NULL);                 // 获取字符串占用的像素宽
int  EGEAPI textwidth(LPCWSTR textstring, PIMAGE pimg = NULL);                 // 获取字符串占用的像素宽
int  EGEAPI textwidth(CHAR  c, PIMAGE pimg = NULL);
int  EGEAPI textwidth(WCHAR c, PIMAGE pimg = NULL);
int  EGEAPI textheight(LPCSTR  textstring, PIMAGE pimg = NULL);                // 获取字符串占用的像素高
int  EGEAPI textheight(LPCWSTR textstring, PIMAGE pimg = NULL);                // 获取字符串占用的像素高
int  EGEAPI textheight(CHAR  c, PIMAGE pimg = NULL);
int  EGEAPI textheight(WCHAR c, PIMAGE pimg = NULL);
void EGEAPI settextjustify(int horiz, int vert, PIMAGE pimg = NULL);

// 设置当前字体样式(详见帮助)
//      nHeight: 字符的平均高度；
//      nWidth: 字符的平均宽度(0 表示自适应)；
//      lpszFace: 字体名称；
//      nEscapement: 字符串的书写角度(单位 0.1 度)；
//      nOrientation: 每个字符的书写角度(单位 0.1 度)；
//      nWeight: 字符的笔画粗细(0 表示默认粗细)；
//      bItalic: 是否斜体；
//      bUnderline: 是否下划线；
//      bStrikeOut: 是否删除线；
//      fbCharSet: 指定字符集；
//      fbOutPrecision: 指定文字的输出精度；
//      fbClipPrecision: 指定文字的剪辑精度；
//      fbQuality: 指定文字的输出质量；
//      fbPitchAndFamily: 指定以常规方式描述字体的字体系列。
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
void EGEAPI setfont(const LOGFONTA *font, PIMAGE pimg = NULL); // 设置当前字体样式
void EGEAPI setfont(const LOGFONTW *font, PIMAGE pimg = NULL); // 设置当前字体样式
void EGEAPI getfont(LOGFONTA *font, PCIMAGE pimg = NULL);      // 获取当前字体样式
void EGEAPI getfont(LOGFONTW *font, PCIMAGE pimg = NULL);      // 获取当前字体样式


//图片函数
#define getmaxx getwidth
#define getmaxy getheight

int EGEAPI getwidth(PCIMAGE pimg = NULL);          // 获取图片宽度
int EGEAPI getheight(PCIMAGE pimg = NULL);         // 获取图片高度
int EGEAPI getx(PCIMAGE pimg = NULL);              // 获取当前 x 坐标
int EGEAPI gety(PCIMAGE pimg = NULL);              // 获取当前 y 坐标

PIMAGE         EGEAPI newimage();                      // 创建 PIMAGE
PIMAGE         EGEAPI newimage(int width, int height); // 创建 PIMAGE
void           EGEAPI delimage(PCIMAGE pImg);          // 删除 PIMAGE
color_t*       EGEAPI getbuffer(PIMAGE pImg);
const color_t* EGEAPI getbuffer(PCIMAGE pImg);

int  EGEAPI resize_f(PIMAGE pDstImg, int width, int height);  //重设尺寸，但不填充背景色
int  EGEAPI resize(PIMAGE pDstImg, int width, int height); //重设尺寸，并填充背景色
int EGEAPI getimage(PIMAGE pDstImg, int srcX, int srcY, int srcWidth, int srcHeight);                             // 从屏幕获取图像
int EGEAPI getimage(PIMAGE pDstImg, PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);            // 从另一个 PIMAGE 对象中获取图像
int  EGEAPI getimage(PIMAGE pDstImg, LPCSTR  pImgFile, int zoomWidth = 0, int zoomHeight = 0);                     // 从图片文件获取图像(bmp/jpg/gif/emf/wmf)
int  EGEAPI getimage(PIMAGE pDstImg, LPCWSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);                     // 从图片文件获取图像(bmp/jpg/gif/emf/wmf)
int  EGEAPI getimage(PIMAGE pDstImg, LPCSTR  pResType, LPCSTR  pResName, int zoomWidth = 0, int zoomHeight = 0);   // 从资源文件获取图像(bmp/jpg/gif/emf/wmf)
int  EGEAPI getimage(PIMAGE pDstImg, LPCWSTR pResType, LPCWSTR pResName, int zoomWidth = 0, int zoomHeight = 0);   // 从资源文件获取图像(bmp/jpg/gif/emf/wmf)
void EGEAPI putimage(int dstX, int dstY, PCIMAGE pSrcImg, DWORD dwRop = SRCCOPY);                                  // 绘制图像到屏幕
void EGEAPI putimage(int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);                                // 绘制图像到屏幕(指定宽高)
void EGEAPI putimage(int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY);   // 绘制图像到屏幕(指定源宽高和目标宽高进行拉伸)
void EGEAPI putimage(PIMAGE pDstImg, int dstX, int dstY, PCIMAGE pSrcImg, DWORD dwRop = SRCCOPY);                                                                 // 绘制图像到另一图像中
void EGEAPI putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);                // 绘制图像到另一图像中(指定宽高)
void EGEAPI putimage(PIMAGE pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, PCIMAGE pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight, DWORD dwRop = SRCCOPY);   // 绘制图像到另一图像中(指定源宽高和目标宽高进行拉伸)
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

// 其它函数

HWND        EGEAPI getHWnd();         // 获取绘图窗口句柄
HINSTANCE   EGEAPI getHInstance();
HDC         EGEAPI getHDC(PCIMAGE pImg = NULL);

PVOID       EGEAPI getProcfunc();
int         EGEAPI getGraphicsVer();   // 获取当前版本
float       EGEAPI getfps();           // 获取当前帧率

//随机函数
void            EGEAPI randomize();
unsigned int    EGEAPI random(unsigned int n);
double          EGEAPI randomf();

//高级输入函数
// title 对话框标题， text 对话框提示文字， buf接收输入数据的字符串指针， len指出buf的最大长度，也同时会限制输入内容长度
int EGEAPI inputbox_getline(LPCSTR  title, LPCSTR  text, LPSTR  buf, int len);  //弹出对话框，让用户输入，当前程序运行暂停，返回非0表示输入有效，0为无效
int EGEAPI inputbox_getline(LPCWSTR title, LPCWSTR text, LPWSTR buf, int len);  //弹出对话框，让用户输入，当前程序运行暂停，返回非0表示输入有效，0为无效


//键盘处理函数
int     EGEAPI kbmsg();
key_msg EGEAPI getkey();
EGE_DEPRECATE(getchEx)
int     EGEAPI getchEx(int flag);
EGE_DEPRECATE(kbhitEx)
int     EGEAPI kbhitEx(int flag);
int     EGEAPI keystate(int key);       // 获得键码为key的键（见key_code_e）是否按下，如果key使用鼠标按键的键码，则获得的是鼠标键状态
void    EGEAPI flushkey();              // 清空键盘消息缓冲区

#if !defined(_INC_CONIO) && !defined(_CONIO_H_)
#define _INC_CONIO
#define _CONIO_H_
int EGEAPI getch();
int EGEAPI kbhit();
#else
#define getch getchEx
#define kbhit kbhitEx
#endif

//鼠标处理函数
int         EGEAPI mousemsg();                  // 检查是否存在鼠标消息
mouse_msg   EGEAPI getmouse();                  // 获取一个鼠标消息。如果没有，就等待
EGE_DEPRECATE(GetMouseMsg)
MOUSEMSG    EGEAPI GetMouseMsg();               // （不推荐使用的函数）获取一个鼠标消息。如果没有，就等待

void        EGEAPI flushmouse();                // 清空鼠标消息缓冲区
int         EGEAPI showmouse(int bShow);        // 设置是否显示鼠标
int         EGEAPI mousepos(int *x, int *y); // 获取当前鼠标位置

/*
callback function define as:
int __stdcall on_msg_key(void* param, unsigned msg, int key);
msg: see 'enum message_event'
key: keycode
return zero means process this message, otherwise means pass it and then process with 'getkey' function
*/
//int message_addkeyhandler(void* param, LPMSG_KEY_PROC func);        //设置键盘回调函数
/*
callback function define as:
int __stdcall on_msg_mouse(void* param, unsigned msg, int key, int x, int y);
msg: see 'enum message_event'
key: see 'enum message_mouse', if msg==MSG_EVENT_WHELL, key is a int number that indicates the distance the wheel is rotated, expressed in multiples or divisions of WHEEL_DELTA, which is 120.
x,y: current mouse (x, y)
return zero means process this message, otherwise means pass it and then process with 'GetMouseMsg' function
*/
//int message_addmousehandler(void* param, LPMSG_MOUSE_PROC func);    //设置鼠标回调函数
int EGEAPI SetCloseHandler(LPCALLBACK_PROC func);


class MUSIC
{
public:
	MUSIC();
	virtual ~MUSIC();
	operator HWND()const{ return (HWND)m_dwCallBack; }
public:
	int   IsOpen() { return (m_DID != MUSIC_ERROR) ? 1 : 0; }
	DWORD OpenFile(LPCSTR  filepath);
	DWORD OpenFile(LPCWSTR filepath);
	DWORD Play(DWORD dwFrom = MUSIC_ERROR, DWORD dwTo = MUSIC_ERROR);
	DWORD Pause();
	DWORD Seek(DWORD dwTo); //播放位置定位，单位为ms
	DWORD SetVolume(float value);
	DWORD Close();
	DWORD Stop();
	DWORD GetPosition();
	DWORD GetLength();
	// 以下函数GetPlayStatus的返回值为以下之一（意义看后缀）：
	// MUSIC_MODE_NOT_OPEN   //没有正确打开
	// MUSIC_MODE_NOT_READY  //设备没准备好 （较少使用）
	// MUSIC_MODE_PAUSE  //暂停中
	// MUSIC_MODE_PLAY   //正在播放
	// MUSIC_MODE_STOP   //成功打开后，或者播放完是这个状态
	// MUSIC_MODE_OPEN   //打开中 （较少使用）
	// MUSIC_MODE_SEEK   //定位中 （较少使用）
	DWORD GetPlayStatus();
private:
	DWORD       m_DID;
	PVOID       m_dwCallBack;
};

/* 压缩函数 */
/* 压缩时dest缓冲区要保证最小大小为sourceLen * 1.001 + 16 */
/* 调用compress/compress2前，*destLen必须有值，表示dest缓冲区的最大大小，返回时这个值表示实际大小 */
/* compress2 的level 从0-9，0不压缩，9最大压缩，compress函数使用默认值6 */
int             EGEAPI ege_compress(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen);
int             EGEAPI ege_compress2(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen, int level);
int             EGEAPI ege_uncompress(void *dest, unsigned long *destLen, const void *source, unsigned long sourceLen);
unsigned long   EGEAPI ege_uncompress_size(const void *source, unsigned long sourceLen); /* 返回0表示错误，其它表示大小 */

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
