//色彩的计算
#include <graphics.h>

int main()
{
	initgraph(640, 480);

	//设置画图颜色，使用RGB宏，三个参数值的范围均为0 - 255 (0xFF)
	//分别表示红色亮度，绿色亮度，蓝色亮度
	//EGERGB(0xFF, 0x0, 0x0) (纯红色) 等价于0xFF0000
	setcolor(EGERGB(0xFF, 0x0, 0x0));

	//画一圆，圆心在(200,200)，半径100
	circle(200, 200, 100);

	getch();

	closegraph();
	return 0;
}
