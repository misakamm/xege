//高级：绘画渐变色/彩虹
#include <graphics.h>

// alpha 混合计算函数
color_t alphasingle(color_t a, color_t b, color_t alpha)
{
	return (a * (0xFF - alpha) + b * alpha) >> 8;
}

// 颜色alpha混合计算函数
color_t alphacol(color_t d, color_t s, color_t alpha)
{
	return (alphasingle(d&0xFF00FF, s&0xFF00FF, alpha) & 0xFF00FF)
		| (alphasingle(d&0xFF00, s&0xFF00, alpha) >> 8 << 8);
}

int main()
{
	initgraph(640, 480);
	int c;

	//画渐变色
	for (c=0; c<256; ++c)
	{
		color_t a = 0xFF0000, b = 0x0000FF;
		setcolor(alphacol(a, b, c));
		line(0, c, 200, c);
	}

	//画彩虹
	for (c=0; c<360; ++c)
	{
		setcolor(hsl2rgb((float)c, 1.0f, 0.5f));
		line(200, c, 400, c);
	}
	getch();

	closegraph();
	return 0;
}
