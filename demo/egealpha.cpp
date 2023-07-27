#include <graphics.h>
#include <math.h>

int main()
{
	/// 初始化绘图窗口
	initgraph(640, 480);
	
	// @note Step 1: 绘制背景
	setbkcolor(WHITE);
	setcolor(BLUE);
	setfillcolor(BLUE);
	bar(100, 100, 600, 400);
	
	// @note Step 2: 创建一个PIMAGE对象并设置背景透明度
	PIMAGE img = newimage(640, 480);
	setbkcolor(EGERGBA(0, 0, 0, 0), img);
	
	// @note Step 3: 绘制形状并设置透明度为255（不透明）
	setfillcolor(EGEACOLOR(255, RED), img);
	setcolor(EGEACOLOR(255, RED), img);
	setlinestyle(CENTER_LINE, 0, 1, img);
	setlinewidth(10, img);
	ege_line(100, 100, 400, 400, img);
	// 改变颜色
	setcolor(EGEACOLOR(255, GREEN), img);
	setlinestyle(DOTTED_LINE, 0, 1, img);
	setlinewidth(5, img);
	ege_ellipse(200, 100, 200, 200, img);
	// 设置填充颜色
	setfillcolor(EGEACOLOR(255, LIGHTMAGENTA), img);
	ege_fillellipse(10, 10, 50, 50, img);
	
	setfillcolor(EGEACOLOR(255, LIGHTBLUE), img);
	ege_fillellipse(100, 10, 50, 50, img);
	
	// 将图像绘制到目标图像上（为NULL则绘制到屏幕），保留透明度信息
	putimage_withalpha(NULL, img, 0, 0);
	
	// @note Step 4: 清除图像对象
	delimage(img);
	
	// @note 暂停程序，等待用户按下任意键继续
	getch();
	return 0;
}
