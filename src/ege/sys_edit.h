#ifndef EGE_SYS_EDIT_H
#define EGE_SYS_EDIT_H

#ifndef EGE_H
#error include "sys_edit.h" must after include "ege.h" or "graphics.h"
#endif

#include "egecontrolbase.h"

namespace ege {

class sys_edit : public egeControlBase
{
public:
    CTL_PREINIT(sys_edit, egeControlBase) {
        // do sth. before sub objects' construct function call
    } CTL_PREINITEND;
    sys_edit(CTL_DEFPARAM) : CTL_INITBASE(egeControlBase) {
        CTL_INIT; // must be the first linef
        directdraw(true);
        m_hwnd = NULL;
    }
    ~sys_edit() {
        destroy();
    }
    int create(bool multiline = false, int scrollbar = 2) {
        if (m_hwnd) {
            destroy();
        }
        msg_createwindow msg = {NULL};
        msg.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        msg.classname = L"EDIT";
        msg.id = egeControlBase::allocId();
        msg.style = WS_CHILD | WS_BORDER |
                    ES_LEFT | ES_WANTRETURN;
        if (multiline) {
            msg.style |= ES_MULTILINE | WS_VSCROLL;
        } else {
            msg.style |= ES_AUTOHSCROLL;
        }
        msg.exstyle = WS_EX_CLIENTEDGE;// | WS_EX_STATICEDGE;
        msg.param = this;

        ::PostMessageW(getHWnd(), WM_USER + 1, 1, (LPARAM)&msg);
        ::WaitForSingleObject(msg.hEvent, INFINITE);

        m_hwnd = msg.hwnd;
        m_hFont     = NULL;
        m_hBrush    = NULL;
        m_color     = 0x0;
        m_bgcolor   = 0xFFFFFF;

        ::SetWindowLongPtrW(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
        m_callback = ::GetWindowLongPtrW(m_hwnd, GWLP_WNDPROC);
        ::SetWindowLongPtrW(m_hwnd, GWLP_WNDPROC, (LONG_PTR)getProcfunc());
        {
            char fontname[] = {'\xcb', '\xce', '\xcc', '\xe5', 0, 0};
            setfont(12, 6, fontname);
        }
        visible(false);

        ::CloseHandle(msg.hEvent);

        return 0;
    }
    int destroy() {
        if (m_hwnd) {
            visible(false);          	
            msg_createwindow msg = {NULL};
            msg.hwnd = m_hwnd;
            msg.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
            ::SendMessage(m_hwnd, WM_SETFONT, 0, 0);
            ::DeleteObject(m_hFont);
            ::PostMessageW(getHWnd(), WM_USER + 1, 0, (LPARAM)&msg);
            ::WaitForSingleObject(msg.hEvent, INFINITE);
            ::CloseHandle(msg.hEvent);
            if (m_hBrush) ::DeleteObject(m_hBrush);
            m_hwnd = NULL;
            return 1;
        }
        return 0;
    }
    LRESULT onMessage(UINT message, WPARAM wParam, LPARAM lParam);
    void visible(bool bvisible) {
        egeControlBase::visible(bvisible);
        ::ShowWindow(m_hwnd, (int)bvisible);
    }
    void setfont(int h, int w, LPCSTR fontface) {
        {
            LOGFONTA lf = {0};
            lf.lfHeight         = h;
            lf.lfWidth          = w;
            lf.lfEscapement     = 0;
            lf.lfOrientation    = 0;
            lf.lfWeight         = FW_DONTCARE;
            lf.lfItalic         = 0;
            lf.lfUnderline      = 0;
            lf.lfStrikeOut      = 0;
            lf.lfCharSet        = DEFAULT_CHARSET;
            lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
            lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
            lf.lfQuality        = DEFAULT_QUALITY;
            lf.lfPitchAndFamily = DEFAULT_PITCH;
            lstrcpyA(lf.lfFaceName, fontface);
            HFONT hFont = CreateFontIndirectA(&lf);
            if (hFont) {
                ::SendMessageA(m_hwnd, WM_SETFONT, (WPARAM)hFont, 0);
                ::DeleteObject(m_hFont);
                m_hFont = hFont;
            }
        }
    }
    void setfont(int h, int w, LPCWSTR fontface) {
        {
            LOGFONTW lf = {0};
            lf.lfHeight         = h;
            lf.lfWidth          = w;
            lf.lfEscapement     = 0;
            lf.lfOrientation    = 0;
            lf.lfWeight         = FW_DONTCARE;
            lf.lfItalic         = 0;
            lf.lfUnderline      = 0;
            lf.lfStrikeOut      = 0;
            lf.lfCharSet        = DEFAULT_CHARSET;
            lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
            lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
            lf.lfQuality        = DEFAULT_QUALITY;
            lf.lfPitchAndFamily = DEFAULT_PITCH;
            lstrcpyW(lf.lfFaceName, fontface);
            HFONT hFont = CreateFontIndirectW(&lf);
            if (hFont) {
                ::SendMessageW(m_hwnd, WM_SETFONT, (WPARAM)hFont, 0);
                ::DeleteObject(m_hFont);
                m_hFont = hFont;
            }
        }
    }
    void move(int x, int y) {
        egeControlBase::move(x, y);
        ::MoveWindow(m_hwnd, m_x, m_y, m_w, m_h, TRUE);
    }
    void size(int w, int h) {
        egeControlBase::size(w, h);
        ::MoveWindow(m_hwnd, m_x, m_y, m_w, m_h, TRUE);
    }
    void settext(LPCSTR text) {
        ::SendMessageA(m_hwnd, WM_SETTEXT, 0, (LPARAM)text);
    }
    void settext(LPCWSTR text) {
        ::SendMessageW(m_hwnd, WM_SETTEXT, 0, (LPARAM)text);
    }
    void gettext(int maxlen, LPSTR text) {
        ::SendMessageA(m_hwnd, WM_GETTEXT, (WPARAM)maxlen, (LPARAM)text);
    }
    void gettext(int maxlen, LPWSTR text) {
        ::SendMessageW(m_hwnd, WM_GETTEXT, (WPARAM)maxlen, (LPARAM)text);
    }
    void setmaxlen(int maxlen) {
        ::SendMessageW(m_hwnd, EM_LIMITTEXT, (WPARAM)maxlen, 0);
    }
    void setcolor(color_t color) {
        m_color = color;
        ::InvalidateRect(m_hwnd, NULL, TRUE);
    }
    void setbgcolor(color_t bgcolor) {
        m_bgcolor = bgcolor;
        //::RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE);
        ::InvalidateRect(m_hwnd, NULL, TRUE);
    }
    void setreadonly(bool readonly) {
        ::SendMessageW(m_hwnd, EM_SETREADONLY, (WPARAM)readonly, 0);
        ::InvalidateRect(m_hwnd, NULL, TRUE);
    }
    void setfocus() {
        msg_createwindow msg = {NULL};
        msg.hwnd = m_hwnd;
        msg.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        ::PostMessageW(getHWnd(), WM_USER + 2, 0, (LPARAM)&msg);
        ::WaitForSingleObject(msg.hEvent, INFINITE);
    }
protected:
    HWND        m_hwnd;
    HFONT       m_hFont;
    HBRUSH      m_hBrush;
    color_t     m_color;
    color_t     m_bgcolor;
    LONG_PTR    m_callback;
};

} // namespace ege
#endif /*EGE_SYS_EDIT_H*/
