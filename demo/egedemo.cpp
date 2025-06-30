// @warning 该示例程序文本输出乱码了
#include "graphics.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

class SceneBase
{
public:
    virtual SceneBase* Update()
    {
        return NULL;
    }
};

class SceneHelloWorld6 : public SceneBase
{
public:
    SceneHelloWorld6(SceneBase* parent)
    {
        m_parent = parent;
    }

    void smain()
    {
        setcolor(RED);
        arc(100, 100, 0, 180, 50);
        arc(200, 100, 0, 180, 50);
        line(50, 100, 150, 200);
        line(250, 100, 150, 200);
    }

    SceneBase* Update()
    {
        char str[] = "#include \"graphics.h\"\n\nint main()\n{\n    initgraph(640, 480);\n\n    setcolor(RED);\n    //画弧线，以(100,100)为圆心，0度到180度，半径50\n    arc(100, 100, 0, 180, 50);\n    //同画弧线，只是位置不同\n    arc(200, 100, 0, 180, 50);\n    //从(50,100)到(150,200)画线\n    line(50, 100, 150, 200);\n    //从(250,100)到(150,200)画线\n    line(250, 100, 150, 200);\n    getch(); //等待用户按键，相当于暂停\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();

        smain();

        setfont(12, 0, "宋体");
        setcolor(0x808080);
        line(320, 0, 320, 480);
        setcolor(0xFFFFFF);
        outtextrect(320, 100, 320, 380, str);
        outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");

        getch();
        return m_parent;
    }

private:
    SceneBase* m_parent;
};

class SceneHelloWorld5 : public SceneBase
{
public:
    SceneHelloWorld5(SceneBase* parent)
    {
        m_parent = parent;
    }

    void smain()
    {
        setcolor(YELLOW);
        //setfillstyle(SOLID_FILL, MAGENTA);
        fillellipse(150, 200, 50, 100);
    }

    SceneBase* Update()
    {
        char str[] = "#include \"graphics.h\"\n\nint main()\n{\n    //图形窗口初始化为640*480大小\n    initgraph(640, 480);\n\n    //设置颜色为黄色\n    setcolor(YELLOW);\n    //设置填充颜色为紫红色\n    setfillstyle(SOLID_FILL, MAGENTA);\n    //以(150,200)为圆心，x半径为50，y半径为100，画一个实心椭圆\n    fillellipse(150, 200, 50, 100);\n\n    getch(); //等待用户按键，相当于暂停\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();

        smain();

        setfont(12, 0, "宋体");
        setcolor(0x808080);
        line(320, 0, 320, 480);
        setcolor(0xFFFFFF);
        outtextrect(320, 100, 320, 380, str);
        outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");

        getch();
        return new SceneHelloWorld6(m_parent);
    }

private:
    SceneBase* m_parent;
};

class SceneHelloWorld4 : public SceneBase
{
public:
    SceneHelloWorld4(SceneBase* parent)
    {
        m_parent = parent;
    }

    void smain()
    {
        setfillstyle(SOLID_FILL, GREEN);
        bar(100, 100, 200, 400);
    }

    SceneBase* Update()
    {
        char str[] = "#include \"graphics.h\"\n\nint main()\n{\n    //图形窗口初始化为640*480大小\n    initgraph(640, 480);\n\n    //设置填充颜色为绿色，注意是用来填充颜色\n    setfillstyle(SOLID_FILL, GREEN);\n    //从(100,100)到(200,400)画一个实心矩形，使用填充颜色\n    bar(100, 100, 200, 400);\n\n    getch(); //等待用户按键，相当于暂停\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();

        smain();

        setfont(12, 0, "宋体");
        setcolor(0x808080);
        line(320, 0, 320, 480);
        setcolor(0xFFFFFF);
        outtextrect(320, 100, 320, 380, str);
        outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");

        getch();
        return new SceneHelloWorld5(m_parent);
    }

private:
    SceneBase* m_parent;
};

class SceneHelloWorld3 : public SceneBase
{
public:
    SceneHelloWorld3(SceneBase* parent)
    {
        m_parent = parent;
    }

    void smain()
    {
        setcolor(GREEN);
        circle(200, 100, 80);
    }

