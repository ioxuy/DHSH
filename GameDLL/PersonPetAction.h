#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETACTION_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETACTION_H__

#include "GameBase.h"
class CPersonPetAction : public MyTools::CRelfexBaseClass
{
public:
	CPersonPetAction() = default;
	~CPersonPetAction() = default;
	
	// ≤π≥‰≥ˆ’Ω≥ËŒÔ÷“≥œ∂»
	BOOL SupplementLoyalty() CONST;
	
	// ≤π≥‰HP
	BOOL SupplementHp() CONST;

	// ≤π≥‰MP
	BOOL SupplementMp() CONST;

public:
	static CPersonPetAction* CreateInstance()
	{
		return new CPersonPetAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPersonPetAction*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CPersonPetAction";
		return wsClassName;
	}
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETACTION_H__
