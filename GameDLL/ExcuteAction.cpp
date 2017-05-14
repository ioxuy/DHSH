#include "stdafx.h"
#include "ExcuteAction.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLThread.h>
#include "GameVariable.h"
#include <thread>

#define _SELF L"ExcuteAction.cpp"
CExcuteAction::CExcuteAction() : _LockQueMethodPtr(L"CExcuteAction::_LockQueMethodPtr")
{
	hWndProc = NULL;
}

VOID CExcuteAction::PushPtrToMainThread(_In_ std::function<VOID(VOID)> MethodPtr)
{
	ThreadMethodInfo ThreadMethodInfo_;
	ThreadMethodInfo_.hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	ThreadMethodInfo_.ThreadExcutePtr = MethodPtr;

	_LockQueMethodPtr.Access([this, &ThreadMethodInfo_] {_QueMethodPtr.push(ThreadMethodInfo_); });

	::SendMessageW(MyTools::InvokeClassPtr<CGameVariable>()->GetAccountShareContent()->AccountStatus.hGameWnd, WM_NULL, NULL, NULL);
	::WaitForSingleObject(ThreadMethodInfo_.hEvent, INFINITE);
	LOG_C_D(L"Excute Succ!");
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

HHOOK CExcuteAction::hWndProc = NULL;
LRESULT CALLBACK CExcuteAction::CallWndProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	MyTools::InvokeClassPtr<CExcuteAction>()->ExcutePtr();
	return ::CallNextHookEx(hWndProc, nCode, wParam, lParam);
}

VOID CExcuteAction::SetRun(_In_ BOOL bRun)
{
	static HANDLE hEvent = NULL;
	if (hEvent == NULL)
		hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);

	if (bRun)
	{
		std::thread t([]
		{
			hWndProc = ::SetWindowsHookExW(WH_CALLWNDPROC, CallWndProc, NULL, MyTools::CLThread::GetMainThreadId());
			::WaitForSingleObject(hEvent, INFINITE);
			LOG_C_D(L"Release Hook GetMessage Thread");
		});
		t.detach();
	}
	else
	{
		::UnhookWindowsHookEx(hWndProc);
		::SetEvent(hEvent);
		::Sleep(1000);
		hEvent = NULL;
	}
}

