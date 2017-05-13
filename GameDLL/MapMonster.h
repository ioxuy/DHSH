#ifndef __DHSH_GAMEDLL_GAME_OBJECT_MONSTER_MAPMONSTER_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_MONSTER_MAPMONSTER_H__

#include "Player.h"

class CMapMonster : public CPlayer
{
public:
	CMapMonster();
	CMapMonster(_In_ DWORD dwNodeBase);
	~CMapMonster() = default;

private:

};


#endif // !__DHSH_GAMEDLL_GAME_OBJECT_MONSTER_MAPMONSTER_H__
