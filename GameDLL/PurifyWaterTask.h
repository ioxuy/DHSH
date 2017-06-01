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
	//
	BOOL PickTask() CONST;

	//
	BOOL IsEnoughMoney() CONST;

	//
	std::wstring GetItemName(_In_ CONST CTaskObject& TaskObject) CONST;

	//
	BOOL ExistTask(_Out_ CTaskObject* pTaskObject) CONST;

	//
	BOOL BuyItem(_In_ CONST std::wstring& wsItemName) CONST;

	//
	BOOL MoveToShopNpc(_In_ DWORD dwStoreIndex, _Out_ std::wstring& wsNpcName) CONST;

	//
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
