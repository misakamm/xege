#include <graphics.h>
#include <ege/fps.h>
#include <time.h>
#include <math.h>

struct scene {
	int x, y, r;
	int delay;
	int hit;
	int cnt;
};

void init_scene(scene& s) {
	randomize();
	s.r = random(50) + 50;
	s.x = random(getwidth() - s.r * 2) + s.r;
	s.y = random(getheight() - s.r * 2) + s.r;
	s.delay = 12;
	s.hit = 0;
	s.cnt = 0;
}

int distance(int x1, int y1, int x2, int y2) {
	return (int)(sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))+.3);
}

void mainloop (scene& s) {
	for ( ; is_run(); delay_fps(120) ) {
		while (mousemsg()) {
			mouse_msg m = getmouse();
			if (m.msg == mouse_msg_down) {
				if (distance(m.x, m.y, s.x, s.y) <= s.r) {
					s.hit = 1;
				}
			}
		}
		if (s.hit) {
			--s.delay;
			if (s.delay == 0) {
				s.r *= 2;
			} else if (s.delay <= -30) {
				s.r = random(50) + 50;
				s.x = random(getwidth() - s.r * 2) + s.r;
				s.y = random(getheight() - s.r * 2) + s.r;
				s.delay = (s.cnt > 50 ? 1 : 12+6);
				s.hit = 0;
				s.cnt ++;
			}
		}
		cleardevice();
		setfillcolor(EGERGB(0x80, 0x80, 0xF0));
		fillellipse(s.x, s.y, s.r, s.r);
		xyprintf(0, 15, "%d", s.cnt);
	}
}

int main()
{
	initgraph( 640, 480 );
	setrendermode(RENDER_MANUAL);
	fps f;
	scene s;
	init_scene(s);
	mainloop(s);
	closegraph();
	return 0;
}

