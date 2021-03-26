#define SHOW_CONSOLE
#include "ege.h"


using namespace ege;

int main() {
	initgraph(640, 640);
	char buf[1024] = "Default";
	inputbox_getline("≤‚ ‘", "≤‚ ‘", buf, 1024);
	outtextxy(0, 10, buf);
	getch();
	return 0;
}
