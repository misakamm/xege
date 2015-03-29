#include "graphics.h"
#include <time.h>
#include <math.h>

#define myrand(m) ((float)(randomf() * m ))

typedef struct
{
     float x, y;
     float vx, vy;
     int color;
}Point;

class AniObj
{
public:
     //��ʼ������������
     AniObj()
     {
         Init();
     }

     void Init()
     {
         n = 100;
         float x = myrand(600.0f) + 20.0f;
         float y = myrand(100.0f) + 100.0f;
         for (int i = 0; i < n; i++)
         {
             p[i].x = x;
             p[i].y = y;
             p[i].vx = 1.0f - myrand(2.0f);
             p[i].vy = 1.0f - myrand(2.0f);
             p[i].color = HSVtoRGB(myrand(360.0f), 1.0f, 1.0f);
         }
         color = HSVtoRGB(myrand(360.0f), 1.0f, 1.0f);
         start = rand() % 300;
         cnt = 0;
     }

     //����λ�õ��������
     void updateobj()
     {
         if (cnt++ > start)
             for (int i = 0; i < n; i++)
             {
                 p[i].vy += 0.01f;
                 p[i].x += p[i].vx;
                 p[i].y += p[i].vy;
             }
         if (cnt > start + 400) Init();
     }

     //��������ֵ�滭
     void drawobj()
     {
         for (int i = 0; i < n; i++)
         {
             putpixel(p[i].x, p[i].y, color);
         }
     }

     //�ͷ��������ʱ����
     ~AniObj()
     {
     }

private:
     Point p[100];
     int n;
     int color;
     int start;
     int cnt;
};


#define MAXOBJ 20

int main()
{
     initgraph(640, 480);
     randomize(); //��ʼ���������

     AniObj obj[MAXOBJ]; //�����������
     int n;

     setrendermode(RENDER_MANUAL);
     for ( ; kbhit() == 0; delay_fps(60) )
     {
         for (n = 0; n < MAXOBJ; ++n)
         {
             obj[n].updateobj(); //����λ��
         }

         imagefilter_blurring(NULL, 0x4F, 0x100);
         for (n = 0; n < MAXOBJ; ++n)
         {
             obj[n].drawobj(); //�滭
         }
     }

     closegraph();
     return 0;
}

