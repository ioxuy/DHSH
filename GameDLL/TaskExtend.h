#ifndef __DHSH_GAMEDLL_GAME_OBJECT_TASK_TASKEXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_TASK_TASKEXTEND_H__

#include "GameBase.h"

class CTaskObject;
class CTaskExtend : public MyTools::CRelfexBaseClass
{
public:
	CTaskExtend() = default;
	~CTaskExtend() = default;
	
	// 
	UINT GetTask(_Out_ std::vector<CTaskObject>& Vec, _In_ std::function<BOOL(CONST CTaskObject&)> FilterPtr) CONST;
	
	//
	BOOL ExistTask_By_PartName_Action(_In_ CONST std::wstring wsPartName, _In_ std::function<VOID(CONST CTaskObject&)> ActionPtr) CONST;
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CTaskExtend*>(lpObjectAddr);
	}
	
	CTaskExtend(CONST CTaskExtend&) = delete;
	
	CTaskExtend& operator=(CONST CTaskExtend&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_TASK_TASKEXTEND_H__
