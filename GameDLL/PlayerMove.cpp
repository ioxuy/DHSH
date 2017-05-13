#include "stdafx.h"
#include "PlayerMove.h"
#include <MyTools/Character.h>

DWORD CPlayerMove::IsMoving() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(C_map_base) + C_map_Addr) + C_ZOU_pi_1) + C_ZOU_pi_2);
}