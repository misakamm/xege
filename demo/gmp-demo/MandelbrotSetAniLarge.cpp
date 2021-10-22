#include "graphics.h"
//#include <complex>
#include <time.h>
#include <gmpxx.h>

//using namespace std;

#define BASEITERATIONS 1   //首次迭代次数
#define ITERATIONS 256     //增量迭代次数
#define MAXCOLOR 0x20      //颜色数
#define COLORMASK (0x200 - 1)

#define SC_W (640 * 2) //1280
#define SC_H (480 * 2) //960

#define for if(1) for

/////////////////////////////////////////////////
// 定义复数及乘、加运算
/////////////////////////////////////////////////

/*
// 定义复数
template <class TFLOAT>
struct complex
{
    TFLOAT re;
    TFLOAT im;
    complex()
    {
        re = im = 0.0;
    }
    TFLOAT& real() { return re; }
    TFLOAT& imag() { return im; }
};

typedef complex<double> COMPLEX;
//*/

typedef mpf_class Float;
struct COMPLEXI
{
    int re;
    int im;
};
mp_bitcnt_t g_prec = 64;

struct COMPLEX
{
    Float re;
    Float im;
    COMPLEX()
    {
        re = 0.0;
        im = 0.0;
        re.set_prec(g_prec);
        im.set_prec(g_prec);
    }
    void setprec()
    {
        re.set_prec(g_prec);
        im.set_prec(g_prec);
    }
    void setzero()
    {
        re = 0.0;
        im = 0.0;
        re.set_prec(0);
        im.set_prec(0);
    }
    Float& real() { return re; }
    Float& imag() { return im; }
};//*/

// 定义复数“乘”运算
COMPLEX operator * (const COMPLEX& a, const double& b)
{
    COMPLEX c;
    c.re = a.re * b;
    c.im = a.im * b;
    return c;
}

// 定义复数“乘”运算
inline
COMPLEX operator * (const COMPLEX& a, const COMPLEX& b)
{
    COMPLEX c;
    c.re = a.re * b.re - a.im * b.im;
    c.im = a.im * b.re + a.re * b.im;
    return c;
}

// 定义复数“乘”运算
inline
COMPLEX& operator *= (COMPLEX& a, const COMPLEX& b)
{
    Float c = a.im * b.re + a.re * b.im;
    a.re = a.re * b.re - a.im * b.im;
    a.im = c;
    return a;
}

// 定义复数“加”运算
inline
COMPLEX& operator += (COMPLEX& a, const COMPLEX& b)
{
    a.re += b.re;
    a.im += b.im;
    return a;
}

// 定义复数“除”运算
COMPLEX operator / (const COMPLEX& a, const double& b)
{
    COMPLEX c = a;
    c.re /= b;
    c.im /= b;
    return c;
}

// 定义复数“加”运算
COMPLEX operator + (const COMPLEX& a, const double& b)
{
    COMPLEX c;
    c.re = a.re + b;
    c.im = a.im;
    return c;
}

// 定义复数“加”运算
inline
COMPLEX operator + (const COMPLEX& a, const COMPLEX& b)
{
    COMPLEX c;
    c.re = a.re + b.re;
    c.im = a.im + b.im;
    return c;
}

inline
bool abs4(const COMPLEX& c)
{
    //Float limit = 16.0, mlimit = -limit;
    //if (c.re > limit || c.re < mlimit || c.im > limit || c.im < mlimit) return true;
    double d1 = mpf_get_d(c.re.get_mpf_t()), d2 = mpf_get_d(c.im.get_mpf_t());
    return d1*d1 + d2*d2 > 4.0;
    //if (c.re * c.re + c.im * c.im > 16.0) return true;
    //return false;
}

//typedef complex<double> COMPLEX;

typedef struct PIXEL
{
    COMPLEX* last;
    COMPLEX* last2;
    COMPLEX* c;
    int nIter;
    int nMinIter;
    int nLastIter;
    int ed;
    int calc;
    PIXEL()
    {
        last2 = last = c = NULL;
    }
}PIXEL;

