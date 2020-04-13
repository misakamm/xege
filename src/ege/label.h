#ifndef _EGE_LABEL_H_
#define _EGE_LABEL_H_

#ifndef _EGE_H_
#error include "label.h" must after include "ege.h" or "graphics.h"
#endif

class label : public egeControlBase
{
public:
    CTL_PREINIT(label, egeControlBase) {
        // do sth. before sub objects' construct function call
    } CTL_PREINITEND;
    label(CTL_DEFPARAM) : CTL_INITBASE(egeControlBase) {
        CTL_INIT; // must be the first line
        size(64, 16);
        m_color = WHITE;
        m_bkcolor = BLACK;
        m_fontheight = 12;
        m_alpha = 0xff;
        m_transparent = false;
        strcpy(m_face, "ËÎÌו");
        redraw();
    }
    void caption(const char* text) {
        strcpy(m_caption, text);
        redraw();
    }
    const char* caption() const {
        return m_caption;
    }
    void fontsize(int height) {
        m_fontheight = height;
        redraw();
    }
    int fontsize() const {
        return m_fontheight;
    }
    void font(const char* fontface) {
        strcpy(m_face, fontface);
        redraw();
    }
    const char* font() const {
        return m_face;
    }
    void color(color_t color) {
        m_color = color;
        redraw();
    }
    color_t color() const {
        return m_color;
    }
    void bkcolor(color_t color) {
        m_bkcolor = color;
        redraw();
    }
    color_t bkcolor() const {
        return m_bkcolor;
    }
    void transparent(bool t) {
        m_transparent = t;
        redraw();
    }
    bool transparent() const {
        return m_transparent;
    }
    void alpha(int alpha) {
        if (alpha < 0) alpha = 0;
        if (alpha > 0xff) alpha = 0xff;
        m_alpha = alpha;
        redraw();
    }
    int alpha() const {
        return m_alpha;
    }
    void redraw() {
        PushTarget targer(buf());

        if (m_alpha < 0xff || m_transparent) blendmode(true);
        else blendmode(false);

        setbkcolor_f(m_bkcolor);
        setcolor(m_color);
        cleardevice();
        setbkmode(TRANSPARENT);
        setfont(m_fontheight, 0, m_face);
        outtextrect(0, 0, getw(), geth(), m_caption);

        if (m_transparent) {
            setbkcolor_f(BLACK, filter());
            cleardevice(filter());
            if (m_alpha < 0xff) {
                setcolor(RGB(m_alpha, m_alpha, m_alpha), filter());
            } else {
                setcolor(0xFFFFFF, filter());
            }
            setbkmode(TRANSPARENT, filter());
            setfont(m_fontheight, 0, m_face, filter());
            outtextrect(0, 0, getw(), geth(), m_caption, filter());
        } else {
            if (m_alpha < 0xff) {
                setbkcolor_f(RGB(m_alpha, m_alpha, m_alpha), filter());
                cleardevice(filter());
            }
        }
    }
protected:
    char m_caption[1024];
    char m_face[32];
    color_t m_color;
    color_t m_bkcolor;
    int m_alpha;
    bool m_transparent;
    int m_fontheight;
};

#endif /* _EGE_LABEL_H_ */
