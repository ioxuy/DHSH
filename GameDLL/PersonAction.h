#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONACTION_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONACTION_H__

#include "GameBase.h"

class CPersonAction : public MyTools::CRelfexBaseClass
{
public:
	CPersonAction() = default;
	~CPersonAction() = default;

	// ≤π≥‰HP
	BOOL SupplementHp() CONST;
	
	// ≤π≥‰MP
	BOOL SupplementMp() CONST;
public:
	static CPersonAction* CreateInstance()
	{
		return new CPersonAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPersonAction*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CPersonAction";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONACTION_H__
