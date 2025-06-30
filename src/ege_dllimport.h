#pragma once

#include <windows.h>
#include <windef.h>

namespace dll
{
    // 加载所有的 dll (可重复调用)
    void loadDllsIfNot();

    // 加载 winmm.dll (可重复调用)
    bool loadWinmmDll();

    // 加载 msimg32.dll (可重复调用)
    bool loadMsimg32Dll();

    // 加载 imm32.dll (可重复调用)
    bool loadImm32Dll();

    // 释放所有加载的 dll
    void freeDlls();

    // 临时从 dll 中加载 CreateStreamOnHGlobal 函数并调用
    HRESULT CreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease, LPSTREAM *ppstm);

    // --------------------- imm32.dll -----------------------
    HIMC ImmGetContext(HWND hwnd);
    BOOL ImmSetCompositionWindow(HIMC,LPCOMPOSITIONFORM);

    // --------------------- msimg32.dll ---------------------
    BOOL AlphaBlend(HDC hdcDest,int xoriginDest,int yoriginDest,int wDest,int hDest,HDC hdcSrc,int xoriginSrc,int yoriginSrc,int wSrc,int hSrc,BLENDFUNCTION ftn);
    BOOL GradientFill(HDC hdc, PTRIVERTEX pVertex, ULONG nVertex, PVOID pMesh, ULONG nMesh, ULONG ulMode);

    // --------------------- winmm.dll -----------------------
    MMRESULT timeBeginPeriod(UINT uPeriod);
    MMRESULT timeEndPeriod(UINT uPeriod);
    MMRESULT timeSetEvent(UINT uDelay,UINT uResolution,LPTIMECALLBACK fptc,DWORD_PTR dwUser,UINT fuEvent);
    MMRESULT timeKillEvent(UINT uTimerID);
    MCIERROR mciSendCommandW(MCIDEVICEID mciId,UINT uMsg,DWORD_PTR dwParam1,DWORD_PTR dwParam2);
}
