#ifndef __DHSH_GAMEDLL_GAME_TEXT_RES_RESTEXT_H__
#define __DHSH_GAMEDLL_GAME_TEXT_RES_RESTEXT_H__

#include "GameBase.h"
#include <MyTools/RelfexStruct.h>

class CResText : public MyTools::CRelfexBaseClass, public MyTools::CRelfexStruct
{
public:
	struct ResNpcMapPointText
	{
		std::wstring wsNpcName;
		std::wstring wsMapName;
		std::wstring wsGlobleMapName;
		Point        FixPoint;
	};

	enum em_StoreIndex
	{
		em_StoreIndex_Ying_Weaspon = 0x1,
		em_StoreIndex_Ying_Defence = 0x2,
		em_StoreIndex_Ying_Jewelry = 0x3,
		em_StoreIndex_Xing_Weaspon = 0x4,
		em_StoreIndex_Xing_Defence = 0x5,
		em_StoreIndex_Xing_Jewelry = 0x6,
	};

	struct ResShopEquiText
	{
		std::wstring    wsEquiName;
		DWORD			dwStoreIndex;
		DWORD		    dwItemIndex;
	};

	struct ResStoreItemText
	{
		std::wstring  wsItemName;
		std::wstring  wsNpcName;
		DWORD		  dwStoreIndex;
		DWORD		  dwSinglePrice;
	};

	struct ResCollectItemPath
	{
		std::wstring		wsMapName;
		std::vector<Point>	VecPoint;
	};

	struct ResNameId
	{
		std::wstring	wsResName;
		DWORD			dwResId;
	};

public:
	CResText() = default;
	~CResText() = default;
	
	BOOL Initialize();

private:
	// Npc Text
	VOID RegisterResNpcMapText();

	// Shop Equi Text
	VOID RegisterResShopEquiText();

	// Shop Item Text
	VOID RegisterResShopItemText();

	// Map Path Text
	VOID RegisterResMapPathText();

	// 
	VOID RegisterResMapCollectPathText();

	//
	VOID RegisterHomeResNameIdText();

public:
	static CResText* CreateInstance()
	{
		return new CResText;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CResText*>(lpObjectAddr);
	}
};



#endif // !__DHSH_GAMEDLL_GAME_TEXT_RES_RESTEXT_H__
