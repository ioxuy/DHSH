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


DWORD dwGetMessageAAddr = NULL;
__declspec(naked) VOID WINAPIV HookGetMessageAMethod()
{
	__asm PUSHAD;

	MyTools::InvokeClassPtr<CExcuteAction>()->ExcutePtr();

	__asm POPAD;
	__asm PUSH EBP;
	__asm MOV EBP, ESP;
	__asm PUSH dwGetMessageAAddr;
	__asm RETN;
}

VOID CExcuteAction::SetRun(_In_ BOOL bRun)
{
	if (bRun)
	{
		if (HookGetMessageAContent.dwHookAddr != NULL)
		{
			LOG_CF_E(L"Do Not Repeat Hook!");
			return;
		}

		HookGetMessageAContent.dwHookAddr = reinterpret_cast<DWORD>(::GetMessageA);
		HookGetMessageAContent.dwFunAddr = reinterpret_cast<DWORD>(HookGetMessageAMethod);
		HookGetMessageAContent.uNopCount = 0;

		dwGetMessageAAddr = HookGetMessageAContent.dwHookAddr + 5;
		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HookGetMessageAContent);
	}
	else
	{
		if (HookGetMessageAContent.dwHookAddr == NULL)
		{
			LOG_CF_E(L"Do Not Repeat Release Hook!");
			return;
		}

		MyTools::CLHook::UnHook_Fun_Jmp_MyAddr(&HookGetMessageAContent);
		HookGetMessageAContent.Release();
	}
}

