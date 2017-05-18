#ifndef __DHSH_GAMEDLL_GAME_TEXT_VARIABLE_GAMEVARIABLE_H__
#define __DHSH_GAMEDLL_GAME_TEXT_VARIABLE_GAMEVARIABLE_H__

#include "GameBase.h"

class CGameVariable : public MyTools::CRelfexBaseClass, public MyTools::CVariable<em_TextVar, DWORD>
{
public:
	CGameVariable() = default;
	~CGameVariable() = default;
	
	// 初始化血量的设置, 比如<?吃红, 吃蓝
	VOID InitVariable() CONST;
	
	GameShareContent*& GetGameShareContent() CONST;

	GameAccountShareContent*& GetAccountShareContent() CONST;
	
public:
	static CGameVariable* CreateInstance()
	{
		return new CGameVariable;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CGameVariable*>(lpObjectAddr);
	}

	CGameVariable(CONST CGameVariable&) = delete;
	
	CGameVariable& operator=(CONST CGameVariable&) = delete;
};





#endif // !__DHSH_GAMEDLL_GAME_TEXT_VARIABLE_GAMEVARIABLE_H__

