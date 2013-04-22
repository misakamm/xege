//基础动画十，场景管理下的输入输出控制

//本示例运行时，可以使用P键暂停或播放
//小球会自动跟踪鼠标
#include <ege.h>

#include <math.h>
#include <stdlib.h>

const float base_speed = 0.5f;
const float randspeed = 4.0f;

//返回一个浮点数的符号
float sgn(float f)
{
	if (f > 0) return 1;
	if (f < 0) return -1;
	return 0;
}
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
		_mouse_x = (int)_x;
		_mouse_x = (int)_y;

		_img = ege::newimage(_r * 2, _r * 2);

		ege::color_t col = ege::hsv2rgb(myrand(360.0f), 1.0f, 1.0f);
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
		bool crash = false;
		// 当前位置 + 速度
		_x += _dx;
		_y += _dy;
		if (_x < 0) crash = true; //碰左
		if (_y < 0) crash = true; //碰上
		if (_x >= ege::getwidth()  - _r * 2) crash = true; //碰右
		if (_y >= ege::getheight() - _r * 2) crash = true; //碰下

		if (crash)
		{
			float r;
			_dx = _mouse_x - _x;
			_dy = _mouse_y - _y;
			r = sqrt(_dx * _dx + _dy * _dy);
			_dx = (_dx + sgn(_dx) * base_speed / randspeed) / r * randspeed;
			_dy = (_dy + sgn(_dx) * base_speed / randspeed) / r * randspeed;
		}

		if (_x < 0 && _dx < 0 || _x >= ege::getwidth()	- _r * 2 && _dx > 0) _dx = -_dx;
		if (_y < 0 && _dy < 0 || _y >= ege::getheight() - _r * 2 && _dy > 0) _dy = -_dy;

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

	void onmouse(int x, int y)
	{
		_mouse_x = x - _r;
		_mouse_y = y - _r;
	}

private:
	float _x, _y;
	int _r;
	float _dx, _dy;
	int _mouse_x, _mouse_y;
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
	void onmouse(int x, int y)
	{
		// 广播鼠标消息
		for (int n = 0; n < m_cntObj; ++n)
		{
			m_pobj[n].onmouse(x, y);
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
		while (ege::mousemsg())
		{
			ege::mouse_msg msg = ege::getmouse();
			scene.onmouse(msg.x, msg.y);
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
