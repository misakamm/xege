#include <graphics.h>
#include <stdio.h>
#include <ege/sys_edit.h>

#define BUFFER_LEN 200

int main()
{
	initgraph(640, 480);
	setbkcolor(WHITE);

	char buffer[BUFFER_LEN+1];

	sys_edit editBox;
	editBox.create(false);
	editBox.size(200, 24 + 8);
	editBox.setbgcolor(YELLOW);
	editBox.setfont(24, 0, "黑体");
	editBox.move(40, 40);
	editBox.visible(true);

	sys_edit editBox1;
	editBox1.create(false);
	editBox1.size(200, 24+8);
	editBox1.setbgcolor(YELLOW);
	editBox1.setfont(24, 0, "黑体");
	editBox1.move(40, 140);
	editBox1.visible(true);

	editBox.setfocus();

	//处理交互，不可少，否则直接退出
	key_msg msg;
	while (is_run()) {
		msg = getkey();
		if ((msg.msg == key_msg_down) && (msg.key == key_enter)) {
			if (editBox.isfocus()) {
				//跳到下一个输入框
				editBox1.setfocus();
			} else if (editBox1.isfocus()) {
				//结束输入
				flushkey();
				break;
			}
		}
	}

	editBox.gettext(BUFFER_LEN,buffer);

	xyprintf(40, 80, buffer);

	editBox1.gettext(BUFFER_LEN,buffer);

	xyprintf(40, 180, buffer);


	getch();
	closegraph();

	return 0;
}
