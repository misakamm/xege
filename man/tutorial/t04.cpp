//颜色填充
#include <graphics.h>

int main()
{
	initgraph(640, 480);

	//设置填充颜色，一般为图形内部颜色
	setfillcolor(EGERGB(0xFF, 0x0, 0x80));

	//画一实心矩形，范围为：x坐标从50-300，y坐标从100-200
	bar(50, 100, 300, 200);

	getch();

	closegraph();
	return 0;
}
