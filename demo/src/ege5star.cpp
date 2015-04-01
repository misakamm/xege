#include <graphics.h>
#include <time.h>
#include <math.h>

void paintstar(double x, double y, double r, double a)
{
    int pt[10];
    for (int n = 0; n < 5; ++n)
    {
        pt[n*2] = (int)( -cos( PI * 4 / 5 * n + a ) * r + x );
        pt[n*2+1] = (int)( sin( PI * 4 / 5 * n + a) * r + y );
    }
    fillpoly(5, pt);
}

int main()
{
    initgraph( 640, 480 );
    setcolor( RGB(0xff, 0xff, 0xff) );
    setfillcolor( RGB(0, 0, 0xff) );
    setrendermode(RENDER_MANUAL);
    double r = 0;
    for ( ; is_run(); delay_fps(60) )
    {
        r += 0.02;
        if (r > PI * 2) r -= PI * 2;

        cleardevice();
        paintstar(300, 200, 100, r);
    }
    return 0;
}

