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

	// ID
	DWORD GetItemId() CONST;

	// 获取数量
	DWORD GetCount() CONST;

	// 使用物品
	BOOL UseItem() CONST;

	// 卖
	BOOL Sell(_In_ CONST CNpc& Npc) CONST;

	// 丢弃
	VOID Drop() CONST;

	// 分解
	VOID Decompose() CONST;

	// 保存
	VOID Save() CONST;

	// 品质
	std::wstring GetItemQuality() CONST;
private:

};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_BGAITEM_BAGITEM_H__
