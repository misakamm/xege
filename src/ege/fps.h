#ifndef _EGE_FPS_H_
#define _EGE_FPS_H_

#ifndef _EGE_H_
#error include "fps.h" must after include "ege.h" or "graphics.h"
#endif

namespace ege {

class fps : public egeControlBase
{
public:
    CTL_PREINIT(fps, egeControlBase) {
        // do sth. before sub objects' construct function call
    } CTL_PREINITEND;
    fps(CTL_DEFPARAM) : CTL_INITBASE(egeControlBase) {
        CTL_INIT; // must be the first line
        directdraw(true);
        enable(false);
    }

    void onDraw(PIMAGE pimg) const {
        char str[16] = "fps        ", *pstr = str;
        double fps = getfps() + 0.005;
        int a = (int)fps, b = (int)((fps - a) * 100);
        while (*pstr) ++pstr;
        pstr[-1] = (char)(b % 10 + '0');
        pstr[-2] = (char)(b / 10 + '0');
        pstr[-3] = '.';
        pstr -= 4;
        for ( ; a > 0; --pstr) {
            pstr[0] = a % 10 + '0';
            a /= 10;
        }
        setcolor(WHITE, pimg);
        setfillcolor(BLACK, pimg);
        setbkmode(OPAQUE, pimg);
        setfont(12, 0, "ËÎÌו", pimg);
        outtextxy(0, 0, str, pimg);
    }
};

} // namespace ege
#endif /*_EGE_FPS_H_*/
