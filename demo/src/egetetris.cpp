#include "graphics.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

#include "ege/fps.h"

const int g_width = 400;
const int g_height = 520;

/*记录7种形状及其4种变化的表*/
static int g_trs_map[8][4][4][4];
/*变化数目表*/
static int g_map_mod[] = {1, 4, 4, 4, 2, 2, 2, 1, 0};

/*初始化全局数据及图形显示*/
void initgr() {
    initgraph(g_width, g_height);
    setfont(12, 6, "宋体");
    int Trs_map[8][4][4][4] =
    {
        {{{0}}},{{
            {0,0,0,0},{1,1,1,0},{0,1,0,0},
        },{
            {0,1,0,0},{1,1,0,0},{0,1,0,0},
        },{
            {0,1,0,0},{1,1,1,0},
        },{
            {0,1,0,0},{0,1,1,0},{0,1,0,0},
        },},{{
            {2,2,0,0},{0,2,0,0},{0,2,0,0},
        },{
            {0,0,2,0},{2,2,2,0},
        },{
            {0,2,0,0},{0,2,0,0},{0,2,2,0},
        },{
            {0,0,0,0},{2,2,2,0},{2,0,0,0},
        },},{{
            {0,3,3,0},{0,3,0,0},{0,3,0,0},
        },{
            {0,0,0,0},{3,3,3,0},{0,0,3,0},
        },{
            {0,3,0,0},{0,3,0,0},{3,3,0,0},
        },{
            {3,0,0,0},{3,3,3,0},
        },},{{
            {4,4,0,0},{0,4,4,0},
        },{
            {0,0,4,0},{0,4,4,0},{0,4,0,0},
        },},{{
            {0,5,5,0},{5,5,0,0},
        },{
            {0,5,0,0},{0,5,5,0},{0,0,5,0},
        },},{{
            {0,0,0,0},{6,6,6,6},
        },{
            {0,0,6,0},{0,0,6,0},{0,0,6,0},{0,0,6,0},
        },},{{
            {0,0,0,0},{0,7,7,0},{0,7,7,0},
        },},
    };
    memcpy(g_trs_map, Trs_map, sizeof(Trs_map));
}