    SceneBase* Update()
    {
        char str[] = "#include \"graphics.h\"\n\nint main()\n{\n    //图形窗口初始化为640*480大小\n    initgraph(640, 480);\n\n    //设置颜色为绿色\n    setcolor(GREEN);\n    //在x=200,y=100的地方，画一个半径80的圆\n    circle(200, 100, 80);\n\n    getch(); //等待用户按键，相当于暂停\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();

        smain();

        setfont(12, 0, "宋体");
        setcolor(0x808080);
        line(320, 0, 320, 480);
        setcolor(0xFFFFFF);
        outtextrect(320, 100, 320, 380, str);
        outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");

        getch();
        return new SceneHelloWorld4(m_parent);
    }

private:
    SceneBase* m_parent;
};

class SceneHelloWorld2 : public SceneBase
{
public:
    SceneHelloWorld2(SceneBase* parent)
    {
        m_parent = parent;
    }
    void smain()
    {
        circle(200, 100, 80);
    }
    SceneBase* Update()
    {
        char str[] = "#include \"graphics.h\"\n\nint main()\n{\n    //图形窗口初始化为640*480大小\n    initgraph(640, 480);\n\n    //在x=200,y=100的地方，画一个半径80的圆\n    circle(200, 100, 80);\n\n    getch(); //等待用户按键，相当于暂停\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);

        smain();
        {
            setfont(12, 0, "宋体");
            setcolor(0x808080);
            line(320, 0, 320, 480);
            setcolor(0xFFFFFF);
            outtextrect(320, 100, 320, 380, str);
            outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");
        }
        getch();
        return new SceneHelloWorld3(m_parent);
    }

private:
    SceneBase* m_parent;
};

class SceneHelloWorld : public SceneBase
{
public:
    SceneHelloWorld(SceneBase* parent)
    {
        m_parent = parent;
    }
    SceneBase* Update()
    {
        char str[] = "//由两个斜杠\'//\'开始后面的内容为注释，不影响编译\n//以下这个是PowerEasyX图形库的头文件，并不是TC图形的头文件，请注意\n//要正确编译本程序，请先为你的VC或者C-Free安装好PEX\n加了包含这个头文件后，就可以使用图形函数了\n#include \"graphics.h\"\n\nint main() //请使用int声明main，作为规范\n{\n    //图形窗口初始化为640*480大小\n    initgraph(640, 480);\n\n    //设置字体高度为20，宽度为默认值的宋体字\n    setfont(20, 0, \"宋体\");\n\n    //在x=100,y=0的地方开始，显示一段文字\n    outtextxy(100, 0, \"Hello World\");\n\n    //等待用户按键，相当于暂停，注意这是图形库的函数\n    getch();\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        setfont(20, 0, "宋体");
        outtextxy(100, 0, "Hello World");
        {
            setfont(12, 0, "宋体");
            setcolor(0x808080);
            line(320, 0, 320, 480);
            setcolor(0xFFFFFF);
            outtextrect(320, 100, 320, 380, str);
            outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");
        }
        getch();
        return new SceneHelloWorld2(m_parent);
    }

private:
    SceneBase* m_parent;
};

class SceneForLoop9 : public SceneBase
{
public:
    SceneForLoop9(SceneBase* parent)
    {
        m_parent = parent;
        img = newimage();
    }

    ~SceneForLoop9()
    {
        delimage(img);
    }

    void smain()
    {
        int y, x;
        setbkcolor(DARKGRAY);
        for (y = 0; y < 8; ++y)
        {
            for (x = 0; x < 8; ++x)
            {
                setfillcolor(((x^y)&1) ? BLACK : WHITE );
                bar(x * 30, y * 30, (x + 1) * 30, (y + 1) * 30);
            }
        }
        info();
    }

