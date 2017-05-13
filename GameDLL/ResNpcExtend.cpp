#include "stdafx.h"
#include "ResNpcExtend.h"
#include <MyTools/RelfexStruct.h>
#include <MyTools/RelfexClass.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
#include "ResText.h"
#include "ResNpc.h"

BOOL CResNpcExtend::GetNpcResMapPoint(_In_ CONST std::wstring& wsNpcName, _Out_ ResNpcMapPointText& NpcResText) CONST
{
#ifdef GameDLL_Release
	return FALSE;
#else
	auto pVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<ResNpcMapPointText>*>(L"ResNpcPointText");
	return pVec == nullptr ? FALSE : MyTools::CLPublic::Vec_find_if_Const(*pVec, &NpcResText, [wsNpcName](CONST ResNpcMapPointText& Res_) { return Res_.wsNpcName == wsNpcName; });
#endif // GameDLL_Release

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
