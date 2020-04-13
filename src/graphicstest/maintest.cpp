#if 0

#include<graphics.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
    double a , b , c , d , r; //定义变量 

    printf("Please input four float number:\n"); //请输入四个数 
    scanf("%lf %lf %lf %lf", &a,&b,&c,&d); //输入 
    //sscanf(NULL, NULL);

    r = ( (a + b ) * (a - b) + c ) / d ; //计算过程 

    printf("The result is %lf",r); //输出结果 
    system("pause");
    return 0;

} 

#else

#include <graphics.h>
#include <ege/fps.h>
#include <ege/sys_edit.h>
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
    edit.visable(true);
    for ( ; kbhit() != -1; delay_fps(60)) {
        //f.zorderup();
        {
            char str[20];
            sprintf(str, "%.2f", getfps());
            edit.settext(str);
        }
        cleardevice();
    }
    return 0;
}

#endif
/*
#include "graphics.h"

#include <cmath>
#include <ctime>
#include <cstdio>

#define NET_W  256
#define NET_H  256

int g_width = 800;
int g_height = 600;
int g_mod_show = 3;

double g_d_friction = 0.001;
double g_d_min_a = 0.001;
double g_k = 0.03;

struct vector_t {
    double dx;
    double dy;
};

struct point_t {
    double x;
    double y;
    double vx;
    double vy;
    double ax;
    double ay;
    struct vector_t vt[2];
};

struct net {
    struct point_t pt[2][NET_H][NET_W];
    int layer;
    int w;
    int h;
    double dtw;
    double dth;
    double dmw;
    double dmh;
};

double s_sqrt(double d) {
    if (d<0) {
        return -sqrt(-d);
    }
    return sqrt(d);
}

double s_pow(double a, double p) {
    if (fabs(a)<1) {
        return 0;
    }else if (a<0) {
        return -pow(-a, p);
    }
    return pow(a, p);
}

double s_minus(double a, double b) {
    if (b>=0) {
        if (a >= b) {
            return a - b;
        } else if (a <= -b) {
            return a + b;
        } else {
            return 0;
        }
    } else {
        if (a>=0) {
            return a - b;
        } else {
            return a + b;
        }
    }
}

struct vector_t get_power (struct point_t *p, double x, double y) {
    struct vector_t vt;
    vt.dx = x - p->x;
    vt.dy = y - p->y;
    //vt.dx = s_minus(vt.dx, minx);
    //vt.dy = s_minus(vt.dy, miny);
    vt.dx *= g_k;
    vt.dy *= g_k;
    //vt.dx *= fabs(vt.dx);
    //vt.dy *= fabs(vt.dy);
    return vt;
}

void move_point(struct net* pnet, int x, int y) {
    double ax = 0;
    double ay = 0;
    struct vector_t vt;
    int i;
    int l = pnet->layer;
    int dxy[8][2] = {
        { 1, 0},
        { 0, 1},
        {-1, 0},
        { 0,-1},
    };
    struct point_t *op = &(pnet->pt[l][y][x]);

    pnet->pt[l^1][y][x] = *op;
    for (i=0; i<2; ++i) {
        struct point_t *p = &pnet->pt[l][y+dxy[i][1]][x+dxy[i][0]];
        vt = get_power( op, p->x - dxy[i][0]*pnet->dtw, p->y - dxy[i][1]*pnet->dth);
        //vt = get_power( op, p->x, p->y, pnet->dtw, pnet->dth);
        p->vt[i] = vt;
        ax += vt.dx;
        ay += vt.dy;
    }
    ax -= op->vt[0].dx;
    ay -= op->vt[0].dy;
    ax -= op->vt[1].dx;
    ay -= op->vt[1].dy;

    op = &(pnet->pt[l^1][y][x]);
    op->ax = ax;
    op->ay = ay;

    op->vx += op->ax;
    op->vy += op->ay;

    op->x += op->vx;
    op->y += op->vy;

    op->vx *= 1 - g_d_friction;
    op->vy *= 1 - g_d_friction;
    //op->vx = s_minus(op->vx, g_d_friction);
    //op->vy = s_minus(op->vy, g_d_friction);
}

void move_net(struct net* pnet) {
    int x, y = 0, l = pnet->layer, i = 1;
    struct vector_t vt;
    for (x=1; x<=pnet->w; ++x) {
        struct point_t *p = &pnet->pt[l][y+1][x+0];
        struct point_t *op = &(pnet->pt[l][y][x]);
        vt = get_power( op, p->x, p->y - pnet->dth);
        //vt = get_power( op, p->x, p->y, pnet->dtw, pnet->dth);
        p->vt[i] = vt;
    }
    i = 0;
    for (y=1; y<=pnet->h; ++y) {
        x = 0;
        {
            struct point_t *p = &pnet->pt[l][y][x+1];
            struct point_t *op = &(pnet->pt[l][y][x]);
            vt = get_power( op, p->x - pnet->dtw, p->y);
            //vt = get_power( op, p->x, p->y, pnet->dtw, pnet->dth);
            p->vt[i] = vt;
            for (x=1; x<=pnet->w; ++x) {
                move_point(pnet, x, y);
            }
        }
    }
    pnet->layer ^= 1;
}

void init_net(struct net* pnet, int w, int h, int sw, int sh) {
    double dtw = sw / (double)(w - 1);
    double dth = sh / (double)(h - 1);
    int x, y, l;
    struct point_t pt = {0};

    pnet->layer = 0;
    pnet->w = w;
    pnet->h = h;
    pnet->dtw = dtw;
    pnet->dth = dth;

    for (y=0; y<=h+1; ++y) {
        for (x=0; x<=w+1; ++x) {
            for (l=0; l<2; ++l) {
                pnet->pt[l][y][x] = pt;
                pnet->pt[l][y][x].x = (dtw*(x-1));
                pnet->pt[l][y][x].y = (dth*(y-1));
            }
        }
    }
}

void draw_net(struct net* pnet) {
    int x, y, l = pnet->layer;
    POINT pt[NET_W + NET_H];

    for (y=0; y<=pnet->h; ++y) {
        //if (y % g_mod_show == 0)
        {
            for (x=0; x<=pnet->w + 1; ++x) {
                pt[x].x = (int)(pnet->pt[l][y][x].x + 0.5);
                pt[x].y = (int)(pnet->pt[l][y][x].y + 0.5);
                //line_f(pnet->pt[l][y][x].x, pnet->pt[l][y][x].y, pnet->pt[l][y][x+1].x, pnet->pt[l][y][x+1].y);
            }
            drawbezier(pnet->w + 2, (int*)pt);
        }
    }
    for (x=0; x<=pnet->w; ++x) {
        //if (x % g_mod_show == 0)
        {
            for (y=0; y<=pnet->h + 1; ++y) {
                pt[y].x = (int)(pnet->pt[l][y][x].x + 0.5);
                pt[y].y = (int)(pnet->pt[l][y][x].y + 0.5);
                //line_f(pnet->pt[l][y][x].x, pnet->pt[l][y][x].y, pnet->pt[l][y+1][x].x, pnet->pt[l][y+1][x].y);
            }
            drawbezier(pnet->h + 2, (int*)pt);
        }
    }
}

void cap_pt(struct net* pnet, int px, int py, int op) {
    static int cp_x, cp_y, b_cp = 0;

    if (op) {
        int y, x, l=pnet->layer;
        if (b_cp == 0) {
            int mx = 1, my = 1;
            double mdis = 1e9, dis;
            for (y=1; y<pnet->h; ++y) {
                if (y % g_mod_show == 0)
                    continue;
                for (x=1; x<pnet->w; ++x) {
                    if (x % g_mod_show == 0)
                        continue;
                    dis = fabs(px - pnet->pt[l][y][x].x) + fabs(py - pnet->pt[l][y][x].y);
                    if (dis < mdis) {
                        mx = x;
                        my = y;
                        mdis = dis;
                    }
                }
            }
            cp_x = mx;
            cp_y = my;
            pnet->pt[l][cp_y][cp_x].x = px;
            pnet->pt[l][cp_y][cp_x].y = py;
            b_cp = 1;
        } else {
            pnet->pt[l][cp_y][cp_x].x = px;
            pnet->pt[l][cp_y][cp_x].y = py;
        }
        pnet->pt[l][cp_y][cp_x].vx = 0;
        pnet->pt[l][cp_y][cp_x].vy = 0;
    } else {
        b_cp = 0;
    }
}

void init() {
    int g = TRUECOLORSIZE, m = (g_height<<16) | g_width;
    //initgraph(&g, &m, "碧波荡漾");
    //setinitmode(3);
    initgraph(640, 480);
    g_width  = getwidth();
    g_height = getheight();
}

void getmouse(int *x, int *y, int *key) {
    GetMousePos(x, y);
    *key = keystate(VK_LBUTTON);
}

struct net g_net;

int main(int argc, char* argv[]) {
    char str[100] = "";
    int basepoint = 20;

    init();
    init_net(&g_net, basepoint * 4, basepoint * 3, g_width, g_height);
    setbkmode(TRANSPARENT);
    setfont(12, 0, "宋体");

    WindowLock();
    for (; kbhit() == 0; delay_fps(60)) {
        move_net(&g_net);
        move_net(&g_net);
        {
            int x, y, k;
            getmouse(&x, &y, &k);
            if (k) {
                cap_pt(&g_net, x, y, 1);
            } else {
                cap_pt(&g_net, x, y, 0);
            }
        }

        cleardevice();
        setcolor(0x8000);
        draw_net(&g_net);

        setcolor(0xFFFFFF);
        sprintf(str, "帧率 %.2f fps", GetFPS());
        outtextxy(0,0,str);
    }
    closegraph();
    return 0;
}
// */
