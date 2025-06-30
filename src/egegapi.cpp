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
#include "ege_common.h"
#include "ege_extension.h"
#include "gdi_conv.h"

#include <cmath>
#include <cstdarg>
#include <cstdio>


namespace ege
{

bool is_run()
{
    struct _graph_setting* pg = &graph_setting;
    if (pg->exit_window || pg->exit_flag) {
        return false;
    }
    return true;
}

bool isinitialized()
{
    return graph_setting.has_init;
}

int showmouse(int bShow)
{
    struct _graph_setting* pg = &graph_setting;
    int ret = pg->mouse_show;
    pg->mouse_show = bShow;
    return ret;
}

int mousepos(int* x, int* y)
{
    struct _graph_setting* pg = &graph_setting;
    *x = pg->mouse_pos.x;
    *y = pg->mouse_pos.y;
    return 0;
}

void setwritemode(int mode, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    SetROP2(img->m_hDC, mode);
    CONVERT_IMAGE_END;
}

static inline bool in_rect(int x, int y, int w, int h)
{
    return !((x < 0) || (y < 0) || (x >= w) || (y >= h));
}

color_t getpixel(int x, int y, PCIMAGE pimg)
{
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

void putpixel(int x, int y, color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    x += img->m_vpt.left;
    y += img->m_vpt.top;
    if (in_rect(x, y, img->m_vpt.right, img->m_vpt.bottom)) {
        img->m_pBuffer[y * img->m_width + x] = color;
    }
    CONVERT_IMAGE_END;
}

void putpixels(int numOfPoints, const int* points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    int x, y, c;
    PDWORD pb = &img->m_pBuffer[img->m_vpt.top * img->m_width + img->m_vpt.left];
    int w = img->m_vpt.right - img->m_vpt.left, h = img->m_vpt.bottom - img->m_vpt.top;
    int tw = img->m_width;
    for (int n = 0; n < numOfPoints; ++n, points += 3) {
        x = points[0], y = points[1], c = points[2];
        if (in_rect(x, y, w, h)) {
            pb[y * tw + x] = c;
        }
    }
    CONVERT_IMAGE_END;
}

void putpixels_f(int numOfPoints, const int* points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    int x, y, c;
    int tw = img->m_width;
    int th = img->m_height;
    for (int n = 0; n < numOfPoints; ++n, points += 3) {
        x = points[0], y = points[1], c = points[2];
        if (in_rect(x, y, tw, th)) {
            img->m_pBuffer[y * tw + x] = c;
        }
    }
    CONVERT_IMAGE_END;
}

color_t getpixel_f(int x, int y, PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_F_CONST(pimg);
    if (in_rect(x, y, img->m_width, img->m_height)) {
        return img->m_pBuffer[y * img->m_width + x];
    }
    return 0;
}

void putpixel_f(int x, int y, color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE_F(pimg);
    if (in_rect(x, y, img->m_width, img->m_height)) {
        img->m_pBuffer[y * img->m_width + x] = color;
    }
}

void putpixel_withalpha(int x, int y, color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    x += img->m_vpt.left;
    y += img->m_vpt.top;
    if (in_rect(x, y, img->m_vpt.right, img->m_vpt.bottom)) {
        color_t& dst_color = (color_t&)img->m_pBuffer[y * img->m_width + x];
        dst_color = colorblend_inline(dst_color, color, EGEGET_A(color));
    }
    CONVERT_IMAGE_END;
}

void putpixel_withalpha_f(int x, int y, color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE_F(pimg);
    if (in_rect(x, y, img->m_width, img->m_height)) {
        color_t& dst_color = (color_t&)img->m_pBuffer[y * img->m_width + x];
        dst_color = colorblend_inline_fast(dst_color, color, EGEGET_A(color));
    }
    CONVERT_IMAGE_END;
}

void putpixel_savealpha(int x, int y, color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    x += img->m_vpt.left;
    y += img->m_vpt.top;
    if (in_rect(x, y, img->m_vpt.right, img->m_vpt.bottom)) {
        color_t& dst_color = (color_t&)img->m_pBuffer[y * img->m_width + x];
        dst_color = EGECOLORA(color, EGEGET_A(dst_color));
    }
    CONVERT_IMAGE_END;
}

void putpixel_savealpha_f(int x, int y, color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE_F(pimg);
    if (in_rect(x, y, img->m_width, img->m_height)) {
        color_t& dst_color = (color_t&)img->m_pBuffer[y * img->m_width + x];
        dst_color = EGECOLORA(color, EGEGET_A(dst_color));
    }
    CONVERT_IMAGE_END;
}

void putpixel_alphablend(int x, int y, color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    x += img->m_vpt.left;
    y += img->m_vpt.top;
    if (in_rect(x, y, img->m_vpt.right, img->m_vpt.bottom)) {
        color_t& dst_color = (color_t&)img->m_pBuffer[y * img->m_width + x];
        dst_color = alphablend_inline(dst_color, color);
    }
    CONVERT_IMAGE_END;
}

void putpixel_alphablend_f(int x, int y, color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (in_rect(x, y, img->m_width, img->m_height)) {
        color_t& dst_color = (color_t&)img->m_pBuffer[y * img->m_width + x];
        dst_color = alphablend_inline(dst_color, color);
    }
    CONVERT_IMAGE_END;
}

void putpixel_alphablend(int x, int y, color_t color, unsigned char alphaFactor, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    x += img->m_vpt.left;
    y += img->m_vpt.top;
    if (in_rect(x, y, img->m_vpt.right, img->m_vpt.bottom)) {
        color_t& dst_color = (color_t&)img->m_pBuffer[y * img->m_width + x];
        dst_color = alphablend_inline(dst_color, color, alphaFactor);
    }
    CONVERT_IMAGE_END;
}

void putpixel_alphablend_f(int x, int y, color_t color, unsigned char alphaFactor, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (in_rect(x, y, img->m_width, img->m_height)) {
        color_t& dst_color = (color_t&)img->m_pBuffer[y * img->m_width + x];
        dst_color = alphablend_inline(dst_color, color, alphaFactor);
    }
    CONVERT_IMAGE_END;
}

void moveto(int x, int y, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    MoveToEx(img->m_hDC, x, y, NULL);
    CONVERT_IMAGE_END;
}

void moverel(int dx, int dy, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    POINT pt;
    GetCurrentPositionEx(img->m_hDC, &pt);
    dx += pt.x;
    dy += pt.y;
    MoveToEx(img->m_hDC, dx, dy, NULL);
    CONVERT_IMAGE_END;
}

void line(int x1, int y1, int x2, int y2, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle != NULL_LINE) {
            MoveToEx(img->m_hDC, x1, y1, NULL);
            LineTo(img->m_hDC, x2, y2);
            MoveToEx(img->m_hDC, x1, y1, NULL);
        }
    }
    CONVERT_IMAGE_END;
}

void linerel(int dx, int dy, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        POINT pt;
        GetCurrentPositionEx(img->m_hDC, &pt);
        dx += pt.x;
        dy += pt.y;
        if (img->m_linestyle.linestyle != NULL_LINE) {
            LineTo(img->m_hDC, dx, dy);
        } else {
            MoveToEx(img->m_hDC, dx, dy, NULL);
        }
    }
    CONVERT_IMAGE_END;
}

void lineto(int x, int y, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle != NULL_LINE) {
            LineTo(img->m_hDC, x, y);
        } else {
            MoveToEx(img->m_hDC, x, y, NULL);
        }
    }
    CONVERT_IMAGE_END;
}

/* private function */
static void line_base(float x1, float y1, float x2, float y2, PIMAGE img)
{
    int bswap = 2;
    color_t col = getcolor(img);
    color_t endp = 0;
    color_t* pBuffer = (color_t*)img->m_pBuffer;
    int rw = img->m_width;
    if (x1 > x2) {
        float ft;
        SWAP(x1, x2, ft);
        SWAP(y1, y2, ft);
        if (bswap) {
            bswap ^= 3;
        }
    }
    if (x2 < img->m_vpt.left) {
        return;
    }
    if (x1 > img->m_vpt.right) {
        return;
    }
    if (x1 < img->m_vpt.left) {
        if (x2 - x1 < FLOAT_EPS) {
            return;
        }
        float d = (x2 - img->m_vpt.left) / (x2 - x1);
        y1 = (y1 - y2) * d + y2;
        x1 = (float)img->m_vpt.left;
        if (bswap == 1) {
            bswap = 0;
        }
    }
    if (x2 > img->m_vpt.right) {
        if (x2 - x1 < FLOAT_EPS) {
            return;
        }
        float d = (img->m_vpt.right - x1) / (x2 - x1);
        y2 = (y2 - y1) * d + y1;
        x2 = (float)img->m_vpt.right;
        if (bswap == 2) {
            bswap = 0;
        }
    }
    if (y1 > y2) {
        float ft;
        SWAP(x1, x2, ft);
        SWAP(y1, y2, ft);
        if (bswap) {
            bswap ^= 3;
        }
    }
    if (y2 < img->m_vpt.top) {
        return;
    }
    if (y1 > img->m_vpt.bottom) {
        return;
    }
    if (y1 < img->m_vpt.top) {
        if (y2 - y1 < FLOAT_EPS) {
            return;
        }
        float d = (y2 - img->m_vpt.top) / (y2 - y1);
        x1 = (x1 - x2) * d + x2;
        y1 = (float)img->m_vpt.top;
        if (bswap == 1) {
            bswap = 0;
        }
    }
    if (y2 > img->m_vpt.bottom) {
        if (y2 - y1 < FLOAT_EPS) {
            return;
        }
        float d = (img->m_vpt.bottom - y1) / (y2 - y1);
        x2 = (x2 - x1) * d + x1;
        y2 = (float)img->m_vpt.bottom;
        if (bswap == 2) {
            bswap = 0;
        }
    }
    if (bswap) {
        if (bswap == 1) {
            endp = pBuffer[(int)y1 * rw + (int)x1];
        } else {
            endp = pBuffer[(int)y2 * rw + (int)x2];
        }
    }
    if (y2 - y1 > fabs(x2 - x1)) {
        int y = (int)(y1 + 0.9f);
        int ye = (int)(y2);
        float x, dx;
        if (y < y1) {
            ++y;
        }
        dx = (x2 - x1) / (y2 - y1);
        x = (y - y1) * dx + x1 + 0.5f;
        if (ye >= img->m_vpt.bottom) {
            ye = img->m_vpt.bottom - 1;
        }
        if (ye < y2) {
            bswap = 0;
        }
        for (; y <= ye; ++y, x += dx) {
            pBuffer[y * rw + (int)x] = col;
        }
    } else {
        if (x1 > x2) {
            float ft;
            SWAP(x1, x2, ft);
            SWAP(y1, y2, ft);
            if (bswap) {
                bswap ^= 3;
            }
        }
        int x = (int)(x1 + 0.9f);
        int xe = (int)(x2);
        float y, dy;
        if (x < x1) {
            ++x;
        }
        dy = (y2 - y1) / (x2 - x1);
        y = (x - x1) * dy + y1 + 0.5f;
        if (xe >= img->m_vpt.right) {
            xe = img->m_vpt.right - 1;
        }
        if (xe < x2) {
            bswap = 0;
        }
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

void lineto_f(float x, float y, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        POINT pt;
        GetCurrentPositionEx(img->m_hDC, &pt);
        line_base((float)pt.x, (float)pt.y, x, y, img);
        MoveToEx(img->m_hDC, (int)round(x), (int)round(y), NULL);
    }
    CONVERT_IMAGE_END;
}

void linerel_f(float dx, float dy, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        POINT pt;
        GetCurrentPositionEx(img->m_hDC, &pt);
        float endX = (float)pt.x + dx, endY = (float)pt.y + dy;
        line_base((float)pt.x, (float)pt.y, endX, endY, img);
        MoveToEx(img->m_hDC, (int)round(endX), (int)round(endY), NULL);
    }
    CONVERT_IMAGE_END;
}

