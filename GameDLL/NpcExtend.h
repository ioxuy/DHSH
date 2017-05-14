#ifndef __DHSH_GAMEDLL_GAME_OBJECT_NPC_NPCEXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_NPC_NPCEXTEND_H__

#include "GameBase.h"

class CNpc;
class CNpcExtend : public MyTools::CRelfexBaseClass
{
public:
	CNpcExtend() = default;
	~CNpcExtend() = default;
	
	BOOL FindNpc_By_Name_ExcutePtr(_In_ CONST std::wstring& wsNpcName, _In_ std::function<VOID(CONST CNpc&)> ActionPtr);
	
	
public:
	static CNpcExtend* CreateInstance()
	{
		return new CNpcExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CNpcExtend*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CNpcExtend";
		return wsClassName;
	}
	
	CNpcExtend(CONST CNpcExtend&) = delete;
	
	CNpcExtend& operator=(CONST CNpcExtend&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_NPC_NPCEXTEND_H__
