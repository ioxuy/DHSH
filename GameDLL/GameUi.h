#ifndef __DHSH_GAMEDLL_GAME_OBJECT_GAMEUI_GAMEUI_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_GAMEUI_GAMEUI_H__

#include "BaseObject.h"

class CGameUi : public CBaseObject
{
public:
	CGameUi();
	explicit CGameUi(_In_ DWORD dwNodeBase);
	~CGameUi() = default;

	virtual VOID SetName();

	BOOL IsShow() CONST;
private:

};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_GAMEUI_GAMEUI_H__
