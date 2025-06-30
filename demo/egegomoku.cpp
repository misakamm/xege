/**
 * @file egegomoku.cpp
 * @author wysaid (this@xege.org)
 * @brief 传统规则下的五子棋游戏, 附带简单的AI对手
 * @version 0.1
 * @date 2025-06-18
 *
 */

#include "graphics.h"
#include <vector>

/// 是否禁用音效. 如果存在编译问题， 可以把下面这行的值改成 0
#ifdef _MSC_VER
#define ENABLE_SOUNDS 1
#else
// 如果使用非MSVC编译器, 需要手动链接 winmm.lib
// 这里默认禁用音效, 避免非MSVC编译器编译时出错
#define ENABLE_SOUNDS 0
#endif

#if ENABLE_SOUNDS
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")
#endif

// 文本本地化宏定义
#ifdef _MSC_VER
// MSVC编译器使用中文文案
#define TEXT_WINDOW_TITLE   "五子棋游戏 - EGE Demo"
#define TEXT_CURRENT_PLAYER "当前玩家: %s"
#define TEXT_GAME_MODE      "游戏模式: %s"
#define TEXT_BLACK_PIECE    "黑子"
#define TEXT_WHITE_PIECE    "白子"
#define TEXT_BLACK_WIN      "黑子获胜!"
#define TEXT_WHITE_WIN      "白子获胜!"
#define TEXT_PLAYER_WIN     "玩家获胜!"
#define TEXT_PLAYER_LOSE    "玩家失败!"
#define TEXT_DRAW           "平局!"
#define TEXT_EXIT_HINT      "按 ESC 退出游戏, 按 R 切换先后手并重新开始"
#define TEXT_MODE_AI        "人机对战 (按M切换)"
#define TEXT_MODE_HUMAN     "双人对战 (按M切换)"
#define TEXT_FIRST_PLAYER   "先手: %s"
#define TEXT_PLAYER_HUMAN   "玩家"
#define TEXT_PLAYER_AI      "AI"
#define TEXT_FONT_NAME      "宋体"
#define TEXT_RESTART_MSG    "游戏重新初始化..."
#else
// 非MSVC编译器使用英文文案
#define TEXT_WINDOW_TITLE   "Gomoku Game - EGE Demo"
#define TEXT_CURRENT_PLAYER "Current Player: %s"
#define TEXT_GAME_MODE      "Game Mode: %s"
#define TEXT_BLACK_PIECE    "Black"
#define TEXT_WHITE_PIECE    "White"
#define TEXT_BLACK_WIN      "Black Wins!"
#define TEXT_WHITE_WIN      "White Wins!"
#define TEXT_PLAYER_WIN     "Player Wins!"
#define TEXT_PLAYER_LOSE    "Player Loses!"
#define TEXT_DRAW           "Draw!"
#define TEXT_EXIT_HINT      "Press ESC to Exit, Press R to Switch First Player and Restart"
#define TEXT_MODE_AI        "VS AI (Press M to Switch)"
#define TEXT_MODE_HUMAN     "VS Human (Press M to Switch)"
#define TEXT_FIRST_PLAYER   "First: %s"
#define TEXT_PLAYER_HUMAN   "Human"
#define TEXT_PLAYER_AI      "AI"
#define TEXT_FONT_NAME      "Arial"
#define TEXT_RESTART_MSG    "Game Restarting..."
#endif

// 游戏常量
const int BOARD_SIZE     = 15; // 棋盘大小
const int CELL_SIZE      = 32; // 格子大小
const int BOARD_OFFSET_X = 50; // 棋盘X偏移
const int BOARD_OFFSET_Y = 50; // 棋盘Y偏移
const int WINDOW_WIDTH   = BOARD_OFFSET_X * 2 + BOARD_SIZE * CELL_SIZE;
const int WINDOW_HEIGHT  = BOARD_OFFSET_Y * 2 + BOARD_SIZE * CELL_SIZE + 100; // 额外空间显示信息

// 棋子类型
enum PieceType
{
    EMPTY       = 0,
    BLACK_PIECE = 1,
    WHITE_PIECE = 2
};

