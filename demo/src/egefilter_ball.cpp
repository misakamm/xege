// 弹球物理碰撞演示

#include <graphics.h>
#include <ege/fps.h>
#include <math.h>
#include <stdlib.h>
#include <locale>

#define SC_W 400
#define SC_H 300

#define myrand(m)                 ((float)(randomf() * (m)))
#define IsCrash(a, b)             ((a.x - b.x)*(a.x - b.x)+(a.y - b.y)*(a.y - b.y) < (a.r + b.r)*(a.r + b.r))
#define IsWEdge(a)                (a.x < a.r || a.x >= SC_W-a.r)
#define IsHEdge(a)                (a.y < a.r || a.y >= SC_H-a.r)
#define IsEdge(a)                 (IsWEdge(a) || IsHEdge(a))
#define Distance(x1, y1, x2, y2)  (((x1) - (x2))*((x1) - (x2)) + ((y1) - (y2))*((y1) - (y2)))


typedef struct {
    float x, y;
    float vx, vy;
    int r;
    int color;
} Obj;


//定义一个AniObj类
class AniObj {
public:
    AniObj() {
        n = 8;
        int i, j;
        bool goon;
        for ( i = 0; i < n; i++ ) {
            do {
                goon = false;
                obj[i].x = ( float )random( getwidth() );
                obj[i].y = ( float )random( getheight() );
                obj[i].r = random( 30 ) + 15;
                if ( IsEdge( obj[i] ) )
                    goon = true;
                else if ( i != 0 )
                    for ( j = i - 1; j >= 0; j-- )
                        if ( IsCrash( obj[i], obj[j] ) ) {
                            goon = true;
                            break;
                        }
            } while ( goon );
            obj[i].vx = obj[i].vy = 0.0f;
            obj[i].color = EGEAGRAY( 0xff, HSVtoRGB( myrand( 360.0f ), 1.0f, 1.0f ) );
        }
    }

    //更新位置等相关属性
    void updateobj() {
        int i, j;
        for ( i = 0; i < n; i++ ) {
            obj[i].vy += 0.05f;
            obj[i].x += obj[i].vx;
            obj[i].y += obj[i].vy;
            if ( obj[i].y >= SC_H - obj[i].r && obj[i].vy > 0 ) {
                obj[i].y -= obj[i].vy;
                obj[i].vy = - obj[i].vy;
            }
            if ( obj[i].x < obj[i].r && obj[i].vx < 0 ) obj[i].vx = - obj[i].vx;
            if ( obj[i].x >= SC_W - obj[i].r && obj[i].vx > 0 ) obj[i].vx = - obj[i].vx;
        }
        for ( i = 1; i < n; i++ )
            for ( j = i - 1; j >= 0; j-- )
                if ( IsCrash( obj[i], obj[j] ) &&
                        ( Distance( obj[i].x, obj[i].y, obj[j].x, obj[j].y ) >
                          Distance( obj[i].x + obj[i].vx, obj[i].y + obj[i].vy, obj[j].x + obj[j].vx, obj[j].y + obj[j].vy )
                        )
                   )
                    Crash( obj[i], obj[j] );
    }

    //根据属性值绘画
    void drawobj() {
        for ( int i = 0; i < n; i++ ) {
            setfillcolor( obj[i].color );
            ege_fillellipse( obj[i].x - obj[i].r, obj[i].y - obj[i].r,
                             obj[i].r * 2, obj[i].r * 2 );
        }
    }

    ~AniObj() {
    }

private:
    void Crash( Obj &a, Obj &b ) {
        float ma = a.r * a.r, mb = b.r * b.r;

        float sx = a.x - b.x;
        float sy = a.y - b.y;
        float s1x = sx / sqrt( sx*sx + sy*sy );
        float s1y = sy / sqrt( sx*sx + sy*sy );
        float t1x = -s1y;
        float t1y = s1x;

        float vas = a.vx * s1x + a.vy * s1y;
        float vat = a.vx * t1x + a.vy * t1y;
        float vbs = b.vx * s1x + b.vy * s1y;
        float vbt = b.vx * t1x + b.vy * t1y;

        float vasf = ( 2 * mb * vbs + vas * ( ma - mb ) ) / ( ma + mb );
        float vbsf = ( 2 * ma * vas - vbs * ( ma - mb ) ) / ( ma + mb );

        float nsx = vasf * s1x;
        float nsy = vasf * s1y;
        float ntx = vat * t1x;
        float nty = vat * t1y;

        a.vx = nsx + ntx;
        a.vy = nsy + nty;

        nsx = vbsf * s1x;
        nsy = vbsf * s1y;
        ntx = vbt * t1x;
        nty = vbt * t1y;

        b.vx = nsx + ntx;
        b.vy = nsy + nty;
    }

private:
    Obj obj[20];
    int n;
};

void draw( color_t* d_pt, color_t* s_pt, int w, int h ) {
    for ( int y = 0; y < h; ++y ) {
        color_t* dp = &d_pt[y * w * 4];
        color_t* sp = &s_pt[y * w];
        for ( int x = 0; x < w; ++x ) {
            color_t c = sp[x];
            dp[x * 2] = c;
            dp[x * 2 + 1] = c;
            dp[x * 2 + w*2] = c;
            dp[x * 2 + w*2 + 1] = c;
        }
    }
}

