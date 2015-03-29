//cyd@20150329
// 用GCC编译时，compile参数需指明为 -finput-charset=UTF-8
// 否则，编译后的程序运行中汉字说明会变成乱码 
#include <graphics.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
#define BALL_AMOUNT		300

#ifdef	PI
#undef	PI
#endif

#define PI				3.14159f
#define PI2				(2*PI)
#define FLOATCOMP0(x)	(x < 0.01f && x > -0.01f)

using std::vector;

float getAngleByNormal(float x, float y)
{
	float d = sqrtf(x*x+y*y);
	if(FLOATCOMP0(d)) return .0f;
	float angle = asinf(y / d);
	if(x > .0f)
	{
		if(y > .0f)	return angle;
		else return PI2 + angle;
	}
	else
	{
		return PI - angle;
	}
}

class Ball
{
public:
	Ball() : m_x(SCREEN_WIDTH / 2), m_y(SCREEN_HEIGHT / 2)
	{
		s_speed += .01f;
		m_dx = s_speed * cosf(s_direction);
		m_dy = s_speed * sinf(s_direction);
		m_speed = s_speed;
		m_radius = randomf() * 5.0f + 2.0f;
		m_color = random(0xff0000) + 0xffff;
		s_direction += .1f;
	}
	
	void update()
	{
		m_x += m_dx;
		m_y += m_dy;

		if(m_x < .0f || m_x > SCREEN_WIDTH)
		{
			m_dx = -m_dx;
			m_x += m_dx;
		}
		if(m_y < .0f || m_y > SCREEN_HEIGHT)
		{
			m_dy = -m_dy;
			m_y += m_dy;
		}

#ifdef _DEBUG

		if(m_x < .0f || m_x > SCREEN_WIDTH || m_y < .0f || m_y > SCREEN_HEIGHT)
		{
			setcaption("检测到某些点出界了!!");
		}

#endif
	}

	void render()
	{
		setfillcolor(m_color);
		setcolor(m_color);
		fillellipse(m_x, m_y, m_radius, m_radius);
	}

	void turn(float x, float y)
	{
		float aglBall = getAngleByNormal(-m_dx, -m_dy);
		float aglLine = getAngleByNormal(x, y);
		float angle = 2 * aglLine - aglBall;
		m_dx = cosf(angle) * m_speed;
		m_dy = sinf(angle) * m_speed;
		m_x += m_dx;
		m_y += m_dy;
	}
	
	void getPos(float& x, float& y)
	{
		x = m_x;
		y = m_y;
	}

	float getRadius() { return m_radius; }

	~Ball() {}

private:
	float m_x, m_y;
	float m_dx, m_dy;
	float m_radius;
	float m_speed;
	COLORREF m_color;
	static float s_direction;
	static float s_speed;
};

float Ball::s_direction;
float Ball::s_speed = 0.5f;

class Line
{
public:
	Line() {}
	Line(int x1, int y1, int x2, int y2)
		: m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_maxx(x1), m_minx(x2), m_maxy(y1), m_miny(y2)
	{
		if(m_maxx < m_minx) std::swap(m_maxx, m_minx);
		if(m_maxy < m_miny) std::swap(m_maxy, m_miny);
		getLineEquation();
	}
	~Line() {}

	void getLineEquation()  //ax + by + c = 0
	{
		float a = m_y1 - m_y2, b = m_x1 - m_x2, c = m_x1 * m_y2 - m_x2 * m_y1;
		if(FLOATCOMP0(b))
		{
			m_a = 1;
			m_b = 0.0f;
			m_c = -m_x1;
		}
		else
		{
			m_a = a / b;
			m_b = -1.0f;
			m_c = c / b;
		}
		m_d2 = m_a * m_a + m_b * m_b;
		m_d = sqrtf(m_d2);
	}

	void render() { line(m_x1, m_y1, m_x2, m_y2); }

	void collisionDetect(Ball& ball)
	{
		float x,y;
		ball.getPos(x,y);
		float prjX = (m_b*m_b*x - m_a*m_b*y - m_a*m_c) / m_d2;
		float prjY = (m_a*m_a*y - m_b*m_c - m_a*m_b*x) / m_d2;

		if(prjX >= m_minx && prjX <= m_maxx && prjY >= m_miny && prjY <= m_maxy)
		{
			float distance = fabs(m_a * x + m_b * y + m_c) / m_d;
			if(distance < ball.getRadius())
			{
				ball.turn(m_a, m_b);
			}
		}
		else
		{
			//处理可能的与端点碰撞，如无处理，则事件不确定。
		}
	}

private:
	int m_x1, m_y1;
	int m_x2, m_y2;
	int m_maxx, m_maxy, m_minx, m_miny;
	float m_a, m_b, m_c, m_d, m_d2;
};

class Scene
{
public:
	Scene() : m_activeBallNum(0), m_bUpdate(true), m_bBlurring(true) {}
	~Scene(){}

	void update()
	{
		static bool bFirstTime = true;
		if(bFirstTime)
		{
			setbkmode(TRANSPARENT);  // 真正双缓冲啊!也只有这个才会出现这种奇葩现象。
			setfont(20, 10, "黑体"); // 你没保存系统状态,如果去掉这两句，你看看就知道了。
			if(m_activeBallNum < BALL_AMOUNT) ++m_activeBallNum;
			outtextxy(10, 10, "如果小球数量过多(最多300)造成卡顿，请按'+'和'-'来调节，一次增加或减少10个小球");
			outtextxy(10, 30, "使用空格键可以定住小球方便画线，使用回车键可以取消画面模糊!");
			outtextxy(10, 50, "使用BackSpace 或 Delete键可以撤销上一次的画线，直至消除所有直线.");
			outtextxy(10, 70, "也许你可以按下space键来尝试画一个多边形，^_^ ");
			outtextxy(10, 100, "必须说明的是, 没有做直线两端点的检测,这个很简单,弹回去嘛,但是太没意思");
			outtextxy(10, 130, "所以……我是让小球沿着直线滑动，并且入射角度越大滑动越快，好玩吧!");
			outtextxy(100, 300, "现在, 恩, 按下任意键就不用看到讨厌的我啦!");
		}
		if(kbhit())
		{
			bFirstTime = false;
			int c;
			switch(c = getch())
			{
			case 27:
				closegraph();
				exit(0);
			case ' ': m_bUpdate = !m_bUpdate; break;
			case '\r': m_bBlurring = !m_bBlurring; break;
			case '+': if((m_activeBallNum += 10) > BALL_AMOUNT) m_activeBallNum = BALL_AMOUNT; break;
			case '-': if((m_activeBallNum -= 10) <= 0) m_activeBallNum = 1; break;
			case 8: case 302: if(!m_vec.empty()) m_vec.erase(m_vec.end() - 1);
			default:;
			}
			flushkey();
		}
		if(m_bBlurring)
		{
			imagefilter_blurring(NULL, 0x7e, 0xff);
			swappage();
			delay_fps(60);
		}
		else
		{

			swappage();
			delay_fps(60);
			cleardevice();			
		}
		
	}

	void addLine()
	{
		static int s_x = 0, s_y = 0;
		static bool isLbuttonDown = false;

		if(mousemsg())
		{
			MOUSEMSG msg;
			do
			msg = GetMouseMsg();
			while(msg.uMsg == WM_MOUSEMOVE && mousemsg());
			if(msg.uMsg == WM_LBUTTONDOWN)
			{
				s_x = msg.x;
				s_y = msg.y;
				isLbuttonDown = true;
			}
			if(msg.uMsg == WM_LBUTTONUP)
			{
				m_vec.push_back(Line(s_x, s_y, msg.x, msg.y));
				isLbuttonDown = false;
			}
		}
		if(isLbuttonDown)
		{
			int x, y;
			mousepos(&x, &y);
			setlinestyle(PS_DASH);
			setcolor(RED);
			line(s_x, s_y, x, y);
			setlinestyle(PS_SOLID);
		}
	}

	void show()
	{
		for(int i = 0; i != m_activeBallNum; ++i)
		{
			if(m_bUpdate) m_ball[i].update();
			m_ball[i].render();
			for(std::vector<Line>::iterator iter = m_vec.begin(); iter != m_vec.end(); ++iter)
			{
				iter->collisionDetect(m_ball[i]);
			}
		}
		setcolor(WHITE);
		for(std::vector<Line>::iterator iter = m_vec.begin(); iter != m_vec.end(); ++iter)
		{
			iter->render();
		}
		addLine();
	}

private:
	Ball m_ball[BALL_AMOUNT];
	vector<Line> m_vec;
	int m_activeBallNum;
	bool m_bBlurring, m_bUpdate;
};

int main()
{
	setinitmode(0);
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
	setcaption("任意直线的碰撞检测——wysaid");	
	randomize();
	setrendermode(RENDER_MANUAL);
	setbkmode(TRANSPARENT);
	setfont(20, 10, "黑体");

	Scene scene;
	
	for( ; ; )
	{
		scene.update();
		scene.show();
	}
	return 0;
}

