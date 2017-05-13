#include "stdafx.h"
#include "BaseObject.h"

CONST std::wstring& CBaseObject::GetName() CONST
{
	return _wsName;
}

DWORD CBaseObject::GetNodeBase() CONST
{
	return _dwNodeBase;
}
