#include "ege_head.h"
#include "ege_common.h"
#include "ege_extension.h"

namespace ege
{

void api_sleep(long ms)
{
    if (ms >= 0) {
        dll::timeBeginPeriod(1);
        ::Sleep(ms);
        dll::timeEndPeriod(1);
    }
}

void ege_sleep(long ms)
{
    if (ms <= 0) {
        return;
    }

    if (0) { // 经济模式，占CPU极少
        ::Sleep(ms);
    } else if (0) { // 精确模式，占CPU略高
        static HANDLE hTimer = ::CreateEvent(NULL, TRUE, FALSE, NULL);
        static MMRESULT resTimer = 0;
        ::ResetEvent(hTimer);
        if (resTimer) {
            dll::timeKillEvent(resTimer);
        }
        resTimer = dll::timeSetEvent(ms, 1, (LPTIMECALLBACK)hTimer, 0, TIME_ONESHOT | TIME_CALLBACK_EVENT_SET);
        if (resTimer) {
            ::WaitForSingleObject(hTimer, INFINITE);
        } else {
            ::Sleep(1);
        }
        //::CloseHandle(hTimer);
    } else if (1) { // 高精模式，占CPU更高
        static HANDLE hTimer = ::CreateWaitableTimer(NULL, TRUE, NULL);
        LARGE_INTEGER liDueTime;

        dll::timeBeginPeriod(1);
        liDueTime.QuadPart = ms * (LONGLONG)-10000;

        if (hTimer) {
            if (::SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, FALSE)) {
                ::WaitForSingleObject(hTimer, INFINITE); // != WAIT_OBJECT_0;
            }
            //::CloseHandle(hTimer);
        } else {
            ::Sleep(ms);
        }
        dll::timeEndPeriod(1);
    }
}

void delay(long ms)
{
    ege_sleep(ms);
}

/**
 * 刷新后延时。（刷新：处理事件以及交换双缓冲）
 * @param ms 延时时间，单位为毫秒
 */
void delay_ms(long ms)
{
    struct _graph_setting* pg = &graph_setting;
    egeControlBase* root = pg->egectrl_root;
    pg->skip_timer_mark = true;

    const double targetTime = get_highfeq_time_ls(pg) * 1000.0 + ms;

    /* 处理 UI 事件，更新 UI 控件数据 */
    guiupdate(pg, root);

    /* 绘图后重绘UI并交换缓冲区 */
    if (needToUpdate(pg)) {
        root->draw(NULL);
        graphupdate(pg);
    }

    /* 延时 */
    if (ms == 0) {
        /* Sleep(0): 让出 CPU 时间片，处理后立即返回 */
        ::Sleep(0);
    } else if (ms > 0) {
        double currentTime = get_highfeq_time_ls(pg) * 1000.0;

        if (currentTime < targetTime) {
            ege_sleep((long)(targetTime - currentTime));
        }
    } else {
        /* ms < 0: Do nothing.*/
    }

    pg->skip_timer_mark = false;
}

/*
延迟1/fps的时间，调用间隔不大于200ms时能保证每秒能返回fps次
*/
void delay_fps(int fps)
{
    delay_fps((double)fps);
}

void delay_fps(long fps)
{
    delay_fps((double)fps);
}

void delay_fps(double fps)
{
    struct _graph_setting* pg = &graph_setting;
    egeControlBase* root = pg->egectrl_root;
    pg->skip_timer_mark = true;
    double delay_time = 1000.0 / fps;
    double avg_max_time = delay_time * 10.0; // 误差时间在这个数值以内做平衡
    double dw = get_highfeq_time_ls(pg) * 1000.0;
    int nloop = 0;

    if (pg->delay_fps_dwLast == 0) {
        pg->delay_fps_dwLast = get_highfeq_time_ls(pg) * 1000.0;
    }

    if (pg->delay_fps_dwLast + delay_time + avg_max_time > dw) {
        dw = pg->delay_fps_dwLast;
    }

    root->draw(NULL);

    for (; nloop >= 0; --nloop) {
        if ((dw + delay_time + (100.0) >= get_highfeq_time_ls(pg) * 1000.0)) {
            do {
                ege_sleep((int)(dw + delay_time - get_highfeq_time_ls(pg) * 1000.0));
            } while (dw + delay_time >= get_highfeq_time_ls(pg) * 1000.0);
        }

        dealmessage(pg, FORCE_UPDATE);
        dw = get_highfeq_time_ls(pg) * 1000.0;
        guiupdate(pg, root);

        if (pg->delay_fps_dwLast + delay_time + avg_max_time <= dw || pg->delay_fps_dwLast > dw) {
            pg->delay_fps_dwLast = dw;
        } else {
            pg->delay_fps_dwLast += delay_time;
        }
    }
    pg->skip_timer_mark = false;
}

