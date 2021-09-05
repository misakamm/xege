//文字绘制与文字相关设置
#include <graphics.h>

int main()
{
	initgraph(640, 480);

	//先画一个矩形，以便于看出背景色的问题
	setfillcolor(EGERGB(0x0, 0x80, 0x80));
	bar(50, 50, 500, 200);

	//文字颜色
	setcolor(EGERGB(0x0, 0xFF, 0x0));

	//文字背景色（注意setbkcolor函数也会同时改变文字背景色）
	setfontbkcolor(EGERGB(0x80, 0x00, 0x80));

	//设置字体，第一个参数是字体的高度（像素），第二个参数是字体的宽度，第二个参数如果为0，就使用默认比例值
	//如果高度为12，即相当于小五号字，或者9磅字，实际的换算就自己完成吧
	setfont(12, 0, "宋体");

	//写文字，注意：outtextxy不支持\t \n这类格式化用的特殊字符，这类字符会被忽略
	//要使用特殊格式化字符请用outtextrect
	outtextxy(100, 100, "Hello EGE Graphics");
	outtextrect(100, 120, 200, 100, "\tHello EGE Graphics\nHere is the next line.");

	//设置文字背景填充方式为透明，默认为OPAQUE不透明
	setbkmode(TRANSPARENT);
	outtextxy(100, 150, "Hello EGE Graphics, 中文也是支持的");

	getch();

	closegraph();
	return 0;
}
