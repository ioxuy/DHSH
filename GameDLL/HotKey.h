#ifndef __DHSH_GAMEDLL_GAME_OBJECT_HOTKEY_HOTKEY_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_HOTKEY_HOTKEY_H__

#include "BaseObject.h"

class CHotKey : public CBaseObject
{
public:
	CHotKey();
	explicit CHotKey(_In_ DWORD dwAddr);
	~CHotKey() = default;

	//
	virtual VOID SetName();

	//
	DWORD GetKeyValue() CONST;

	//
	DWORD GetKeyType() CONST;

	//
	DWORD GetOffice() CONST;

	//
	DWORD GetIndex() CONST;
private:

};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_HOTKEY_HOTKEY_H__
