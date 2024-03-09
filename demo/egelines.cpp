/**********************************************************************
 * 文件名：egelines.cpp
 *
 * 程序目的：实现变幻线屏保效果
 *
 * 使用的图形库：EGE
 *
 **********************************************************************/

#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ege/fps.h"

int width = 640, height = 480;

/**
 * @brief 点结构体，包含点的坐标和速度
 */
struct point
{
	double x;   /**< 点的横坐标 */
	double y;   /**< 点的纵坐标 */
	double dx;  /**< 点在横向上的速度 */
	double dy;  /**< 点在纵向上的速度 */
};

/**
 * @brief 多边形结构体，包含点的个数和点的数组
 */
struct poly
{
	int n_point;        /**< 点的个数 */
	point p[20];        /**< 点的数组 */
};

/**
 * @brief 多边形队列组结构体
 */
struct polys
{
	int n_poly;                 /**< 多边形队列的长度 */
	int color;                  /**< 当前颜色 */
	int nextcolor, prevcolor;   /**< 上一次的颜色和目标颜色 */
	int chtime, nowtime;        /**< 过渡变化时间和当前时间 */
	int time;                   /**< 距离下次改变颜色的时间 */
	poly p[100];                /**< 多边形数组 */
};

/**
 * @brief 返回一个位于 [db, db+dv] 范围内的随机浮点数
 * @param dv 随机数范围的大小
 * @param db 随机数范围的起始点
 * @return 随机生成的浮点数
 */
double rand_float(double dv, double db)
{
	return randomf() * dv + db;
}

/**
 * @brief 根据点的速度属性移动点的位置，若移出屏幕则进行反弹计算
 * @param b 需要移动的点
 */
void movepoint(struct point* b)
{
	double dv = 1.0, db = 0.5;
	double tw = width / 640.0, th = height / 480.0;

	if (b->x < 0)
		b->dx = rand_float(dv, db) * tw;
	if (b->y < 0)
		b->dy = rand_float(dv, db) * th;
	if (b->x > width)
		b->dx = -rand_float(dv, db) * tw;
	if (b->y > height)
		b->dy = -rand_float(dv, db) * th;

	b->x += b->dx;
	b->y += b->dy;
}

/**
 * @brief 移动单个多边形，内部调用点的移动
 * @param p 需要移动的多边形
 */
void movepoly(struct poly* p)
{
	int i;

	for (i = 0; i < p->n_point; ++i)
	{
		movepoint(&(p->p[i]));
	}
}

/**
 * @brief 移动多边形队列，包含时间检测和颜色计算
 * @param p 需要移动的多边形队列
 */
void movepolys(struct polys* p)
{
	int i;

	for (i = p->n_poly - 1; i > 0; --i)
	{
		p->p[i] = p->p[i - 1];
	}

	movepoly(p->p);
	++(p->nowtime);

	if (--(p->time) <= 0)
	{
		p->prevcolor = p->color;
		p->nextcolor = HSVtoRGB((float)random(360), 1.0f, (float)rand_float(0.5, 0.5));
		p->time = random(1000);
		p->chtime = random(1000) + 60;
		p->nowtime = 0;
	}

	if (p->nowtime >= p->chtime)
	{
		p->color = p->nextcolor;
	}
	else
	{
		double dr = p->prevcolor & 0xFF, dg = (p->prevcolor >> 8) & 0xFF, db = (p->prevcolor >> 16) & 0xFF;
		double dt = 1 - p->nowtime / (double)(p->chtime);

		dr -= p->nextcolor & 0xFF, dg -= (p->nextcolor >> 8) & 0xFF, db -= (p->nextcolor >> 16) & 0xFF;
		dr *= dt, dg *= dt, db *= dt;
		dr += p->nextcolor & 0xFF, dg += (p->nextcolor >> 8) & 0xFF, db += (p->nextcolor >> 16) & 0xFF;

		p->color = ((int)dr) | ((int)dg << 8) | ((int)db << 16);
	}
}

/**
 * @brief 初始化多边形队列组
 * @param p 多边形队列组
 * @param npoly 多边形队列的长度
 * @param npoint 多边形中点的个数
 */
void initpolys(struct polys* p, int npoly, int npoint)
{
	int i, j;

	p->n_poly = npoly;
	p->color = 0;
	p->time = 1000;
	p->prevcolor = p->color;
	p->nextcolor = HSVtoRGB((float)random(360), 1.0f, 0.5f);
	p->chtime = 1000;
	p->nowtime = 0;

	j = 0;
	p->p[j].n_point = npoint;

	for (i = 0; i < npoint; ++i)
	{
		p->p[j].p[i].x = random(width);
		p->p[j].p[i].y = random(height);
		p->p[j].p[i].dx = (randomf() * 2 + 1);
		p->p[j].p[i].dy = (randomf() * 2 + 1);
	}

	for (j = 1; j < npoly; ++j)
	{
		p->p[i] = p->p[i - 1];
	}
}

/**
 * @brief

  绘制一个多边形
 * @param p 多边形对象
 * @param color 颜色值
 */
void draw_poly(struct poly* p, int color)
{
	int points[100];
	int i;

	for (i = 0; i < p->n_point; ++i)
	{
		points[i * 2] = (int)(p->p[i].x + 0.5f);
		points[i * 2 + 1] = (int)(p->p[i].y + 0.5f);
	}

	points[i * 2] = (int)(p->p[0].x + 0.5f);
	points[i * 2 + 1] = (int)(p->p[0].y + 0.5f);

	setcolor(color);
	drawpoly(p->n_point + 1, points);
}

/**
 * @brief 绘制多边形队列的多边形（只绘制第一个和最后一个，最后一个用于擦除）
 * @param p 多边形队列对象
 */
void draw_polys(struct polys* p)
{
	draw_poly(&(p->p[p->n_poly - 1]), 0);
	draw_poly(&(p->p[0]), p->color);
}

int main()
{
	static struct polys p[10] = {{0}};
	int n_points[10] = {4, 3, 5, 6, 7};
	int n_poly[10] = {80, 40, 10, 5, 1};
	int n_polys = 2, i;
	randomize();

	// 图形初始化
{
	setinitmode(1, 0, 0);
	initgraph(-1, -1);
	width = getmaxx();
	height = getmaxy();
	setrendermode(RENDER_MANUAL);
}

	// 多边形对象初始化
	for (i = 0; i < n_polys; ++i)
	{
		initpolys(&p[i], n_poly[i], n_points[i]);
	}

	setfont(12, 6, "宋体");
	fps ui_fps;

	// 主循环
	for (; is_run(); delay_fps(60))
	{
		if (kbhit() > 0) // 有按键按下就退出
		{
			break;
		}

		for (i = 0; i < n_polys; ++i)
		{
			movepolys(&(p[i]));
		}

		for (i = 0; i < n_polys; ++i)
		{
			draw_polys(&(p[i]));
		}
	}

	closegraph();
	return 0;
}

