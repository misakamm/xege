//第一个图形窗口程序
#include <graphics.h>

int main()
{
	//初始化为640*480大小
	initgraph(640, 480);

	//等待用户按键
	getch();

	//关闭图形界面
	closegraph();
	return 0;
}
