#ifndef __DHSH_GAMEDLL_GAME_OBJECT_BGAITEM_BAGITEM_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_BGAITEM_BAGITEM_H__

#include "BaseObject.h"

class CNpc;
class CBagItem : public CBaseObject
{
public:
	CBagItem();
	CBagItem(_In_ DWORD dwObjAddr);
	~CBagItem() = default;

	virtual VOID SetName();

	DWORD GetItemId() CONST;

	DWORD GetCount() CONST;

	BOOL UseItem() CONST;

	// 
	BOOL Sell(_In_ CONST CNpc& Npc) CONST;

	//
	VOID Drop() CONST;

	//
	VOID Decompose() CONST;

	// 
	VOID Save() CONST;
private:

};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_BGAITEM_BAGITEM_H__
