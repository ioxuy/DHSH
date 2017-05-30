#include "stdafx.h"
#include "ExcuteAction.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLThread.h>
#include <MyTools/LdrHeader.h>
#include "GameVariable.h"
#include <thread>

#define _SELF L"ExcuteAction.cpp"

CExcuteAction::PeekMessageAPtr CExcuteAction::_OldPeekMessagePtr = nullptr;
CExcuteAction::CExcuteAction() : _LockQueMethodPtr(L"CExcuteAction::_LockQueMethodPtr")
{
	
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
		while (!_QueMethodPtr.empty())
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
			CExcuteAction::SendKeyToConsole(WM_CUSTOME_HOME);
		else if (lpMsg->wParam == VK_END)
			CExcuteAction::SendKeyToConsole(WM_CUSTOME_END);
	}
	MyTools::InvokeClassPtr<CExcuteAction>()->ExcutePtr();
	return bRetCode;
}

VOID CExcuteAction::SendKeyToConsole(_In_ DWORD dwMsg)
{
	std::wstring wsConsoleTitle;
	auto pGameVariable = MyTools::InvokeClassPtr<CGameVariable>();
	if (pGameVariable->GetGameShareContent() == nullptr || pGameVariable->GetAccountShareContent() == nullptr || pGameVariable->GetGameShareContent()->GlobalConfig.dwConsoleTitle == NULL)
		return;

	MyTools::CCharacter::FormatText(wsConsoleTitle, L"%08X", pGameVariable->GetGameShareContent()->GlobalConfig.dwConsoleTitle);

	HWND hMainFormDlg = ::FindWindowW(NULL, wsConsoleTitle.c_str());
	if (hMainFormDlg != NULL)
	{
		Point Pt;
		RECT r;
		::GetWindowRect(pGameVariable->GetAccountShareContent()->AccountStatus.hGameWnd, &r);
		Pt.X = r.left;
		Pt.Y = r.top;

		//LOG_CF_D(L"发送消息显示控制台! Point=[%d,%d]", Pt.X, Pt.Y);
		::PostMessageW(hMainFormDlg, dwMsg, pGameVariable->GetAccountShareContent()->uIndex, Pt.Encode());
	}
}

