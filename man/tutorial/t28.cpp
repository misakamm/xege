//基础动画八，使用C++封装你的对象

//看本文章前，你都看了前几篇了吧？觉不觉得似乎有一个固定模式？
//如果你有这个感觉，说明你真的看懂了
//封装一个图形对象，四大必要接口函数：初始化，逻辑更新，绘画（渲染），释放（析构）
//函数少了就绝对不正确，接口函数多了就说明你的封装可能还需要调整
//一般的应用下，就这四个接口就足够了，最少也必须有这四个，请你一定要记得
//另：不是说这个类只有这四个函数，可以有更多的函数方便你自己使用，但对外（对主循环函数）暴露使用的应该就这四个

// 从本文开始，改用此头文件，调用函数时的区别是需要ege这个namespace
// 这样的好处是vc的自动提示可以更精确，阅读代码的时候可读性更好，缺点是编写的时候略麻烦一些
#include <ege.h>

const float base_speed = 0.5f;
const float randspeed = 1.5f;

//自定义函数，用来返回一个0 - m之间的浮点数
float myrand(float m)
{
	return (float)(ege::randomf() * m);
}

//定义一个AniObj类
class AniObj
{
public:
	//初始化，设置坐标，速度方向，透明度，创建IMAGE等
	AniObj()
	{
		_x = myrand((float)ege::getwidth());
		_y = myrand((float)ege::getheight());
		_r = ege::random(20) + 20;
		_dx = myrand(randspeed) + base_speed;
		_dy = myrand(randspeed) + base_speed;
		_dx = _dx * float(ege::random(2) * 2.0 - 1);
		_dy = _dy * float(ege::random(2) * 2.0 - 1);
		_alpha = ege::random(250) + 2;
		_da = ege::random(2) * 2 - 1;

		_img = ege::newimage(_r * 2, _r * 2);

		color_t col = ege::hsv2rgb(myrand(360.0f), 1.0f, 1.0f);
		ege::setcolor(col, _img);
		ege::setfillcolor(col, _img);

		ege::fillellipse(_r, _r, _r, _r, _img);
	}

	//更新位置等相关属性
	void update()
	{
		// 当前位置 + 速度
		_x += _dx;
		_y += _dy;
		if (_x < 0) _dx = myrand(randspeed) + base_speed; //碰左
		if (_y < 0) _dy = myrand(randspeed) + base_speed; //碰上
		if (_x >= ege::getwidth()  - _r * 2) _dx = -(myrand(randspeed) + base_speed); //碰右
		if (_y >= ege::getheight() - _r * 2) _dy = -(myrand(randspeed) + base_speed); //碰下

		// 改变alpha值
		_alpha += _da;
		if (_alpha <= 0)	_da = 1;
		if (_alpha >= 0xFF) _da = -1;
	}

	//根据属性值绘画
	void render()
	{
		ege::putimage_alphatransparent(NULL, _img, (int)_x, (int)_y, ege::BLACK, (unsigned char)_alpha);
	}

	//释放这个对象时调用
	~AniObj()
	{
		ege::delimage(_img);
	}

private:
	float _x, _y;
	int _r;
	float _dx, _dy;
	int _alpha, _da;
	ege::PIMAGE _img;
};


void mainloop()
{
	const int MAXOBJ = 30;
	AniObj obj[MAXOBJ]; //定义对象数组
	int n;

	//为什么这里没有obj初始化？因为在类对象声明的时候就调用了构造函数初始化好了

	for ( ; ege::is_run(); ege::delay_fps(60) )
	{
		for (n = 0; n < MAXOBJ; ++n)
		{
			obj[n].update(); //更新位置
		}

		ege::cleardevice();
		for (n = 0; n < MAXOBJ; ++n)
		{
			obj[n].render(); //绘画
		}
	}

	//为什么这里没有释放？因为obj是数组对象，会在本过程结束后自动调用释构函数
}

int main(void)
{
	ege::setinitmode(ege::INIT_ANIMATION);
	// 图形初始化，窗口尺寸640x480
	ege::initgraph(640, 480);
	// 随机数初始化，如果需要使用随机数的话
	ege::randomize();
	// 程序主循环
	mainloop();
	// 关闭绘图设备
	ege::closegraph();
	return 0;
}
