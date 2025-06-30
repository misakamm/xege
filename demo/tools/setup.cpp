#include "graphics.h"
#include <stdio.h>

char installpath[8][MAX_PATH];
char g_output[1024 * 16];
char strbasepath[] = "SOFTWARE\\";
char ver[8][64] = { "Microsoft\\VisualStudio\\6.0\\Setup\\Microsoft Viaual C++",
    "Microsoft\\VisualStudio\\8.0\\Setup\\VC",
    "Microsoft\\VisualStudio\\9.0\\Setup\\VC",
    "Microsoft\\VisualStudio\\10.0\\Setup\\VC",
    "C-Free\\5",
};

#define SC_W 640
#define SC_H 240

double g_zoom = 0.1;

class Mira
{
public:
    Mira(int w, int h)
    {
        m_a = 0.1 + random(10000) / 10000.0 * 1.6 - 0.8, m_b = 0.99;
        m_da = 0.0002 * ((int)random(2) * 2 - 1), m_db = 0.0000061 * ((int)random(2) * 2 - 1);
        m_cr = 0.0;
        m_tt = random(10000) / 10000.0 * 16.0 + 4;
        m_zoom = 0.7 / m_tt;
        pmira = new IMAGE(w, h);
        m_w = w;
        m_h = h;
    }
    ~Mira()
    {
        delete pmira;
    }
    void drawpixel(double _x, double _y, int color)
    {
        double dt = (m_h + m_w) * m_zoom / 4;
        int x = (int)((_x - 1.0) * dt + m_w / 2), y = (int)(_y * dt + m_h / 2);
        if (x >= 0 && x < m_w && y >= 0 && y < m_h)
        {
            putpixel_f(x, y, color, pmira);
        }
    }

    int mira(double a, double b, double x, double y, int niter, int color)
    {
        int i;
        double c, t, u, w;
        c = 2 - 2 * a;
        w = a * x + c * (x * x) / (1 + x * x);
        for (i = 0; i < niter; ++i)
        {
            if (i > 8) drawpixel(x, y, color);
            t = x;
            x = b * y + w;
            u = x * x;
            w = a * x + c * u / (1 + u);
            y = w - t;
        }
        return 0;
    }
    int update()
    {
        m_a += m_da;
        m_b += m_db;
        if (m_a > 1.0 && m_da > 0) m_da = -m_da;
        if (m_a < -1.0 && m_da < 0) m_da = -m_da;
        if (m_b >= 1 && m_db > 0) m_db = -m_db;
        if (m_b < 1-0.01 && m_db < 0) m_db = -m_db;
        m_cr += 2.5;
        if (m_cr >= 360) m_cr -= 360;
        return 0;
    }
    int render(int _x, int _y)
    {
        imagefilter_blurring(pmira, 0x30, 0x100);
        int color = HSVtoRGB((float)m_cr, 1.0f, 1.0f);
        int e = 1;
        for (int y = -e; y <= e; ++y)
        {
            if (y == 0) continue;
            for (int x = -e; x <= e; ++x)
            {
                if (x == 0) continue;
                mira(m_a, m_b, x * m_tt, y * m_tt, 900, color);
            }
        }
        putimage(_x, _y, pmira);
        return 0;
    }
private:
    double m_a, m_b;
    double m_da, m_db;
    double m_cr, m_tt;
    double m_zoom;
    int m_w, m_h;
    PIMAGE pmira;
};


int info_scene()
{
    cleardevice();
    setcolor(0xFFFFFF);
    Mira mira(640, 300);
    IMAGE imgtext(440, 130);
    char infostr[] = "欢迎使用Easy Graphics Enginge (EGE) V0.3.8 ，本库是一个面向新手，或者面向快速图形程序开发的图形库，使用方便快捷，容易上手，特别适合于新手学习图形程序设计。本程序为安装程序，如果你要继续安装，请按'y'键继续";
    setcolor(0xFFFF, &imgtext);
    setfont(18, 0, "宋体", &imgtext);
    setbkmode(TRANSPARENT, &imgtext);
    for (int i = 0; i < 8; ++i)
    {
        outtextrect(5, 5, 440, 280, infostr, &imgtext);
        imagefilter_blurring(&imgtext, 0xF0, 0x100);
    }
    setcolor(0xFF, &imgtext);
    outtextrect(5, 5, 440, 280, infostr, &imgtext);

    BeginBatchDraw();
    for ( ; kbhit() == 0; delay_fps(60))
    {
        mira.update();
        mira.render(0, 480 - 300);
        putimage(100, 50, &imgtext);
    }
    EndBatchDraw();
    return getch();
}

