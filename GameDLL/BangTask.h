#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_BANG_BANGTASK_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_BANG_BANGTASK_H__

#include "GameBase.h"

class CTaskObject;
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
	BOOL PickBangTask(_Out_ CTaskObject* pTaskObject);

	// 
	BOOL ExistBangTask(_Out_ CTaskObject* pTaskObject) CONST;

	//
	BOOL MoveToBangDelivery() CONST;

private: // 打造神兵
	//
	std::wstring GetBangTaskRequestItemQuality(_In_ CONST CTaskObject& TaskObject) CONST;

	// 
	BOOL Task_MakeEquiment(_In_ CONST CTaskObject& TaskObject, _In_ CONST std::wstring& wsItemQuality) CONST;

	//
	BOOL Task_MoveToBlackSmith(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsNpcName, _In_ CONST std::wstring& wsClickText) CONST;

	//
	BOOL BangTask_IsFinishMakeEquiment(_In_ CONST std::wstring& wsItemQuality) CONST;
private: // 击杀强盗
	// 
	BOOL KillRobber(_In_ CONST CTaskObject& TaskObject) CONST;

	//
	struct RobberContent
	{
		std::wstring wsMapName;
		std::wstring wsMonsterName;
		Point TarPoint;
	};
	BOOL GetRobberPoint(_In_ CONST CTaskObject& TaskObject, _Out_ RobberContent& RobberContent_) CONST;
private:
	mutable BOOL _bFinishTask;
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CBangTask*>(lpObjectAddr);
	}
	
	CBangTask(CONST CBangTask&) = delete;
	
	CBangTask& operator=(CONST CBangTask&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_BANG_BANGTASK_H__
