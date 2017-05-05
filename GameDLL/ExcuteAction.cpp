#include "stdafx.h"
#include "ExcuteAction.h"

CExcuteAction::CExcuteAction() : _LockQueMethodPtr(L"CExcuteAction::_LockQueMethodPtr")
{

}

VOID CExcuteAction::PushPtrToMainThread(_In_ std::function<VOID(VOID)> MethodPtr)
{
	ThreadMethodInfo ThreadMethodInfo_;
	ThreadMethodInfo_.hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	ThreadMethodInfo_.ThreadExcutePtr = MethodPtr;

	_LockQueMethodPtr.Access([this, &ThreadMethodInfo_] {_QueMethodPtr.push(ThreadMethodInfo_); });

	::WaitForSingleObject(ThreadMethodInfo_.hEvent, INFINITE);
	::CloseHandle(ThreadMethodInfo_.hEvent);
	ThreadMethodInfo_.hEvent = INVALID_HANDLE_VALUE;
}

DWORD WINAPI CExcuteAction::_ExcuteActionThread(LPVOID lpParm)
{
	auto pExcuteAction = reinterpret_cast<CExcuteAction *>(lpParm);


}
