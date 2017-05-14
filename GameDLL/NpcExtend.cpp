#include "stdafx.h"
#include "NpcExtend.h"
#include "Npc.h"
#include "PlayerExtend.h"

BOOL CNpcExtend::FindNpc_By_Name_ExcutePtr(_In_ CONST std::wstring& wsNpcName, _In_ std::function<VOID(CONST CNpc&)> ActionPtr)
{
	std::vector<CNpc> VecNpc;
	if (MyTools::InvokeClassPtr<CPlayerExtend>()->GetAroundObject<CNpc>(VecNpc, [wsNpcName](CONST CPlayer& Npc) { return Npc.GetName() == wsNpcName; }) == 0)
		return FALSE;

	if (ActionPtr != nullptr)
		ActionPtr(VecNpc.at(0));
	return TRUE;
}
