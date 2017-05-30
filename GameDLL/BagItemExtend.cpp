#include "stdafx.h"
#include "BagItemExtend.h"
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
#include "BagItem.h"
#include "ExcuteAction.h"

BOOL CBagItemExtend::GetShopEquiResText(_In_ CONST std::wstring& wsEquiName, _Out_ CResText::ResShopEquiText& ResEquiText) CONST
{
#ifdef GameDLL_Release
	return FALSE;
#else
	auto pVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<CResText::ResShopEquiText>*>(L"ResShopEquiText");
	return pVec == nullptr ? FALSE : MyTools::CLPublic::Vec_find_if_Const(*pVec, &ResEquiText, [wsEquiName](CONST CResText::ResShopEquiText& Res_) { return Res_.wsEquiName == wsEquiName; });
#endif
}

BOOL CBagItemExtend::FindItem_By_Name_To_ExcutePtr(_In_ CONST std::wstring& wsItemName, _In_ std::function<VOID(CONST CBagItem & )> ExcutePtr) CONST
{
	std::vector<CBagItem> Vec;
	if (GetVecBagItem(Vec, [wsItemName](CONST CBagItem& item) { return item.GetName() == wsItemName; }) != NULL)
	{
		if(ExcutePtr != nullptr)
			ExcutePtr(Vec.at(0));
		return TRUE;
	}
	return FALSE;
}

DWORD CBagItemExtend::GetCount_By_ItemName(_In_ CONST std::wstring& wsItemName) CONST
{
	std::vector<CBagItem> Vec;
	if (GetVecBagItem(Vec, [wsItemName](CONST CBagItem& item) { return item.GetName() == wsItemName; }) == NULL)
		return 0;

	DWORD dwCount = 0;
	for (CONST auto& itm : Vec)
		dwCount += itm.GetCount();

	return dwCount;
}

DWORD CBagItemExtend::GetCount_By_ItemPartName(_In_ CONST std::wstring& wsItemName) CONST
{
	std::vector<CBagItem> Vec;
	if (GetVecBagItem(Vec, [wsItemName](CONST CBagItem& item) { return item.GetName().find(wsItemName) != -1; }) == NULL)
		return 0;

	DWORD dwCount = 0;
	for (CONST auto& itm : Vec)
		dwCount += itm.GetCount();

	return dwCount;
}

UINT CBagItemExtend::GetVecBagItem(_Out_ std::vector<CBagItem>& Vec, _In_ std::function<BOOL(CONST CBagItem&)> FilterPtr) CONST
{
	DWORD dwRoot = ReadDWORD(ReadDWORD(C_Box_base) + C_Box_yiji);

	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([=, &Vec]
	{
		int nCount = static_cast<int>(ReadDWORD(ReadDWORD(C_Box_base) + C_Box_yiji + 4) - dwRoot) >> 0x2;
		for (int i = 0; i < nCount; ++i) 
		{
			DWORD dwObjAddr = ReadDWORD(dwRoot + i * 4);
			if (ReadDWORD(dwObjAddr + C_Box_lei) != 30)
				continue;

			CBagItem Item_(dwObjAddr);
			if (FilterPtr == nullptr)
			{
				Vec.push_back(std::move(Item_));
				continue;
			}
			else if (FilterPtr(Item_))
			{
				Vec.push_back(std::move(Item_));
				break;
			}
		}
	});
	
	return Vec.size();
}

BOOL CBagItemExtend::IsBagFull(_In_ DWORD dwLastCount) CONST
{
	std::vector<CBagItem> Vec;
	return GetVecBagItem(Vec, nullptr) == 20 - dwLastCount;
}
