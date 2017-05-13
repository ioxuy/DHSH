#ifndef __DHSH_GAMEDLL_GAME_METHOD_FILTER_ITEMFILTER_H__
#define __DHSH_GAMEDLL_GAME_METHOD_FILTER_ITEMFILTER_H__

#include "GameBase.h"
#include <map>

class CItemFilter : public MyTools::CRelfexBaseClass
{
public:
	enum class em_ItemFilterType
	{
		em_ItemFilterType_Drop,
		em_ItemFilterType_Decompose,
		em_ItemFilterType_Save,
		em_ItemFilterType_Keep,
		em_ItemFilterType_Default,
		em_ItemFilterType_Sell
	};

public:
	CItemFilter();
	~CItemFilter() = default;
	
	//
	VOID SetItemFilterType(_In_ CONST std::wstring& wsItemName, _In_ em_ItemFilterType emType);
	
	//
	em_ItemFilterType GetItemFilterType(_In_ CONST std::wstring& wsItemName) CONST;
private:
	std::map<std::wstring, em_ItemFilterType> _MapItemFilter;
	MyTools::CLLock _Lock;
public:
	static CItemFilter* CreateInstance()
	{
		return new CItemFilter;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CItemFilter*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CItemFilter";
		return wsClassName;
	}
};


#endif // !__DHSH_GAMEDLL_GAME_METHOD_FILTER_ITEMFILTER_H__
