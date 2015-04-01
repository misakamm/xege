#include "graphics.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ege/fps.h"

int width = 640, height = 480;

struct point //����㣬�������꣬�ٶ�
{
    double x;
    double y;
    double dx;
    double dy;
};

struct poly //�������Σ�������ĸ������͵�����
{
    int n_point;
    point p[20];
};

struct polys //�������ζ�����
{
    int n_poly;                 //����ζ��г���
    int color;                  //��ɫ
    int nextcolor, prevcolor;   //��һ�ε���ɫ��Ŀ����ɫ
    int chtime, nowtime;        //���ɱ仯ʱ�䣬��ǰʱ��
    int time;                   //����һ�´θı���ɫ��ʱ��
    poly p[100];                //���������
};

double rand_float(double dv, double db) //����һ��db �� db+dv֮������������
{
    return randomf()*dv + db;
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
}

void movepoly(struct poly* p) //�ƶ���������Σ��ڲ����õ���ƶ�
{
    int i;
    for (i=0; i<p->n_point; ++i)
    {
        movepoint(&(p->p[i]));
    }
}

void movepolys(struct polys* p) //�ƶ�����ζ��У�����ʱ���⣬��ɫ����
{
    int i;
    for (i=p->n_poly-1; i>0; --i)
    {
        p->p[i] = p->p[i-1];
    }
    movepoly(p->p);
    ++(p->nowtime);
    if (--(p->time) <= 0)
    {
        p->prevcolor = p->color;
        p->nextcolor = HSVtoRGB((float)random(360), 1.0f, (float)rand_float(0.5, 0.5));
        p->time = random(1000);
        p->chtime = random(1000)+60;
        p->nowtime = 0;
    }
    if (p->nowtime >= p->chtime)
    {
        p->color = p->nextcolor;
    }
    else
    {
        double dr = p->prevcolor&0xFF, dg = (p->prevcolor>>8)&0xFF, db = (p->prevcolor>>16)&0xFF;
        double dt = 1 - p->nowtime / (double)(p->chtime);
        dr -= p->nextcolor&0xFF, dg -= (p->nextcolor>>8)&0xFF, db -= (p->nextcolor>>16)&0xFF;
        dr *= dt, dg *= dt, db *= dt;
        dr += p->nextcolor&0xFF, dg += (p->nextcolor>>8)&0xFF, db += (p->nextcolor>>16)&0xFF;
        p->color = ((int)dr) | ((int)dg<<8) | ((int)db<<16);
    }
}

void initpolys(struct polys* p, int npoly, int npoint) //��ʼ������ζ�����
{
    int i,j;
    p->n_poly = npoly;
    p->color = 0;
    p->time = 1000;
    p->prevcolor = p->color;
    p->nextcolor = HSVtoRGB((float)random(360), 1.0f, 0.5f);
    p->chtime = 1000;
    p->nowtime = 0;
    j = 0;
    p->p[j].n_point = npoint;
    for (i=0; i<npoint; ++i)
    {
        p->p[j].p[i].x = random(width);
        p->p[j].p[i].y = random(height);
        p->p[j].p[i].dx = (randomf() * 2 + 1);
        p->p[j].p[i].dy = (randomf() * 2 + 1);
    }
    for (j=1; j<npoly; ++j)
    {
        p->p[i] = p->p[i-1];
    }
}

void draw_poly(struct poly* p, int color) //����һ�������
{
    int points[100];
    int i;
    for (i=0; i<p->n_point; ++i)
    {
        points[i*2  ] = (int)(p->p[i].x+.5f);
        points[i*2+1] = (int)(p->p[i].y+.5f);
    }
    points[i*2  ] = (int)(p->p[0].x+.5f);
    points[i*2+1] = (int)(p->p[0].y+.5f);
    setcolor(color);
    drawpoly(p->n_point+1, points);
}

void draw_polys(struct polys* p) //���ƶ���ζ��У�ֻ����һ�������һ�������һ�����ڲ�����
{
    draw_poly(&(p->p[p->n_poly-1]),        0);
    draw_poly(&(p->p[          0]), p->color);
    //for (int i = 0; i < 4; ++i)
    //    draw_poly(&(p->p[i]), p->color);
}

int main()
{
    static struct polys p[10] = {{0}};
    int n_points[10] = {4,3,5,6,7};
    int n_poly[10] = {80,40,10,5,1};
    int n_polys = 2, i;
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
    setfont(12, 6, "����");
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
            movepolys(&(p[i]));
        }
        for (i=0; i< n_polys; ++i)
        {
            draw_polys(&(p[i]));
        }
        //imagefilter_blurring(NULL, 0xff, 0x100);
    }
    closegraph();
    return 0;
}