    void info()
    {
        if (getwidth(img) <= 1)
        {
            char str[] = "#include \"graphics.h\"\nint main()\n{\
\n    initgraph(640, 480);\
\n    int y, x;\
\n    setbkcolor(DARKGRAY);\
\n    for (y = 0; y < 8; ++y)\
\n    {\
\n        for (x = 0; x < 8; ++x)\
\n        {\
\n            setfillcolor(((x^y)&1) ? BLACK : WHITE );\
\n            bar(x * 30, y * 30,\
\n                (x+1) * 30, (y+1) * 30));\
\n        }\
\n    }\
\n    getch();\n    return 0;\n}";
            resize(img, 320, 480);
            setfont(12, 0, "宋体", img);
            setbkmode(TRANSPARENT, img);
            setcolor(0x808080, img);
            line(0, 0, 0, 480, img);
            setcolor(0xFFFFFF, img);
            outtextrect(0, 50, 320, 480, str, img);
            outtextrect(0, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子", img);
        }
        putimage(320, 0, img);
    }

    SceneBase* Update()
    {
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        while (kbhit())
        {
            getch();
        }
        smain();
        getch();
        return m_parent;
    }

private:
    SceneBase* m_parent;
    PIMAGE img;
};

class SceneForLoop8 : public SceneBase
{
public:
    SceneForLoop8(SceneBase* parent)
    {
        m_parent = parent;
        img = newimage();
    }

    ~SceneForLoop8()
    {
        delimage(img);
    }

    void smain()
    {
        int y, x;
        setbkcolor(DARKGRAY);
        for (y = 0; y < 8; ++y)
        {
            for (x = 0; x < 8; ++x)
            {
                setfillstyle(SOLID_FILL, ((x^y)&1) ? BLACK : WHITE );
                bar(x * 30, y * 30, (x + 1) * 30, (y + 1) * 30);
            }
        }
        info();
    }

    void info()
    {
        if (getwidth(img) <= 1)
        {
            char str[] = "#include \"graphics.h\"\nint main()\n{\
\n    initgraph(640, 480);\
\n    int y, x;\
\n    setbkcolor(DARKGRAY);\
\n    for (y = 0; y < 8; ++y)\
\n    {\
\n        for (x = 0; x < 8; ++x)\
\n        {\
\n            setfillstyle(SOLID_FILL, ((x^y)&1) ? BLACK : WHITE );\
\n            bar(x * 30, y * 30,\
\n                (x+1) * 30, (y+1) * 30));\
\n        }\
\n    }\
\n    getch();\n    return 0;\n}";
            resize(img, 320, 480);
            setfont(12, 0, "宋体", img);
            setbkmode(TRANSPARENT, img);
            setcolor(0x808080, img);
            line(0, 0, 0, 480, img);
            setcolor(0xFFFFFF, img);
            outtextrect(0, 50, 320, 480, str, img);
            outtextrect(0, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子", img);
        }
        putimage(320, 0, img);
    }

    SceneBase* Update()
    {
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        while (kbhit())
        {
            getch();
        }
        smain();
        getch();
        return m_parent;
    }

private:
    SceneBase* m_parent;
    PIMAGE img;
};

class SceneForLoop7 : public SceneBase
{
public:
    SceneForLoop7(SceneBase* parent)
    {
        m_parent = parent;
        img = newimage();
    }

    ~SceneForLoop7()
    {
        delimage(img);
    }

    void smain()
    {
        int y;
        for (y = 0; y < 360; ++y)
        {
            setcolor(HSVtoRGB((float)y, 1.0f, 1.0f));
            line(0, y, 200, y);
        }
        info();
    }

    void info()
    {
        if (getwidth(img) <= 1)
        {
            char str[] = "#include \"graphics.h\"\nint main()\n{\
\n    initgraph(640, 480);\
\n    {\
\n        int y;\
\n        for (y = 0; y < 360; ++y)\
\n        {\
\n            setcolor(HSVtoRGB((float)y, 1.0f, 1.0f));\
\n            line(0, y, 200, y);\
\n        }\
\n    }\n    getch();\n    return 0;\n}";
            resize(img, 320, 480);
            setfont(12, 0, "宋体", img);
            setbkmode(TRANSPARENT, img);
            setcolor(0x808080, img);
            line(0, 0, 0, 480, img);
            setcolor(0xFFFFFF, img);
            outtextrect(0, 50, 320, 480, str, img);
            outtextrect(0, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子", img);
        }
        putimage(320, 0, img);
    }

    SceneBase* Update()
    {
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        while (kbhit())
        {
            getch();
        }
        smain();
        getch();
        return new SceneForLoop8(m_parent);
    }

private:
    SceneBase* m_parent;
    PIMAGE img;
};

class SceneForLoop6 : public SceneBase
{
public:
    SceneForLoop6(SceneBase* parent)
    {
        m_parent = parent;
        img = newimage();
    }