void line_f(float x1, float y1, float x2, float y2, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        line_base(x1, y1, x2, y2, img);
    }
    CONVERT_IMAGE_END;
}

/*private function*/
static int saveBrush(PIMAGE img, int save) // 此函数调用前，已经有Lock
{
    struct _graph_setting* pg = &graph_setting;
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

void rectangle(int left, int top, int right, int bottom, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (saveBrush(img, 1)) {
        Rectangle(img->m_hDC, left, top, right, bottom);
        saveBrush(img, 0);
    }
    CONVERT_IMAGE_END;
}

color_t getcolor(PCIMAGE pimg)
{
    return getlinecolor(pimg);
}

color_t getlinecolor(PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);

    if (img && img->m_hDC) {
        CONVERT_IMAGE_END;
        return img->m_linecolor;
    }
    CONVERT_IMAGE_END;
    return IMAGE::initial_line_color;
}

// 将描述线形的位模式转换为 style 数组
// 用于 ExtCreatePen 中。
// 返回值为用到的数组元素数。
static int upattern2array(unsigned short pattern, DWORD style[])
{
    int n, segments = 0, segmentLength = 1;
    int state = !!(pattern & 1);
    for (n = 1; n < 16; n++) {
        int currentBit = !!(pattern & (1 << n));
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

    // 若 pattern 以 0 开头且为偶数段
    if (!(pattern & 1) && segments % 2 == 0) {
        DWORD p0 = style[0];
        for (int i = 0; i < segments - 1; ++i) {
            style[i] = style[i + 1];
        }
        style[segments - 1] = p0;
    }

    return segments;
}

static void update_pen(PIMAGE img)
{
    const int linestyle = img->m_linestyle.linestyle;
    const unsigned short pattern = img->m_linestyle.upattern;
    const int thickness = img->m_linestyle.thickness;

    HPEN hpen;

    if ((thickness == 1) && ((linestyle == SOLID_LINE) || (linestyle == NULL_LINE))) {
        LOGPEN logPen;
        logPen.lopnStyle = linestyle; // Other styles may be drawn incorrectly
        logPen.lopnWidth.x = 1;       // Width
        logPen.lopnWidth.y = 1;       // Unuse
        logPen.lopnColor = ARGBTOZBGR(img->m_linecolor);

        hpen = CreatePenIndirect(&logPen);
    } else {
        unsigned int penStyle = linestyle;

        penStyle |= PS_GEOMETRIC;

        switch (img->m_linestartcap) {
            case LINECAP_FLAT :  penStyle |= PS_ENDCAP_FLAT;   break;
            case LINECAP_ROUND:  penStyle |= PS_ENDCAP_ROUND;  break;
            case LINECAP_SQUARE: penStyle |= PS_ENDCAP_SQUARE; break;
            default:             penStyle |= PS_ENDCAP_FLAT;   break;
        }

        switch(img->m_linejoin) {
            case LINEJOIN_MITER: penStyle |= PS_JOIN_MITER;    break;
            case LINEJOIN_BEVEL: penStyle |= PS_JOIN_BEVEL;    break;
            case LINEJOIN_ROUND: penStyle |= PS_JOIN_ROUND;    break;
            default:             penStyle |= PS_JOIN_MITER;    break;
        }

        LOGBRUSH lbr;
        lbr.lbColor = ARGBTOZBGR(img->m_linecolor);
        lbr.lbStyle = BS_SOLID;
        lbr.lbHatch = 0;

        if (linestyle == USERBIT_LINE) {
            DWORD style[20] = {0};
            int bn = upattern2array(pattern, style);
            hpen = ExtCreatePen(penStyle, thickness, &lbr, bn, style);
        } else {
            hpen = ExtCreatePen(penStyle, thickness, &lbr, 0, NULL);
        }
    }

    if (hpen) {
        DeleteObject(SelectObject(img->m_hDC, hpen));
    }

    SetMiterLimit(img->m_hDC, img->m_linejoinmiterlimit, NULL);

    // why update pen not in IMAGE???
#ifdef EGE_GDIPLUS
    Gdiplus::Pen* pen = img->getPen();
    pen->SetColor(img->m_linecolor);
    pen->SetWidth(img->m_linewidth);
    pen->SetDashStyle(linestyle_to_dashstyle(img->m_linestyle.linestyle));

    pen->SetStartCap(convertToGdiplusLineCap(img->m_linestartcap));
    pen->SetEndCap(convertToGdiplusLineCap(img->m_lineendcap));
    pen->SetLineJoin(convertToGdiplusLineJoin(img->m_linejoin));
    pen->SetMiterLimit(img->m_linejoinmiterlimit);
#endif
}

void setcolor(color_t color, PIMAGE pimg)
{
    setlinecolor(color, pimg);
    settextcolor(color, pimg);
}

void setlinecolor(color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img && img->m_hDC) {
        img->m_linecolor = color;
        update_pen(img);
    }
    CONVERT_IMAGE_END
}

void setfillcolor(color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    img->m_fillcolor = color;
    HBRUSH hbr = CreateSolidBrush(ARGBTOZBGR(color));
    if (hbr) {
        DeleteObject(SelectObject(img->m_hDC, hbr));
    }
#ifdef EGE_GDIPLUS
    img->set_pattern(NULL);
#endif
    CONVERT_IMAGE_END;
}

color_t getfillcolor(PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);

    if (img && img->m_hDC) {
        CONVERT_IMAGE_END;
        return img->m_fillcolor;
    }
    CONVERT_IMAGE_END;
    return IMAGE::initial_fill_color;
}

color_t getbkcolor(PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);

    if (img) {
        if (img->m_hDC) {
            return img->m_bk_color;
        }
    } else {
        _graph_setting* pg = &graph_setting;
        if (!pg->has_init) {
            return pg->window_initial_color;
        }
    }

    CONVERT_IMAGE_END;

    return IMAGE::initial_bk_color;
}

color_t gettextcolor(PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);

    if (img && img->m_hDC) {
        return img->m_textcolor;
    }
    CONVERT_IMAGE_END;
    return IMAGE::initial_text_color;
}

void setbkcolor(color_t color, PIMAGE pimg)
{
    color_t oldBkColor = getbkcolor(pimg);
    setbkcolor_f(color, pimg);
    replacePixels(pimg, oldBkColor, color);
}

void setbkcolor_f(color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img) {
        if (img->m_hDC) {
            img->m_bk_color = color;
            SetBkColor(img->m_hDC, ARGBTOZBGR(color));
        }
    } else {
        _graph_setting* pg = &graph_setting;
        if (!pg->has_init) {
            pg->window_initial_color = color;
        }
    }

    CONVERT_IMAGE_END;
}

void settextcolor(color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img && img->m_hDC) {
        img->m_textcolor = color;
        SetTextColor(img->m_hDC, ARGBTOZBGR(color));
    }
    CONVERT_IMAGE_END;
}

void setfontbkcolor(color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img && img->m_hDC) {
        SetBkColor(img->m_hDC, ARGBTOZBGR(color));
    }
    CONVERT_IMAGE_END;
}

void setbkmode(int bkMode, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img && img->m_hDC) {
        SetBkMode(img->m_hDC, bkMode);
    }
    CONVERT_IMAGE_END;
}

PIMAGE gettarget()
{
    struct _graph_setting* pg = &graph_setting;
    return pg->imgtarget_set;
}

int settarget(PIMAGE pbuf)
{
    struct _graph_setting* pg = &graph_setting;
    pg->imgtarget_set = pbuf;
    if (pbuf == NULL) {
        pg->imgtarget = pg->img_page[graph_setting.active_page];
    } else {
        pg->imgtarget = pbuf;
    }
    return 0;
}

void cleardevice(PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img && img->m_hDC) {
        color_t c = getbkcolor(img);
        for (color_t *p = (color_t*)img->getbuffer(), *e = (color_t*)&img->getbuffer()[img->m_width * img->m_height];
             p != e;
             ++p)
        {
            *p = c;
        }
    }
    CONVERT_IMAGE_END;
}

