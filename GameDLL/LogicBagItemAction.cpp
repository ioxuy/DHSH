#include "stdafx.h"
#include "LogicBagItemAction.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "PersonAttribute.h"
#include "GameVariable.h"
#include "BagItemAction.h"
#include "PersonPetAction.h"
#include "PersonAction.h"
#include "BagItemExtend.h"
#include "PlayerMove.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "ResText.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "GameUiExtend.h"
#include "PersonPetExtend.h"

#define _SELF L"LogicBagItemAction.cpp"
BOOL CLogicBagItemAction::AfterFight_Item() CONST
{
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	if (pPersonAttributePtr->IsWar())
		return TRUE;

	// 检查驱魔香
	if (!CheckExorcism())
	{
		LOG_CF_D(L"驱魔香不足!");
		return FALSE;
	}

	// 检查自己的HP MP
	CONST auto pPersonActionPtr = MyTools::InvokeClassPtr<CPersonAction>();
	if (!pPersonActionPtr->SupplementHp())
	{
		LOG_CF_D(L"HP药不足!");
		return FALSE;
	}
	else if (!pPersonActionPtr->SupplementMp())
	{
		LOG_CF_D(L"MP药不足!");
		return FALSE;
	}

	// 检查宠物的HP MP
	CONST auto pPersonPetAction = MyTools::InvokeClassPtr<CPersonPetAction>();
	if (!pPersonPetAction->SupplementHp())
	{
		LOG_CF_D(L"HP药不足!");
		return FALSE;
	}

	if (!pPersonPetAction->SupplementMp())
	{
		LOG_CF_D(L"MP药不足!");
		return FALSE;
	}

	// 检查宠物的忠诚度
	if (!pPersonPetAction->SupplementLoyalty())
	{
		LOG_CF_D(L"欢悦铃不足!");
		return FALSE;
	}

	return TRUE;
}

BOOL CLogicBagItemAction::CheckExorcism() CONST
{
	BOOL bExist = TRUE;
	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 1, [&bExist]
	{
		if (!MyTools::InvokeClassPtr<CPersonAttribute>()->ExistPersonBuff_By_PartName(L"驱魔香"))
			bExist = MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"驱魔香");
	});

	return bExist;
}

BOOL CLogicBagItemAction::Check() CONST
{
	BOOL bFaild = FALSE;
	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_AutoBuyExorcism, 1, [this, &bFaild]
	{
		if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"驱魔香") == 0)
		{
			LOG_CF_D(L"开启了自动购买驱魔香, 但是身上并不存在驱魔香! 去买驱魔香");
			bFaild = !SupplementItem(L"驱魔香", 10);
		}
	});
	if (bFaild)
		return FALSE;

	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_AutoBuyHappyBell, 1, [this, &bFaild]
	{
		if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"欢悦铃") == 0)
		{
			LOG_CF_D(L"身上并不存在欢悦铃! 去买欢悦铃");
			bFaild = !SupplementItem(L"欢悦铃", 10);
		}
	});
	if (bFaild)
		return FALSE;

	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 0, [this]
	{
		if (MyTools::InvokeClassPtr<CPersonAttribute>()->ExistPersonBuff_By_PartName(L"驱魔香"))
		{
			LOG_CF_D(L"设置不使用驱魔香, 但是却检测到身上有驱魔香BUFF, 放弃驱魔香任务!");
			RemoveExorcism();
		}
	});

	// Set Enter War Pet!
	MyTools::InvokeClassPtr<CPersonPetAction>()->SetPetEnterWar();



	return AfterFight_Item();
}

BOOL CLogicBagItemAction::SupplementItem(_In_ CONST std::wstring& wsItemName, _In_ UINT uCount) CONST
{
	// 背包是否还有空格
	if (MyTools::InvokeClassPtr<CBagItemExtend>()->IsBagFull())
	{
		LOG_CF_E(L"背包满了!");
		return FALSE;
	}

	std::wstring wsNpcName;
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToGeneralStore(wsNpcName))
		return FALSE;

	DWORD dwNpcId = 0;
	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(wsNpcName, [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
	{ 
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			bClickOption = Npc.ClickOption(L"main1", L"shop_npcbuy");
			dwNpcId = Npc.GetId();
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"打开Npc[%s] 失败!", wsNpcName.c_str());
		return FALSE;
	}

	if (!bClickOption)
	{
		LOG_CF_E(L"点击Npc选项[%s] 失败!", wsNpcName.c_str());
		return FALSE;
	}

	auto pVecResItem = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<CResText::ResStoreItemText>*>(L"ResShopItemText");
	if (pVecResItem == nullptr)
		return FALSE;

	auto pResItem = MyTools::CLPublic::Vec_find_if_Const(*pVecResItem, [wsItemName, wsNpcName](CONST CResText::ResStoreItemText& ResItem) { return ResItem.wsItemName == wsItemName && ResItem.wsNpcName == wsNpcName; });
	if (pResItem == nullptr)
	{
		LOG_CF_E(L"不存在资源[%s],[%s]", wsNpcName.c_str(), wsItemName.c_str());
		return FALSE;
	}

	// 够不够钱!
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetMoney() < pResItem->dwSinglePrice * uCount)
	{
		LOG_CF_E(L"不够钱……物品单价[%d],数量[%d],总价[%d],身上只有[%d]", pResItem->dwSinglePrice, uCount, pResItem->dwSinglePrice * uCount, MyTools::InvokeClassPtr<CPersonAttribute>()->GetMoney());
		return FALSE;
	}

	LOG_CF_D(L"购买物品:[%s], 数量:[%d]", pResItem->wsItemName.c_str(), uCount);
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([dwNpcId, pResItem, uCount]
	{
		MyTools::InvokeClassPtr<CGameCALL>()->Buy_By_Npc(dwNpcId, pResItem->dwStoreIndex, uCount);
	});

	GameSleep(1000);

	// Close shop dlg
	if (!MyTools::InvokeClassPtr<CGameUiExtend>()->CloseDlg(L"shop_npcbuy"))
		return FALSE;

	if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(wsItemName) == 0)
	{
		LOG_CF_E(L"网络波动或者延迟导致买[%s]失败!", wsItemName.c_str());
		return FALSE;
	}
	return TRUE;
}

VOID CLogicBagItemAction::RemoveExorcism() CONST
{
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] { MyTools::InvokeClassPtr<CGameCALL>()->AbandonTask(0x7E6); });
}

