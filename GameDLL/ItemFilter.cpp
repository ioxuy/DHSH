#include "stdafx.h"
#include "ItemFilter.h"

CItemFilter::CItemFilter() : _Lock(L"CItemFilter._Lock")
{

}

VOID CItemFilter::SetItemFilterType(_In_ CONST std::wstring& wsItemName, _In_ em_ItemFilterType emType)
{
	_Lock.Access([this, wsItemName, emType] 
	{
		auto itr = _MapItemFilter.find(wsItemName);
		if (itr == _MapItemFilter.end())
			_MapItemFilter.insert(std::make_pair(wsItemName, emType));
		else
			itr->second = emType;
	});
}

CItemFilter::em_ItemFilterType CItemFilter::GetItemFilterType(_In_ CONST std::wstring& wsItemName) CONST
{
	em_ItemFilterType emType = em_ItemFilterType::em_ItemFilterType_Default;

	_Lock.Access([this, wsItemName, &emType] 
	{
		auto itr = _MapItemFilter.find(wsItemName);
		if (itr != _MapItemFilter.end())
			emType = itr->second;
	});
	return emType;
}

UINT CItemFilter::GetVecItem_By_FilterType(_In_ em_ItemFilterType emType, _Out_ std::vector<std::wstring>& Vec) CONST
{
	_Lock.Access([this, emType, &Vec]
	{
		for (CONST auto& itm : _MapItemFilter)
		{
			if (itm.second == emType)
				Vec.push_back(itm.first);
		}
	});
	return Vec.size();
}

