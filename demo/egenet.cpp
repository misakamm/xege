//碧波荡漾（鼠标拖动弹力物理模拟演示）
#include <graphics.h>
#include <ege/fps.h>

#include <math.h>
#include <time.h>
#include <stdio.h>

#define NET_W  256
#define NET_H  256

int g_width = 800;
int g_height = 600;
int g_mod_show = 3;

double g_d_friction = 0.001;
double g_d_min_a = 0.001;
double g_k = 0.03; /* 劲度系数，不能大于等于0.5 */

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

struct vector_t get_power (struct point_t *p, double x, double y) { /*, double minx=0, double miny=0*/
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
    mousepos(x, y);
    *key = keystate(key_mouse_l);
}

struct net g_net;

int main(int argc, char* argv[]) {
    char str[100] = "";
    int basepoint = 20;
    /*
    if (argc < 2) return 0;
    if (stricmp(argv[1], "/p") == 0)
    {
        HWND hwnd;
        sscanf(argv[2], "%d", &hwnd);
        attachHWND(hwnd);
    }
    else if (stricmp(argv[1], "/s")) return 0;//*/

    init();
    init_net(&g_net, basepoint * 4, basepoint * 3, g_width, g_height);
    setbkmode(TRANSPARENT);

    fps fps_obj;
    setrendermode(RENDER_MANUAL);
    for (; kbhit() == 0; delay_fps(60)) {
        cleardevice();
        setcolor(0x8000);
        draw_net(&g_net);

        setcolor(0xFFFFFF);
        outtextxy(0,0,str);

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
    }
    closegraph();
    return 0;
}

