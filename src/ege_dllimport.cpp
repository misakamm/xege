#include "ege_def.h"
#include "ege_dllimport.h"

#include <windef.h>
#include <windows.h>
#include <stdio.h>


#define LOG(logtext)                                        \
do                                                          \
{                                                           \
    fprintf(stderr, "%s\n", (logtext));                     \
} while (0)

#define LOG_IF(express, logtext)                            \
do                                                          \
{                                                           \
    if (!!(express)) {                                      \
        LOG(logtext);                                       \
    }                                                       \
} while (0)

namespace dll
{
    // ----------------------------- imm32.dll ------------------------------------
    static HMODULE imm32Dll;
    static HIMC (WINAPI *func_ImmGetContext)(HWND);
    static BOOL (WINAPI *func_ImmSetCompositionWindow)(HIMC , LPCOMPOSITIONFORM);

    bool loadImm32Dll()
    {
        if(imm32Dll == NULL) {
            imm32Dll = LoadLibraryA("imm32.dll");
            if (imm32Dll == NULL) {
                LOG("ege error: Failed to load imm32.dll.");
                return false;
            }
        }

        // ImmGetContext
        if (func_ImmGetContext == NULL) {
            typedef HIMC (WINAPI *ImmGetContext_FuncType)(HWND);
            func_ImmGetContext = (ImmGetContext_FuncType)GetProcAddress(imm32Dll, "ImmGetContext");
            LOG_IF(func_ImmGetContext == NULL,"ege error: The 'ImmGetContext' function cannot be found from the imm32.dll.");
        }

        // ImmSetCompositionWindow
        if (func_ImmSetCompositionWindow == NULL) {
            typedef BOOL (WINAPI *ImmSetCompositionWindow_FuncType)(HIMC, LPCOMPOSITIONFORM);
            func_ImmSetCompositionWindow = (ImmSetCompositionWindow_FuncType)GetProcAddress(imm32Dll, "ImmSetCompositionWindow");
            LOG_IF(func_ImmSetCompositionWindow == NULL, "ege error: The 'ImmSetCompositionWindow' function cannot be found from the imm32.dll.");
        }

        return  func_ImmGetContext && func_ImmSetCompositionWindow;
    }

    HIMC ImmGetContext(HWND hwnd)
    {
        if (func_ImmGetContext) {
            return func_ImmGetContext(hwnd);
        }
        return NULL;
    }

    BOOL ImmSetCompositionWindow(HIMC imc, LPCOMPOSITIONFORM compositionForm)
    {
        if (func_ImmSetCompositionWindow) {
            return func_ImmSetCompositionWindow(imc, compositionForm);
        }

        return FALSE;
    }

    // ----------------------------- msimg32.dll ----------------------------------
    static HMODULE msimg32Dll;
    static BOOL (WINAPI *func_AlphaBlend)(HDC hdcDest,int xoriginDest,int yoriginDest,int wDest,int hDest,HDC hdcSrc,int xoriginSrc,int yoriginSrc,int wSrc,int hSrc,BLENDFUNCTION ftn);
    static BOOL (WINAPI *func_GradientFill)(HDC hdc, PTRIVERTEX pVertex, ULONG nVertex, PVOID pMesh, ULONG nMesh, ULONG ulMode);

    bool loadMsimg32Dll()
    {
        // -- msimg32.dll --
        if (msimg32Dll == NULL) {
            msimg32Dll = LoadLibraryA("msimg32.dll");
            if (msimg32Dll == NULL) {
                LOG("ege error: Failed to load msimg32.dll.");
                return false;
            }
        }

        // AlphaBlend
        if (func_AlphaBlend == NULL) {
            typedef BOOL (WINAPI *AlphaBlend_FuncType)(HDC hdcDest,int xoriginDest,int yoriginDest,int wDest,int hDest,HDC hdcSrc,int xoriginSrc,int yoriginSrc,int wSrc,int hSrc,BLENDFUNCTION ftn);
            func_AlphaBlend = (AlphaBlend_FuncType)GetProcAddress(msimg32Dll, "AlphaBlend");
            LOG_IF(func_AlphaBlend == NULL,"ege error: The 'AlphaBlend' function cannot be found from the msimg32.dll.");
        }

        // GradientFill
        if (func_GradientFill == NULL) {
            typedef BOOL (WINAPI *GradientFill_FuncType)(HDC hdc, PTRIVERTEX pVertex, ULONG nVertex, PVOID pMesh, ULONG nMesh, ULONG ulMode);
            func_GradientFill = (GradientFill_FuncType)GetProcAddress(msimg32Dll, "GradientFill");
            LOG_IF(func_GradientFill == NULL, "ege error: The 'GradientFill' function cannot be found from the msimg32.dll.");
        }

        return  func_AlphaBlend && func_GradientFill;
    }

