//基础动画二：简单平移动画
#include <graphics.h>

void mainloop()
{
	// 动画控制变量，控制横坐标，初始值为0
	int x = 0;

	setcolor(EGERGB(0, 0xFF, 0));
	setfillcolor(EGERGB(0, 0, 0xFF));

	for ( ; is_run(); delay_fps(60) )
	{
		// todo: 逻辑更新
		//计算新坐标，右移一个像素，如果等于440则重新移回x=0，达到动画循环
		x = ( x + 1 ) % 440;

		// todo: 图形更新
		//清屏，重新在新的位置绘图图像
		cleardevice();
		//以x为圆的左边界绘画，为什么是左边界？因为圆心坐标是 (x + 半径) 了
		fillellipse(x + 100, 200, 100, 100);
	}
}

int main(void)
{
	//INIT_ANIMATION相当于INIT_NOFORCEEXIT|INIT_DEFAULT|INIT_RENDERMANUAL
	//下面就不需要再多一步setrendermode
	setinitmode(INIT_ANIMATION);
	// 图形初始化，窗口尺寸640x480
	initgraph(640, 480);
	// 随机数初始化，如果需要使用随机数的话
	randomize();
	// 程序主循环
	mainloop();
	// 关闭绘图设备
	closegraph();
	return 0;
}