#define CPPOOLSIZE (SC_W * 64)
struct COMPLEXPOOL
{
    COMPLEX z;
    int use;
};
COMPLEXPOOL* g_cpnum_pool = new COMPLEXPOOL[CPPOOLSIZE];
int g_cp_pool_index;

COMPLEX* new_cp()
{
    for (int i = 0; i < CPPOOLSIZE; ++i)
    {
        if (g_cpnum_pool[g_cp_pool_index].use == 0)
        {
            g_cpnum_pool[g_cp_pool_index].use = 1;
            COMPLEX* ret = &g_cpnum_pool[g_cp_pool_index].z;
            g_cp_pool_index = (g_cp_pool_index + 1) % CPPOOLSIZE;
            return ret;
        }
        g_cp_pool_index ++;
        if (g_cp_pool_index >= CPPOOLSIZE) g_cp_pool_index -= CPPOOLSIZE;
    }
    return NULL;
}

void delete_cp(COMPLEX** p)
{
    if (p && *p)
    {
        int index = (COMPLEXPOOL*)*p - g_cpnum_pool;
        g_cpnum_pool[index].use = 0;
        *p = NULL;
        if (g_cpnum_pool[g_cp_pool_index].use)
        {
            g_cp_pool_index = index;
        }
    }
}

PIXEL (*pMap)[SC_W] = new PIXEL[SC_H][SC_W];

struct updatelist
{
    POINT *p, *pn;
    POINT m_list[2][SC_H*SC_W];
    int nBeg, nLen;
    int nLen_n;
    updatelist()
    {
        clear();
    }
    void clear()
    {
        p = m_list[0];
        pn = m_list[1];
        nBeg = nLen = nLen_n = 0;
    }
    void push(int x, int y)
    {
        pn[nLen_n].x = x;
        pn[nLen_n].y = y;
        ++nLen_n;
    }
    int pop(int *x, int *y)
    {
        if (nBeg == nLen) return 0;
        *x = p[nBeg].x;
        *y = p[nBeg].y;
        ++nBeg;
        return 1;
    }
    void swap()
    {
        nBeg = 0;
        nLen = nLen_n;
        nLen_n = 0;
        POINT* _p = p;
        p = pn;
        pn = _p;
    }
}*g_pudlist = new updatelist;
updatelist& g_udlist = *g_pudlist;

/////////////////////////////////////////////////
// 定义颜色及初始化颜色
/////////////////////////////////////////////////

// 定义颜色
int Color[COLORMASK+1];

void fixcolor(int* color)
{
    return ;
    int r = *color & 0xFF;
    int g = (*color>>8) & 0xFF;
    int b = (*color>>16) & 0xFF;
    double fr,fg,fb;
    fr = r / 255.0;
    fg = g / 255.0;
    fb = b / 255.0;
    fr *= fr;
    fg *= fg;
    fb *= fb;
    r = (int)(fr * 255);
    g = (int)(fg * 255);
    b = (int)(fb * 255);
    *color = EGERGB(r, g, b);
}

