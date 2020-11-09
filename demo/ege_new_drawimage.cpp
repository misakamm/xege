#include <graphics.h>

int main()
{
	initgraph( 800, 600 );
	// draw background image
	setbkcolor(WHITE);
	setcolor( BLACK);
	setfillcolor( BLUE );
	
	for (int i=0;i<12;i++) {
		bar(0,i*50,800,i*50+20);
	}
   
   	//image to be rotated and put
	PIMAGE  img = newimage(200,200);
	setbkcolor(EGERGBA(0,0,0,0),img);
	setcolor(RED,img);
	setfillcolor(YELLOW,img);
	
	ege_point points[4];
	points[0].x=100;
	points[0].y=50;
	points[1].x=50;
	points[1].y=150;
	points[2].x=150;
	points[2].y=150;
	points[3].x=100;
	points[3].y=50;
	ege_fillpoly(3,points,img);
	ege_drawpoly(4,points,img);
	
	//draw without scale
	ege_drawimage(img,0,0);
	
	//draw with scale
	ege_drawimage(img,200,0,400,300,0,0,200,200);

	//draw with transform
	ege_transform_matrix m;
	//save old transform
	ege_get_transform(&m);
	ege_transform_translate(400,450); //center of bottom half window
	ege_transform_rotate(45.0); // rotate
	ege_transform_translate(-getwidth(img)/2, -getheight(img)/2); // center of src image
	ege_drawimage(img,0,0); 
	//restore old transform
	ege_set_transform(&m);
	
	//use it to clear transform
	//ege_transform_reset();
	
	//draw to test transform is correctly restored
	ege_drawimage(img,600,400);
	
	getch();
		
	delimage(img);

	return 0;
}
