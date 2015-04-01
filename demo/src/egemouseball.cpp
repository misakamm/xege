#include <graphics.h>
#include <math.h>

#define LEN 640
#define WID 480
#define MAXBALL 10

class BALL
{
    public:
        BALL()
        {
            r = random(20) + 20;
            x = random(LEN - r * 2) + r;
            y = random(WID - r * 2) + r;
            vx = (6 * randomf() + 0.1f) * (random(2) * 2.0 - 1);
            vy = (6 * randomf() + 0.1f) * (random(2) * 2.0 - 1);
            color = HSVtoRGB(randomf() * 360.0f, 1.0f, 0.8f);

            float a = randomf();//随机下每个对象的摩擦系数

            fa = 1 / (100.0);
            goon = 1;
        }

        void drawball()
        {
            setfillcolor(color);
            setcolor(color);
            fillellipse((int)x, (int)y, (int)r, (int)r);
        }

        void update()
        {

            if(x - r <= 0)
            {
                x = r;
                if ( vx <= 0)
                    vx = -vx;
            }
            if (x + r >= LEN)
            {
                x = LEN - r;
                if (vx >= 0)
                    vx = -vx;
            }
            if(y - r <= 0)
            {
                y = r;
                if (vy <= 0)
                    vy = -vy;
            }
            if (y + r >= WID)
            {
                y = WID - r;
                if (vy >= 0)
                    vy = -vy;
            }
            if (goon)
            {
                x += vx;
                y += vy;

                double fv = sqrt((double)vx * vx + (double)vy * vy);
                if (fv > 1e-9)
                {
                    vx = vx - vx * fa / fv;
                    vy = vy - vy * fa / fv;
                }
                else
                {
                    vx = 0;
                    vy = 0;
                }
            }
        }
        bool isCrash(int _x, int _y)
        {
            double dx = _x - x, dy = _y - y;
            return dx * dx + dy * dy < r * r;
        }

        bool resmouse(mouse_msg mouse, double dx, double dy)
        {
            double f = 0.9;
            if(mouse.msg == mouse_msg_down)
            {
                if( isCrash(mouse.x, mouse.y) )//如果鼠标的位置在圆内。。
                {
                    vx = 0;
                    vy = 0;
                    x = mouse.x;
                    y = mouse.y;
                    goon = 0;
                    return 1;
                }
            }
            else if (mouse.msg == mouse_msg_up)
            {
                goon = 1;
                return 0;
            }
            else if (mouse.msg == mouse_msg_move)
            {
                if (dx*dx + dy*dy > vx*vx + vy*vy)
                {
                    vx = dx;
                    vy = dy;
                }
                else
                {
                    vx *= f;
                    vy *= f;
                }
                x = mouse.x;
                y = mouse.y;
                goon = 0;
                return 1;
            }
            else if (! goon)
            {
                vx *= f;
                vy *= f;
            }
            return 0;
        }


    private:
        int r;
        float x, y;
        float vx, vy;//速度分量
        int color;
        float ax, ay;//加速度分量
        float fa;//摩擦
        bool goon;//标志
};

void dealMouse(BALL* ball)
{
    static int iCapture = -1, mx, my;
    int i;
    mouse_msg mouse = {0};
    while (mousemsg())
    {
        double dx, dy;
        mouse = getmouse();
        dx = (mouse.x - mx) * 1.0f;
        dy = (mouse.y - my) * 1.0f;
        mx = mouse.x;
        my = mouse.y;

        if(iCapture == -1 && mouse.msg == mouse_msg_down)
        {
            for(i = MAXBALL - 1; i > -1; --i)
                if(ball[i].resmouse(mouse, dx, dy))
                {
                    iCapture = i;
                    break;
                }
        }
        else if (iCapture >= 0 && (mouse.msg == mouse_msg_up || mouse.msg == mouse_msg_move) )
        {
            if(ball[iCapture].resmouse(mouse, dx, dy) == 0)
                iCapture = -1;
        }
    }
    if (iCapture >= 0)
    {
        mouse.msg = (mouse_msg_e)0;
        ball[iCapture].resmouse(mouse, 0, 0);
    }
}

int main(void)
{
    {
        setinitmode(INIT_ANIMATION);
        initgraph(LEN, WID);
        setcaption("碰撞小球");
        randomize();
    }
    int i;

    BALL ball[MAXBALL];

    for ( ; is_run(); delay_fps(60))
    {
        dealMouse(ball);

        for(i = 0; i < MAXBALL; ++i)
            ball[i].update();

        cleardevice();

        for(i = 0; i < MAXBALL; ++i)
            ball[i].drawball();

    }

    closegraph();
    return 0;
}

