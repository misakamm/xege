/**
 * @file egejulia.cpp
 *
 * @brief Julia集计算屏保动画
 *        （编译后改为scr后缀使用）
 */

#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// 定义常量
#define MAXCOLOR 64     // 颜色数
#define BF_W 1200
#define BF_H 1200

int g_w, g_h;

/////////////////////////////////////////////////
// 定义复数及乘、加运算
/////////////////////////////////////////////////

/**
 * @brief 复数结构体
 */
struct COMPLEX
{
	double re; /**< 实部 */
	double im; /**< 虚部 */
};

/**
 * @brief 复数乘法运算符重载
 * @param a 复数a
 * @param b 复数b
 * @return 两个复数的乘积
 */
COMPLEX operator * (COMPLEX a, COMPLEX b)
{
	COMPLEX c;
	c.re = a.re * b.re - a.im * b.im;
	c.im = a.im * b.re + a.re * b.im;
	return c;
}

/**
 * @brief 复数加法运算符重载
 * @param a 复数a
 * @param b 复数b
 * @return 两个复数的和
 */
COMPLEX operator + (COMPLEX a, COMPLEX b)
{
	COMPLEX c;
	c.re = a.re + b.re;
	c.im = a.im + b.im;
	return c;
}


/////////////////////////////////////////////////
// 定义颜色及初始化颜色
/////////////////////////////////////////////////

int Color[MAXCOLOR]; /**< 颜色数组 */

/**
 * @brief 初始化颜色
 */
void InitColor()
{
	// 使用 HSL 颜色模式产生角度 h1 到 h2 的渐变色
	int h1 = 240, h2 = 30;
	for(int i=0; i<MAXCOLOR/2; i++)
	{
		Color[i] = HSLtoRGB((float)h1, 1.0f, i * 2.0f / MAXCOLOR);
		Color[MAXCOLOR-1-i] = HSLtoRGB((float)h2, 1.0f, i * 2.0f / MAXCOLOR);
	}
}

double bilout = 256;
double logmap[(18 * 18) * 32];
double col_ins = 1.0 / bilout / bilout * 256 * 32;
double col_r = 28, col_g = 16, col_b = 32;
double col_ar = 0, col_ag = 0, col_ab = 128;

const double gc_lb = log(2.0);

/**
 * @brief 计算以2为底的对数
 * @param d 指数
 * @return 对数值
 */
double myLog2(double d) {
	return log(d) / gc_lb;
}

/**
 * @brief 初始化对数映射表
 */
void InitLog()
{
	for (int i=0; i<(18 * 18) * 32; ++i)
	{
		double r = i / col_ins;
		logmap[i] = 1 - myLog2(myLog2(r)/2);
	}
}

/**
 * @brief 将迭代次数转换为颜色值
 * @param iter 迭代次数
 * @return 颜色值
 */
int iterToColor(double iter)
{
	return (int)(fabs(fmod(iter + 255, 510) - 255));
}

/**
 * @brief 根据复数和迭代次数生成颜色
 * @param z 复数
 * @param iteration 迭代次数
 * @return 颜色值
 */
color_t colorMap(COMPLEX z, int iteration)
{
	double r = z.re * z.re + z.im * z.im;
	r = iteration + logmap[(int)(r * col_ins)];
	return EGERGB(iterToColor(r * col_r + col_ar), iterToColor(r * col_g + col_ag), iterToColor(r * col_b + col_ab));
}

int g_mi[BF_H][BF_W];

struct state
{
	int iter; /**< 迭代次数 */
	int ed; /**< 是否已经退出 */
	COMPLEX z; /**< 复数z */
	COMPLEX c; /**< 复数c */
};

state (*pMap)[BF_W] = new state[BF_H][BF_W];

