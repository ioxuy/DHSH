#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONACTION_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONACTION_H__

#include "GameBase.h"

class CPersonAction : public MyTools::CRelfexBaseClass
{
public:
	CPersonAction() = default;
	~CPersonAction() = default;

	// ≤π≥‰HP
	VOID SupplementHp() CONST;
	
	// ≤π≥‰MP
	VOID SupplementMp() CONST;
public:
	static CPersonAction* CreateInstance()
	{
		return new CPersonAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPersonAction*>(lpObjectAddr);
	}
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONACTION_H__
