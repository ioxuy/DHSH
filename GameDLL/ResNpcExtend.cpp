#include "stdafx.h"
#include "ResNpcExtend.h"
#include <MyTools/RelfexStruct.h>
#include <MyTools/RelfexClass.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
#include "ResNpc.h"

BOOL CResNpcExtend::GetNpcResMapPoint(_In_ CONST std::wstring& wsNpcName, _Out_ CResText::ResNpcMapPointText& NpcResText) CONST
{
	return GetNpcResMapPoint_By_Condition(NpcResText, [wsNpcName](CONST auto& itm) { return itm.wsNpcName == wsNpcName; });
}

BOOL CResNpcExtend::GetResNpc_By_MapName_NpcName(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsNpcName, _Out_ CResText::ResNpcMapPointText& NpcResText)
{
	return GetNpcResMapPoint_By_Condition(NpcResText, [wsNpcName, wsMapName](CONST auto& itm) { return itm.wsNpcName == wsNpcName && itm.wsMapName == wsMapName;; });
}

BOOL CResNpcExtend::GetNpcResMapPoint_By_Condition(_Out_ CResText::ResNpcMapPointText& NpcResText, _In_ CONST std::function<BOOL(CONST CResText::ResNpcMapPointText&)> FilterPtr) CONST
{
	auto pVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<CResText::ResNpcMapPointText>*>(L"ResNpcPointText");
	return pVec == nullptr ? FALSE : MyTools::CLPublic::Vec_find_if_Const(*pVec, &NpcResText, FilterPtr);
}

UINT CResNpcExtend::GetVecResNpc(_Out_ std::vector<CResNpc>& Vec) CONST
{
	DWORD dwArrayHead = ReadDWORD(ReadDWORD(0x8BCAD4) + 0x14);
	
	int nCount = static_cast<int>(ReadDWORD(ReadDWORD(0x8BCAD4) + 0x18) - dwArrayHead) / 4;
	for (int i = 0;i < nCount; ++i)
	{
		CResNpc Npc(ReadDWORD(dwArrayHead + i * 4));
		Vec.push_back(std::move(Npc));
	}

	return Vec.size();
}
