// 弹球物理碰撞演示

#include <graphics.h>
#include <ege/fps.h>
#include <math.h>

#define myrand(m)                 ((float)random(10000) * m / 10000.0f)
#define IsCrash(a, b)             ((a.x - b.x)*(a.x - b.x)+(a.y - b.y)*(a.y - b.y) < (a.r + b.r)*(a.r + b.r))
#define IsWEdge(a)               (a.x < a.r || a.x >= 640-a.r)
#define IsHEdge(a)               (a.y < a.r || a.y >= 480-a.r)
#define IsEdge(a)                 (IsWEdge(a) || IsHEdge(a))
#define Distance(x1, y1, x2, y2) (((x1) - (x2))*((x1) - (x2)) + ((y1) - (y2))*((y1) - (y2)))


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
                obj[i].r = random( 40 ) + 20;
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
            if ( obj[i].y >= 480 - obj[i].r && obj[i].vy > 0 ) {
                obj[i].y -= obj[i].vy;
                obj[i].vy = - obj[i].vy;
            }
            if ( obj[i].x < obj[i].r && obj[i].vx < 0 ) obj[i].vx = - obj[i].vx;
            if ( obj[i].x >= 640 - obj[i].r && obj[i].vx > 0 ) obj[i].vx = - obj[i].vx;
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

int main() {
    setinitmode( INIT_ANIMATION );
    initgraph( 640, 480 );
    randomize(); //初始化随机种子

    AniObj aniobj; //定义对象
    fps f;
    ege_enable_aa( true );

    for ( ; is_run(); delay_fps( 120 ) ) {
        aniobj.updateobj(); //更新位置
        cleardevice();
        aniobj.drawobj(); //绘画
    }

    closegraph();
    return 0;
}

