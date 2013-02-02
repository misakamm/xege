// 指针式时钟显示+抗锯齿演示
#include <ege.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define for if (0); else for

ege::ege_point getpos(ege::ege_point center, float rad, float r)
{
    ege::ege_point pos;
    pos.x = sin(rad) * r + center.x;
    pos.y = -cos(rad) * r + center.y;
    return pos;
}

void draw()
{
    float pi2 = ege::PI * 2;
    ege::ege_point center;
    center.x = 200, center.y = 200;
    float r = 150;
    ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
    ege::setfont(24, 0, "Courier New");
    ege::setbkmode(TRANSPARENT);

    ege::ege_enable_aa(true);
    ege::setfillcolor(EGEARGB(0xff, 0x40, 0x40, 0x40));
    ege::setcolor(EGEARGB(0xff, 0x80, 0x00, 0xf0));
    ege::ege_fillellipse(center.x - r * 1.2f, center.y - r * 1.2f,
            r * 1.2f * 2.0f, r * 1.2f * 2.0f);

    ege::setcolor(ege::WHITE);
    for (int num = 1; num <= 12; ++num)
    {
        char str[8];
        ege::ege_point p = getpos(center, float(num * pi2 / 12), r);
        sprintf(str, "%d", num);
        ege::outtextxy((int)p.x, (int)p.y, str);
    }
    time_t t_now;
    time(&t_now);
    tm* t = localtime(&t_now);
    ege::setcolor(EGEARGB(0xff, 0x0, 0x0, 0xff));
    ege::setlinewidth(10.0f);
    {
        float h = float(t->tm_hour + t->tm_min / 60.0);
        char str[8];
        ege::ege_point p = getpos(center, float(h * pi2 / 12), r * 0.5f);
        ege::ege_line(p.x, p.y, center.x, center.y);
    }
    ege::setcolor(EGEARGB(0xff, 0xff, 0x0, 0xff));
    ege::setlinewidth(5.0f);
    {
        float m = float(t->tm_min + t->tm_sec / 60.0);
        char str[8];
        ege::ege_point p = getpos(center, float(m * pi2 / 60), r * 0.9f);
        ege::ege_line(p.x, p.y, center.x, center.y);
    }
    ege::setcolor(EGEARGB(0xff, 0xff, 0xff, 0));
    ege::setfillcolor(EGEARGB(0xff, 0xff, 0xff, 0));
    ege::setlinewidth(1.0f);
    {
        float s = float(t->tm_sec);
        char str[8];
        ege::ege_point p = getpos(center, float(s * pi2 / 60), r * 1.0f);
        ege::ege_line(p.x, p.y, center.x, center.y);
        ege::ege_fillellipse(center.x - r * 0.05f, center.y - r * 0.05f,
                r * 0.1f, r * 0.1f);
    }
    {
        char str[32];
        sprintf(str, "%d/%02d/%02d %2d:%02d:%02d",
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec);
        ege::setcolor(EGERGB(0xff, 0xff, 0));
        ege::outtextxy((int)center.x, (int)(center.y + r * 1.4f), str);
    }
}

void mainloop()
{

    for ( ; ege::is_run(); ege::delay_fps(60) )
    {
        ege::cleardevice();
        draw();
    }
}

int main()
{
    ege::setinitmode(ege::INIT_ANIMATION);
    ege::initgraph(400, 480);
    ege::randomize();
    // 程序主循环
    mainloop();
    // 关闭绘图设备
    ege::closegraph();
    return 0;
}


