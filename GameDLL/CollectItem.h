#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_COLLECT_COLLECTITEM_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_COLLECT_COLLECTITEM_H__

#include "GameBase.h"

class CCollectItem : public MyTools::CRelfexBaseClass
{
public:
	CCollectItem() = default;
	~CCollectItem() = default;
	
	//
	BOOL Run() CONST;
	
	//
	BOOL Check() CONST;

private:
	// 物品‘宠物饮料’的数量
	DWORD GetPetDrinksCount() CONST;

	// 补充饮料
	BOOL SupplementPetDrinks() CONST;

	//
	CONST std::vector<Point>* GetCollectPath_By_MapName(_In_ CONST std::wstring& wsMapName) CONST;

	//
	BOOL CheckAroundCollectItem(_In_ CONST std::wstring& wsCollectItemName) CONST;

	//
	BOOL UsePetDrinks() CONST;

	//
	VOID SetDefaultValue() CONST;

	//
	BOOL Collect_In_Home(_In_ CONST std::wstring& wsCollectItemName) CONST;

	// 
	BOOL CollectFurniture(_In_ DWORD dwResId) CONST;

	//
	BOOL Collect_In_Courtyard(_In_ DWORD dwResId, _In_ CONST std::wstring& wsCollectItemName) CONST;
public:
	static CCollectItem* CreateInstance()
	{
		return new CCollectItem;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CCollectItem*>(lpObjectAddr);
	}
	
	CCollectItem(CONST CCollectItem&) = delete;
	
	CCollectItem& operator=(CONST CCollectItem&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_COLLECT_COLLECTITEM_H__
