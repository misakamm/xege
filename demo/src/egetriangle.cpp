#include "graphics.h"
#include "ege/fps.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int width = 640, height = 480;

struct point //����㣬�������꣬�ٶ�
{
    double x;
    double y;
    double dx;
    double dy;
    color_t color;                  //��ɫ
    color_t nextcolor, prevcolor;   //��һ�ε���ɫ��Ŀ����ɫ
    int chtime, nowtime;            //���ɱ仯ʱ�䣬��ǰʱ��
    int nextcolortime;              //����һ�´θı���ɫ��ʱ��
};

struct poly //�������Σ�������ĸ������͵�����
{
    int n_point;
    point p[20];
};

struct polys //�������ζ�����
{
    int n_poly;                 //����ζ��г���
    poly p[100];                //���������
};

double rand_float(double dv, double db) //����һ��db �� db+dv֮������������
{
    return randomf()*dv + db;
}

color_t getcolor(color_t prevcolor, color_t nextcolor, double t)
{
    if (t <= 0) return prevcolor;
    if (t >= 1) return nextcolor;
    color_t r, g, b;
    r = (color_t)(EGEGET_R(prevcolor) * (1 - t) + EGEGET_R(nextcolor) * t);
    g = (color_t)(EGEGET_G(prevcolor) * (1 - t) + EGEGET_G(nextcolor) * t);
    b = (color_t)(EGEGET_B(prevcolor) * (1 - t) + EGEGET_B(nextcolor) * t);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    return EGERGB(r, g, b);
}

void movepoint(struct point* b) //���ݵ���ٶ������ƶ�����㣬����Ƴ���Ļ����з�������
{
    double dv = 1.0, db = 0.5;
    double tw = width / 640.0, th = height / 480.0;
    if (b->x <0) b->dx = rand_float(dv, db) * tw;
    if (b->y <0) b->dy = rand_float(dv, db) * th;
    if (b->x >width) b->dx = -rand_float(dv, db) * tw;
    if (b->y >height) b->dy = -rand_float(dv, db) * th;
    b->x += b->dx;
    b->y += b->dy;

    b->nowtime += 1;
    if (b->nowtime > b->chtime + b->nextcolortime)
    {
        b->nowtime = 0;
        b->prevcolor = b->nextcolor;
        b->nextcolor = hsv2rgb((float)random(360), 1.0f, 1.0f);
        b->chtime = random(1024) + 512;
        b->nextcolortime = random(1024) + 512;
    }
    b->color = getcolor(b->prevcolor, b->nextcolor, (double)b->nowtime / b->chtime);
}

void movepoly(struct poly* p) //�ƶ���������Σ��ڲ����õ���ƶ�
{
    int i;
    for (i=0; i<p->n_point; ++i)
    {
        movepoint(&(p->p[i]));
    }
}

void initpolys(struct polys* p, int npoly, int npoint) //��ʼ������ζ�����
{
    int i,j;
    p->n_poly = npoly;
    j = 0;
    p->p[j].n_point = npoint;
    for (i=0; i<npoint; ++i)
    {
        p->p[j].p[i].x = random(width);
        p->p[j].p[i].y = random(height);
        p->p[j].p[i].dx = (randomf() * 2 + 1);
        p->p[j].p[i].dy = (randomf() * 2 + 1);
        p->p[j].p[i].color = 0;
        p->p[j].p[i].prevcolor = 0;
        p->p[j].p[i].nextcolor = hsv2rgb((float)random(360), 1.0f, 0.5f);
        p->p[j].p[i].chtime = 1000;
        p->p[j].p[i].nowtime = 0;
        p->p[j].p[i].nextcolortime = 1000;
    }
    for (j=1; j<npoly; ++j)
    {
        p->p[j] = p->p[j-1];
    }
}

void draw_poly(struct poly* p) //����һ�������
{
    ege_colpoint points[100];
    int i;
    for (i=0; i<p->n_point; ++i)
    {
        points[i].x = p->p[i].x;
        points[i].y = p->p[i].y;
        points[i].color = p->p[i].color;
    }
        points[i].x = p->p[0].x;
        points[i].y = p->p[0].y;
        points[i].color = p->p[0].color;
    //setcolor(color);
    fillpoly_gradient(p->n_point, points);
}

int main()
{
    static struct polys p[10] = {{0}};
    int n_points[10] = {3,3,3,6,7};
    int n_poly[10] = {1,1,1,1,1};
    int n_polys = 3, i;
    randomize();
    //ͼ�γ�ʼ��
    {
        setinitmode(1, 0, 0);
        initgraph(-1, -1);
        width  = getmaxx();
        height = getmaxy();
        setrendermode(RENDER_MANUAL);
    }
    //����ζ����ʼ��
    for (i=0; i< n_polys; ++i)
    {
        initpolys(&p[i], n_poly[i], n_points[i]);
    }
    fps ui_fps;
    //��ѭ��
    for ( ; is_run(); delay_fps(60))
    {
        if (kbhit() > 0) //�а������¾��˳�
        {
            break;
        }
        for (i=0; i< n_polys; ++i)
        {
            movepoly(p[i].p);
        }
        cleardevice();
        for (i=0; i< n_polys; ++i)
        {
            draw_poly(p[i].p);
        }
        //imagefilter_blurring(NULL, 0xff, 0x100);
    }
    closegraph();
    return 0;
}

