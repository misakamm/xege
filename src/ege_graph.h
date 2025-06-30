#pragma once

#include "ege_common.h"


#ifndef _MSC_VER
#define GRADIENT_FILL_RECT_H   0x00000000
#define GRADIENT_FILL_RECT_V   0x00000001
#define GRADIENT_FILL_TRIANGLE 0x00000002
#define GRADIENT_FILL_OP_FLAG  0x000000ff
#endif

namespace ege
{

extern struct _graph_setting graph_setting;
class egeControlBase;   // 前置声明

initmode_flag getinitmode();

void logoscene();

int dealmessage(_graph_setting* pg, bool force_update);

bool needToUpdate(_graph_setting* pg);

int graphupdate(_graph_setting* pg);

void guiupdate(_graph_setting* pg, egeControlBase* root);

int waitdealmessage(_graph_setting* pg);

float EGE_PRIVATE_GetFPS(int add); // 获取帧数

void setmode(int gdriver, int gmode);

// GDI+ 初始化
void gdiplusinit();

Gdiplus::Graphics* recreateGdiplusGraphics(HDC hdc, const Gdiplus::Graphics* oldGraphics);

Gdiplus::LineCap convertToGdiplusLineCap(line_cap_type linecap);

Gdiplus::LineJoin convertToGdiplusLineJoin(line_join_type linejoin);

int frameBufferCopy(HDC frontDC, const Point& frontPoint, HDC backDC, const Rect& rect);

int  swapbuffers();

bool isinitialized();

void replacePixels(PIMAGE pimg, color_t src, color_t dst, bool ignoreAlpha = false);


}