void arc(int x, int y, int startAngle, int endAngle, int radius, PIMAGE pimg)
{
    ellipse(x, y, startAngle, endAngle, radius, radius, pimg);
}

void arcf(float x, float y, float startAngle, float endAngle, float radius, PIMAGE pimg)
{
    ellipsef(x, y, startAngle, endAngle, radius, radius, pimg);
}

void circle(int x, int y, int radius, PIMAGE pimg) { ellipse(x, y, 0, 360, radius, radius, pimg); }

void circlef(float x, float y, float radius, PIMAGE pimg) { ellipsef(x, y, 0.0f, 360.0f, radius, radius, pimg); }

void ellipse(int x, int y, int startAngle, int endAngle, int xRadius, int yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    double sr = startAngle / 180.0 * PI, er = endAngle / 180.0 * PI;

    if (img) {
        Arc(img->m_hDC,
            x - xRadius,
            y - yRadius,
            x + xRadius,
            y + yRadius,
            (int)(x + xRadius * cos(sr)),
            (int)(y - yRadius * sin(sr)),
            (int)(x + xRadius * cos(er)),
            (int)(y - yRadius * sin(er)));
    }
    CONVERT_IMAGE_END;
}

void ellipsef(float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    double sr = startAngle / 180.0 * PI, er = endAngle / 180.0 * PI;

    if (img) {
        Arc(img->m_hDC,
            (int)(x - xRadius),
            (int)(y - yRadius),
            (int)(x + xRadius),
            (int)(y + yRadius),
            (int)(x + xRadius * cos(sr)),
            (int)(y - yRadius * sin(sr)),
            (int)(x + xRadius * cos(er)),
            (int)(y - yRadius * sin(er)));
    }
    CONVERT_IMAGE_END;
}

void pieslice(int x, int y, int startAngle, int endAngle, int radius, PIMAGE pimg)
{
    sector(x, y, startAngle, endAngle, radius, radius, pimg);
}

void pieslicef(float x, float y, float startAngle, float endAngle, float radius, PIMAGE pimg)
{
    sectorf(x, y, startAngle, endAngle, radius, radius, pimg);
}

void sector(int x, int y, int startAngle, int endAngle, int xRadius, int yRadius, PIMAGE pimg)
{
    fillpie(x, y, startAngle, endAngle, xRadius, yRadius, pimg);
}

void sectorf(float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg)
{
    fillpief(x, y, startAngle, endAngle, xRadius, yRadius, pimg);
}

void pie(int x, int y, int startAngle, int endAngle, int xRadius, int yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldBrush = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_BRUSH));
    fillpie(x, y, startAngle, endAngle, xRadius, yRadius, pimg);
    SelectObject(img->m_hDC, oldBrush);
    CONVERT_IMAGE_END
}

void pief(float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldBrush = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_BRUSH));
    fillpief(x, y, startAngle, endAngle, xRadius, yRadius, pimg);
    SelectObject(img->m_hDC, oldBrush);
    CONVERT_IMAGE_END
}

void fillpie(int x, int y, int startAngle, int endAngle, int xRadius, int yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    double sr = startAngle / 180.0 * PI, er = endAngle / 180.0 * PI;
    if (img) {
        Pie(img->m_hDC,
            x - xRadius,
            y - yRadius,
            x + xRadius,
            y + yRadius,
            (int)round(x + xRadius * cos(sr)),
            (int)round(y - yRadius * sin(sr)),
            (int)round(x + xRadius * cos(er)),
            (int)round(y - yRadius * sin(er)));
    }
    CONVERT_IMAGE_END;
}

void fillpief(float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    double sr = startAngle / 180.0 * PI, er = endAngle / 180.0 * PI;
    if (img) {
        Pie(img->m_hDC,
            (int)(x - xRadius),
            (int)(y - yRadius),
            (int)(x + xRadius),
            (int)(y + yRadius),
            (int)round(x + xRadius * cos(sr)),
            (int)round(y - yRadius * sin(sr)),
            (int)round(x + xRadius * cos(er)),
            (int)round(y - yRadius * sin(er)));
    }
    CONVERT_IMAGE_END;
}

