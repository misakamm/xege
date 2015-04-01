#include <graphics.h>
typedef struct chartarget
{
    float x, y;
    float dy;
    char c;
    char vis;
}chartarget;
int movechar(chartarget* ct)
{
    ct->y += ct->dy;
    if (ct->y > getheight() || ct->vis == 0) return 1;
    return 0;
}
void drawchar(chartarget* ct)
{
    outtextxy((int)ct->x, (int)ct->y, ct->c);
}
void movechars(int* nobj, chartarget* ct, int bnew)
{
    int i;
    if (bnew)
    {
        int n = (*nobj)++;
        ct[n].x = (float)(random(getwidth() - 40) + 20 - 9);
        ct[n].y = -50;
        ct[n].dy = (float)(randomf() * 3 + 1);
        ct[n].c = random(26) + 'a';
        ct[n].vis = 1;
    }
    for (i = 0; i < *nobj; ++i)
    {
        if (movechar(ct + i))
        {
            ct[i] = ct[--*nobj];
        }
    }
}
int main()
{
    initgraph(400, 400);
    randomize();
    chartarget ct[256];
    int nobj = 0;
    int t = 1000;
    setfont(36, 0, "ËÎÌו");
    setcolor(WHITE);
    setbkmode(TRANSPARENT);
    setrendermode(RENDER_MANUAL);
    for ( ; kbhit() != -1; delay_fps(60))
    {
        int bnew = 0, i;
        if (++t > 30) bnew = 1, t = 0;
        while (kbhit())
        {
            int key = getch();
            int my = -1;
            if (key >= 'A' && key <= 'Z') key += 'a' - 'A';
            if (key < 'a' || key > 'z') continue;
            for (i = 0; i < nobj; ++i)
            {
                if (ct[i].vis == 0 || ct[i].c != key) continue;
                if (my < 0) my = i;
                else if (ct[i].y > ct[my].y) my = i;
            }
            if (my >= 0)
            {
                ct[my].vis = 0;
            }
        }
        movechars(&nobj, ct, bnew);
        //cleardevice();
        imagefilter_blurring(NULL, 0x40, 0xD0);
        for (i = 0; i < nobj; ++i)
        {
            drawchar(ct + i);
        }
    }
    return 0;
}

