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
#include "TextVariable.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "TextVariable.h"
#include "ItemFilter.h"
#include "ScriptServices.h"

#define _SELF L"CollectItemc.pp"

BOOL CCollectItem::Run() CONST
{
	if (!Check())
		return FALSE;

	CONST std::wstring wsMapName = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Collect_MapName);
	CONST std::wstring wsCollectItemName = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Collect_ItemName);

	if (wsMapName == L"家园")
		return Collect_In_Home(wsCollectItemName);

	auto pVecPath = GetCollectPath_By_MapName(wsMapName);
	if (pVecPath == nullptr)
	{
		LOG_CF_E(L"不存在地图[%s]的采集路径……联系老夫添加!", wsMapName.c_str());
		return FALSE;
	}

	CONST auto pPersonAttribute = MyTools::InvokeClassPtr<CPersonAttribute>();
	if (pPersonAttribute->GetCurrentMapName() != wsMapName)
	{
		LOG_CF_D(L"跑到采集点去!");
		MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(wsMapName, pVecPath->at(0));
	}

	while (GameRun)
	{
		for (CONST auto& itm : *pVecPath)
		{
			while (GameRun && pPersonAttribute->GetDis(itm) > 2)
			{
				GameSleep(1000);

				if (MyTools::InvokeClassPtr<CBagItemExtend>()->IsBagFull())
				{
					LOG_CF_E(L"背包满了!");
					StopGame;
					break;
				}

				// Check驱魔香, 是否用完了饮料
				if ((!MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism() || GetPetDrinksCount() == 0) && !Check())
				{
					LOG_CF_E(L"采集过程中补充物资失败……");
					return FALSE;
				}

				if (pPersonAttribute->IsWar())
				{
					LOG_CF_D(L"采集过程中发生战斗了……");
					MyTools::InvokeClassPtr<CFarmMonster>()->Fight();
					continue;
				}

				// Check Answer!
				MyTools::InvokeClassPtr<CPlayerMove>()->FindDlg_By_BindMethod();

				if (!pPersonAttribute->IsMoving())
				{
					// 
					CheckAroundCollectItem(wsCollectItemName);
					continue;
				}

				MyTools::InvokeClassPtr<CPlayerMove>()->Move(wsMapName, itm);
				GameSleep(1000);
			}
		}
	}
	return TRUE;
}

BOOL CCollectItem::Check() CONST
{
	// 检查宠物饮料
	if (GetPetDrinksCount() == 0 && !SupplementPetDrinks())
	{
		LOG_CF_D(L"购买宠物饮料失败!");
		return FALSE;
	}

	if (!MyTools::InvokeClassPtr<CItemFilter>()->ReadItemFilterFile())
		return FALSE;

	return CScriptServices::CommonCheck();
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

BOOL CCollectItem::CheckAroundCollectItem(_In_ CONST std::wstring& wsCollectItemName) CONST
{
	// Collect Around ResItem
	BOOL bExist = MyTools::InvokeClassPtr<CNpcExtend>()->FindPlayer_By_Condition_ExcuteAction([wsCollectItemName](CONST CPlayer& ResItem)
	{
		return wsCollectItemName == ResItem.GetName();;
	}, [](CONST CNpc& ResItem) 
	{
		LOG_CF_D(L"Collecting~");
		ResItem.Collect();
	});

	// Supplement
	UsePetDrinks();

	// Check Bag! Drop Some Item
	MyTools::InvokeClassPtr<CBagItemAction>()->DropItem();
	return bExist;
}

BOOL CCollectItem::UsePetDrinks() CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetPetPhysicalStrength() < 50)
	{
		LOG_CF_D(L"宠物的体力<50, 补充一下体力!");
		for (int i = 0; GameRun && i < 3 && MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"宠物饮料"); ++i)
			GameSleep(1000);
	}
	return TRUE;
}

VOID CCollectItem::SetDefaultValue() CONST
{
	
}

BOOL CCollectItem::Collect_In_Home(_In_ CONST std::wstring& wsCollectItemName) CONST
{
	if (!MyTools::InvokeClassPtr<CPersonAttribute>()->IsInHome(TRUE) && !MyTools::InvokeClassPtr<CPlayerMove>()->MoveToHome())
	{
		LOG_CF_E(L"回家园失败……");
		return FALSE;
	}

	//em_TextVar_CollectHome_Furniture
	std::wstring wsFurnitureName = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_CollectHome_Furniture);

	auto pVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<CResText::ResNameId>*>(L"VecHomeRes");
	if (pVec == nullptr)
		return FALSE;

	auto pElement = MyTools::CLPublic::Vec_find_if_Const(*pVec, [wsFurnitureName](CONST auto& itm) { return itm.wsResName == wsFurnitureName; });
	if (pElement == nullptr)
	{
		LOG_MSG_CF(L"家具[%s]并没有记录……联系老夫添加一下!", wsFurnitureName.c_str());
		return FALSE;
	}

	return Collect_In_Courtyard(pElement->dwResId, wsCollectItemName);
}

BOOL CCollectItem::CollectFurniture(_In_ DWORD dwResId) CONST
{
	while (GameRun && MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == L"庭院")
	{
		LOG_CF_D(L"从庭院回到住宅……");
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([]
		{
			MyTools::InvokeClassPtr<CGameCALL>()->MoveToPoint_Mouse(Point(1, 26));
		});
		GameSleep(2000);
	}

	BOOL bExist = MyTools::InvokeClassPtr<CNpcExtend>()->FindPlayer_By_Condition_ExcuteAction([dwResId](CONST CPlayer& itm) { return itm.GetResId() == dwResId; }, [](CONST CNpc& itm) 
	{
		itm.CollectFurniture();
	});

	if (!bExist && GameRun)
	{
		LOG_MSG_CF(L"采集的家具不存在!!!");
		return FALSE;
	}

	GameSleep(2 * 1000);
	return TRUE;
}

BOOL CCollectItem::Collect_In_Courtyard(_In_ DWORD dwResId, _In_ CONST std::wstring& wsCollectItemName) CONST
{
	while (GameRun)
	{
		GameSleep(1000);
		if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetHomeVitalityValue() < 20)
		{
			LOG_CF_D(L"活力值<20,等待中……");
			GameSleep(10 * 1000);
			continue;
		}

		if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != L"庭院")
		{
			// Move To Yard
			BOOL bExist = MyTools::InvokeClassPtr<CNpcExtend>()->FindPlayer_By_Condition_ExcuteAction([](CONST CPlayer& itm) { return itm.GetName().find(L"的管家") != -1; }, [](CONST CNpc& Npc)
			{
				MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(Npc.GetPoint());
				if (Npc.OpenNpcDlg())
				{
					GameSleep(1000);
					Npc.CLickOption_DisableDlg(L"Move2Garden", L"npcdlg");
				}
				
			});

			if (!bExist)
			{
				LOG_MSG_CF(L"住宅管家找不到了……");
				return FALSE;
			}
			continue;
		}

		if (GetPetDrinksCount() == 0 && Check())
		{
			LOG_CF_E(L"采集过程中补充物资失败……");
			return FALSE;
		}

		if (!CheckAroundCollectItem(wsCollectItemName))
		{
			// UnExist Res Packet
			LOG_CF_D(L"周围没有资源包了……去采集家具!");
			CollectFurniture(dwResId);
		}
	}
	return TRUE;
}
