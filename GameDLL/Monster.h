#ifndef __DHSH_GAMEDLL_GAME_OBJECT_MONSTER_MONSTER_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_MONSTER_MONSTER_H__

#include "BaseObject.h"

class CMonster : public CBaseObject
{
public:
	CMonster();
	CMonster(_In_ DWORD dwNodeBase);
	~CMonster() = default;

	virtual VOID SetName() override;

	DWORD GetId() CONST;
private:

};


#endif // !__DHSH_GAMEDLL_GAME_OBJECT_MONSTER_MONSTER_H__
