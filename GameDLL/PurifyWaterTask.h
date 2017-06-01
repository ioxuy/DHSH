#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_PURIFYWATER_PURIFYWATERTASK_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_PURIFYWATER_PURIFYWATERTASK_H__

#include "GameVariable.h"

class CTaskObject;
class CPurifyWaterTask : public MyTools::CRelfexBaseClass
{
public:
	CPurifyWaterTask() = default;
	~CPurifyWaterTask() = default;
	
	BOOL Run();
private:
	// 接任务
	BOOL PickTask() CONST;

	// 获取物资名
	std::wstring GetItemName(_In_ CONST CTaskObject& TaskObject) CONST;

	// 是否存在净水任务
	BOOL ExistTask(_Out_ CTaskObject* pTaskObject) CONST;

	// 购买物资
	BOOL BuyItem(_In_ CONST std::wstring& wsItemName) CONST;

	// 走到商店Npc旁
	BOOL MoveToShopNpc(_In_ DWORD dwStoreIndex, _Out_ std::wstring& wsNpcName) CONST;

	// 交任务
	BOOL SumitTask() CONST;
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPurifyWaterTask*>(lpObjectAddr);
	}
	
	CPurifyWaterTask(CONST CPurifyWaterTask&) = delete;
	
	CPurifyWaterTask& operator=(CONST CPurifyWaterTask&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_PURIFYWATER_PURIFYWATERTASK_H__
