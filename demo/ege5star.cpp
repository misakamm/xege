////////////////////////////////////////
/// @file ege5star.cpp
/// @brief ege基础动画 demo，实现五角星旋转动画
///
/// 1. 实现动画框架。
/// 2. 根据角度绘制五角星。
///
/// @date 2023-07-07 (创建日期: 2020-10-25)
///
////////////////////////////////////////

#include <graphics.h>
#include <time.h>
#include <math.h>

const double rotatingSpeed = -0.03; ///< 旋转速度（单位：弧度/秒）
const double fullCircleRatation = PI * 2; ///< 完整圆周角度（单位：弧度）
const double starAngle = PI * 4 / 5; ///< 五角星角度

/**
  @brief 绘制五角星
  @param x 中心点 x 坐标
  @param y 中心点 y 坐标
  @param r 半径
  @param a 旋转角度
 */
void paintstar(double x, double y, double r, double a)
{
	int pt[10];
	for (int n = 0; n < 5; ++n)
	{
		pt[n * 2] = (int)(-cos(starAngle * n + a) * r + x);
		pt[n * 2 + 1] = (int)(sin(starAngle * n + a) * r + y);
	}
	fillpoly(5, pt);
}
int main()
{
	initgraph(640, 480); // 初始化绘图窗口
	setrendermode(RENDER_MANUAL);
	double r = 0;
	for (; is_run(); delay_fps(60))
	{
		r += rotatingSpeed;
		if (r > fullCircleRatation)
			r -= fullCircleRatation;

		cleardevice(); // 清空屏幕
		setcolor(EGERGB(0xff, 0xff, 0xff)); // 设置绘制颜色
		setfillcolor(EGERGB(0, 0, 0xff));
		paintstar(300, 200, 100, r); // 绘制五角星
	}
	return 0;
}
