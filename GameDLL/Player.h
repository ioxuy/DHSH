#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PLAYER_PLAYER_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PLAYER_PLAYER_H__

#include "BaseObject.h"

class CPlayer : public CBaseObject
{
public:
	enum em_PlayerType
	{
		em_PlayerType_Pet = 0x9,
		em_PlayerType_Monster = 0x4,
		em_PlayerType_Player = 0x2,
		em_PlayerType_Npc = 0x3,
		em_PlayerType_ALL = 0x0,
		em_PlayerType_Self = 0x1
	};
public:
	CPlayer();
	explicit CPlayer(_In_ DWORD dwNodeBase);
	~CPlayer() = default;

	virtual VOID SetName() override;

	virtual DWORD GetId() CONST;

	virtual Point GetPoint() CONST;

	virtual em_PlayerType GetType() CONST;

	virtual CONST std::wstring& GetTextType() CONST;

	virtual float GetDis() CONST;

	virtual DWORD GetResId() CONST;
private:

};




#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PLAYER_PLAYER_H__