    ~SceneForLoop6()
    {
        delimage(img);
    }

    void smain()
    {
        int x = 0, dx = 1, color = 0;
        for (; kbhit() == 0; delay_fps(60))
        {
            cleardevice();
            if (x >= 320)
            {
                dx = -1;
            }
            else if (x <= 0)
            {
                dx = 1;
            }
            x += dx;
            color += 1;
            if (color >= 360)
            {
                color = 0;
            }
            setcolor(HSVtoRGB((float)color, 1.0f, 1.0f));
            circle(x, 100, 100);
            info();
        }
    }

    void info()
    {
        if (getwidth(img) <= 1)
        {
            char str[] = "#include \"graphics.h\"\nint main()\n{\
\n    initgraph(640, 480);\
\n    int x = 0, dx = 1, color = 0; //x表示圆的横坐标，dx表示速度方向\
\n    //动画主循环，kbhit()检测当前有没有按键，有就退出\
\n    //delay_fps(60)控制这个循环每秒循环60次\
\n    for (; kbhit() == 0; delay_fps(60))\
\n    {\
\n        cleardevice();\
\n        //根据x来调整dx的符号\
\n        if (x >= 320)\
\n        {\
\n            dx = -1;\
\n        }\
\n        else if (x <= 0)\
\n        {\
\n            dx = 1;\
\n        }\
\n        //通过对dx的控制，间接控制x的增减方向\
\n        x += dx;\
\n        color += 1;\
\n        if (color >= 360)\
\n        {\
\n            color = 0;\
\n        }\
\n        //使用HSV方式指定颜色\
\n        //关于HSV的介绍见图形库文档或者Google\
\n        setcolor(HSVtoRGB((float)color, 1.0f, 1.0f));\
\n        circle(x, 100, 100);\n    }\n    getch();\n    return 0;\n}";
            resize(img, 320, 480);
            setfont(12, 0, "宋体", img);
            setbkmode(TRANSPARENT, img);
            setcolor(0x808080, img);
            line(0, 0, 0, 480, img);
            setcolor(0xFFFFFF, img);
            outtextrect(0, 50, 320, 480, str, img);
            outtextrect(0, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子", img);
        }
        putimage(320, 0, img);
    }

    SceneBase* Update()
    {
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        while (kbhit())
        {
            getch();
        }
        smain();
        getch();
        return new SceneForLoop7(m_parent);
    }

private:
    SceneBase* m_parent;
    PIMAGE img;
};

class SceneForLoop5 : public SceneBase
{
public:
    SceneForLoop5(SceneBase* parent)
    {
        m_parent = parent;
        img = newimage();
    }

    ~SceneForLoop5()
    {
        delimage(img);
    }

    void smain()
    {
        int x = 0, dx = 1;
        for (; kbhit() == 0; delay_fps(60))
        {
            cleardevice();
            if (x >= 320)
            {
                dx = -1;
            }
            else if (x <= 0)
            {
                dx = 1;
            }
            x += dx;
            setcolor(0xFF0080);
            circle(x, 100, 100);
            info();
        }
    }

    void info()
    {
        if (getwidth(img) <= 1)
        {
            char str[] = "#include \"graphics.h\"\n\nint main()\n{\
\n    initgraph(640, 480);\
\n    int x = 0, dx = 1; //x表示圆的横坐标，dx表示速度方向\
\n    //动画主循环，kbhit()检测当前有没有按键，有就退出\
\n    //delay_fps(60)控制这个循环每秒循环60次\
\n    for (; kbhit() == 0; delay_fps(60))\
\n    {\
\n        //清屏\
\n        cleardevice();\
\n        //根据x来调整dx的符号\
\n        if (x >= 320)\
\n        {\
\n            dx = -1;\
\n        }\
\n        else if (x <= 0)\
\n        {\
\n            dx = 1;\
\n        }\
\n        //通过对dx的控制，间接控制x的增减方向\
\n        x += dx;\
\n        //使用RGB分量方式指定颜色\
\n        //红色为80，绿为0，蓝为FF\
\n        setcolor(0xFF0080);\
\n        circle(x, 100, 100);\n    }\n    getch();\n    return 0;\n}";
            resize(img, 320, 480);
            setfont(12, 0, "宋体", img);
            setbkmode(TRANSPARENT, img);
            setcolor(0x808080, img);
            line(0, 0, 0, 480, img);
            setcolor(0xFFFFFF, img);
            outtextrect(0, 50, 320, 480, str, img);
            outtextrect(0, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子", img);
        }
        putimage(320, 0, img);
    }

    SceneBase* Update()
    {
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        while (kbhit())
        {
            getch();
        }
        smain();
        getch();
        return new SceneForLoop6(m_parent);
    }

private:
    SceneBase* m_parent;
    PIMAGE img;
};

class SceneForLoop4 : public SceneBase
{
public:
    SceneForLoop4(SceneBase* parent)
    {
        m_parent = parent;
    }

