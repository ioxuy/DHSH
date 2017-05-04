#pragma once
#include <Windows.h>


//
//NOTICE
//
//Example
//typedef int (WINAPI * __pfnMessageBoxA)
//(
//    IN HWND hWnd,
//    IN LPCSTR lpText,
//    IN LPCSTR lpCaption,
//    IN UINT uType);
//int
//WINAPI
//OnMessageBoxA(
//    IN HWND hWnd,
//    IN LPCSTR lpText,
//    IN LPCSTR lpCaption,
//    IN UINT uType);
//__pfnMessageBoxA pfnMessageBoxA = NULL;
//
//HINSTANCE hUSER32 = NULL;
//hUSER32 = LoadLibrary(L"user32.dll");
//InlineHook((__pfnMessageBoxA)GetProcAddress(hUSER32,"MessageBoxA"), OnMessageBoxA, (void **)&pfnMessageBoxA);
//UnInlineHook((__pfnMessageBoxA)GetProcAddress(hUSER32,"MessageBoxA"), pfnMessageBoxA);



//
//Define
//
BOOL WINAPI
WriteReadOnlyMemory(
LPBYTE	lpDest,
LPBYTE	lpSource,
ULONG	Length
);

BOOL WINAPI
GetPatchSize(
IN	void *Proc,
IN	DWORD dwNeedSize,
OUT LPDWORD lpPatchSize
);

BOOL WINAPI
InlineHook(
IN	void *OrgProc,		/* 需要Hook的函数地址 */
IN	void *NewProc,		/* 代替被Hook函数的地址 */
OUT	void **RealProc		/* 返回原始函数的入口地址 */
);

VOID WINAPI
UnInlineHook(
void *OrgProc,  /* 需要恢复Hook的函数地址 */
void *RealProc  /* 原始函数的入口地址 */
);