#include <graphics.h>

// ���峣��
#define ITERATIONS 1000     // ����������Խ�ߣ�ͼ��Խ��ϸ
#define MAXCOLOR    300     // ��ɫ����Խ��ͼ��Խƽ�����������ڵ�������


/////////////////////////////////////////////////
// ���帴�����ˡ�������
/////////////////////////////////////////////////

// ���帴��
struct COMPLEX
{
    double re;
    double im;
};

// ���帴�����ˡ�����
COMPLEX mul(COMPLEX a, COMPLEX b)
{
    COMPLEX c;
    c.re = a.re * b.re - a.im * b.im;
    c.im = a.im * b.re + a.re * b.im;
    return c;
}

// ���帴�����ӡ�����
COMPLEX add(COMPLEX a, COMPLEX b)
{
    COMPLEX c;
    c.re = a.re + b.re;
    c.im = a.im + b.im;
    return c;
}


/////////////////////////////////////////////////
// ������ɫ����ʼ����ɫ
/////////////////////////////////////////////////

// ������ɫ
int Color[MAXCOLOR];

// ��ʼ����ɫ
void InitColor()
{
    // ʹ�� HSL ��ɫģʽ�����Ƕ� h1 �� h2 �Ľ���ɫ
    int h1 = 240, h2 = 330, i;
    for (i=0; i<MAXCOLOR/2; i++)
    {
        Color[i] = HSLtoRGB((float)h1, 1.0f, i*2.0f/MAXCOLOR);
        Color[MAXCOLOR-1-i] = HSLtoRGB((float)h2, 1.0f, i*2.0f/MAXCOLOR);
    }
}

//��������������Ϊ��ʽ�е�C������ֵΪ����ʣ�����
int f(COMPLEX c)
{
    COMPLEX z = {0, 0}; //��ʼ��Ϊ0
    int maxcalc = ITERATIONS;  //����������
    while (--maxcalc)
    {
        z = mul(z, z);
        z = add(z, c);
        if ( z.re*z.re + z.im*z.im > 4.0 )
        {
            break; //��ģ����4���϶���ɢ������
        }
    }
    return maxcalc;
}


/////////////////////////////////////////////////
// ���� Mandelbrot Set (���²����ؼ�)
/////////////////////////////////////////////////
void Draw(double fromx, double fromy, double tox, double toy)
{
    COMPLEX z, c;
    int x, y;
    for (x=0; x<640; x++)
    {
        c.re = fromx + (tox - fromx) * (x / 640.0);
        for (y=0; y<480; y++)
        {
            int k;
            c.im = fromy + (toy - fromy) * (y / 480.0);
            k = f(c);
            if (k > 0) k = Color[(ITERATIONS - k) % MAXCOLOR];
            putpixel(x, y, k);
        }
    }
}


/////////////////////////////////////////////////
// ������
/////////////////////////////////////////////////
int main()
{
    double fromx, fromy, tox, toy;

    // ��ʼ����ͼ���ڼ���ɫ
    initgraph(640, 480);
    InitColor();


    // ��ʼ�� Mandelbrot Set(���²����ؼ�)����ϵ
    fromx = -2.2; tox = 2.2;
    fromy = -1.65; toy = 1.65;
    Draw(fromx, fromy, tox, toy);


    // ������������ʵ�ַŴ����ѡ������
    {
        MOUSEMSG m;
        bool isLDown = false;
        int selfx, selfy, seltx, selty; // ����ѡ��

        while (kbhit() != -1)
        {
            m = GetMouseMsg(); // ��ȡһ�������Ϣ

            switch (m.uMsg)
            {
                // ������Ҽ��ָ�ԭͼ������ϵ
            case WM_RBUTTONUP:
                fromx = -2.2; tox = 1.2;
                fromy = -1.65; toy = 1.65;
                Draw(fromx, fromy, tox, toy);
                break;

                // �����������϶���ѡ������
            case WM_MOUSEMOVE:
                if (isLDown)
                {
                    rectangle(selfx, selfy, seltx, selty);
                    seltx = m.x;
                    selty = m.y;
                    rectangle(selfx, selfy, seltx, selty);
                }
                break;

                // �����������϶���ѡ������
            case WM_LBUTTONDOWN:
                setcolor(WHITE);
                setwritemode(R2_XORPEN);
                isLDown = true;
                selfx = seltx = m.x;
                selfy = selty = m.y;
                rectangle(selfx, selfy, seltx, selty);

                break;

                // �����������϶���ѡ������
            case WM_LBUTTONUP:
                rectangle(selfx, selfy, seltx, selty);
                setwritemode(R2_COPYPEN);
                isLDown = false;
                seltx = m.x;
                selty = m.y;

                if (selfx == seltx || selfy == selty) break;

                // ����ѡ��Ϊ 4:3
                {
                    int tmp;
                    if (selfx > seltx)
                    {
                        tmp = selfx; selfx = seltx; seltx = tmp;
                    }
                    if (selfy > selty)
                    {
                        tmp = selfy; selfy = selty; selty = tmp;
                    }
                }

                if ( (seltx - selfx) * 0.75 < (selty - selfy) )
                {
                    selty += (3 - (selty - selfy) % 3);
                    selfx -= (selty - selfy) / 3 * 4 / 2
                             - (seltx - selfx) / 2;
                    seltx = selfx + (selty - selfy) / 3 * 4;
                }
                else
                {
                    seltx += (4 - (seltx - selfx) % 4);
                    selfy -= (seltx - selfx) * 3 / 4 / 2
                             - (selty - selfy ) / 2;
                    selty = selfy + (seltx - selfx ) * 3 / 4;
                }

                // ��������ϵ
                {
                    double f, t;
                    f = fromx + (tox - fromx) * selfx / 640;
                    t = fromx + (tox - fromx) * seltx / 640;
                    fromx = f;
                    tox = t;
                    f = fromy + (toy - fromy) * selfy / 480;
                    t = fromy + (toy - fromy) * selty / 480;
                    fromy = f;
                    toy = t;
                }

                // ��ͼ��
                Draw(fromx, fromy, tox, toy);
                break;
            }
        }
    }

    getch();
    closegraph();
    return 0;
}

