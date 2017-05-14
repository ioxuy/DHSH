// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CmdLog.h>
#include "Expr.h"
#include "ResText.h"

#define _SELF L"dllmain.cpp"

HANDLE hWorkThread = NULL;
DWORD WINAPI _WorkThread(LPVOID)
{
	//HMODULE hmDLL = ::GetModuleHandleW(L"GameDLL.dll");
	WCHAR wszGamePath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszGamePath);
	lstrcatW(wszGamePath, L"\\Log\\");
	MyTools::CLog::GetInstance().SetClientName(L"Game", wszGamePath, TRUE, 20 * 1024 * 1024);
	MyTools::CCmdLog::GetInstance().Run(L"Game", CExpr::GetInstance().GetVec());
	MyTools::InvokeClassPtr<CResText>()->Initialize();



	return 0;
}

BOOL APIENTRY DllMain(HMODULE ,DWORD dwReason ,LPVOID )
{
	if (dwReason == DLL_PROCESS_ATTACH && hWorkThread == NULL)
	{
		hWorkThread = cbBEGINTHREADEX(NULL, NULL, _WorkThread, NULL, NULL, NULL);
		::CloseHandle(hWorkThread);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		MyTools::CCmdLog::GetInstance().Stop();
	}
	return TRUE;
}

extern "C" __declspec(dllexport)  VOID WINAPIV ExitDLL()
{
	//
}