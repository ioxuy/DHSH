#include "stdafx.h"
#include "HotKey.h"
#include <MyTools/Character.h>

CHotKey::CHotKey()
{
	_dwNodeBase = NULL;
}

CHotKey::CHotKey(_In_ DWORD dwAddr)
{
	_dwNodeBase = dwAddr;
}

VOID CHotKey::SetName()
{

}

DWORD CHotKey::GetKeyValue() CONST
{
	return ReadDWORD(GetNodeBase() + 0x0);
}

DWORD CHotKey::GetKeyType() CONST
{
	return ReadDWORD(GetNodeBase() + 0x8);
}

DWORD CHotKey::GetOffice() CONST
{
	return ReadDWORD(GetNodeBase() + 0x10);
}

DWORD CHotKey::GetIndex() CONST
{
	return ReadDWORD(GetNodeBase() + 0x14);
}