// 游戏状态
enum GameState
{
    PLAYING,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

// 游戏类
class Gomoku
{
public:
    Gomoku()
    {
        initGame();
#if ENABLE_SOUNDS
        // 打开音频设备
        midiOutOpen(&m_device, m_deviceID, 0, 0, CALLBACK_NULL);
#endif
    }

    // 初始化游戏
    void initGame(bool toggleFirst = false)
    {
        // 清空棋盘
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                m_board[i][j] = EMPTY;
            }
        }

        // 重置最后落子位置
        m_lastRow = -1;
        m_lastCol = -1;

        // 在AI模式下，如果需要轮换先手
        if (m_vsAI && toggleFirst) {
            m_humanFirst = !m_humanFirst;

            if (!m_humanFirst) {
                // AI先手，直接下在中心位置
                placePiece(BOARD_SIZE / 2, BOARD_SIZE / 2, BLACK_PIECE);
            }
        }

        // 根据先手设置确定当前玩家
        if (m_vsAI) {
            m_currentPlayer = m_humanFirst ? BLACK_PIECE : WHITE_PIECE;
        } else {
            m_currentPlayer = BLACK_PIECE; // 双人模式总是黑子先行
        }

        m_gameState = PLAYING;
    }

    // 绘制棋盘
    void drawBoard()
    {
        // 设置背景色为木色
        setbkcolor(EGERGB(222, 184, 135));
        cleardevice();

        // 绘制棋盘线条
        setcolor(ege::BLACK);
        setlinestyle(PS_SOLID, 2);

        // 绘制横线
        for (int i = 0; i < BOARD_SIZE; i++) {
            int y = BOARD_OFFSET_Y + i * CELL_SIZE;
            line(BOARD_OFFSET_X, y, BOARD_OFFSET_X + (BOARD_SIZE - 1) * CELL_SIZE, y);
        }

        // 绘制竖线
        for (int j = 0; j < BOARD_SIZE; j++) {
            int x = BOARD_OFFSET_X + j * CELL_SIZE;
            line(x, BOARD_OFFSET_Y, x, BOARD_OFFSET_Y + (BOARD_SIZE - 1) * CELL_SIZE);
        }

        // 绘制天元和星位
        setfillcolor(ege::BLACK);
        int center = BOARD_SIZE / 2;
        // 天元
        fillcircle(BOARD_OFFSET_X + center * CELL_SIZE, BOARD_OFFSET_Y + center * CELL_SIZE, 3); // 四个星位
        int starPos = 3;
        fillcircle(BOARD_OFFSET_X + starPos * CELL_SIZE, BOARD_OFFSET_Y + starPos * CELL_SIZE, 2);
        fillcircle(BOARD_OFFSET_X + (BOARD_SIZE - 1 - starPos) * CELL_SIZE, BOARD_OFFSET_Y + starPos * CELL_SIZE, 2);
        fillcircle(BOARD_OFFSET_X + starPos * CELL_SIZE, BOARD_OFFSET_Y + (BOARD_SIZE - 1 - starPos) * CELL_SIZE, 2);
        fillcircle(BOARD_OFFSET_X + (BOARD_SIZE - 1 - starPos) * CELL_SIZE,
            BOARD_OFFSET_Y + (BOARD_SIZE - 1 - starPos) * CELL_SIZE, 2);
    }

    // 绘制棋子
    void drawPieces()
    {
        ege_enable_aa(true);
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (m_board[i][j] != EMPTY) {
                    int x = BOARD_OFFSET_X + j * CELL_SIZE;
                    int y = BOARD_OFFSET_Y + i * CELL_SIZE;

                    // 判断是否为最后一颗落子
                    bool isLastPiece = (i == m_lastRow && j == m_lastCol);

                    if (m_board[i][j] == BLACK_PIECE) {
                        setfillcolor(ege::BLACK);
                        setcolor(EGERGB(64, 64, 64));
                    } else {
                        setfillcolor(ege::WHITE);
                        setcolor(EGERGB(192, 192, 192));
                    }

                    // 使用抗锯齿的EGE绘制函数，提供更平滑的圆形效果
                    ege_fillcircle(x, y, CELL_SIZE / 2 - 2);
                    ege_circle(x, y, CELL_SIZE / 2 - 2);

                    // 如果是最后一颗落子，绘制高亮标记
                    if (isLastPiece) {
                        // 绘制红色小十字
                        setcolor(EGERGB(255, 0, 0));
                        setlinewidth(2);
                        line(x - CELL_SIZE / 6, y, x + CELL_SIZE / 6, y);
                        line(x, y - CELL_SIZE / 6, x, y + CELL_SIZE / 6);
                    }
                }
            }
        }
        ege_enable_aa(false);
    }

    // 绘制游戏信息
    void drawInfo()
    {
        setcolor(ege::BLACK);
        setfont(20, 0, TEXT_FONT_NAME);

        int infoY = BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE + 20;

        if (m_gameState == PLAYING) {
            xyprintf(BOARD_OFFSET_X, infoY, TEXT_CURRENT_PLAYER,
                m_currentPlayer == BLACK_PIECE ? TEXT_BLACK_PIECE : TEXT_WHITE_PIECE);
            xyprintf(BOARD_OFFSET_X, infoY + 25, TEXT_GAME_MODE, m_vsAI ? TEXT_MODE_AI : TEXT_MODE_HUMAN);

            // 在AI模式下显示先手信息
            if (m_vsAI) {
                xyprintf(
                    BOARD_OFFSET_X, infoY + 50, TEXT_FIRST_PLAYER, m_humanFirst ? TEXT_PLAYER_HUMAN : TEXT_PLAYER_AI);
            }
        } else {
            const char* winner = getWinnerText();
            xyprintf(BOARD_OFFSET_X, infoY, winner);
        }

        xyprintf(BOARD_OFFSET_X, infoY + (m_vsAI && m_gameState == PLAYING ? 75 : 50), TEXT_EXIT_HINT);
    }

    void drawGameOver()
    {
        if (m_gameState == PLAYING) {
            if (m_gameEndImage != nullptr) {
                // 清理缓存， 降低内存占用.
                delimage(m_gameEndImage);
                m_gameEndImage = nullptr;
            }
            return; // 游戏未结束，不绘制
        }

        if (m_gameEndImage == nullptr) {
            int bgWidth  = 300;
            int bgHeight = 100;

            m_gameEndImage = newimage(bgWidth, bgHeight);

            settarget(m_gameEndImage);

            // 绘制半透明背景
            setfillcolor(0xffffffff);
            setfillstyle(SOLID_FILL, 0xffffffff);
            // 绘制背景矩形
            setlinewidth(3);
            setbkmode(TRANSPARENT);

            // 设置大号字体
            setcolor(EGERGBA(255, 0, 0, 255)); // 红色文字
            setfont(48, 0, TEXT_FONT_NAME);

            // 根据游戏状态确定显示文本
            const char* gameOverText = getWinnerText();

            fillrect(0, 0, bgWidth, bgHeight);
            rectangle(0, 0, bgWidth, bgHeight); // 计算文本居中位置
            int textWidth  = textwidth(gameOverText);
            int textHeight = textheight(gameOverText);
            int textX      = (bgWidth - textWidth) / 2;
            int textY      = (bgHeight - textHeight) / 2;

            // 绘制游戏结束文本
            outtextxy(textX, textY, gameOverText);
            m_imgX = (WINDOW_WIDTH - bgWidth) / 2;
            m_imgY = (WINDOW_HEIGHT - bgHeight) / 2;

            ege_setalpha(0xa0, m_gameEndImage); // 设置半透明效果
            settarget(nullptr);                 // 结束绘制到临时图像
        }

        // putimage(m_imgX, m_imgY, m_gameEndImage);
        putimage_withalpha(nullptr, m_gameEndImage, m_imgX, m_imgY);
    }

    // 获取获胜者文案
    const char* getWinnerText()
    {
        if (m_gameState == DRAW) {
            return TEXT_DRAW;
        }

        if (m_vsAI) {
            // AI模式下判断玩家是否获胜
            bool playerWins = false;
            if (m_humanFirst && m_gameState == BLACK_WIN) {
                playerWins = true; // 玩家是黑子且黑子获胜
            } else if (!m_humanFirst && m_gameState == WHITE_WIN) {
                playerWins = true; // 玩家是白子且白子获胜
            }
            return playerWins ? TEXT_PLAYER_WIN : TEXT_PLAYER_LOSE;
        } else {
            // 双人模式下使用传统文案
            if (m_gameState == BLACK_WIN) {
                return TEXT_BLACK_WIN;
            } else if (m_gameState == WHITE_WIN) {
                return TEXT_WHITE_WIN;
            }
        }

        return TEXT_DRAW;
    }

    // 鼠标点击转换为棋盘坐标
    bool mouseToBoard(int mouseX, int mouseY, int& row, int& col)
    {
        int x = mouseX - BOARD_OFFSET_X;
        int y = mouseY - BOARD_OFFSET_Y;

        // 计算最近的交叉点
        col = (x + CELL_SIZE / 2) / CELL_SIZE;
        row = (y + CELL_SIZE / 2) / CELL_SIZE;

        // 检查是否在棋盘范围内
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) {
            // 检查点击是否在交叉点附近
            int actualX = col * CELL_SIZE;
            int actualY = row * CELL_SIZE;
            int dx      = x - actualX;
            int dy      = y - actualY;

            return (dx * dx + dy * dy <= (CELL_SIZE / 2) * (CELL_SIZE / 2));
        }

        return false;
    }

    // 下棋
    bool placePiece(int row, int col, PieceType piece)
    {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            return false;
        }

        if (m_board[row][col] != EMPTY) {
            return false;
        }

        m_board[row][col] = piece;

        // 更新最后落子位置
        m_lastRow = row;
        m_lastCol = col;

        return true;
    }

    // 检查是否五子连珠
    bool checkWin(int row, int col, PieceType piece)
    {
        // 四个方向：水平、垂直、主对角线、副对角线
        int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

        for (int dir = 0; dir < 4; dir++) {
            int count = 1; // 包含当前棋子
            int dr    = directions[dir][0];
            int dc    = directions[dir][1];

            // 正方向统计
            int r = row + dr, c = col + dc;
            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && m_board[r][c] == piece) {
                count++;
                r += dr;
                c += dc;
            }

            // 反方向统计
            r = row - dr, c = col - dc;
            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && m_board[r][c] == piece) {
                count++;
                r -= dr;
                c -= dc;
            }

            if (count >= 5) {
                return true;
            }
        }

        return false;
    }

    // 检查棋盘是否已满
    bool isBoardFull()
    {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (m_board[i][j] == EMPTY) {
                    return false;
                }
            }
        }
        return true;
    }

    // 简单AI：寻找最佳落子位置
    void aiMove()
    {
        if (m_gameState != PLAYING) {
            return;
        }

        // 检查是否轮到AI
        bool isAITurn = (m_humanFirst && m_currentPlayer == WHITE_PIECE) ||
            (!m_humanFirst && m_currentPlayer == BLACK_PIECE);
        if (!isAITurn) {
            return;
        }

        int bestRow = -1, bestCol = -1;
        int bestScore = -1000;

        // 遍历所有空位，评估每个位置的得分
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (m_board[i][j] == EMPTY) {
                    int score = evaluatePosition(i, j);
                    if (score > bestScore) {
                        bestScore = score;
                        bestRow   = i;
                        bestCol   = j;
                    }
                }
            }
        }

        // 如果找到了合适的位置，就下棋
        if (bestRow != -1 && bestCol != -1) {
            makeMove(bestRow, bestCol);
        }
    }

    // 评估位置得分
    int evaluatePosition(int row, int col)
    {
        int score = 0;

        // 基础得分：靠近中心的位置更好
        int centerRow           = BOARD_SIZE / 2;
        int centerCol           = BOARD_SIZE / 2;
        int distanceFromCenter  = abs(row - centerRow) + abs(col - centerCol);
        score                  += (BOARD_SIZE - distanceFromCenter);

        // 获取AI的棋子类型
        PieceType aiPiece    = m_humanFirst ? WHITE_PIECE : BLACK_PIECE;
        PieceType humanPiece = m_humanFirst ? BLACK_PIECE : WHITE_PIECE;

        // 攻击得分：评估AI能形成的连珠
        score += evaluateDirection(row, col, aiPiece) * 10;

        // 防守得分：阻止玩家形成连珠
        score += evaluateDirection(row, col, humanPiece) * 8;

        return score;
    }

    // 评估方向得分
    int evaluateDirection(int row, int col, PieceType piece)
    {
        int score            = 0;
        int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

        for (int dir = 0; dir < 4; dir++) {
            int dr = directions[dir][0];
            int dc = directions[dir][1];

            // 统计连续的同色棋子
            int  count   = 0;
            bool blocked = false;

            // 正方向
            int r = row + dr, c = col + dc;
            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
                if (m_board[r][c] == piece) {
                    count++;
                } else if (m_board[r][c] != EMPTY) {
                    blocked = true;
                    break;
                } else {
                    break;
                }
                r += dr;
                c += dc;
            }

            // 反方向
            r = row - dr, c = col - dc;
            while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
                if (m_board[r][c] == piece) {
                    count++;
                } else if (m_board[r][c] != EMPTY) {
                    if (!blocked) {
                        blocked = true;
                    }
                    break;
                } else {
                    break;
                }
                r -= dr;
                c -= dc;
            }

            // 根据连子数量和是否被阻挡给分
            if (count >= 4) {
                score += 1000; // 四连，非常重要
            } else if (count >= 3) {
                score += blocked ? 5 : 50;
            } else if (count >= 2) {
                score += blocked ? 2 : 10;
            } else if (count >= 1) {
                score += blocked ? 1 : 3;
            }
        }

        return score;
    }

    // 执行移动
    void makeMove(int row, int col)
    {
        if (placePiece(row, col, m_currentPlayer)) {
            if (checkWin(row, col, m_currentPlayer)) {
                m_gameState = (m_currentPlayer == BLACK_PIECE) ? BLACK_WIN : WHITE_WIN;
            } else if (isBoardFull()) {
                m_gameState = DRAW;
            } else {
                // 切换玩家
                m_currentPlayer = (m_currentPlayer == BLACK_PIECE) ? WHITE_PIECE : BLACK_PIECE;
            }
        }

        playPieceSound(m_currentPlayer); // 播放下棋音效
    }

    // 处理鼠标点击
    void handleMouseClick(int mouseX, int mouseY)
    {
        if (m_gameState != PLAYING) {
            return;
        }

        // 如果是AI回合，忽略鼠标点击
        if (m_vsAI &&
            ((m_humanFirst && m_currentPlayer == WHITE_PIECE) || (!m_humanFirst && m_currentPlayer == BLACK_PIECE)))
        {
            return;
        }

        int row, col;
        if (mouseToBoard(mouseX, mouseY, row, col)) {
            makeMove(row, col);
        }
    }

    // 获取当前游戏状态
    GameState getGameState() const { return m_gameState; }

    // 获取当前玩家
    PieceType getCurrentPlayer() const { return m_currentPlayer; }

    // 是否为AI模式
    bool isVsAI() const { return m_vsAI; }

    // 切换游戏模式
    void toggleMode()
    {
        m_vsAI = !m_vsAI;
        if (m_vsAI) {
            m_humanFirst = true;
        }
        initGame();
    }

    // 重新开始游戏（在AI模式下轮换先手）
    void restartGame()
    {
        if (m_vsAI) {
            initGame(true); // 轮换先手
        } else {
            initGame(); // 不轮换先手
        }
    }

    enum // MIDI音符编号定义
    {
        MIDI_BLACK = 45,
        MIDI_WHITE = 57,
    };

    // 播放声音
    void playPieceSound(PieceType piece)
    {
#if ENABLE_SOUNDS
        if (m_device == nullptr) {
            return;
        }
        // 设置音色为木琴(Xylophone)，音色编号13，更符合棋子落盘的感觉
        DWORD msg = 0xC000 | 13; // 0xC0 是更改乐器的控制命令，13 是木琴的乐器号
        midiOutShortMsg(m_device, msg);

        if (m_lastSound != 0) {
            // 如果上一个音符还在播放，先停止它
            midiOutShortMsg(m_device, 0x80 | (m_lastSound << 8)); // 0x80是音符关闭命令
        }

        // 播放黑子下棋音效 - 使用较低沉的G4音符
        if (piece == BLACK_PIECE) {
            // 0x90是音符开启命令，80是适中的音量(比127更柔和)
            midiOutShortMsg(m_device, 0x90 | (MIDI_BLACK << 8) | (80 << 16));
            m_lastSound = MIDI_BLACK; // 记录上一个音符
        } else if (piece == WHITE_PIECE) {
            // 播放白子下棋音效 - 使用较清脆的C5音符，与G4形成完美四度音程
            midiOutShortMsg(m_device, 0x90 | (MIDI_WHITE << 8) | (80 << 16));
            m_lastSound = MIDI_WHITE; // 记录上一个音符
        }
        m_soundTimer = 20; // 音效持续20帧
#endif
    }

    void updatePieceSound()
    {
#if ENABLE_SOUNDS
        if (m_soundTimer <= 0 || m_device == nullptr) {
            return;
        }
        --m_soundTimer;
        if (m_soundTimer == 0 && m_lastSound != 0) {
            // 停止上一个音符
            midiOutShortMsg(m_device, 0x80 | (m_lastSound << 8)); // 0x80是音符关闭命令
            m_lastSound = 0;                                      // 重置上一个音符
        }
#endif
    }