void setinitcolor(int* color, int len, int h1, int h2, float s = 0.8f)
{
    int i;
    for (i=0; i<len/2; i++)
    {
        color[i]       = HSLtoRGB((float)h1, s, i*2.0f/len*0.8f + 0.1f);
        fixcolor(&color[i]);
        color[len-1-i] = HSLtoRGB((float)h2, s, i*2.0f/len*0.8f + 0.1f);
        fixcolor(&color[len- 1 - i]);
    }
}
// 初始化颜色
void InitColor()
{
    // 使用 HSL 颜色模式产生角度 h1 到 h2 的渐变色
    int h1, h2 = 0;
    h1 = 240; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*0, MAXCOLOR, h1, h2);
    h1 = 300; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*1, MAXCOLOR, h1, h2);
    h1 = 0; h2 = h1 + 300;
    setinitcolor(Color+MAXCOLOR*2, MAXCOLOR, h1, h2);
    h1 = 60; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*3, MAXCOLOR, h1, h2);
    h1 = 120; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*4, MAXCOLOR, h1, h2);
    h1 = 180; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*5, MAXCOLOR, h1, h2);
    h1 = 270; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*6, MAXCOLOR, h1, h2);
    h1 = 330; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*7, MAXCOLOR, h1, h2);
    h1 = 30; h2 = h1 + 300;
    setinitcolor(Color+MAXCOLOR*8, MAXCOLOR, h1, h2);
    h1 = 90; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*9, MAXCOLOR, h1, h2);
    h1 = 150; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*10, MAXCOLOR, h1, h2);
    h1 = 210; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*11, MAXCOLOR, h1, h2);
    h1 = 270; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*12, MAXCOLOR, h1, h2);
    h1 = 45; h2 = h1 + 300;
    setinitcolor(Color+MAXCOLOR*13, MAXCOLOR, h1, h2);
    h1 = 225; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*14, MAXCOLOR, h1, h2);
    h1 = 315; h2 = h1 - 60;
    setinitcolor(Color+MAXCOLOR*15, MAXCOLOR, h1, h2);
    /*
    h1 = 240, h2 = 60;
    setinitcolor(Color, MAXCOLOR, h1, h2);
    h1 = 270, h2 = 90;
    setinitcolor(Color+MAXCOLOR, MAXCOLOR, h1, h2);
    h1 = 300, h2 = 120;
    setinitcolor(Color+MAXCOLOR*2, MAXCOLOR, h1, h2);
    h1 = 330, h2 = 150;
    setinitcolor(Color+MAXCOLOR*3, MAXCOLOR, h1, h2);
    h1 = 0, h2 = 180;
    setinitcolor(Color+MAXCOLOR*4, MAXCOLOR, h1, h2);
    h1 = 30, h2 = 210;
    setinitcolor(Color+MAXCOLOR*5, MAXCOLOR, h1, h2);
    h1 = 60, h2 = 240;
    setinitcolor(Color+MAXCOLOR*6, MAXCOLOR, h1, h2);
    h1 = 90, h2 = 270;
    setinitcolor(Color+MAXCOLOR*7, MAXCOLOR, h1, h2);
    */
}

//#define func(z, c, ed) {z*=z; if (z.re > 4) {ed = 1; break;} z+=c;} //(z * z * z * z * z * z + c)
#define func(z, c) z *= z, z += c;

int g_base_iters = BASEITERATIONS;


int g_iters = ITERATIONS;

int g_b_update_mark;
unsigned int g_min_iter_last;
unsigned int g_max_iter;
unsigned int g_max_iter_last;

int g_dx_iters;


inline
int MandelbrotEx(PIXEL& z)
{
    z.calc = 0;
    //*
    if (z.nIter < z.nMinIter)
    {
        int ed = std::min(z.nMinIter, 8), last = z.nMinIter - ed;
        *z.last2 = *z.last;
        for (; ed > 2; ed = std::min(last >> 1, 8), last = last - ed)
        {
            for (int k = 0; k < ed; ++k)
            {
                func(*z.last, *z.c);
            }
            if ( abs4(*z.last) )
            {
                *z.last = *z.last2;
                break;
            }
            else
            {
                z.nIter += ed;
                *z.last2 = *z.last;
            }
        }
        z.nMinIter = 0;
    }
    // */
    int k = g_iters;
    if (z.nIter > (g_max_iter << 1)) return z.nIter;
    if (z.nIter + k < g_max_iter) k = g_max_iter - z.nIter;
    int b = k;
    while (k > 0)
    {
        --k;
        //func(z.last, z.c, z.ed);
        func(*z.last, *z.c);
        if ( abs4(*z.last) )
        {
            z.ed = 1;
            int t = z.nIter + (b - k);
            if (t > g_max_iter_last) g_max_iter_last = t;
            if (t < g_min_iter_last) g_min_iter_last = t;
            delete_cp(&z.last);
            delete_cp(&z.last2);
            delete_cp(&z.c);
            break;
        }
    }
    z.nIter += b - k;
    return z.nIter;
}