void solidpie(int x, int y, int startAngle, int endAngle, int xRadius, int yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillpie(x, y, startAngle, endAngle, xRadius, yRadius, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void solidpief(float x, float y, float startAngle, float endAngle, float xRadius, float yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillpief(x, y, startAngle, endAngle, xRadius, yRadius, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void fillellipse(int x, int y, int xRadius, int yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Ellipse(img->m_hDC, x - xRadius, y - yRadius, x + xRadius, y + yRadius);
    }
    CONVERT_IMAGE_END;
}

void fillellipsef(float x, float y, float xRadius, float yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Ellipse(img->m_hDC, (int)(x - xRadius), (int)(y - yRadius), (int)(x + xRadius), (int)(y + yRadius));
    }
    CONVERT_IMAGE_END;
}

void solidellipse(int x, int y, int xRadius, int yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillellipse(x, y, xRadius, yRadius, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void solidellipsef(float x, float y, float xRadius, float yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillellipsef(x, y, xRadius, yRadius, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void fillcircle(int x, int y, int radius, PIMAGE pimg)
{
    fillellipse(x, y, radius, radius, pimg);
}

void fillcirclef(float x, float y, float radius, PIMAGE pimg)
{
    fillellipsef(x,y,radius,radius,pimg);
}

void solidcircle(int x, int y, int radius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillcircle(x, y, radius, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void solidcirclef(float x, float y, float radius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillcirclef(x, y, radius, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void bar(int left, int top, int right, int bottom, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    RECT rect = {left, top, right, bottom};
    HBRUSH hbr_last = (HBRUSH)GetCurrentObject(img->m_hDC, OBJ_BRUSH); //(HBRUSH)SelectObject(pg->g_hdc, hbr);

    if (img) {
        FillRect(img->m_hDC, &rect, hbr_last);
    }
    CONVERT_IMAGE_END;
}

void roundrect(int left, int top, int right, int bottom, int xRadius, int yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        HBRUSH oldBrush = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_BRUSH));
        RoundRect(img->m_hDC, left, top, right, bottom, xRadius * 2 , yRadius * 2);
        SelectObject(img->m_hDC, oldBrush);
    }
    CONVERT_IMAGE_END;
}

void roundrect(int left, int top, int right, int bottom, int radius,  PIMAGE pimg)
{
    roundrect(left, top, right, bottom, radius, radius, pimg);
}

void fillroundrect(int left, int top, int right, int bottom, int radius,  PIMAGE pimg)
{
    fillroundrect(left, top, right, bottom, radius, radius, pimg);
}

void solidroundrect(int left, int top, int right, int bottom, int radius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillroundrect(left, top, right, bottom, radius, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void fillroundrect(int left, int top, int right, int bottom, int xRadius, int yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        RoundRect(img->m_hDC, left, top, right, bottom, xRadius * 2, yRadius * 2);
    }
    CONVERT_IMAGE_END;
}

void solidroundrect(int left, int top, int right, int bottom, int xRadius, int yRadius, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillroundrect(left, top, right, bottom, xRadius, yRadius, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void fillrect(int left, int top, int right, int bottom, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Rectangle(img->m_hDC, left, top, right, bottom);
    }
    CONVERT_IMAGE_END;
}

void solidrect(int left, int top, int right, int bottom, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillrect(left, top, right, bottom, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void bar3d(int left, int top, int right, int bottom, int depth, int topFlag, PIMAGE pimg)
{
    /* 6个外边界顶点(从左上角开始逆时针数) */
    POINT boundVertexes[6] = {
        {left, top},
        {left,  bottom},
        {right, bottom},
        {right + depth, bottom - depth},
        {right + depth, top - depth},
        {left  + depth, top - depth},
        };


    bar(left, top, right, bottom, pimg);

    line_cap_type startCap, endCap;
    getlinecap(&startCap, &endCap, pimg);
    setlinecap(LINECAP_FLAT, pimg);

    if (topFlag) {
        /* 正面右上边界的3个顶点 */
        POINT sideVertexes[3] = {{left, top}, {right, top}, {right, bottom}};
        polygon(6, (const int*)boundVertexes, pimg);
        polyline(3, (const int*)&sideVertexes, pimg);
        line(right, top, right + depth, top - depth, pimg);
    } else {
        /* 只绘制与底部相连的 5 条边 */
        polyline(5, (const int*)boundVertexes, pimg);
        line(right, top, right, bottom, pimg);
    }

    setlinecap(startCap, endCap, pimg);
}

void drawpoly(int numOfPoints, const int* points, PIMAGE pimg)
{
    Gdiplus::Point* pointArray = (Gdiplus::Point*)points;

    /* 闭合曲线, 转为绘制带边框无填充多边形 */
    if ((numOfPoints > 3) && (pointArray[0].Equals(pointArray[numOfPoints-1]))) {
        polygon(numOfPoints - 1, points, pimg);
    } else {
        polyline(numOfPoints, points, pimg);
    }
}

void fillpoly(int numOfPoints, const int* points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img) {
        Polygon(img->m_hDC, (const POINT*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void solidpoly(int numOfPoints, const int *points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    HBRUSH oldPen = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_PEN));
    fillpoly(numOfPoints, points, pimg);
    SelectObject(img->m_hDC, oldPen);
    CONVERT_IMAGE_END
}

void polyline(int numOfPoints, const int *points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Polyline(img->m_hDC, (const POINT*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void polygon(int numOfPoints, const int *points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        HBRUSH oldBrush = (HBRUSH)SelectObject(img->m_hDC, GetStockObject(NULL_BRUSH));
        Polygon(img->m_hDC, (const POINT*)points, numOfPoints);
        SelectObject(img->m_hDC, oldBrush);
    }
    CONVERT_IMAGE_END;
}

void fillpoly_gradient(int numOfPoints, const ege_colpoint* points, PIMAGE pimg)
{
    if (numOfPoints < 3) {
        return;
    }
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        TRIVERTEX* vert = (TRIVERTEX*)malloc(sizeof(TRIVERTEX) * numOfPoints);
        if (vert) {
            GRADIENT_TRIANGLE* tri = (GRADIENT_TRIANGLE*)malloc(sizeof(GRADIENT_TRIANGLE) * (numOfPoints - 2));
            if (tri) {
                for (int i = 0; i < numOfPoints; ++i) {
                    vert[i].x = (long)points[i].x;
                    vert[i].y = (long)points[i].y;
                    vert[i].Red = EGEGET_R(points[i].color) << 8;
                    vert[i].Green = EGEGET_G(points[i].color) << 8;
                    vert[i].Blue = EGEGET_B(points[i].color) << 8;
                    // vert[i].Alpha   = EGEGET_A(points[i].color) << 8;
                    vert[i].Alpha = 0;
                }
                for (int j = 0; j < numOfPoints - 2; ++j) {
                    tri[j].Vertex1 = j;
                    tri[j].Vertex2 = j + 1;
                    tri[j].Vertex3 = j + 2;
                }
                dll::GradientFill(img->getdc(), vert, numOfPoints, tri, numOfPoints - 2, GRADIENT_FILL_TRIANGLE);
                free(tri);
            }
            free(vert);
        }
    }
    CONVERT_IMAGE_END;
}

void drawbezier(int numOfPoints, const int* points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (numOfPoints % 3 != 1) {
            numOfPoints = numOfPoints - (numOfPoints + 2) % 3;
        }
        PolyBezier(img->m_hDC, (POINT*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void drawlines(int numlines, const int* points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        DWORD* pl = (DWORD*)malloc(sizeof(DWORD) * numlines);
        for (int i = 0; i < numlines; ++i) {
            pl[i] = 2;
        }
        PolyPolyline(img->m_hDC, (POINT*)points, pl, numlines);
        free(pl);
    }
    CONVERT_IMAGE_END;
}



void floodfill(int x, int y, int borderColor, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        FloodFill(img->m_hDC, x, y, ARGBTOZBGR(borderColor));
    }
    CONVERT_IMAGE_END;
}

void floodfillsurface(int x, int y, color_t areacolor, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        ExtFloodFill(img->m_hDC, x, y, ARGBTOZBGR(areacolor), FLOODFILLSURFACE);
    }
    CONVERT_IMAGE_END;
}

void getlinestyle(int* linestyle, unsigned short* pattern, int* thickness, PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
    if (linestyle) {
        *linestyle = img->m_linestyle.linestyle;
    }
    if (pattern) {
        *pattern = img->m_linestyle.upattern;
    }
    if (thickness) {
        *thickness = img->m_linestyle.thickness;
    }
    CONVERT_IMAGE_END;
}

void setlinestyle(int linestyle, unsigned short pattern, int thickness, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (!(img && img->m_hDC)) {
        CONVERT_IMAGE_END;
        return;
    }

    img->m_linestyle.thickness = thickness;
    img->m_linewidth = (float)thickness;
    img->m_linestyle.linestyle = linestyle;
    img->m_linestyle.upattern = pattern;

    update_pen(img);

    CONVERT_IMAGE_END;
}

void setlinewidth(float width, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img && img->m_hDC) {
        img->m_linestyle.thickness = (int)width;
        img->m_linewidth = width;

        update_pen(img);
    }
    CONVERT_IMAGE_END;
}

Gdiplus::LineCap convertToGdiplusLineCap(line_cap_type linecap)
{
    Gdiplus::LineCap cap = Gdiplus::LineCapFlat;
    switch(linecap) {
        case LINECAP_FLAT:   cap = Gdiplus::LineCapFlat;   break;
        case LINECAP_SQUARE: cap = Gdiplus::LineCapSquare; break;
        case LINECAP_ROUND:  cap = Gdiplus::LineCapRound;  break;
    }

    return cap;
}

Gdiplus::LineJoin convertToGdiplusLineJoin(line_join_type linejoin)
{
    Gdiplus::LineJoin joinType = Gdiplus::LineJoinMiter;
    switch(linejoin) {
        case LINEJOIN_MITER: joinType = Gdiplus::LineJoinMiter; break;
        case LINEJOIN_BEVEL: joinType = Gdiplus::LineJoinBevel; break;
        case LINEJOIN_ROUND: joinType = Gdiplus::LineJoinRound; break;
    }

    return joinType;
}

void setlinecap(line_cap_type linecap, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img && img->m_hDC) {
        img->m_linestartcap = linecap;
        img->m_lineendcap   = linecap;

        update_pen(img);
    }
    CONVERT_IMAGE_END;
}

void setlinecap(line_cap_type startCap, line_cap_type endCap, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img && img->m_hDC) {
        img->m_linestartcap = startCap;
        img->m_lineendcap   = endCap;

        update_pen(img);
    }
    CONVERT_IMAGE_END;
}

void getlinecap(line_cap_type* startCap, line_cap_type* endCap, PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
    if (img && img->m_hDC) {
        if (startCap != NULL) {
            *startCap = img->m_linestartcap;
        }

        if (endCap != NULL) {
            *endCap = img->m_lineendcap;
        }
    }
    CONVERT_IMAGE_END
}

line_cap_type getlinecap(PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);

    if (img && img->m_hDC) {
        return img->m_linestartcap;
    }
    CONVERT_IMAGE_END;
    return LINECAP_FLAT;
}

void setlinejoin(line_join_type linejoin, PIMAGE pimg)
{
    float miterLimit;
    getlinejoin(NULL, &miterLimit, pimg);
    setlinejoin(linejoin, miterLimit, pimg);
}

void setlinejoin(line_join_type linejoin, float miterLimit, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img && img->m_hDC) {
        miterLimit = MAX(1.0f, miterLimit);
        img->m_linejoin = linejoin;
        img->m_linejoinmiterlimit = miterLimit;
        update_pen(img);
    }
    CONVERT_IMAGE_END;
}

void getlinejoin(line_join_type *linejoin, float *miterLimit, PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
    if (img && img->m_hDC) {
        if (linejoin != NULL) {
            *linejoin = img->m_linejoin;
        }

        if (miterLimit != NULL) {
            *miterLimit = img->m_linejoinmiterlimit;
        }
    }
    CONVERT_IMAGE_END
}

line_join_type getlinejoin(PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);

    if (img && img->m_hDC) {
        return img->m_linejoin;
    }
    CONVERT_IMAGE_END;
    return LINEJOIN_MITER;
}

void setfillstyle(int pattern, color_t color, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    LOGBRUSH lbr = {0};
    img->m_fillcolor = color;
    lbr.lbColor = ARGBTOZBGR(color);
    // SetBkColor(img->m_hDC, color);
    if (pattern == EMPTY_FILL) {
        lbr.lbStyle = BS_NULL;
    } else if (pattern == SOLID_FILL) {
        lbr.lbStyle = BS_SOLID;
    } else if (pattern < USER_FILL) { // dose not finish
        int hatchmap[] = {
            HS_HORIZONTAL,
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
        lbr.lbStyle = BS_SOLID;
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

void setrendermode(rendermode_e mode)
{
    if (mode == RENDER_MANUAL) {
        struct _graph_setting* pg = &graph_setting;
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
        struct _graph_setting* pg = &graph_setting;
        delay_ms(0);
        SetTimer(pg->hwnd, RENDER_TIMER_ID, 50, NULL);
        pg->skip_timer_mark = false;
        pg->lock_window = false;
    }
}

void setactivepage(int page)
{
    struct _graph_setting* pg = &graph_setting;
    if (0 <= page && page < BITMAP_PAGE_SIZE) {
        pg->active_page = page;

        /* 为未创建的绘图页分配图像 */
        if (pg->img_page[page] == NULL) {
            color_t bkColor = (page == 0) ? pg->window_initial_color : BLACK;
            pg->img_page[page] = new IMAGE(pg->dc_w, pg->dc_h, bkColor);
        }

        pg->imgtarget = pg->img_page[page];
        pg->dc = pg->img_page[page]->m_hDC;
    }
}

void setvisualpage(int page)
{
    struct _graph_setting* pg = &graph_setting;
    if (0 <= page && page < BITMAP_PAGE_SIZE) {
        pg->visual_page = page;
        if (pg->img_page[page] == NULL) {
            pg->img_page[page] = new IMAGE(pg->dc_w, pg->dc_h, BLACK);
        }
        pg->update_mark_count = 0;
    }
}

void swappage()
{
    struct _graph_setting* pg = &graph_setting;
    setvisualpage(pg->active_page);
    setactivepage(1 - pg->active_page);
}

void window_getviewport(struct viewporttype* viewport)
{
    struct _graph_setting* pg = &graph_setting;
    viewport->left = pg->base_x;
    viewport->top = pg->base_y;
    viewport->right = pg->base_w + pg->base_x;
    viewport->bottom = pg->base_h + pg->base_y;
}

void window_getviewport(int* left, int* top, int* right, int* bottom)
{
    struct _graph_setting* pg = &graph_setting;
    if (left) {
        *left = pg->base_x;
    }
    if (top) {
        *top = pg->base_y;
    }
    if (right) {
        *right = pg->base_w + pg->base_x;
    }
    if (bottom) {
        *bottom = pg->base_h + pg->base_y;
    }
}

void window_setviewport(int left, int top, int right, int bottom)
{
    struct _graph_setting* pg = &graph_setting;
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
    /* 修正窗口大小 */
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
                rect.left -= pt.x;
                rect.top -= pt.y;
                rect.right -= pt.x;
                rect.bottom -= pt.y;
            }

            MoveWindow(pg->hwnd, rect.left, rect.top, rect.right + dw - rect.left, rect.bottom + dh - rect.top, TRUE);
        }
    }
}

void getviewport(int* left, int* top, int* right, int* bottom, int* clip, PCIMAGE pimg)
{
    PCIMAGE img = CONVERT_IMAGE_CONST(pimg);
    if (left) {
        *left = img->m_vpt.left;
    }
    if (top) {
        *top = img->m_vpt.top;
    }
    if (right) {
        *right = img->m_vpt.right;
    }
    if (bottom) {
        *bottom = img->m_vpt.bottom;
    }
    if (clip) {
        *clip = img->m_enableclip;
    }
    CONVERT_IMAGE_END;
}

void setviewport(int left, int top, int right, int bottom, int clip, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    Bound viewport(left, top, right, bottom, false);

    if (!viewport.isNormalized()) {
        return;
    }

    Point oldOrigin(img->m_vpt.left, img->m_vpt.top);
    SetViewportOrgEx(img->m_hDC, 0, 0, NULL);

    img->m_vpt = viewport;
    img->m_enableclip = clip;

    if (clip) {
        HRGN rgn = CreateRectRgn(viewport.left, viewport.top, viewport.right, viewport.bottom);
        SelectClipRgn(img->m_hDC, rgn);
        DeleteObject(rgn);
    } else {
        SelectClipRgn(img->m_hDC, NULL); /* 清除裁剪区域，不做裁剪*/
    }

    /* GDI+ 设置裁剪区域时受当前坐标系影响，确保在设备坐标系下进行 */
    Gdiplus::Graphics* graphics = img->getGraphics();
    Gdiplus::Matrix matrix;
    graphics->GetTransform(&matrix);
    graphics->ResetTransform();

    if (clip) {
        graphics->SetClip(Gdiplus::Rect(viewport.x(), viewport.y(), viewport.width(), viewport.height()));
    } else {
        graphics->ResetClip();
    }

    /* 恢复 GDI+ 坐标系，同时将原点调整至视口区域左上角 */
    graphics->SetTransform(&matrix);
    graphics->TranslateTransform(left - oldOrigin.x, top - oldOrigin.y, Gdiplus::MatrixOrderAppend);
    SetViewportOrgEx(img->m_hDC, left, top, NULL);

    /* 改变视口区域后将当前位置重置为 (0, 0)*/
    MoveToEx(img->m_hDC, 0, 0, NULL);

    CONVERT_IMAGE_END;
}

void clearviewport(PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);

    if (img && img->m_hDC) {
        RECT rect = {0, 0, img->m_vpt.right - img->m_vpt.left, img->m_vpt.bottom - img->m_vpt.top};
        HBRUSH hbr = CreateSolidBrush(GetBkColor(img->m_hDC));
        FillRect(img->m_hDC, &rect, hbr);
        DeleteObject(hbr);
    }
    CONVERT_IMAGE_END;
}

#ifdef EGE_GDIPLUS
Gdiplus::DashStyle linestyle_to_dashstyle(int linestyle)
{
    switch (linestyle) {
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

void ege_line(float x1, float y1, float x2, float y2, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Pen* pen = img->getPen();
        graphics->DrawLine(pen, x1, y1, x2, y2);
    }
    CONVERT_IMAGE_END;
}

void ege_drawpoly(int numOfPoints, const ege_point* points, PIMAGE pimg)
{
    /* 当首尾顶点为同一坐标时转成多边形，否则绘制折线 */
    if (numOfPoints > 3 && points[0].x == points[numOfPoints-1].x
        && points[0].y == points[numOfPoints-1].y) {
        ege_polygon(numOfPoints - 1, points, pimg);
    } else {
        ege_polyline(numOfPoints, points, pimg);
    }
}

void ege_polyline(int numOfPoints, const ege_point *points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Pen* pen = img->getPen();
        graphics->DrawLines(pen, (const Gdiplus::PointF*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void ege_polygon(int numOfPoints, const ege_point *points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Pen* pen = img->getPen();
        graphics->DrawPolygon(pen, (const Gdiplus::PointF*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void ege_drawcurve(int numOfPoints, const ege_point* points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Pen* pen = img->getPen();
        graphics->DrawCurve(pen, (const Gdiplus::PointF*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void ege_drawcurve(int numOfPoints, const ege_point *points, float tension, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->DrawCurve(img->getPen(), (const Gdiplus::PointF*)points, numOfPoints, tension);
    }
    CONVERT_IMAGE_END;
}

void ege_drawclosedcurve(int numOfPoints, const ege_point *points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->DrawClosedCurve(img->getPen(), (const Gdiplus::PointF*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void ege_drawclosedcurve(int numOfPoints, const ege_point *points, float tension, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->DrawClosedCurve(img->getPen(), (const Gdiplus::PointF*)points, numOfPoints, tension);
    }
    CONVERT_IMAGE_END;
}


void ege_fillclosedcurve(int numOfPoints, const ege_point *points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->FillClosedCurve(img->getBrush(), (const Gdiplus::PointF*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void ege_fillclosedcurve(int numOfPoints, const ege_point *points, float tension, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->FillClosedCurve(img->getBrush(), (const Gdiplus::PointF*)points, numOfPoints,
            Gdiplus::FillModeAlternate, tension);
    }
    CONVERT_IMAGE_END;
}

void ege_rectangle(float x, float y, float w, float h, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Pen* pen = img->getPen();
        graphics->DrawRectangle(pen, x, y, w, h);
    }
    CONVERT_IMAGE_END;
}

void ege_circle(float x, float y, float radius, PIMAGE pimg)
{
    ege_ellipse(x - radius, y - radius, radius * 2.0f, radius * 2.0f, pimg);
}

void ege_fillcircle(float x, float y, float radius, PIMAGE pimg)
{
    ege_fillellipse(x - radius, y - radius, radius * 2.0f, radius * 2.0f, pimg);
}

void ege_ellipse(float x, float y, float w, float h, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Pen* pen = img->getPen();
        graphics->DrawEllipse(pen, x, y, w, h);
    }
    CONVERT_IMAGE_END;
}

void ege_pie(float x, float y, float w, float h, float startAngle, float sweepAngle, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Pen* pen = img->getPen();
        graphics->DrawPie(pen, x, y, w, h, startAngle, sweepAngle);
    }
    CONVERT_IMAGE_END;
}

void ege_arc(float x, float y, float w, float h, float startAngle, float sweepAngle, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Pen* pen = img->getPen();
        graphics->DrawArc(pen, x, y, w, h, startAngle, sweepAngle);
    }
    CONVERT_IMAGE_END;
}

void ege_bezier(int numOfPoints, const ege_point* points, PIMAGE pimg)
{
    ege_drawbezier(numOfPoints, points, pimg);
}

void ege_drawbezier(int numOfPoints, const ege_point* points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (img->m_linestyle.linestyle == PS_NULL) {
            return;
        }
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Pen* pen = img->getPen();
        graphics->DrawBeziers(pen, (const Gdiplus::PointF*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void ege_setpattern_none(PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    img->set_pattern(NULL);
    CONVERT_IMAGE_END;
}

void ege_setpattern_lineargradient(float x1, float y1, color_t c1, float x2, float y2, color_t c2, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::LinearGradientBrush* pbrush = new Gdiplus::LinearGradientBrush(
            Gdiplus::PointF(x1, y1), Gdiplus::PointF(x2, y2), Gdiplus::Color(c1), Gdiplus::Color(c2));
        img->set_pattern(pbrush);
    }
    CONVERT_IMAGE_END;
}

void ege_setpattern_pathgradient(ege_point center, color_t centerColor,
    int count, const ege_point* points,
    int colorCount, const color_t* pointColors,
    PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::PathGradientBrush* pbrush =
            new Gdiplus::PathGradientBrush((const Gdiplus::PointF*)points, count, Gdiplus::WrapModeTile);
        pbrush->SetCenterColor(Gdiplus::Color(centerColor));
        pbrush->SetCenterPoint(Gdiplus::PointF(center.x, center.y));
        pbrush->SetSurroundColors((const Gdiplus::Color*)pointColors, &colorCount);
        img->set_pattern(pbrush);
    }
    CONVERT_IMAGE_END;
}

void ege_setpattern_ellipsegradient(ege_point center,
    color_t centerColor,
    float x,
    float y,
    float w,
    float h,
    color_t color,
    PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::GraphicsPath path;
        path.AddEllipse(x, y, w, h);
        Gdiplus::PathGradientBrush* pbrush = new Gdiplus::PathGradientBrush(&path);
        int count = 1;
        pbrush->SetCenterColor(Gdiplus::Color(centerColor));
        pbrush->SetCenterPoint(Gdiplus::PointF(center.x, center.y));
        pbrush->SetSurroundColors((const Gdiplus::Color*)&color, &count);
        img->set_pattern(pbrush);
    }
    CONVERT_IMAGE_END;
}

void ege_setpattern_texture(PIMAGE srcimg, float x, float y, float w, float h, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (srcimg->m_texture) {
            Gdiplus::TextureBrush* pbrush =
                new Gdiplus::TextureBrush((Gdiplus::Image*)srcimg->m_texture, Gdiplus::WrapModeTile, x, y, w, h);
            img->set_pattern(pbrush);
        }
    }
    CONVERT_IMAGE_END;
}

void ege_fillpoly(int numOfPoints, const ege_point* points, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Brush* brush = img->getBrush();
        graphics->FillPolygon(brush, (const Gdiplus::PointF*)points, numOfPoints);
    }
    CONVERT_IMAGE_END;
}

void ege_fillrect(float x, float y, float w, float h, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Brush* brush = img->getBrush();
        graphics->FillRectangle(brush, x, y, w, h);
    }
    CONVERT_IMAGE_END;
}

static Gdiplus::GraphicsPath* createRoundRectPath(float x, float y, float w, float h,
    float radius1, float radius2, float radius3, float radius4)
{
    if ((w <= 0.0f) || (h <= 0.0f))
        return NULL;

    radius1 = clamp(radius1, 0.0f, MIN(w, h));
    radius2 = clamp(radius2, 0.0f, MIN(w - radius1, h));
    radius3 = clamp(radius3, 0.0f, MIN(h - radius2, w));
    radius4 = clamp(radius4, 0.0f, MIN(h - radius1, w - radius3));

    Gdiplus::GraphicsPath* path = new Gdiplus::GraphicsPath;

    if (radius2 < w - radius1)
        path->AddLine(x + radius1,  y,  x + w - radius2,  y);

    if (radius2 > 0.0f)
        path->AddArc (x + w - (radius2 * 2),  y,  radius2 * 2,  radius2 * 2,  270,  90);

    if (radius3 < h - radius2)
        path->AddLine(x + w,  y + radius2,  x + w,  y + h - radius3);

    if (radius3 > 0.0f)
        path->AddArc (x + w - (radius3 * 2),  y + h - (radius3 * 2),  radius3 * 2,  radius3 * 2,  0,  90);

    if (radius4 < w - radius3)
        path->AddLine(x + w - radius3,  y + h,  x + radius4,  y + h);

    if (radius4 > 0.0f)
        path->AddArc (x,  y + h - (radius4 * 2),  radius4 * 2,  radius4 * 2,  90,  90);

    if (radius4 < w - radius1)
        path->AddLine(x,  y + h - radius4 ,  x,  y + radius1);

    if (radius1 > 0.0f)
        path->AddArc (x,  y, radius1 * 2,  radius1 * 2,  180,  90);

    path->CloseFigure();

    return path;
}

void ege_roundrect(float x, float y, float w, float h,  float radius, PIMAGE pimg)
{
    ege_roundrect(x, y, w, h, radius,  radius,  radius,  radius);
}
void ege_fillroundrect(float x, float y, float w, float h,  float radius, PIMAGE pimg)
{
    ege_fillroundrect(x, y, w, h, radius,  radius,  radius,  radius, pimg);
}
void ege_roundrect(float x, float y, float w, float h,  float radius1, float radius2, float radius3, float radius4, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    Gdiplus::GraphicsPath* path = createRoundRectPath(x, y, w, h, radius1,  radius2,  radius3,  radius4);

    if (path != NULL) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->DrawPath(img->getPen(), path);
        delete path;
    }
    CONVERT_IMAGE_END
}

void ege_fillroundrect(float x, float y, float w, float h,  float radius1, float radius2, float radius3, float radius4, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    Gdiplus::GraphicsPath* path = createRoundRectPath(x, y, w, h, radius1,  radius2,  radius3,  radius4);

    if (path != NULL) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->FillPath(img->getBrush(), path);
        delete path;
    }
    CONVERT_IMAGE_END
}

void ege_fillellipse(float x, float y, float w, float h, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Brush* brush = img->getBrush();
        graphics->FillEllipse(brush, x, y, w, h);
    }
    CONVERT_IMAGE_END;
}

void ege_fillpie(float x, float y, float w, float h, float startAngle, float sweepAngle, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Brush* brush = img->getBrush();
        graphics->FillPie(brush, x, y, w, h, startAngle, sweepAngle);
    }
    CONVERT_IMAGE_END;
}

void ege_setalpha(int alpha, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img && img->m_hDC) {
        int a = alpha << 24;
        int len = img->m_width * img->m_height;
        for (int i = 0; i < len; ++i) {
            DWORD c = img->m_pBuffer[i];
            img->m_pBuffer[i] = a | (c & 0xFFFFFF);
        }
    }
    CONVERT_IMAGE_END;
}

void ege_gentexture(bool generate, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        img->gentexture(generate);
    }
    CONVERT_IMAGE_END;
}

void ege_puttexture(PCIMAGE srcimg, float x, float y, float w, float h, PIMAGE pimg)
{
    ege_rect dest = {x, y, w, h};
    ege_puttexture(srcimg, dest, pimg);
}

void ege_puttexture(PCIMAGE srcimg, ege_rect dest, PIMAGE pimg)
{
    ege_rect src;
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        src.x = 0;
        src.y = 0;
        src.w = (float)srcimg->getwidth();
        src.h = (float)srcimg->getheight();
        ege_puttexture(srcimg, dest, src, img);
    }
    CONVERT_IMAGE_END;
}

void ege_puttexture(PCIMAGE srcimg, ege_rect dest, ege_rect src, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        if (srcimg->m_texture) {
            Gdiplus::Graphics* graphics = img->getGraphics();
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
            // graphics.SetTransform();
            graphics->DrawImage((Gdiplus::Image*)srcimg->m_texture,
                Gdiplus::RectF(dest.x, dest.y, dest.w, dest.h),
                src.x, src.y, src.w, src.h,
                Gdiplus::UnitPixel,
                NULL);
        }
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_drawimage(PCIMAGE srcimg, int xDest, int yDest, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img && srcimg) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Bitmap bitmap(srcimg->getwidth(),
            srcimg->getheight(),
            4 * srcimg->getwidth(),
            PixelFormat32bppARGB,
            (BYTE*)(srcimg->m_pBuffer));
        Gdiplus::Point p(xDest, yDest);
        graphics->DrawImage(&bitmap, p);
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_drawimage(PCIMAGE srcimg,
    int xDest,
    int yDest,
    int widthDest,
    int heightDest,
    int xSrc,
    int ySrc,
    int srcWidth,
    int srcHeight,
    PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img && srcimg) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Bitmap bitmap(srcimg->getwidth(),
            srcimg->getheight(),
            4 * srcimg->getwidth(),
            PixelFormat32bppARGB,
            (BYTE*)(srcimg->m_pBuffer));
        Gdiplus::Point destPoints[3] = {
            Gdiplus::Point(xDest, yDest), Gdiplus::Point(xDest + widthDest, yDest), Gdiplus::Point(xDest, yDest + heightDest)};
        graphics->DrawImage(
            &bitmap, destPoints, 3, xSrc, ySrc, srcWidth, srcHeight, Gdiplus::UnitPixel, NULL, NULL, NULL);
    }
    CONVERT_IMAGE_END;
}


ege_path::ege_path()
{
    gdiplusinit();
    m_data = new Gdiplus::GraphicsPath;
}

ege_path::ege_path(const ege_point *points, const unsigned char *types, int count)
{
    gdiplusinit();
    m_data = new Gdiplus::GraphicsPath((const Gdiplus::PointF*)points, (const BYTE*)types, count);
}

ege_path::ege_path(const ege_path &path)
{
    const Gdiplus::GraphicsPath* graphicsPath = (const Gdiplus::GraphicsPath*)path.m_data;
    m_data = (graphicsPath != NULL) ? graphicsPath->Clone() : NULL;
}

ege_path::~ege_path()
{
    if (m_data != NULL) {
        delete (Gdiplus::GraphicsPath*)m_data;
    }
}

const void* ege_path::data() const
{
    return m_data;
}

void* ege_path::data()
{
    return m_data;
}

ege_path& ege_path::operator=(const ege_path& path)
{
    if (this != &path) {
        if (m_data != NULL) {
            delete (Gdiplus::GraphicsPath*)m_data;
        }

        const Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path.m_data;
        m_data = (graphicsPath != NULL) ? graphicsPath->Clone() : NULL;
    }

    return *this;
}

void ege_drawpath(const ege_path* path, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if ((img != NULL) && (path != NULL)) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->DrawPath(img->getPen(), (Gdiplus::GraphicsPath*)path->data());
    }
    CONVERT_IMAGE_END;
}

void ege_fillpath(const ege_path* path, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if ((img != NULL) && (path != NULL)) {
        const Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            Gdiplus::Graphics* graphics = img->getGraphics();
            graphics->FillPath(img->getBrush(), graphicsPath);
        }
    }
    CONVERT_IMAGE_END;
}

void ege_drawpath(const ege_path* path, float x, float y, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if ((img != NULL) && (path != NULL)) {
        const Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            Gdiplus::Graphics* graphics = img->getGraphics();
            graphics->TranslateTransform(x, y);
            graphics->DrawPath(img->getPen(), graphicsPath);
            graphics->TranslateTransform(-x, -y);
        }
    }
    CONVERT_IMAGE_END;
}

void ege_fillpath(const ege_path* path, float x, float y, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if ((img != NULL) && (path != NULL)) {
        const Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            Gdiplus::Graphics* graphics = img->getGraphics();
            graphics->TranslateTransform(x, y);
            graphics->FillPath(img->getBrush(), graphicsPath);
            graphics->TranslateTransform(-x, -y);
        }
    }
    CONVERT_IMAGE_END;
}

