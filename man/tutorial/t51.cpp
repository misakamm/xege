#include <ege.h>
#include <ege/fps.h>

#include <stdio.h>

class graph : public ege::egeControlBase
{
public:
	CTL_PREINIT(graph, ege::egeControlBase) {
		int w = 40;
		size(w * 2, w * 2);
		//enable(false);
	} CTL_PREINITEND;
	graph(CTL_DEFPARAM) : CTL_INITBASE(ege::egeControlBase) {
		CTL_INIT;
		// 初始化坐标，
		x = ege::random(parent()->getw()), y = ege::random(parent()->geth());
		// 开启透明半透明混合
		blendmode(ALPHABLEND);

		{
			int w = (int)(geth() * 0.5);
			ege::color_t col = ege::hsv2rgb((float)ege::random(360), 1.0f, 1.0f);
			ege::setbkcolor_f(col);
			ege::cleardevice();

			ege::settarget(filter());
			col = (ege::color_t)EGEGRAY(0xF0);
			ege::setcolor(col);
			ege::setfillcolor(EGEGRAYA(col, 0xFF));
			//ege::fillellipse(w, w, w, w);
			ege::ege_enable_aa(true);
			ege::ege_fillellipse(0.0f, 0.0f,
				(float)getw(), (float)geth());
		}
		dx = 0.3 + ege::randomf() * 2, dy = 0.3 + ege::randomf() * 2;
	}
	int onUpdate() {
		if (dx < 0 && x < 0) dx = -dx;
		if (dx > 0 && x + getw() > parent()->getw()) dx = -dx;
		if (dy < 0 && y < 0) dy = -dy;
		if (dy > 0 && y + geth() > parent()->geth()) dy = -dy;
		x += dx, y += dy;
		move((int)x, (int)y);
		return 0;
	}
	void onDraw(ege::PIMAGE pimg) const {
		;
	}
private:
	double dx, dy;
	double x, y;
};

class Window : public ege::egeControlBase
{
public:
	CTL_PREINIT(Window, ege::egeControlBase) {
		size(400, 300);
	} CTL_PREINITEND;
	Window(CTL_DEFPARAM) : CTL_INITBASE(ege::egeControlBase) {
		CTL_INIT;
		blendmode(ALPHABLEND);
		m_col = ege::hsv2rgb(float(ege::randomf() * 360), 1.0f, 0.2f);
		m_capmouse = 0;
		// 绘画椭圆可见区域
		ege::setbkcolor_f(0x0, filter());
		ege::cleardevice(filter());
		ege::setcolor((ege::color_t)EGEGRAY(0xAF), filter());
		ege::setfillcolor((ege::color_t)EGEGRAYA(0xAF, 0xFF), filter());
		//ege::fillellipse(getw() / 2, geth() / 2,
		//	  (int)(getw() * 0.5), (int)(geth() * 0.5), filter());
		ege::ege_enable_aa(true, filter());
		ege::ege_fillellipse(0.0f, 0.0f,
			(float)getw(), (float)geth(), filter());
	}
	int onMouse(int x, int y, int flag) {
		if (m_capmouse == 0 && (flag & ege::mouse_msg_down)) {
			if (flag & ege::mouse_flag_left) {
				capmouse(true);
				m_capmouse = 1;
				m_capx = x;
				m_capy = y;
			}
		} else if ((flag & ege::mouse_msg_up)) {
			capmouse(false);
			if (m_capmouse && (flag & ege::mouse_flag_left)) {
				m_capmouse = 0;
			}
		} else if (m_capmouse) {
			move(x - m_capx + getx(), y - m_capy + gety());
		}
		return 0;
	}
	void onDraw(ege::PIMAGE pimg) const {
		// 重画控件背景
		ege::setbkcolor_f(m_col, pimg);
		ege::cleardevice(pimg);
	}
private:
	int m_capmouse;
	int m_capx, m_capy;
	graph m_graph[4];
	ege::color_t m_col;
};

// 最简单的继承示例
class Window2 : public Window
{
public:
	CTL_PREINIT(Window2, Window) {
	} CTL_PREINITEND;
	Window2(CTL_DEFPARAM) : CTL_INITBASE(Window) {
		CTL_INIT;
		static int s_x = 0;
		move(s_x, s_x);
		s_x = (s_x + 32) % 300;
	}
private:
	//graph m_graph[4];
};

int main()
{
	ege::setinitmode(ege::INIT_DEFAULT | ege::INIT_NOFORCEEXIT);
	ege::initgraph(800, 600);
	ege::setrendermode(ege::RENDER_MANUAL);
	ege::randomize();

	Window2 w[4];
	ege::fps f;
	for ( ; ege::is_run(); ege::delay_fps(60)) {
		f.zorderup();
		ege::cleardevice();
	}
	return 0;
}