/////////////////////////////////////////////////
// 绘制 Mandelbrot Set (曼德布洛特集)
/////////////////////////////////////////////////

void initqueue(int bcross)
{
    for (int y=0; y<SC_H; y++)
    {
        for (int x=0; x<SC_W; x++)
        {
            pMap[y][x].ed = 0;
            pMap[y][x].calc = 0;
            pMap[y][x].nIter = 0;
            pMap[y][x].nMinIter = 0;
            delete_cp(&pMap[y][x].last);
            delete_cp(&pMap[y][x].last2);
            delete_cp(&pMap[y][x].c);
        }
    }
    /*
    int sh = SC_H / 2 - 1, sw = SC_W / 2 - 1;
    for (int y=0; y<SC_H / 2; y += 159)
    {
        for (int x=0; x<SC_W / 2; x += 159)
        {
            pMap[sh + y][sw + x].calc = 1;
            pMap[sh + y][sw - x].calc = 1;
            pMap[sh - y][sw + x].calc = 1;
            pMap[sh - y][sw - x].calc = 1;
        }
    }
    // */
    //*
    for (int y=0; y<SC_H; y++)
    {
        if (pMap[y][0].ed == 0) pMap[y][0].calc = 1;
        if (pMap[y][SC_W-1].ed== 0) pMap[y][SC_W-1].calc = 1;
    }
    for (int x=0; x<SC_W; x++)
    {
        if (pMap[0][x].ed == 0) pMap[0][x].calc = 1;
        if (pMap[SC_H-1][x].ed == 0) pMap[SC_H-1][x].calc = 1;
    }
    if (bcross)
    {
        for (int x=0; x<SC_W; x++)
        {
            if (pMap[x*SC_H/SC_W][x].ed == 0) pMap[x*SC_H/SC_W][x].calc = 1;
            if (pMap[SC_H - x*SC_H/SC_W - 1][x].ed == 0) pMap[SC_H - x*SC_H/SC_W - 1][x].calc = 1;
        }
    }
    // */
    /*
    for (int y=1; y<SC_H-1; y++)
    {
        for (int x=1; x<SC_W-1; x++)
        {
            PIXEL& p = pMap[y][x];
            if (p.ed)
            {
                pMap[y-1][x].calc = 1;
                pMap[y+1][x].calc = 1;
                pMap[y][x-1].calc = 1;
                pMap[y][x+1].calc = 1;
            }
        }
    }// */
    g_udlist.clear();
    for (int y=0; y<SC_H; y++)
    {
        for (int x=0; x<SC_W; x++)
        {
            PIXEL& p = pMap[y][x];
            if (p.calc)
            {
                //p.nMinIter = 0;
                g_udlist.push(x, y);
            }
            /*else if (p.ed == 0 && (random(20000)) == 0)
            {
                g_udlist.push(x, y);
                p.calc = 1;
            }//*/
        }
    }
    /*
    {
        int x = SC_W / 2, y = SC_H / 2;
        PIXEL& p = pMap[y][x];
        if (p.ed == 0)
        {
            g_udlist.push(x, y);
            p.calc = 1;
        }
    }// */
    g_udlist.swap();
}

void addpoint(int x, int y, int it = -1)
{
    if (x < 0 || x >= SC_W || y < 0 || y >= SC_H) return;
    if (pMap[y][x].ed == 0 && pMap[y][x].calc == 0)
    {
        pMap[y][x].calc = 1;
        if (it >= 0)
        {
            if (it > 64) it -= 16;
            pMap[y][x].nMinIter = it;
        }
        g_udlist.push(x, y);
    }
}


//int travel(int x, int y, )

