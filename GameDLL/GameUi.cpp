#include "stdafx.h"
#include "GameUi.h"
#include <MyTools/Character.h>

CGameUi::CGameUi()
{

}

CGameUi::CGameUi(_In_ DWORD dwNodeBase)
{
	_dwNodeBase = dwNodeBase;
	SetName();
}

VOID CGameUi::SetName()
{
	_wsName = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CHAR*>(GetNodeBase() + 0x60)));
}

BOOL CGameUi::IsShow() CONST
{
	return (ReadDWORD(GetNodeBase() + 0x16C) & 0xFF) > 0;
}
