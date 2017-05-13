#ifndef __DHSH_GAMEDLL_GAME_OBJECT_BAGITEM_BAGITEMEXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_BAGITEM_BAGITEMEXTEND_H__
#include "GameBase.h"

class CBagItem;
class CBagItemExtend : public MyTools::CRelfexBaseClass
{
public:
	CBagItemExtend() = default;
	~CBagItemExtend() = default;
	
	// 
	BOOL GetShopEquiResText(_In_ CONST std::wstring& wsEquiName, _Out_ ResShopEquiText& ResEquiText) CONST; 
	
	// 
	BOOL FindItem_By_Name_To_ExcutePtr(_In_ CONST std::wstring& wsItemName, _In_ std::function<VOID(CONST CBagItem&)> ExcutePtr) CONST;

	//
	DWORD GetCount_By_ItemName(_In_ CONST std::wstring& wsItemName) CONST;

	//
	UINT GetVecBagItem(_Out_ std::vector<CBagItem>& Vec, _In_ std::function<BOOL(CONST CBagItem&)> FilterPtr) CONST;
public:
	static CBagItemExtend* CreateInstance()
	{
		return new CBagItemExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CBagItemExtend*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CBagItemExtend";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_BAGITEM_BAGITEMEXTEND_H__