class Game {
public:
    /*状态表*/
    enum {
        ST_START, /*游戏重新开始*/
        ST_NEXT,  /*准备下一个方块*/
        ST_NORMAL,/*玩家控制阶段*/
        ST_OVER   /*游戏结束，F2重新开始*/
    };
    Game(int w, int h, int bw, int bh) {
        int colormap[10] = {0, 0xA00000, 0xA05000, 0xA0A000, 0xC000,
            0x00A0A0, 0x4040C0, 0xA000A0, 0x808080, 0xFFFFFF};
        memcpy(m_colormap, colormap, sizeof(m_colormap));

        int Keys[8] = {VK_F2,VK_LEFT,VK_RIGHT,VK_DOWN,VK_UP,VK_NUMPAD0,VK_SPACE};
        memcpy(m_Keys, Keys, sizeof(Keys));

        memset(m_KeyState, 0, sizeof(m_KeyState));
        m_gamepool_w = w;
        m_gamepool_h = h;
        m_base_w = bw;
        m_base_h = bh;

        randomize();
        m_ctl_t = -1;
        m_pcb = newimage();
        for (int i=0; i<10; ++i) {
            drawtile(bw * i, 0, bw, bh, 5, colormap[i]);
        }
        getimage(m_pcb, 0, 0, bw*10, bh);
        m_state = ST_START;
    }
    /*状态转换处理*/
    int deal () {
        int nRet = 0;
        if ( m_state == ST_START ) { //初始化 
            m_next1_s = random(7) + 1;
            m_next2_s = random(7) + 1;
            m_pause = 0;
            memset(m_gamepool, 255, sizeof(m_gamepool));
            for (int y = 1; y <= m_gamepool_h; ++y) {
                for (int x = 1; x <= m_gamepool_w; ++x)
                    m_gamepool[y][x] = 0;
            }
            m_forbid_down = 0;
            m_ctl_t = -1;
            nRet = 1;
            m_state = ST_NEXT;
        } else if ( m_state == ST_NEXT ) {
            m_ctl_x = (m_gamepool_w - 4) / 2 + 1;
            m_ctl_y = 1;
            m_ctl_t = 0;
            m_ctl_s = m_next1_s;
            m_ctl_dy = 0;
            m_next1_s = m_next2_s;
            m_next2_s = random(7) + 1;
            m_curtime = m_droptime;
            m_curxtime = 0;
            nRet = 1;
            if ( isCrash() ) {
                m_gray_y = m_gamepool_h * 2;
                m_over_st = 0;
                m_state = ST_OVER;
            } else {
                m_state = ST_NORMAL;
            }
        } else if (m_state == ST_NORMAL) {
            /*处理自由下落*/
            int i, j;
            if ( m_KeyState[3] == 0 || m_forbid_down) {
                --m_curtime, m_cursubtime = 1;
            }
            if ( m_curxtime ) {
                if (m_curxtime<0)
                    m_curxtime++;
                else
                    m_curxtime--;
            }
            /*按键处理*/
            for (i = 1, j = 1; i<=2; ++i, j-=2) {
                for ( ; m_KeyFlag[i] > 0; --m_KeyFlag[i]) {
                    m_ctl_x -= j;
                    if ( isCrash() )
                        m_ctl_x += j;
                    else
                        m_curxtime = m_movxtime * j;
                }
            }
            m_ctl_dx = float(double(m_curxtime) / m_movxtime); //处理x方向平滑
            for (i = 4, j = 1; i<=5; ++i, j-=2) {
                for (int t ; m_KeyFlag[i] > 0; --m_KeyFlag[i]) {
                    m_ctl_t=((t=m_ctl_t)+g_map_mod[m_ctl_s]+j)%g_map_mod[m_ctl_s];
                    if ( isCrash() ) m_ctl_t = t;
                }
            }
            if ( m_forbid_down == 0 && (m_KeyState[3] ) ) {
                    m_curtime -= m_cursubtime++;
            }
            if (m_curtime<0) {
                ++m_ctl_y;
                if ( isCrash() ) {
                    --m_ctl_y;
                    merge();
                    m_ctl_dy = 0; m_ctl_dx = 0; m_ctl_t = -1;
                    if ( m_KeyState[3] )
                        m_forbid_down = 1;
                    m_state = ST_NEXT;
                } else {
                    m_curtime += m_droptime;
                }
            }
            if (m_state == ST_NORMAL) {
                m_ctl_dy = float(double(m_curtime) / m_droptime);//处理y方向平滑
            }
        } else if (m_state == ST_OVER) {
            if ( m_gray_y>0 && (m_gray_y % 2) == 0)
                for (int x = 1; x <= m_gamepool_w; ++x)
                    if ( m_gamepool[m_gray_y>>1][x] )
                        m_gamepool[m_gray_y>>1][x] = 8;
            m_gray_y--;
            ++m_over_st;
            if ( m_KeyFlag[0] > 0 )
                m_state = ST_START;
        }
        memset(m_KeyFlag, 0, sizeof(m_KeyFlag));
        return nRet;
    }
    /*碰撞检测*/
    bool isCrash() {
        for (int y=0; y<4; ++y) {
            for (int x=0; x<4; ++x)
                if ( g_trs_map[m_ctl_s][m_ctl_t][y][x] ) {
                    if ( m_ctl_y + y < 0 || m_ctl_x + x < 0
                        || m_gamepool[m_ctl_y + y][m_ctl_x + x])
                        return true;
                }
        }
        return false;
    }
    void merge() {
        int y, x, cy = m_gamepool_h;
        /*合并处理*/
        for (y=0; y<4; ++y) {
            for (x=0; x<4; ++x)
                if ( g_trs_map[m_ctl_s][m_ctl_t][y][x] )
                    m_gamepool[m_ctl_y + y][m_ctl_x + x]
                        = g_trs_map[m_ctl_s][m_ctl_t][y][x];
        }
        /*消行计算*/
        for (y = m_gamepool_h; y >= 1; --y) {
            for (x = 1; x <= m_gamepool_w; ++x) {
                if ( m_gamepool[y][x] == 0 )
                    break;
            }
            if ( x <= m_gamepool_w ) {
                if ( cy != y ) {
                    for (x = 1; x <= m_gamepool_w; ++x)
                        m_gamepool[cy][x] = m_gamepool[y][x];
                }
                --cy;
            }
        }
        for (y = cy; y >= 1; --y) {
            for (x = 1; x <= m_gamepool_w; ++x)
                m_gamepool[y][x] = 0;
        }
    }
    /*逻辑更新主函数*/
    void update() {
        key_msg key;
        int k = kbmsg();
        while ( k ) {
            key = getkey();
            for (int i=0; i<8; ++i) {
                if (key.key == m_Keys[i]) {
                    if (key.msg == key_msg_down) {
                        m_KeyFlag[i]++;
                        m_KeyState[i] = 1;
                    } else if (key.msg == key_msg_up) {
                        m_KeyFlag[i] = 0;
                        m_KeyState[i] = 0;
                        if ( i == 3 )
                            m_forbid_down = 0;
                    }
                }
            }
            k = kbmsg();
        }
        while ( deal() );
    }
    void drawedge(int x, int y, int w, int h, int color, int bdark = 1) {
        setcolor(getchangcolor(color, 1.5f));
        line(x, y+h, x, y);
        lineto(x+w, y);
        if ( bdark )
            setcolor(getchangcolor(color, 0.7f));
        lineto(x+w, y+h);
        lineto(x, y+h);
    }
    void drawtile(int x, int y, int w, int h, int d, int color) {
        w--, h--;
        setfillcolor(color);
        bar(x+1, y+1, x+w, y+h);
        drawedge(x, y, w, h, color);
        drawedge(x+1, y+1, w-2, h-2, color);
    }
    void drawframe(int x, int y, int w, int h, int d = 0) {
        int coll[] = {0x400040, 0x600060, 0xA000A0, 0xFF00FF,
            0xA000A0, 0x600060, 0x400040};
        setfillcolor(0x010101);
        bar(x, y, x + w--, y + h--);
        for (int i=0; i<7; ++i) {
            --x, --y, w += 2, h += 2;
            drawedge(x, y, w, h, coll[i], 0);
        }
    }
    void draw44(int bx, int by, int mat[4][4],
        float dx=0, float dy=0, int nc=0, int deep=5) {
        for (int y = 3; y >= 0; --y) {
            for (int x = 0, c; x < 4; ++x) {
                if ( c = mat[y][x] ) {
                    if ( nc ) c = nc;
                    drawtile(int(bx + (x + dx) * m_base_w + 1000.5) - 1000,
                            int(by + (y - dy) * m_base_h + 1000.5) - 1000,
                            m_base_w, m_base_h, deep,
                            m_colormap[c]);
                }
            }
        }
    }
    /*图形更新主函数*/
    void render() {
        int x, y, c, bx, by;
        /*画背景框*/
        cleardevice();
        drawframe(  m_base_x + 5 * m_base_w,
                    m_base_y,
                    m_gamepool_w * m_base_w,
                    m_gamepool_h * m_base_h);
        drawframe(m_base_x, m_base_y,              4*m_base_w, 4*m_base_h);
        drawframe(m_base_x, m_base_y + 5*m_base_h, 4*m_base_w, 4*m_base_h);
        /*画主游戏池*/
        bx = m_base_x + 4 * m_base_w;
        by = m_base_y - 1 * m_base_h;
        for (y = m_gamepool_h; y >= 1; --y) {
            for (x = 1; x <= m_gamepool_w; ++x) {
                if ( c = m_gamepool[y][x] )
                        putimage(bx + x * m_base_w, by + y * m_base_h,
                            m_base_w, m_base_h, m_pcb,
                            c * m_base_w, 0);
            }
        }
        /*画控制块*/
        if ( m_ctl_t >=0 ) {
            bx = m_base_x + (m_ctl_x + 4) * m_base_w;
            by = m_base_y + (m_ctl_y - 1) * m_base_h;
            draw44(bx, by, g_trs_map[m_ctl_s][m_ctl_t], m_ctl_dx, m_ctl_dy);
        }
        /*画下一块和下二块*/
        bx = m_base_x;
        by = m_base_y;
        draw44(bx, by, g_trs_map[m_next1_s][0]);
        bx = m_base_x;
        by = m_base_y + 5 * m_base_h;
        draw44(bx, by, g_trs_map[m_next2_s][0], 0, 0, 8);
        setcolor(0xFFFFFF);
        if ( m_state == ST_OVER ) { // 结束提示文字显示
            outtextxy(m_base_x+5*m_base_w, m_base_y, "Press F2 to Restart game");
        }
    }
    static int dealbit(int a, float dt) {
        a = int(a * dt);
        if ( a>255 )    a = 255;
        else if ( a<0 ) a = 0;
        return a;
    }
    static int getchangcolor(int Color, float t) {
        int r = EGEGET_R(Color), g = EGEGET_G(Color), b = EGEGET_B(Color);
        r = dealbit(r, t);
        g = dealbit(g, t);
        b = dealbit(b, t);
        return EGERGB(r, g, b);
    }
public:
    int m_base_x, m_base_y, m_base_w, m_base_h;
    int m_droptime;
    int m_curtime;
    int m_cursubtime;
    int m_movxtime;
    int m_curxtime;
private:
    int m_gamepool_w, m_gamepool_h;
    int m_gamepool[30][30]; //从1为起始下标，0用于边界碰撞检测
    int m_ctl_x, m_ctl_y, m_ctl_t, m_ctl_s; //当前控制块属性
    float m_ctl_dx, m_ctl_dy;
    int m_next1_s, m_next2_s;
    int m_forbid_down;
    int m_colormap[10];
public:
    int m_pause;
    int m_state; //游戏主状态
    int m_gray_y;
    int m_over_st;
    int m_Keys[8];
    int m_KeyFlag[8];
    int m_KeyState[8];
private:
    PIMAGE m_pcb;
};

int main() {
    int nfps = 120;
    initgr();

    fps ui_fps;
    Game game(10, 20, 24, 24);
    game.m_base_x = 20;
    game.m_base_y = 20;
    game.m_droptime = nfps / 2;
    game.m_movxtime = 10;

    setrendermode(RENDER_MANUAL);
    for ( ; is_run(); delay_fps(nfps)) {
        game.update();
        game.render();
    }
    return 0;
}

