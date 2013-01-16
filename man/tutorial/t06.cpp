//屏幕背景色
#include <graphics.h>

int main()
{
	initgraph(640, 480);

	//第一次设置背景色
	setbkcolor(EGERGB(0x0, 0x40, 0x0)); //浅绿色

	setcolor(EGERGB(0xFF, 0xFF, 0x0));
	setfillcolor(EGERGB(0xFF, 0x0, 0x80));

	//画带填充的扇形
	sector(200, 200, 45, 135, 150, 100);

	getch();

	//按下一任意键后，再次改变背景色
	setbkcolor(EGERGB(0x0, 0x0, 0x40)); //浅蓝色

	getch();

	closegraph();
	return 0;
}

//补充：另有setbkcolor_f函数，它设置的是cleardevice函数所使用的颜色，
//所以要等到cleardevice调用时，它的效果才会展现。
//当然，setbkcolor也会设置，但同时也会立即把屏幕上的原背景色改变

