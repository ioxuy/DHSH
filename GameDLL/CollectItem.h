#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_COLLECT_COLLECTITEM_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_COLLECT_COLLECTITEM_H__

#include "GameBase.h"

class CCollectItem : public MyTools::CRelfexBaseClass
{
public:
	CCollectItem() = default;
	~CCollectItem() = default;
	
	//
	BOOL Run(_In_ CONST std::wstring& wsMapName, _In_ CONST std::vector<std::wstring>& VecItem) CONST;
	
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
	BOOL CheckAroundCollectItem(_In_ CONST std::vector<std::wstring>& VecItem) CONST;

	//
	BOOL UsePetDrinks() CONST;
public:
	static CCollectItem* CreateInstance()
	{
		return new CCollectItem;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CCollectItem*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CCollectItem";
		return wsClassName;
	}
	
	CCollectItem(CONST CCollectItem&) = delete;
	
	CCollectItem& operator=(CONST CCollectItem&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_COLLECT_COLLECTITEM_H__
