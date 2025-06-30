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
    INIT_DEFAULT         = 0x0,   ///< Default mode
    INIT_NOBORDER        = 0x1,   ///< Borderless window
    INIT_CHILD           = 0x2,   ///< Child window mode
    INIT_TOPMOST         = 0x4,   ///< Topmost window
    INIT_RENDERMANUAL    = 0x8,   ///< Manual rendering mode
    INIT_NOFORCEEXIT     = 0x10,  ///< Don't force exit program when closing window, only set internal flag, is_run() can get the flag
    INIT_UNICODE         = 0x20,  ///< Unicode character messages (equivalent to setunicodecharmessage(true))
    INIT_HIDE            = 0x40,  ///< Hidden window
    INIT_WITHLOGO        = 0x100, ///< Show EGE Logo animation on startup (not shown by default in Debug version)
    INIT_ANIMATION       = INIT_DEFAULT | INIT_RENDERMANUAL | INIT_NOFORCEEXIT ///< Animation mode
};

/**
 * @enum rendermode_e
 * @brief Rendering mode
 */
enum rendermode_e
{
    RENDER_AUTO,   ///< Automatic rendering
    RENDER_MANUAL  ///< Manual rendering
};

/**
 * @enum graphics_errors
 * @brief Graphics operation error codes
 * 
 * Defines various error codes that can be returned by graphics operations
 */
enum graphics_errors
{
    grOk                 = 0,           ///< Operation successful
    grNoInitGraph        = -1,          ///< Graphics system not initialized
    grNotDetected        = -2,          ///< Graphics device not detected
    grFileNotFound       = -3,          ///< File not found
    grInvalidDriver      = -4,          ///< Invalid driver
    grNoLoadMem          = -5,          ///< Memory loading failed
    grNoScanMem          = -6,          ///< Scan memory failed
    grNoFloodMem         = -7,          ///< Fill memory failed
    grFontNotFound       = -8,          ///< Font not found
    grNoFontMem          = -9,          ///< Insufficient font memory
    grInvalidMode        = -10,         ///< Invalid mode
    grError              = -11,         ///< General error
    grIOerror            = -12,         ///< I/O error
    grInvalidFont        = -13,         ///< Invalid font
    grInvalidFontNum     = -14,         ///< Invalid font number
    grInvalidVersion     = -18,         ///< Version incompatible

    grException          = 16,          ///< EGE exception
    grParamError         = 17,          ///< Parameter error
    grInvalidRegion      = 18,          ///< Invalid region
    grOutOfMemory        = 19,          ///< Out of memory
    grNullPointer        = 20,          ///< Null pointer
    grAllocError         = 21,          ///< Allocation error
    grInvalidFileFormat  = 22,          ///< Invalid file format
    grUnsupportedFormat  = 23,          ///< Unsupported format
    grInvalidMemory      = 0xCDCDCDCD   ///< Invalid memory(-842150451)
};

/**
 * @enum message_event
 * @brief Message event types
 * 
 * Defines event types for mouse and keyboard messages, used for message handling
 */
enum message_event
{
    MSG_EVENT_UP         = 0x00,    ///< Key/mouse button release event
    MSG_EVENT_DOWN       = 0x01,    ///< Key/mouse button press event  
    MSG_EVENT_CLICK      = 0x01,    ///< Mouse click event (equivalent to DOWN)
    MSG_EVENT_DBCLICK    = 0x02,    ///< Mouse double-click event
    MSG_EVENT_MOVE       = 0x04,    ///< Mouse move event
    MSG_EVENT_WHEEL      = 0x10     ///< Mouse wheel event
};

/**
 * @enum message_mouse
 * @brief Mouse button identifiers
 * 
 * Defines different mouse buttons, can be combined using bitwise OR operations
 */
enum message_mouse
{
    MSG_MOUSE_LEFT  	 = 0x01,    ///< Left mouse button
    MSG_MOUSE_RIGHT 	 = 0x02,    ///< Right mouse button
    MSG_MOUSE_MID   	 = 0x04     ///< Middle mouse button (wheel button)
};


#ifndef EGE_COLOR_T_TYPEDEF
#define EGE_COLOR_T_TYPEDEF
/// @brief Color type definition, uses 32-bit unsigned integer to represent ARGB color
typedef uint32_t color_t;
#endif

/**
 * @enum alpha_type
 * @brief Alpha channel types
 * 
 * Defines different handling methods for image alpha channels
 */
enum alpha_type
{
    ALPHATYPE_STRAIGHT      = 0,    ///< Straight alpha (non-premultiplied alpha)
    ALPHATYPE_PREMULTIPLIED = 1     ///< Premultiplied alpha
};

/**
 * @struct ege_point
 * @brief Floating-point coordinate point structure
 * 
 * Used to represent a point in 2D space with floating-point coordinates
 */
struct ege_point
{
    float x;    ///< x coordinate
    float y;    ///< y coordinate
};

/**
 * @struct ege_rect
 * @brief Rectangle area structure
 * 
 * Used to represent a rectangular area, including position and size information
 */
struct ege_rect
{
    float x;    ///< Rectangle top-left x coordinate
    float y;    ///< Rectangle top-left y coordinate
    float w;    ///< Rectangle width
    float h;    ///< Rectangle height
};

/**
 * @struct ege_colpoint
 * @brief Coordinate point structure with color
 * 
 * Used to represent a 2D coordinate point with color information, commonly used for gradient effects
 */
struct ege_colpoint
{
    float   x;      ///< x coordinate
    float   y;      ///< y coordinate
    color_t color;  ///< Color value of this point
};

/**
 * @enum COLORS
 * @brief Predefined color constants
 * 
 * Provides commonly used color constants, defined based on web-safe color standards
 * Color values use RGB format and can be used directly in drawing functions
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
 * @brief Line styles
 * 
 * Defines different styles that can be used when drawing lines
 */
enum line_styles
{
    SOLID_LINE           = PS_SOLID,        ///< Solid line
    CENTER_LINE          = PS_DASH,         ///< Center line (dashed line)
    DOTTED_LINE          = PS_DOT,          ///< Dotted line
    DASHED_LINE          = PS_DASHDOT,      ///< Dash-dot line
    NULL_LINE            = PS_NULL,         ///< Null line (no drawing)
    USERBIT_LINE         = PS_USERSTYLE     ///< User-defined line style
};

/**
 * @struct line_style_type
 * @brief Line style structure
 * 
 * Describes detailed style attributes of lines
 */
struct line_style_type
{
    int             linestyle;  ///< Line style
    unsigned short  upattern;   ///< User-defined line pattern
    int             thickness;  ///< Line thickness
};

/**
 * @enum line_cap_type
 * @brief Line cap styles
 * 
 * Defines the drawing styles for line endpoints
 */
enum line_cap_type
{
    LINECAP_FLAT   = 0,     ///< Flat cap
    LINECAP_SQUARE,         ///< Square cap  
    LINECAP_ROUND           ///< Round cap
};

/**
 * @enum line_join_type
 * @brief Line join styles
 * 
 * Defines the drawing styles for line connection points
 */
enum line_join_type
{
    LINEJOIN_MITER = 0,     ///< Miter join
    LINEJOIN_BEVEL,         ///< Bevel join
    LINEJOIN_ROUND          ///< Round join
};

/**
 * @enum fill_patterns
 * @brief Fill patterns
 * 
 * Defines different pattern styles that can be used when filling geometric shapes
 */
enum fill_patterns
{
    EMPTY_FILL,         ///< No fill
    SOLID_FILL,         ///< Solid fill (fill with fill color)
    LINE_FILL,          ///< Horizontal line fill ---
    LTSLASH_FILL,       ///< Light slash fill "///"
    SLASH_FILL,         ///< Heavy slash fill "///"
    BKSLASH_FILL,       ///< Heavy backslash fill "\\\"
    LTBKSLASH_FILL,     ///< Light backslash fill "\\\"
    HATCH_FILL,         ///< Light grid fill
    XHATCH_FILL,        ///< Heavy cross grid fill
    INTERLEAVE_FILL,    ///< Interleaved line fill
    WIDE_DOT_FILL,      ///< Sparse dot fill
    CLOSE_DOT_FILL,     ///< Dense dot fill
    USER_FILL           ///< User-defined fill
};

/**
 * @enum fill_mode
 * @brief Fill modes
 * 
 * Defines filling algorithms for complex graphics
 */
enum fill_mode
{
    FILLMODE_DEFAULT   = 0,     ///< Default fill mode
    FILLMODE_ALTERNATE = 1,     ///< Alternate fill mode
    FILLMODE_WINDING   = 2      ///< Winding fill mode
};

/**
 * @enum text_just
 * @brief Text alignment methods
 * 
 * Defines horizontal and vertical alignment methods for text
 */
enum text_just
{
    LEFT_TEXT            = 0,   ///< Left align
    CENTER_TEXT          = 1,   ///< Center align
    RIGHT_TEXT           = 2,   ///< Right align

    TOP_TEXT             = 0,   ///< Top align
/*  CENTER_TEXT          = 1,     Already defined above */
    BOTTOM_TEXT          = 2    ///< Bottom align
};

/**
 * @struct textsettingstype
 * @brief Text settings structure
 * 
 * Contains text settings such as font, direction, size and alignment
 */
struct textsettingstype
{
    int font;       ///< Font type
    int direction;  ///< Text direction
    int charsize;   ///< Character size
    int horiz;      ///< Horizontal alignment
    int vert;       ///< Vertical alignment
};

/**
 * @enum font_styles
 * @brief Font styles
 * 
 * Defines various font styles, can be combined using bitwise OR operations
 */
enum font_styles
{
    FONTSTYLE_BOLD       = 1,   ///< Bold
    FONTSTYLE_ITALIC     = 2,   ///< Italic  
    FONTSTYLE_UNDERLINE  = 4,   ///< Underline
    FONTSTYLE_STRIKEOUT  = 8    ///< Strikeout
};

/**
 * @enum music_state_flag
 * @brief Music playback state flags
 * 
 * Defines various states of the music player
 */
enum music_state_flag
{
    MUSIC_MODE_NOT_OPEN  = 0x0,     ///< Not opened state
    MUSIC_MODE_NOT_READY = 0x20C,   ///< Not ready state
    MUSIC_MODE_PAUSE     = 0x211,   ///< Paused state
    MUSIC_MODE_PLAY      = 0x20E,   ///< Playing state
    MUSIC_MODE_STOP      = 0x20D,   ///< Stopped state
    MUSIC_MODE_OPEN      = 0x212,   ///< Opened state
    MUSIC_MODE_SEEK      = 0x210    ///< Seeking state
};

/// @brief Music operation error code
#define MUSIC_ERROR  0xFFFFFFFF

/**
 * @enum key_msg_flag
 * @brief Key message flags
 * 
 * Defines types and state flags for key messages
 */
enum key_msg_flag
{
    KEYMSG_CHAR_FLAG     = 2,       ///< Character message flag
    KEYMSG_DOWN_FLAG     = 1,       ///< Key down message flag
    KEYMSG_UP_FLAG       = 1,       ///< Key up message flag

    KEYMSG_CHAR          = 0x40000, ///< Character message
    KEYMSG_DOWN          = 0x10000, ///< Key down message
    KEYMSG_UP            = 0x20000, ///< Key up message
    KEYMSG_FIRSTDOWN     = 0x80000  ///< First key down message
};

/**
 * @enum key_code_e
 * @brief Keyboard and mouse key codes
 * 
 * Defines key code values for all detectable keyboard keys and mouse buttons
 * Key code values are based on Windows Virtual Key Codes
 */
enum key_code_e
{
    // Mouse buttons
    key_mouse_l         = 0x01,     ///< Left mouse button
    key_mouse_r         = 0x02,     ///< Right mouse button
    key_mouse_m         = 0x04,     ///< Middle mouse button
    key_mouse_x1        = 0x05,     ///< Mouse X1 button
    key_mouse_x2        = 0x06,     ///< Mouse X2 button
    
    // Special function keys
    key_back            = 0x08,     ///< Backspace key
    key_tab             = 0x09,     ///< Tab key
    key_enter           = 0x0d,     ///< Enter key
    key_shift           = 0x10,     ///< Shift key
    key_control         = 0x11,     ///< Ctrl key
    key_menu            = 0x12,     ///< Alt key
    key_pause           = 0x13,     ///< Pause key
    key_capslock        = 0x14,     ///< Caps Lock key
    key_esc             = 0x1b,     ///< Escape key
    key_space           = 0x20,     ///< Space key

    // Navigation keys
    key_pageup          = 0x21,     ///< Page Up key
    key_pagedown        = 0x22,     ///< Page Down key
    key_end             = 0x23,     ///< End key
    key_home            = 0x24,     ///< Home key

    // Arrow keys
    key_left            = 0x25,     ///< Left arrow key
    key_up              = 0x26,     ///< Up arrow key
    key_right           = 0x27,     ///< Right arrow key
    key_down            = 0x28,     ///< Down arrow key

    // Editing keys
    key_print           = 0x2a,     ///< Print key
    key_snapshot        = 0x2c,     ///< Print Screen key
    key_insert          = 0x2d,     ///< Insert key
    key_delete          = 0x2e,     ///< Delete key

    // Number keys (main keyboard)
    key_0               = 0x30,     ///< Number key 0
    key_1               = 0x31,     ///< Number key 1
    key_2               = 0x32,     ///< Number key 2
    key_3               = 0x33,     ///< Number key 3
    key_4               = 0x34,     ///< Number key 4
    key_5               = 0x35,     ///< Number key 5
    key_6               = 0x36,     ///< Number key 6
    key_7               = 0x37,     ///< Number key 7
    key_8               = 0x38,     ///< Number key 8
    key_9               = 0x39,     ///< Number key 9

    // Letter keys
    key_A               = 0x41,     ///< Letter key A
    key_B               = 0x42,     ///< Letter key B
    key_C               = 0x43,     ///< Letter key C
    key_D               = 0x44,     ///< Letter key D
    key_E               = 0x45,     ///< Letter key E
    key_F               = 0x46,     ///< Letter key F
    key_G               = 0x47,     ///< Letter key G
    key_H               = 0x48,     ///< Letter key H
    key_I               = 0x49,     ///< Letter key I
    key_J               = 0x4a,     ///< Letter key J
    key_K               = 0x4b,     ///< Letter key K
    key_L               = 0x4c,     ///< Letter key L
    key_M               = 0x4d,     ///< Letter key M
    key_N               = 0x4e,     ///< Letter key N
    key_O               = 0x4f,     ///< Letter key O
    key_P               = 0x50,     ///< Letter key P
    key_Q               = 0x51,     ///< Letter key Q
    key_R               = 0x52,     ///< Letter key R
    key_S               = 0x53,     ///< Letter key S
    key_T               = 0x54,     ///< Letter key T
    key_U               = 0x55,     ///< Letter key U
    key_V               = 0x56,     ///< Letter key V
    key_W               = 0x57,     ///< Letter key W
    key_X               = 0x58,     ///< Letter key X
    key_Y               = 0x59,     ///< Letter key Y
    key_Z               = 0x5a,     ///< Letter key Z
    
    // Windows keys
    key_win_l           = 0x5b,     ///< Left Windows key
    key_win_r           = 0x5c,     ///< Right Windows key

    key_sleep           = 0x5f,     ///< Sleep key

    // Numeric keypad
    key_num0            = 0x60,     ///< Numeric keypad 0
    key_num1            = 0x61,     ///< Numeric keypad 1
    key_num2            = 0x62,     ///< Numeric keypad 2
    key_num3            = 0x63,     ///< Numeric keypad 3
    key_num4            = 0x64,     ///< Numeric keypad 4
    key_num5            = 0x65,     ///< Numeric keypad 5
    key_num6            = 0x66,     ///< Numeric keypad 6
    key_num7            = 0x67,     ///< Numeric keypad 7
    key_num8            = 0x68,     ///< Numeric keypad 8
    key_num9            = 0x69,     ///< Numeric keypad 9

    // Numeric keypad operators
    key_multiply        = 0x6a,     ///< Numeric keypad multiply (*)
    key_add             = 0x6b,     ///< Numeric keypad add (+)
    key_separator       = 0x6c,     ///< Numeric keypad separator
    key_subtract        = 0x6d,     ///< Numeric keypad subtract (-)
    key_decimal         = 0x6e,     ///< Numeric keypad decimal point (.)
    key_divide          = 0x6f,     ///< Numeric keypad divide (/)

    // Function keys
    key_f1              = 0x70,     ///< F1 function key
    key_f2              = 0x71,     ///< F2 function key
    key_f3              = 0x72,     ///< F3 function key
    key_f4              = 0x73,     ///< F4 function key
    key_f5              = 0x74,     ///< F5 function key
    key_f6              = 0x75,     ///< F6 function key
    key_f7              = 0x76,     ///< F7 function key
    key_f8              = 0x77,     ///< F8 function key
    key_f9              = 0x78,     ///< F9 function key
    key_f10             = 0x79,     ///< F10 function key
    key_f11             = 0x7a,     ///< F11 function key
    key_f12             = 0x7b,     ///< F12 function key

    // Lock keys
    key_numlock         = 0x90,     ///< Num Lock key
    key_scrolllock      = 0x91,     ///< Scroll Lock key

    // Left/right distinguished modifier keys
    key_shift_l         = 0xa0,     ///< Left Shift key
    key_shift_r         = 0xa1,     ///< Right Shift key
    key_control_l       = 0xa2,     ///< Left Ctrl key
    key_control_r       = 0xa3,     ///< Right Ctrl key
    key_menu_l          = 0xa4,     ///< Left Alt key
    key_menu_r          = 0xa5,     ///< Right Alt key

