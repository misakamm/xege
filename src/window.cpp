#include "ege_head.h"
#include "ege_common.h"

#include "window.h"

#define STYLE_NORMAL  (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN)

namespace ege
{

// -----------------------------------------------------------------------------

static HWND    g_attach_hwnd     = 0;
// -----------------------------------------------------------------------------

void setcaption(const char* caption)
{
    const std::wstring& new_caption = mb2w(caption);
    setcaption(new_caption.c_str());
}

void setcaption(const wchar_t* caption)
{
    struct _graph_setting* pg = &graph_setting;
    if (pg->has_init) {
        ::SetWindowTextW(getHWnd(), caption);
        ::UpdateWindow(getHWnd()); // for vc6
    }

    pg->window_caption = caption;
}

void seticon(int icon_id)
{
    struct _graph_setting* pg = &graph_setting;
    HICON hIcon = NULL;
    HINSTANCE instance = GetModuleHandle(NULL);

    if (icon_id == 0) {
        hIcon = ::LoadIconW(NULL, (LPCWSTR)IDI_APPLICATION);
    } else {
        hIcon = ::LoadIconW(instance, MAKEINTRESOURCEW(icon_id));
    }
    if (hIcon) {
        pg->window_hicon = hIcon;
        if (pg->has_init) {
#ifdef _WIN64
            ::SetClassLongPtrW(getHWnd(), GCLP_HICON, (LONG_PTR)hIcon);
#else
            ::SetClassLongW(getHWnd(), GCL_HICON, (LONG)hIcon);
#endif
        }
    }
}

void showwindow()
{
    struct _graph_setting* pg = &graph_setting;

    bool showLogo = false;

    PIMAGE background = NULL;
    color_t bkColor = getbkcolor();

    if (pg->first_show) {
        pg->first_show = false;

        int initmode = getinitmode();
        if ((initmode & INIT_WITHLOGO) && (initmode & INIT_HIDE)) {
            showLogo = true;
        }
    }

    if (showLogo) {
        background = newimage();
        getimage(background, 0, 0, getwidth(), getheight());
        setbkcolor_f(EGERGB(0, 0, 0));
        cleardevice();
    }

    ShowWindow(pg->hwnd, SW_SHOWNORMAL);
    BringWindowToTop(pg->hwnd);
    SetForegroundWindow(pg->hwnd);

    if (showLogo) {
        bool isRenderManual = pg->lock_window;

        logoscene();

        setbkcolor_f(bkColor);

        if (background != NULL) {
            putimage(0, 0, background);
            flushwindow();
            delimage(background);
        }

        if (!isRenderManual) {
            setrendermode(RENDER_AUTO);
        }
    }
}

void hidewindow()
{
    struct _graph_setting* pg = &graph_setting;
    ShowWindow(pg->hwnd, SW_HIDE);
}

void movewindow(int x, int y, bool redraw)
{
    ::MoveWindow(getHWnd(), x, y, getwidth(), getheight(), redraw);
}

void flushwindow()
{
    if (!isinitialized())
        return;
    struct _graph_setting* pg = &graph_setting;
    pg->skip_timer_mark = true;
    swapbuffers();
    pg->skip_timer_mark = false;
}

HWND getParentWindow()
{
    return g_attach_hwnd;
}

void getParentSize(int* width, int* height)
{
    RECT rect;
    if (g_attach_hwnd) {
        GetClientRect(g_attach_hwnd, &rect);
    } else {
        GetWindowRect(GetDesktopWindow(), &rect);
    }

    *width  = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

void EGEAPI resizewindow(int width, int height)
{
    int parentW, parentH;
    getParentSize(&parentW, &parentH);

    if (width <= 0) {
        width = parentW;
    }
    if (height <= 0) {
        height = parentH;
    }

    if ((width == getwidth() && height == getheight())) {
        return;
    }

    setmode(TRUECOLORSIZE, width | (height << 16));
    _graph_setting* pg = &graph_setting;

    for (int i = 0; i < BITMAP_PAGE_SIZE; ++i) {
        if (pg->img_page[i] != NULL) {
            resize(pg->img_page[i], width, height);
        }
    }

    /* 修改窗口宽高参数 */
    pg->base_w = width;
    pg->base_h = height;
}

int attachHWND(HWND hWnd)
{
    g_attach_hwnd = hWnd;
    return 0;
}

HWND createWindow(HWND parentWindow, const wchar_t* caption, DWORD style, DWORD exstyle, POINT pos, SIZE size)
{
    HWND window = NULL;
    window = CreateWindowExW(exstyle, EGE_WNDCLSNAME_W, caption, style & ~WS_VISIBLE,
            pos.x, pos.y, size.cx,size.cy, parentWindow, NULL, getHInstance(), NULL);

    return window;
}

ATOM register_classW(struct _graph_setting* pg, HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};

    wcex.cbSize = sizeof(wcex);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = (WNDPROC)getProcfunc();
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = pg->window_hicon;
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = EGE_WNDCLSNAME_W;

    return RegisterClassExW(&wcex);
}

} // namespace ege
