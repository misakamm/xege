//基础动画九，使用场景，更灵活地控制动画

//本课要讲如何控制动画，动画的播放，暂停
//我们把动画封装在一个场景，由场景类来控制动画
//当然，现在开始就不是单纯的动画，还加入了用户交互的时候，接口就要增加了
//本示例运行时，可以使用P键暂停或播放
#include <ege.h>

const float base_speed = 0.5f;
const float randspeed = 1.5f;

//自定义函数，用来返回一个0 - m之间的浮点数
float myrand(float m)
{
	return (float)(ege::randomf() * m);
}

//定义一个AniObj类，这个与前一个除了函数名，和update加了返回值以外没有变化
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

	//释放这个对象时调用
	~AniObj()
	{
		ege::delimage(_img);
	}

	//更新位置等相关属性
	int update()
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
		return 0;
	}

	//根据属性值绘画
	void render()
	{
		ege::putimage_alphatransparent(NULL, _img, (int)_x, (int)_y, ege::BLACK, (unsigned char)_alpha);
	}

	void onkey(int key)
	{
		//
	}

private:
	float _x, _y;
	int _r;
	float _dx, _dy;
	int _alpha, _da;
	ege::PIMAGE _img;
};

class Scene
{
public:
	//初始化，参数为对象个数
	Scene(int nAniObj)
	{
		m_cntObj = nAniObj;
		m_pobj = new AniObj[m_cntObj];
		m_pause = 0;
		m_endscene = 0;
	}
	~Scene()
	{
		delete [] m_pobj;
	}
	int update()
	{
		// 非暂停状态才更新
		if (m_pause == 0)
		{
			for (int n = 0; n < m_cntObj; ++n)
			{
				m_pobj[n].update();
			}
		}
		return m_endscene;
	}
	void render()
	{
		for (int n = 0; n < m_cntObj; ++n)
		{
			m_pobj[n].render();
		}
	}
	void onkey(int key)
	{
		if (key == 'P' || key == 'p') //按下P键就在播放与暂停之间转换
		{
			m_pause = !m_pause;
		}
		if (key == VK_ESCAPE) //如果是ESC键，就标记为退出场景，VK_ESCAPE是SDK定义的宏
		{
			m_endscene = 1;
		}
	}
private:
	AniObj* m_pobj;
	int m_cntObj;
	int m_pause;
	int m_endscene;
};

void mainloop()
{
	Scene scene(30); //定义场景，初始化参数为30

	for ( ; ege::is_run(); ege::delay_fps(60) )
	{
		while (ege::kbhit())
		{
			int key = ege::getch();
			scene.onkey(key); //所有按键消息发送给scene
		}
		if (scene.update()) //如果update返回非0表示场景结束，这时退出主循环
		{
			break;
		}

		ege::cleardevice();
		scene.render();
	}
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

//后记，scene对象里，已经有自己的对象列表，如果做成复杂一点的结构，
//还可以动态控制对象的个数，按对象的属性在更高层控制这个对象，
//比如已经越出屏幕就回收这个对象，单独控制一个对象的显隐等（当然显隐也可以由那个对象自己控制，但场景可以强制让其不显示而不由对象本身控制）


//后记，为什么要定义场景？除了为了更好地控制动画外，
//不同场景可以有不同的动画，不同的用户响应，
//比如一个开场动画场景，菜单场景，正式游戏场景，这样分场景管理起来，
//比全部挤在一个主循环管理全部对象，会清晰很多
//这样你就可以轻松实现出专业级别的动画或者游戏了，
//不至于从头到尾只看到一个正式游戏界面那样单纯枯燥了
