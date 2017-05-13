#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPET_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPET_H__

#include "BaseObject.h"

class CPersonPet : public CBaseObject
{
public:
	CPersonPet();
	CPersonPet(_In_ DWORD dwNodeBase);
	~CPersonPet() = default;

	virtual VOID SetName() override;

	DWORD GetLoyalty() CONST;

	// 
	BOOL IsJoinWar() CONST;

	//
	DWORD GetHp() CONST;
	DWORD GetMaxHp() CONST;
	DWORD GetPercentHp() CONST;

	DWORD GetMp() CONST;
	DWORD GetMaxMp() CONST;
	DWORD GetPercentMp() CONST;

	//
	DWORD GetId() CONST;

	// 
	VOID EnterWar() CONST;
private:

};


#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPET_H__
