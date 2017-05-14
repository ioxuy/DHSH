#ifndef __DHSH_GAMEDLL_GAME_TEXT_RES_RESTEXT_H__
#define __DHSH_GAMEDLL_GAME_TEXT_RES_RESTEXT_H__

#include "GameBase.h"
#include <MyTools/RelfexStruct.h>

class CResText : public MyTools::CRelfexBaseClass, public MyTools::CRelfexStruct
{
public:
	CResText() = default;
	~CResText() = default;
	
	BOOL Initialize();

private:
	// Npc Text
	VOID RegisterResNpcMapText();

	// Shop Equi Text
	VOID RegisterResShopEquiText();

	// Shop Item Text
	VOID RegisterResShopItemText();
public:
	static CResText* CreateInstance()
	{
		return new CResText;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CResText*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CResText";
		return wsClassName;
	}
};



#endif // !__DHSH_GAMEDLL_GAME_TEXT_RES_RESTEXT_H__