    void smain()
    {
        for (int n = 0; n < 320; n++)
        {
            double x = ((double)n - 160) / 20;
            double y = sin(x);
            y = -y * 80 + 240;
            putpixel(n, (int)y, WHITE);
        }
        line(0, 240, 320, 240);
        line(160, 0, 160, 480);
    }

    SceneBase* Update()
    {
        char str[] = "#include \"graphics.h\"\n\nint main()\n{\n    initgraph(640, 480);\n    int n; //声明变量x\n    //变量x从0到320，取出每个横坐标\
\n    for (int n = 0; n < 320; n++)\
\n    {\
\n        //映射到-8到8的浮点数范围\
\n        double x = ((double)n - 160) / 20;\
\n        //计算对应的y\
\n        double y = sin(x);\
\n        //把y映射回屏幕坐标\
\n        y = -y * 80 + 240;\
\n        //画出这个点\
\n        putpixel(n, (int)y, WHITE);\
\n    }\
\n    //画坐标轴\
\n    line(0, 240, 320, 240);\
\n    line(160, 0, 160, 480);\
\n    getch();\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);

        smain();
        setfont(12, 0, "宋体");
        setcolor(0x808080);
        line(320, 0, 320, 480);
        setcolor(0xFFFFFF);
        outtextrect(320, 100, 320, 380, str);
        outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");

        getch();
        return new SceneForLoop5(m_parent);
    }

private:
    SceneBase* m_parent;
};

class SceneForLoop3 : public SceneBase
{
public:
    SceneForLoop3(SceneBase* parent)
    {
        m_parent = parent;
    }

    void smain()
    {
        for (int n = 0; n < 320; n++)
        {
            double x = ((double)n - 160) / 80;
            double y = x * x;
            y = -y * 80 + 240;
            putpixel(n, (int)y, WHITE);
        }
        line(0, 240, 320, 240);
        line(160, 0, 160, 480);
    }

    SceneBase* Update()
    {
        char str[] = "#include \"graphics.h\"\n\nint main()\n{\n    initgraph(640, 480);\n    int n; //声明变量x\n    //变量x从0到320，取出每个横坐标\
\n    for (int n = 0; n < 320; n++)\
\n    {\
\n        //映射到-2到2的浮点数范围\
\n        double x = ((double)n - 160) / 80;\
\n        //计算对应的y\
\n        double y = x * x;\
\n        //把y映射回屏幕坐标\
\n        y = -y * 80 + 240;\
\n        //画出这个点\
\n        putpixel(n, (int)y, WHITE);\
\n    }\
\n    //画坐标轴\
\n    line(0, 240, 320, 240);\
\n    line(160, 0, 160, 480);\
\n    getch();\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        smain();

        setfont(12, 0, "宋体");
        setcolor(0x808080);
        line(320, 0, 320, 480);
        setcolor(0xFFFFFF);
        outtextrect(320, 100, 320, 380, str);
        outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");

        getch();
        return new SceneForLoop4(m_parent);
    }

private:
    SceneBase* m_parent;
};

class SceneForLoop2 : public SceneBase
{
public:
    SceneForLoop2(SceneBase* parent)
    {
        m_parent = parent;
    }

    void smain()
    {
        for (int x = 100; x < 300; x += 3)
        {
            putpixel(x, 100, GREEN);
        }
    }

