#include "stdafx.h"
#include "ResNpc.h"
#include <MyTools/Character.h>

CResNpc::CResNpc()
{
	_dwNodeBase = NULL;
}

CResNpc::CResNpc(_In_ DWORD dwNodeBase)
{
	_dwNodeBase = dwNodeBase;
	SetName();
}

VOID CResNpc::SetName()
{
	_wsName = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CONST CHAR*>(GetNodeBase() + 0x4)));
}

CONST std::wstring CResNpc::GetNpcMapName() CONST
{
	return MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CONST CHAR*>(GetNodeBase() + 0x24)));
}

Point CResNpc::GetPoint() CONST
{
	return Point(ReadDWORD(GetNodeBase() + 0x84), ReadDWORD(GetNodeBase() + 0x88));
}

CONST std::wstring CResNpc::GetNpcGlobleMapName() CONST
{
	return MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CONST CHAR*>(GetNodeBase() + 0x8C)));
}

DWORD CResNpc::GetResId() CONST
{
	return ReadDWORD(GetNodeBase());
}
