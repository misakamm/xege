#include <graphics.h>
#include "ege_head.h"
#include <math.h>

int main()
{
	int points[6];
    initgraph( 640, 480 );
    setbkcolor(WHITE);
    setcolor( BLACK);
    setfillcolor( RED );
    
    bar(50,50,600,400);    
   
    PIMAGE  img = newimage(200,200);
    setbkcolor(WHITE,img);
	setcolor(RED,img);
	setfillcolor(YELLOW,img);

	points[0]=100;
	points[1]=50;
	points[2]=50;
	points[3]=150;
	points[4]=150;
	points[5]=150;
	fillpoly(3,points,img);

	putimage_rotatetransparent(NULL,img,300,200,100,100,WHITE,45.0/180*PI,2);
    getch();
    return 0;
}