ege_path* ege_path_create()
{
    return new(std::nothrow) ege_path;
}

ege_path* ege_path_createfrom(const ege_point* points, const unsigned char* types, int count)
{
    return new(std::nothrow) ege_path(points, types, count);
}

ege_path* ege_path_clone(const ege_path* path)
{
    if (path == NULL)
        return NULL;

    return new(std::nothrow) ege_path(*path);
}

void ege_path_destroy(const ege_path* path)
{
    delete path;
}

void ege_path_start(ege_path* path)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->StartFigure();
        }
    }
}

void ege_path_close(ege_path* path)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->CloseFigure();
        }
    }
}

void ege_path_closeall(ege_path* path)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->CloseAllFigures();
        }
    }
}

void ege_path_setfillmode(ege_path* path, fill_mode mode)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            Gdiplus::FillMode fillMode = Gdiplus::FillModeAlternate;
            switch (mode) {
                case FILLMODE_ALTERNATE: fillMode = Gdiplus::FillModeAlternate; break;
                case FILLMODE_WINDING:   fillMode = Gdiplus::FillModeWinding;   break;
                default:                                                        break;
            }
            graphicsPath->SetFillMode(fillMode);
        }
    }
}

void ege_path_reset(ege_path* path)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->Reset();
        }
    }
}

