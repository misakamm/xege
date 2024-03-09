// 第一个画图程序
#include <graphics.h>

int main()
{
    initgraph(640, 480);

    // 设置画图颜色，GREEN是颜色常数，详细可以查graphics.h对这个宏的定义的值
    setcolor(GREEN);

    // 画一直线，从(100,100)到(500,200)
    // 特别注意的是端点，(100,100)这个点会被画上颜色，但(500,200)不会画上颜色
    // 以下所有的矩形（或者说两点）描述方式，总是前一个点取得到，但后一个点取不到
    line(100, 100, 500, 200);

    getch();

    closegraph();
    return 0;
}
