//高级：更多的文字对齐方式设置
#include <graphics.h>

int main()
{
	initgraph(640, 480);

	setfont(18, 0, "宋体");
	//settextjustify(LEFT_TEXT, TOP_TEXT); //默认值
	outtextxy(300, 0, "Hello Graphics");

	settextjustify(RIGHT_TEXT, TOP_TEXT);
	outtextxy(300, 50, "Hello Graphics");

	settextjustify(CENTER_TEXT, TOP_TEXT);
	outtextxy(300, 100, "Hello Graphics");

	settextjustify(LEFT_TEXT, BOTTOM_TEXT);
	outtextxy(300, 50, "Hello Graphics");

	getch();

	closegraph();
	return 0;
}