int getpath_scene()
{
    //HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio
    char strpath[MAX_PATH];
    HKEY key;
    int it;
    cleardevice();
    setfont(18, 0, "宋体");

    outtextrect(100, 30, 440, 280, "以下显示本程序所搜索到的IDE安装路径，如果有找不到的，请把本程序连同子文件夹一起复制到安装目录下，然后再在那个目录下运行本程序。例如如果你使用的是VC6的绿色版，那就可能会找不到，假如你安装在'D:\\Program Files\\Microsoft Visual Studio\\'，那你只需要把本程序目录下的include, lib目录复制到'D:\\Program Files\\Microsoft Visual Studio\\VC98\\'，并确认文件夹合并就可以了。\n如果你要继续安装，请按Y键继续");

    for (it = 0; ver[it][0]; ++it)
    {
        sprintf(strpath, "%s%s", strbasepath, ver[it]);
        if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strpath, 0, KEY_READ, &key) == ERROR_SUCCESS)
        {
            DWORD dwtype = REG_SZ;
            DWORD dwsize = MAX_PATH;
            if (::RegQueryValueEx(key, "ProductDir", NULL, &dwtype, (BYTE*)(installpath[it]), &dwsize))
            {
                ::RegQueryValueEx(key, "InstallDir", NULL, &dwtype, (BYTE*)(installpath[it]), &dwsize);
                strcat(installpath[it], "\\mingw");
            }
            ::RegCloseKey(key);
        }
        outtextxy(100, it * 20 + 250, installpath[it]);
    }

    //::GetCurrentDirectory(MAX_PATH, installpath[it]);
    //outtextxy(100, it * 20 + 250, installpath[it]);
    return getch();
}

int copyfile(char* path1, char* pathnew, char* dir,char* file)
{
    char strpath1[MAX_PATH];
    char strpath2[MAX_PATH];
    if (path1[strlen(path1) - 1] == '\\')
    {
        sprintf(strpath1, "%s%s\\%s", path1, dir, file);
    }
    else
    {
        sprintf(strpath1, "%s\\%s\\%s", path1, dir, file);
    }
    if (pathnew[strlen(pathnew) - 1] == '\\')
    {
        sprintf(strpath2, "%s%s\\%s", pathnew, dir, file);
    }
    else
    {
        sprintf(strpath2, "%s\\%s\\%s", pathnew, dir, file);
    }
    int ret = ::CopyFile(strpath1, strpath2, FALSE);
    if (ret == 0)
    {
        sprintf(strpath1, "Copy %s ERROR\n", strpath2);
        strcat(g_output, strpath1);
    }
    else
    {
        sprintf(strpath1, "Copy %s SUCCESS\n", strpath2);
        strcat(g_output, strpath1);
    }
    return ret;
}

int setup_scene()
{
    cleardevice();
    int it;
    g_output[0] = 0;
    setfont(14, 0, "宋体");
    for (it = 0; ver[it][0]; ++it)
    {
        if (installpath[it][0] == 0) continue;
        copyfile(".\\", installpath[it], "include", "graphics.h");
        if (it == 0) copyfile(".\\", installpath[it], "lib", "graphics.lib");
        if (it == 1) copyfile(".\\", installpath[it], "lib", "graphics05.lib");
        if (it == 2 || it == 3 ) copyfile(".\\", installpath[it], "lib", "graphics08.lib");
        if (it == 4) copyfile(".\\", installpath[it], "lib", "libgraphics.a");
    }
    if (g_output[0])
    {
        outtextxy(10, 10, "以下为复制记录列表，如果有发生错误的话，可能需要你手工安装");
    }
    outtextrect(10, 30, 600, 400, g_output);
    outtextxy(10, 400, "安装步骤执行完成，按任意键退出本程序");
    return getch();
}

void setup()
{
    int ret, i;
    ret = info_scene();

    for(i = 0; i < 60 * 3; ++i)
    {
        imagefilter_blurring(NULL, 0xF0, 0x100);
        delay_fps(60);
    }
    if (ret != 'y' && ret != 'Y') return;

    ret = getpath_scene();

    for(i = 0; i < 60 * 1; ++i)
    {
        imagefilter_blurring(NULL, 0xF0, 0x100);
        delay_fps(60);
    }
    if (ret != 'y' && ret != 'Y') return;

    setup_scene();

    for(i = 0; i < 60 * 1; ++i)
    {
        imagefilter_blurring(NULL, 0xF0, 0x100);
        delay_fps(60);
    }
    //for ( ; kbhit() != -1; delay_fps(60))
    {
    }

}

int main()
{
    initgraph(640, 480);
    randomize();

    setup();
    closegraph();
    return 0;
}


