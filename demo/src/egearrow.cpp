#include <graphics.h>
#include <math.h>

void draw_arrow(float sx, float sy,
                float ex, float ey,
                float r, float len) {
    float c = cos(r), s = sin(r);
    float dx = sx - ex, dy = sy - ey;
    ege_line(sx, sy, ex, ey);
    ege_point points[3] = {
        ex, ey,
        len * (dx * c + dy * s) + ex,
        len * (-dx * s + dy * c) + ey,
        len * (dx * c - dy * s) + ex,
        len * (dx * s + dy * c) + ey
    };
    ege_fillpoly(3, points);
}

int main( void ) {
    initgraph(640, 480);
    ege_enable_aa(true);
    setcolor(EGEARGB(0xff, 0xff, 0xff, 0xff));
    setfillcolor(EGEARGB(0xff, 0xff, 0x0, 0xff));
    setlinewidth(2.0f);
    draw_arrow(100.0f, 100.0f, 300.0f, 150.0f,
            (float)(PI/8), 0.2f);
    getch();
    return 0;
}

