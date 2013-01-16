//基础动画一：规范框架
#include <graphics.h>

void mainloop()
{
	// is_run 检测程序是否收到关闭消息，收到的话会返回false，即应该退出程序
	// delay_fps 控制帧率，60表示“平均延时”为1000/60毫秒
	// 为什么使用60？因为60是显示器的默认刷新率，99.9%的显示器都是这个设置
	for ( ; is_run(); delay_fps(60) )
	{
		// todo: 逻辑更新
		//

		// todo: 图形更新
		// 清屏
		cleardevice();
	}
}

int main(void)
{
	// 各种初始化
	// setinitmode设置初始化模式：
	// INIT_DEFAULT ege默认配置
	// INIT_NOFORCEEXIT 点了右上角X的时候不强制退出程序，由程序本身检测和控制结束
	// INIT_RENDERMANUAL 与以下的setrendermode(RENDER_MANUAL)等效
	setinitmode(INIT_DEFAULT|INIT_NOFORCEEXIT);
	// 图形初始化，窗口尺寸640x480
	initgraph(640, 480);
	// 随机数初始化，如果需要使用随机数的话
	randomize();
	// 绘图更新模式设置为手动，仅调用delay_fps/delay_ms等函数时才刷新，避免闪烁
	// 区别是在RENDER_AUTO模式下，任何绘图操作进行时都可能刷新，甚至刚cleardevice就显示
	// 导致屏幕上有时仅显示一个背景而没有其它内容导致闪烁
	setrendermode(RENDER_MANUAL);
	// 程序主循环
	mainloop();
	// 关闭绘图设备
	closegraph();
	return 0;
}
