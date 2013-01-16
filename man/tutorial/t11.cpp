//使用IMAGE——透明，半透明
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

	//先画一个非黑色背景，以便于比较
	setbkcolor(EGERGB(0x80, 0x80, 0x80));
	cleardevice();

	//四种贴图比较
	putimage(0, 0, img);
	putimage_alphablend(NULL, img, 160, 0, 0x80); //半透明度为0x80
	putimage_transparent(NULL, img, 0, 80, BLACK);	//透明贴图，关键色为BLACK，源图为这个颜色的地方会被忽略
	putimage_alphatransparent(NULL, img, 160, 80, BLACK, 0xA0); //同时使用透明和半透明

	getch();

	delimage(img);

	closegraph();
	return 0;
}
