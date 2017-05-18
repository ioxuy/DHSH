#ifndef __DHSH_GAMEDLL_GAME_OBJECT_NPC_RESNPCEXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_NPC_RESNPCEXTEND_H__

#include "GameBase.h"
#include "ResText.h"

class CResNpc;
class CResNpcExtend : public MyTools::CRelfexBaseClass
{
public:
	CResNpcExtend() = default;
	~CResNpcExtend() = default;
	
	//
	BOOL GetNpcResMapPoint(_In_ CONST std::wstring& wsNpcName, _Out_ CResText::ResNpcMapPointText& NpcResText) CONST;

	//
	BOOL GetResNpc_By_MapName_NpcName(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsNpcName, _Out_ CResText::ResNpcMapPointText& NpcResText);
private:
	// 
	BOOL GetNpcResMapPoint_By_Condition(_Out_ CResText::ResNpcMapPointText& NpcResText, _In_ CONST std::function<BOOL(CONST CResText::ResNpcMapPointText&)> FilterPtr) CONST;

	//
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
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_NPC_NPCEXTEND_H__