    SceneBase* Update()
    {
        char str[] = "#include \"graphics.h\"\n\nint main()\n{\n    initgraph(640, 480);\n    int x; //声明变量x\n    //变量x从100到300，步长为3，这样画出虚线\n    for (x = 100; x < 300; x += 3)\n    {\n        //在y=100的地方画绿点，多个连续点构成线\n        putpixel(x, 100, GREEN);\n    }\n    getch();\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        smain();

        setfont(12, 0, "宋体");
        setcolor(0x808080);
        line(320, 0, 320, 480);
        setcolor(0xFFFFFF);
        outtextrect(320, 100, 320, 380, str);
        outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");

        getch();
        return new SceneForLoop3(m_parent);
    }

private:
    SceneBase* m_parent;
};

class SceneForLoop : public SceneBase
{
public:
    SceneForLoop(SceneBase* parent)
    {
        m_parent = parent;
    }

    void smain()
    {
        for (int x = 100; x < 300; x++)
        {
            putpixel(x, 100, RED);
        }
    }

    SceneBase* Update()
    {
        char str[] = "#include \"graphics.h\"\n\nint main()\n{\n    initgraph(640, 480);\n    int x; //声明变量x\n    //变量x从100到300\n    for (x = 100; x < 300; x++)\n    {\n        //在y=100的地方画红点，多个连续点构成线\n        putpixel(x, 100, RED);\n    }\n    getch();\n    return 0;\n}";
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);

        smain();
        setfont(12, 0, "宋体");
        setcolor(0x808080);
        line(320, 0, 320, 480);
        setcolor(0xFFFFFF);
        outtextrect(320, 100, 320, 380, str);
        outtextrect(320, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子");

        getch();
        return new SceneForLoop2(m_parent);
    }
private:
    SceneBase* m_parent;
};

class SceneArray2 : public SceneBase
{
public:
    SceneArray2(SceneBase* parent)
    {
        m_parent = parent;
        m_dline = 0;
        m_resettext = 1;
        img = newimage();
    }

    ~SceneArray2()
    {
        delimage(img);
    }

    void mydelay(int ms)
    {
        int nms = 0;
        for ( ; nms < ms; delay_ms(50), nms += 50)
        {
            while (kbhit())
            {
                int key = getch();
                if ( (key & KEYMSG_DOWN) == 0) continue;
                key &= 0xFFFF;
                if (key == 'W' || key == VK_UP)
                {
                    m_dline += 1;
                    m_resettext = 1;
                }
                else if ((key == 'S' || key == VK_DOWN) && m_dline > 0)
                {
                    m_dline -= 1;
                    m_resettext = 1;
                }
            }
            info();
        }
    }

    void display(int arr[], int n, int i)
    {
        int a;
        cleardevice();
        for (a = 0; a < n; ++a)
        {
            setcolor(WHITE);
            setfillstyle(SOLID_FILL, HSLtoRGB(120.0f, 1.0f, (float)(arr[a] / 32.0)));
            fillellipse(100, 20 * a + 30, 9, 9);
        }
        if (i >= 0)
        {
            setfillstyle(SOLID_FILL, HSLtoRGB(120.0f, 1.0f, 1.0f));
            fillellipse(80, 20 * i + 30, 9, 9);
            fillellipse(80, 20 * (i + 1) + 30, 9, 9);
        }
        mydelay(500);
    }

    void smain()
    {
        int arr[20];
        int a, b;
        randomize();
        for (a = 0; a < 20; ++a)
        {
            arr[a] = random(32);
        }
        display(arr, 20, -1);
        setfont(12, 0, "宋体");
        outtextxy(0, 0, "请按任意键开始演示冒泡排序");
        info();
        getch();
        cleardevice();
        for (b = 20; b > 0; --b)
        {
            for (a = 1; a < b; ++a)
            {
                if ( arr[a] < arr[a-1])
                {
                    int t = arr[a];
                    arr[a] = arr[a-1];
                    arr[a-1] = t;
                }
                display(arr, 20, a-1);
            }
        }
        outtextxy(0, 0, "排序完成");
    }

