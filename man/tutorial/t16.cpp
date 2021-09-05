//用户交互——字符串数据输入* （非重点）
#include <graphics.h>

#include <stdio.h>

int main()
{
	initgraph(640, 480);

	//用来接收输入
	char str[100];
	//调用对话框函数
	inputbox_getline("请输入", "你爱输入什么就输入什么（回车确认）", str, 100);

	//显示输入的内容
	outtextxy(0, 0, str);

	getch();

	closegraph();
	return 0;
}
