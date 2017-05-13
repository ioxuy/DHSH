#ifndef __DHSH_GAMEDLL_GAME_OBJECT_BASEOBJECT_BASEOBJECT_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_BASEOBJECT_BASEOBJECT_H__

#include "GameBase.h"

class CBaseObject
{
public:
	CBaseObject() = default;
	virtual ~CBaseObject() = default;

	virtual CONST std::wstring& GetName() CONST;
	
	virtual VOID SetName() = NULL;

	DWORD GetNodeBase() CONST;

protected:
	DWORD _dwNodeBase;
	mutable std::wstring _wsName;
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_BASEOBJECT_BASEOBJECT_H__
