#include "ege_head.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <wincon.h>


#undef _INC_CONIO
#undef _CONIO_H_
#include <conio.h>

// macro to check whether hConsole is valid
#define CHECK_CONSOLE_HANDLE(hHandle) \
    {                                 \
        if (hHandle == NULL)          \
            return FALSE;             \
    }

namespace ege
{

// We don't support vc6
#ifndef EGE_COMPILERINFO_VC6

static HANDLE hInput = NULL;
static HANDLE hOutput = NULL;
static HWND hConsoleWnd = NULL;
static FILE fOldIn;
static FILE fOldOut;

bool init_console()
{
    HMENU hMenu;
    int hCrt;
    FILE* hf;
    if (hInput != NULL) {
        return false;
    }
    if (GetConsoleWindow() != NULL) {
        return false;
    }
    if (!AllocConsole()) {
        return false;
    }
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    if (INVALID_HANDLE_VALUE == hOutput) {
        return false;
    }
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    if (INVALID_HANDLE_VALUE == hInput) {
        return false;
    }
    SetConsoleTitle("EGE CONSOLE");
    hConsoleWnd = GetConsoleWindow();
    if (INVALID_HANDLE_VALUE == hConsoleWnd) {
        return false;
    }
    hMenu = GetSystemMenu(hConsoleWnd, 0);
    if (hMenu != NULL) {
        DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
        DrawMenuBar(hConsoleWnd);
    }

    hCrt = _open_osfhandle((intptr_t)hOutput, _O_TEXT);
    hf = _fdopen(hCrt, "w");
    fOldOut = *stdout;
    *stdout = *hf;
    setvbuf(stdout, NULL, _IONBF, 0);

    hCrt = _open_osfhandle((intptr_t)hInput, _O_TEXT);
    hf = _fdopen(hCrt, "r");
    fOldIn = *stdin;
    *stdin = *hf;
    // setvbuf( stdin, NULL, _IONBF, 0 );
    //  test code

    // SetConsoleTextAttribute(hOutput, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED ); // white text on black bg
    clear_console();
    ShowWindow(hConsoleWnd, SW_SHOW);
    return true;
}

bool clear_console()
{
    /***************************************/
    // This code is from one of Microsoft's
    // knowledge base articles, you can find it at
    // http://support.microsoft.com/default.aspx?scid=KB;EN-US;q99261&
    /***************************************/

    COORD coordScreen = {0, 0};

    bool bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
    DWORD dwConSize;

    if (hOutput == NULL) {
        return false;
    }

    /* get the number of character cells in the current buffer */

    bSuccess = GetConsoleScreenBufferInfo(hOutput, &csbi);

    if (!bSuccess) {
        return false;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    /* fill the entire screen with blanks */

    bSuccess = FillConsoleOutputCharacter(hOutput, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
    if (!bSuccess) {
        return false;
    }

    /* get the current text attribute */

    bSuccess = GetConsoleScreenBufferInfo(hOutput, &csbi);
    if (!bSuccess) {
        return false;
    }

    /* now set the buffer's attributes accordingly */

    bSuccess = FillConsoleOutputAttribute(hOutput, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    if (!bSuccess) {
        return false;
    }

    /* put the cursor at (0, 0) */

    bSuccess = SetConsoleCursorPosition(hOutput, coordScreen);
    if (!bSuccess) {
        return false;
    }

    return true;
}

bool show_console()
{
    CHECK_CONSOLE_HANDLE(hConsoleWnd);
    return ShowWindow(hConsoleWnd, SW_SHOW);
}

bool hide_console()
{
    CHECK_CONSOLE_HANDLE(hConsoleWnd);
    return ShowWindow(hConsoleWnd, SW_HIDE);
}

bool close_console()
{
    if (!FreeConsole()) {
        return false;
    }

    hOutput = NULL;
    hInput = NULL;
    hConsoleWnd = NULL;
    *stdout = fOldOut;
    *stdin = fOldIn;
    return true;
};

#endif

int getch_console()
{
#ifdef MSVC_VER
    return ::_getch();
#else
    return ::getch();
#endif
}

int kbhit_console()
{
#ifdef MSVC_VER
    return ::_kbhit();
#else
    return ::kbhit();
#endif
}

} // namespace ege
