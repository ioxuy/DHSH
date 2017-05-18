#ifndef __DHSH_GAMEDLL_GAME_OBJECT_BAGITEM_BAGITEMACTION_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_BAGITEM_BAGITEMACTION_H__

#include "GameBase.h"

class CBagItemAction : public MyTools::CRelfexBaseClass
{
public:
	CBagItemAction() = default;
	~CBagItemAction() = default;
	
	//
	BOOL UseItem(_In_ CONST std::wstring& wsItemName) CONST;

	// by Filter
	VOID DropItem() CONST;

	// by Filter
	VOID Decompose() CONST;
	
	// by Filter
	VOID Save() CONST;
public:
	static CBagItemAction* CreateInstance()
	{
		return new CBagItemAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CBagItemAction*>(lpObjectAddr);
	}

};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_BAGITEM_BAGITEMACTION_H__