void draw_colorscan( color_t* d_pt, color_t* s_pt, int w, int h ) {
    for ( int y = 0; y < h; ++y ) {
        color_t* dp = &d_pt[y * w * 4];
        color_t* sp = &s_pt[y * w];
        for ( int x = 0; x < w; ++x ) {
            color_t c = sp[x];
            dp[x * 2] = c & 0xFF;
            dp[x * 2 + 1] = c & 0xFF00;
            dp[x * 2 + w*2] = c & 0xFF0000;
            dp[x * 2 + w*2 + 1] = 0;
        }
    }
}

void draw_colorscan2( color_t* d_pt, color_t* s_pt, int w, int h ) {
    for ( int y = 0; y < h; ++y ) {
        color_t* dp = &d_pt[y * w * 4];
        color_t* sp = &s_pt[y * w];
        color_t lc = 0;
        for ( int x = 0; x < w; ++x ) {
            color_t c = sp[x];
            color_t nc = sp[x + 1];
            if ( x + 1 == w ) nc = 0;
            dp[x * 2] = lc & 0xFF00FF;
            dp[x * 2 + 1] = c & 0xFF00;
            dp[x * 2 + w*2] = c & 0xFF00;
            dp[x * 2 + w*2 + 1] = nc & 0xFF00FF;
            lc = c;
        }
    }
}

void draw_colorscan3( color_t* d_pt, color_t* s_pt, int w, int h ) {
    for ( int y = 0; y < h; ++y ) {
        color_t* dp = &d_pt[y * w * 4];
        color_t* sp = &s_pt[y * w];
        color_t lc = 0;
        for ( int x = 0; x < w; ++x ) {
            color_t c = sp[x];
            color_t nc = sp[x + 1];
            if ( x + 1 == w ) nc = 0;
            dp[x * 2] = lc & 0xFF00FF | c & 0xFF00;
            dp[x * 2 + 1] = c;
            dp[x * 2 + w*2] = c;
            dp[x * 2 + w*2 + 1] = c & 0xFF00FF | nc & 0xFF00;
            lc = c;
        }
    }
}

void draw_smooth( color_t* d_pt, color_t* s_pt, int w, int h ) {
    for ( int y = 0; y < h; ++y ) {
        color_t* dp = &d_pt[y * w * 4];
        color_t* sp = &s_pt[y * w];
        color_t lc = 0;
        for ( int x = 0; x < w; ++x ) {
            color_t c = sp[x];
            color_t nc = sp[x + 1];
            color_t tc;
            if ( x + 1 == w ) nc = 0;
            dp[x * 2] = c;
            dp[x * 2 + 1] = tc = ( ( c & 0xFEFEFE ) >> 1 ) + ( ( nc & 0xFEFEFE ) >> 1 );
            dp[x * 2 + w*2] = c;
            dp[x * 2 + w*2 + 1] = tc;
            if ( y > 0 ) {
                dp[x * 2 - w*2] = ( ( dp[x * 2 - w*2] & 0xFEFEFE ) >> 1 ) + ( ( c & 0xFEFEFE ) >> 1 );
                dp[x * 2 - w*2 + 1] = ( ( dp[x * 2 - w*2 + 1] & 0xFEFEFE ) >> 1 ) + ( ( tc & 0xFEFEFE ) >> 1 );
            }
            lc = c;
        }
    }
}

void draw_scanline50( color_t* d_pt, color_t* s_pt, int w, int h ) {
    for ( int y = 0; y < h; ++y ) {
        color_t* dp = &d_pt[y * w * 4];
        color_t* sp = &s_pt[y * w];
        for ( int x = 0; x < w; ++x ) {
            color_t c = sp[x];
            dp[x * 2] = c;
            dp[x * 2 + 1] = c;
            dp[x * 2 + w*2] = 0;
            dp[x * 2 + w*2 + 1] = 0;
        }
    }
}

int main() {
    typedef void ( *filter_fun )( color_t* , color_t* , int , int );
    setinitmode( INIT_ANIMATION );
    initgraph( SC_W * 2, SC_H * 2 );
    randomize(); //初始化随机种子

    AniObj aniobj; //定义对象
    fps f;
    PIMAGE img = newimage( SC_W, SC_H );
    color_t* d_pt = ( color_t* )getbuffer( NULL );
    color_t* s_pt = ( color_t* )getbuffer( img );
    filter_fun f_list[] = {draw, draw_smooth,
                           draw_colorscan, draw_scanline50, draw_colorscan2, draw_colorscan3
                          };
    const char* str_list[] = {"soft_2x", "smooth",
                              "colorscan", "scanline 50%", "colorscan2", "colorscan3"
                             };

    int pause = 0;
    int i_fun = 0;

    for ( ; is_run(); delay_fps( 1000 ) ) {
        if ( kbhit() ) {
            int k = toupper( getch() );
            if ( k == 'P' ) pause = 1 - pause;
            if ( k == 'C' ) i_fun = ( 1 + i_fun ) % ( sizeof( f_list ) / sizeof( *f_list ) );
        }
        if ( pause == 0 ) {
            aniobj.updateobj(); //更新位置

            settarget( img );
            ege_enable_aa( true );
            //cleardevice();
            imagefilter_blurring( NULL, 0x40, 0xFF );
            aniobj.drawobj(); //绘画
        }
        settarget( NULL );
        f_list[i_fun]( d_pt, s_pt, SC_W, SC_H );
        outtextxy( 0, 16, str_list[i_fun] );
        //putimage(0, 0, img);
    }

    closegraph();
    return 0;
}