int DrawEx(Float& fromx, Float& fromy, Float& tox, Float& toy, int mode = 0)
{
    int t = clock();
    //if (mode == 0)
    {
        int x, y;
        while (g_udlist.pop(&x, &y))
        {
            PIXEL& p = pMap[y][x];
            if (p.nIter == 0 && p.ed == 0)
            {
                COMPLEX z, c;
                c.re = fromx + (tox - fromx) * (x / (double)SC_W);
                c.im = fromy + (toy - fromy) * (y / (double)SC_H);
                z.re = z.im = 0.0;
                pMap[y][x].last = new_cp();
                pMap[y][x].last2 = new_cp();
                pMap[y][x].c = new_cp();
                pMap[y][x].last->setprec();
                pMap[y][x].last2->setprec();
                pMap[y][x].c->setprec();
                pMap[y][x].last[0] = z;
                pMap[y][x].c[0] = c;
            }
            if (p.ed == 0)
            {
                int k;
                k = MandelbrotEx(p);
                if (p.ed)
                {
                    if (1)
                    {
                        if (x == 0)
                        {
                            //addpoint(x+1, y, k);
                        }
                        else if (pMap[y][x-1].ed && pMap[y][x-1].nIter != k)
                        {
                            addpoint(x  , y-1, k);
                            addpoint(x-1, y-1, k);
                            addpoint(x  , y+1, k);
                            addpoint(x-1, y+1, k);
                        }
                        if (x+1 == SC_W)
                        {
                            //addpoint(x-1, y, k);
                        }
                        else if (pMap[y][x+1].ed && pMap[y][x+1].nIter != k)
                        {
                            addpoint(x  , y-1, k);
                            addpoint(x+1, y-1, k);
                            addpoint(x  , y+1, k);
                            addpoint(x+1, y+1, k);
                        }
                        if (y == 0)
                        {
                            //addpoint(x, y+1, k);
                        }
                        else if (pMap[y-1][x].ed && pMap[y-1][x].nIter != k)
                        {
                            addpoint(x-1, y-1, k);
                            addpoint(x-1, y  , k);
                            addpoint(x+1, y-1, k);
                            addpoint(x+1, y  , k);
                        }
                        if (y+1 == SC_H)
                        {
                            //addpoint(x, y-1, k);
                        }
                        else if (pMap[y+1][x].ed && pMap[y+1][x].nIter != k)
                        {
                            addpoint(x-1, y+1, k);
                            addpoint(x-1, y  , k);
                            addpoint(x+1, y+1, k);
                            addpoint(x+1, y  , k);
                        }
                        {
                            //
                        }
                    }
                    else
                    {
                        addpoint(x, y-1, k);
                        addpoint(x, y+1, k);
                        addpoint(x-1, y, k);
                        addpoint(x+1, y, k);
                    }
                    g_b_update_mark += 1;
                    putpixel_f(x, y, Color[k & COLORMASK]);
                }
                else
                {
                    addpoint(x, y);
                }
                if (clock() - t > 5000)
                {
                    if (keystate('J') && keystate('N')) return 1;
                    delay_ms(0);
                    t = clock();
                }
            }
        }
    }
    //else
    if (0)
    {
        int x, y;
        while (g_udlist.pop(&x, &y))
        {
            PIXEL& p = pMap[y][x];
            if (p.nIter == 0 && p.ed == 0) //err
            {
                COMPLEX z, c;
                c.re = fromx + (tox - fromx) * (x / (double)SC_W);
                c.im = fromy + (toy - fromy) * (y / (double)SC_H);
                z.re = z.im = 0.0;
                pMap[y][x].last = new_cp();
                pMap[y][x].c = new_cp();
                pMap[y][x].last->setprec();
                pMap[y][x].c->setprec();
                pMap[y][x].last[0] = z;
                pMap[y][x].c[0] = c;
            }
            if (p.ed == 0)
            {
                int k = MandelbrotEx(p);
                if (p.ed)
                {
                    addpoint(x, y-1);
                    addpoint(x, y+1);
                    addpoint(x-1, y);
                    addpoint(x+1, y);
                    g_b_update_mark += 1;
                    putpixel_f(x, y, Color[k & COLORMASK]);
                }
                else
                {
                    addpoint(x, y);
                }
                if (clock() - t > 5000)
                {
                    if (keystate('J') && keystate('N')) return 1;
                    delay(0);
                    t = clock();
                }
            }
        }
    }
    g_udlist.swap();
    return 0;
}

