//基础动画七，使用非整数参数，随机色彩，使你的动画更自然
#include <graphics.h>

//定义一个AniObj，即前一例里面的圆，相关属性写在这个结构体里
struct AniObj
{
	float x, y;
	int r;
	float dx, dy;
	int alpha, da;
	PIMAGE img;
};

//自定义函数，用来返回一个0 - m之间的浮点数
float myrand(float m)
{
	// randomf() 返回一个 0 到 1 之间的随机浮点数
	return (float)(randomf() * m);
}

//定义速度基准值，base_speed是最低速度，base_speed+randspeed是最高速度
const float base_speed = 0.5f;
const float randspeed = 1.5f;

//初始化，设置坐标，速度方向，透明度，创建IMAGE等
void initobj(AniObj* obj)
{
	obj->x = myrand((float)getwidth());
	obj->y = myrand((float)getheight());
	obj->r = random(20) + 20;
	// 随机速度
	obj->dx = myrand(randspeed) + base_speed;
	obj->dy = myrand(randspeed) + base_speed;
	// 随机方向
	obj->dx = obj->dx * float(random(2) * 2.0 - 1);
	obj->dy = obj->dy * float(random(2) * 2.0 - 1);
	// 以上的处理方式用来保证速度不过慢

	obj->alpha = random(250) + 2;
	obj->da = (int)random(2) * 2 - 1;

	obj->img = newimage(obj->r * 2, obj->r * 2);

	color_t col = hsv2rgb(myrand(360.0f), 1.0f, 1.0f);
	setcolor(col, obj->img);
	setfillcolor(col, obj->img);

	fillellipse(obj->r, obj->r, obj->r, obj->r, obj->img);
}

//更新位置等相关属性
void updateobj(AniObj* obj)
{
	// 当前位置 + 速度
	obj->x += obj->dx;
	obj->y += obj->dy;
	if (obj->x < 0) obj->dx = myrand(randspeed) + base_speed; //碰左
	if (obj->y < 0) obj->dy = myrand(randspeed) + base_speed; //碰上
	if (obj->x >= ege::getwidth()  - obj->r * 2) obj->dx = -(myrand(randspeed) + base_speed); //碰右
	if (obj->y >= ege::getheight() - obj->r * 2) obj->dy = -(myrand(randspeed) + base_speed); //碰下

	// 改变alpha值
	obj->alpha += obj->da;
	if (obj->alpha <= 0) obj->da = 1;
	if (obj->alpha >= 0xFF) obj->da = -1;
}

//根据属性值绘画
void drawobj(AniObj* obj)
{
	putimage_alphatransparent(NULL, obj->img, (int)obj->x, (int)obj->y, BLACK, (unsigned char)obj->alpha);
}

//释放这个对象时调用
void releaseobj(AniObj* obj)
{
	delimage(obj->img);
}

void mainloop()
{
	const int MAXOBJ = 30;
	AniObj obj[MAXOBJ]; //定义对象数组
	int n;

	for (n = 0; n < MAXOBJ; ++n)
	{
		initobj(&obj[n]); //初始化
	}

	for ( ; is_run(); delay_fps(60) )
	{
		for (n = 0; n < MAXOBJ; ++n)
		{
			updateobj(&obj[n]); //更新位置
		}

		cleardevice();
		for (n = 0; n < MAXOBJ; ++n)
		{
			drawobj(&obj[n]); //绘画
		}
	}

	for (n = 0; n < MAXOBJ; ++n)
	{
		releaseobj(&obj[n]); //释放
	}
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
