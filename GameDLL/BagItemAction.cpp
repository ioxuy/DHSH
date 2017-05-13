#include "stdafx.h"
#include "BagItemAction.h"
#include "BagItemExtend.h"
#include "BagItem.h"
#include "ItemFilter.h"
#include <MyTools/Log.h>

#define _SELF L"BagItemAction.cpp"
BOOL CBagItemAction::UseItem(_In_ CONST std::wstring& wsItemName) CONST
{
	return MyTools::InvokeClassPtr<CBagItemExtend>()->FindItem_By_Name_To_ExcutePtr(wsItemName, [](CONST CBagItem& itm) { return itm.UseItem(); });
}

VOID CBagItemAction::DropItem() CONST
{
	std::vector<CBagItem> Vec;
	MyTools::InvokeClassPtr<CBagItemExtend>()->GetVecBagItem(Vec, [](CONST CBagItem& itm) 
	{
		return MyTools::InvokeClassPtr<CItemFilter>()->GetItemFilterType(itm.GetName()) == CItemFilter::em_ItemFilterType::em_ItemFilterType_Drop;
	});

	for (CONST auto& itm : Vec)
	{
		LOG_CF_D(L"DropItem, Name=%s, Count=%d", itm.GetName().c_str(), itm.GetCount());
		itm.Drop();
	}
}

VOID CBagItemAction::Decompose() CONST
{
	std::vector<CBagItem> Vec;
	MyTools::InvokeClassPtr<CBagItemExtend>()->GetVecBagItem(Vec, [](CONST CBagItem& itm)
	{
		return MyTools::InvokeClassPtr<CItemFilter>()->GetItemFilterType(itm.GetName()) == CItemFilter::em_ItemFilterType::em_ItemFilterType_Decompose;
	});

	for (CONST auto& itm : Vec)
	{
		LOG_CF_D(L"DecomposeItem, Name=%s, Count=%d", itm.GetName().c_str(), itm.GetCount());
		itm.Decompose();
	}
}

VOID CBagItemAction::Save() CONST
{
	std::vector<CBagItem> Vec;
	MyTools::InvokeClassPtr<CBagItemExtend>()->GetVecBagItem(Vec, [](CONST CBagItem& itm)
	{
		return MyTools::InvokeClassPtr<CItemFilter>()->GetItemFilterType(itm.GetName()) == CItemFilter::em_ItemFilterType::em_ItemFilterType_Save;
	});

	for (CONST auto& itm : Vec)
	{
		LOG_CF_D(L"SaveItem, Name=%s, Count=%d", itm.GetName().c_str(), itm.GetCount());
		itm.Save();
	}
}
