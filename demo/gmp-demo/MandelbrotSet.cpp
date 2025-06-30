#include "graphics.h"
//#include <complex>
#include <time.h>
#include <gmpxx.h>
#include <algorithm>

//using namespace std;

#define BASEITERATIONS 1  //首次迭代次数
#define ITERATIONS 16     //增量迭代次数
#define MAXCOLOR 0x40     //颜色数
#define COLORMASK (0x200 - 1)

#define SC_W 160
#define SC_H 120

//#define for if(1) for

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
        re = im = 0.0;
        re.set_prec(g_prec);
        im.set_prec(g_prec);
    }
    void setprec()
    {
        re.set_prec(g_prec);
        im.set_prec(g_prec);
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
double abs(const COMPLEX& c)
{
    double d1 = mpf_get_d(c.re.get_mpf_t()), d2 = mpf_get_d(c.im.get_mpf_t());
    return d1*d1 + d2*d2;
}

//typedef complex<double> COMPLEX;

#define W_B 10
#define WIDTH (1<<W_B)

typedef struct PIXEL
{
    COMPLEX last;
    int nIter;
    int ed;
    int calc;
}PIXEL;

PIXEL (*pMap)[WIDTH] = new PIXEL[SC_H][WIDTH];

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

/*
struct calclist
{
    int m_len;
    int m_list[SC_H*SC_W];
    void clear()
    {
        m_len = 0;
    }
    void push(int x, int y)
    {
        ++m_len;
        m_list[m_len] = y<<W_B + x;
        PIXEL* p = (PIXEL*)pMap;
        int t = m_len;
        for ( ; t>1; t >>= 1)
        {
            if (p[m_list[t]].nIter < p[m_list[t>>1]].nIter)
            {
                std::swap(m_list[t], m_list[t>>1]);
            }
            else break;
        }
    }
    void update(int x, int y)
    {
        int t = y<<W_B + x;
        int t2 = t<<1;
        PIXEL* p = (PIXEL*)pMap;
        for ( ; t2 < m_len; t2 = (t<<1))
        {
            if (p[m_list[t2]].nIter <= p[m_list[t2+1]].nIter)
            {
                if (p[m_list[t]].nIter <= p[m_list[t2]].nIter)
                {
                    return ;
                }
                else
                {
                    std::swap(m_list[t], m_list[t2]);
                    t = t2;
                }
            }
            else
            {
                t2 += 1;
                if (p[m_list[t]].nIter <= p[m_list[t2]].nIter)
                {
                    return ;
                }
                else
                {
                    std::swap(m_list[t], m_list[t2]);
                    t = t2;
                }
            }
        }
        if (t2 <= m_len)
        {
            if (p[m_list[t]].nIter > p[m_list[t2]].nIter)
            {
                std::swap(m_list[t], m_list[t2]);
            }
        }
    }
    int pop(int x, int y)
    {
        if (m_len <= 0) return -1;
        int t = y<<W_B + x;
        std::swap(m_list[t], m_list[m_len]);
        --m_len;
        update(x, y);
        return 0;
    }
};//*/

/////////////////////////////////////////////////
// 定义颜色及初始化颜色
/////////////////////////////////////////////////

// 定义颜色
int Color[COLORMASK+1];

void fixcolor(int* color)
{
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

void setinitcolor(int* color, int len, int h1, int h2)
{
    int i;
    for (i=0; i<len/2; i++)
    {
        color[i] = HSLtoRGB((float)h1, 0.9f, i * 2.0f / len * 0.7f + 0.15f);
        fixcolor(&color[i]);
        color[len-1-i] = HSLtoRGB((float)h1, 0.9f, i*2.0f/len*0.7f + 0.15f);
        fixcolor(&color[len- 1 - i]);
    }
}
// 初始化颜色
void InitColor()
{
    // 使用 HSL 颜色模式产生角度 h1 到 h2 的渐变色
    int h1, h2 = 0;
    h1 = 240;
    setinitcolor(Color+MAXCOLOR*0, MAXCOLOR, h1, h2);
    h1 = 30;
    setinitcolor(Color+MAXCOLOR*1, MAXCOLOR, h1, h2);
    h1 = 330;
    setinitcolor(Color+MAXCOLOR*2, MAXCOLOR, h1, h2);
    h1 = 180;
    setinitcolor(Color+MAXCOLOR*3, MAXCOLOR, h1, h2);
    h1 = 270;
    setinitcolor(Color+MAXCOLOR*4, MAXCOLOR, h1, h2);
    h1 = 0;
    setinitcolor(Color+MAXCOLOR*5, MAXCOLOR, h1, h2);
    h1 = 300;
    setinitcolor(Color+MAXCOLOR*6, MAXCOLOR, h1, h2);
    h1 = 150;
    setinitcolor(Color+MAXCOLOR*7, MAXCOLOR, h1, h2);
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

inline
int Mandelbrot(COMPLEX z, COMPLEX c, int x, int y)
{
    int k = BASEITERATIONS;
    pMap[y][x].ed = 0;
    while (k > 0)
    {
        //func(z, c,  pMap[y][x].ed);
        --k;
        func(z, c);
        if ( abs(z) > 4.0 )
        {
            pMap[y][x].ed = 1;
            break;
        }
    }
    pMap[y][x].last.setprec();
    pMap[y][x].last = z;
    pMap[y][x].nIter = BASEITERATIONS - k;
    pMap[y][x].calc = 0;
    return pMap[y][x].nIter;
}

int interations = ITERATIONS;
int g_miniter;
inline
int MandelbrotEx(PIXEL& z, COMPLEX& c)
{
    int k = interations;
    //double r = 1.41403398, mr = -r;
    while (k > 0)
    {
        //func(z.last, c, z.ed);
        --k;
        func(z.last, c);
        //if (z.last.re > r || z.last.re < mr || z.last.im > r || z.last.im < mr)
        {
            if ( abs(z.last) > 4.0 )
            {
                z.ed = 1;
                int it = z.nIter + interations - k;
                if (it > g_miniter || g_miniter == 0)
                {
                    g_miniter = it;
                }
                break;
            }
        }
    }
    z.nIter += interations - k;
    return z.nIter;
}

/////////////////////////////////////////////////
// 绘制 Mandelbrot Set (曼德布洛特集)
/////////////////////////////////////////////////
void Draw(Float fromx, Float fromy, Float tox, Float toy, int mode = 0, COMPLEX _c = COMPLEX())
{
    unsigned t = clock();
    COMPLEX z, c;
    g_miniter = 0;
    if (mode == 0)
    {
        for (int y=0; y<SC_H; y++)
        {
            c.im = fromy + (toy - fromy) * (y / (double)SC_H);
            for (int x=0; x<SC_W; x++)
            {
                int k;
                c.re = fromx + (tox - fromx) * (x / (double)SC_W);
                z.re = z.im = 0.0;
                k = Mandelbrot(z, c, x, y);
                if (pMap[y][x].ed == 0)
                    putpixel_f(x, y, BLACK);
                else
                    putpixel_f(x, y, Color[k & COLORMASK]);
            }
            if (clock() - t > 50)
            {
                delay(0);
                t = clock();
            }
        }
    }
    else
    {
        c = _c;
        for (int y=0; y<SC_H; y++)
        {
            for (int x=0; x<SC_W; x++)
            {
                int k;
                z.im = fromy + (toy - fromy) * (y / (double)SC_H);
                z.re = fromx + (tox - fromx) * (x / (double)SC_W);
                k = Mandelbrot(z, c, x, y);
                if (pMap[y][x].ed == 0)
                    putpixel_f(x, y, BLACK);
                else
                    putpixel_f(x, y, Color[k & COLORMASK]);
            }
            if (clock() - t > 100)
            {
                delay(0);
                t = clock();
            }
        }
    }
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
    }
    g_udlist.clear();
    for (int y=0; y<SC_H; y++)
    {
        for (int x=0; x<SC_W; x++)
        {
            PIXEL& p = pMap[y][x];
            if (p.calc)
            {
                g_udlist.push(x, y);
            }
            else if (p.ed == 0 && (random(10000)) == 0)
            {
                g_udlist.push(x, y);
                p.calc = 1;
            }
        }
    }
    {
        int x = SC_W / 2, y = SC_H / 2;
        PIXEL& p = pMap[y][x];
        if (p.ed == 0)
        {
            g_udlist.push(x, y);
            p.calc = 1;
        }
    }
    g_udlist.swap();
}

void addpoint(int x, int y)
{
    if (x < 0 || x >= SC_W || y < 0 || y >= SC_H) return;
    if (pMap[y][x].ed == 0)
    {
        pMap[y][x].calc = 1;
        g_udlist.push(x, y);
    }
}

void DrawEx(Float fromx, Float fromy, Float tox, Float toy, int mode = 0, COMPLEX _c = COMPLEX())
{
    COMPLEX z, c;
    if (mode == 0)
    {
        int x, y;
        while (g_udlist.pop(&x, &y))
        {
            PIXEL& p = pMap[y][x];
            if (p.ed == 0)
            {
                int k;
                c.re = fromx + (tox - fromx) * (x / (double)SC_W);
                c.im = fromy + (toy - fromy) * (y / (double)SC_H);
                k = MandelbrotEx(p, c);
                if (p.ed)
                {
                    addpoint(x, y-1);
                    addpoint(x, y+1);
                    addpoint(x-1, y);
                    addpoint(x+1, y);
                    putpixel_f(x, y, Color[k & COLORMASK]);
                }
                else
                {
                    addpoint(x, y);
                }
            }
        }
    }
    else
    {
        c = _c;
        int x, y;
        while (g_udlist.pop(&x, &y))
        {
            PIXEL& p = pMap[y][x];
            if (p.ed == 0)
            {
                int k = MandelbrotEx(p, c);
                if (p.ed)
                {
                    addpoint(x, y-1);
                    addpoint(x, y+1);
                    addpoint(x-1, y);
                    addpoint(x+1, y);
                    putpixel_f(x, y, Color[k & COLORMASK]);
                }
                else
                {
                    addpoint(x, y);
                }
            }
        }
    }
    g_udlist.swap();
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
/////////////////////////////////////////////////
// 主函数
/////////////////////////////////////////////////
int WinMain()
{
    // 初始化绘图窗口及颜色
    int w = SC_W, h = SC_H, th = 12, tel = 4, tl = tel * 3 + 1;
    int rw = 640, rh = 280;
    freopen("log.txt", "w", stdout);
    setinitmode(0);
    initgraph(rw, rh + th*tl, INIT_DEFAULT|INIT_RENDERMANUAL);
    randomize();
    InitColor();
    setfont(12, 0, "宋体");
    SetWindowTextA(getHWnd(), "Mandelbrot Set by 御坂美琴 -- PowerEasyX V0.3.4 Release (20110129)");
    //mpf_set_prec(100);


    // 初始化 Mandelbrot Set(曼德布洛特集)坐标系
    COMPLEX from, to;
    COMPLEX from_b, to_b;
    from.re = -2.2; to.re = 2.2;
    from.im = -1.65; to.im = 1.65;
    //*
    do
    {
        FILE * fp = fopen("MandelbrotSet.ini", "r");
        char str[1024] = {0};
        COMPLEX center, delta;
        center.re = "0";
        if (fp)
        {
            g_prec = 32*80;
            center.setprec();
            delta.setprec();
            from.setprec();
            to.setprec();

            fgets(str, 1000, fp);
            str[strlen(str)-1] = 0;
            printf("%s\n", str);
            center.re = str;
            fgets(str, 1000, fp);
            str[strlen(str)-1] = 0;
            printf("%s\n", str);
            center.im = str;
            fgets(str, 1000, fp);
            str[strlen(str)-1] = 0;
            printf("%s\n", str);
            delta.re = str;
            fclose(fp);

            delta.re /= 2;
            delta.im = delta.re * 0.75;
            from.re = center.re - delta.re;
            to.re = center.re + delta.re;
            from.im = center.im - delta.im;
            to.im = center.im + delta.im;
            setgprec(delta.re);
            from.setprec();
            to.setprec();
        }
        else
        {
            g_prec = 64;
        }
    }
    while (0);
    //*/

    Draw(from.re, from.im, to.re, to.im);


    // 捕获鼠标操作，实现放大鼠标选中区域
    MOUSEMSG m;
    bool isLDown = false;
    COMPLEXI self, selt;    // 定义选区
    COMPLEXI self_b, selt_b;    // 定义选区
    COMPLEX js_c;
    int mode = 0;
    mp_bitcnt_t prec_b = 32;
    //IMAGE img_def, img_ms;
    //img_def.getimage(0, 0, w, h);

    //BeginBatchDraw();
    for ( ; ; )
    {
        int bmsg = 0;
        if (kbhit())
        {
            int k = getch();
            bmsg = 1;
            if (k == 'b' || k == 'B')
            {
                Float x1 = from.re + (from.re - to.re) / 2;
                Float x2 = to.re - (from.re - to.re) / 2;
                Float y1 = from.im + (from.im - to.im) / 2;
                Float y2 = to.im - (from.im - to.im) / 2;
                from.re = x1; to.re = x2;
                from.im = y1; to.im = y2;
                Draw(from.re, from.im, to.re, to.im, mode, js_c);
            }
            if (k == 's' || k == 'S')
            {
                PIMAGE pimg;
                getimage(pimg, 0, 0, w, h);
                saveimage(pimg, "m.bmp");
                delimage(pimg);
            }
            //*
            if (k == 'r' || k == 'R')
            {
                FILE * fp = fopen("MandelbrotSet.ini", "w");
                if (fp)
                {
                    char str[1000];
                    Float x = (from.re + to.re) * 0.5,
                          y = (from.im + to.im) * 0.5,
                          d = to.re - from.re;
                    gmp_sprintf(str, "%.500Ff", x.get_mpf_t());
                    fprintf(fp, "%s\n", str);
                    gmp_sprintf(str, "%.500Ff", y.get_mpf_t());
                    fprintf(fp, "%s\n", str);
                    gmp_sprintf(str, "%.500Ff", d.get_mpf_t());
                    fprintf(fp, "%s\n", str);
                    fclose(fp);
                }
            }//*/
        }
        if (mousemsg())
        {
            bmsg = 1;
            m = GetMouseMsg();    // 获取一条鼠标消息

            switch (m.uMsg)
            {
                // 按鼠标右键恢复原图形坐标系
            case WM_RBUTTONUP:
                if (mode == 0)
                {
                    from.re = -2.2; to.re = 2.2;
                    from.im = -1.65; to.im = 1.65;
                    Draw(from.re, from.im, to.re, to.im);
                    //img_def.putimage(0, 0);
                }
                else
                {
                    from.re = -2.0; to.re = 2.0;
                    from.im = -1.5; to.im = 1.5;
                    Draw(from.re, from.im, to.re, to.im, mode, js_c);
                }
                break;

            case WM_MBUTTONUP:
                mode = 1 - mode;
                if (mode == 0)
                {
                    g_prec = prec_b;
                    from.setprec();
                    to.setprec();
                    self.re = self_b.re;
                    self.im = self_b.im;
                    selt.re = selt_b.re;
                    selt.im = selt_b.im;
                    from.re = from_b.re;
                    from.im = from_b.im;
                    to.re = to_b.re;
                    to.im = to_b.im;
                    Draw(from.re, from.im, to.re, to.im, mode, js_c);
                    //putimage(0, 0, &img_ms);
                }
                else
                {
                    from_b.setprec();
                    to_b.setprec();
                    js_c.setprec();
                    prec_b = g_prec;
                    g_prec = 64;
                    self_b.re = self.re;
                    self_b.im = self.im;
                    selt_b.re = selt.re;
                    selt_b.im = selt.im;
                    js_c.re = from.re + (to.re - from.re) * self.re / w;
                    js_c.im = from.im + (to.im - from.im) * self.im / h;
                    from_b.re = from.re;
                    from_b.im = from.im;
                    to_b.re = to.re;
                    to_b.im = to.im;
                    from.re = -2.2; to.re = 2.2;
                    from.im = -1.65; to.im = 1.65;
                    from.setprec();
                    to.setprec();
                    //img_ms.getimage(0, 0, w, h);
                    Draw(from.re, from.im, to.re, to.im, mode, js_c);
                }
                flushmouse();
                break;
                // 按鼠标左键并拖动，选择区域
            case WM_MOUSEMOVE:
                if (isLDown)
                {
                    rectangle(self.re, self.im, selt.re, selt.im);
                    selt.re = m.x;
                    selt.im = m.y;
                    rectangle(self.re, self.im, selt.re, selt.im);
                }
                {
                    char str[200];
                    Float x = from.re + (to.re - from.re) * m.x / w,
                          y = from.im + (to.im - from.im) * m.y / h,
                          d = to.re - from.re;
                    setcolor(0xFFFFFF);
                    gmp_sprintf(str, "%+.420Ff", x.get_mpf_t());
                    outtextrect(0, rh + th*0, 640, th*tel, str);
                    gmp_sprintf(str, "%+.420Ff", y.get_mpf_t());
                    outtextrect(0, rh + th*tel, 640, th*tel, str);
                    gmp_sprintf(str, "%+.420Ff", d.get_mpf_t());
                    outtextrect(0, rh + th*tel*2, 640, th*tel, str);
                    gmp_sprintf(str, "%-6d %9d", mpf_get_prec(y.get_mpf_t()), g_miniter);;
                    outtextxy(0, rh + th*tel*3, str);
                    //SetWindowTextA(GetHWnd(), str);
                }
                break;

                // 按鼠标左键并拖动，选择区域
            case WM_LBUTTONDOWN:
                setcolor(WHITE);
                setwritemode(R2_XORPEN);
                isLDown = true;
                self.re = selt.re = m.x;
                self.im = selt.im = m.y;
                rectangle(self.re, self.im, selt.re, selt.im);

                break;

                // 按鼠标左键并拖动，选择区域
            case WM_LBUTTONUP:
                if (isLDown == true)
                {
                    rectangle(self.re, self.im, selt.re, selt.im);
                    setwritemode(R2_COPYPEN);
                    isLDown = false;
                    selt.re = m.x;
                    selt.im = m.y;

                    if (self.re == selt.re || self.im == selt.im) break;

                    // 修正选区为 4:3
                    int tmp;
                    if (self.re > selt.re)    {tmp = self.re; self.re = selt.re; selt.re = tmp;}
                    if (self.im > selt.im)    {tmp = self.im; self.im = selt.im; selt.im = tmp;}

                    if ( (selt.re - self.re) * 0.75 < (selt.im - self.im) )
                    {
                        selt.im += (3 - (selt.im - self.im) % 3);
                        self.re -= (selt.im - self.im) / 3 * 4 / 2 - (selt.re - self.re) / 2;
                        selt.re = self.re + (selt.im - self.im) / 3 * 4;
                    }
                    else
                    {
                        selt.re += (4 - (selt.re - self.re) % 4);
                        self.im -= (selt.re - self.re) * 3 / 4 / 2 - (selt.im - self.im ) / 2;
                        selt.im = self.im + (selt.re - self.re ) * 3 / 4;
                    }

                    // 更新坐标系
                    Float f = from.re + (to.re - from.re) * self.re / w,
                          t = from.re + (to.re - from.re) * selt.re / w;
                    from.re = f;
                    to.re = t;
                    f = from.im + (to.im - from.im) * self.im / h;
                    t = from.im + (to.im - from.im) * selt.im / h;
                    from.im = f;
                    to.im = t;
                    {
                        mp_bitcnt_t t = 0;
                        f = to.re - from.re;
                        setgprec(f);
                        mpf_set_default_prec(g_prec);
                        from.setprec();
                        to.setprec();
                    }

                    // 画图形
                    Draw(from.re, from.im, to.re, to.im, mode, js_c);
                    flushmouse();
                }
                break;
            }
        }
        if (bmsg)
        {
            //
        }
        else
        {
            //*
            for (int n=0, t = clock(); g_udlist.nLen > 0 && n < 1024; ++n)
            {
                DrawEx(from.re, from.im, to.re, to.im, mode, js_c);
                if (clock() - t > 100) break;
            }
            //*/
            delay_fps(1000);
        }
    }
    //EndBatchDraw();

    getch();
    closegraph();
    return 0;
}

