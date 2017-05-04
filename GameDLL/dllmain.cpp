// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <MyTools/Log.h>
#include <MyTools/CLNoModuleDLL.h>
#include <MyTools/Character.h>

#define _SELF L"dllmain.cpp"

HANDLE hWorkThread = NULL;
DWORD WINAPI _WorkThread(LPVOID)
{
	//HMODULE hmDLL = ::GetModuleHandleW(L"GameDLL.dll");
	WCHAR wszGamePath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszGamePath);
	lstrcatW(wszGamePath, L"\\Log\\");
	CLog::GetInstance().SetClientName(L"Game", wszGamePath, TRUE, 20 * 1024 * 1024);
	for (;;)
	{
		LOG_C_D(L"Running");
		::Sleep(5 * 1000);
	}
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	//CLNoModuleDLL NoModuleDLL;
	//NoModuleDLL.HideSelf(hModule, ul_reason_for_call, lpReserved);
	if (hWorkThread == NULL /*&& ul_reason_for_call == NoModuleDLL.GetNoModuleReasonValue()*/)
	{
		hWorkThread = cbBEGINTHREADEX(NULL, NULL, _WorkThread, NULL, NULL, NULL);
	}
	//else if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	//	return FALSE;

	return TRUE;
}

