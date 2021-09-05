//用户交互——键盘2
#include <graphics.h>

//这个例子需要这个头文件
#include <stdio.h>

int main()
{
	initgraph(640, 480);

	setfont(18, 0, "宋体");
	outtextxy(0, 0, "press any key");

	key_msg k = {0};
	for ( ; k.key != key_esc; ) // key_esc是ege定义的按键常数
	{
		char str[32];
		//等待用户按键，并把得到的按键给k
		//如果你不想等待，可以在调用getkey之前，使用kbmsg检测是否有按键按下
		//如 if ( kbmsg() ) k = getkey();
		k = getkey();

		//格式化输出为字符串，用于后面输出
		//msg参见key_msg_e, flag参见key_flag_e
		sprintf(str, "key= %c %3d %2x  msg=%d flag=%d", k.key, k.key, k.key, k.msg, k.flags);

		cleardevice();
		outtextxy(0, 0, str);
	}

	closegraph();
	return 0;
}