    // Punctuation keys
    key_semicolon       = 0xba,     ///< Semicolon key (;)
    key_plus            = 0xbb,     ///< Equals/plus key (=)
    key_comma           = 0xbc,     ///< Comma key (,)
    key_minus           = 0xbd,     ///< Minus/underscore key (-)
    key_period          = 0xbe,     ///< Period key (.)
    key_slash           = 0xbf,     ///< Slash key (/)
    key_tilde           = 0xc0,     ///< Tilde key (~)
    key_lbrace          = 0xdb,     ///< Left bracket key ([)
    key_backslash       = 0xdc,     ///< Backslash key (\)
    key_rbrace          = 0xdd,     ///< Right bracket key (])
    key_quote           = 0xde,     ///< Quote key (')

    key_ime_process     = 0xe5      ///< IME process key
};

/**
 * @enum key_msg_e
 * @brief Key message types
 * 
 * Defines specific types of key events
 */
enum key_msg_e
{
    key_msg_down        = 1,    ///< Key press message
    key_msg_up          = 2,    ///< Key release message
    key_msg_char        = 4     ///< Character input message
};

/**
 * @enum key_flag_e
 * @brief Key state flags
 * 
 * Defines modifier key states and special flags for key events
 */
enum key_flag_e
{
    key_flag_shift      = 0x100,    ///< Shift key is pressed
    key_flag_ctrl       = 0x200,    ///< Ctrl key is pressed
    key_flag_first_down = 0x80000   ///< First press flag
};

/**
 * @struct key_msg
 * @brief Key message structure
 * 
 * Contains complete key event information
 */
struct key_msg
{
    int             key;    ///< Key code
    key_msg_e       msg;    ///< Message type
    unsigned int    flags;  ///< State flags
};

/**
 * @enum mouse_msg_e
 * @brief Mouse message types
 * 
 * Defines specific types of mouse events
 */
enum mouse_msg_e
{
    mouse_msg_down      = 0x10,     ///< Mouse button press message
    mouse_msg_up        = 0x20,     ///< Mouse button release message
    mouse_msg_move      = 0x40,     ///< Mouse move message
    mouse_msg_wheel     = 0x80      ///< Mouse wheel message
};

/**
 * @enum mouse_flag_e
 * @brief Mouse state flags
 * 
 * Defines the states of various mouse buttons and modifier keys in mouse events
 */
enum mouse_flag_e
{
    mouse_flag_left     = 0x001,    ///< Left mouse button is pressed
    mouse_flag_right    = 0x002,    ///< Right mouse button is pressed
    mouse_flag_mid      = 0x004,    ///< Middle mouse button is pressed
    mouse_flag_x1       = 0x008,    ///< Mouse X1 button is pressed
    mouse_flag_x2       = 0x010,    ///< Mouse X2 button is pressed
    mouse_flag_shift    = 0x100,    ///< Shift key is pressed
    mouse_flag_ctrl     = 0x200     ///< Ctrl key is pressed
};

/**
 * @struct mouse_msg
 * @brief Mouse message structure
 * 
 * Contains complete mouse event information with convenient state query methods
 */
struct mouse_msg
{
    int             x;      ///< Mouse x coordinate
    int             y;      ///< Mouse y coordinate
    mouse_msg_e     msg;    ///< Message type
    unsigned int    flags;  ///< State flags
    int             wheel;  ///< Wheel scroll delta

    /// @brief Check if it's a left mouse button event
    bool is_left()  const {return (flags & mouse_flag_left)  != 0;}
    /// @brief Check if it's a right mouse button event
    bool is_right() const {return (flags & mouse_flag_right) != 0;}
    /// @brief Check if it's a middle mouse button event
    bool is_mid()   const {return (flags & mouse_flag_mid)   != 0;}
    /// @brief Check if it's a mouse X1 button event
    bool is_x1()    const {return (flags & mouse_flag_x1)    != 0;}
    /// @brief Check if it's a mouse X2 button event
    bool is_x2()    const {return (flags & mouse_flag_x2)    != 0;}

    /// @brief Check if it's a button press event
    bool is_down()  const {return msg == mouse_msg_down; }
    /// @brief Check if it's a button release event
    bool is_up()    const {return msg == mouse_msg_up;   }
    /// @brief Check if it's a mouse move event
    bool is_move()  const {return msg == mouse_msg_move; }
    /// @brief Check if it's a wheel event
    bool is_wheel() const {return msg == mouse_msg_wheel;}
};

/**
 * @struct MOUSEMSG
 * @brief Legacy mouse message structure (compatibility)
 * 
 * Provides mouse message format compatible with older versions
 */
struct MOUSEMSG
{
    UINT  uMsg;         ///< Windows message ID
    bool  mkCtrl;       ///< Ctrl key state
    bool  mkShift;      ///< Shift key state
    bool  mkLButton;    ///< Left button state
    bool  mkMButton;    ///< Middle button state
    bool  mkRButton;    ///< Right button state
    bool  mkXButton1;   ///< X1 button state
    bool  mkXButton2;   ///< X2 button state
    short x;            ///< x coordinate
    short y;            ///< y coordinate
    short wheel;        ///< Wheel delta
};

/**
 * @struct viewporttype
 * @brief Viewport type structure
 * 
 * Defines the boundary rectangle of the drawing viewport
 */
/**
 * @struct viewporttype
 * @brief Viewport type structure
 * 
 * Defines the boundary rectangle of the drawing viewport
 */
struct viewporttype
{
    int left;       ///< Left boundary
    int top;        ///< Top boundary
    int right;      ///< Right boundary
    int bottom;     ///< Bottom boundary
    int clipflag;   ///< Clipping flag
};

/**
 * @struct ege_transform_matrix
 * @brief 2D transformation matrix
 * 
 * 3x2 matrix for 2D graphics transformations, supports translation, rotation, scaling, etc.
 */
struct ege_transform_matrix
{
    float m11, m12;     ///< First row: [m11, m12]
    float m21, m22;     ///< Second row: [m21, m22]
    float m31, m32;     ///< Third row: [m31, m32] translation components
};

/**
 * @struct ege_path
 * @brief Graphics path
 * 
 * Used to define complex graphics paths, supporting combinations of lines, curves and other graphic elements
 */
struct ege_path
{
private:
    void* m_data;       ///< Internal data pointer

public:
    /// @brief Default constructor
    ege_path();
    
    /// @brief Construct path from point array and type array
    /// @param points Point array
    /// @param types Path type array
    /// @param count Number of points
    ege_path(const ege_point* points, const unsigned char* types, int count);
    
    /// @brief Copy constructor
    /// @param path Path to copy
    ege_path(const ege_path& path);
    
    /// @brief Destructor
    virtual ~ege_path();

    /// @brief Get read-only data pointer
    /// @return Constant data pointer
    const void* data() const;
    
    /// @brief Get writable data pointer
    /// @return Data pointer
    void* data();
    
    /// @brief Assignment operator
    /// @param path Path to assign
    /// @return Path reference
    ege_path& operator=(const ege_path& path);
};

/**
 * @struct msg_createwindow
 * @brief Create window message structure
 * 
 * Message structure used to pass parameters when creating windows
 */
struct msg_createwindow
{
    HANDLE  hEvent;         ///< Event handle
    HWND    hwnd;           ///< Window handle
    const wchar_t* classname; ///< Window class name
    DWORD   style;          ///< Window style
    DWORD   exstyle;        ///< Extended window style
    size_t  id;             ///< Window ID
    LPVOID  param;          ///< Parameter pointer
};

/// @brief Generic callback function type
typedef void (CALLBACK_PROC)();

/// @brief Keyboard message handler callback function type
/// @param userdata User data pointer
/// @param message Message type
/// @param key Key code
/// @return Processing result
typedef int (__stdcall MSG_KEY_PROC  )(void*, unsigned, int);

/// @brief Mouse message handler callback function type
/// @param userdata User data pointer
/// @param message Message type
/// @param x x coordinate
/// @param y y coordinate
/// @param flags Flag bits
/// @return Processing result
typedef int (__stdcall MSG_MOUSE_PROC)(void*, unsigned, int, int, int);

/// @brief Callback function pointer type
typedef CALLBACK_PROC       * LPCALLBACK_PROC;
/// @brief Keyboard message handler function pointer type
typedef MSG_KEY_PROC        * LPMSG_KEY_PROC;
/// @brief Mouse message handler function pointer type
typedef MSG_MOUSE_PROC      * LPMSG_MOUSE_PROC;

struct VECTOR3D;

/// @brief Rotate 3D point around X axis
/// @param point Pointer to 3D point to rotate
/// @param rad Rotation angle (radians)
void EGEAPI rotate_point3d_x(VECTOR3D* point, float rad);

/// @brief Rotate 3D point around Y axis
/// @param point Pointer to 3D point to rotate
/// @param rad Rotation angle (radians)
void EGEAPI rotate_point3d_y(VECTOR3D* point, float rad);

/// @brief Rotate 3D point around Z axis
/// @param point Pointer to 3D point to rotate
/// @param rad Rotation angle (radians)
void EGEAPI rotate_point3d_z(VECTOR3D* point, float rad);

/**
 * @struct VECTOR3D
 * @brief 3D vector structure
 * 
 * Represents vectors or points in 3D space, provides basic 3D graphics computation functionality
 */
struct VECTOR3D
{
    float x, y, z;      ///< 3D coordinate components

    /// @brief Default constructor, initialize to origin
    VECTOR3D() : x(0.0f), y(0.0f), z(0.0f) {}
    
    /// @brief Constructor
    /// @param x x coordinate
    /// @param y y coordinate
    /// @param z z coordinate (default 0)
    VECTOR3D(float x, float y, float z = 0.0f) : x(x), y(y), z(z) {}    /// @brief Assignment operator
    /// @param vector Vector to assign
    /// @return Vector reference
    VECTOR3D& operator=(const VECTOR3D& vector)
    {
        x = vector.x;
        y = vector.y;
        z = vector.z;
        return *this;
    }

    /// @brief Vector addition assignment operator
    VECTOR3D& operator+=(const VECTOR3D& vector);
    /// @brief Vector subtraction assignment operator
    VECTOR3D& operator-=(const VECTOR3D& vector);
    /// @brief Vector addition operator
    VECTOR3D  operator+ (const VECTOR3D& vector) const;
    /// @brief Vector subtraction operator
    VECTOR3D  operator- (const VECTOR3D& vector) const;
    /// @brief Scalar multiplication assignment operator
    VECTOR3D& operator*=(float scale);
    /// @brief Scalar multiplication operator
    VECTOR3D  operator* (float scale) const;
    /// @brief Vector dot product operator
    float     operator* (const VECTOR3D& vector) const;
    /// @brief Vector cross product operator
    VECTOR3D  operator& (const VECTOR3D& vector) const;
    /// @brief Vector cross product assignment operator
    VECTOR3D& operator&=(const VECTOR3D& vector);
    
    /// @brief Get vector magnitude
    /// @return Vector magnitude
    float     GetModule() const;

    /// @brief Get squared vector magnitude
    /// @return Squared vector magnitude
    float GetSqrModule() const { return float(x * x + y * y + z * z); }

    /// @brief Set vector magnitude
    /// @param m New magnitude
    /// @return Vector reference
    VECTOR3D& SetModule(float m)
    {
        float t  = m / GetModule();
        *this   *= t;
        return *this;
    }

    /// @brief Rotate vector around specified axis
    /// @param rad Rotation angle (radians)
    /// @param vector Rotation axis vector
    /// @return Vector reference
    VECTOR3D& Rotate(float rad, const VECTOR3D& vector);

    /// @brief Rotate vector around specified axis
    /// @param rad Rotation angle (radians)
    /// @param x Rotation axis x component
    /// @param y Rotation axis y component
    /// @param z Rotation axis z component
    /// @return Vector reference
    VECTOR3D& Rotate(float rad, float x, float y, float z)
    {
        VECTOR3D v(x, y, z);
        return Rotate(rad, v);
    }

    /// @brief Rotate from start vector to end vector
    /// @param e End vector
    /// @param s Start vector (default to positive z-axis)
    /// @return Vector reference
    VECTOR3D&    Rotate  (const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f));
    
    /// @brief Calculate angle between two vectors
    /// @param e First vector
    /// @param s Second vector (default to positive z-axis)
    /// @return Angle (radians)
    static float GetAngle(const VECTOR3D& e, const VECTOR3D& s = VECTOR3D(0.0f, 0.0f, 1.0f));
};

/// @brief Image object forward declaration
class IMAGE;
/// @brief Image object pointer type
typedef IMAGE *PIMAGE;
/// @brief Constant image object pointer type
typedef const IMAGE *PCIMAGE;

/**
 * @brief Set code page
 * 
 * Set character encoding, affects text processing and display
 * @param codepage Code page, should use EGE_CODEPAGE_XXX constants, default is EGE_CODEPAGE_ANSI
 */
void EGEAPI setcodepage(unsigned int codepage);

/**
 * @brief Get current code page
 * 
 * @return Currently set code page
 */
unsigned int EGEAPI getcodepage();

/**
 * @brief Set whether to enable Unicode character messages
 * 
 * Control whether getkey() function uses UTF-16 encoded character messages
 * @param enable true enables UTF-16, false uses ANSI
 */
void EGEAPI setunicodecharmessage(bool enable);

/**
 * @brief Get Unicode character message setting status
 * 
 * @return true indicates UTF-16 is enabled, false indicates ANSI is used
 */
bool EGEAPI getunicodecharmessage();

/**
 * @brief Set initialization mode
 * 
 * Set default parameters when creating windows
 * @param mode Initialization mode flags
 * @param x Initial window x coordinate (default CW_USEDEFAULT)
 * @param y Initial window y coordinate (default CW_USEDEFAULT)
 */
void EGEAPI setinitmode(initmode_flag mode, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT);
inline void setinitmode(int mode, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT)
{
    setinitmode(static_cast<initmode_flag>(mode), x, y);
}

/**
 * @brief Get current initialization mode
 * 
 * @return Currently set initialization mode flags
 */
initmode_flag  EGEAPI getinitmode();

/**
 * @brief Create EGE graphics window and perform environment initialization
 * 
 * This is the main initialization function of the EGE graphics library, creates and displays graphics window after execution
 * 
 * @param width Window width (pixels)
 * @param height Window height (pixels)
 * @param mode Initialization mode flags, controls various window attributes
 * 
 * @code
 * // Create an 800x600 default window
 * initgraph(800, 600, INIT_DEFAULT);
 * 
 * // Create a borderless topmost window
 * initgraph(640, 480, INIT_NOBORDER | INIT_TOPMOST);
 * @endcode
 */
void EGEAPI initgraph(int width, int height, initmode_flag mode);
inline void initgraph(int width, int height, int mode)
{
    initgraph(width, height, static_cast<initmode_flag>(mode));
}

/**
 * @brief Create EGE graphics window (simplified version)
 * 
 * Create graphics window using currently set initialization mode
 * Uses default mode in debug version, shows EGE logo in release version
 * 
 * @param width Window width (pixels)
 * @param height Window height (pixels)
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
 * @brief Initialize graphics system (BGI compatible version)
 * 
 * Provides compatible interface with traditional BGI graphics library
 * 
 * @param graphdriver Graphics driver type pointer, usually pass DETECT for auto-detection
 * @param graphmode Graphics mode pointer, usually pass 0 for auto-selection
 * @param pathtodriver BGI driver file path, can pass empty string "" if driver file is in current directory
 */
void initgraph(int *graphdriver, int *graphmode, const char *pathtodriver);

/**
 * @brief Close graphics system
 * 
 * Logically close graphics system
 * After execution, EGE window will be hidden, but resources won't be fully released, is_run() function still returns true (note this)
 * If IMAGE object resources need to be released, delimage function still needs to be called
 */
void EGEAPI closegraph();

/**
 * @brief Check if graphics environment is running
 * 
 * @return true When EGE graphics environment exists and window is not closed
 * @return false When EGE graphics environment doesn't exist or user clicks close button
 */
bool EGEAPI is_run();

/**
 * @brief Set window title
 * @param caption Window title string
 */
void EGEAPI setcaption(const char* caption);

/**
 * @brief Set window title (Unicode version)
 * @param caption Window title wide string
 */
void EGEAPI setcaption(const wchar_t* caption);

/**
 * @brief Set window icon
 * @param icon_id Icon resource ID
 */
void EGEAPI seticon(int icon_id);

/**
 * @brief Attach to existing window handle
 * @param hWnd Window handle to attach to
 * @return Operation result code
 */
int  EGEAPI attachHWND(HWND hWnd);

/**
 * @brief Show window
 */
void EGEAPI showwindow();

/**
 * @brief Hide window
 */
void EGEAPI hidewindow();

/**
 * @brief Move window to specified position
 * @param x New x coordinate
 * @param y New y coordinate
 * @param redraw Whether to redraw window (default true)
 */
void EGEAPI movewindow(int x, int y, bool redraw = true);

/**
 * @brief Resize window
 * @param width New window width
 * @param height New window height
 */
void EGEAPI resizewindow(int width, int height);

/**
 * @brief Refresh window display
 */
void EGEAPI flushwindow();

/**
 * @brief Set render mode
 * @param mode Render mode (automatic or manual)
 */
void EGEAPI setrendermode(rendermode_e mode);

/**
 * @brief Get current drawing target
 * @return Current drawing target image pointer, NULL means screen
 */
PIMAGE      gettarget();

/**
 * @brief Set drawing target
 * @param pbuf Target image pointer, NULL means screen
 * @return Setting result code
 */
int         settarget(PIMAGE pbuf);

/**
 * @brief Clear device (clear screen)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI cleardevice(PIMAGE pimg = NULL);

/**
 * @brief Get viewport settings
 * @param left Left boundary output pointer
 * @param top Top boundary output pointer
 * @param right Right boundary output pointer
 * @param bottom Bottom boundary output pointer
 * @param clip Clipping flag output pointer (optional)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI getviewport(int* left, int* top, int* right, int* bottom, int* clip = 0, PCIMAGE pimg = NULL);

/**
 * @brief Set viewport
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param clip Whether to enable clipping (default 1)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setviewport(int  left, int  top, int  right, int  bottom, int  clip = 1, PIMAGE  pimg = NULL);

/**
 * @brief Clear viewport area
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI clearviewport(PIMAGE pimg = NULL);

/**
 * @deprecated Deprecated, please use image functions instead
 * @brief Set active page
 * @param page Page number
 */
