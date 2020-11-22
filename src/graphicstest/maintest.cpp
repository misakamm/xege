#include <graphics.h>
#include "ege/egecontrolbase.h"
#include "ege/fps.h"
#include "ege/sys_edit.h"
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>


class graph : public egeControlBase
{
public:
    CTL_PREINIT(graph, egeControlBase) {
        int w = 40;
        size(w * 2, w * 2);
        //enable(false);
    } CTL_PREINITEND;
    graph(CTL_DEFPARAM) : CTL_INITBASE(egeControlBase) {
        CTL_INIT;
        // 初始化坐标，
        x = random(parent()->getw()), y = random(parent()->geth());
        // 开启透明半透明混合
        blendmode(1);

        {
            int w = geth() / 2;
            color_t col = ege::hsv2rgb((float)random(360), 1.0f, 1.0f);
            setbkcolor_f(col);
            cleardevice();
            settarget(filter());
            col = EGEGRAYA(0xFF, 0x7F);
            ege_enable_aa(true);
            setcolor(col);
            setfillcolor(col);
            ege_fillellipse(1.0f, 1.0f, (float)(getw()-2), (float)(geth()-2));
        }
        dx = 0.3 + randomf() * 2, dy = 0.3 + randomf() * 2;
    }
    int onUpdate() {
        if (dx < 0 && x < 0) dx = -dx;
        if (dx > 0 && x + getw() > parent()->getw()) dx = -dx;
        if (dy < 0 && y < 0) dy = -dy;
        if (dy > 0 && y + geth() > parent()->geth()) dy = -dy;
        x += dx, y += dy;
        move((int)x, (int)y);
        return 0;
    }
    void onDraw(PIMAGE pimg) const {
        ;
    }
private:
    double dx, dy;
    double x, y;
};

class Window : public egeControlBase
{
public:
    CTL_PREINIT(Window, egeControlBase) {
        size(400, 300);
    } CTL_PREINITEND;
    Window(CTL_DEFPARAM) : CTL_INITBASE(egeControlBase) {
        CTL_INIT;
        blendmode(1);
        m_col = HSVtoRGB(float(randomf() * 360), 1.0f, 0.2f);
        m_capmouse = 0;
    }
    int onMouse(int x, int y, int flag) {
        if (m_capmouse == 0 && (flag & mouse_msg_down)) {
            if (flag & mouse_flag_left) {
                capmouse(true);
                m_capmouse = 1;
                m_capx = x;
                m_capy = y;
            }
        } else if ((flag & mouse_msg_up)) {
            capmouse(false);
            if (m_capmouse && (flag & mouse_flag_left)) {
                m_capmouse = 0;
            }
        } else if (m_capmouse) {
            move(x - m_capx + getx(), y - m_capy + gety());
        }
        return 0;
    }
    void onDraw(PIMAGE pimg) const {
        setbkcolor_f(m_col, buf());
        cleardevice(buf());
        setbkcolor_f(0xBF, filter());
        cleardevice(filter());
    }
private:
    int m_capmouse;
    int m_capx, m_capy;
    graph m_graph[4];
    color_t m_col;
};

// 最简单的继承示例
class Window2 : public Window
{
public:
    CTL_PREINIT(Window2, Window) {
        int a = 0;
    } CTL_PREINITEND;
    Window2(CTL_DEFPARAM) : CTL_INITBASE(Window) {
        CTL_INIT;
        static int s_x = 0;
        move(s_x, s_x);
        s_x = (s_x + 16) % 300;
    }
private:
    //graph m_graph[4];
};

int main()
{
    initgraph(800, 600);
    setrendermode(RENDER_MANUAL);
    randomize();

    Window2 w[3];
    //fps f;
    sys_edit edit;
    scanf("%*s");
    edit.create();
    edit.size(100, 18);
    edit.visible(true);
    for ( ; kbhit() != -1; delay_fps(60)) {
        //f.zorderup();
        {
            char str[20];
            sprintf(str, "%.2f", getfps());
            edit.settext(str);
        }
        cleardevice();
    }
    closegraph();
    return 0;
}
