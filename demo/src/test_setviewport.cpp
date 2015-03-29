#include "graphics.h"

#include <stdio.h>
#include <time.h>

int main()
{
    initgraph(400, 300);
    setfont(24, 12, "宋体");
    {
        char str[] = "滚动字幕示例，Hello EGE !! Welcome to graphics programming !!!!!!~~~~~~";
        int w = textwidth(str);        //记录下字幕的完整宽度
        int view_x = 100, view_w = 200; //设置可见区的位置和大小（只要x方向）
        int t = clock(), roll_time = 10000; //记录下起始时间，和滚动完所需要的时间
        setrendermode(RENDER_MANUAL);
        for ( ; ; delay(0))
        {
            int nt = clock(); //取得当前时间，nt-t就是时间差，(nt-t) / roll_time 就是当前时间应该滚动的比例
            cleardevice();
            if (nt - t > roll_time) //和总时间比较，如果已经完成，t=nt重新设置起始时间
            {
                t = nt;
            }
            else
            {
                //设置绘图区域，在这区域外的不会绘画，以达到裁剪的目的，同时，坐标(0,0)将变成从这区域里开始算
                setviewport(view_x, 100, view_x + view_w, 300);
                //以下这个表达式：view_w - (w + view_w) * (nt-t) / roll_time 需要详细解释一下
                //我们要从右向左，那么，view_w最右端就是基准点
                //w + view_w 是我们需要滚动的总长度，总长度乘以当前的滚动比例，以得到实际的位置
                outtextxy(view_w - (w + view_w) * (nt-t) / roll_time, 0, str);
                setviewport(0, 0, 400, 300); //还原绘图区
            }
        }
    }
    return 0;
}
