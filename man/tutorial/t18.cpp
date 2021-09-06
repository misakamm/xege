//高级：更多的颜色表示办法(HSV/HSL)
#include <graphics.h>

int main()
{
	initgraph(640, 480);

	//初始化随机数种子
	randomize();

	for (; ;)
	{
		//使用随机色彩，纯度为1.0，亮度为0.5
		//第一个参数范围从0 - 360，表示一个角度，这个颜色所在的HSL双头圆锥的方位
		//0是红色，120是绿色，240是蓝色，详见pdf文档内的HSL颜色空间的说明
		setfillcolor(hsl2rgb(float(randomf() * 360), 1.0f, 0.5f));

		bar(100,100, 300, 200);

		//按一下按键变化一次颜色，ESC退出
		if (getch() == 27) break;
	}

	closegraph();
	return 0;
}