    BOOL AlphaBlend(HDC hdcDest,int xoriginDest,int yoriginDest,int wDest,int hDest,HDC hdcSrc,int xoriginSrc,int yoriginSrc,int wSrc,int hSrc,BLENDFUNCTION ftn)
    {
        if (func_AlphaBlend) {
            return func_AlphaBlend(hdcDest, xoriginDest, yoriginDest, wDest, hDest, hdcSrc, xoriginSrc, yoriginSrc, wSrc, hSrc, ftn);
        }
        return FALSE;
    }

    BOOL GradientFill(HDC hdc, PTRIVERTEX pVertex, ULONG nVertex, PVOID pMesh, ULONG nMesh, ULONG ulMode)
    {
        if(func_GradientFill) {
            return func_GradientFill(hdc, pVertex, nVertex, pMesh, nMesh, ulMode);
        }
        return FALSE;
    }

    // --------------------------------- winmm.dll -------------------------------------------
    static HMODULE winmmDll;
    static MMRESULT (WINAPI *func_timeBeginPeriod)(UINT uPeriod);
    static MMRESULT (WINAPI *func_timeEndPeriod)(UINT uPeriod);
    static MMRESULT (WINAPI *func_timeSetEvent)(UINT uDelay,UINT uResolution,LPTIMECALLBACK fptc,DWORD_PTR dwUser,UINT fuEvent);
    static MMRESULT (WINAPI *func_timeKillEvent)(UINT uTimerID);
    static MCIERROR (WINAPI *func_mciSendCommandW)(MCIDEVICEID mciId,UINT uMsg,DWORD_PTR dwParam1,DWORD_PTR dwParam2);

    bool loadWinmmDll()
    {
        // winmm.dll
        if (winmmDll == NULL) {
            winmmDll = LoadLibraryA("winmm.dll");
            if (winmmDll == NULL) {
                LOG("ege error: Failed to load winmm.dll.");
                return false;
            }
        }

        // timeBeginPeriod
        if (func_timeBeginPeriod == NULL) {
            typedef MMRESULT (WINAPI *timeBeginPeriod_FuncType)(UINT uPeriod);
            func_timeBeginPeriod = (timeBeginPeriod_FuncType)GetProcAddress(winmmDll, "timeBeginPeriod");
            LOG_IF(func_timeBeginPeriod == NULL, "ege error: The 'timeBeginPeriod' function cannot be found from the winmm.dll.");
        }

        // timeEndPeriod
        if (func_timeEndPeriod == NULL) {
            typedef MMRESULT (WINAPI *timeEndPeriod_FuncType)(UINT uPeriod);
            func_timeEndPeriod = (timeEndPeriod_FuncType)GetProcAddress(winmmDll, "timeEndPeriod");
            LOG_IF(func_timeEndPeriod == NULL, "ege error: The 'timeEndPeriod' function cannot be found from the winmm.dll.");
        }


        // timeSetEvent
        if (func_timeSetEvent == NULL) {
            typedef MMRESULT (WINAPI *timeSetEvent_FuncType)(UINT uDelay,UINT uResolution,LPTIMECALLBACK fptc,DWORD_PTR dwUser,UINT fuEvent);
            func_timeSetEvent = (timeSetEvent_FuncType)GetProcAddress(winmmDll, "timeSetEvent");
            LOG_IF(func_timeSetEvent == NULL, "ege error: The 'timeSetEvent' function cannot be found from the winmm.dll.");
        }

        // timeKillEvent
        if (func_timeKillEvent == NULL) {
            typedef MMRESULT (WINAPI *timeKillEvent_FuncType)(UINT uTimerID);
            func_timeKillEvent = (timeKillEvent_FuncType)GetProcAddress(winmmDll, "timeKillEvent");
            LOG_IF(func_timeKillEvent == NULL, "ege error: The 'timeKillEvent' function cannot be found from the winmm.dll.");
        }


        // mciSendCommandW
        if (func_mciSendCommandW == NULL) {
            typedef MCIERROR (WINAPI *mciSendCommandW_FuncType)(MCIDEVICEID mciId,UINT uMsg,DWORD_PTR dwParam1,DWORD_PTR dwParam2);
            func_mciSendCommandW = (mciSendCommandW_FuncType)GetProcAddress(winmmDll, "mciSendCommandW");
            LOG_IF(func_mciSendCommandW == NULL, "ege error: The 'mciSendCommandW' function cannot be found from the winmm.dll.");
        }

        return  func_timeBeginPeriod && func_timeEndPeriod && func_timeSetEvent && func_timeKillEvent && func_mciSendCommandW;
    }

