//用户交互——鼠标1
#include <graphics.h>

#include <stdio.h>

int main()
{
	initgraph(640, 480);

	setfont(18, 0, "宋体");

	int k = 0;

	// 这个循环，is_run判断窗口是否还在，delay_fps是延时
	// 后面讲动画的时候会详细讲解，现不要在此纠结
	for ( ; is_run(); delay_fps(60))
	{
		char str[32];
		int x, y;

		//获取鼠标坐标，此函数不等待。若鼠标移出了窗口，那么坐标值不会更新
		//特殊情况是，你按着鼠标键不放，拖出窗口，这样坐标值会依然更新
		mousepos(&x, &y);

		//格式化输出为字符串，用于后面输出
		sprintf(str, "%4d %4d", x, y);

		outtextxy(0, 0, str);
	}

	closegraph();
	return 0;
}
