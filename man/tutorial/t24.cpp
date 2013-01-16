//基础动画四，半透明淡入淡出
#include <graphics.h>

void mainloop()
{
	// x,y记录当前位置，dx,dy记录速度的方向，r记录圆半径
	int x = 0, y = 0, dx = 1, dy = 1, r = 100;
	// alpha记录当前alpha值，da为alpha变化增量
	int alpha = 0, da = 1;

	// 需要借助img实现透明半透明
	PIMAGE img;

	// img 创建为 w=r*2, h=r*2大小
	img = newimage(r * 2, r * 2);

	setcolor(0x00FF00, img);
	setfillcolor(0xFF0000, img);
	
	// 画在img上
	fillellipse(r, r, r, r, img);

	setbkcolor(DARKGRAY);

	for ( ; is_run(); delay_fps(60) )
	{
		// todo: 逻辑更新
		// 当前位置 + 速度
		x += dx;
		y += dy;
		if (x < 0) dx = 1; //碰左
		if (y < 0) dy = 1; //碰上
		if (x >= ege::getwidth()  - r * 2) dx = -1; //碰右
		if (y >= ege::getheight() - r * 2) dy = -1; //碰下

		// 改变alpha值，参数范围为 0 ~ 0xFF(255)
		alpha += da;
		if (alpha <= 0) da = 1;
		if (alpha >= 0xFF) da = -1;

		// todo: 图形更新
		cleardevice();
		putimage_alphatransparent(NULL, img, x, y, BLACK, (unsigned char)alpha);
	}

	// 释放img
	delimage(img);
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
