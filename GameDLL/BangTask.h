#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_BANG_BANGTASK_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_BANG_BANGTASK_H__

#include "GameBase.h"

class CBangTask : public MyTools::CRelfexBaseClass
{
public:
	CBangTask() = default;
	~CBangTask() = default;
	
	BOOL Run();
	
	BOOL Check() CONST;
private:
	// 回帮派
	BOOL MoveToBang() CONST;

	// 走到'金库管理员'
	BOOL MoveToManagerNpc() CONST;

	// PickTask
	BOOL PickBangTask() CONST;

	// 
	BOOL ExistBangTask() CONST;
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CBangTask*>(lpObjectAddr);
	}
	
	CBangTask(CONST CBangTask&) = delete;
	
	CBangTask& operator=(CONST CBangTask&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_BANG_BANGTASK_H__
