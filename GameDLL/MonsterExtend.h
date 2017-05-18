#ifndef __DHSH_GAMEDLL_GAME_OBJECT_MONSTER_MONSTEREXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_MONSTER_MONSTEREXTEND_H__

#include "GameBase.h"

class CMonster;
class CMonsterExtend : public MyTools::CRelfexBaseClass
{
public:
	CMonsterExtend() = default;
	~CMonsterExtend() = default;
	
	// 
	UINT GetVecMonster(_Out_ std::vector<CMonster>& Vec, _In_ std::function<BOOL(CONST CMonster&)> FilterPtr) CONST;
	
	//
	DWORD GetAttackMonsterNodeBase() CONST;
	
public:
	static CMonsterExtend* CreateInstance()
	{
		return new CMonsterExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CMonsterExtend*>(lpObjectAddr);
	}

	CMonsterExtend(CONST CMonsterExtend&) = delete;
	
	CMonsterExtend& operator=(CONST CMonsterExtend&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_MONSTER_MONSTEREXTEND_H__
