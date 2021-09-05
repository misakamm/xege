//使用IMAGE——拉伸
#include <graphics.h>

int main()
{
	PIMAGE img;

	initgraph(640, 480);

	//先随便画一些东西
	setcolor(EGERGB(0xFF, 0xFF, 0x0));
	setfillcolor(EGERGB(0xFF, 0x0, 0x80));
	fillellipse(50, 50, 50, 50);

	img = newimage();
	getimage(img, 0, 0, 160, 120);

	int x, y;
	//把img平铺在屏幕上，使用一个二重循环
	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			//把img整个，拉伸画在指定的矩形里
			//需要指定目标矩形，源矩形
			//源矩形参数不能忽略，否则会和其它非拉伸功能的贴图函数混淆
			putimage(x * 80, y * 60, 80, 60, img, 0, 0, 160, 120);
		}
	}

	getch();

	delimage(img);

	closegraph();
	return 0;
}
