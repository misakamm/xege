//基础动画三，更丰富的变化与帧率

#include <graphics.h>

#include <stdio.h>

void mainloop()
{
	//x,y记录当前位置，dx,dy记录速度向量，r记录圆半径
	int x = 0, y = 0, dx = 1, dy = 1, r = 100;

	for ( ; is_run(); delay_fps(60) )
	{
		// todo: 逻辑更新
		// 当前位置 + 速度
		x += dx;
		y += dy;
		if (x < r) dx = 1; //碰左
		if (y < r) dy = 1; //碰上
		if (x >= ege::getwidth()  - r) dx = -1; //碰右
		if (y >= ege::getheight() - r) dy = -1; //碰下

		// todo: 图形更新
		cleardevice();
		setcolor(GREEN);
		setfillcolor(BLUE);
		fillellipse(x, y, r, r);
		{// 画帧率文字
			char str[20];
			sprintf(str, "fps %.02f", getfps()); //调用getfps取得当前帧率
			setcolor(WHITE);
			outtextxy(0, 0, str);
		}
	}
}

int main(void)
{
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
