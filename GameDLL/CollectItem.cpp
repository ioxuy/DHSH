#include "stdafx.h"
#include "CollectItem.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "GameVariable.h"
#include "BagItemExtend.h"
#include "LogicBagItemAction.h"
#include "PersonAttribute.h"
#include "PersonPetAction.h"
#include "ResText.h"
#include "PlayerMove.h"
#include "NpcExtend.h"
#include "Player.h"
#include "Npc.h"
#include "BagItemAction.h"
#include "FarmMonster.h"

#define _SELF L"CollectItemc.pp"

BOOL CCollectItem::Run(_In_ CONST std::wstring& wsMapName, _In_ CONST std::vector<std::wstring>& VecItem) CONST
{
	auto pVecPath = GetCollectPath_By_MapName(wsMapName);
	if (pVecPath == nullptr)
	{
		LOG_CF_E(L"不存在地图[%s]的采集路径……联系老夫添加!", wsMapName.c_str());
		return FALSE;
	}

	CONST auto pPersonAttribute = MyTools::InvokeClassPtr<CPersonAttribute>();
	while (GameRun)
	{
		for (CONST auto& itm : *pVecPath)
		{
			while (GameRun && pPersonAttribute->GetDis(itm) > 2)
			{
				GameSleep(1000);

				// Check驱魔香, 是否用完了饮料
				if ((!MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism() || GetPetDrinksCount() == 0) && !Check())
				{
					LOG_CF_E(L"采集过程中补充物资失败……");
					return FALSE;
				}

				if (!pPersonAttribute->IsMoving())
				{
					if (pPersonAttribute->IsWar())
					{
						LOG_CF_D(L"采集过程中发生战斗了……");
						MyTools::InvokeClassPtr<CFarmMonster>()->Fight();
						continue;
					}

					// 
					CheckAroundCollectItem(VecItem);
					continue;
				}

				if (pPersonAttribute->GetCurrentMapName() == wsMapName)
					MyTools::InvokeClassPtr<CPlayerMove>()->Move(wsMapName, itm);
				else
					MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(wsMapName, itm);

				GameSleep(1000);
			}
		}
	}
}

BOOL CCollectItem::Check() CONST
{
	BOOL bFaild = FALSE;
	if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"驱魔香") == 0)
	{
		LOG_CF_D(L"开启了自动购买驱魔香, 但是身上并不存在驱魔香! 去买驱魔香");
		bFaild = !MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"驱魔香", 10);
	}
	if (bFaild)
		return FALSE;

	// 检查宠物饮料
	if (GetPetDrinksCount() == 0 && !SupplementPetDrinks())
	{
		LOG_CF_D(L"购买宠物饮料失败!");
		return FALSE;
	}

	return TRUE;
}

DWORD CCollectItem::GetPetDrinksCount() CONST
{
	return MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"宠物饮料");
}

BOOL CCollectItem::SupplementPetDrinks() CONST
{
	DWORD dwCount = MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyPetDrinksCount);
	LOG_CF_D(L"设置需要购买宠物饮料数量=%d", dwCount);

	dwCount -= GetPetDrinksCount();
	LOG_CF_D(L"减去背包的货存,需要购买宠物饮料数量=%d", dwCount);

	if (MyTools::InvokeClassPtr<CBagItemExtend>()->IsBagFull(dwCount / 30 + 1))
	{
		LOG_CF_E(L"背包满了……装不下辣么多的宠物饮料");
		return FALSE;
	}

	return MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"宠物饮料", dwCount);
}

CONST std::vector<Point>* CCollectItem::GetCollectPath_By_MapName(_In_ CONST std::wstring& wsMapName) CONST
{
	auto pVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<CResText::ResCollectItemPath>*>(L"VecResCollectItemPath");
	if (pVec == nullptr)
		return nullptr;

	auto p = MyTools::CLPublic::Vec_find_if_Const(*pVec, [wsMapName](CONST auto& itm) { return itm.wsMapName == wsMapName; });
	return p == nullptr ? nullptr : &p->VecPoint;
}

BOOL CCollectItem::CheckAroundCollectItem(_In_ CONST std::vector<std::wstring>& VecItem) CONST
{
	// Collect Around ResItem
	MyTools::InvokeClassPtr<CNpcExtend>()->FindPlayer_By_Condition_ExcuteAction([VecItem](CONST CPlayer& ResItem)
	{
		return MyTools::CLPublic::Vec_find_if_Const(VecItem, [ResItem](CONST std::wstring& wsItemName) { return wsItemName == ResItem.GetName(); }) != nullptr;
	}, [](CONST CNpc& ResItem) 
	{
		ResItem.Collect();
	});

	// Supplement
	UsePetDrinks();

	// Check Bag! Drop Some Item
	MyTools::InvokeClassPtr<CBagItemAction>()->DropItem();
	return TRUE;
}

BOOL CCollectItem::UsePetDrinks() CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetPetPhysicalStrength() < 50)
	{
		LOG_CF_D(L"宠物的体力<50, 补充一下体力!");
		MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"宠物饮料");
	}
	return TRUE;
}
