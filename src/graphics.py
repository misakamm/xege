#griphics driver
DETECT = 0
VGA = 9
TRUECOLOR = 11
TRUECOLORSIZE = 12
CURRENT_DRIVER = -1

# err code
grOk                =   0
grNoInitGraph       =  -1
grNotDetected       =  -2
grFileNotFound      =  -3
grInvalidDriver     =  -4
grNoLoadMem         =  -5
grNoScanMem         =  -6
grNoFloodMem        =  -7
grFontNotFound      =  -8
grNoFontMem         =  -9
grInvalidMode       = -10
grError             = -11
grIOerror           = -12
grInvalidFont       = -13
grInvalidFontNum    = -14
grInvalidVersion    = -18
grException         = 0x10
grParamError        = 0x11
grInvalidRegion     = 0x12
grOutOfMemory       = 0x13
grNullPointer       = 0x14
grAllocError        = 0x15
grInvalidMemory     = 0xCDCDCDCD

#color
BLACK = 0
BLUE = 1
GREEN = 2
CYAN = 3
RED = 4
MAGENTA = 5
BROWN = 6
LIGHTGRAY = 7
DARKGRAY = 8
LIGHTBLUE = 9
LIGHTGREEN = 10
LIGHTCYAN = 11
LIGHTRED = 12
LIGHTMAGENTA = 13
YELLOW = 14
WHITE = 15

#line_style
SOLID_LINE   = 0
DOTTED_LINE  = 1
CENTER_LINE  = 2
DASHED_LINE  = 3

#line_widths
NORM_WIDTH  = 1
THICK_WIDTH = 3

#text_just
LEFT_TEXT   = 0
CENTER_TEXT = 1
RIGHT_TEXT  = 2

BOTTOM_TEXT = 0
TOP_TEXT    = 2

#font_names
DEFAULT_FONT    = 0
TRIPLEX_FONT    = 1
SMALL_FONT      = 2
SANS_SERIF_FONT = 3
GOTHIC_FONT     = 4
USER_FONT       = 5

#fill_patterns
EMPTY_FILL      = 0
SOLID_FILL      = 1
LINE_FILL       = 2
LTSLASH_FILL    = 3
SLASH_FILL      = 4
BKSLASH_FILL    = 5
LTBKSLASH_FILL  = 6
HATCH_FILL      = 7
XHATCH_FILL     = 8
INTERLEAVE_FILL = 9
WIDE_DOT_FILL   = 10
CLOSE_DOT_FILL  = 11

#putimage_ops
COPY_PUT    = 0
XOR_PUT     = 1
OR_PUT      = 2
AND_PUT     = 3
NOT_PUT     = 4

#page_set
PAGE_USING  = 0x40000000

#graphics_settings
GRAPHICS_FORCEEXIT          = 0x01
GRAPHICS_TOPMOST            = 0x02
GRAPHICS_PRIORITY           = 0x03
GRAPHICS_OBJECT             = 0x04
GRAPHICS_SHOWMOUSE          = 0x05
GRAPHICS_PRESETSTYLE        = 0x06
GRAPHICS_PRESETEXSTYLE      = 0x07
GRAPHICS_SETTITLE           = 0x08
GRAPHICS_SETSTYLE           = 0x09
GRAPHICS_SETEXSTYLE         = 0x0A
GRAPHICS_ALWAYS_RENDER      = 0x10
GRAPHICS_DELAY_LEASTMODE    = 0x11
GRAPHICS_NULL               = 0x00

#message_event
MSG_EVENT_UP            = 0x00
MSG_EVENT_DOWN          = 0x01
MSG_EVENT_CLICK         = 0x01
MSG_EVENT_DBCLICK       = 0x02
MSG_EVENT_MOVE          = 0x04
MSG_EVENT_WHEEL         = 0x10

#message_mouse
MSG_MOUSE_LEFT      = 0x01
MSG_MOUSE_RIGHT     = 0x02
MSG_MOUSE_MID       = 0x04