int fill_map()
{
    for (int y=1; y<SC_H-1; y++)
    {
        for (int x=1; x<SC_W-1; x++)
        {
            if (pMap[y][x].nIter == 0)
            {
                if (pMap[y - 1][x].ed && pMap[y][x - 1].ed && pMap[y][x - 1].nIter == pMap[y - 1][x].nIter)
                {
                    pMap[y][x].nIter = pMap[y - 1][x].nIter;
                    pMap[y][x].ed = 1;
                    putpixel_f(x, y, Color[pMap[y][x].nIter & COLORMASK]);
                }
            }
        }
    }
}

void setgprec(Float f)
{
    mp_bitcnt_t t = 0;
    while (f < 1)
    {
        f *= 2;
        t += 1;
    }
    t /= 32;
    t += 1;
    t = t + t / 16 + 1;
    t *= 32;
    g_prec = t;
}

#include <stdio.h>
#include <io.h>
/////////////////////////////////////////////////
// 主函数
/////////////////////////////////////////////////
int WinMain()
{
    // 初始化绘图窗口及颜色
    int w = SC_W, h = SC_H, dh = 48;
    setinitmode(0);
    initgraph(w, h + dh, INIT_DEFAULT|INIT_RENDERMANUAL);
    randomize();
    InitColor();
    setfillstyle(0x0, getcolor());
    setfont(12, 0, "宋体");
    SetWindowTextA(getHWnd(), "Mandelbrot Set");
    //SetWindowTextA(getHWnd(), "Mandelbrot Set by 御坂美琴 -- PowerEasyX V0.3.4 Release (20110129)");
    //mpf_set_prec(100);


    // 初始化 Mandelbrot Set(曼德布洛特集)坐标系
    IMAGE& mimage = *newimage();
    COMPLEX center, delta, mindelta;
    COMPLEX from, to;
    COMPLEX js_c;
    int mode = 0;
    int ncnt = 0;
    int nbeg = -1;
    double delta_mul = 0.5; //0.707;
    g_prec = 2048;
    center.setprec();
    mindelta.setprec();
    delta.setprec();
    {
        FILE * fp = fopen("MandelbrotSetAni.ini", "r");
        char str[1024] = {0};
        if (fp)
        {
            fgets(str, 1024, fp);
            center.re = str;
            fgets(str, 1024, fp);
            center.im = str;
            fgets(str, 1024, fp);
            mindelta.re = str;
            fgets(str, 1024, fp);
            //sscanf(str, "%lf", &delta_mul);
            fgets(str, 1024, fp);
            sscanf(str, "%d", &nbeg);
            fclose(fp);
        }
    }
    //mindelta.re = "0.000000000000001";
    delta.re = "2.0";

    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
    //BeginBatchDraw();
    for (; delta.re > mindelta.re; delta.re *= delta_mul, ++ncnt)
    {
        setgprec(delta.re);
        from.setprec();
        to.setprec();
        delta.im = delta.re * 0.75;
        from.re = center.re - delta.re;
        to.re = center.re + delta.re;
        from.im = center.im - delta.im;
        to.im = center.im + delta.im;
        {
            char str[200];
            Float x = (from.re + to.re) * 0.5,
                  y = (from.im + to.im) * 0.5,
                  d = to.re - from.re;
            setcolor(0xFFFFFF);
            gmp_sprintf(str, "%+.210Ff", x.get_mpf_t());
            outtextxy(0, SC_H + 12*0, str);
            gmp_sprintf(str, "%+.210Ff", y.get_mpf_t());
            outtextxy(0, SC_H + 12*1, str);
            gmp_sprintf(str, "%+.210Ff", d.get_mpf_t());
            outtextxy(0, SC_H + 12*2, str);
            gmp_sprintf(str, "%-6d %4d", mpf_get_prec(y.get_mpf_t()), ncnt);;
            outtextxy(0, SC_H + 12*3, str);
            delay(0);
        }
        int calc = 1;
        if (ncnt < nbeg)
        {
            calc = 0;
        }
        else
        {
            char str[30];
            sprintf(str, "snap%06d.bmp", ncnt);
            if (access(str, 0) == 0)
            {
                calc = 0;
            }
            else
            {
                FILE * fp = fopen(str, "w");
                if (fp)
                {
                    fclose(fp);
                }
                else
                {
                    calc = 0;
                }
            }
        }
        if (calc)
        {
            bar(0, 0, SC_W, SC_H);
            //delay(0);
            g_max_iter_last = 16;
            //Draw(from.re, from.im, to.re, to.im, mode);
            int mend = 0xFFFFFFF;
            int minmark = 0;
            //if (g_prec <= 64) minmark = 0, mend = 16;
            int addmark = 0;

            initqueue(g_prec <= 64);
            g_max_iter = 2048;
            if (g_prec <= 64) g_max_iter = 16;
            //g_iters = ITERATIONS;

            int last_min = 0;
            for (int m=0, t = clock(); g_udlist.nLen > 0 && m < mend; ++m)
            {
                g_b_update_mark = 0;
                g_min_iter_last = 0x7FFFFFFF;
                if (clock() - t > 50)
                {
                    t = clock();
                    {
                        char str[100];
                        sprintf(str, "%8d %8d %8d %8d", g_base_iters, g_max_iter, g_max_iter_last, last_min);
                        outtextxy(100, SC_H + 12*3, str);
                    }
                    {
                        RECT rect, crect;
                        int _dw, _dh;
                        GetClientRect(getHWnd(), &crect);
                        GetWindowRect(getHWnd(), &rect);
                        _dw = w - crect.right;
                        _dh = h + dh - crect.bottom;
                        MoveWindow(
                            getHWnd(),
                            rect.left,
                            rect.top,
                            rect.right  + _dw - rect.left,
                            rect.bottom + _dh - rect.top,
                            TRUE);
                    }
                    delay_ms(0);
                }
                if (DrawEx(from.re, from.im, to.re, to.im, mode)) break;
                if (g_b_update_mark + addmark > minmark)
                {
                    if (mend == 0xFFFFFFF)
                    {
                        //g_base_iters += (m + 1) * g_iters;
                        //mend = m * 2 + 1;
                        //if (mend < g_prec) mend = g_prec;
                        mend = 16;
                    }//*/
                    //if (mend < (m + 1) * 2)
                    //    mend = (m + 1) * 2;
                    m = -1;
                    addmark = 0;
                    if (g_min_iter_last < last_min) g_min_iter_last = last_min;
                    if (g_max_iter_last >= g_max_iter) g_max_iter = (g_max_iter_last << 1);
                    if (g_max_iter > (g_min_iter_last << 1)) g_max_iter = (g_min_iter_last << 1);
                    last_min = g_min_iter_last;
                }
                else
                {
                    if (mend != 0xFFFFFFF && g_max_iter > last_min * 2)
                    {
                        break;
                    }
                    if (g_max_iter <= 0x10000)
                    {
                        g_max_iter <<= 1;
                    }
                    else if (g_max_iter < 0x1000000)
                    {
                        g_max_iter += g_max_iter >> 1;
                    }
                    addmark += g_b_update_mark;
                }
            }
            fill_map();
            {
                char str[30];
                getimage(&mimage, 0, 0, SC_W, SC_H);
                sprintf(str, "snap%06d.bmp", ncnt);
                saveimage(&mimage, str);
            }
        }
        delay_ms(0);
    }
    //EndBatchDraw();

    //getch();
    delimage(&mimage);
    closegraph();
    return 0;
}


