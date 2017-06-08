#include "stdafx.h"
#include "ItemFilter.h"
#include <MyTools/CLPublic.h>
#include <MyTools/CLFile.h>
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include "GameVariable.h"

#define _SELF L"ItemFilter.cpp"
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

BOOL CItemFilter::ReadItemFilterFile()
{
	std::wstring wsPath = GetFilePath();
	if (!MyTools::CLPublic::FileExist(wsPath))
	{
		LOG_MSG_CF(L"物品过滤文件不存在:%s", wsPath.c_str());
		return FALSE;
	}

	std::vector<ItemFilterFileText> Vec;
	if (!GetItemFilterFileText(wsPath, Vec))
		return FALSE;

	_Lock.Access([this] {_MapItemFilter.clear(); });
	for (CONST auto& itm : Vec)
	{
		auto emType = GetType_By_Text(itm.wsFilterText);
		if (emType == em_ItemFilterType::em_ItemFilterType_Invalid)
		{
			LOG_MSG_CF(L"无效的过滤类型:%s", itm.wsFilterText.c_str());
			return FALSE;
		}

		SetItemFilterType(itm.wsItemName, emType);
	}
	return TRUE;
}

std::wstring CItemFilter::GetFilePath() CONST throw()
{
	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_IsRunDlg))
	{
		std::wstring wsPath = LR"(C:\物品过滤.txt)";
		return std::move(wsPath);
	}

	std::wstring wsPath = MyTools::InvokeClassPtr<CGameVariable>()->GetGameShareContent()->wszConsolePath;
	wsPath += LR"(\物品过滤.txt)";

	return std::move(wsPath);
}

BOOL CItemFilter::GetItemFilterFileText(_In_ CONST std::wstring& wsPath, _Out_ std::vector<CItemFilter::ItemFilterFileText>& Vec) CONST
{
	std::wstring wsContent;
	MyTools::CLFile::ReadASCIIFile(wsPath, wsContent);

	std::vector<std::wstring> VecLine;
	MyTools::CCharacter::Split(wsContent, L"\r\n", VecLine, Split_Option_RemoveEmptyEntries);

	for (CONST auto& itm : VecLine)
	{
		if (itm.find(L"-") == -1)
		{
			LOG_MSG_CF(L"物品过滤格式错误:[%s]", itm.c_str());
			return FALSE;
		}

		std::vector<std::wstring> VecText;
		if (MyTools::CCharacter::Split(itm, L"-", VecText, Split_Option_RemoveEmptyEntries) != 2)
		{
			LOG_MSG_CF(L"物品过滤格式错误:[%s]", itm.c_str());
			return FALSE;
		}

		ItemFilterFileText ItemFilterFileText_ = { VecText.at(0),VecText.at(1) };
		Vec.push_back(std::move(ItemFilterFileText_));
	}

	return TRUE;
}

CItemFilter::em_ItemFilterType CItemFilter::GetType_By_Text(_In_ CONST std::wstring& wsText) CONST throw()
{
	struct ItemFilterText
	{
		std::wstring wsItemFilterText;
		em_ItemFilterType emType;
	};

	CONST static std::vector<ItemFilterText> Vec = 
	{
		{ L"丢弃",em_ItemFilterType::em_ItemFilterType_Drop },
		{ L"分解",em_ItemFilterType::em_ItemFilterType_Decompose },
		{ L"存仓",em_ItemFilterType::em_ItemFilterType_Save },
		{ L"保留",em_ItemFilterType::em_ItemFilterType_Keep },
		{ L"默认",em_ItemFilterType::em_ItemFilterType_Default },
		{ L"卖",em_ItemFilterType::em_ItemFilterType_Sell },
		{ L"采集",em_ItemFilterType::em_ItemFilterType_Collect },
	};

	auto p = MyTools::CLPublic::Vec_find_if_Const(Vec, [wsText](CONST ItemFilterText& ItemFilterText_) { return ItemFilterText_.wsItemFilterText == wsText; });
	return p == nullptr ? em_ItemFilterType::em_ItemFilterType_Invalid : p->emType;
}

