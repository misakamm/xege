/////////////////////////////////////////
// 程序名：弹幕
// 编译环境：visual C++ / EGE库
/////////////////////////////////////////
#include <graphics.h>
#include <math.h>
#define MaxNum 500
#define PI 3.1415926
#define Mid_X 320
#define Mid_Y 240
/////////////////////////////////////////
// 定义类、常量
/////////////////////////////////////////

// 子弹节点
struct point
{
    double x, y;
    double vx, vy;
};
// 一个发射类
class object
{
    point p[MaxNum];
    int angle, speed, num; // 3个变量分别为初始角度、离心速度、当前已经发射的数目
public:
    PIMAGE img;
    object (int initAngle = 0);
    ~object () {closegraph ();}
    void initial (int x0, int y0);
    void draw ();
    void update ();
};
// 构造函数
object::object(int initAngle) : angle(initAngle), speed(5), num(0)
{
    img = newimage();
    initial (320, 240);
    // 下面3行用来画一个圆，并保存为一个贴图
    setfillcolor (WHITE);
    fillellipse (10, 10, 10, 10);
    getimage (img,0,0,20,20);
}
// 初始化点
void object::initial (int x0, int y0)
{
    // 初始化所有点
    for (int i=0; i<MaxNum; i++)
    {
        p[i].x      = x0;
        p[i].y      = y0;
        p[i].vx      =    cos ( angle * PI /180 ) * speed;
        p[i].vy      = -sin ( angle * PI /180 ) * speed;
        // 每次生成一个点，增加10度
        angle      += 10;
        if (angle >=360) angle=0;
    }
}
// 画出所有节点
void object::draw ()
{
    int i = 0;
    while (i<MaxNum)
    {
        if ( p[i].x>0 && p[i].x<640 && p[i].y>0 && p[i].y<480 )
            // 这个是EGE库的函数，是将前面那个获取的贴图，用透明贴图的方式画出来
            putimage_transparent (NULL, img, (int)p[i].x, (int)p[i].y, BLACK); 
        i++;
    }
}
// 更新所有节点坐标
void object::update ()
{
    // 已经发射的点必须每次都更新坐标
    for (int i=0; i<=num; i++)
    {
        p[i].x += p[i].vx;
        p[i].y += p[i].vy;
    }
    // 更新了坐标之后得继续发射下一个点
    num++;
    if (num >= MaxNum-1 )
    {
        // 如果500个点都发射完，那就重新初始化
        num %= (MaxNum-1);
        initial (320, 240);
    }
}

int main ()
{
    setinitmode(INIT_DEFAULT|INIT_RENDERMANUAL|INIT_NOFORCEEXIT);
    initgraph (640, 480);
    object o(120); // 从120度开始发射
    for (; is_run(); delay_fps(60))
    {
        cleardevice ();
        o.draw ();
        o.update ();
    }
    return 0;
}

