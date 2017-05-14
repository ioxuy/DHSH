#include "stdafx.h"
#include "MonsterExtend.h"
#include <MyTools/Character.h>
#include "Monster.h"

UINT CMonsterExtend::GetVecMonster(_Out_ std::vector<CMonster>& Vec, _In_ std::function<BOOL(CONST CMonster&)> FilterPtr) CONST
{
	DWORD dwAddr = ReadDWORD(ReadDWORD(ReadDWORD(C_Monster_base) + C_Monster_UI) + C_Monster_UI_1);
	for (int i = 0; i < 20; ++i)
	{
		DWORD dwObjAddr = dwAddr + i * 0x98;
		if (ReadDWORD(dwObjAddr) == NULL)
			continue;

		CMonster Monster(dwObjAddr);
		if (FilterPtr == nullptr)
			Vec.push_back(std::move(Monster));
		else if (FilterPtr(Monster))
		{
			Vec.push_back(std::move(Monster));
			break;
		}
	}
	return Vec.size();
}

DWORD CMonsterExtend::GetAttackMonsterNodeBase() CONST
{
	std::vector<CMonster> Vec;
	if (GetVecMonster(Vec, nullptr) == 0)
		return NULL;

	return Vec.at(0).GetNodeBase();
}
