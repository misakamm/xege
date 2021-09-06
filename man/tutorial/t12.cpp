//用户交互——键盘1
#include <graphics.h>

//这个例子需要这个头文件
#include <stdio.h>

int main()
{
	initgraph(640, 480);

	setfont(18, 0, "宋体");
	outtextxy(0, 0, "please press any key");

	int k = 0;
	for ( ; k != key_esc; ) // key_esc是ege定义的按键常数
	{
		char str[32];
		//等待用户按键，并把得到的按键给k
		//如果你不想等待，可以在调用getch之前，使用kbhit检测是否有按键按下
		//如 if ( kbhit() ) k = getch();
		k = getch();

		//格式化输出为字符串，用于后面输出
		sprintf(str, "%c %3d", k, k);

		cleardevice();
		outtextxy(0, 0, str);
	}

	closegraph();
	return 0;
}
