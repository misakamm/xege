//���������ˣ�ʹ��C++��װ��Ķ���

//��������ǰ���㶼����ǰ��ƪ�˰ɣ����������ƺ���һ���̶�ģʽ��
//�����������о���˵������Ŀ�����
//��װһ��ͼ�ζ����Ĵ��Ҫ�ӿں�������ʼ�����߼����£��滭����Ⱦ�����ͷţ�������
//�������˾;��Բ���ȷ���ӿں������˾�˵����ķ�װ���ܻ���Ҫ����
//һ���Ӧ���£������ĸ��ӿھ��㹻�ˣ�����Ҳ���������ĸ�������һ��Ҫ�ǵ�
//������˵�����ֻ�����ĸ������������и���ĺ����������Լ�ʹ�ã������⣨����ѭ����������¶ʹ�õ�Ӧ�þ����ĸ�

// �ӱ��Ŀ�ʼ�����ô�ͷ�ļ������ú���ʱ����������Ҫege���namespace
// �����ĺô���vc���Զ���ʾ���Ը���ȷ���Ķ������ʱ��ɶ��Ը��ã�ȱ���Ǳ�д��ʱ�����鷳һЩ
#include <ege.h>

const float base_speed = 0.5f;
const float randspeed = 1.5f;

//�Զ��庯������������һ��0 - m֮��ĸ�����
float myrand(float m)
{
	return (float)(ege::randomf() * m);
}

//����һ��AniObj��
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

	//����λ�õ��������
	void update()
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
	}

	//��������ֵ�滭
	void render()
	{
		ege::putimage_alphatransparent(NULL, _img, (int)_x, (int)_y, ege::BLACK, (unsigned char)_alpha);
	}

	//�ͷ��������ʱ����
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
	AniObj obj[MAXOBJ]; //�����������
	int n;

	//Ϊʲô����û��obj��ʼ������Ϊ�������������ʱ��͵����˹��캯����ʼ������

	for ( ; ege::is_run(); ege::delay_fps(60) )
	{
		for (n = 0; n < MAXOBJ; ++n)
		{
			obj[n].update(); //����λ��
		}

		ege::cleardevice();
		for (n = 0; n < MAXOBJ; ++n)
		{
			obj[n].render(); //�滭
		}
	}

	//Ϊʲô����û���ͷţ���Ϊobj��������󣬻��ڱ����̽������Զ������͹�����
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