    void info()
    {
        if (m_resettext)
        {
            char str[] = "#include \"graphics.h\"\n#include <stdio.h>\n#include <time.h>\n#include <string.h>\
\nvoid display(int arr[], int n)\
\n{\
\n    int a;\
\n    cleardevice();\
\n    for (a = 0; a < n; ++a)\
\n    {\
\n        setcolor(WHITE);\
\n        setfillstyle(SOLID_FILL, HSLtoRGB(120.0f, 1.0f, (float)(arr[a] / 32.0)));\
\n        fillellipse(100, 20 * a, 9, 9);\
\n    }\
\n    if (i >= 0)\
\n    {\
\n        setfillstyle(SOLID_FILL, HSLtoRGB(120.0f, 1.0f, 1.0f)));\
\n        fillellipse(80, 20 * i + 30, 9, 9);\
\n        fillellipse(80, 20 * (i + 1) + 30, 9, 9);\
\n    }\
\n    delay(500);\
\n}\
\nint main()\
\n{\
\n    int arr[20];\
\n    int a, b;\
\n    initgraph(640, 480);\
\n    randomize();\
\n    for (a = 0; a < 20; ++a)\
\n    {\
\n        arr[a] = random(32);\
\n    }\
\n    display(arr, 20);\
\n    setfont(12, 0, \"宋体\");\
\n    outtextxy(0, 0, \"请按任意键开始演示\");\
\n    getch();\
\n    cleardevice();\
\n    for (b = 20; b > 0; --b)\
\n    {\
\n        for (a = 1; a < b; ++a)\
\n        {\
\n            if ( arr[a] < arr[a-1])\
\n            {\
\n                int t = arr[a];\
\n                arr[a] = arr[b];\
\n                arr[b] = t;\
\n            }\
\n            display(arr, 20, a-1);\
\n        }\
\n    }\
\n    outtextxy(0, 0, \"排序完成\");\
\n    return 0;\
\n}\
";
            m_resettext = 0;
            resize(img, 320, 480);
            setfont(12, 0, "宋体", img);
            setbkmode(TRANSPARENT, img);
            setcolor(0x808080, img);
            line(0, 0, 0, 480, img);
            setcolor(0xFFFFFF, img);
            outtextrect(0, 50 - m_dline * 12, 320, 2048, str, img);
            outtextrect(0, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子", img);
        }
        putimage(320, 0, img);
    }

    SceneBase* Update()
    {
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        while (kbhit())
        {
            getch();
        }
        smain();
        getch();
        return m_parent;
    }

private:
    SceneBase* m_parent;
    PIMAGE img;
    int m_dline;
    int m_resettext;
};

class SceneArray : public SceneBase
{
public:
    SceneArray(SceneBase* parent)
    {
        m_parent = parent;
        img = newimage();
    }

    ~SceneArray()
    {
        delimage(img);
    }

    void smain()
    {
        int t = clock();
        char str[100];
        for (; kbhit() == 0; delay_fps(60))
        {
            cleardevice();
            sprintf(str, "经过时间%d", clock() - t);
            setfont(36, 0, "幼圆");
            outtextxy(0, 0, str);
            info();
        }
    }

    void info()
    {
        if (getwidth(img) <= 1)
        {
            char str[] = "#include \"graphics.h\"\n#include <stdio.h>\n#include <time.h>\n#include <string.h>\nint main()\n{\
\n    initgraph(640, 480);\
\n    {\
\n        int t = clock(); //记录超始时间\
\n        char str[100];\
\n        for (; kbhit() == 0; delay_fps(60))\
\n        {\
\n            cleardevice();\
\n            //把clock()-t的结果输出到字符串str\
\n            //实现简单的计时，可扩展成秒表\
\n            sprintf(str, \"经过时间%d\", clock() - t;\
\n            setfont(36, 0, \"幼圆\");\
\n            outtextxy(0, 0, str);\
\n        }\
\n    }\
\n    getch();\n    return 0;\n}";
            resize(img, 320, 480);
            setfont(12, 0, "宋体", img);
            setbkmode(TRANSPARENT, img);
            setcolor(0x808080, img);
            line(0, 0, 0, 480, img);
            setcolor(0xFFFFFF, img);
            outtextrect(0, 50, 320, 480, str, img);
            outtextrect(0, 0, 320, 400, "左半边是程序运行结果，下面是相应的源代码\n按任意键查看下一个例子", img);
        }
        putimage(320, 0, img);
    }

