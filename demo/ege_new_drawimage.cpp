////////////////////////////////////////
/// ege绘图函数和PIMAGE使用 demo
/// @file ege_new_drawimage.cpp
/// @brief 实现绘制白底蓝条纹背景上有四个黄色三角形。
///
/// 1. 绘制白底蓝条纹背景。
/// 2. 绘制第一个三角形，先绘制到PIMAGE变量img上，然后绘制到屏幕。
/// 3. 绘制第二个三角形，将img的部分（实际上是完整的）绘制到屏幕。
/// 4. 绘制第三个三角形，通过变换矩阵将img旋转45度后绘制。
/// 5. 绘制第四个三角形，绘制还原后的img。
///
/// @date 2023.6.23 (创建日期: 2020.10.25)
///
///
////////////////////////////////////////

#include <graphics.h>

int main()
{
	/// 初始化绘图窗口
	initgraph(800, 600);

	// @note Step 1
	// @note 绘制背景
	setbkcolor(WHITE);
	setfillcolor(BLUE);
	for (int i = 0; i < 12; i++) {
		// @note 画无边框填充矩形
		bar(0, i * 50, 800, i * 50 + 20);
	}

	// @note Step 2
	// @note 创建一个PIMAGE对象
	PIMAGE img = newimage(200, 200);
	// @note 将背景设为透明色
	setbkcolor(EGERGBA(0, 0, 0, 0), img);
	setcolor(RED, img);
	setfillcolor(YELLOW, img);

	// @note 绘制一个多边形（示例使用三角形）
	ege_point points[4];
	points[0].x = 100;
	points[0].y = 50;
	points[1].x = 50;
	points[1].y = 150;
	points[2].x = 150;
	points[2].y = 150;
	// @note 最后一个点需要连回原点以构成封闭多边形，用于绘制边框
	points[3].x = 100;
	points[3].y = 50;
	// @note 绘制填充多边形到目标图像（为NULL则绘制到屏幕）并绘制边框
	ege_fillpoly(3, points, img);
	ege_drawpoly(4, points, img);
	// @note 将图像绘制到目标图像上（为NULL则绘制到屏幕），保留透明度信息
	putimage_withalpha(NULL, img, 0, 0);

	// @note Step 3
	// @note 将图像的指定部分绘制到屏幕，从(200,0)点开始绘制到屏幕，到(400,300)点结束
	ege_drawimage(img, 200, 0, 400, 300, 0, 0, 200, 200);

	// @note Step 4
	// @note 通过变换矩阵来绘制图像
	ege_transform_matrix m;
	ege_get_transform(&m);
	ege_transform_translate(400, 450);
	ege_transform_rotate(45.0);
	ege_transform_translate(-getwidth(img) / 2, -getheight(img) / 2);
	ege_drawimage(img, 0, 0);

	// @note Step 5
	// @note 还原图像
	ege_set_transform(&m);
	/*
	  @note 下面的语句可以清除矩阵：
	  ege_transform_reset();
	 */
	// @note 绘制到窗口
	ege_drawimage(img, 600, 400);

	// @note 暂停程序，等待用户按下任意键继续
	getch();
	delimage(img);
	closegraph();
	return 0;
}
