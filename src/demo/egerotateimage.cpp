// 旋转图片演示程序
#include <graphics.h>
#include <ege/fps.h>
#include <time.h>
#include <math.h>

int main()
{
	initgraph( 640, 480 );
	setrendermode(RENDER_MANUAL);
	PIMAGE img = newimage();
	getimage(img, "bg.jpg");
	double r = 0;
	fps f;
	for ( ; is_run(); delay_fps(60) )
	{
		r += 0.02;
		if (r > PI * 2) r -= PI * 2;

		cleardevice();
		putimage_rotatezoom(NULL, img, 320, 240, 0.5f, 0.5f, r, 0.5f, 0, -1, 1);
	}
	return 0;
}