    SceneBase* Update()
    {
        setbkcolor_f(BLACK);
        cleardevice();
        setcolor(LIGHTGRAY);
        while (kbhit())
        {
            getch();
        }
        smain();
        getch();
        return new SceneArray2(m_parent);
    }

private:
    SceneBase* m_parent;
    PIMAGE img;
};

class SceneMenu : public SceneBase
{
public:
    SceneMenu()
    {
        memset(m_strlist, 0, sizeof(m_strlist));
        strcpy(m_strlist[0], "1.如果我刚学会Hello World");
    }

    SceneBase* Update()
    {
        setbkcolor_f(0x808080);
        cleardevice();
        setcolor(0xFFFFFF);
        setfont(24, 0, "宋体");
        outtextrect(100, 200, 500, 500, "1.如果我刚刚学会Hello World\n2.如果我刚刚学会循环和分支\n3.如果我刚刚学会数组和字符串\n（更多内容有待添加）\n");
        outtextxy(100, 100, "请按数字键选你要看的内容");
        int k;
        while (1)
        {
            k = getch();
            if (k == '1')
            {
                return new SceneHelloWorld(new SceneMenu);
            }
            else if (k == '2')
            {
                return new SceneForLoop(new SceneMenu);
            }
            else if (k == '3')
            {
                return new SceneArray(new SceneMenu);
            }
        }
    }

private:
    int m_x, m_y;
    int m_highlight;
    char m_strlist[100][32];
};

class SceneIntroduce : public SceneBase
{
public:
    SceneIntroduce()
    {
        memset(m_str, 0, sizeof(m_str));
        // 此处没有直接用宽字符串字面量初始化 m_str, 因为 VC6 会转出乱码
        const char* str = "你是刚刚学习Ｃ语言的新手吗？你是不是觉得单纯的字符输出有点无聊？Ｃ语言只能做这些吗？能不能做更有趣的？比如写游戏？\r\n本演示程序就是为了给你解开这个疑惑，本程序将带你进入精彩的Ｃ语言图形世界！不管你现在的C是刚刚开始学，还是学了一段时间，只要你有VC或者C-Free，都可以享受这个图形的盛宴。。。\r\n在正式开始前，请你百度“EGE”，下载并按里面的说明文档安装好。如果安装时遇到什么困难，可以加QQ群1060223135说明你的情况，会有人协助你解决的。\r\n（请按任意键继续）\r\n";
        MultiByteToWideChar(getcodepage(), 0, str, -1, m_str, 1024);

        // 理想状态:
        // wcscpy(m_str, L"你是刚刚学习Ｃ语言的新手吗？...");
    }

    SceneBase* Update()
    {
        wchar_t str[1024] = {0};
        int len = 0;
        setfont(20, 0, "宋体");
        for (len = 0 ; len<=0x80; delay_fps(60))
        {
            setbkcolor_f(EGERGB(len, len, len));
            cleardevice();
            ++len;
        }

        for (len = 0 ; m_str[len]; delay_fps(30))
        {
            wcsncpy(str, m_str, len);
            len += 1;
            cleardevice();
            setcolor(0x0);
            outtextrect(102, 101, 440, 480, str);
            setcolor(0xFFFFFF);
            outtextrect(100, 100, 440, 480, str);
        }
        getch();
        PIMAGE img = newimage();
        getimage(img, 0, 0, 640, 480);
        for (len = 255 ; len>=0; delay_fps(60))
        {
            cleardevice();
            putimage_alphablend(NULL, img, 0, 0, len);
            len -= 3;
        }
        return new SceneMenu;
    }

private:
    wchar_t m_str[1024];
};

int main()
{
    setcodepage(EGE_CODEPAGE_UTF8);
    initgraph(640, 480);
    SceneBase* scene = new SceneIntroduce; //SceneIntroduce; SceneMenu
    setbkmode(TRANSPARENT);

    for (SceneBase* newscene = scene; newscene != NULL; delay_fps(60))
    {
        newscene = scene->Update();
        if (newscene != scene)
        {
            delete scene;
            scene = newscene;
        }
    }

    closegraph();
    return 0;
}

