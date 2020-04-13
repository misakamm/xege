//���������ţ�ʹ�ó����������ؿ��ƶ���

//����Ҫ����ο��ƶ����������Ĳ��ţ���ͣ
//���ǰѶ�����װ��һ���������ɳ����������ƶ���
//��Ȼ�����ڿ�ʼ�Ͳ��ǵ����Ķ��������������û�������ʱ�򣬽ӿھ�Ҫ������
//��ʾ������ʱ������ʹ��P����ͣ�򲥷�
#include <ege.h>

const float base_speed = 0.5f;
const float randspeed = 1.5f;

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

		_img = ege::newimage(_r * 2, _r * 2);

		color_t col = ege::hsv2rgb(myrand(360.0f), 1.0f, 1.0f);
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
		// ��ǰλ�� + �ٶ�
		_x += _dx;
		_y += _dy;
		if (_x < 0) _dx = myrand(randspeed) + base_speed; //����
		if (_y < 0) _dy = myrand(randspeed) + base_speed; //����
		if (_x >= ege::getwidth()  - _r * 2) _dx = -(myrand(randspeed) + base_speed); //����
		if (_y >= ege::getheight() - _r * 2) _dy = -(myrand(randspeed) + base_speed); //����

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

//��ǣ�scene������Ѿ����Լ��Ķ����б�������ɸ���һ��Ľṹ��
//�����Զ�̬���ƶ���ĸ�����������������ڸ��߲�����������
//�����Ѿ�Խ����Ļ�ͻ���������󣬵�������һ������������ȣ���Ȼ����Ҳ�������Ǹ������Լ����ƣ�����������ǿ�����䲻��ʾ�����ɶ�������ƣ�


//��ǣ�ΪʲôҪ���峡��������Ϊ�˸��õؿ��ƶ����⣬
//��ͬ���������в�ͬ�Ķ�������ͬ���û���Ӧ��
//����һ�����������������˵���������ʽ��Ϸ�����������ֳ�������������
//��ȫ������һ����ѭ������ȫ�����󣬻������ܶ�
//������Ϳ�������ʵ�ֳ�רҵ����Ķ���������Ϸ�ˣ�
//�����ڴ�ͷ��βֻ����һ����ʽ��Ϸ������������������
