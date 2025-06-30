//#define _GRAPH_NO_LIB_
//#include "ege.h"

#define _TUTORIAL_ 0
#if _TUTORIAL_ == 0

#include <graphics.h>
int main() {
	ege::setinitmode(ege::INIT_DEFAULT | ege::INIT_NOFORCEEXIT);
	ege::initgraph(640, 480);
	setbkcolor_f(EGERGB(0x80, 0, 0x80));
	cleardevice();
	ege::getch();
	ege::closegraph();
	ege::initgraph(800, 600);
	ege::getch();
	ege::closegraph();
	return 0;
}

#elif _TUTORIAL_ == 1
#include "../../man/tutorial/t01.cpp"
#elif _TUTORIAL_ == 2
#include "../../man/tutorial/t02.cpp"
#elif _TUTORIAL_ == 3
#include "../../man/tutorial/t03.cpp"
#elif _TUTORIAL_ == 4
#include "../../man/tutorial/t04.cpp"
#elif _TUTORIAL_ == 5
#include "../../man/tutorial/t05.cpp"
#elif _TUTORIAL_ == 6
#include "../../man/tutorial/t06.cpp"
#elif _TUTORIAL_ == 7
#include "../../man/tutorial/t07.cpp"
#elif _TUTORIAL_ == 8
#include "../../man/tutorial/t08.cpp"
#elif _TUTORIAL_ == 9
#include "../../man/tutorial/t09.cpp"
#elif _TUTORIAL_ == 10
#include "../../man/tutorial/t10.cpp"
#elif _TUTORIAL_ == 11
#include "../../man/tutorial/t11.cpp"
#elif _TUTORIAL_ == 12
#include "../../man/tutorial/t12.cpp"
#elif _TUTORIAL_ == 13
#include "../../man/tutorial/t13.cpp"
#elif _TUTORIAL_ == 14
#include "../../man/tutorial/t14.cpp"
#elif _TUTORIAL_ == 15
#include "../../man/tutorial/t15.cpp"
#elif _TUTORIAL_ == 16
#include "../../man/tutorial/t16.cpp"
#elif _TUTORIAL_ == 17
#include "../../man/tutorial/t17.cpp"
#elif _TUTORIAL_ == 18
#include "../../man/tutorial/t18.cpp"
#elif _TUTORIAL_ == 19
#include "../../man/tutorial/t19.cpp"
#elif _TUTORIAL_ == 21
#include "../../man/tutorial/t21.cpp"
#elif _TUTORIAL_ == 22
#include "../../man/tutorial/t22.cpp"
#elif _TUTORIAL_ == 23
#include "../../man/tutorial/t23.cpp"
#elif _TUTORIAL_ == 24
#include "../../man/tutorial/t24.cpp"
#elif _TUTORIAL_ == 25
#include "../../man/tutorial/t25.cpp"
#elif _TUTORIAL_ == 26
#include "../../man/tutorial/t26.cpp"
#elif _TUTORIAL_ == 27
#include "../../man/tutorial/t27.cpp"
#elif _TUTORIAL_ == 28
#include "../../man/tutorial/t28.cpp"
#elif _TUTORIAL_ == 29
#include "../../man/tutorial/t29.cpp"
#elif _TUTORIAL_ == 30
#include "../../man/tutorial/t30.cpp"
#elif _TUTORIAL_ == 50
#include "../../man/tutorial/t50.cpp"
#elif _TUTORIAL_ == 51
#include "../../man/tutorial/t51.cpp"
#elif _TUTORIAL_ == 52

#include <graphics.h>
#include <ege/fps.h>

#include <stdio.h>

class input : public egeControlBase
{
public:
	CTL_PREINIT(input, egeControlBase) {
		// do sth. before sub objects' construct function call
	} CTL_PREINITEND;
	input(CTL_DEFPARAM) : CTL_INITBASE(egeControlBase) {
		CTL_INIT; // must be the first line
		m_key = 0;
		size(64, 16);
	}
	int  onKeyDown(int key, int flag) {
		m_key = key;
		return 0;
	}
	int  onKeyUp(int key, int flag) {
		if (m_key == key) m_key = 0;
		return 0;
	}
	void onDraw(PIMAGE pimg) const {
		char str[10] = "";
		if (m_key) {
			sprintf(str, "%d %c", m_key, m_key);
		}
		setbkcolor_f(GREEN, pimg);
		cleardevice(pimg);
		setcolor(WHITE, pimg);
		setbkmode(TRANSPARENT, pimg);
		setfont(12, 0, "宋体", pimg);
		outtextxy(0, 0, str, pimg);
	}
protected:
	int m_key;
};

class input2 : public input
{
public:
	CTL_PREINIT(input2, input) {
		// do sth. before sub objects' construct function call
	} CTL_PREINITEND;
	input2(CTL_DEFPARAM) : CTL_INITBASE(input) {
		CTL_INIT; // must be the first line
		m_key = 0;
		size(64, 128);
		move(200, 0);
		w.move(0, 64);
	}
	void onDraw(PIMAGE pimg) const {
		char str[10] = "";
		if (m_key) sprintf(str, "%c", m_key);
		setbkcolor_f(RED, pimg);
		cleardevice(pimg);
		setcolor(WHITE, pimg);
		setbkmode(TRANSPARENT, pimg);
		setfont(12, 0, "宋体", pimg);
		outtextxy(0, 0, str, pimg);
	}
private:
	input w;
};


int main()
{
	initgraph(320, 240);
	ege::setrendermode(ege::RENDER_MANUAL);
	randomize();

	input w[3];
	input2 w2;
	fps f;
	for (int i = 0; i < 3; ++i) {
		w[i].move(100, i * 32);
	}
	for ( ; is_run(); delay_fps(60)) {
		f.zorderup();
		cleardevice();
	}
	return 0;
}

#elif _TUTORIAL_ == 53

#include <ege.h>
#include <ege/fps.h>

#include <stdio.h>

class alphafilter : public ege::egeControlBase
{
public:
	CTL_PREINIT(alphafilter, ege::egeControlBase) {
		// do sth. before sub objects' construct function call
	} CTL_PREINITEND;
	alphafilter(CTL_DEFPARAM) : CTL_INITBASE(ege::egeControlBase) {
		CTL_INIT; // must be the first line
		size(parent()->getw(), parent()->geth());
		blendmode(ALPHABLEND);

		_h = 0.0f;
		ege::PushTarget target(filter());
		ege::cleardevice();
		ege::ege_enable_aa(true);
		ege::ege_point center = {(float)getw() / 2.0f, (float)geth() / 2.0f};
		ege::ege_setpattern_lineargradient(0.0f, 0.0f, EGEAGRAY(0xffu, 0xff), 0.0f, (float)geth() / 2.0f, EGEAGRAY(0xffu, 0x0));
		//ege::ege_setpattern_ellipsegradient(center, EGEAGRAY(0xffu, 0x0),
		//    0.0f, 0.0f, (float)getw() * 1.5f, (float)geth() * 1.5f, EGEAGRAY(0xffu, 0xff));
		ege::ege_fillellipse(0.0f, 0.0f, (float)getw(), (float)geth());
	}
	int  onMouse(int x, int y, int flag) {
		if (flag & ege::mouse_msg_down) {
			capmouse(true);
		} else if (flag & ege::mouse_msg_up) {
			capmouse(false);
		}
		_x = (float)x;
		_y = (float)y;
		return 0;
	}
	int  onUpdate() {
		_h += 1.0f;
		if (_h > 360.0f) _h -= 360.0f;
		return 0;
	}
	void draw() const {
		ege::PushTarget target(buf());
		ege::ege_colpoint rect[4];
		float w = 320.0f, h = 240.0f;

		ege::cleardevice();

		rect[0].x = _x - w / 2.0f;
		rect[0].y = _y - h / 2.0f;
		rect[0].color = ege::hsv2rgb(_h, 1.0f, 1.0f);

		rect[1].x = _x - w / 2.0f;
		rect[1].y = _y + h / 2.0f;
		rect[1].color = ege::hsv2rgb(_h + 90.0f, 1.0f, 1.0f);

		rect[3].x = _x + w / 2.0f;
		rect[3].y = _y + h / 2.0f;
		rect[3].color = ege::hsv2rgb(_h + 180.0f, 1.0f, 1.0f);

		rect[2].x = _x + w / 2.0f;
		rect[2].y = _y - h / 2.0f;
		rect[2].color = ege::hsv2rgb(_h + 270.0f, 1.0f, 1.0f);

		ege::fillpoly_gradient(4, rect);
	}
	void onDraw(ege::PIMAGE pimg) const {
		draw();
	}
private:
	float _x, _y;
	float _h;
};


int main()
{
	ege::initgraph(640, 480);
	ege::setrendermode(ege::RENDER_MANUAL);
	ege::randomize();

	ege::fps f;
	alphafilter a;
	for ( ; ege::is_run(); ege::delay_fps(60)) {
		f.zorderup();
		ege::cleardevice();
	}
	return 0;
}
#elif _TUTORIAL_ == 60
// 星空屏保程序，请生成为scr后缀，或者手工改后缀，不要直接运行exe
#include "graphics.h"
#include <time.h>
#include <stdio.h>

#define MAXSTAR 2000 // 星星总数
int sc_width, sc_heigh; // 记录窗口宽高
int g_max;

struct STAR {
	double x;
	int y;
	double step;
	int color;
} star[MAXSTAR];

// 初始化星星
void InitStar( int i ) {
	double speed = 0.006;
	star[i].x = 0;
	star[i].y = random( sc_heigh );
	star[i].step = randomf() * speed * 0.9 + speed * 0.1;
	star[i].color = ( int )( star[i].step * 255 / speed + 0.5 ); // 速度越快，颜色越亮
	if ( star[i].color > 255 ) {
		star[i].color = 255;
	}
	star[i].color = EGERGB( star[i].color, star[i].color, star[i].color );
}

// 移动星星
void MoveStar( int i, double dt ) {
	// 擦掉原来的星星
	putpixel( ( int )( star[i].x * sc_width ), star[i].y, 0 );
	// 计算新位置
	star[i].x += star[i].step * dt * 60;
	if ( star[i].x > 1 ) InitStar( i );
	// 画新星星
	putpixel( ( int )( star[i].x * sc_width ), star[i].y, star[i].color );
}

int preinit( int argc, char* argv[] ) {
	setinitmode( INIT_NOBORDER | INIT_TOPMOST ); // 指定初始化为无边框顶层窗口，并且窗口左上角坐标为(0, 0)
	g_max = MAXSTAR;
	if ( argc < 2 ) {
		//MessageBoxA( NULL, "本屏幕保护程序无配置", "星空屏保", MB_OK );
		//return -1;
	} else if ( stricmp( argv[1], "/p" ) == 0 ) { // 小窗口预览模式
		HWND hwnd;
		sscanf( argv[2], "%d", &hwnd );
		attachHWND( hwnd ); // 新ege函数
		setinitmode( INIT_NOBORDER | INIT_CHILD | INIT_WITHLOGO ); // 指定初始化为无边框子窗口
		g_max = 200;
		return 1;
	} else if ( stricmp( argv[1], "/s" ) ) { // 非测试运行模式
		MessageBoxA( NULL, "本屏幕保护程序无配置", "星空屏保", MB_OK );
		return -1;
	}
	return 0; // 全屏模式
}

// 主函数
int main( int argc, char* argv[] ) {
	int i, ms_x = -1024, ms_y = -1024, exitflag = 0;
	int fps = 60;
	double dtime;

	int mode = preinit( argc, argv ); // 记录初始化模式
	if ( mode < 0 ) return 0;

	randomize(); // 初始化随机种子
	initgraph( -1, -1 ); // 打开图形窗口，以全屏模式

	showmouse( mode );
	sc_width = getwidth();
	sc_heigh = getheight();

	// 初始化所有星星
	for ( i = 0; i < g_max; i++ ) {
		InitStar( i );
		star[i].x = randomf();
	}
	// 绘制星空，按任意键或移动鼠标退出
	setfont( 12, 6, "宋体" );
	setrendermode( RENDER_MANUAL );
	dtime = fclock();
	while ( kbmsg() ) getkey();

	for ( ; !exitflag && is_run() && kbmsg() == 0; delay_fps( fps ) ) { //每秒画120帧，kbhit(1)是获取键盘任意键的消息，详见pdf
		// 如果有鼠标消息
		while ( mousemsg() ) {
			mouse_msg msg = getmouse();
			if ( ms_x <= -1024 ) {
				ms_x = msg.x;
				ms_y = msg.y;
			}
			// 处理鼠标，移动超出范围就退出
			if ( mode == 0 ) { // 仅全屏模式才处理鼠标
				int x = msg.x, y = msg.y;
				x -= ms_x;
				y -= ms_y;
				if ( x * x + y * y > 400 ) exitflag = 1;
			}
		}
		// 显示星星
		double dt = 1.0 / fps; //fclock() - dtime;
		dtime += dt;
		for ( int i = 0; i < g_max; i++ ) {
			MoveStar( i, dt );
		}
		// 显示FPS
		{
			char str[60];
			sprintf( str, "%8.2f FPS", getfps());
			outtextxy( 0, 0, str ); //显示fps
		}
	}
	closegraph(); // 关闭图形窗口
	return 0;
}


#elif _TUTORIAL_ == 61

#include <stdio.h>
#include <graphics.h>
#include <ege/label.h>
#include "ege/sys_edit.h"

int main()
{
	initgraph(640, 480, INIT_ANIMATION);
	//*
	{
		char str[1024];
		inputbox_getline("aaa", "bbb", str, sizeof(str));
		//inputbox_getline("aaa", "bbb", str, sizeof(str));
		int a = strlen(str);
	}//*/
	label lb;
	//button bt;
	printf("");
	lb.caption("abcdefg");
	//bt.setcaption("click");
	PIMAGE img = newimage();
	getimage(img, "qq_003.jpg");
	sys_edit edit;
	edit.create(true);
	edit.move(100, 100);
	edit.size(100, 100);
	edit.settext("去死");
	edit.setmaxlen(30);
	edit.visible(true);
	edit.setcolor(EGERGB(0, 0x80, 0));
	edit.setbgcolor(EGERGB(0xff, 0x80, 0xff));
	setbkcolor(EGERGB(0x80, 0, 0x80));
	float r = 0;
	MUSIC mus;
	mus.OpenFile("S:\\music\\kugou\\akia - alifornia.mp3");
	mus.Play();
	float v = 0.0f, dv = 0.01f;
	for (; is_run(); delay_fps(60))
	{
		v += dv;
		if (v > 1.0f) dv = -0.01f, v = 1.0f;
		if (v < 0.0f) dv = 0.01f, v = 0.0f;
		mus.SetVolume(v);
		r += 0.01f;
		if (r > PI * 2.0f) r -= float(PI * 2.0f);
		cleardevice();
		edit.visible(true);
		putimage_rotatezoom(NULL, img, 200, 200, 0.5f, 0.5f, r, 1.5f, 0, -1, 1);
		xyprintf(200, 200, "%d", 100);
	}
	closegraph();
	return 0;
}

#elif _TUTORIAL_ == 62

#include <map>

template <typename str_t>
class IniParser {
public:
	typedef std::map<str_t, str_t> * key_val_ptr;
	IniParser() {}
	virtual ~IniParser() {}
	void clear() {
		_data.clear();
	}
private:
	std::map<str_t, key_val_ptr > _data;
};

#include <string>
int main() {
	IniParser<std::string> d;
	d.clear();
}
#endif
