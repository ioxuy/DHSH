#include "stdafx.h"
#include "Monster.h"
#include <MyTools/Character.h>

CMonster::CMonster()
{
	_dwNodeBase = NULL;
}

CMonster::CMonster(_In_ DWORD dwNodeBase)
{
	_dwNodeBase = dwNodeBase;
	SetName();
}

VOID CMonster::SetName()
{
	_wsName = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CONST CHAR*>(ReadDWORD(GetNodeBase() + 0x18))));
}

DWORD CMonster::GetId() CONST
{
	return ReadDWORD(GetNodeBase() + 0x1C);
}