void ege_path_reverse(ege_path* path)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->Reverse();
        }
    }
}

void ege_path_widen(ege_path* path, float lineWidth, const ege_transform_matrix* matrix)
{
    ege_path_widen(path, lineWidth, matrix, Gdiplus::FlatnessDefault);
}

void ege_path_widen(ege_path* path, float lineWidth, const ege_transform_matrix* matrix,  float flatness)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            const Gdiplus::Pen pen(Gdiplus::Color(), lineWidth);

            if (matrix != NULL) {
                Gdiplus::Matrix mat;
                matrixConvert(*matrix, mat);
                graphicsPath->Widen(&pen, &mat, flatness);
            } else {
                graphicsPath->Widen(&pen, NULL, flatness);
            }
        }
    }
}

void ege_path_flatten(ege_path* path, const ege_transform_matrix* matrix)
{
    ege_path_flatten(path, matrix, Gdiplus::FlatnessDefault);
}

void ege_path_flatten(ege_path* path, const ege_transform_matrix* matrix, float flatness)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();

        if (graphicsPath != NULL) {
            if (matrix != NULL) {
                Gdiplus::Matrix mat;
                matrixConvert(*matrix, mat);
                graphicsPath->Flatten(&mat, flatness);
            } else {
                graphicsPath->Flatten(NULL, flatness);
            }
        }
    }
}

