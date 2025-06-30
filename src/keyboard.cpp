#include "ege_head.h"
#include "ege_common.h"

namespace ege
{

static int _getkey(_graph_setting* pg)
{
    EGEMSG msg;

    while (pg->msgkey_queue->pop(msg)) {
        if (msg.message == WM_CHAR) {
            return (KEYMSG_CHAR | ((int)msg.wParam & 0xFFFF));
        } else if (msg.message == WM_KEYDOWN) {
            return (KEYMSG_DOWN | ((int)msg.wParam & 0xFFFF) | (msg.lParam & 0x40000000 ? 0 : KEYMSG_FIRSTDOWN));
        } else if (msg.message == WM_KEYUP) {
            return (KEYMSG_UP | ((int)msg.wParam & 0xFFFF));
        }
    }
    return 0;
}

/*private function*/
static int peekkey(_graph_setting* pg)
{
    EGEMSG msg;

    while (pg->msgkey_queue->pop(msg)) {
        if (msg.message == WM_CHAR || msg.message == WM_KEYDOWN) {
            if (msg.message == WM_KEYDOWN) {
                if (msg.wParam <= key_space || (msg.wParam >= key_0 && msg.wParam < key_f1) ||
                    (msg.wParam >= key_semicolon && msg.wParam <= key_quote))
                {
                    continue;
                }
            }
            pg->msgkey_queue->unpop();
            if (msg.message == WM_CHAR) {
                return (KEYMSG_CHAR | ((int)msg.wParam & 0xFFFF));
            } else if (msg.message == WM_KEYDOWN) {
                if (msg.wParam >= 0x70 && msg.wParam < 0x80) {
                    return (KEYMSG_DOWN | ((int)msg.wParam + 0x100));
                }
                return (KEYMSG_DOWN | ((int)msg.wParam & 0xFFFF));
            } else if (msg.message == WM_KEYUP) {
                return (KEYMSG_UP | ((int)msg.wParam & 0xFFFF));
            }
        }
    }
    return 0;
}

/*private function*/
static int peekallkey(_graph_setting* pg, int flag)
{
    EGEMSG msg;

    while (pg->msgkey_queue->pop(msg)) {
        if ((msg.message == WM_CHAR    && (flag & KEYMSG_CHAR_FLAG)) ||
            (msg.message == WM_KEYUP   && (flag & KEYMSG_UP_FLAG)) ||
            (msg.message == WM_KEYDOWN && (flag & KEYMSG_DOWN_FLAG)))
        {
            pg->msgkey_queue->unpop();
            if (msg.message == WM_CHAR) {
                return (KEYMSG_CHAR | ((int)msg.wParam & 0xFFFF));
            } else if (msg.message == WM_KEYDOWN) {
                return (KEYMSG_DOWN | ((int)msg.wParam & 0xFFFF) | (msg.lParam & 0x40000000 ? 0 : KEYMSG_FIRSTDOWN));
            } else if (msg.message == WM_KEYUP) {
                return (KEYMSG_UP | ((int)msg.wParam & 0xFFFF));
            }
        }
    }
    return 0;
}

int getflush()
{
    struct _graph_setting* pg = &graph_setting;
    EGEMSG                 msg;
    int                    lastkey = 0;

    if (!pg->msgkey_queue->empty()) {
        while (pg->msgkey_queue->pop(msg)) {
            if (msg.message == WM_CHAR) {
                if (msg.message == WM_CHAR) {
                    lastkey = (int)msg.wParam;
                }
            }
        }
    }
    return lastkey;
}

int kbmsg()
{
    struct _graph_setting* pg = &graph_setting;
    if (pg->exit_window) {
        return 0;
    }
    return peekallkey(pg, 3);
}

int kbhitEx(int flag)
{
    struct _graph_setting* pg = &graph_setting;
    if (pg->exit_window) {
        return 0;
    }
    if (flag == 0) {
        return peekkey(pg);
    } else {
        return peekallkey(pg, flag);
    }
}

int getchEx(int flag)
{
    struct _graph_setting* pg = &graph_setting;
    if (pg->exit_window) {
        return -1;
    }

    {
        int    key;
        EGEMSG msg;
        DWORD  dw = GetTickCount();
        do {
            key = kbhitEx(flag);
            if (key < 0) {
                break;
            }
            if (key > 0) {
                key = _getkey(pg);
                if (key) {
                    msg = pg->msgkey_queue->last();
                    if (dw < msg.time + 1000) {
                        int ogn_key  = key;
                        int ret      = 0;
                        key         &= 0xFFFF;
                        ret          = key;
                        if (flag) {
                            ret = ogn_key;
                        } else {
                            if ((ogn_key & KEYMSG_DOWN) &&
                                ((msg.wParam >= 0x70 && msg.wParam < 0x80) || (msg.wParam > ' ' && msg.wParam < '0')))
                            {
                                ret |= 0x100;
                            }
                        }
                        return ret;
                    }
                }
            }
        } while (!pg->exit_window && !pg->exit_flag && waitdealmessage(pg));
    }
    return 0;
}

int ege_kbhit()
{
    return kbhitEx(0);
}

int ege_getch()
{
    return getchEx(0);
}

int kbhit()
{
    return ege_kbhit();
}

int getch()
{
    return ege_getch();
}

key_msg getkey()
{
    struct _graph_setting* pg  = &graph_setting;
    key_msg                ret = {0};
    if (pg->exit_window) {
        return ret;
    }

    {
        int key = 0;
        do {
            key = _getkey(pg);
            if (key) {
                key_msg msg = {0};
                if (key & KEYMSG_DOWN) {
                    msg.msg = key_msg_down;
                } else if (key & KEYMSG_UP) {
                    msg.msg = key_msg_up;
                } else if (key & KEYMSG_CHAR) {
                    msg.msg = key_msg_char;
                }
                msg.key = key & 0xFFFF;
                if (key & KEYMSG_FIRSTDOWN) {
                    msg.flags |= key_flag_first_down;
                }
                if (keystate(VK_CONTROL)) {
                    msg.flags |= key_flag_ctrl;
                }
                if (keystate(VK_SHIFT)) {
                    msg.flags |= key_flag_shift;
                }
                return msg;
            }
        } while (!pg->exit_window && !pg->exit_flag && waitdealmessage(pg));
    }
    return ret;
}

void flushkey()
{
    struct _graph_setting* pg = &graph_setting;
    EGEMSG                 msg;

    if (!pg->msgkey_queue->empty()) {
        while (pg->msgkey_queue->pop(msg)) {
            ;
        }
    }
}

bool keystate(int key)
{
    struct _graph_setting* pg = &graph_setting;
    if (key <= 0 || key >= MAX_KEY_VCODE) {
        return false;
    }

    return pg->keystatemap[key];
}

int keypress(int key)
{
    struct _graph_setting* pg = &graph_setting;

    int keyCount = 0;

    if (key > 0 && key < MAX_KEY_VCODE) {
        keyCount = pg->key_press_count[key];
        pg->key_press_count[key] = 0;           /* 计数清零*/
    }

    return keyCount;
}

int keyrelease(int key)
{
    struct _graph_setting* pg = &graph_setting;

    int keyCount = 0;

    if (key > 0 && key < MAX_KEY_VCODE) {
        keyCount                   = pg->key_release_count[key];
        pg->key_release_count[key] = 0; /* 计数清零*/
    }

    return keyCount;
}

int keyrepeat(int key)
{
    struct _graph_setting* pg = &graph_setting;

    int keyCount = 0;

    if (key > 0 && key < MAX_KEY_VCODE) {
        keyCount = pg->key_repeat_count[key];
        pg->key_repeat_count[key] = 0; /* 计数清零*/
    }

    return keyCount;
}

} // namespace ege
