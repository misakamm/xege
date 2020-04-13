//���������ߣ�ʹ�÷��������������ɫ�ʣ�ʹ��Ķ�������Ȼ
#include <graphics.h>

//����һ��AniObj����ǰһ�������Բ���������д������ṹ����
struct AniObj
{
	float x, y;
	int r;
	float dx, dy;
	int alpha, da;
	PIMAGE img;
};

//�Զ��庯������������һ��0 - m֮��ĸ�����
float myrand(float m)
{
	// randomf() ����һ�� 0 �� 1 ֮������������
	return (float)(randomf() * m);
}

//�����ٶȻ�׼ֵ��base_speed������ٶȣ�base_speed+randspeed������ٶ�
const float base_speed = 0.5f;
const float randspeed = 1.5f;

//��ʼ�����������꣬�ٶȷ���͸���ȣ�����IMAGE��
void initobj(AniObj* obj)
{
	obj->x = myrand((float)getwidth());
	obj->y = myrand((float)getheight());
	obj->r = random(20) + 20;
	// ����ٶ�
	obj->dx = myrand(randspeed) + base_speed;
	obj->dy = myrand(randspeed) + base_speed;
	// �������
	obj->dx = obj->dx * float(random(2) * 2.0 - 1);
	obj->dy = obj->dy * float(random(2) * 2.0 - 1);
	// ���ϵĴ���ʽ������֤�ٶȲ�����

	obj->alpha = random(250) + 2;
	obj->da = (int)random(2) * 2 - 1;

	obj->img = newimage(obj->r * 2, obj->r * 2);

	color_t col = hsv2rgb(myrand(360.0f), 1.0f, 1.0f);
	setcolor(col, obj->img);
	setfillcolor(col, obj->img);

	fillellipse(obj->r, obj->r, obj->r, obj->r, obj->img);
}

//����λ�õ��������
void updateobj(AniObj* obj)
{
	// ��ǰλ�� + �ٶ�
	obj->x += obj->dx;
	obj->y += obj->dy;
	if (obj->x < 0) obj->dx = myrand(randspeed) + base_speed; //����
	if (obj->y < 0) obj->dy = myrand(randspeed) + base_speed; //����
	if (obj->x >= ege::getwidth()  - obj->r * 2) obj->dx = -(myrand(randspeed) + base_speed); //����
	if (obj->y >= ege::getheight() - obj->r * 2) obj->dy = -(myrand(randspeed) + base_speed); //����

	// �ı�alphaֵ
	obj->alpha += obj->da;
	if (obj->alpha <= 0) obj->da = 1;
	if (obj->alpha >= 0xFF) obj->da = -1;
}

//��������ֵ�滭
void drawobj(AniObj* obj)
{
	putimage_alphatransparent(NULL, obj->img, (int)obj->x, (int)obj->y, BLACK, (unsigned char)obj->alpha);
}

//�ͷ��������ʱ����
void releaseobj(AniObj* obj)
{
	delimage(obj->img);
}

void mainloop()
{
	const int MAXOBJ = 30;
	AniObj obj[MAXOBJ]; //�����������
	int n;

	for (n = 0; n < MAXOBJ; ++n)
	{
		initobj(&obj[n]); //��ʼ��
	}

	for ( ; is_run(); delay_fps(60) )
	{
		for (n = 0; n < MAXOBJ; ++n)
		{
			updateobj(&obj[n]); //����λ��
		}

		cleardevice();
		for (n = 0; n < MAXOBJ; ++n)
		{
			drawobj(&obj[n]); //�滭
		}
	}

	for (n = 0; n < MAXOBJ; ++n)
	{
		releaseobj(&obj[n]); //�ͷ�
	}
}

int main(void)
{
	setinitmode(INIT_ANIMATION);
	// ͼ�γ�ʼ�������ڳߴ�640x480
	initgraph(640, 480);
	// �������ʼ���������Ҫʹ��������Ļ�
	randomize();
	// ������ѭ��
	mainloop();
	// �رջ�ͼ�豸
	closegraph();
	return 0;
}