void ege_path_warp(ege_path* path, const ege_point* points, int count, const ege_rect* rect,
     const ege_transform_matrix* matrix)
{
    ege_path_warp(path, points, count , rect, matrix, Gdiplus::FlatnessDefault);
}

void ege_path_warp(ege_path* path, const ege_point* points, int count, const ege_rect* rect,
    const ege_transform_matrix* matrix, float flatness)
{
    if ((path != NULL) && (points != NULL) && (rect != NULL) && ((count == 3) || (count == 4))) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            const Gdiplus::PointF* p = (const Gdiplus::PointF*)points;
            const Gdiplus::RectF r(rect->x, rect->y, rect->w, rect->h);

            if (matrix != NULL) {
                Gdiplus::Matrix mat;
                matrixConvert(*matrix, mat);
                graphicsPath->Warp(p, count, r, &mat, Gdiplus::WarpModePerspective, flatness);
            } else {
                graphicsPath->Warp(p, count, r, NULL, Gdiplus::WarpModePerspective, flatness);
            }
        }
    }
}

void ege_path_outline(ege_path* path, const ege_transform_matrix* matrix)
{
    ege_path_outline(path, matrix, Gdiplus::FlatnessDefault);
}

void ege_path_outline(ege_path* path, const ege_transform_matrix* matrix, float flatness)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            if (matrix != NULL) {
                Gdiplus::Matrix mat;
                matrixConvert(*matrix, mat);
                graphicsPath->Outline(&mat, flatness);
            } else {
                graphicsPath->Outline(NULL, flatness);
            }
        }
    }
}

bool ege_path_inpath(const ege_path* path, float x, float y)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            return graphicsPath->IsVisible(x, y);
        }
    }
    return false;
}

bool ege_path_inpath(const ege_path* path, float x, float y, PCIMAGE pimg)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            PIMAGE img = CONVERT_IMAGE_CONST((PIMAGE)pimg);
            if ((img != NULL) && (img->m_hDC != NULL)) {
                return graphicsPath->IsVisible(x, y, img->getGraphics());
            }
        }
    }
    return false;
}

bool ege_path_instroke(const ege_path* path, float x, float y)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            Gdiplus::Pen pen(Gdiplus::Color(), 1.0f);
            return graphicsPath->IsOutlineVisible(x, y, &pen);
        }
    }
    return false;
}

bool ege_path_instroke(const ege_path* path, float x, float y, PCIMAGE pimg)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            PIMAGE img = CONVERT_IMAGE_CONST((PIMAGE)pimg);
            if ((img != NULL) && (img->m_hDC != NULL)) {
                return graphicsPath->IsOutlineVisible(x, y, img->getPen(), img->getGraphics());
            }
        }
    }
    return false;
}

ege_point ege_path_lastpoint(const ege_path* path)
{
    ege_point lastPoint = {0.0f, 0.0f};
    if (path != NULL) {
        const Gdiplus::GraphicsPath* graphicsPath = (const Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->GetLastPoint((Gdiplus::PointF*)&lastPoint);
        }
    }
    return lastPoint;
}

int ege_path_pointcount(const ege_path* path)
{
    int pointCount = 0;
    if (path != NULL) {
        const Gdiplus::GraphicsPath* graphicsPath = (const Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            pointCount = graphicsPath->GetPointCount();
        }
    }
    return pointCount;
}

ege_rect ege_path_getbounds(const ege_path* path, const ege_transform_matrix* matrix)
{
    ege_rect bounds = {0.0f, 0.0f, 0.0f, 0.0f};
    if (path != NULL) {
        const Gdiplus::GraphicsPath* graphicsPath = (const Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            if (matrix != NULL) {
                Gdiplus::Matrix mat;
                matrixConvert(*matrix, mat);
                graphicsPath->GetBounds((Gdiplus::RectF*)&bounds, &mat);
            } else {
                graphicsPath->GetBounds((Gdiplus::RectF*)&bounds, NULL);
            }
        }
    }

    return bounds;
}

ege_rect ege_path_getbounds(const ege_path* path, const ege_transform_matrix* matrix, PCIMAGE pimg)
{
    ege_rect bounds = {0.0f, 0.0f, 0.0f, 0.0f};
    if (path != NULL) {
        const Gdiplus::GraphicsPath* graphicsPath = (const Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            PIMAGE img = CONVERT_IMAGE_CONST((PIMAGE)pimg);
            if (matrix != NULL) {
                Gdiplus::Matrix mat;
                matrixConvert(*matrix, mat);
                graphicsPath->GetBounds((Gdiplus::RectF*)&bounds, &mat, img->getPen());
            } else {
                graphicsPath->GetBounds((Gdiplus::RectF*)&bounds, NULL, img->getPen());
            }
            CONVERT_IMAGE_END
        }
    }

    return bounds;
}

ege_point* ege_path_getpathpoints(const ege_path* path, ege_point* points)
{
    if ((path != NULL)) {
        const Gdiplus::GraphicsPath* graphicsPath = (const Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            int pointCount = graphicsPath->GetPointCount();

            if (points == NULL) {
                points = new(std::nothrow) ege_point[pointCount];
            }

            if (points != NULL) {
                graphicsPath->GetPathPoints((Gdiplus::PointF*)points, pointCount);
            }
            return points;
        }
    }

    return NULL;
}

unsigned char* ege_path_getpathtypes(const ege_path* path, unsigned char* types)
{
    if ((path != NULL)) {
        const Gdiplus::GraphicsPath* graphicsPath = (const Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            int pointCount = graphicsPath->GetPointCount();

            if (types == NULL) {
                types = new(std::nothrow) unsigned char[pointCount];
            }

            if (types != NULL) {
                graphicsPath->GetPathTypes(types, pointCount);
            }

            return types;
        }
    }

    return NULL;
}

void ege_path_transform(ege_path* path, const ege_transform_matrix *matrix)
{
    if ((path != NULL) && (matrix != NULL)) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            Gdiplus::Matrix mat;
            matrixConvert(*matrix, mat);
            graphicsPath->Transform(&mat);
        }
    }
}

void ege_path_addpath(ege_path* dstPath, const ege_path* srcPath, bool connect)
{
    if ((dstPath != NULL) && (srcPath != NULL)) {
        Gdiplus::GraphicsPath* dstGraphicsPath = (Gdiplus::GraphicsPath*)dstPath->data();
        const Gdiplus::GraphicsPath* srcGraphicsPath = (const Gdiplus::GraphicsPath*)srcPath->data();
        if ((dstGraphicsPath != NULL) && (srcGraphicsPath != NULL)) {
            dstGraphicsPath->AddPath(srcGraphicsPath, connect);
        }
    }
}

void ege_path_addline(ege_path* path, float x1, float y1, float x2, float y2)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddLine(x1, y1, x2, y2);
        }
    }
}

void ege_path_addarc(ege_path* path, float x, float y, float width, float height, float startAngle, float sweepAngle)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddArc(x, y, width, height, startAngle, sweepAngle);
        }
    }
}

void ege_path_addpolyline(ege_path* path, int numOfPoints, const ege_point *points)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddLines((const Gdiplus::PointF*)points, numOfPoints);
        }
    }
}

void ege_path_addbezier(ege_path* path, int numOfPoints, const ege_point *points)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddBeziers((const Gdiplus::PointF*)points, numOfPoints);
        }
    }
}

void ege_path_addbezier(ege_path* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddBezier(x1, y1, x2, y2, x3, y3, x4, y4);
        }
    }
}

void ege_path_addcurve(ege_path* path, int numOfPoints, const ege_point *points)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddCurve((const Gdiplus::PointF*)points, numOfPoints);
        }
    }
}

void ege_path_addcurve(ege_path* path, int numOfPoints, const ege_point *points, float tension)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddCurve((const Gdiplus::PointF*)points, numOfPoints, tension);
        }
    }
}

void ege_path_addcircle(ege_path* path, float x, float y, float radius)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddEllipse(x - radius, y - radius, radius * 2.0f, radius * 2.0f);
        }
    }
}

void ege_path_addrect(ege_path* path, float x, float y, float width, float height)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            Gdiplus::RectF rect(x, y, width, height);
            graphicsPath->AddRectangle(rect);
        }
    }
}

void ege_path_addellipse(ege_path* path, float x, float y, float width, float height)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddEllipse(x, y, width, height);
        }
    }
}

void ege_path_addpie(ege_path* path, float x, float y, float width, float height, float startAngle, float sweepAngle)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddPie(x, y, width, height, startAngle, sweepAngle);
        }
    }
}

void ege_path_addtext(ege_path* path, float x, float y, const char* text, float height, int length,
     const char* typeface, int fontStyle)
{
    ege_path_addtext(path, x, y, mb2w(text).c_str(), height, length, mb2w(typeface).c_str(), fontStyle);
}

