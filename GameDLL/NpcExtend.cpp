#include "stdafx.h"
#include "NpcExtend.h"
#include <MyTools/Log.h>
#include "Npc.h"
#include "PlayerExtend.h"

#define _SELF L"NpcExtend.cpp"

BOOL CNpcExtend::FindNpc_By_Name_ExcutePtr(_In_ CONST std::wstring& wsNpcName, _In_ std::function<VOID(CONST CNpc&)> ActionPtr)
{
	return FindPlayer_By_Condition_ExcuteAction([wsNpcName](CONST CPlayer& Npc) { return Npc.GetName() == wsNpcName; }, ActionPtr);
}

BOOL CNpcExtend::FindPlayer_By_Condition_ExcuteAction(_In_ std::function<BOOL(CONST CPlayer&)> CondtionPtr, _In_ std::function<VOID(CONST CNpc&)> ActionPtr)
{
	std::vector<CNpc> VecNpc;
	if (MyTools::InvokeClassPtr<CPlayerExtend>()->GetAroundObject<CNpc>(VecNpc, CondtionPtr) == 0)
		return FALSE;

	if (ActionPtr != nullptr)
		ActionPtr(VecNpc.at(0));
	return TRUE;
}
