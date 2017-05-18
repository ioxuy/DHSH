#ifndef __DHSH_GAMEDLL_GAME_OBJECT_HOTKEY_HOTKEYEXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_HOTKEY_HOTKEYEXTEND_H__

#include "GameBase.h"

class CHotKey;
class CHotKeyExtend : public MyTools::CRelfexBaseClass
{
public:
	CHotKeyExtend() = default;
	~CHotKeyExtend() = default;
	
	UINT GetVecHotKey(_Out_ std::vector<CHotKey>& Vec) CONST;
	
	
public:
	static CHotKeyExtend* CreateInstance()
	{
		return new CHotKeyExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CHotKeyExtend*>(lpObjectAddr);
	}

};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_HOTKEY_HOTKEYEXTEND_H__
