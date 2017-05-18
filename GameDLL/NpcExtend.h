#ifndef __DHSH_GAMEDLL_GAME_OBJECT_NPC_NPCEXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_NPC_NPCEXTEND_H__

#include "GameBase.h"

class CNpc;
class CPlayer;
class CNpcExtend : public MyTools::CRelfexBaseClass
{
public:
	CNpcExtend() = default;
	~CNpcExtend() = default;
	
	BOOL FindNpc_By_Name_ExcutePtr(_In_ CONST std::wstring& wsNpcName, _In_ std::function<VOID(CONST CNpc&)> ActionPtr);
	
	BOOL FindPlayer_By_Condition_ExcuteAction(_In_ std::function<BOOL(CONST CPlayer&)> CondtionPtr, _In_ std::function<VOID(CONST CNpc&)> ActionPtr);
public:
	static CNpcExtend* CreateInstance()
	{
		return new CNpcExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CNpcExtend*>(lpObjectAddr);
	}

	CNpcExtend(CONST CNpcExtend&) = delete;
	
	CNpcExtend& operator=(CONST CNpcExtend&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_NPC_NPCEXTEND_H__
