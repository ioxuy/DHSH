#ifndef __DHSH_GAMEDLL_GAME_OBJECT_RESNPC_NPC_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_RESNPC_NPC_H__

#include "BaseObject.h"

class CResNpc : public CBaseObject
{
public:
	CResNpc();
	CResNpc(_In_ DWORD dwNodeBase);
	virtual ~CResNpc() = default;

	// Name
	virtual VOID SetName();

	// Npc所在地图
	CONST std::wstring GetNpcMapName() CONST;

	// 坐标
	Point GetPoint() CONST;

	// Npc所在世界地图
	CONST std::wstring GetNpcGlobleMapName() CONST;

	// ResId
	DWORD GetResId() CONST;

private:

};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_NPC_NPC_H__
