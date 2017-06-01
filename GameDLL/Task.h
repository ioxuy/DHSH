#ifndef __DHSH_GAMEDLL_GAME_OBJECT_TASK_TASK_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_TASK_TASK_H__

#include "BaseObject.h"

class CTaskObject : public CBaseObject
{
public:
	enum class em_Task_Gang_Type
	{
		em_Task_Gang_Type_None,
		// 打造武器
		em_Task_Gang_Type_MakeEqui,
		// 击杀强盗
		em_Task_Gang_Type_KillMonster,
	};

	struct BangTaskNpc
	{
		std::wstring wsNpcName;
		Point        NpcPoint;
	};
public:
	CTaskObject(_In_ DWORD dwNodeBase);
	~CTaskObject() = default;

	virtual VOID SetName() override;

	// 任务的提示内容
	std::wstring GetTaskContent() CONST;

	// 帮派任务分类
	em_Task_Gang_Type GetBangTaskType() CONST;

	// 帮派任务Npc
	BangTaskNpc GetBangTaskNpc() CONST;

private:
	
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_TASK_TASK_H__
