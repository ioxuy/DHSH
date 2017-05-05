// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>

#define _SELF L"dllmain.cpp"

HANDLE hWorkThread = NULL;
DWORD WINAPI _WorkThread(LPVOID)
{
	//HMODULE hmDLL = ::GetModuleHandleW(L"GameDLL.dll");
	WCHAR wszGamePath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszGamePath);
	lstrcatW(wszGamePath, L"\\Log\\");
	MyTools::CLog::GetInstance().SetClientName(L"Game", wszGamePath, TRUE, 20 * 1024 * 1024);
	for (;;)
	{
		LOG_C_D(L"Running");
		::Sleep(5 * 1000);
	}

}

BOOL APIENTRY DllMain(HMODULE ,DWORD  ,LPVOID )
{
	if (hWorkThread == NULL)
	{
		hWorkThread = cbBEGINTHREADEX(NULL, NULL, _WorkThread, NULL, NULL, NULL);
	}
	return TRUE;
}

