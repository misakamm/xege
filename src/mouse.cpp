#include "ege_head.h"
#include "ege_common.h"

#include "mouse.h"

#include <windowsx.h>

namespace ege
{

/*private function*/
static EGEMSG _getmouse(_graph_setting* pg)
{
    EGEMSG msg = {0};

    while (pg->msgmouse_queue->pop(msg)) {
        return msg;
    }
    return msg;
}

/*private function*/
static EGEMSG peekmouse(_graph_setting* pg)
{
    EGEMSG msg = {0};

    while (pg->msgmouse_queue->pop(msg)) {
        pg->msgmouse_queue->unpop();
        return msg;
    }
    return msg;
}

mouse_msg mouseMessageConvert(UINT message, WPARAM wParam, LPARAM lParam, int* key)
{
    mouse_msg msg = {0};

    /* WINAPI bug: WM_MOUSEWHEEL 提供的是屏幕坐标，DPI 不等于 100% 时 ScreenToClient 的计算
     * 结果与其它鼠标消息提供的坐标不一致。因此 lParam 参数已经处理过，直接使用之前记录的客户区坐标，
     * 这里不需要再进行转换。
     */
    msg.x = GET_X_LPARAM(lParam);
    msg.y = GET_Y_LPARAM(lParam);

    int vkCode = 0;

    if (message == WM_MOUSEMOVE) {
        msg.msg = mouse_msg_move;
    } else if (message == WM_MOUSEWHEEL) {
        msg.msg = mouse_msg_wheel;
        msg.wheel = GET_WHEEL_DELTA_WPARAM(wParam);
    } else  if ((message >= WM_XBUTTONDOWN) && (message <= WM_XBUTTONDBLCLK)) {
        switch(GET_XBUTTON_WPARAM(wParam)) {
            case XBUTTON1: msg.flags |= mouse_flag_x1; vkCode = key_mouse_x1; break;
            case XBUTTON2: msg.flags |= mouse_flag_x2; vkCode = key_mouse_x2; break;
            default: break;
        }

        switch (message) {
            case WM_XBUTTONDOWN:   msg.msg = mouse_msg_down;  break;
            case WM_XBUTTONUP:     msg.msg = mouse_msg_up;    break;
            //case WM_XBUTTONDBLCLK: msg.msg = mouse_msg_down;  break;
            default:break;
        }
    } else {
        switch(message) {
            case WM_LBUTTONDOWN:   msg.flags |= mouse_flag_left;  msg.msg = mouse_msg_down;  vkCode = key_mouse_l; break;
            case WM_LBUTTONUP:     msg.flags |= mouse_flag_left;  msg.msg = mouse_msg_up;    vkCode = key_mouse_l; break;
            //case WM_LBUTTONDBLCLK:  msg.flags |= mouse_flag_left; break;

            case WM_RBUTTONDOWN:   msg.flags |= mouse_flag_right; msg.msg = mouse_msg_down;  vkCode = key_mouse_r; break;
            case WM_RBUTTONUP:     msg.flags |= mouse_flag_right; msg.msg = mouse_msg_up;    vkCode = key_mouse_r; break;
            //case WM_RBUTTONDBLCLK: msg.flags |= mouse_flag_right; break;

            case WM_MBUTTONDOWN:   msg.flags |= mouse_flag_mid;   msg.msg = mouse_msg_down;  vkCode = key_mouse_m; break;
            case WM_MBUTTONUP:     msg.flags |= mouse_flag_mid;   msg.msg = mouse_msg_up;    vkCode = key_mouse_m; break;
            //case WM_MBUTTONDBLCLK: msg.flags |= mouse_flag_mid; break;
            default: break;
        }
    }

    /* 读取辅助键状态 */
    msg.flags |= (wParam & MK_CONTROL) ? mouse_flag_ctrl  : 0;
    msg.flags |= (wParam & MK_SHIFT)   ? mouse_flag_shift : 0;

    if (key != NULL) {
        *key = vkCode;
    }

    return msg;
}

void flushmouse()
{
    struct _graph_setting* pg = &graph_setting;
    EGEMSG                 msg;

    if (!pg->msgmouse_queue->empty()) {
        while (pg->msgmouse_queue->pop(msg)) {
            ;
        }
    }
}

int mousemsg()
{
    struct _graph_setting* pg = &graph_setting;
    if (pg->exit_window) {
        return 0;
    }
    EGEMSG msg;
    msg = peekmouse(pg);
    if (msg.hwnd) {
        return 1;
    }
    return 0;
}

mouse_msg getmouse()
{
    struct _graph_setting* pg = &graph_setting;
    mouse_msg mmsg = {0};

    if (pg->exit_window) {
        return mmsg;
    }

    do {
        EGEMSG msg = _getmouse(pg);
        if (msg.hwnd) {
            return mouseMessageConvert(msg.message, msg.wParam, msg.lParam);
        }
    } while (!pg->exit_window && !pg->exit_flag && waitdealmessage(pg));

    return mmsg;
}

MOUSEMSG GetMouseMsg()
{
    struct _graph_setting* pg   = &graph_setting;
    MOUSEMSG               mmsg = {0};
    if (pg->exit_window) {
        return mmsg;
    }

    EGEMSG msg = {0};
    do {
        msg = _getmouse(pg);
        if (msg.hwnd) {
            mmsg.uMsg    = msg.message;

            /* WINAPI bug: WM_MOUSEWHEEL 提供的是屏幕坐标，DPI 不等于 100% 时 ScreenToClient 的计算
             * 结果与其它鼠标消息提供的坐标不一致。因此 lParam 参数已经处理过，直接使用之前记录的客户区坐标，
             * 这里不需要再进行转换。
             */
            mmsg.x       = GET_X_LPARAM(msg.lParam);
            mmsg.y       = GET_Y_LPARAM(msg.lParam);

            /* mouse key states */
            mmsg.mkCtrl     = ((msg.wParam & MK_CONTROL)  != 0);
            mmsg.mkShift    = ((msg.wParam & MK_SHIFT)    != 0);
            mmsg.mkLButton  = ((msg.wParam & MK_LBUTTON)  != 0);
            mmsg.mkRButton  = ((msg.wParam & MK_RBUTTON)  != 0);
            mmsg.mkMButton  = ((msg.wParam & MK_MBUTTON)  != 0);
            mmsg.mkXButton1 = ((msg.wParam & MK_XBUTTON1) != 0);
            mmsg.mkXButton2 = ((msg.wParam & MK_XBUTTON2) != 0);

            if (msg.message == WM_MOUSEWHEEL) {
                mmsg.wheel = GET_WHEEL_DELTA_WPARAM(msg.wParam);
            }
            return mmsg;
        }
    } while (!pg->exit_window && waitdealmessage(pg));

    return mmsg;
}

}