/*
延迟1/fps的时间，调用间隔不大于200ms时能保证每秒能返回fps次
*/
void delay_jfps(int fps)
{
    delay_jfps((double)fps);
}

void delay_jfps(long fps)
{
    delay_jfps((double)fps);
}

void delay_jfps(double fps)
{
    struct _graph_setting* pg = &graph_setting;
    egeControlBase* root = pg->egectrl_root;
    pg->skip_timer_mark = true;
    double delay_time = 1000.0 / fps;
    double avg_max_time = delay_time * 10.0;
    double dw = get_highfeq_time_ls(pg) * 1000.0;
    int nloop = 0;

    if (pg->delay_fps_dwLast == 0) {
        pg->delay_fps_dwLast = get_highfeq_time_ls(pg) * 1000.0;
    }

    if (pg->delay_fps_dwLast + delay_time + avg_max_time > dw) {
        dw = pg->delay_fps_dwLast;
    }

    root->draw(NULL);

    for (; nloop >= 0; --nloop) {
        int bSleep = 0;

        while (dw + delay_time >= get_highfeq_time_ls(pg) * 1000.0) {
            ege_sleep((int)(dw + delay_time - get_highfeq_time_ls(pg) * 1000.0));
            bSleep = 1;
        }

        if (bSleep) {
            dealmessage(pg, FORCE_UPDATE);
        } else {
            updateFrameRate(false);
        }

        dw = get_highfeq_time_ls(pg) * 1000.0;
        guiupdate(pg, root);

        if (pg->delay_fps_dwLast + delay_time + avg_max_time <= dw || pg->delay_fps_dwLast > dw) {
            pg->delay_fps_dwLast = dw;
        } else {
            pg->delay_fps_dwLast += delay_time;
        }
    }
    pg->skip_timer_mark = false;
}

double get_highfeq_time_ls(struct _graph_setting* pg)
{
    static LARGE_INTEGER llFeq = {{0}}; /* 此实为常数 */
    LARGE_INTEGER llNow = {{0}};

    if (pg->get_highfeq_time_start.QuadPart == 0) {
        if (1) {
            SetThreadAffinityMask(::GetCurrentThread(), 0);
            QueryPerformanceCounter(&pg->get_highfeq_time_start);
            QueryPerformanceFrequency(&llFeq);
        } else if (0) {
            dll::timeBeginPeriod(1);
            pg->get_highfeq_time_start.QuadPart = ::timeGetTime();
            dll::timeEndPeriod(1);
            llFeq.QuadPart = 1000;
        } else if (1) {
            pg->get_highfeq_time_start.QuadPart = ::GetTickCount();
            llFeq.QuadPart = 1000;
        } else if (0) {
            ::GetSystemTimeAsFileTime((LPFILETIME)&pg->get_highfeq_time_start);
            llFeq.QuadPart = 10000000;
        }
        return 0;
    } else {
        if (1) {
            QueryPerformanceCounter(&llNow);
        } else if (0) {
            dll::timeBeginPeriod(1);
            llNow.QuadPart = ::timeGetTime();
            dll::timeEndPeriod(1);
        } else if (1) {
            llNow.QuadPart = ::GetTickCount();
        } else if (0) {
            ::GetSystemTimeAsFileTime((LPFILETIME)&llNow);
        }
        llNow.QuadPart -= pg->get_highfeq_time_start.QuadPart;
        return (double)llNow.QuadPart / llFeq.QuadPart;
    }
}


} //namespace ege
