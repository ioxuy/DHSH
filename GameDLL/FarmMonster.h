#ifndef __DHSH_GAMEDLL_GAME_METHOD_FARM_FARMMONSTER_H__
#define __DHSH_GAMEDLL_GAME_METHOD_FARM_FARMMONSTER_H__

#include "GameBase.h"

class CFarmMonster : public MyTools::CRelfexBaseClass
{
public:
	CFarmMonster() = default;
	~CFarmMonster() = default;
	
	BOOL Fight() CONST;
	
private:
	enum class em_FightSwitch
	{
		em_FightSwitch_None,
		em_FightSwitch_Person,
		em_FightSwitch_Pet
	};
	// 战斗时刻
	em_FightSwitch GetFightSwitch() CONST;

	// 人物的战斗行动
	VOID Fight_Person() CONST;

	// 宠物的战斗行动
	VOID Fight_Pet() CONST;

	//
	enum class em_MouseShape
	{
		em_MouseShape_None,
		em_MouseShape_Skill
	};
	// 战斗鼠标形状
	em_MouseShape GetPersonMouseShape() CONST;
	em_MouseShape GetPetMouseShape() CONST;

	// 
	DWORD GetRoundOrder() CONST;

public:
	static CFarmMonster* CreateInstance()
	{
		return new CFarmMonster;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CFarmMonster*>(lpObjectAddr);
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_FARM_FARMMONSTER_H__
