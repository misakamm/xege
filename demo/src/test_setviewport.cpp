#include "graphics.h"

#include <stdio.h>
#include <time.h>

int main()
{
    initgraph(400, 300);
    setfont(24, 12, "����");
    {
        char str[] = "������Ļʾ����Hello EGE !! Welcome to graphics programming !!!!!!~~~~~~";
        int w = textwidth(str);        //��¼����Ļ���������
        int view_x = 100, view_w = 200; //���ÿɼ�����λ�úʹ�С��ֻҪx����
        int t = clock(), roll_time = 10000; //��¼����ʼʱ�䣬�͹���������Ҫ��ʱ��
        setrendermode(RENDER_MANUAL);
        for ( ; ; delay(0))
        {
            int nt = clock(); //ȡ�õ�ǰʱ�䣬nt-t����ʱ��(nt-t) / roll_time ���ǵ�ǰʱ��Ӧ�ù����ı���
            cleardevice();
            if (nt - t > roll_time) //����ʱ��Ƚϣ�����Ѿ���ɣ�t=nt����������ʼʱ��
            {
                t = nt;
            }
            else
            {
                //���û�ͼ��������������Ĳ���滭���Դﵽ�ü���Ŀ�ģ�ͬʱ������(0,0)����ɴ��������￪ʼ��
                setviewport(view_x, 100, view_x + view_w, 300);
                //����������ʽ��view_w - (w + view_w) * (nt-t) / roll_time ��Ҫ��ϸ����һ��
                //����Ҫ����������ô��view_w���Ҷ˾��ǻ�׼��
                //w + view_w ��������Ҫ�������ܳ��ȣ��ܳ��ȳ��Ե�ǰ�Ĺ����������Եõ�ʵ�ʵ�λ��
                outtextxy(view_w - (w + view_w) * (nt-t) / roll_time, 0, str);
                setviewport(0, 0, 400, 300); //��ԭ��ͼ��
            }
        }
    }
    return 0;
}