    MMRESULT timeBeginPeriod(UINT uPeriod)
    {
        if (func_timeBeginPeriod) {
            return func_timeBeginPeriod(uPeriod);
        }
        return MMSYSERR_NOTSUPPORTED;
    }

    MMRESULT timeEndPeriod(UINT uPeriod)
    {
        if (func_timeEndPeriod) {
            func_timeEndPeriod(uPeriod);
        }
        return MMSYSERR_NOTSUPPORTED;
    }

    MMRESULT timeSetEvent(UINT uDelay,UINT uResolution,LPTIMECALLBACK fptc,DWORD_PTR dwUser,UINT fuEvent)
    {
        if (func_timeSetEvent) {
            return func_timeSetEvent(uDelay, uResolution, fptc, dwUser, fuEvent);
        }

        return MMSYSERR_NOTSUPPORTED;
    }

    MMRESULT timeKillEvent(UINT uTimerID)
    {
        if (func_timeKillEvent) {
            return func_timeKillEvent(uTimerID);
        }

        return MMSYSERR_NOTSUPPORTED;
    }

    MCIERROR mciSendCommandW(MCIDEVICEID mciId,UINT uMsg,DWORD_PTR dwParam1,DWORD_PTR dwParam2)
    {
        if (func_mciSendCommandW) {
            return func_mciSendCommandW(mciId, uMsg, dwParam1, dwParam2);
        }

        return MMSYSERR_ERROR;
    }

    // ----------------------------------------------------------------------------
    // 加载 dll 以及需要的符号
    void loadDllsIfNot()
    {
        static bool loadingIsFinished = false;

        if (!loadingIsFinished) {
            bool isSuccessful = true;

            isSuccessful = loadImm32Dll()   && isSuccessful;
            isSuccessful = loadMsimg32Dll() && isSuccessful;
            isSuccessful = loadWinmmDll()   && isSuccessful;

            loadingIsFinished = isSuccessful;
        }
    }

    // 释放所有加载的 dll (imm32.dll, msimg32.dll, winmm.dll)
    void freeDlls()
    {
        if (imm32Dll != NULL) {
            FreeLibrary(imm32Dll);
        }

        if (msimg32Dll != NULL) {
            FreeLibrary(msimg32Dll);
        }

        if (winmmDll != NULL) {
            FreeLibrary(winmmDll);
        }
    }

    // 临时从 dll 中加载 CreateStreamOnHGlobal 函数并调用
    HRESULT CreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease, LPSTREAM *ppstm)
    {
        HMODULE Ole32Dll = LoadLibraryA("Ole32.dll");
        if (Ole32Dll == NULL) {
            LOG("ege error: Failed to load Ole32.dll.");
            return S_FALSE;
        }

        typedef HRESULT (WINAPI *CreateStreamOnHGlobalFuncType) (HGLOBAL hGlobal, BOOL fDeleteOnRelease, LPSTREAM *ppstm);
        CreateStreamOnHGlobalFuncType CreateStreamOnHGlobalFunc = (CreateStreamOnHGlobalFuncType)GetProcAddress(Ole32Dll, "CreateStreamOnHGlobal");

        HRESULT result;
        if (CreateStreamOnHGlobalFunc == NULL) {
            LOG("ege error: The 'CreateStreamOnHGlobal' function cannot be found from the Ole32.dll.");
            result = S_FALSE;
        } else {
            result = CreateStreamOnHGlobalFunc(hGlobal, fDeleteOnRelease, ppstm);
        }

        FreeLibrary(Ole32Dll);
        return result;
    }

}

