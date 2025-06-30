#include <graphics.h>
#include <string.h>
#include <stdio.h>

#define MSG_LEN 200

/**
 * @brief 主函数
 *
 * 这个程序演示了使用图形库中的getimage函数读取和显示PNG和JPG图像文件。
 * 它首先读取一个PNG文件（getimage.png），并使用putimage_withalpha函数在坐标（50, 50）处以带有alpha透明度的方式显示图像。
 * 然后，它读取一个JPG文件（getimage.jpg），并使用putimage函数在坐标（200, 50）处显示图像。
 * 最后，它尝试读取一个不存在的PNG文件（getimage1.png），如果读取失败，则显示错误消息。
 */
int main()
{
	char msg[MSG_LEN+1];
	PIMAGE img;
	int result;
	initgraph( 640, 480 );
	setbkcolor(WHITE);

	// @note 读取一个PNG文件（getimage.png），并使用putimage_withalpha函数在坐标（50, 50）处以带有alpha透明度的方式显示图像
	img = newimage();
	result = getimage(img, "getimage.png");
	if (result != grOk) {
		sprintf(msg, "getimage(png) failed with %d.", result);
		outtextxy(0, 440, msg);
		getch();
		exit(-1);
	}
	putimage_withalpha(NULL, img, 50, 50);
	putimage_withalpha(NULL, img, 50, 200, 150, 150, 0, 0, getwidth(img), getheight(img));
	delimage(img);

	// @note 读取一个JPG文件（getimage.jpg），并使用putimage函数在坐标（200, 50）处显示图像。
	img = newimage();
	result = getimage(img, "getimage.jpg");
	if (result != grOk) {
		sprintf(msg, "getimage(jpg) failed with %d.", result);
		outtextxy(0, 440, msg);
		getch();
		exit(-1);
	}
	putimage(200, 50, img);
	delimage(img);

	// @note 尝试读取一个不存在的PNG文件（getimage1.png），如果读取失败，则显示错误消息。
	img = newimage();
	result = getimage(img, "getimage1.png");
	if (result != grOk) {
		sprintf(msg, "getimage(png) failed with %d.", result);
		outtextxy(0, 440, msg);
		getch();
		exit(-1);
	}
	putimage(50, 50, img);
	delimage(img);

	getch();
	closegraph();
	return 0;
}

