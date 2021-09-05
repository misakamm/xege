#include <ege.h>
#include <ege/fps.h>

#include <stdio.h>

class graph : public ege::egeControlBase
{
public:
	graph() {
		ege::PushTarget _target; // 把当前绘图目标缓冲，函数退出时自动还原
		settarget(buf()); // 设置绘图目标
		x = ege::random(parent()->getw()), y = ege::random(parent()->geth());
		blendmode(1);
		size(40 * 2, 40 * 2);

		{
			int w = geth() / 2;
			ege::color_t col = ege::hsv2rgb((float)ege::random(360), 1.0f, 1.0f);
			ege::setcolor(col);
			ege::setfillcolor(col);
			ege::fillellipse(w, w, w, w);
			settarget(filter());
			col = (ege::color_t)EGEGRAY(0xC0);
			ege::setcolor(col);
			ege::setfillcolor(col);
			ege::fillellipse(w, w, w, w);
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

int main()
{
	ege::setinitmode(ege::INIT_DEFAULT | ege::INIT_NOFORCEEXIT);
	ege::initgraph(640, 480);
	ege::setrendermode(ege::RENDER_MANUAL);
	ege::randomize();

	graph g[10];
	ege::fps f;

	for ( ; ege::is_run(); ege::delay_fps(60))
	{
		ege::cleardevice();
	}
	return 0;
}
