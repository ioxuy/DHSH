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
#include "PersonAttribute.h"
#include "Player.h"
#include "PlayerExtend.h"
#include "GameClient.h"
#include "GameUi.h"
#include "GameUiExtend.h"
#define _SELF L"dllmain.cpp"

HWND hGameWnd = NULL;
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
					hGameWnd = hLauncher;
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

HANDLE hKeepALiveThread = NULL;
BOOL bRunGame = TRUE;
DWORD WINAPI _KeepALiveThread(LPVOID lpParam)
{
	LOG_CF_D(L"Runing KeepALive Thread");
	while (bRunGame)
	{
		MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountStatus.ulOnlineTick = ::GetTickCount64();
		// Socket~ Keep ALive

		for (int i = 0; i < 5 && bRunGame; ++i)
			::Sleep(1000);
		
		MyTools::InvokeClassPtr<CGameUiExtend>()->Action_By_DlgName_When_ShowDlg(L"sys_msgdlg0", [](CONST CGameUi&) 
		{
			StopGame;
			LOG_MSG_CF(L"游戏断开连接了!");
		});
	}
	return 0;
}

HANDLE hWorkThread = NULL;
DWORD WINAPI _WorkThread(LPVOID)
{
	MyTools::InvokeClassPtr<CResText>()->Initialize();
	MyTools::InvokeClassPtr<CGameVariable>()->InitVariable();

	while (bRunGame && EnumWindows((WNDENUMPROC)EnumSetWinName, NULL))
		::Sleep(1000);

	auto pGameVariable = MyTools::InvokeClassPtr<CGameVariable>();
	if (!pGameVariable->SetGameSharePtr())
	{
		WCHAR wszGamePath[MAX_PATH] = { 0 };
		::GetCurrentDirectoryW(MAX_PATH, wszGamePath);
		lstrcatW(wszGamePath, L"\\Log\\");
		MyTools::CLog::GetInstance().SetClientName(L"Game", wszGamePath, TRUE, 20 * 1024 * 1024);
		MyTools::CCmdLog::GetInstance().Run(L"Game", CExpr::GetInstance().GetVec());

		pGameVariable->GetGameShareContent() = new GameShareContent;
		ZeroMemory(pGameVariable->GetGameShareContent(), sizeof(GameShareContent));
		pGameVariable->SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_IsRunDlg, TRUE);

		CPlayer Person;
		if (!MyTools::InvokeClassPtr<CPlayerExtend>()->GetPerson(&Person))
		{
			LOG_CF_D(L"Person = NULL;");
			return 0;
		}
		if (!pGameVariable->SetAccountGameShare(Person.GetName()))
		{
			LOG_CF_D(L"SetAccountGameShare = FALSE;");
			return 0;
		}
		MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountStatus.hGameWnd = hGameWnd;
		LOG_CF_D(L"Run in Dlg");
	}
	else if (!MyTools::InvokeClassPtr<CGameClient>()->Run(SERVERIP, SERVERPORT, 3 * 1000))
	{
		::MessageBoxW(NULL, L"连接服务器失败!", L"", NULL);
		ExitProcess(0);
	}

	// Hook PeekMessage
	MyTools::InvokeClassPtr<CExcuteAction>()->SetRun(TRUE);
	pGameVariable->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_IsRunDlg, FALSE, [pGameVariable]
	{
		while (bRunGame && MyTools::InvokeClassPtr<CPersonAttribute>()->GetLevel() == 0)
			::Sleep(3 * 1000);

		CPlayer Person;
		if (!MyTools::InvokeClassPtr<CPlayerExtend>()->GetPerson(&Person))
		{
			LOG_CF_D(L"Person = NULL;");
			return;
		}

		if (!pGameVariable->SetAccountGameShare(Person.GetName()))
		{
			LOG_CF_D(L"SetAccountGameShare = FALSE;");
			return;
		}

		while (bRunGame && EnumWindows((WNDENUMPROC)EnumSetWinName, NULL))
			::Sleep(1000);

		MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountStatus.hGameWnd = hGameWnd;

		// Set Log
		std::wstring wsLogPath = pGameVariable->GetGameShareContent()->wszConsolePath;
		wsLogPath += LR"(\Log\)";

		MyTools::CLog::GetInstance().SetClientName(Person.GetName(), wsLogPath, TRUE, 20 * 1024 * 1024);

		MyTools::CCmdLog::GetInstance().Run(Person.GetName(), CExpr::GetInstance().GetVec());
		MyTools::InvokeClassPtr<CGameClient>()->SetEchoErrorPtr([] {ExitProcess(0); });

		if (!MyTools::InvokeClassPtr<CGameClient>()->GameLogin())
		{
			::MessageBoxW(NULL, L"登录失败……或许是有效时间不足!", L"", NULL);
			ExitProcess(0);
		}
		else
			LOG_C_D(L"Login Succ!");

		// Set KeepALive
		MyTools::InvokeClassPtr<CGameClient>()->RunKeepALive();
		hKeepALiveThread = cbBEGINTHREADEX(NULL, NULL, _KeepALiveThread, NULL, NULL, NULL);
	});
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD dwReason ,LPVOID )
{
	if (dwReason == DLL_PROCESS_ATTACH && hWorkThread == NULL)
	{
		hWorkThread = cbBEGINTHREADEX(NULL, NULL, _WorkThread, NULL, NULL, NULL);
		::CloseHandle(hWorkThread);
		::DisableThreadLibraryCalls(hModule);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		bRunGame = FALSE;
		StopGame;
		MyTools::InvokeClassPtr<CExcuteAction>()->SetRun(FALSE);
		MyTools::CCmdLog::GetInstance().Stop();
		if (hKeepALiveThread != NULL)
		{
			::WaitForSingleObject(hKeepALiveThread, INFINITE);
			hKeepALiveThread = NULL;
		}
	}
	return TRUE;
}

extern "C" __declspec(dllexport)  VOID WINAPIV ExitDLL()
{
	//
}