EGE_DEPRECATE(setactivepage, "Please use the image function instead.")
void EGEAPI setactivepage(int page);

/**
 * @deprecated Deprecated, please use image functions instead
 * @brief Set visual page
 * @param page Page number
 */
EGE_DEPRECATE(setvisualpage, "Please use the image function instead.")
void EGEAPI setvisualpage(int page);

/**
 * @deprecated Deprecated, please use image functions instead
 * @brief Swap pages
 */
EGE_DEPRECATE(swappage, "Please use the image function instead.")
void EGEAPI swappage();

/**
 * @brief Get window viewport settings
 * @param viewport Viewport structure pointer
 */
void EGEAPI window_getviewport(viewporttype * viewport);

/**
 * @brief Get window viewport settings
 * @param left Left boundary output pointer
 * @param top Top boundary output pointer
 * @param right Right boundary output pointer
 * @param bottom Bottom boundary output pointer
 */
void EGEAPI window_getviewport(int* left, int* top, int* right, int* bottom);

/**
 * @brief Set window viewport
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 */
void EGEAPI window_setviewport(int  left, int  top, int  right, int  bottom);

/**
 * @brief Set line width
 * @param width Line width (pixels)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setlinewidth(float width, PIMAGE pimg = NULL);

/**
 * @brief Get line style
 * @param linestyle Line style output pointer
 * @param pattern Line pattern output pointer (optional)
 * @param thickness Line thickness output pointer (optional)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI getlinestyle(int* linestyle, unsigned short* pattern = NULL, int* thickness = NULL, PCIMAGE pimg = NULL);

/**
 * @brief Set line style
 * @param linestyle Line style
 * @param pattern Line pattern (default 0)
 * @param thickness Line thickness (default 1)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setlinestyle(int  linestyle, unsigned short  pattern = 0,    int  thickness = 1,    PIMAGE  pimg = NULL);

/**
 * @brief Set line cap style
 * @param linecap Cap style
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setlinecap(line_cap_type linecap, PIMAGE pimg = NULL);

/**
 * @brief Set line cap style (separately set start and end caps)
 * @param startCap Start cap style
 * @param endCap End cap style
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setlinecap(line_cap_type  startCap, line_cap_type  endCap, PIMAGE  pimg = NULL);

/**
 * @brief Get line cap style
 * @param startCap Start cap style output pointer
 * @param endCap End cap style output pointer
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI getlinecap(line_cap_type* startCap, line_cap_type* endCap, PCIMAGE pimg = NULL);

/**
 * @brief Get line cap style
 * @param pimg Target image pointer, NULL means current ege window
 * @return Cap style
 */
line_cap_type EGEAPI getlinecap(PCIMAGE pimg = NULL);

/**
 * @brief Set line join style
 * @param linejoin Join style
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setlinejoin(line_join_type  linejoin, PIMAGE pimg = NULL);

/**
 * @brief Set line join style (with miter limit)
 * @param linejoin Join style
 * @param miterLimit Miter limit value
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setlinejoin(line_join_type  linejoin, float  miterLimit, PIMAGE pimg = NULL);

/**
 * @brief Get line join style
 * @param linejoin Join style output pointer
 * @param miterLimit Miter limit value output pointer
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI getlinejoin(line_join_type* linejoin, float* miterLimit, PCIMAGE pimg = NULL);

/**
 * @brief Get line join style
 * @param pimg Target image pointer, NULL means current ege window
 * @return Join style
 */
line_join_type EGEAPI getlinejoin(PCIMAGE pimg = NULL);

/**
 * @brief Set fill style
 * @param pattern Fill pattern type
 * @param color Fill color
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setfillstyle(int pattern, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set write mode
 * @param mode Write mode
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setwritemode(int mode, PIMAGE pimg = NULL);

/**
 * @brief Get drawing color
 * @param pimg Target image pointer, NULL means current ege window
 * @return Current drawing color
 */
color_t EGEAPI getcolor      (PCIMAGE pimg = NULL);

/**
 * @brief Get line color
 * @param pimg Target image pointer, NULL means current ege window
 * @return Current line color
 */
color_t EGEAPI getlinecolor  (PCIMAGE pimg = NULL);

/**
 * @brief Get fill color
 * @param pimg Target image pointer, NULL means current ege window
 * @return Current fill color
 */
color_t EGEAPI getfillcolor  (PCIMAGE pimg = NULL);

/**
 * @brief Get background color
 * @param pimg Target image pointer, NULL means current ege window
 * @return Current background color
 */
color_t EGEAPI getbkcolor    (PCIMAGE pimg = NULL);

/**
 * @brief Get text color
 * @param pimg Target image pointer, NULL means current ege window
 * @return Current text color
 */
color_t EGEAPI gettextcolor  (PCIMAGE pimg = NULL);

/**
 * @brief Set drawing color
 * @param color Drawing color
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI setcolor      (color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set line color
 * @param color Line color
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI setlinecolor  (color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set fill color
 * @param color Fill color
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI setfillcolor  (color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set background color, will actively replace old background color pixels with new background color pixels
 * @param color Background color
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI setbkcolor    (color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set background color, but will not replace old background color pixels with new background color pixels
 * @param color Background color
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI setbkcolor_f  (color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set text color
 * @param color Text color
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI settextcolor  (color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set font background color
 * @param color Font background color
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI setfontbkcolor(color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set background blend mode
 * @param bkMode Background mode (TRANSPARENT or OPAQUE)
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI setbkmode(int bkMode, PIMAGE pimg = NULL);

/// @defgroup ColorConversion Color conversion macro definitions
/// @{
#define RGBtoGRAY   rgb2gray    ///< RGB to grayscale
#define RGBtoHSL    rgb2hsl     ///< RGB to HSL
#define RGBtoHSV    rgb2hsv     ///< RGB to HSV
#define HSLtoRGB    hsl2rgb     ///< HSL to RGB
#define HSVtoRGB    hsv2rgb     ///< HSV to RGB
/// @}

/**
 * @brief RGB color to grayscale conversion
 * @param rgb RGB color value
 * @return Grayscale color value
 */
color_t EGEAPI rgb2gray(color_t rgb);

/**
 * @brief RGB color to HSL conversion
 * @param rgb RGB color value
 * @param H Hue output pointer (0-360 degrees)
 * @param S Saturation output pointer (0-1)
 * @param L Lightness output pointer (0-1)
 */
void    EGEAPI rgb2hsl(color_t rgb, float* H, float* S, float* L);

/**
 * @brief RGB color to HSV conversion
 * @param rgb RGB color value
 * @param H Hue output pointer (0-360 degrees)
 * @param S Saturation output pointer (0-1)
 * @param V Value output pointer (0-1)
 */
void    EGEAPI rgb2hsv(color_t rgb, float* H, float* S, float* V);

/**
 * @brief HSL color to RGB conversion
 * @param H Hue (0-360 degrees)
 * @param S Saturation (0-1)
 * @param L Lightness (0-1)
 * @return RGB color value
 */
color_t EGEAPI hsl2rgb(float H, float S, float L);

/**
 * @brief HSV color to RGB conversion
 * @param H Hue (0-360 degrees)
 * @param S Saturation (0-1)
 * @param V Value (0-1)
 * @return RGB color value
 */
color_t EGEAPI hsv2rgb(float H, float S, float V);

/**
 * @brief Color blending
 * @param dst Destination color
 * @param src Source color
 * @param alpha Alpha transparency (0-255)
 * @return Blended color
 */
color_t EGEAPI colorblend  (color_t dst, color_t src, unsigned char alpha);

/**
 * @brief Color blending (fast version, lower precision)
 * @param dst Destination color
 * @param src Source color
 * @param alpha Alpha transparency (0-255)
 * @return Blended color
 */
color_t EGEAPI colorblend_f(color_t dst, color_t src, unsigned char alpha);

/**
 * @brief Alpha blending
 * @param dst Destination color
 * @param src Source color (including alpha channel)
 * @return Blended color
 */
color_t EGEAPI alphablend  (color_t dst, color_t src);

/**
 * @brief Alpha blending (with transparency factor)
 * @param dst Destination color
 * @param src Source color
 * @param srcAlphaFactor Source color transparency factor (0-255)
 * @return Blended color
 */
color_t EGEAPI alphablend  (color_t dst, color_t src, unsigned char srcAlphaFactor);

/**
 * @brief Alpha blending (premultiplied alpha)
 * @param dst Destination color
 * @param src Source color (premultiplied alpha)
 * @return Blended color
 */
color_t EGEAPI alphablend_premultiplied(color_t dst, color_t src);

/**
 * @brief Alpha blending (premultiplied alpha, with transparency factor)
 * @param dst Destination color
 * @param src Source color (premultiplied alpha)
 * @param srcAlphaFactor Source color transparency factor (0-255)
 * @return Blended color
 */
color_t EGEAPI alphablend_premultiplied(color_t dst, color_t src, unsigned char srcAlphaFactor);

/**
 * @brief Get pixel color
 * @param x X coordinate
 * @param y Y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 * @return Pixel color value
 */
color_t EGEAPI getpixel   (int x, int y, PCIMAGE pimg = NULL);

/**
 * @brief Set pixel color
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel   (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Get pixel color (fast version, no boundary check, ignores viewport)
 * @param x X coordinate
 * @param y Y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 * @return Pixel color value
 */
color_t EGEAPI getpixel_f (int x, int y, PCIMAGE pimg = NULL);

/**
 * @brief Set pixel color (fast version, no boundary check, ignores viewport)
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel_f (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set multiple pixels in batch
 * @param numOfPoints Number of points
 * @param points Point coordinate array, format: [x1,y1,x2,y2,...]
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixels  (int numOfPoints, const int* points, PIMAGE pimg = NULL);

/**
 * @brief Set multiple pixels in batch (fast version, no boundary check)
 * @param numOfPoints Number of points
 * @param points Point coordinate array, format: [x1,y1,x2,y2,...]
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixels_f(int numOfPoints, const int* points, PIMAGE pimg = NULL);

/**
 * @brief Set pixel (RGB channels are set to the result of blending with ARGB color, alpha channel remains unchanged)
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value (including alpha channel)
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel_withalpha   (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set pixel (preserve alpha channel, fast version, no boundary check)
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value (including alpha channel)
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel_withalpha_f (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set pixel (RGB channels are replaced with specified color value, alpha channel remains unchanged)
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel_savealpha   (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set pixel (preserve alpha channel, fast version, no boundary check)
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel_savealpha_f (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set pixel (alpha blending)
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel_alphablend  (int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set pixel (alpha blending, fast version, no boundary check)
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel_alphablend_f(int x, int y, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set pixel (alpha blending with transparency factor)
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value
 * @param alphaFactor Transparency factor (0-255)
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel_alphablend  (int x, int y, color_t color, unsigned char alphaFactor, PIMAGE pimg = NULL);

/**
 * @brief Set pixel (alpha blending with transparency factor, fast version, no boundary check)
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Color value
 * @param alphaFactor Transparency factor (0-255)
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI putpixel_alphablend_f(int x, int y, color_t color, unsigned char alphaFactor, PIMAGE pimg = NULL);

/**
 * @brief Move current drawing position
 * @param x New x coordinate
 * @param y New y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI moveto (int x,  int y,  PIMAGE pimg = NULL);

/**
 * @brief Move current drawing position relatively
 * @param dx X direction offset
 * @param dy Y direction offset
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI moverel(int dx, int dy, PIMAGE pimg = NULL);

/**
 * @brief Draw line
 * @param x1 Start point x coordinate
 * @param y1 Start point y coordinate
 * @param x2 End point x coordinate
 * @param y2 End point y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI line     (int   x1, int   y1, int   x2, int   y2, PIMAGE pimg = NULL);

/**
 * @brief Draw line (floating point version)
 * @param x1 Start point x coordinate
 * @param y1 Start point y coordinate
 * @param x2 End point x coordinate
 * @param y2 End point y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI line_f   (float x1, float y1, float x2, float y2, PIMAGE pimg = NULL);

/**
 * @brief Draw line from current position to specified point
 * @param x End point x coordinate
 * @param y End point y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI lineto   (int   x,  int   y,  PIMAGE pimg = NULL);

/**
 * @brief Draw line from current position to specified point (floating point version)
 * @param x End point x coordinate
 * @param y End point y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI lineto_f (float x,  float y,  PIMAGE pimg = NULL);

/**
 * @brief Draw relative line from current position
 * @param dx X direction offset
 * @param dy Y direction offset
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI linerel  (int   dx, int   dy, PIMAGE pimg = NULL);

/**
 * @brief Draw relative line from current position (floating point version)
 * @param dx X direction offset
 * @param dy Y direction offset
 * @param pimg Target image pointer, NULL means current ege window
 */
void    EGEAPI linerel_f(float dx, float dy, PIMAGE pimg = NULL);

//void EGEAPI getarccoords(int *px, int *py, int *pxstart, int *pystart, int *pxend, int *pyend, PIMAGE pimg = NULL);    // ###

