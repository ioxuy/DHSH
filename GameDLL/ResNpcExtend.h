#ifndef __DHSH_GAMEDLL_GAME_OBJECT_NPC_RESNPCEXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_NPC_RESNPCEXTEND_H__

#include "GameBase.h"

class CResNpc;
class CResNpcExtend : public MyTools::CRelfexBaseClass
{
public:
	CResNpcExtend() = default;
	~CResNpcExtend() = default;
	
	BOOL GetNpcResMapPoint(_In_ CONST std::wstring& wsNpcName, _Out_ ResNpcMapPointText& NpcResText) CONST;
	
	UINT GetVecResNpc(_Out_ std::vector<CResNpc>& Vec) CONST;
public:
	static CResNpcExtend* CreateInstance()
	{
		return new CResNpcExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CResNpcExtend*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CResNpcExtend";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_NPC_NPCEXTEND_H__
