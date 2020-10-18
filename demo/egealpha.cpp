#include <graphics.h>
#include <math.h>

int main()
{
	initgraph( 640, 480 );

	setbkcolor(WHITE);
	setcolor(BLUE);
	setfillcolor(BLUE);
	bar(100,100,600,400);

	PIMAGE img=newimage(640,480);
	//set backgrounds alpha to 0
	setbkcolor(EGERGBA(0,0,0,0),img);

	//draw shapes with alpha to 255
	setfillcolor(EGEACOLOR(255,RED),img);
	setcolor(EGEACOLOR(255,RED),img);
	setlinestyle(CENTER_LINE,0,1,img);
	setlinewidth(10,img);
	ege_line(100,100,400,400,img);
	//change color
	setcolor(EGEACOLOR(255,GREEN),img);
	setlinestyle(DOTTED_LINE,0,1,img);
	setlinewidth(5,img);
	ege_ellipse(200,100,200,200,img);
	//set fill color
	setfillcolor(EGEACOLOR(255,LIGHTMAGENTA),img);
	ege_fillellipse(10,10,50,50,img);

	setfillcolor(EGEACOLOR(255,LIGHTBLUE),img);
	ege_fillellipse(100,10,50,50,img);

	putimage_withalpha(NULL,img,0,0);

	//putimage(0,0,img);
	delimage(img);

	getch();
	return 0;
}