/**
 * @brief Draw elliptical arc
 * @param x Ellipse center x coordinate
 * @param y Ellipse center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ellipse      (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw elliptical arc (floating point version)
 * @param x Ellipse center x coordinate
 * @param y Ellipse center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ellipsef     (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw sector (outline only)
 * @param x Sector center x coordinate
 * @param y Sector center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI sector       (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw sector (outline only, floating point version)
 * @param x Sector center x coordinate
 * @param y Sector center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI sectorf      (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw pie chart (outline only)
 * @param x Pie center x coordinate
 * @param y Pie center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI pie          (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw pie chart (outline only, floating point version)
 * @param x Pie center x coordinate
 * @param y Pie center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI pief         (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw filled pie chart
 * @param x Pie center x coordinate
 * @param y Pie center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillpie      (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw filled pie chart (floating point version)
 * @param x Pie center x coordinate
 * @param y Pie center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillpief     (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw solid pie chart
 * @param x Pie center x coordinate
 * @param y Pie center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidpie     (int   x, int   y, int   startAngle, int   endAngle, int   xRadius, int   yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw solid pie chart (floating point version)
 * @param x Pie center x coordinate
 * @param y Pie center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidpief    (float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw circular arc
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI arc          (int   x, int   y, int   startAngle, int   endAngle, int   radius, PIMAGE pimg = NULL);

/**
 * @brief Draw circular arc (floating point version)
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI arcf         (float x, float y, float startAngle, float endAngle, float radius, PIMAGE pimg = NULL);

/**
 * @brief Draw circular sector
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI pieslice     (int   x, int   y, int   startAngle, int   endAngle, int   radius, PIMAGE pimg = NULL);

/**
 * @brief Draw circular sector (floating point version)
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param startAngle Start angle (degrees)
 * @param endAngle End angle (degrees)
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI pieslicef    (float x, float y, float startAngle, float endAngle, float radius, PIMAGE pimg = NULL);

/**
 * @brief Draw filled ellipse
 * @param x Ellipse center x coordinate
 * @param y Ellipse center y coordinate
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillellipse  (int   x, int   y, int   xRadius,    int   yRadius,  PIMAGE pimg = NULL);

/**
 * @brief Draw filled ellipse (floating point version)
 * @param x Ellipse center x coordinate
 * @param y Ellipse center y coordinate
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillellipsef (float x, float y, float xRadius,    float yRadius,  PIMAGE pimg = NULL);

/**
 * @brief Draw solid ellipse
 * @param x Ellipse center x coordinate
 * @param y Ellipse center y coordinate
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidellipse (int   x, int   y, int   xRadius,    int   yRadius,  PIMAGE pimg = NULL);

/**
 * @brief Draw solid ellipse (floating point version)
 * @param x Ellipse center x coordinate
 * @param y Ellipse center y coordinate
 * @param xRadius X-axis radius
 * @param yRadius Y-axis radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidellipsef(float x, float y, float xRadius,    float yRadius,  PIMAGE pimg = NULL);

/**
 * @brief Draw circle outline
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI circle       (int   x, int   y, int   radius, PIMAGE pimg = NULL);

/**
 * @brief Draw circle outline (floating point version)
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI circlef      (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief Draw filled circle
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillcircle   (int   x, int   y, int   radius, PIMAGE pimg = NULL);

/**
 * @brief Draw filled circle (floating point version)
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillcirclef  (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief Draw solid circle
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidcircle  (int   x, int   y, int   radius, PIMAGE pimg = NULL);

/**
 * @brief Draw solid circle (floating point version)
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidcirclef (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief Draw 3D bar
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param depth Depth
 * @param topFlag Whether to draw top face
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI bar3d        (int left, int top, int right, int bottom, int depth,   int topFlag, PIMAGE pimg = NULL);

/**
 * @brief Draw bar (filled rectangle)
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI bar          (int left, int top, int right, int bottom, PIMAGE pimg = NULL);

/**
 * @brief Draw rectangle outline
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI rectangle    (int left, int top, int right, int bottom, PIMAGE pimg = NULL);

/**
 * @brief Draw filled rectangle
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillrect     (int left, int top, int right, int bottom, PIMAGE pimg = NULL);

/**
 * @brief Draw solid rectangle
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidrect    (int left, int top, int right, int bottom, PIMAGE pimg = NULL);

/**
 * @brief Draw rounded rectangle outline
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param radius Corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI roundrect     (int left, int top, int right, int bottom, int radius,  PIMAGE pimg = NULL);

/**
 * @brief Draw filled rounded rectangle
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param radius Corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillroundrect (int left, int top, int right, int bottom, int radius,  PIMAGE pimg = NULL);

/**
 * @brief Draw solid rounded rectangle
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param radius Corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidroundrect(int left, int top, int right, int bottom, int radius,  PIMAGE pimg = NULL);

/**
 * @brief Draw rounded rectangle outline (different corner radii)
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param xRadius X-axis corner radius
 * @param yRadius Y-axis corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI roundrect     (int left, int top, int right, int bottom, int xRadius, int yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw filled rounded rectangle (different corner radii)
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param xRadius X-axis corner radius
 * @param yRadius Y-axis corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillroundrect (int left, int top, int right, int bottom, int xRadius, int yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw solid rounded rectangle (different corner radii)
 * @param left Left boundary
 * @param top Top boundary
 * @param right Right boundary
 * @param bottom Bottom boundary
 * @param xRadius X-axis corner radius
 * @param yRadius Y-axis corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidroundrect(int left, int top, int right, int bottom, int xRadius, int yRadius, PIMAGE pimg = NULL);

/**
 * @brief Draw polygon
 * @param numOfPoints Number of points
 * @param points Point coordinate array, format: [x1,y1,x2,y2,...]
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI drawpoly      (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief Draw multiple line segments
 * @param numOfPoints Number of points
 * @param points Point coordinate array, format: [x1,y1,x2,y2,...]
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI polyline      (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief Draw polygon outline
 * @param numOfPoints Number of points
 * @param points Point coordinate array, format: [x1,y1,x2,y2,...]
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI polygon       (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief Draw filled polygon
 * @param numOfPoints Number of points
 * @param points Point coordinate array, format: [x1,y1,x2,y2,...]
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillpoly      (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief Draw solid polygon
 * @param numOfPoints Number of points
 * @param points Point coordinate array, format: [x1,y1,x2,y2,...]
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI solidpoly     (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief Draw gradient filled polygon
 * @param numOfPoints Number of points
 * @param points Colored point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI fillpoly_gradient(int numOfPoints, const ege_colpoint* points, PIMAGE pimg = NULL);

/**
 * @brief Draw multiple line segments
 * @param numOfLines Number of line segments
 * @param points Line segment endpoint coordinate array, every two points form one line segment
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI drawlines     (int numOfLines,  const int *points, PIMAGE pimg = NULL);

/**
 * @brief Draw Bezier curve
 * @param numOfPoints Number of control points
 * @param points Control point coordinate array, format: [x1,y1,x2,y2,...]
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI drawbezier    (int numOfPoints, const int *points, PIMAGE pimg = NULL);

/**
 * @brief Flood fill (based on boundary color)
 * @param x Fill starting point x coordinate
 * @param y Fill starting point y coordinate
 * @param borderColor Boundary color
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI floodfill     (int x, int y, int borderColor, PIMAGE pimg = NULL);

/**
 * @brief Area fill (based on area color)
 * @param x Fill starting point x coordinate
 * @param y Fill starting point y coordinate
 * @param areaColor Area color to be replaced
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI floodfillsurface (int x, int y, color_t areaColor, PIMAGE pimg = NULL);

#ifdef EGE_GDIPLUS
/// @defgroup EGEGDIPlus EGE GDI+ enhanced functions
/// Enhanced drawing functions that require EGE_GDIPLUS macro to be defined
/// @{

/**
 * @brief Enable or disable anti-aliasing
 * @param enable true to enable anti-aliasing, false to disable
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_enable_aa(bool enable, PIMAGE pimg = NULL);

/**
 * @brief Set global alpha transparency
 * @param alpha Alpha value (0-255, 0 fully transparent, 255 fully opaque)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_setalpha(int alpha, PIMAGE pimg = NULL);

/**
 * @brief Draw line (GDI+ enhanced version)
 * @param x1 Start point x coordinate
 * @param y1 Start point y coordinate
 * @param x2 End point x coordinate
 * @param y2 End point y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_line(float x1, float y1, float x2, float y2, PIMAGE pimg = NULL);

/**
 * @brief Draw polygon (GDI+ enhanced version)
 * @param numOfPoints Number of points
 * @param points Point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawpoly       (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief Draw multiple line segments (GDI+ enhanced version)
 * @param numOfPoints Number of points
 * @param points Point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_polyline       (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief Draw polygon outline (GDI+ enhanced version)
 * @param numOfPoints Number of points
 * @param points Point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_polygon        (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief Draw filled polygon (GDI+ enhanced version)
 * @param numOfPoints Number of points
 * @param points Point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillpoly       (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief Draw Bezier curve (GDI+ enhanced version)
 * @param numOfPoints Number of control points
 * @param points Control point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_bezier         (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief Draw Bezier curve (same as ege_bezier)
 * @param numOfPoints Number of control points
 * @param points Control point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawbezier     (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);  // Same as ege_bezier

/**
 * @brief Draw cardinal spline curve
 * @param numOfPoints Number of control points
 * @param points Control point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawcurve      (int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief Draw closed cardinal spline curve
 * @param numOfPoints Number of control points
 * @param points Control point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawclosedcurve(int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief Draw filled closed cardinal spline curve
 * @param numOfPoints Number of control points
 * @param points Control point coordinate array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillclosedcurve(int numOfPoints, const ege_point* points, PIMAGE pimg = NULL);

/**
 * @brief Draw cardinal spline curve (with tension parameter)
 * @param numOfPoints Number of control points
 * @param points Control point coordinate array
 * @param tension Tension parameter (0.0 for straight lines, higher values make curve more curved)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawcurve      (int numOfPoints, const ege_point* points, float tension, PIMAGE pimg = NULL);

/**
 * @brief Draw closed cardinal spline curve (with tension parameter)
 * @param numOfPoints Number of control points
 * @param points Control point coordinate array
 * @param tension Tension parameter (0.0 for straight lines, higher values make curve more curved)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawclosedcurve(int numOfPoints, const ege_point* points, float tension, PIMAGE pimg = NULL);

/**
 * @brief Draw filled closed cardinal spline curve (with tension parameter)
 * @param numOfPoints Number of control points
 * @param points Control point coordinate array
 * @param tension Tension parameter (0.0 for straight lines, higher values make curve more curved)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillclosedcurve(int numOfPoints, const ege_point* points, float tension, PIMAGE pimg = NULL);

/**
 * @brief Draw rectangle outline (GDI+ enhanced version)
 * @param x Rectangle top-left corner x coordinate
 * @param y Rectangle top-left corner y coordinate
 * @param w Rectangle width
 * @param h Rectangle height
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_rectangle    (float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief Draw filled rectangle (GDI+ enhanced version)
 * @param x Rectangle top-left corner x coordinate
 * @param y Rectangle top-left corner y coordinate
 * @param w Rectangle width
 * @param h Rectangle height
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillrect     (float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief Draw circle outline (GDI+ enhanced version)
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_circle       (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief Draw filled circle (GDI+ enhanced version)
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param radius Radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillcircle   (float x, float y, float radius, PIMAGE pimg = NULL);

/**
 * @brief Draw ellipse outline (GDI+ enhanced version)
 * @param x Ellipse bounding rectangle top-left corner x coordinate
 * @param y Ellipse bounding rectangle top-left corner y coordinate
 * @param w Ellipse bounding rectangle width
 * @param h Ellipse bounding rectangle height
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_ellipse      (float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief Draw filled ellipse (GDI+ enhanced version)
 * @param x Ellipse bounding rectangle top-left corner x coordinate
 * @param y Ellipse bounding rectangle top-left corner y coordinate
 * @param w Ellipse bounding rectangle width
 * @param h Ellipse bounding rectangle height
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillellipse  (float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief Draw elliptical arc (GDI+ enhanced version)
 * @param x Ellipse bounding rectangle top-left corner x coordinate
 * @param y Ellipse bounding rectangle top-left corner y coordinate
 * @param w Ellipse bounding rectangle width
 * @param h Ellipse bounding rectangle height
 * @param startAngle Start angle (degrees)
 * @param sweepAngle Sweep angle (degrees)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_arc          (float x, float y, float w, float h, float startAngle, float sweepAngle, PIMAGE pimg = NULL);

/**
 * @brief Draw pie chart outline (GDI+ enhanced version)
 * @param x Ellipse bounding rectangle top-left corner x coordinate
 * @param y Ellipse bounding rectangle top-left corner y coordinate
 * @param w Ellipse bounding rectangle width
 * @param h Ellipse bounding rectangle height
 * @param startAngle Start angle (degrees)
 * @param sweepAngle Sweep angle (degrees)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_pie          (float x, float y, float w, float h, float startAngle, float sweepAngle, PIMAGE pimg = NULL);

/**
 * @brief Draw filled pie chart (GDI+ enhanced version)
 * @param x Ellipse bounding rectangle top-left corner x coordinate
 * @param y Ellipse bounding rectangle top-left corner y coordinate
 * @param w Ellipse bounding rectangle width
 * @param h Ellipse bounding rectangle height
 * @param startAngle Start angle (degrees)
 * @param sweepAngle Sweep angle (degrees)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillpie      (float x, float y, float w, float h, float startAngle, float sweepAngle, PIMAGE pimg = NULL);

/**
 * @brief Draw rounded rectangle outline (GDI+ enhanced version)
 * @param x Rectangle top-left corner x coordinate
 * @param y Rectangle top-left corner y coordinate
 * @param w Rectangle width
 * @param h Rectangle height
 * @param radius Corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_roundrect    (float x, float y, float w, float h,  float radius, PIMAGE pimg = NULL);

/**
 * @brief Draw filled rounded rectangle (GDI+ enhanced version)
 * @param x Rectangle top-left corner x coordinate
 * @param y Rectangle top-left corner y coordinate
 * @param w Rectangle width
 * @param h Rectangle height
 * @param radius Corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillroundrect(float x, float y, float w, float h,  float radius, PIMAGE pimg = NULL);

/**
 * @brief Draw rounded rectangle outline (different radius for each corner)
 * @param x Rectangle top-left corner x coordinate
 * @param y Rectangle top-left corner y coordinate
 * @param w Rectangle width
 * @param h Rectangle height
 * @param radius1 Top-left corner radius
 * @param radius2 Top-right corner radius
 * @param radius3 Bottom-right corner radius
 * @param radius4 Bottom-left corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_roundrect    (float x, float y, float w, float h,  float radius1, float radius2, float radius3, float radius4, PIMAGE pimg = NULL);

/**
 * @brief Draw filled rounded rectangle (different radius for each corner)
 * @param x Rectangle top-left corner x coordinate
 * @param y Rectangle top-left corner y coordinate
 * @param w Rectangle width
 * @param h Rectangle height
 * @param radius1 Top-left corner radius
 * @param radius2 Top-right corner radius
 * @param radius3 Bottom-right corner radius
 * @param radius4 Bottom-left corner radius
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillroundrect(float x, float y, float w, float h,  float radius1, float radius2, float radius3, float radius4, PIMAGE pimg = NULL);

/**
 * @brief Clear fill pattern (set to no pattern)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_setpattern_none(PIMAGE pimg = NULL);

/**
 * @brief Set linear gradient fill pattern
 * @param x1 Start point x coordinate
 * @param y1 Start point y coordinate
 * @param c1 Start point color
 * @param x2 End point x coordinate
 * @param y2 End point y coordinate
 * @param c2 End point color
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_setpattern_lineargradient(float x1, float y1, color_t c1, float x2, float y2, color_t c2, PIMAGE pimg = NULL);

/**
 * @brief Set path gradient fill pattern
 * @param center Center point
 * @param centerColor Center color
 * @param count Number of boundary points
 * @param points Boundary point array
 * @param colorCount Number of boundary colors
 * @param pointColors Boundary color array
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_setpattern_pathgradient(ege_point center, color_t centerColor, int count, const ege_point* points, int colorCount, const color_t* pointColors, PIMAGE pimg = NULL);

/**
 * @brief Set ellipse gradient fill pattern
 * @param center Center point
 * @param centerColor Center color
 * @param x Ellipse top-left corner x coordinate
 * @param y Ellipse top-left corner y coordinate
 * @param w Ellipse width
 * @param h Ellipse height
 * @param color Boundary color
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_setpattern_ellipsegradient(ege_point center, color_t centerColor, float x, float y, float w, float h, color_t color, PIMAGE pimg = NULL);

/**
 * @brief Set texture fill pattern
 * @param imgSrc Source texture image
 * @param x Texture area top-left corner x coordinate
 * @param y Texture area top-left corner y coordinate
 * @param w Texture area width
 * @param h Texture area height
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_setpattern_texture(PIMAGE imgSrc, float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief Draw text (GDI+ enhanced version)
 * @param text Text to draw
 * @param x Text top-left corner x coordinate
 * @param y Text top-left corner y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawtext(const char*    text, float x, float y, PIMAGE pimg = NULL);

/**
 * @brief Draw text (GDI+ enhanced version, Unicode)
 * @param text Text to draw
 * @param x Text top-left corner x coordinate
 * @param y Text top-left corner y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawtext(const wchar_t* text, float x, float y, PIMAGE pimg = NULL);

/**
 * @brief Generate texture
 * @param generate Whether to generate texture
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_gentexture(bool generate, PIMAGE pimg = NULL);

/**
 * @brief Draw texture image
 * @param imgSrc Source texture image
 * @param x Target area top-left corner x coordinate
 * @param y Target area top-left corner y coordinate
 * @param w Target area width
 * @param h Target area height
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_puttexture(PCIMAGE imgSrc, float x, float y, float w, float h, PIMAGE pimg = NULL);

/**
 * @brief Draw texture image (specify target rectangle)
 * @param imgSrc Source texture image
 * @param dest Target rectangle
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_puttexture(PCIMAGE imgSrc, ege_rect dest, PIMAGE pimg = NULL);

/**
 * @brief Draw texture image (specify source and target rectangles)
 * @param imgSrc Source texture image
 * @param dest Target rectangle
 * @param src Source rectangle
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_puttexture(PCIMAGE imgSrc, ege_rect dest, ege_rect src, PIMAGE pimg = NULL);

//draw image
/**
 * @brief Draw image (GDI+ enhanced version)
 * @param imgSrc Source image
 * @param xDest Target x coordinate
 * @param yDest Target y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawimage(PCIMAGE imgSrc,int xDest, int yDest, PIMAGE pimg = NULL);

/**
 * @brief Draw image (GDI+ enhanced version, specify source and target areas)
 * @param imgSrc Source image
 * @param xDest Target area top-left corner x coordinate
 * @param yDest Target area top-left corner y coordinate
 * @param widthDest Target area width
 * @param heightDest Target area height
 * @param xSrc Source area top-left corner x coordinate
 * @param ySrc Source area top-left corner y coordinate
 * @param widthSrc Source area width
 * @param heightSrc Source area height
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawimage(PCIMAGE imgSrc,int xDest, int yDest, int widthDest, int heightDest, int xSrc, int ySrc, int widthSrc, int heightSrc,PIMAGE pimg = NULL);

/**
 * @brief Draw path outline
 * @param path Path object pointer
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawpath(const ege_path* path, PIMAGE pimg = NULL);

/**
 * @brief Draw filled path
 * @param path Path object pointer
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillpath(const ege_path* path, PIMAGE pimg = NULL);

/**
 * @brief Draw path outline (with offset)
 * @param path Path object pointer
 * @param x X direction offset
 * @param y Y direction offset
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_drawpath(const ege_path* path, float x, float y, PIMAGE pimg = NULL);

/**
 * @brief Draw filled path (with offset)
 * @param path Path object pointer
 * @param x X direction offset
 * @param y Y direction offset
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_fillpath(const ege_path* path, float x, float y, PIMAGE pimg = NULL);

/// @defgroup PathManagement Path management functions
/// @{

/**
 * @brief Create new path object
 * @return Path object pointer
 */
ege_path* EGEAPI ege_path_create     ();

/**
 * @brief Create path from point array and type array
 * @param points Point coordinate array
 * @param types Point type array
 * @param count Number of points
 * @return Path object pointer
 */
ege_path* EGEAPI ege_path_createfrom (const ege_point* points, const unsigned char* types, int count);

/**
 * @brief Clone path
 * @param path Source path object pointer
 * @return New path object pointer
 */
ege_path* EGEAPI ege_path_clone      (const ege_path* path);

/**
 * @brief Destroy path object
 * @param path Path object pointer
 */
void      EGEAPI ege_path_destroy    (const ege_path* path);

/**
 * @brief Start new subpath
 * @param path Path object pointer
 */
void      EGEAPI ege_path_start      (ege_path* path);

/**
 * @brief Close current subpath
 * @param path Path object pointer
 */
void      EGEAPI ege_path_close      (ege_path* path);

/**
 * @brief Close all open subpaths
 * @param path Path object pointer
 */
void      EGEAPI ege_path_closeall   (ege_path* path);

/**
 * @brief Set path fill mode
 * @param path Path object pointer
 * @param mode Fill mode
 */
void      EGEAPI ege_path_setfillmode(ege_path* path, fill_mode mode);

/**
 * @brief Reset path (clear all subpaths)
 * @param path Path object pointer
 */
void      EGEAPI ege_path_reset      (ege_path* path);

/**
 * @brief Reverse path direction
 * @param path Path object pointer
 */
void      EGEAPI ege_path_reverse    (ege_path* path);

/**
 * @brief Widen path (generate outline)
 * @param path Path object pointer
 * @param lineWidth Line width
 * @param matrix Transform matrix (optional)
 */
void      EGEAPI ege_path_widen      (ege_path* path, float lineWidth, const ege_transform_matrix* matrix = NULL);

/**
 * @brief Widen path (generate outline, specify flatness)
 * @param path Path object pointer
 * @param lineWidth Line width
 * @param matrix Transform matrix
 * @param flatness Flatness value
 */
