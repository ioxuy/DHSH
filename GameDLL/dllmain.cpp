// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CmdLog.h>
#include <MyTools/CLThread.h>
#include "Expr.h"
#include "ResText.h"
#include "ExcuteAction.h"
#include "GameVariable.h"
#define _SELF L"dllmain.cpp"

BOOL CALLBACK EnumSetWinName(HWND hLauncher, LPARAM l)
{
	if (IsWindow(hLauncher) && IsWindowVisible(hLauncher) && (GetWindowLong(hLauncher, GWL_EXSTYLE)&WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW && (GetWindowLong(hLauncher, GWL_HWNDPARENT) == 0))
	{
		WCHAR wszWinText[64] = { 0 };
		WCHAR wszWinClass[64] = { 0 };

		if (GetClassNameW(hLauncher, wszWinClass, _countof(wszWinClass) / sizeof(WCHAR)) > 0 && GetWindowTextW(hLauncher, wszWinText, _countof(wszWinText)) > 0)
		{
			if (MyTools::CCharacter::wstrcmp_my(wszWinClass, L"fsgamehero0150"))//如果能遍历到这个,并且是
			{
				DWORD PID;
				::GetWindowThreadProcessId(hLauncher, &PID);
				if (PID == ::GetCurrentProcessId())
				{
					MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountStatus.hGameWnd = hLauncher;
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

HANDLE hWorkThread = NULL;
DWORD WINAPI _WorkThread(LPVOID)
{
	WCHAR wszGamePath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszGamePath);
	lstrcatW(wszGamePath, L"\\Log\\");

	MyTools::CLog::GetInstance().SetClientName(L"Game", wszGamePath, TRUE, 20 * 1024 * 1024);
	MyTools::CCmdLog::GetInstance().Run(L"Game", CExpr::GetInstance().GetVec());

	MyTools::InvokeClassPtr<CResText>()->Initialize();
	MyTools::InvokeClassPtr<CGameVariable>()->InitVariable();

	auto pGameVariable = MyTools::InvokeClassPtr<CGameVariable>();
	pGameVariable->GetGameShareContent() = new GameShareContent;
	pGameVariable->GetAccountShareContent() = &pGameVariable->GetGameShareContent()->arrGameArrount[0];

	while (EnumWindows((WNDENUMPROC)EnumSetWinName, NULL))
		::Sleep(1000);

	MyTools::InvokeClassPtr<CExcuteAction>()->SetRun(TRUE);
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
		StopGame;
		MyTools::InvokeClassPtr<CExcuteAction>()->SetRun(FALSE);
		MyTools::CCmdLog::GetInstance().Stop();
		delete MyTools::InvokeClassPtr<CGameVariable>()->GetGameShareContent();
	}
	return TRUE;
}

extern "C" __declspec(dllexport)  VOID WINAPIV ExitDLL()
{
	//
}