private:
    int       m_board[BOARD_SIZE][BOARD_SIZE]; // 棋盘数组
    PIMAGE    m_gameEndImage{};                // 用于临时缓存绘制
    int       m_imgX{}, m_imgY{};              // 用于结束动画
    PieceType m_currentPlayer{};               // 当前玩家
    GameState m_gameState{};                   // 游戏状态
    bool      m_vsAI       = true;             // 是否对战AI
    bool      m_humanFirst = true;             // AI模式下是否玩家先手

    // 最后落子位置记录
    int m_lastRow = -1; // 最后落子的行位置
    int m_lastCol = -1; // 最后落子的列位置

#if ENABLE_SOUNDS
    /// 音频相关
    HMIDIOUT m_device{};       // MIDI输出设备句柄
    UINT     m_deviceID   = 0; // 使用默认设备
    int      m_soundTimer = 0; // 音效倒计时, 到0时停止
    DWORD    m_lastSound{};    // 上一个音符
#endif
};

int main()
{
    // 初始化图形窗口
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, INIT_RENDERMANUAL);
    setrendermode(RENDER_MANUAL);
    setcaption(TEXT_WINDOW_TITLE);

    Gomoku game;

    // 游戏主循环
    while (is_run()) {
        // 绘制游戏画面
        game.drawBoard();
        game.drawPieces();
        game.drawInfo();
        game.updatePieceSound(); // 更新音效状态

        GameState state = game.getGameState();
        if (state == PLAYING) {
            // AI移动（如果是AI回合）
            if (game.isVsAI()) {
                game.aiMove();
            }
        } else {
            game.drawGameOver();
        }

        // 处理鼠标消息
        while (mousemsg()) {
            mouse_msg msg = getmouse();
            if (msg.is_down() && msg.is_left()) {
                game.handleMouseClick(msg.x, msg.y);
            }
        }

        // 处理键盘消息
        if (kbhit()) {
            int key = getch();
            if (key == 27) { // ESC键退出
                break;
            } else if (key == 'r' || key == 'R') { // R键重新开始
                cleardevice(0);
                /// 打印重新初始化消息，提醒玩家游戏重开了
                setcolor(EGERGB(255, 0, 0));
                setfont(50, 0, TEXT_FONT_NAME);
                xyprintf(50, WINDOW_HEIGHT / 2, TEXT_RESTART_MSG);
                Sleep(500); // 稍等一会, 让玩家感知到游戏重开了.
                game.restartGame();
            } else if (key == 'm' || key == 'M') { // M键切换模式
                game.toggleMode();
            }
        }

        delay_fps(60);
    }

    closegraph();
    return 0;
}