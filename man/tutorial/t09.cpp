//使用IMAGE——抓图，绘画到IMAGE，IMAGE到屏幕
#include <graphics.h>

//IMAGE对象相当于一个绘图板，而同时也可以作为图片画到其它IMAGE上
//它是一个非常灵活的对象，复杂的绘图都要借助IMAGE，以便更好地保存和输出图像

int main()
{
	//声明一个img图片对象
	PIMAGE img;

	initgraph(640, 480);

	//先随便画一些东西
	setcolor(EGERGB(0xFF, 0xFF, 0x0));
	setfillcolor(EGERGB(0xFF, 0x0, 0x80));
	fillellipse(50, 50, 50, 50);

	//用newimage在initgraph后创建这个对象。但记得要在不使用的时候delimage
	img = newimage();

	//从屏幕上截取(0, 0) - (80, 60)这部分作为img，这个img大小为80*60
	//img的尺寸会重设，大小由第三第四个参数决定
	//注意，(0,0)这点会被包含在img里，但(80, 60)不包含
	getimage(img, 0, 0, 80, 60);

	//对img设置填充色为绿色
	setfillcolor(EGERGB(0x0, 0x70, 0x0), img);

	//对img画实心矩形
	bar(40, 20, 70, 50, img);

	int x, y;
	//把img平铺在屏幕上，使用一个二重循环
	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			//把img整个，画在指定的坐标上，左上角对齐这个坐标
			putimage(x * 80, y * 60, img);
		}
	}

	getch();

	delimage(img);

	closegraph();
	return 0;
}
