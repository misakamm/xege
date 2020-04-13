//��������ʮ�����������µ������������

//��ʾ������ʱ������ʹ��P����ͣ�򲥷�
//С����Զ��������
#include <ege.h>

#include <math.h>
#include <stdlib.h>

const float base_speed = 0.5f;
const float randspeed = 4.0f;

//����һ���������ķ���
float sgn(float f)
{
	if (f > 0) return 1;
	if (f < 0) return -1;
	return 0;
}
//�Զ��庯������������һ��0 - m֮��ĸ�����
float myrand(float m)
{
	return (float)(ege::randomf() * m);
}

//����һ��AniObj�࣬�����ǰһ�����˺���������update���˷���ֵ����û�б仯
class AniObj
{
public:
	//��ʼ�����������꣬�ٶȷ���͸���ȣ�����IMAGE��
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

	//�ͷ��������ʱ����
	~AniObj()
	{
		ege::delimage(_img);
	}

	//����λ�õ��������
	int update()
	{
		bool crash = false;
		// ��ǰλ�� + �ٶ�
		_x += _dx;
		_y += _dy;
		if (_x < 0) crash = true; //����
		if (_y < 0) crash = true; //����
		if (_x >= ege::getwidth()  - _r * 2) crash = true; //����
		if (_y >= ege::getheight() - _r * 2) crash = true; //����

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

		// �ı�alphaֵ
		_alpha += _da;
		if (_alpha <= 0)	_da = 1;
		if (_alpha >= 0xFF) _da = -1;
		return 0;
	}

	//��������ֵ�滭
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
	//��ʼ��������Ϊ�������
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
		// ����ͣ״̬�Ÿ���
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
		if (key == 'P' || key == 'p') //����P�����ڲ�������֮ͣ��ת��
		{
			m_pause = !m_pause;
		}
		if (key == VK_ESCAPE) //�����ESC�����ͱ��Ϊ�˳�������VK_ESCAPE��SDK����ĺ�
		{
			m_endscene = 1;
		}
	}
	void onmouse(int x, int y)
	{
		// �㲥�����Ϣ
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
	Scene scene(30); //���峡������ʼ������Ϊ30

	for ( ; ege::is_run(); ege::delay_fps(60) )
	{
		while (ege::kbhit())
		{
			int key = ege::getch();
			scene.onkey(key); //���а�����Ϣ���͸�scene
		}
		while (ege::mousemsg())
		{
			ege::mouse_msg msg = ege::getmouse();
			scene.onmouse(msg.x, msg.y);
		}
		if (scene.update()) //���update���ط�0��ʾ������������ʱ�˳���ѭ��
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
	// ͼ�γ�ʼ�������ڳߴ�640x480
	ege::initgraph(640, 480);
	// �������ʼ���������Ҫʹ��������Ļ�
	ege::randomize();
	// ������ѭ��
	mainloop();
	// �رջ�ͼ�豸
	ege::closegraph();
	return 0;
}
