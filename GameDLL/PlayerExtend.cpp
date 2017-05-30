#include "stdafx.h"
#include "PlayerExtend.h"
#include <MyTools/Log.h>
#include "Player.h"

BOOL CPlayerExtend::GetPerson(_Out_ CPlayer* pPlayer) CONST
{
	std::vector<CPlayer> Vec;
	if (GetAroundObject(Vec, [](CONST CPlayer& Player) { return Player.GetType() == CPlayer::em_PlayerType::em_PlayerType_Self; }) == 0)
		return FALSE;

	if (pPlayer != nullptr)
		*pPlayer = std::move(Vec.at(0));

	return TRUE;
}
