#include<graphics.h>
#include<time.h>

int main() {
    int i;
    float x=0,y=0,r,u=0;
    initgraph( 640,480 );
    srand( time( 0 ) );
    for ( i=0; i<=500000; i++ ) {
        r=rand()%100;

        if ( r < 1 ) {
            x = 0;
            y = .16 * y;
        }
        if ( r>=1 && r<86 ) {
            u = .85 * x + .04 * y;
            y = -.04*x + .85 * y + 1.6;
            x = u;
        }
        if ( r>=86 && r<97 ) {
            u = .2 * x - .26 * y;
            y = .23 * x + .22 * y + 1.6;
            x = u;
        }
        if ( r>=97 ) {
            u = -.15 * x + .28 * y;
            y = .26 * x + .24 * y +.44;
            x = u;
        }
        putpixel( 45*x+230,480-45*y,RGB( rand()%256,rand()%256,rand()%256 ) );
    }
    getch();
    closegraph();
    return 0;
}

