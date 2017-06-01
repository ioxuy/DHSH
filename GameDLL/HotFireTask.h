#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_HOTFIRE_HOTFIRETASK_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_HOTFIRE_HOTFIRETASK_H__

#include "GameVariable.h"

class CTaskObject;
class CHotFireTask : public MyTools::CRelfexBaseClass
{
public:
	CHotFireTask() = default;
	~CHotFireTask() = default;
	
	
	BOOL Run();
	
private:
	// 
	BOOL PickTask() CONST;

	//
	struct RobberContent
	{
		std::wstring wsMonsterName;
		std::wstring wsMapName;
		Point FixPoint;
	};
	BOOL GetMonsterPoint(_In_ CONST CTaskObject& TaskObject, _Out_ RobberContent& RobberContent_) CONST;

	//
	BOOL ExistTask(_Out_ CTaskObject* pTaskObject) CONST;

	// 
	BOOL KillRobber(_In_ CONST RobberContent& RobberContent_) CONST;

	//
	BOOL SumitTask() CONST;
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CHotFireTask*>(lpObjectAddr);
	}
	
	CHotFireTask(CONST CHotFireTask&) = delete;
	
	CHotFireTask& operator=(CONST CHotFireTask&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_HOTFIRE_HOTFIRETASK_H__
