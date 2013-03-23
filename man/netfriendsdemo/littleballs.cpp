#include <graphics.h>
#include <cmath>
#define SCR_WIDTH      800
#define SCR_HEIGHT     600

struct Ball {
    Ball() {
        x = 400;
        y= 300;
        randomize();
        sp += 0.01f;
        dx = /*(randomf() + 0.5)* pow(-1.0,random(10)) * */ sp * cosf( dir );
        dy = /*(randomf() + 0.5)* pow(-1.0,random(10)) * */ sp * sinf( dir );
        radius = randomf() * 5 + 2;
        color = random( 0xff0000 ) + 0xffff;
        dir += 0.1f;
    }
    static float dir;
    static float sp;
    float x,y;
    float dx,dy;
    float radius;
    color_t color;
} ball[500];

float Ball::dir = 0.0f;
float Ball::sp = 0.5f;
int main() {
    setinitmode(INIT_DEFAULT|INIT_RENDERMANUAL|INIT_NOFORCEEXIT);
    initgraph( SCR_WIDTH,SCR_HEIGHT );
    int n = 1;
    double t = fclock();
    for ( ; is_run(); delay_fps( 60 ) ) {
        if ( kbhit() ) {
            if ( getch() == 27 )
                break;
            n += 63;
            n %= 500;
        }
        if ( fclock() - t > 0.2 ) {
            t = fclock();
            if (n < 500) ++n;
        }
        for ( int i=0; i != n; ++i ) {
            setfillcolor( ball[i].color );
            setcolor( ball[i].color );
            fillellipse( ball[i].x,ball[i].y,ball[i].radius,ball[i].radius );
            ball[i].x +=   ball[i].dx;
            ball[i].y +=   ball[i].dy;
            if ( ball[i].x < 0.0f || ball[i].x > SCR_WIDTH ) ball[i].dx = -ball[i].dx;
            if ( ball[i].y < 0.0f || ball[i].y > SCR_HEIGHT ) ball[i].dy = -ball[i].dy;
        }
        imagefilter_blurring( NULL,0x70,0x100 );
    }
    closegraph();
    return 0;
}