void      EGEAPI ege_path_widen      (ege_path* path, float lineWidth, const ege_transform_matrix* matrix,  float flatness);

/**
 * @brief Flatten path (convert curves to line segments)
 * @param path Path object pointer
 * @param matrix Transform matrix (optional)
 */
void      EGEAPI ege_path_flatten    (ege_path* path, const ege_transform_matrix* matrix = NULL);

/**
 * @brief Flatten path (specify flatness)
 * @param path Path object pointer
 * @param matrix Transform matrix
 * @param flatness Flatness value
 */
void      EGEAPI ege_path_flatten    (ege_path* path, const ege_transform_matrix* matrix, float flatness);

/**
 * @brief Warp path
 * @param path Path object pointer
 * @param points Warp control point array
 * @param count Number of control points
 * @param rect Warp rectangle area
 * @param matrix Transform matrix (optional)
 */
void      EGEAPI ege_path_warp       (ege_path* path, const ege_point* points, int count, const ege_rect* rect, const ege_transform_matrix* matrix = NULL);

/**
 * @brief Warp path (specify flatness)
 * @param path Path object pointer
 * @param points Warp control point array
 * @param count Number of control points
 * @param rect Warp rectangle area
 * @param matrix Transform matrix
 * @param flatness Flatness value
 */
void      EGEAPI ege_path_warp       (ege_path* path, const ege_point* points, int count, const ege_rect* rect, const ege_transform_matrix* matrix, float flatness);

/**
 * @brief Generate path outline
 * @param path Path object pointer
 * @param matrix Transform matrix (optional)
 */
void      EGEAPI ege_path_outline    (ege_path* path, const ege_transform_matrix* matrix = NULL);

/**
 * @brief Generate path outline (specify flatness)
 * @param path Path object pointer
 * @param matrix Transform matrix
 * @param flatness Flatness value
 */
void      EGEAPI ege_path_outline    (ege_path* path, const ege_transform_matrix* matrix, float flatness);

/**
 * @brief Check if point is inside path
 * @param path Path object pointer
 * @param x Test point x coordinate
 * @param y Test point y coordinate
 * @return true if point is inside path, false otherwise
 */
bool      EGEAPI ege_path_inpath     (const ege_path* path, float x, float y);

/**
 * @brief Check if point is inside path (specify image context)
 * @param path Path object pointer
 * @param x Test point x coordinate
 * @param y Test point y coordinate
 * @param pimg Image context pointer
 * @return true if point is inside path, false otherwise
 */
bool      EGEAPI ege_path_inpath     (const ege_path* path, float x, float y, PCIMAGE pimg);

/**
 * @brief Check if point is on path outline
 * @param path Path object pointer
 * @param x Test point x coordinate
 * @param y Test point y coordinate
 * @return true if point is on path outline, false otherwise
 */
bool      EGEAPI ege_path_instroke   (const ege_path* path, float x, float y);

/**
 * @brief Check if point is on path outline (specify image context)
 * @param path Path object pointer
 * @param x Test point x coordinate
 * @param y Test point y coordinate
 * @param pimg Image context pointer
 * @return true if point is on path outline, false otherwise
 */
bool      EGEAPI ege_path_instroke   (const ege_path* path, float x, float y, PCIMAGE pimg);

/**
 * @brief Get coordinates of last point in path
 * @param path Path object pointer
 * @return Coordinates of last point
 */
ege_point      EGEAPI ege_path_lastpoint    (const ege_path* path);

/**
 * @brief Get number of points in path
 * @param path Path object pointer
 * @return Number of points in path
 */
int            EGEAPI ege_path_pointcount   (const ege_path* path);

/**
 * @brief Get path bounding rectangle
 * @param path Path object pointer
 * @param matrix Transform matrix, NULL means no transformation
 * @return Path bounding rectangle
 */
ege_rect       EGEAPI ege_path_getbounds    (const ege_path* path, const ege_transform_matrix* matrix = NULL);

/**
 * @brief Get path bounding rectangle (specify image context)
 * @param path Path object pointer
 * @param matrix Transform matrix
 * @param pimg Image context pointer
 * @return Path bounding rectangle
 */
ege_rect       EGEAPI ege_path_getbounds    (const ege_path* path, const ege_transform_matrix* matrix, PCIMAGE pimg);

/**
 * @brief Get all points in path
 * @param path Path object pointer
 * @param points Point array buffer, NULL means auto-allocate
 * @return Pointer to point array
 * @note If points is NULL, function will allocate memory, caller is responsible for freeing it
 */
ege_point*     EGEAPI ege_path_getpathpoints(const ege_path* path, ege_point* points = NULL);

/**
 * @brief Get all point types in path
 * @param path Path object pointer
 * @param types Type array buffer, NULL means auto-allocate
 * @return Pointer to type array
 * @note If types is NULL, function will allocate memory, caller is responsible for freeing it
 */
unsigned char* EGEAPI ege_path_getpathtypes (const ege_path* path, unsigned char* types = NULL);

/**
 * @brief Transform path
 * @param path Path object pointer
 * @param matrix Transform matrix
 */
void EGEAPI ege_path_transform     (ege_path* path, const ege_transform_matrix* matrix);

/// @defgroup PathAdd Path addition functions
/// @{

/**
 * @brief Add another path to path
 * @param dstPath Target path object pointer
 * @param srcPath Source path object pointer
 * @param connect Whether to connect to the last point of current path
 */
void EGEAPI ege_path_addpath       (ege_path* dstPath, const ege_path* srcPath, bool connect);

/**
 * @brief Add line segment to path
 * @param path Path object pointer
 * @param x1 Start point x coordinate
 * @param y1 Start point y coordinate
 * @param x2 End point x coordinate
 * @param y2 End point y coordinate
 */
void EGEAPI ege_path_addline       (ege_path* path, float x1, float y1, float x2, float y2);

/**
 * @brief Add arc to path
 * @param path Path object pointer
 * @param x Ellipse top-left corner x coordinate
 * @param y Ellipse top-left corner y coordinate
 * @param width Ellipse width
 * @param height Ellipse height
 * @param startAngle Start angle (degrees)
 * @param sweepAngle Sweep angle (degrees)
 */
void EGEAPI ege_path_addarc        (ege_path* path, float x, float y, float width, float height, float startAngle, float sweepAngle);

/**
 * @brief Add polyline to path
 * @param path Path object pointer
 * @param numOfPoints Number of points
 * @param points Point array
 */
