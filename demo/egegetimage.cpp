#include <graphics.h>
#include <string.h> 
#include <stdio.h> 

#define MSG_LEN 200
int main()
{
	char msg[MSG_LEN+1];
	PIMAGE img;
	int result;
    initgraph( 640, 480 );
    setbkcolor(WHITE);
	
	// test read png file
	img=newimage();
	result = getimage(img,"getimage.png");
	if (result != grOk) {
		sprintf(msg,"getimage(png) failed with %d.",result);
		outtextxy(0,440,msg);
		getch();
		exit(-1);
	}    
	putimage_withalpha(NULL,img,50,50);
	delimage(img);
	
	// test read jpg file
	img=newimage();
	result = getimage(img,"getimage.jpg");
	if (result != grOk) {
		sprintf(msg,"getimage(jpg) failed with %d.",result);
		outtextxy(0,440,msg);
		getch();
		exit(-1);
	}    
	putimage(200,50,img);
	delimage(img);
	
	// test read png file and fail
	img=newimage();
	result = getimage(img,"getimage1.png"); // this file doesn't exist.
	if (result != grOk) {
		sprintf(msg,"getimage(png) failed with %d.",result);
		outtextxy(0,440,msg);
		getch();
		exit(-1);
	}    
	putimage(50,50,img);
	delimage(img);
	
	
	getch();
	closegraph();
    return 0;
}
