//基础动画五，对象封装，使代码更有条理
#include <graphics.h>

//定义一个AniObj，即前一例里面的圆，相关属性写在这个结构体里
struct AniObj
{
	int x, y, r;
	int dx, dy;
	int alpha, da;
	PIMAGE img;
};

//初始化，设置坐标，速度方向，透明度，创建IMAGE等
void initobj(AniObj* obj)
{
	obj->x = 0;
	obj->y = 0;
	obj->r = 100;
	obj->dx = 1;
	obj->dy = 1;
	obj->alpha = 0;
	obj->da = 1;

	// 初始化img
	obj->img = newimage(obj->r * 2, obj->r * 2);

	setcolor(0x00FF00, obj->img);
	setfillcolor(0xFF0000, obj->img);

	fillellipse(obj->r, obj->r, obj->r, obj->r, obj->img);
}

//更新位置等相关属性
void updateobj(AniObj* obj)
{
	// 当前位置 + 速度
	obj->x += obj->dx;
	obj->y += obj->dy;
	if (obj->x < 0) obj->dx = 1; //碰左
	if (obj->y < 0) obj->dy = 1; //碰上
	if (obj->x >= ege::getwidth()  - obj->r * 2) obj->dx = -1; //碰右
	if (obj->y >= ege::getheight() - obj->r * 2) obj->dy = -1; //碰下

	// 改变alpha值
	obj->alpha += obj->da;
	if (obj->alpha <= 0) obj->da = 1;
	if (obj->alpha >= 0xFF) obj->da = -1;
}

//根据属性值绘画
void drawobj(AniObj* obj)
{
	putimage_alphatransparent(NULL, obj->img, obj->x, obj->y, BLACK, (unsigned char)obj->alpha);
}

//释放这个对象时调用
void releaseobj(AniObj* obj)
{
	delimage(obj->img);
}

void mainloop()
{
	AniObj obj; //声明AniObj对象
	initobj(&obj); //初始化

	for ( ; is_run(); delay_fps(60) )
	{
		// todo: 逻辑更新
		updateobj(&obj); //更新位置

		// todo: 图形更新
		cleardevice();
		drawobj(&obj); //绘画
	}

	releaseobj(&obj); //释放
}

int main(void)
{
	setinitmode(INIT_ANIMATION);
	// 图形初始化，窗口尺寸640x480
	initgraph(640, 480);
	// 随机数初始化，如果需要使用随机数的话
	randomize();
	// 程序主循环
	mainloop();
	// 关闭绘图设备
	closegraph();
	return 0;
}