void ege_path_addtext(ege_path* path, float x, float y, const wchar_t* text, float height, int length,
     const wchar_t* typeface, int fontStyle)
{
    if ((path != NULL) && (text != NULL) && (length != 0))  {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {

            Gdiplus::REAL emSize = height;
            Gdiplus::PointF origin(x, y);
            const Gdiplus::StringFormat* format = Gdiplus::StringFormat::GenericTypographic();

            if ((typeface == NULL) || (typeface[0] == L'\0')) {
                typeface = L"SimSun";
            }

            Gdiplus::FontFamily fontFamliy(typeface);

            INT style = 0;
            if (fontStyle & FONTSTYLE_BOLD)       style |= Gdiplus::FontStyleBold;
            if (fontStyle & FONTSTYLE_ITALIC)     style |= Gdiplus::FontStyleItalic;
            if (fontStyle & FONTSTYLE_UNDERLINE)  style |= Gdiplus::FontStyleUnderline;
            if (fontStyle & FONTSTYLE_STRIKEOUT)  style |= Gdiplus::FontStyleStrikeout;

            graphicsPath->AddString(text, length, &fontFamliy, style, emSize, origin, format);
        }
    }
}

void ege_path_addpolygon(ege_path* path, int numOfPoints, const ege_point *points)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddPolygon((const Gdiplus::PointF*)points, numOfPoints);
        }
    }
}

void ege_path_addclosedcurve(ege_path* path, int numOfPoints, const ege_point *points)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddClosedCurve((const Gdiplus::PointF*)points, numOfPoints);
        }
    }
}

void ege_path_addclosedcurve(ege_path* path, int numOfPoints, const ege_point *points, float tension)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            graphicsPath->AddClosedCurve((const Gdiplus::PointF*)points, numOfPoints, tension);
        }
    }
}


void EGEAPI ege_transform_rotate(float angle, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->RotateTransform(angle);
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_transform_translate(float x, float y, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->TranslateTransform(x, y);
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_transform_scale(float scale_x, float scale_y, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->ScaleTransform(scale_x, scale_y);
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_transform_reset(PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        graphics->ResetTransform();
    }
    CONVERT_IMAGE_END;
}
void EGEAPI ege_get_transform(ege_transform_matrix* matrix, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img && matrix) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Matrix mat;
        Gdiplus::REAL elements[6];
        graphics->GetTransform(&mat);
        mat.GetElements(elements);
        matrix->m11 = elements[0];
        matrix->m12 = elements[1];
        matrix->m21 = elements[2];
        matrix->m22 = elements[3];
        matrix->m31 = elements[4];
        matrix->m32 = elements[5];
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_set_transform(const ege_transform_matrix* matrix, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img && matrix) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Matrix mat;
        matrixConvert(*matrix, mat);
        graphics->SetTransform(&mat);
    }
    CONVERT_IMAGE_END;
}

ege_point EGEAPI ege_transform_calc(ege_point p, PIMAGE pimg)
{
    return ege_transform_calc(p.x, p.y, pimg);
}

ege_point EGEAPI ege_transform_calc(float x, float y, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    ege_point point = {0.0f, 0.0f};
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        Gdiplus::Matrix matrix;
        graphics->GetTransform(&matrix);
        matrix.TransformPoints((Gdiplus::PointF*)&point, 1);
    }
    CONVERT_IMAGE_END;
    return point;
}

#endif // EGEGDIPLUS

HWND getHWnd()
{
    struct _graph_setting* pg = &graph_setting;
    return pg->hwnd;
}

HINSTANCE getHInstance()
{
    struct _graph_setting* pg = &graph_setting;
    return pg->instance;
}

int message_addkeyhandler(void* param, LPMSG_KEY_PROC func)
{
    struct _graph_setting* pg = &graph_setting;
    pg->callback_key = func;
    pg->callback_key_param = param;
    return grOk;
}

int message_addmousehandler(void* param, LPMSG_MOUSE_PROC func)
{
    struct _graph_setting* pg = &graph_setting;
    pg->callback_mouse = func;
    pg->callback_mouse_param = param;
    return grOk;
}

int SetCloseHandler(LPCALLBACK_PROC func)
{
    struct _graph_setting* pg = &graph_setting;
    pg->callback_close = func;
    return grOk;
}

/* private funcion */
static void draw_frame(PIMAGE img, int l, int t, int r, int b, color_t lc, color_t dc)
{
    setcolor(lc, img);
    line(l, b, l, t, img);
    lineto(r, t, img);
    setcolor(dc, img);
    lineto(r, b, img);
    lineto(l, b, img);
}

int inputbox_getline(const char* title, const char* text, LPSTR buf, int len)
{
    const std::wstring& title_w = mb2w(title);
    const std::wstring& text_w = mb2w(text);
    std::wstring buf_w(len, L'\0');
    int ret = inputbox_getline(title_w.c_str(), text_w.c_str(), &buf_w[0], len);
    if (ret) {
        WideCharToMultiByte(getcodepage(), 0, buf_w.c_str(), -1, buf, len, 0, 0);
    }
    return ret;
}

int inputbox_getline(const wchar_t* title, const wchar_t* text, LPWSTR buf, int len)
{
    IMAGE bg;
    IMAGE window;
    int w = 400, h = 300, x = (getwidth() - w) / 2, y = (getheight() - h) / 2;
    int ret = 0;

    bg.getimage(0, 0, getwidth(), getheight());
    window.resize(w, h);
    buf[0] = 0;

    sys_edit edit(true);
    edit.create(true);
    edit.move(x + 30 + 1, y + 192 + 1);
    edit.size(w - (30 + 1) * 2, h - 40 - 192 - 2);
    edit.setmaxlen(len);
    edit.visible(true);
    edit.setfocus();

    setbkcolor(EGERGB(0x80, 0xA0, 0x80), &window);
    draw_frame(&window, 0, 0, w - 1, h - 1, EGERGB(0xA0, 0xC0, 0xA0), EGERGB(0x50, 0x70, 0x50));
    setfillcolor(EGERGB(0, 0, 0xA0), &window);

    for (int dy = 1; dy < 24; dy++) {
        setcolor(HSLtoRGB(240.0f, 1.0f, 0.5f + float(dy / 24.0 * 0.3)), &window);
        line(1, dy, w - 1, dy, &window);
    }

    setcolor(0xFFFFFF, &window);
    setbkmode(TRANSPARENT, &window);
    setfont(18, 0, L"Tahoma", &window);
    outtextxy(3, 3, title, &window);
    setcolor(0x0, &window);

    {
        RECT rect = {30, 32, w - 30, 128 - 3};
        DrawTextW(window.m_hDC,
            text,
            -1,
            &rect,
            DT_NOPREFIX | DT_LEFT | DT_TOP | TA_NOUPDATECP | DT_WORDBREAK | DT_EDITCONTROL | DT_EXPANDTABS);
    }

    putimage(0, 0, &bg);
    putimage(x, y, &window);
    delay_ms(0);

    while (is_run()) {
        key_msg msg = getkey();
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

static double static_frameRate = 0.0;         /* 帧率 */
static int    static_frameCount = 0;          /* 帧数 */
static double static_totalFrameTime = 0.0;    /* 累计时间 */
static double static_lastFrameTime = 0.0;     /* 上一帧更新时间 */

/**
 * 更新帧率
 * @param addFrameCount 是否增加帧数。{true: 帧数计数加一，同时更新帧率; false: 仅更新帧率}
 * @details 帧率通过统计每个固定周期(0.5秒)内的帧数获得。在每个统计周期中，帧率仅在累计时长满一个周期后才会更新。
 */
void updateFrameRate(bool addFrameCount)
{
    struct _graph_setting* pg = &graph_setting;
    double currentTime = get_highfeq_time_ls(pg);

    if (static_lastFrameTime == 0.0) {
        static_lastFrameTime = currentTime;
        return;
    }

    double elapsedTime = static_totalFrameTime + (currentTime - static_lastFrameTime);

    if (addFrameCount) {
        static_frameCount++;
        static_totalFrameTime = elapsedTime;
        static_lastFrameTime  = currentTime;
    }

    /* 以 0.5 秒为一个统计周期，统计时间不足时不更新帧率 */
    if (elapsedTime >= 0.5) {
        static_frameRate = static_frameCount / elapsedTime;

        static_frameCount = 0;
        static_totalFrameTime = 0.0;
        static_lastFrameTime = currentTime;
    }
}

void resetFrameRate()
{
    static_frameRate = 0.0;
    static_frameCount = 0;
    static_totalFrameTime = 0.0;
    static_lastFrameTime = 0.0;
}

float getfps()
{
    return (float)static_frameRate;
}

double fclock()
{
    struct _graph_setting* pg = &graph_setting;

    if (pg->fclock_start == 0) {
        pg->fclock_start = ::GetTickCount();
    }

    return (::GetTickCount() - pg->fclock_start) / 1000.0; // get_highfeq_time_ls(pg);
}

LRESULT sys_edit::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CTLCOLOREDIT: {
        HDC dc = (HDC)wParam;
        HBRUSH br = ::CreateSolidBrush(ARGBTOZBGR(m_bgcolor));

        ::SetBkColor(dc, ARGBTOZBGR(m_bgcolor));
        ::SetTextColor(dc, ARGBTOZBGR(m_color));
        ::DeleteObject(m_hBrush);
        m_hBrush = br;
        return (LRESULT)br;
    } break;
    case WM_SETFOCUS:
        m_bInputFocus = 1;
        // call textbox's own message process to show caret
        return ((LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM))m_callback)(m_hwnd, message, wParam, lParam);
    case WM_KILLFOCUS:
        m_bInputFocus = 0;
        // call textbox's own message process to hide caret
        return ((LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM))m_callback)(m_hwnd, message, wParam, lParam);
    default:
        return ((LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM))m_callback)(m_hwnd, message, wParam, lParam);
    }
}

} // namespace ege