struct updatelist
{
	POINT *p, *pn;
	POINT m_list[2][1920 * 1920];
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
state *g_st = NULL;

/**
 * @brief 添加一个点到更新列表
 * @param x x坐标
 * @param y y坐标
 * @param it 迭代次数
 */
void addpoint(int x, int y, int it = -1)
{
	if (x < 0 || x >= BF_W || y < 0 || y >= BF_H) return;
	if (pMap[y][x].ed == 0)
	{
		g_udlist.push(x, y);
	}
}

/**
 * @brief 添加一个点到Julia集更新列表
 * @param x x坐标
 * @param y y坐标
 * @param it 迭代次数
 */
void jaddpoint(int x, int y, int it = -1)
{
	if (x < 0 || x >= g_w || y < 0 || y >= g_h) return;
	if (g_st[y * g_w + x].ed == 0)
	{
		g_udlist.push(x, y);
	}
}

/**
 * @brief 计算Mandelbrot集的迭代次数
 * @param z Mandelbrot集的迭代状态
 * @return 迭代次数
 */
int MandelbrotEx(state& z)
{
	if (z.iter >= 64) return z.iter;
	int k = 64;
	int b = k;
	while (k > 0)
	{
		--k;
		z.z = z.z * z.z + z.c;
		if ( z.z.re*z.z.re + z.z.im*z.z.im > 4.0 )
		{
			z.ed = 1;
			break;
		}
	}
	z.iter += b - k;
	return z.iter;
}

/**
 * @brief 检查键盘和鼠标事件
 * @return 是否有键盘或鼠标事件发生
 */
int kbmouhit()
{
	if (kbmsg()) return 1;
	//return kbhit() || MouseHit();
	return 0;
}

/**
 * @brief 绘制Mandelbrot集
 * @param fromx X轴起始点
 * @param fromy Y轴起始点
 * @param tox X轴结束点
 * @param toy Y轴结束点
 * @return 更新的像素点数目
 */
int MDraw(double fromx, double fromy, double tox, double toy)
{
	int t = clock();
	int ret = 0;
{
	int x, y;
	while (g_udlist.pop(&x, &y))
	{
		state& p = pMap[y][x];
		if (p.iter == 0 && p.ed == 0)
		{
			COMPLEX z, c;
			c.re = fromx + (tox - fromx) * (x / (double)BF_W);
			c.im = fromy + (toy - fromy) * (y / (double)BF_H);
			z.re = z.im = 0.0;
			p.c = c;
			p.z = z;
		}
		if (p.ed == 0)
		{
			int k;
			k = MandelbrotEx(p);
			if (p.ed)
			{
				ret++;
{
	addpoint(x, y-1, k);
	addpoint(x, y+1, k);
	addpoint(x-1, y, k);
	addpoint(x+1, y, k);
}
				g_mi[y][x] = k;
				/*
				  {
				  color_t c = 0;
				  c = colorMap(p.z, p.iter);
				  putpixel(x, y, c);
				  }// */
			}
			else
			{
				addpoint(x, y);
			}
		}
		if (kbmouhit()) break;
	}
}
	g_udlist.swap();
	return ret;
}

int g_updatepoint = 0;

/////////////////////////////////////////////////
// 绘制 Julia Set
/////////////////////////////////////////////////

/**
 * @brief 绘制Julia集
 * @param c 复数c
 * @param fromx X轴起始点
 * @param fromy Y轴起始点
 * @param tox X轴结束点
 * @param toy Y轴结束点
 * @param sr sin(rotate)
 * @param cr cos(rotate)
 * @return 更新的像素点数目
 */
int JDraw(COMPLEX c, double fromx, double fromy, double tox, double toy, double sr, double cr)
{
	int ret = 0;
	int update = 0;
	state* st = g_st - 1;
	clock_t tt = clock();
	g_updatepoint = 0;
	for(int y=0; y<g_h; y++)
	{
		for(int x=0; x<g_w; x++)
		{
			++st;
			if (st->ed)
			{
				continue;
			}
			COMPLEX& z = st->z;

			if (st->iter == 0)
			{
				double re = fromx + (tox - fromx) * (x / (double)g_w);
				double im = fromy + (toy - fromy) * (y / (double)g_h);
				z.re = cr * re + sr * im;
				z.im = sr * re - cr * im;
			}
			else
			{
				//z = st->z;
			}
			st->iter++;
{
	z = z * z + c;
	if ( z.re*z.re + z.im*z.im > bilout )
	{
		st->ed = 1;
	}
}
			++ret;
			if ( st->ed )
			{
				color_t c = 0;
				c = colorMap(z, st->iter);
				putpixel(x, y, c);
				g_updatepoint += 1;
			}
			else if (st->iter == 1)
			{
				color_t c = 0;
				//c = colorMap(z, st->iter);
				putpixel_f(x, y, c);
			}
		}
		if (clock() - tt > 10)
		{
			tt = clock();
			if (kbmouhit())
			{
				return -1;
			}
		}
	}
	return ret;
}

/**
 * @brief 绘制Julia集（增量更新）
 * @param c 复数c
 * @param fromx X轴起始点
 * @param fromy Y轴起始点
 * @param tox X轴结束点
 * @param toy Y轴结束点
 * @return 更新的像素点数目
 */
int JDrawA(COMPLEX c, double fromx, double fromy, double tox, double toy)
{
	clock_t tt = clock();
	int ret = 0;
	g_updatepoint = 0;
	state* st = g_st;
{
	int x, y;
	while (g_udlist.pop(&x, &y))
	{
		state& p = st[y * g_w + x];
		if (p.ed == 0)
		{
			int k;
{
	p.iter++;
	k = p.iter;
	COMPLEX &z = p.z;
	z = z * z + c;
	if ( z.re*z.re + z.im*z.im > bilout )
	{
		p.ed = 1;
	}
}
			ret ++;
			if (p.ed)
			{
				color_t c = 0;
				c = colorMap(p.z, k);
				putpixel(x, y, c);
				g_updatepoint += 1;
			}
			else
			{
				g_udlist.push(x, y);
			}
		}
		/*
		  if (clock() - tt > 10)
		  {
		  delay(1);
		  tt = clock();
		  if (0 && kbmouhit())
		  {
		  return -1;
		  }
		  }//*/
	}
}
	g_udlist.swap();
	return ret;
}

/**
 * @brief 初始化状态数组
 * @param x X轴大小
 * @param y Y轴大小 */
void init_st(int x, int y)
{
	memset(g_st, 0, x * y * sizeof(state));
}

/**
 * @brief 主函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 程序退出码
 */
int main(int argc, char* argv[])
{
	// 初始化绘图窗口及颜色
	setinitmode(0x005, 0, 0);
	// if (argc < 2)
	// {
	//     MessageBox(NULL, TEXT("本屏幕保护程序无配置"), TEXT("JuliaSet"), MB_OK);
	//     return 0;
	// }
	// else if (stricmp(argv[1], "/p") == 0)
	// {
	//     HWND hwnd;
	//     sscanf(argv[2], "%d", &hwnd);
	//     attachHWND(hwnd);
	//     setinitmode(0x107, 0, 0);
	// }
	// else if (stricmp(argv[1], "/s"))
	// {
	//     MessageBox(NULL, TEXT("本屏幕保护程序无配置"), TEXT("JuliaSet"), MB_OK);
	//     return 0;
	// }

	//initgraph(320, 240);
	initgraph(-1, -1);

	randomize();
	showmouse(0);
	flushmouse();
	while(kbhit()) getch();

	//InitColor();
	InitLog();
	g_w = getwidth(NULL);
	g_h = getheight(NULL);
	g_st = (state*)malloc(g_w * g_h * sizeof(state));
	COMPLEX c = {0.262, 0.002}, z = {0, 0};
	double r = 1.5, d = g_w / (double)g_h, rotate = 0, sr = sin(rotate), cr = cos(rotate);
	init_st(g_w, g_h);
	int n_update = 0;
	double ftime = fclock();
{
	double dc = 64, dca = 128, db = 16;
	col_r = randomf() * dc + db;
	col_g = randomf() * dc + db;
	col_b = randomf() * dc + db;
	col_ar = randomf() * dca;
	col_ag = randomf() * dca;
	col_ab = randomf() * dca;
	rotate = randomf() * 360;
	sr = sin(rotate), cr = cos(rotate);
}
	setrendermode(RENDER_MANUAL);
	for (int loop = 1; kbmouhit() == 0; ++loop)
	{
		int ret;
		if (loop <= 4)
		{
			ret = JDraw(c, z.re - r * d, z.im - r, z.re + r * d, z.im + r, sr, cr);
			if (loop == 4)
			{
				g_udlist.swap();
				for(int y=0; y<g_h; y++)
				{
					for(int x=0; x<g_w; x++)
					{
						if (g_st[y * g_w + x].ed == 0)
						{
							g_udlist.push(x, y);
						}
					}
				}
				g_udlist.swap();
			}
		}
		else
		{
			static int t = 0;
			ret = JDrawA(c, z.re - r * d, z.im - r, z.re + r * d, z.im + r);
			if (clock() - t > 30)
			{
				delay(1);
				t = clock();
			}
		}
		if (g_updatepoint == 0)
		{
			n_update++;
		}
		else
		{
			n_update = 0;
		}
		if (0)
		{
			char str[500];
			sprintf(str, "%d %d %f %f", g_w, g_h, r, d);
			outtextxy(0, 0, str);
		}
		if (ret == 0 || n_update > 8 || loop > 1000)
		{
			loop = 0;
			if (g_mi[0][0] == 0)
			{
				delay(1);
				memset(pMap, 0, BF_W * BF_H * sizeof(state));
				g_udlist.clear();
				int i;
				for (i = 0; i < BF_W; ++i)
				{
					addpoint(i, 0);
					addpoint(i, BF_H - 1);
				}
				for (i = 0; i < 4; )
				{
					if (MDraw(-1.9, -1.2, 0.5, 1.2) == 0)
					{
						++i;
					}
					else
					{
						i = 0;
					}
					if (kbmouhit()) return 0;
				}
			}
			double dc = 64, dca = 128, db = 16;
			col_r = randomf() * dc + db;
			col_g = randomf() * dc + db;
			col_b = randomf() * dc + db;
			col_ar = randomf() * dca;
			col_ag = randomf() * dca;
			col_ab = randomf() * dca;
			rotate = randomf() * 360;
			sr = sin(rotate), cr = cos(rotate);
			do
			{
				c.re = randomf() * 2.4 - 1.9;
				c.im = randomf() * 2.4 - 1.2;
				int ir = (int)((c.re - (-1.9)) / (0.5 - (-1.9)) * BF_W);
                int im = (int)((c.im - (-1.2)) / (1.2 - (-1.2)) * BF_H);
                if (g_mi[im][ir] >= 16)
                {
                    break;
                }
            } while (1);
            init_st(g_w, g_h);
            n_update = 0;
            if (fclock() - ftime < 3)
            {
                delay_ms((int)((3 - (fclock() - ftime)) * 1000));
            }
            else
            {
                delay(1);
            }
            ftime = fclock();
        }
    }

    closegraph();
    return 0;
}

