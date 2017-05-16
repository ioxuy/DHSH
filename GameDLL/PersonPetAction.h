#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETACTION_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETACTION_H__

#include "GameBase.h"
class CPersonPetAction : public MyTools::CRelfexBaseClass
{
public:
	CPersonPetAction() = default;
	~CPersonPetAction() = default;
	
	// 补充出战宠物忠诚度
	BOOL SupplementLoyalty() CONST;
	
	// 补充HP
	BOOL SupplementHp() CONST;

	// 补充MP
	BOOL SupplementMp() CONST;

	// 设置出战宠物
	BOOL SetPetEnterWar() CONST;

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