void EGEAPI ege_path_addpolyline   (ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief Add Bezier curve to path (point array version)
 * @param path Path object pointer
 * @param numOfPoints Number of control points
 * @param points Control point array
 */
void EGEAPI ege_path_addbezier     (ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief Add Bezier curve to path (coordinate version)
 * @param path Path object pointer
 * @param x1 Start point x coordinate
 * @param y1 Start point y coordinate
 * @param x2 First control point x coordinate
 * @param y2 First control point y coordinate
 * @param x3 Second control point x coordinate
 * @param y3 Second control point y coordinate
 * @param x4 End point x coordinate
 * @param y4 End point y coordinate
 */
void EGEAPI ege_path_addbezier     (ege_path* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

/**
 * @brief Add cardinal spline curve to path
 * @param path Path object pointer
 * @param numOfPoints Number of control points
 * @param points Control point array
 */
void EGEAPI ege_path_addcurve      (ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief Add cardinal spline curve to path (specify tension)
 * @param path Path object pointer
 * @param numOfPoints Number of control points
 * @param points Control point array
 * @param tension Tension value (0.0-1.0)
 */
void EGEAPI ege_path_addcurve      (ege_path* path, int numOfPoints, const ege_point* points, float tension);

/**
 * @brief Add circle to path
 * @param path Path object pointer
 * @param x Circle center x coordinate
 * @param y Circle center y coordinate
 * @param radius Radius
 */
void EGEAPI ege_path_addcircle     (ege_path* path, float x, float y, float radius);

/**
 * @brief Add rectangle to path
 * @param path Path object pointer
 * @param x Rectangle top-left corner x coordinate
 * @param y Rectangle top-left corner y coordinate
 * @param width Rectangle width
 * @param height Rectangle height
 */
void EGEAPI ege_path_addrect       (ege_path* path, float x, float y, float width, float height);

/**
 * @brief Add ellipse to path
 * @param path Path object pointer
 * @param x Ellipse bounding top-left corner x coordinate
 * @param y Ellipse bounding top-left corner y coordinate
 * @param width Ellipse width
 * @param height Ellipse height
 */
void EGEAPI ege_path_addellipse    (ege_path* path, float x, float y, float width, float height);

/**
 * @brief Add pie chart to path
 * @param path Path object pointer
 * @param x X coordinate of ellipse bounding box top-left corner
 * @param y Y coordinate of ellipse bounding box top-left corner
 * @param width Ellipse width
 * @param height Ellipse height
 * @param startAngle Start angle (degrees)
 * @param sweepAngle Sweep angle (degrees)
 */
void EGEAPI ege_path_addpie        (ege_path* path, float x, float y, float width, float height, float startAngle, float sweepAngle);

/**
 * @brief Add text to path (ANSI version)
 * @param path Path object pointer
 * @param x Text starting x coordinate
 * @param y Text starting y coordinate
 * @param text Text content
 * @param height Text height
 * @param length Text length, -1 means auto-calculate
 * @param typeface Font name, NULL means use default font
 * @param fontStyle Font style
 */
void EGEAPI ege_path_addtext       (ege_path* path, float x, float y, const char*    text, float height, int length = -1, const char*    typeface = NULL, int fontStyle = 0);

/**
 * @brief Add text to path (Unicode version)
 * @param path Path object pointer
 * @param x Text starting x coordinate
 * @param y Text starting y coordinate
 * @param text Text content
 * @param height Text height
 * @param length Text length, -1 means auto-calculate
 * @param typeface Font name, NULL means use default font
 * @param fontStyle Font style
 */
void EGEAPI ege_path_addtext       (ege_path* path, float x, float y, const wchar_t* text, float height, int length = -1, const wchar_t* typeface = NULL, int fontStyle = 0);

/**
 * @brief Add polygon to path
 * @param path Path object pointer
 * @param numOfPoints Number of vertices
 * @param points Vertex array
 */
void EGEAPI ege_path_addpolygon    (ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief Add closed cardinal spline curve to path
 * @param path Path object pointer
 * @param numOfPoints Number of control points
 * @param points Control point array
 */
void EGEAPI ege_path_addclosedcurve(ege_path* path, int numOfPoints, const ege_point* points);

/**
 * @brief Add closed cardinal spline curve to path (with specified tension)
 * @param path Path object pointer
 * @param numOfPoints Number of control points
 * @param points Control point array
 * @param tension Tension value (0.0-1.0)
 */
void EGEAPI ege_path_addclosedcurve(ege_path* path, int numOfPoints, const ege_point* points, float tension);

/// @}


/// @defgroup Transform Transform functions
/// @{

/**
 * @brief Rotation transformation
 * @param angle Rotation angle (degrees)
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_transform_rotate(float angle, PIMAGE pimg = NULL);

/**
 * @brief Translation transformation
 * @param x Translation amount in x direction
 * @param y Translation amount in y direction
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_transform_translate(float x, float y, PIMAGE pimg = NULL);

/**
 * @brief Scaling transformation
 * @param xScale Scaling ratio in x direction
 * @param yScale Scaling ratio in y direction
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_transform_scale(float xScale, float yScale, PIMAGE pimg = NULL);

/**
 * @brief Reset transformation matrix to identity matrix
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_transform_reset(PIMAGE pimg = NULL);

/**
 * @brief Get current transformation matrix
 * @param matrix Pointer to output transformation matrix
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_get_transform(ege_transform_matrix* matrix, PIMAGE pimg = NULL);

/**
 * @brief Set transformation matrix
 * @param matrix Transformation matrix to set
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI ege_set_transform(const ege_transform_matrix* matrix, PIMAGE pimg = NULL);

/**
 * @brief Calculate point coordinates after transformation
 * @param p Original point coordinates
 * @param pimg Target image pointer, NULL means current ege window
 * @return Transformed point coordinates
 */
ege_point EGEAPI ege_transform_calc(ege_point p, PIMAGE pimg = NULL);

/// @}

/**
 * @brief Calculate point coordinates after transformation (coordinate version)
 * @param x Original point x coordinate
 * @param y Original point y coordinate
 * @param pimg Target image pointer, NULL means current ege window
 * @return Transformed point coordinates
 */
ege_point EGEAPI ege_transform_calc(float x, float y, PIMAGE pimg = NULL);


#endif

// It is not supported in VC 6.0.
#ifndef EGE_COMPILERINFO_VC6

/// @defgroup Console Console functions
/// @{

/**
 * @brief Initialize console
 * @return Returns true on success, false on failure
 * @note Creates a console window for standard input/output
 */
bool EGEAPI init_console();

/**
 * @brief Clear console
 * @return Returns true on success, false on failure
 */
bool EGEAPI clear_console();

/**
 * @brief Show console window
 * @return Returns true on success, false on failure
 */
bool EGEAPI show_console();

/**
 * @brief Hide console window
 * @return Returns true on success, false on failure
 */
bool EGEAPI hide_console();

/**
 * @brief Close console and restore standard input/output
 * @return Returns true on success, false on failure
 */
bool EGEAPI close_console();
#endif

/**
 * @brief Get character from console (replacement for getch function in <conio.h>)
 * @return Character code obtained
 */
int  EGEAPI getch_console();

/**
 * @brief Check if console has key press (replacement for kbhit function in <conio.h>)
 * @return Returns non-zero if key pressed, 0 if no key pressed
 */
int  EGEAPI kbhit_console();

/// @}

/// @defgroup Time Time related functions
/// @{

/**
 * @brief EGE library delay function
 * @param ms Delay time (milliseconds)
 * @note Precise delay function, not affected by system scheduling
 */
void EGEAPI ege_sleep (long ms);

/**
 * @brief Delay function
 * @param ms Delay time (milliseconds)
 * @note Standard delay function
 */
void EGEAPI delay     (long ms);

/**
 * @brief Millisecond delay function
 * @param ms Delay time (milliseconds)
 */
void EGEAPI delay_ms  (long ms);

/**
 * @brief API delay function
 * @param ms Delay time (milliseconds)
 * @note Delay implemented using system API
 */
void EGEAPI api_sleep (long ms);

/**
 * @brief Delay by frame rate (integer version)
 * @param fps Target frame rate
 * @note Automatically calculates delay time based on frame rate
 */
void EGEAPI delay_fps (int    fps);

/**
 * @brief Delay by frame rate (long integer version)
 * @param fps Target frame rate
 */
void EGEAPI delay_fps (long   fps);

/**
 * @brief Delay by frame rate (double precision floating-point version)
 * @param fps Target frame rate
 */
void EGEAPI delay_fps (double fps);

/**
 * @brief Frame rate delay with frame skipping
 * @param fps Target frame rate
 * @note More precise frame rate control
 */
void EGEAPI delay_jfps(int    fps);

/**
 * @brief Precise frame rate delay (long integer version)
 * @param fps Target frame rate
 */
void EGEAPI delay_jfps(long   fps);

/**
 * @brief Precise frame rate delay (double precision floating-point version)
 * @param fps Target frame rate
 */
void EGEAPI delay_jfps(double fps);

/**
 * @brief Get high-precision clock
 * @return Current time (seconds, double precision floating-point)
 * @note Used for high-precision timing and performance measurement
 */
double EGEAPI fclock();

/// @}

/**
 * @defgroup TextOutput Text output
 * @brief Text output and display related functions
 * @{
 */

/**
 * @brief Output text string at current position
 * @param text Text string to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Text output position is determined by current drawing position, set using moveto()
 */
void EGEAPI outtext(const char*    text, PIMAGE pimg = NULL);

/**
 * @brief Output text string at current position (Unicode version)
 * @param text Text string to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Text output position is determined by current drawing position, set using moveto()
 */
void EGEAPI outtext(const wchar_t* text, PIMAGE pimg = NULL);

/**
 * @brief Output single character at current position
 * @param c Character to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Text output position is determined by current drawing position, set using moveto()
 */
void EGEAPI outtext(char    c, PIMAGE pimg = NULL);

/**
 * @brief Output single character at current position (Unicode version)
 * @param c Character to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Text output position is determined by current drawing position, set using moveto()
 */
void EGEAPI outtext(wchar_t c, PIMAGE pimg = NULL);

/**
 * @brief Output text string at specified position
 * @param x X coordinate of output position
 * @param y Y coordinate of output position
 * @param text Text string to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI outtextxy(int x, int y, const char*    text, PIMAGE pimg = NULL);

/**
 * @brief Output text string at specified position (Unicode version)
 * @param x X coordinate of output position
 * @param y Y coordinate of output position
 * @param text Text string to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI outtextxy(int x, int y, const wchar_t* text, PIMAGE pimg = NULL);

/**
 * @brief Output single character at specified position
 * @param x X coordinate of output position
 * @param y Y coordinate of output position
 * @param c Character to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI outtextxy(int x, int y, char    c, PIMAGE pimg = NULL);

/**
 * @brief Output single character at specified position (Unicode version)
 * @param x X coordinate of output position
 * @param y Y coordinate of output position
 * @param c Character to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI outtextxy(int x, int y, wchar_t c, PIMAGE pimg = NULL);

/**
 * @brief Format and output text at specified position
 * @param x X coordinate of output position
 * @param y Y coordinate of output position
 * @param format Format string (similar to printf)
 * @param ... Variable argument list
 * @note Actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI xyprintf (int x, int y, const char*    format, ...);

/**
 * @brief Format and output text at specified position (Unicode version)
 * @param x X coordinate of output position
 * @param y Y coordinate of output position
 * @param format Format string (similar to printf)
 * @param ... Variable argument list
 * @note Actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI xyprintf (int x, int y, const wchar_t* format, ...);

/**
 * @brief Output text within specified rectangular area
 * @param x X coordinate of rectangle area top-left corner
 * @param y Y coordinate of rectangle area top-left corner
 * @param w Width of rectangle area
 * @param h Height of rectangle area
 * @param text Text string to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Text will wrap automatically within the specified rectangle area, supports text alignment settings
 */
void EGEAPI outtextrect(int x, int y, int w, int h, const char*    text, PIMAGE pimg = NULL);

/**
 * @brief Output text within specified rectangular area (Unicode version)
 * @param x X coordinate of rectangle area top-left corner
 * @param y Y coordinate of rectangle area top-left corner
 * @param w Width of rectangle area
 * @param h Height of rectangle area
 * @param text Text string to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Text will wrap automatically within the specified rectangle area, supports text alignment settings
 */
void EGEAPI outtextrect(int x, int y, int w, int h, const wchar_t* text, PIMAGE pimg = NULL);

/**
 * @brief Format and output text within specified rectangular area
 * @param x X coordinate of rectangle area top-left corner
 * @param y Y coordinate of rectangle area top-left corner
 * @param w Width of rectangle area
 * @param h Height of rectangle area
 * @param format Format string (similar to printf)
 * @param ... Variable argument list
 * @note Text will wrap automatically within the specified rectangle area, supports text alignment settings
 */
void EGEAPI rectprintf (int x, int y, int w, int h, const char*    format, ...);

/**
 * @brief Format and output text within specified rectangular area (Unicode version)
 * @param x X coordinate of rectangle area top-left corner
 * @param y Y coordinate of rectangle area top-left corner
 * @param w Width of rectangle area
 * @param h Height of rectangle area
 * @param format Format string (similar to printf)
 * @param ... Variable argument list
 * @note Text will wrap automatically within the specified rectangle area, supports text alignment settings
 */
void EGEAPI rectprintf (int x, int y, int w, int h, const wchar_t* format, ...);

/**
 * @brief Get display width of text string
 * @param text Text string to measure
 * @param pimg Target image pointer, NULL means current ege window
 * @return Text display width (pixels)
 * @note Return value is affected by current font settings
 */
int  EGEAPI textwidth(const char*    text, PCIMAGE pimg = NULL);

/**
 * @brief Get display width of text string (Unicode version)
 * @param text Text string to measure
 * @param pimg Target image pointer, NULL means current ege window
 * @return Text display width (pixels)
 * @note Return value is affected by current font settings
 */
int  EGEAPI textwidth(const wchar_t* text, PCIMAGE pimg = NULL);

/**
 * @brief Get display width of single character
 * @param c Character to measure
 * @param pimg Target image pointer, NULL means current ege window
 * @return Character display width (pixels)
 * @note Return value is affected by current font settings
 */
int  EGEAPI textwidth(char    c, PCIMAGE pimg = NULL);

/**
 * @brief Get display width of single character (Unicode version)
 * @param c Character to measure
 * @param pimg Target image pointer, NULL means current ege window
 * @return Character display width (pixels)
 * @note Return value is affected by current font settings
 */
int  EGEAPI textwidth(wchar_t c, PCIMAGE pimg = NULL);

/**
 * @brief Get display height of text string
 * @param text Text string to measure
 * @param pimg Target image pointer, NULL means current ege window
 * @return Text display height (pixels)
 * @note Return value is affected by current font settings
 */
int  EGEAPI textheight(const char*    text, PCIMAGE pimg = NULL);

/**
 * @brief Get display height of text string (Unicode version)
 * @param text Text string to measure
 * @param pimg Target image pointer, NULL means current ege window
 * @return Text display height (pixels)
 * @note Return value is affected by current font settings
 */
int  EGEAPI textheight(const wchar_t* text, PCIMAGE pimg = NULL);

/**
 * @brief Get display height of single character
 * @param c Character to measure
 * @param pimg Target image pointer, NULL means current ege window
 * @return Character display height (pixels)
 * @note Return value is affected by current font settings
 */
int  EGEAPI textheight(char    c, PCIMAGE pimg = NULL);

/**
 * @brief Get display height of single character (Unicode version)
 * @param c Character to measure
 * @param pimg Target image pointer, NULL means current ege window
 * @return Character display height (pixels)
 * @note Return value is affected by current font settings
 */
int  EGEAPI textheight(wchar_t c, PCIMAGE pimg = NULL);

/**
 * @brief Output text at specified position (supports floating-point coordinates and ARGB colors)
 * @param x X coordinate of output position (floating-point)
 * @param y Y coordinate of output position (floating-point)
 * @param text Text string to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Uses GDI+ rendering, supports ARGB colors and anti-aliasing, actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI ege_outtextxy(float x, float y, const char*    text, PIMAGE pimg = NULL);

/**
 * @brief Output text at specified position (supports floating-point coordinates and ARGB colors, Unicode version)
 * @param x X coordinate of output position (floating-point)
 * @param y Y coordinate of output position (floating-point)
 * @param text Text string to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Uses GDI+ rendering, supports ARGB colors and anti-aliasing, actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI ege_outtextxy(float x, float y, const wchar_t* text, PIMAGE pimg = NULL);

/**
 * @brief Output single character at specified position (supports floating-point coordinates and ARGB colors)
 * @param x X coordinate of output position (floating-point)
 * @param y Y coordinate of output position (floating-point)
 * @param c Character to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Uses GDI+ rendering, supports ARGB colors and anti-aliasing, actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI ege_outtextxy(float x, float y, char    c, PIMAGE pimg = NULL);

/**
 * @brief Output single character at specified position (supports floating-point coordinates and ARGB colors, Unicode version)
 * @param x X coordinate of output position (floating-point)
 * @param y Y coordinate of output position (floating-point)
 * @param c Character to output
 * @param pimg Target image pointer, NULL means current ege window
 * @note Uses GDI+ rendering, supports ARGB colors and anti-aliasing, actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI ege_outtextxy(float x, float y, wchar_t c, PIMAGE pimg = NULL);

/**
 * @brief Format and output text at specified position (supports floating-point coordinates and ARGB colors)
 * @param x X coordinate of output position (floating-point)
 * @param y Y coordinate of output position (floating-point)
 * @param format Format string (similar to printf)
 * @param ... Variable argument list
 * @note Uses GDI+ rendering, supports ARGB colors and anti-aliasing, actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI ege_xyprintf (float x, float y, const char*    format, ...);

/**
 * @brief Format and output text at specified position (supports floating-point coordinates and ARGB colors, Unicode version)
 * @param x X coordinate of output position (floating-point)
 * @param y Y coordinate of output position (floating-point)
 * @param format Format string (similar to printf)
 * @param ... Variable argument list
 * @note Uses GDI+ rendering, supports ARGB colors and anti-aliasing, actual output position is affected by text alignment and font tilt angle
 */
void EGEAPI ege_xyprintf (float x, float y, const wchar_t* format, ...);

/// @}

/**
 * @defgroup FontSettings Font settings
 * @brief Font and text alignment related setting functions
 * @{
 */

/**
 * @brief Set text alignment mode
 * @param horiz Horizontal alignment (LEFT_TEXT, CENTER_TEXT, RIGHT_TEXT)
 * @param vert Vertical alignment (TOP_TEXT, CENTER_TEXT, BOTTOM_TEXT)
 * @param pimg Target image pointer, NULL means current ege window
 * @note Affects alignment of all subsequent text output functions
 */
void EGEAPI settextjustify(int horiz, int vert, PIMAGE pimg = NULL);

/**
 * @brief Set font (simplified version)
 * @param height Font height (pixels)
 * @param width Font width (pixels), 0 means automatic
 * @param typeface Font name
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setfont(int height, int width, const char* typeface,  PIMAGE pimg = NULL);

/**
 * @brief Set font (simplified version, Unicode)
 * @param height Font height (pixels)
 * @param width Font width (pixels), 0 means automatic
 * @param typeface Font name
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setfont(int height, int width, const wchar_t* typeface, PIMAGE pimg = NULL);

/**
 * @brief Set font (complete version)
 * @param height Font height (pixels)
 * @param width Font width (pixels), 0 means automatic
 * @param typeface Font name
 * @param escapement Font tilt angle (in tenths of a degree)
 * @param orientation Character tilt angle (in tenths of a degree)
 * @param weight Font thickness (100-900, 400 is normal, 700 is bold)
 * @param italic Whether italic
 * @param underline Whether underline
 * @param strikeOut Whether strikethrough
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setfont(int height, int width, const char* typeface,  int escapement, int orientation,
                    int weight, bool italic, bool underline, bool strikeOut, PIMAGE pimg = NULL);

/**
 * @brief Set font (complete version, Unicode)
 * @param height Font height (pixels)
 * @param width Font width (pixels), 0 means automatic
 * @param typeface Font name
 * @param escapement Font tilt angle (in tenths of a degree)
 * @param orientation Character tilt angle (in tenths of a degree)
 * @param weight Font thickness (100-900, 400 is normal, 700 is bold)
 * @param italic Whether italic
 * @param underline Whether underline
 * @param strikeOut Whether strikethrough
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setfont(int height, int width, const wchar_t* typeface, int escapement, int orientation,
                    int weight, bool italic, bool underline, bool strikeOut, PIMAGE pimg = NULL);

/**
 * @brief Set font (advanced version)
 * @param height Font height (pixels)
 * @param width Font width (pixels), 0 means automatic
 * @param typeface Font name
 * @param escapement Font tilt angle (in tenths of a degree)
 * @param orientation Character tilt angle (in tenths of a degree)
 * @param weight Font thickness (100-900, 400 is normal, 700 is bold)
 * @param italic Whether italic
 * @param underline Whether underline
 * @param strikeOut Whether strikethrough
 * @param charSet Character set
 * @param outPrecision Output precision
 * @param clipPrecision Clipping precision
 * @param quality Output quality
 * @param pitchAndFamily Font pitch and family information
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setfont(int height, int width, const char* typeface,  int escapement, int orientation,
                    int weight, bool italic, bool underline, bool strikeOut, BYTE charSet,
                    BYTE outPrecision, BYTE clipPrecision, BYTE quality, BYTE pitchAndFamily, PIMAGE pimg = NULL);

/**
 * @brief Set font (advanced version, Unicode)
 * @param height Font height (pixels)
 * @param width Font width (pixels), 0 means automatic
 * @param typeface Font name
 * @param escapement Font tilt angle (in tenths of a degree)
 * @param orientation Character tilt angle (in tenths of a degree)
 * @param weight Font thickness (100-900, 400 is normal, 700 is bold)
 * @param italic Whether italic
 * @param underline Whether underline
 * @param strikeOut Whether strikethrough
 * @param charSet Character set
 * @param outPrecision Output precision
 * @param clipPrecision Clipping precision
 * @param quality Output quality
 * @param pitchAndFamily Font pitch and family information
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setfont(int height, int width, const wchar_t* typeface, int escapement, int orientation,
                    int weight, bool italic, bool underline, bool strikeOut, BYTE charSet,
                    BYTE outPrecision, BYTE clipPrecision, BYTE quality, BYTE pitchAndFamily, PIMAGE pimg = NULL);

/**
 * @brief Set font using LOGFONTW structure
 * @param font Pointer to LOGFONTW structure
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI setfont(const LOGFONTW *font, PIMAGE pimg = NULL);

/**
 * @brief Get current font settings
 * @param font LOGFONTW structure pointer to receive font information
 * @param pimg Target image pointer, NULL means current ege window
 */
void EGEAPI getfont(LOGFONTW *font, PCIMAGE pimg = NULL);

/**
 * @brief Set font using LOGFONTA structure
 * @param font Pointer to LOGFONTA structure
 * @param pimg Target image pointer, NULL means current ege window
 * @deprecated Recommend using setfont function with LOGFONTW parameter
 */
EGE_DEPRECATE(setfont, "Please use the 'getfont' function with the LOGFONTW* parameter instead.")
void EGEAPI setfont(const LOGFONTA *font, PIMAGE pimg = NULL);

/**
 * @brief Get current font settings (ANSI version)
 * @param font LOGFONTA structure pointer to receive font information
 * @param pimg Target image pointer, NULL means current ege window
 * @deprecated Recommend using getfont function with LOGFONTW parameter
 */
EGE_DEPRECATE(getfont, "Please use the 'getfont' function with the LOGFONTW* parameter instead.")
void EGEAPI getfont(LOGFONTA *font, PCIMAGE pimg = NULL);

/// @}

#define getmaxx getwidth
#define getmaxy getheight

/**
 * @brief Get width of window or image
 * @param pimg Image object pointer, NULL means get width of current drawing window
 * @return Returns width of window or image (pixels), returns 0 if image object is invalid
 * 
 * This function is used to get the width of the specified image or window. When pimg is NULL,
 * it returns the width of the current EGE drawing window; when pimg points to a valid image, it returns the width of that image.
 * 
 * @see getheight() Get height
 * @see getx() Get current x coordinate
 * @see gety() Get current y coordinate
 */
int EGEAPI getwidth(PCIMAGE pimg = NULL);

/**
 * @brief Get height of window or image
 * @param pimg Image object pointer, NULL means get height of current drawing window
 * @return Returns height of window or image (pixels), returns 0 if image object is invalid
 * 
 * This function is used to get the height of the specified image or window. When pimg is NULL,
 * it returns the height of the current EGE drawing window; when pimg points to a valid image, it returns the height of that image.
 * 
 * @see getwidth() Get width
 * @see getx() Get current x coordinate
 * @see gety() Get current y coordinate
 */
int EGEAPI getheight(PCIMAGE pimg = NULL);

/**
 * @brief Get x coordinate of current pen position
 * @param pimg Image object pointer, NULL means get pen position of current drawing window
 * @return Returns x coordinate of current pen position, returns -1 if image object is invalid
 * 
 * Get the x coordinate of the current drawing position. The pen position is usually set by functions like moveto(), lineto(),
 * or changed by the execution of drawing functions (like line()).
 * 
 * @see gety() Get current y coordinate
 * @see moveto() Move pen to specified position
 * @see getwidth() Get width
 * @see getheight() Get height
 */
int EGEAPI getx(PCIMAGE pimg = NULL);

/**
 * @brief Get y coordinate of current pen position
 * @param pimg Image object pointer, NULL means get pen position of current drawing window
 * @return Returns y coordinate of current pen position, returns -1 if image object is invalid
 * 
 * Get the y coordinate of the current drawing position. The pen position is usually set by functions like moveto(), lineto(),
 * or changed by the execution of drawing functions (like line()).
 * 
 * @see getx() Get current x coordinate
 * @see moveto() Move pen to specified position
 * @see getwidth() Get width
 * @see getheight() Get height
 */
int EGEAPI gety(PCIMAGE pimg = NULL);

/**
 * @brief Create a new image object (1x1 pixels)
 * @return Returns pointer to newly created image object, returns NULL on failure
 * 
 * Creates an image object with size 1x1 pixels, background color is black.
 * The created image needs to be destroyed using delimage() function to prevent memory leaks.
 * 
 * @note This function creates the minimum size image, usually used for subsequent resizing or as placeholder
 * @warning Must use delimage() to destroy the created image, otherwise it will cause memory leaks
 * 
 * @see newimage(int, int) Create image with specified size
 * @see delimage() Destroy image object
 * @see resize() Resize image
 */
PIMAGE         EGEAPI newimage();

/**
 * @brief Create new image object with specified size
 * @param width Image width (pixels), automatically adjusted to 1 if less than 1
 * @param height Image height (pixels), automatically adjusted to 1 if less than 1
 * @return Returns pointer to newly created image object, returns NULL on failure
 * 
 * Creates an image object with specified size, background color is black.
 * If the specified width or height is less than 1, it will be automatically adjusted to 1.
 * The created image needs to be destroyed using delimage() function to prevent memory leaks.
 * 
 * @warning Must use delimage() to destroy the created image, otherwise it will cause memory leaks
 * 
 * @see newimage() Create 1x1 pixel image
 * @see delimage() Destroy image object
 * @see resize() Resize image
 */
PIMAGE         EGEAPI newimage(int width, int height);

/**
 * @brief Destroy image object and free memory
 * @param pimg Image object pointer to destroy, ignored if NULL
 * 
 * Destroys image object created by newimage() function, freeing related memory and system resources.
 * The image pointer should not be used after destruction.
 * 
 * @note Passing NULL pointer is safe, the function will ignore NULL parameter
 * @warning Do not use the image pointer after destroying the image, otherwise it may cause program crash
 * 
 * @see newimage() Create image object
 * @see newimage(int, int) Create image object with specified size
 */
void           EGEAPI delimage(PCIMAGE pimg);
//==================================================================================
// Image management and processing functions - Image operation functionality of EGE graphics library
//==================================================================================
/**
 * @defgroup image_management Image management and processing functions
 * @brief Image creation, loading, saving and processing functionality of EGE graphics library
 * 
 * The image management module provides complete image operation capabilities, including:
 * - Image buffer operations: getbuffer() get pixel buffer
 * - Image resizing: resize(), resize_f() adjust image size
 * - Image acquisition: getimage() series functions to get image data from different sources
 * - Image saving: saveimage(), savepng(), savebmp() save images to files
 * 
 * Supported image formats: PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * Support getting images from window, file, resource, other IMAGE objects
 * @{
 */

/**
 * @brief Get image pixel buffer pointer
 * @param pimg Image object pointer to get buffer from, default is NULL (represents window)
 * @return First address of image buffer, buffer is one-dimensional array with size = image width × image height
 * @note Pixel at coordinate (x, y) corresponds to buffer index: buffer[y * width + x]
 * @note Returned pointer can directly manipulate pixel data, changes take effect immediately
 */
color_t*       EGEAPI getbuffer(PIMAGE pimg);

/**
 * @brief Get image pixel buffer pointer (read-only version)
 * @param pimg Image object pointer to get buffer from, default is NULL (represents window)
 * @return First address of image buffer (read-only), buffer is one-dimensional array with size = image width × image height
 * @note Pixel at coordinate (x, y) corresponds to buffer index: buffer[y * width + x]
 * @note Returned pointer can only read pixel data, cannot modify
 */
const color_t* EGEAPI getbuffer(PCIMAGE pimg);

/**
 * @brief Resize image (fast version)
 * @param pimg Image object pointer to resize, cannot be NULL
 * @param width New width of image
 * @param height New height of image
 * @return Returns 0 on success, non-zero value on failure
 * @note Image content is undefined after resizing, viewport is reset to initial state
 * @note This function is fast but does not preserve original image content
 * @warning Runtime exception will occur if pimg is invalid pointer
 */
int  EGEAPI resize_f(PIMAGE pimg, int width, int height);

/**
 * @brief Resize image (standard version)
 * @param pimg Image object pointer to resize, cannot be NULL
 * @param width New width of image
 * @param height New height of image
 * @return Returns 0 on success, non-zero value on failure
 * @note Image is filled with background color after resizing, viewport is reset to initial state
 * @note This function clears image content and fills with background color
 * @warning Runtime exception will occur if pimg is invalid pointer
 */
int  EGEAPI resize  (PIMAGE pimg, int width, int height);

/**
 * @brief Get image from window
 * @param imgDest IMAGE object pointer to save image
 * @param xSrc X coordinate of top-left corner of region to get image from
 * @param ySrc Y coordinate of top-left corner of region to get image from
 * @param widthSrc Width of region to get image from
 * @param heightSrc Height of region to get image from
 * @return Returns grOk(0) on success, corresponding error code on failure
 * @note Get image data from specified region of current window
 * @see getimage(PIMAGE, PCIMAGE, int, int, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, int xSrc, int ySrc, int widthSrc, int heightSrc);

/**
 * @brief Get image from another IMAGE object
 * @param imgDest IMAGE object pointer to save image
 * @param imgSrc Source IMAGE object pointer
 * @param xSrc X coordinate of top-left corner of region to get image from
 * @param ySrc Y coordinate of top-left corner of region to get image from
 * @param widthSrc Width of region to get image from
 * @param heightSrc Height of region to get image from
 * @return Returns grOk(0) on success, corresponding error code on failure
 * @note Copy image data from specified region of source IMAGE object to destination IMAGE object
 * @see getimage(PIMAGE, int, int, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, PCIMAGE imgSrc, int xSrc, int ySrc, int widthSrc, int heightSrc);

/**
 * @brief Get image from image file (char* version)
 * @param imgDest IMAGE object pointer to save image
 * @param imageFile Image file name
 * @param zoomWidth Set image scaling width, 0 means use original width, no scaling
 * @param zoomHeight Set image scaling height, 0 means use original height, no scaling
 * @return Returns grOk(0) on success, corresponding error code on failure (grAllocError/grFileNotFound/grNullPointer/grIOerror)
 * @note Supported formats: PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * @note If image contains multiple frames, only get first frame
 * @see getimage(PIMAGE, const wchar_t*, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, const char*  imageFile, int zoomWidth = 0, int zoomHeight = 0);

/**
 * @brief Get image from image file (wchar_t* version)
 * @param imgDest IMAGE object pointer to save image
 * @param imageFile Image file name (wide character version)
 * @param zoomWidth Set image scaling width, 0 means use original width, no scaling
 * @param zoomHeight Set image scaling height, 0 means use original height, no scaling
 * @return Returns grOk(0) on success, corresponding error code on failure (grAllocError/grFileNotFound/grNullPointer/grIOerror)
 * @note Supported formats: PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * @note If image contains multiple frames, only get first frame
 * @see getimage(PIMAGE, const char*, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, const wchar_t* imageFile, int zoomWidth = 0, int zoomHeight = 0);

/**
 * @brief Get image from resource file (char* version)
 * @param imgDest IMAGE object pointer to save image
 * @param resType Resource type
 * @param resName Resource name
 * @param zoomWidth Set image scaling width, 0 means use original width, no scaling
 * @param zoomHeight Set image scaling height, 0 means use original height, no scaling
 * @return Returns grOk(0) on success, corresponding error code on failure (grAllocError/grFileNotFound/grNullPointer/grIOerror)
 * @note Supported formats: PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * @note If image contains multiple frames, only get first frame
 * @see getimage(PIMAGE, const wchar_t*, const wchar_t*, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, const char*  resType, const char*  resName, int zoomWidth = 0, int zoomHeight = 0);

/**
 * @brief Get image from resource file (wchar_t* version)
 * @param imgDest IMAGE object pointer to save image
 * @param resType Resource type (wide character version)
 * @param resName Resource name (wide character version)
 * @param zoomWidth Set image scaling width, 0 means use original width, no scaling
 * @param zoomHeight Set image scaling height, 0 means use original height, no scaling
 * @return Returns grOk(0) on success, corresponding error code on failure (grAllocError/grFileNotFound/grNullPointer/grIOerror)
 * @note Supported formats: PNG, BMP, JPG, GIF, EMF, WMF, ICO
 * @note If image contains multiple frames, only get first frame
 * @see getimage(PIMAGE, const char*, const char*, int, int)
 */
int  EGEAPI getimage(PIMAGE imgDest, const wchar_t* resType, const wchar_t* resName, int zoomWidth = 0, int zoomHeight = 0);

/**
 * @brief Get image from PNG image file (char* version)
 * @param pimg Image object pointer, need to create first using newimage()
 * @param filename Image file name
 * @return Returns 0 on success, non-zero value on failure
 * @note Image size after getting is consistent with original image size, not the original size of the image
 * @note Specifically used for processing PNG format images
 * @warning Runtime exception will occur if pimg is invalid pointer
 * @see getimage_pngfile(PIMAGE, const wchar_t*)
 */
int  EGEAPI getimage_pngfile(PIMAGE pimg, const char*  filename);

/**
 * @brief Get image from PNG image file (wchar_t* version)
 * @param pimg Image object pointer, need to create first using newimage()
 * @param filename Image file name (wide character version)
 * @return Returns 0 on success, non-zero value on failure
 * @note Image size after getting is consistent with original image size, not the original size of the image
 * @note Specifically used for processing PNG format images
 * @warning Runtime exception will occur if pimg is invalid pointer
 * @see getimage_pngfile(PIMAGE, const char*)
 */
int  EGEAPI getimage_pngfile(PIMAGE pimg, const wchar_t* filename);

//==================================================================================
// putimage series functions - Core image drawing functionality of EGE graphics library
//==================================================================================
/**
 * @defgroup putimage_functions putimage series functions
 * @brief Core image drawing functionality of EGE graphics library
 * 
 * putimage series functions provide rich image drawing and processing capabilities, including:
 * - Basic image drawing: putimage() series overloaded functions
 * - Transparency effects: putimage_transparent(), putimage_alphablend(), putimage_withalpha()
 * - Blending effects: putimage_alphatransparent(), putimage_alphafilter()
 * - Transform effects: putimage_rotate(), putimage_rotatezoom(), putimage_rotatetransparent()
 * 
 * These functions support multiple drawing modes:
 * - Draw to screen (imgDest = NULL) or another image
 * - Support region clipping, stretch scaling, rotation transformation
 * - Support multiple transparency and blending modes
 * - Support raster operation codes (SRCCOPY, SRCAND, SRCPAINT, etc.)
 * - Support smooth processing (anti-aliasing)
 * @{
 */

/**
 * @brief Basic image drawing function - Draw entire image at specified position
 * @param x X coordinate of drawing position
 * @param y Y coordinate of drawing position
 * @param pimg IMAGE object pointer to draw
 * @param dwRop Ternary raster operation code, default is SRCCOPY (direct copy)
 */
void EGEAPI putimage(int x, int y, PCIMAGE pimg, DWORD dwRop = SRCCOPY);

/**
 * @brief Region image drawing function - Draw specified region of image
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param widthDest Drawing width
 * @param heightDest Drawing height
 * @param imgSrc IMAGE object pointer to draw
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param dwRop Ternary raster operation code, default is SRCCOPY (direct copy)
 */
void EGEAPI putimage(int xDest, int yDest, int widthDest, int heightDest, PCIMAGE imgSrc, int xSrc, int ySrc, DWORD dwRop = SRCCOPY);

/**
 * @brief Stretch image drawing function - Stretch source image region to target region
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param widthDest Drawing width
 * @param heightDest Drawing height
 * @param imgSrc IMAGE object pointer to draw
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param widthSrc Width of drawing content in source IMAGE object
 * @param heightSrc Height of drawing content in source IMAGE object
 * @param dwRop Ternary raster operation code, default is SRCCOPY (direct copy)
 */
void EGEAPI putimage(int xDest, int yDest, int widthDest, int heightDest, PCIMAGE imgSrc, int xSrc, int ySrc, int widthSrc, int heightSrc, DWORD dwRop = SRCCOPY);

/**
 * @brief Image to image drawing function - Draw image on another image
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param imgSrc Source IMAGE object pointer
 * @param dwRop Ternary raster operation code, default is SRCCOPY (direct copy)
 */
void EGEAPI putimage(PIMAGE imgDest, int xDest, int yDest, PCIMAGE imgSrc, DWORD dwRop = SRCCOPY);

/**
 * @brief Image to image region drawing function - Draw specified region of image on another image
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param widthDest Drawing width
 * @param heightDest Drawing height
 * @param imgSrc Source IMAGE object pointer
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param dwRop Ternary raster operation code, default is SRCCOPY (direct copy)
 */
void EGEAPI putimage(PIMAGE imgDest, int xDest, int yDest, int widthDest, int heightDest, PCIMAGE imgSrc, int xSrc, int ySrc, DWORD dwRop = SRCCOPY);

/**
 * @brief Image to image stretch drawing function - Stretch draw image on another image
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param widthDest Drawing width
 * @param heightDest Drawing height
 * @param imgSrc Source IMAGE object pointer
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param widthSrc Width of drawing content in source IMAGE object
 * @param heightSrc Height of drawing content in source IMAGE object
 * @param dwRop Ternary raster operation code, default is SRCCOPY (direct copy)
 */
void EGEAPI putimage(PIMAGE imgDest, int xDest, int yDest, int widthDest, int heightDest, PCIMAGE imgSrc, int xSrc, int ySrc, int widthSrc, int heightSrc, DWORD dwRop = SRCCOPY);

/**
 * @brief Save image to file (char* version)
 * @param pimg Image object pointer to save
 * @param filename Save image file name
 * @param withAlphaChannel Whether to save image's alpha channel, true: save, false: don't save, default is false
 * @return Returns grOk(0) on success, non-zero value on failure
 * @note Currently supports BMP and PNG formats
 * @note File name ending with .bmp saves as BMP format, ending with .png or no extension saves as PNG format
 * @note If file already exists, it will overwrite the original file
 * @warning Runtime exception will occur if pimg is invalid pointer
 * @see saveimage(PCIMAGE, const wchar_t*, bool)
 */
int  EGEAPI saveimage(PCIMAGE pimg, const char*  filename, bool withAlphaChannel = false);

/**
 * @brief Save image to file (wchar_t* version)
 * @param pimg Image object pointer to save
 * @param filename Save image file name (wide character version)
 * @param withAlphaChannel Whether to save image's alpha channel, true: save, false: don't save, default is false
 * @return Returns grOk(0) on success, non-zero value on failure
 * @note Currently supports BMP and PNG formats
 * @note File name ending with .bmp saves as BMP format, ending with .png or no extension saves as PNG format
 * @note If file already exists, it will overwrite the original file
 * @warning Runtime exception will occur if pimg is invalid pointer
 * @see saveimage(PCIMAGE, const char*, bool)
 */
int  EGEAPI saveimage(PCIMAGE pimg, const wchar_t* filename, bool withAlphaChannel = false);

/**
 * @brief Save image as PNG format to file (char* version)
 * @param pimg Image object pointer to save
 * @param filename Save image file name
 * @param withAlphaChannel Whether to save image's alpha channel, true: save, false: don't save, default is false
 * @return Returns 0 on success, non-zero value on failure
 * @note Force save as PNG format, not affected by file name extension
 * @note If file already exists, it will overwrite the original file
 * @warning Runtime exception will occur if pimg is invalid pointer
 * @see savepng(PCIMAGE, const wchar_t*, bool)
 */
int  EGEAPI savepng  (PCIMAGE pimg, const char*  filename, bool withAlphaChannel = false);

/**
 * @brief Save image as PNG format to file (wchar_t* version)
 * @param pimg Image object pointer to save
 * @param filename Save image file name (wide character version)
 * @param withAlphaChannel Whether to save image's alpha channel, true: save, false: don't save, default is false
 * @return Returns 0 on success, non-zero value on failure
 * @note Force save as PNG format, not affected by file name extension
 * @note If file already exists, it will overwrite the original file
 * @warning Runtime exception will occur if pimg is invalid pointer
 * @see savepng(PCIMAGE, const char*, bool)
 */
int  EGEAPI savepng  (PCIMAGE pimg, const wchar_t* filename, bool withAlphaChannel = false);

/**
 * @brief Save image as BMP format to file (char* version)
 * @param pimg Image object pointer to save
 * @param filename Save image file name
 * @param withAlphaChannel Whether to save image's alpha channel, true: save, false: don't save, default is false
 * @return Returns 0 on success, non-zero value on failure
 * @note Force save as BMP format, not affected by file name extension
 * @note If file already exists, it will overwrite the original file
 * @warning Runtime exception will occur if pimg is invalid pointer
 * @see savebmp(PCIMAGE, const wchar_t*, bool)
 */
int  EGEAPI savebmp  (PCIMAGE pimg, const char*  filename, bool withAlphaChannel = false);

/**
 * @brief Save image as BMP format to file (wchar_t* version)
 * @param pimg Image object pointer to save
 * @param filename Save image file name (wide character version)
 * @param withAlphaChannel Whether to save image's alpha channel, true: save, false: don't save, default is false
 * @return Returns 0 on success, non-zero value on failure
 * @note Force save as BMP format, not affected by file name extension
 * @note If file already exists, it will overwrite the original file
 * @warning Runtime exception will occur if pimg is invalid pointer
 * @see savebmp(PCIMAGE, const char*, bool)
 */
int  EGEAPI savebmp  (PCIMAGE pimg, const wchar_t* filename, bool withAlphaChannel = false);

/**
 * @brief Transparent color drawing function - Image drawing with specified color becoming transparent
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param transparentColor Pixel color to become transparent
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object, default is 0
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object, default is 0
 * @param widthSrc Width of drawing content in source IMAGE object, default is 0 (use entire image width)
 * @param heightSrc Height of drawing content in source IMAGE object, default is 0 (use entire image height)
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Alpha blending drawing function - Basic version with specified overall transparency
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param alpha Overall image transparency (0-255), 0 is completely transparent, 255 is completely opaque
 * @param alphaType Alpha type of source image pixels, default is ALPHATYPE_STRAIGHT
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Alpha blending drawing function - Specify source image starting position
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param alpha Overall image transparency (0-255), 0 is completely transparent, 255 is completely opaque
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param alphaType Alpha type of source image pixels, default is ALPHATYPE_STRAIGHT
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Alpha blending drawing function - Specify source image region
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param alpha Overall image transparency (0-255), 0 is completely transparent, 255 is completely opaque
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param widthSrc Width of drawing content in source IMAGE object
 * @param heightSrc Height of drawing content in source IMAGE object
 * @param alphaType Alpha type of source image pixels, default is ALPHATYPE_STRAIGHT
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Alpha blending drawing function - Complete version, supports stretching and smooth processing
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param widthDest Drawing width
 * @param heightDest Drawing height
 * @param alpha Overall image transparency (0-255), 0 is completely transparent, 255 is completely opaque
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param widthSrc Width of drawing content in source IMAGE object
 * @param heightSrc Height of drawing content in source IMAGE object
 * @param smooth Whether to use smooth processing (anti-aliasing), default is false
 * @param alphaType Alpha type of source image pixels, default is ALPHATYPE_STRAIGHT
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Alpha transparent color blending drawing function - Combine transparent color and Alpha blending
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param transparentColor Pixel color to become transparent
 * @param alpha Overall image transparency (0-255), 0 is completely transparent, 255 is completely opaque
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object, default is 0
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object, default is 0
 * @param widthSrc Width of drawing content in source IMAGE object, default is 0 (use entire image width)
 * @param heightSrc Height of drawing content in source IMAGE object, default is 0 (use entire image height)
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Alpha channel drawing function - Use image's own Alpha channel, basic version
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer (must contain Alpha channel data)
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object, default is 0
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object, default is 0
 * @param widthSrc Width of drawing content in source IMAGE object, default is 0 (use entire image width)
 * @param heightSrc Height of drawing content in source IMAGE object, default is 0 (use entire image height)
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Alpha channel drawing function - Use image's own Alpha channel, supports stretching and smooth processing
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer (must contain Alpha channel data)
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param widthDest Drawing width
 * @param heightDest Drawing height
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param widthSrc Width of drawing content in source IMAGE object
 * @param heightSrc Height of drawing content in source IMAGE object
 * @param smooth Whether to use smooth processing (anti-aliasing), default is false
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Alpha filter drawing function - Use another image as Alpha mask
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param imgAlpha IMAGE object pointer used as Alpha mask
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param widthSrc Width of drawing content in source IMAGE object
 * @param heightSrc Height of drawing content in source IMAGE object
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Image blur filter function - Apply blur processing to image
 * @param imgDest Target IMAGE object pointer, image to be blurred
 * @param intensity Blur intensity, higher value means stronger blur effect
 * @param alpha Overall image transparency (0-255), 255 is completely opaque
 * @param xDest X coordinate of top-left corner of processing region, default is 0
 * @param yDest Y coordinate of top-left corner of processing region, default is 0
 * @param widthDest Width of processing region, default is 0 (use entire image width)
 * @param heightDest Height of processing region, default is 0 (use entire image height)
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Rotation drawing function - Rotate image around center point
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgTexture Source texture IMAGE object pointer
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param xCenter X coordinate of rotation center point (relative to source image)
 * @param yCenter Y coordinate of rotation center point (relative to source image)
 * @param radian Rotation angle (in radians, clockwise direction)
 * @param transparent Whether to use image's transparent channel, default is false
 * @param alpha Overall image transparency (0-256), -1 means no alpha, default is -1
 * @param smooth Whether to use smooth processing (anti-aliasing), default is false
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Rotation and zoom drawing function - Perform rotation and scaling simultaneously
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgTexture Source texture IMAGE object pointer
 * @param xDest X coordinate of drawing position
 * @param yDest Y coordinate of drawing position
 * @param xCenter X coordinate of rotation center point (relative to source image)
 * @param yCenter Y coordinate of rotation center point (relative to source image)
 * @param radian Rotation angle (in radians, clockwise direction)
 * @param zoom Scaling ratio, 1.0 is original size
 * @param transparent Whether to use image's transparent channel, default is false
 * @param alpha Overall image transparency (0-256), -1 means no alpha, default is -1
 * @param smooth Whether to use smooth processing (anti-aliasing), default is false
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Rotation transparent drawing function - Specify transparent color when rotating, basic version
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer
 * @param xCenterDest X coordinate of rotation center point in target image
 * @param yCenterDest Y coordinate of rotation center point in target image
 * @param xCenterSrc X coordinate of rotation center point in source image
 * @param yCenterSrc Y coordinate of rotation center point in source image
 * @param transparentColor Pixel color to become transparent
 * @param radian Rotation angle (in radians, clockwise direction)
 * @param zoom Scaling ratio, 1.0 is original size, default is 1.0f
 * @return Returns grOk on success, corresponding error code on failure
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
 * @brief Rotation transparent drawing function - Specify transparent color when rotating, complete version
 * @param imgDest Target IMAGE object pointer, if NULL then draw to screen
 * @param imgSrc Source IMAGE object pointer
 * @param xCenterDest X coordinate of rotation center point in target image
 * @param yCenterDest Y coordinate of rotation center point in target image
 * @param xSrc X coordinate of top-left corner of drawing content in source IMAGE object
 * @param ySrc Y coordinate of top-left corner of drawing content in source IMAGE object
 * @param widthSrc Width of drawing content in source IMAGE object
 * @param heightSrc Height of drawing content in source IMAGE object
 * @param xCenterSrc X coordinate of rotation center point in source image
 * @param yCenterSrc Y coordinate of rotation center point in source image
 * @param transparentColor Pixel color to become transparent
 * @param radian Rotation angle (in radians, clockwise direction)
 * @param zoom Scaling ratio, 1.0 is original size, default is 1.0f
 * @return Returns grOk on success, corresponding error code on failure
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

/** @} */ // End putimage_functions group

/**
 * @brief Get drawing window handle
 * @return Drawing window handle (HWND)
 * @note Returns Windows system window handle, can be used for Windows API calls
 * @see getHInstance(), getHDC()
 */
HWND        EGEAPI getHWnd();

/**
 * @brief Get drawing window instance handle
 * @return Application instance handle (HINSTANCE)
 * @note Returns Windows system application instance handle, can be used for Windows API calls
 * @see getHWnd(), getHDC()
 */
HINSTANCE   EGEAPI getHInstance();

/**
 * @brief Get drawing device context
 * @param pimg Image object pointer, if NULL then get drawing window's device context
 * @return Device context handle (HDC)
 * @note Returns Windows system device context handle, can be used for GDI drawing operations
 * @warning Do not manually release returned HDC, managed automatically by EGE library
 * @see getHWnd(), getHInstance()
 */
HDC         EGEAPI getHDC(PCIMAGE pimg = NULL);

/**
 * @brief Get process function pointer
 * @return Process function pointer
 * @note Internal use function, used to get window procedure function pointer
 */
PVOID       EGEAPI getProcfunc();

/**
 * @brief Get EGE graphics library version number
 * @return EGE graphics library version number
 * @note Version number is in numeric format, can be used for compatibility checks
 */
long        EGEAPI getGraphicsVer();

/**
 * @brief Get current frame rate
 * @return Current frame rate (fps)
 * @note Returns frames per second, used for performance monitoring and debugging
 * @see delay_fps()
 */
float       EGEAPI getfps();

/**
 * @brief Initialize random number generator (using current time as seed)
 * @return Random number seed value used
 * @note Uses current timestamp as seed to initialize Mersenne Twister random number generator
 * @see randomize(unsigned int seed), random(), randomf()
 */
unsigned int    EGEAPI randomize();

/**
 * @brief Initialize random number generator (using specified seed)
 * @param seed Random number seed
 * @return Random number seed value used
 * @note Uses specified seed to initialize Mersenne Twister random number generator, same seed produces same sequence
 * @see randomize(), random(), randomf()
 */
unsigned int    EGEAPI randomize(unsigned int seed);

/**
 * @brief Generate random integer
 * @param n Upper limit of random number range, if 0 then generate full range random number
 * @return Generated random integer, range is [0, n) or [0, UINT_MAX]
 * @note Uses Mersenne Twister algorithm to generate high-quality random numbers
 * @see randomize(), randomf()
 */
unsigned int    EGEAPI random(unsigned int n = 0);

/**
 * @brief Generate random floating point number
 * @return Generated random floating point number, range is [0.0, 1.0)
 * @note Uses Mersenne Twister algorithm to generate high-quality random floating point numbers
 * @see randomize(), random()
 */
double          EGEAPI randomf();

/**
 * @brief Show input dialog to get single line text (ASCII version)
 * @param title Dialog title
 * @param text Prompt text
 * @param buf Buffer to store input text
 * @param len Buffer length
 * @return Returns non-zero value on success, 0 on failure or cancel
 * @note Shows a modal dialog for user to input single line text
 * @warning Ensure buffer is large enough to avoid overflow
 * @see inputbox_getline(const wchar_t*, const wchar_t*, LPWSTR, int)
 */
int EGEAPI inputbox_getline(const char*  title, const char*  text, LPSTR  buf, int len);

/**
 * @brief Show input dialog to get single line text (Unicode version)
 * @param title Dialog title
 * @param text Prompt text
 * @param buf Buffer to store input text
 * @param len Buffer length
 * @return Returns non-zero value on success, 0 on failure or cancel
 * @note Shows a modal dialog for user to input single line text, supports Unicode characters
 * @warning Ensure buffer is large enough to avoid overflow
 * @see inputbox_getline(const char*, const char*, LPSTR, int)
 */
int EGEAPI inputbox_getline(const wchar_t* title, const wchar_t* text, LPWSTR buf, int len);



/// @defgroup InputHandling Input handling
/// Keyboard and mouse input handling related functions
/// @{

/// @defgroup KeyboardInput Keyboard input
/// Keyboard input detection and message processing functions
/// @{

/**
 * @brief Check if there are currently keyboard messages
 * @return Non-zero value indicates there are keyboard messages, 0 indicates none, usually used with getkey
 * @note Used for non-blocking detection of keyboard input, typically used with getkey() in main loop
 * @see getkey()
 */
int     EGEAPI kbmsg();

/**
 * @brief Get keyboard message
 * @return Keyboard message structure containing key code, message type and flag bits
 * @note If there are no keyboard messages currently, it waits; supports key press, release and character messages
 * @see kbmsg(), key_msg, key_msg_e, key_flag_e
 */
key_msg EGEAPI getkey();

/**
 * @brief Get keyboard character input (extended version)
 * @param flag Message flag bits, controls the type of messages to receive
 * @return Combination of key code and message type
 * @deprecated Please use getch() function instead
 * @note Supports getting key press and release events
 * @see getch()
 */
EGE_DEPRECATE(getchEx, "Please use the 'getch' function instead.")
int     EGEAPI getchEx(int flag);

/**
 * @brief Check if there are currently keyboard character inputs (extended version)
 * @param flag Message flag bits, controls the type of messages to detect
 * @return Non-zero value indicates there is input, 0 indicates none
 * @deprecated Please use kbhit() function instead
 * @note Supports detection of key press and release events
 * @see kbhit()
 */
EGE_DEPRECATE(kbhitEx, "Please use the 'kbhit' function instead.")
int     EGEAPI kbhitEx(int flag);

/**
 * @brief Determine if a certain key on keyboard or mouse is in pressed state
 * @param key Key code to detect, see key_code_e enumeration
 * @return Non-zero value indicates key is pressed, 0 indicates not pressed, -1 indicates parameter error
 * @note Real-time key state detection, does not consume message queue; supports keyboard and mouse keys
 * @see key_code_e
 */
bool    EGEAPI keystate(int key);
int     EGEAPI keypress(int key);
int     EGEAPI keyrelease(int key);
int     EGEAPI keyrepeat(int key);


/**
 * @brief Clear key message buffer
 * @note Clears all unprocessed keyboard messages, commonly used to ignore cached key inputs
 * @see flushmouse()
 */
void    EGEAPI flushkey();

/// @} // KeyboardInput

/// @defgroup CharacterInput Character input
/// Character-level keyboard input processing functions
/// @{

/**
 * @brief Get keyboard character input (EGE internal version)
 * @return ASCII code of character, special keys return extended codes
 * @note EGE internal implementation, used to avoid conflicts with conio.h library
 * @see getch()
 */
int     EGEAPI ege_getch();

/**
 * @brief Check if there are currently keyboard character inputs (EGE internal version)
 * @return Non-zero value indicates there is character input, 0 indicates none
 * @note EGE internal implementation, used to avoid conflicts with conio.h library
 * @see kbhit()
 */
int     EGEAPI ege_kbhit();

#if !defined(_INC_CONIO) && !defined(_CONIO_H_)
#define _INC_CONIO
#define _CONIO_H_

/**
 * @brief Get keyboard character input
 * @return ASCII code of character, special keys return extended codes
 * @note If there is no character input currently, it waits; equivalent to ege_getch()
 * @see ege_getch(), kbhit()
 */
int EGEAPI getch();  // Same as ege_getch()

/**
 * @brief Check if there are currently keyboard character inputs
 * @return Non-zero value indicates there is character input, 0 indicates none
 * @note Used for non-blocking detection of character input, usually used with getch; equivalent to ege_kbhit()
 * @see ege_kbhit(), getch()
 */
int EGEAPI kbhit();  // Same as ege_kbhit()
#else
#define getch ege_getch
#define kbhit ege_kbhit
#endif

/// @} // CharacterInput

/// @defgroup MouseInput Mouse input
/// Mouse input detection and message processing functions
/// @{

/**
 * @brief Check if there are currently mouse messages
 * @return Non-zero value indicates there are mouse messages, 0 indicates none
 * @note Used for non-blocking detection of mouse input, usually used with getmouse
 * @see getmouse()
 */
int         EGEAPI mousemsg();

/**
 * @brief Get a mouse message
 * @return Mouse message structure containing coordinates, button state and message type
 * @note If current mouse message queue is empty, it waits until new message is generated
 * @see mousemsg(), mouse_msg, mouse_msg_e, mouse_flag_e
 */
mouse_msg   EGEAPI getmouse();

/**
 * @brief Get a mouse message (deprecated function)
 * @return MOUSEMSG structure containing mouse state information
 * @deprecated Please use getmouse() function instead
 * @note Compatible with old version API, recommend using more modern getmouse() function
 * @see getmouse()
 */
EGE_DEPRECATE(GetMouseMsg, "Please use the 'getmouse' function instead.")
MOUSEMSG    EGEAPI GetMouseMsg();

/**
 * @brief Clear mouse message buffer
 * @note Clears all unprocessed mouse messages, commonly used to ignore cached mouse inputs
 * @see flushkey()
 */
void        EGEAPI flushmouse();

/**
 * @brief Set whether mouse pointer is displayed
 * @param bShow Non-zero value shows mouse pointer, 0 hides mouse pointer
 * @return Previous display state
 * @note Used to control mouse cursor visibility in graphics window
 */
int         EGEAPI showmouse(int bShow);

/**
 * @brief Get current mouse position coordinates
 * @param x Pointer to receive mouse x coordinate
 * @param y Pointer to receive mouse y coordinate
 * @return Function execution status
 * @note Gets real-time mouse coordinate position in graphics window
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
 * @brief Music playback class
 * 
 * MUSIC class provides music playback functionality based on Windows Media Control Interface (MCI),
 * supports playing various audio formats such as WAV, MP3, MIDI, etc.
 * 
 * @note This class is based on Windows MCI implementation, only supports Windows platform
 * @note Supported audio formats include: WAV, MP3, MIDI, etc.
 * @see music_state_flag, MUSIC_ERROR
 */
class MUSIC
{
public:
    /**
     * @brief Constructor
     * @note Initializes music player, sets initial state to not opened
     */
    MUSIC();
    
    /**
     * @brief Destructor
     * @note Automatically closes opened music file and cleans up resources
     */
    virtual ~MUSIC();

    /**
     * @brief Type conversion operator
     * @return Returns window handle (HWND)
     * @note Used for interaction with Windows API
     */
    operator HWND() const { return (HWND)m_dwCallBack; }

public:
    /**
     * @brief Check if music file is opened
     * @return 1 indicates opened, 0 indicates not opened
     * @note Used to determine if music file is successfully opened
     */
    int IsOpen() { return (m_DID != MUSIC_ERROR) ? 1 : 0; }

    /**
     * @brief Open music file (ASCII version)
     * @param filepath Music file path (including filename)
     * @return Returns 0 on success, non-zero on failure
     * @note Supports WAV, MP3, MIDI and other formats, playback state becomes MUSIC_MODE_STOP after successful opening
     * @note If another file is already opened, it will be automatically closed
     * @see OpenFile(const wchar_t*), Close()
     */
    DWORD OpenFile(const char* filepath);
    
    /**
     * @brief Open music file (Unicode version)
     * @param filepath Music file path (including filename)
     * @return Returns 0 on success, non-zero on failure
     * @note Supports WAV, MP3, MIDI and other formats, playback state becomes MUSIC_MODE_STOP after successful opening
     * @note If another file is already opened, it will be automatically closed
     * @see OpenFile(const char*), Close()
     */
    DWORD OpenFile(const wchar_t* filepath);
    
    /**
     * @brief Play music
     * @param dwFrom Start position for playback (milliseconds), default is MUSIC_ERROR (continue playing)
     * @param dwTo End position for playback (milliseconds), default is MUSIC_ERROR (play to end)
     * @return Returns 0 on success, non-zero on failure
     * @note Play() continues playback, Play(0) starts from beginning
     * @note Playback state becomes MUSIC_MODE_PLAY
     * @see Pause(), Stop(), RepeatPlay()
     */
    DWORD Play(DWORD dwFrom = MUSIC_ERROR, DWORD dwTo = MUSIC_ERROR);
    
    /**
     * @brief Loop play music
     * @param dwFrom Start position for playback (milliseconds), default is MUSIC_ERROR
     * @param dwTo End position for playback (milliseconds), default is MUSIC_ERROR
     * @return Returns 0 on success, non-zero on failure
     * @note Loop play music within specified time range
     * @see Play(), Pause(), Stop()
     */
    DWORD RepeatPlay(DWORD dwFrom = MUSIC_ERROR, DWORD dwTo = MUSIC_ERROR);
    
    /**
     * @brief Pause playback
     * @return Returns 0 on success, non-zero on failure
     * @note Playback state becomes MUSIC_MODE_PAUSE, playback progress remains unchanged
     * @see Play(), Stop()
     */
    DWORD Pause();
    
    /**
     * @brief Seek to playback position
     * @param dwTo Target playback position (milliseconds)
     * @return Returns 0 on success, non-zero on failure
     * @note Currently this function is invalid, recommend using Play(dwTo) instead
     * @deprecated Recommend using Play(dwTo) to achieve seeking
     * @see Play()
     */
    DWORD Seek(DWORD dwTo);
    
    /**
     * @brief Set playback volume
     * @param value Volume level, range 0.0~1.0
     * @return Returns 0 on success, non-zero on failure
     * @note 0.0 is mute, 1.0 is maximum volume
     */
    DWORD SetVolume(float value);
    
    /**
     * @brief Close music file
     * @return Returns 0 on success, non-zero on failure
     * @note Closes currently opened music file and releases related resources
     * @see OpenFile()
     */
    DWORD Close();
    
    /**
     * @brief Stop playback
     * @return Returns 0 on success, non-zero on failure
     * @note Playback state becomes MUSIC_MODE_STOP, playback progress remains unchanged
     * @see Play(), Pause()
     */
    DWORD Stop();
    
    /**
     * @brief Get current playback position
     * @return Current playback position (milliseconds)
     * @note Returns current playback progress in milliseconds
     * @see GetLength(), GetPlayStatus()
     */
    DWORD GetPosition();
    
    /**
     * @brief Get total music duration
     * @return Total music duration (milliseconds)
     * @note Returns total length of music file in milliseconds
     * @see GetPosition(), GetPlayStatus()
     */
    DWORD GetLength();

    /**
     * @brief Get playback status
     * @return Current playback status, see music_state_flag enumeration
     * @note Return value can be:
     *       - MUSIC_MODE_NOT_OPEN: Not opened
     *       - MUSIC_MODE_NOT_READY: Device not ready
     *       - MUSIC_MODE_PAUSE: Paused
     *       - MUSIC_MODE_PLAY: Playing
     *       - MUSIC_MODE_STOP: Stopped
     *       - MUSIC_MODE_OPEN: Opening
     *       - MUSIC_MODE_SEEK: Seeking
     * @see music_state_flag, GetPosition(), GetLength()
     */
    DWORD GetPlayStatus();

private:
    DWORD m_DID;        ///< MCI device ID
    PVOID m_dwCallBack; ///< Callback handle
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
