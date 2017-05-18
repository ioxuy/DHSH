#ifndef __DHSH_GAMEDLL_GAME_METHOD_CALL_EXCUTEACTION_H__
#define __DHSH_GAMEDLL_GAME_METHOD_CALL_EXCUTEACTION_H__

#include "GameBase.h"
#include <queue>
#include <MyTools/CLLock.h>
#include <MyTools/CLHook.h>

//#define MESSAGE_CUSTOME WM_USER + 0x1A5
class CExcuteAction : public MyTools::CRelfexBaseClass
{
public:
	struct ThreadMethodInfo
	{
		std::function<VOID(VOID)> ThreadExcutePtr;
		HANDLE                    hEvent;
	};
public:
	CExcuteAction();
	~CExcuteAction() = default;
	
	// 将函数丢到主线程去执行!
	VOID PushPtrToMainThread(_In_ std::function<VOID(VOID)> MethodPtr);

	VOID ExcutePtr();

	VOID SetRun(_In_ BOOL bRun);

private:
	MyTools::MYHOOK_CONTENT HookGetMessageAContent;
private:
	// 主线程执行函数队列
	std::queue<ThreadMethodInfo> _QueMethodPtr;

	// 队列锁
	MyTools::CLLock _LockQueMethodPtr;
public:
	static CExcuteAction* CreateInstance()
	{
		return new CExcuteAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CExcuteAction*>(lpObjectAddr);
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_CALL_EXCUTEACTION_H__
