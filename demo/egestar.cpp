// 星空屏保程序，请生成为scr后缀，或者手工改后缀，不要直接运行exe
#include "graphics.h"
#include <time.h>
#include <stdio.h>

#define MAXSTAR 2000 // 星星总数
int sc_width, sc_heigh; // 记录窗口宽高
int g_max;

struct STAR {
    double x;
    int y;
    double step;
    int color;
} star[MAXSTAR];

// 初始化星星
void InitStar( int i ) {
    double speed = 0.006;
    star[i].x = 0;
    star[i].y = random( sc_heigh );
    star[i].step = randomf() * speed * 0.9 + speed * 0.1;
    star[i].color = ( int )( star[i].step * 255 / speed + 0.5 ); // 速度越快，颜色越亮
    if ( star[i].color > 255 ) {
        star[i].color = 255;
    }
    star[i].color = RGB( star[i].color, star[i].color, star[i].color );
}

// 移动星星
void MoveStar( int i, double dt ) {
    // 擦掉原来的星星
    putpixel( ( int )( star[i].x * sc_width ), star[i].y, 0 );
    // 计算新位置
    star[i].x += star[i].step * dt * 60;
    if ( star[i].x > 1 ) InitStar( i );
    // 画新星星
    putpixel( ( int )( star[i].x * sc_width ), star[i].y, star[i].color );
}

int preinit( int argc, char* argv[] ) {
    setinitmode( INIT_NOBORDER | INIT_TOPMOST ); // 指定初始化为无边框顶层窗口，并且窗口左上角坐标为(0, 0)
    g_max = MAXSTAR;
    if ( argc < 2 ) {
        //MessageBoxA( NULL, "本屏幕保护程序无配置", "星空屏保", MB_OK );
        //return -1;
    } else if ( stricmp( argv[1], "/p" ) == 0 ) { // 小窗口预览模式
        HWND hwnd;
        sscanf( argv[2], "%d", &hwnd );
        attachHWND( hwnd ); // 新ege函数
        setinitmode( INIT_NOBORDER | INIT_CHILD | INIT_WITHLOGO ); // 指定初始化为无边框子窗口
        g_max = 200;
        return 1;
    } else if ( stricmp( argv[1], "/s" ) ) { // 非测试运行模式
        MessageBoxA( NULL, "本屏幕保护程序无配置", "星空屏保", MB_OK );
        return -1;
    }
    return 0; // 全屏模式
}

// 主函数
int main( int argc, char* argv[] ) {
    int i, ms_x = -1024, ms_y = -1024, exitflag = 0;
    int fps = 60;
    double dtime;

    int mode = preinit( argc, argv ); // 记录初始化模式
    if ( mode < 0 ) return 0;

    randomize(); // 初始化随机种子
    initgraph( -1, -1 ); // 打开图形窗口，以全屏模式

    showmouse( mode );
    sc_width = getwidth();
    sc_heigh = getheight();

    // 初始化所有星星
    for ( i = 0; i < g_max; i++ ) {
        InitStar( i );
        star[i].x = randomf();
    }
    // 绘制星空，按任意键或移动鼠标退出
    setfont( 12, 6, "宋体" );
    setrendermode( RENDER_MANUAL );
    dtime = fclock();
    while ( kbmsg() ) getkey();

    for ( ; !exitflag && is_run() && kbmsg() == 0; delay_fps( fps ) ) { //每秒画120帧，kbhit(1)是获取键盘任意键的消息，详见pdf
        // 如果有鼠标消息
        while ( mousemsg() ) {
            mouse_msg msg = getmouse();
            if ( ms_x <= -1024 ) {
                ms_x = msg.x;
                ms_y = msg.y;
            }
            // 处理鼠标，移动超出范围就退出
            if ( mode == 0 ) { // 仅全屏模式才处理鼠标
                int x = msg.x, y = msg.y;
                x -= ms_x;
                y -= ms_y;
                if ( x * x + y * y > 400 ) exitflag = 1;
            }
        }
        // 显示星星
        double dt = 1.0 / fps; //fclock() - dtime;
        dtime += dt;
        for ( int i = 0; i < g_max; i++ ) {
            MoveStar( i, dt );
        }
        // 显示FPS
        {
            char str[60];
            sprintf( str, "%8.2f FPS", getfps());
            outtextxy( 0, 0, str ); //显示fps
        }
    }
    closegraph(); // 关闭图形窗口
    return 0;
}

