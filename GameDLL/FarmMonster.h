#ifndef __DHSH_GAMEDLL_GAME_METHOD_FARM_FARMMONSTER_H__
#define __DHSH_GAMEDLL_GAME_METHOD_FARM_FARMMONSTER_H__

#include "GameBase.h"

class CFarmMonster : public MyTools::CRelfexBaseClass
{
public:
	CFarmMonster() = default;
	~CFarmMonster() = default;
	
	BOOL Fight() CONST;
	
	
public:
	static CFarmMonster* CreateInstance()
	{
		return new CFarmMonster;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CFarmMonster*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CFarmMonster";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_FARM_FARMMONSTER_H__
