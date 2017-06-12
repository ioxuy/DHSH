#include "stdafx.h"
#include "ExcuteAction.h"
#include <thread>
#include <future>
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLThread.h>
#include <MyTools/LdrHeader.h>
#include "GameVariable.h"
#include "GameConfig.h"
#include "ScriptServices.h"
#include "GameCALL.h"
#include "PersonAttribute.h"

#define _SELF L"ExcuteAction.cpp"

CExcuteAction::PeekMessageAPtr CExcuteAction::_OldPeekMessagePtr = nullptr;
CExcuteAction::CExcuteAction() : _LockQueMethodPtr(L"CExcuteAction::_LockQueMethodPtr")
{
	_RunComplete = TRUE;
}

VOID CExcuteAction::PushPtrToMainThread(_In_ std::function<VOID(VOID)> MethodPtr)
{
	ThreadMethodInfo ThreadMethodInfo_;
	ThreadMethodInfo_.hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	ThreadMethodInfo_.ThreadExcutePtr = MethodPtr;

	_LockQueMethodPtr.Access([this, &ThreadMethodInfo_] {_QueMethodPtr.push(ThreadMethodInfo_); });

	::PostMessageW(MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountStatus.hGameWnd, WM_NULL, NULL, NULL);
	::WaitForSingleObject(ThreadMethodInfo_.hEvent, INFINITE);
	::CloseHandle(ThreadMethodInfo_.hEvent);
	ThreadMethodInfo_.hEvent = INVALID_HANDLE_VALUE;
}

VOID CExcuteAction::ExcutePtr()
{
	_LockQueMethodPtr.Access([this]
	{
		if (!_QueMethodPtr.empty())
		{
			auto& itm = _QueMethodPtr.front();
			itm.ThreadExcutePtr();
			::SetEvent(itm.hEvent);
			_QueMethodPtr.pop();
		}
	});
}

VOID CExcuteAction::SetRun(_In_ BOOL bRun)
{
	MyTools::CLdrHeader Ldr;

	PeekMessageAPtr RealAddr = reinterpret_cast<PeekMessageAPtr>(::GetProcAddress(::GetModuleHandleW(L"user32.dll"), "PeekMessageA"));
	if (bRun)
		Ldr.InlineHook(RealAddr, PeekMessage_, reinterpret_cast<void **>(&_OldPeekMessagePtr));
	else
		Ldr.UnInlineHook(RealAddr, reinterpret_cast<void *>(_OldPeekMessagePtr));
}

BOOL WINAPI CExcuteAction::PeekMessage_(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax, _In_ UINT wRemoveMsg)
{
	BOOL bRetCode = CExcuteAction::_OldPeekMessagePtr(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);

	if (bRetCode && (lpMsg->message == WM_KEYDOWN || lpMsg->message == WM_SYSKEYDOWN))
	{
		if (lpMsg->wParam == VK_HOME)
			MyTools::InvokeClassPtr<CExcuteAction>()->RunGame();
		else if (lpMsg->wParam == VK_END)
			MyTools::InvokeClassPtr<CExcuteAction>()->Stop();
	}
	MyTools::InvokeClassPtr<CExcuteAction>()->ExcutePtr();
	return bRetCode;
}

VOID CExcuteAction::RunGame()
{
	if (!_RunComplete)
	{
		::MessageBoxW(NULL, L"停止挂机中……", L"错误", NULL);
		return;
	}

	_RunComplete = FALSE;
	std::thread t([this] 
	{
		if (MyTools::InvokeClassPtr<CGameVariable>()->GetGameShareContent() == nullptr || MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent() == nullptr)
		{
			LOG_MSG_CF(L"还没有初始化完毕!");
			_RunComplete = TRUE;
			return;
		}
		auto pGameConfig = MyTools::InvokeClassPtr<CGameConfig>();
		if (!pGameConfig->ExistConfigFile())
		{
			LOG_MSG_CF(L"角色配置文件不存在:%s", pGameConfig->GetConfigPath().c_str());
			_RunComplete = TRUE;
			return;
		}

		if (!pGameConfig->Initialize())
		{
			LOG_MSG_CF(L"Initialize Faild");
			_RunComplete = TRUE;
			return;
		}

		CScriptServices ScriptServices;
		ScriptServices.Run();
		LOG_CF_D(L"线程执行完毕!");
		_RunComplete = TRUE;
	});
	t.detach();
}

VOID CExcuteAction::Stop()
{
	StopGame;
	/*std::thread t([this] 
	{
		

		//WCHAR wszText[MAX_PATH] = { 0 };
		//::GetWindowTextW(MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountStatus.hGameWnd, wszText, MAX_PATH);

		//::SetWindowTextW(MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountStatus.hGameWnd, L"停止挂机中..........");
		//while (!_RunComplete)
		//	::Sleep(1000);
		
		//::SetWindowTextW(MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountStatus.hGameWnd, wszText);
	});
	t.detach();*/
	//StopMove();
}

VOID CExcuteAction::StopMove()
{
	PushPtrToMainThread([] {MyTools::InvokeClassPtr<CGameCALL>()->MoveToPoint_Mouse(MyTools::InvokeClassPtr<CPersonAttribute>()->GetPoint()); });
}
