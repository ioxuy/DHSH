#include "stdafx.h"
#include "PurifyWaterTask.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "PlayerMove.h"
#include "GameVariable.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "GameUiExtend.h"
#include "PersonAttribute.h"
#include "Task.h"
#include "TaskExtend.h"
#include "BagItemExtend.h"
#include "ResText.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "ScriptServices.h"

#define _SELF L"PurifyWaterTask.cpp"
BOOL CPurifyWaterTask::Run()
{
	while (GameRun && CScriptServices::CommonCheck())
	{
		CTaskObject TaskObject(NULL);
		if (!ExistTask(&TaskObject))
		{
			if (!PickTask())
			{
				LOG_CF_E(L"接净水任务失败!");
				break;
			}
			continue;
		}

		if (!BuyItem(GetItemName(TaskObject)))
		{
			LOG_CF_E(L"购买军需物资失败~");
			break;
		}

		if (!SumitTask())
		{
			LOG_CF_E(L"交军需物资任务失败~");
			break;
		}
	}
	return TRUE;
}

BOOL CPurifyWaterTask::PickTask() CONST
{
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToResNpc(L"星秀村", L"【星】军需官"))
	{
		LOG_CF_E(L"走到军需官旁边失败!");
		return FALSE;
	}

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"【星】军需官", [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			if (MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"这是您的奖励"))
			{
				LOG_CF_D(L"净水任务完成!");
				return;
			}
			else if (MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"不够"))
			{
				StopGame;
				LOG_MSG_CF(L"%s", MyTools::InvokeClassPtr<CGameUiExtend>()->GetNpcDlgText().c_str());
				return;
			}

			Npc.ClickOption_Once(L"WZCJ,2");
			if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_PurifyWater_PromiseType) == 0)
			{
				LOG_CF_D(L"花钱去接净水任务");
				Npc.ClickOption_Once(L"WZCJ,4");
			}
			else
			{
				LOG_CF_D(L"花帮贡去接净水任务");
				Npc.ClickOption_Once(L"WZCJ,3");
			}
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"打开Npc[【星】军需官] 失败!");
		return FALSE;
	}

	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	return TRUE;
}

std::wstring CPurifyWaterTask::GetItemName(_In_ CONST CTaskObject& TaskObject) CONST
{
	std::wstring wsText = TaskObject.GetTaskContent();
	wsText = wsText.substr(wsText.find(L"去找一个") + 4);
	return wsText.substr(0, wsText.find(L"回来交给"));
}

BOOL CPurifyWaterTask::ExistTask(_Out_ CTaskObject* pTaskObject) CONST
{
	return MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"[活动]物资筹集", [&pTaskObject](CONST CTaskObject& TaskObject)
	{
		MyTools::CLPublic::SetPtr(pTaskObject, TaskObject);
	});;
}

BOOL CPurifyWaterTask::BuyItem(_In_ CONST std::wstring& wsItemName) CONST
{
	LOG_CF_D(L"准备购买物品:%s", wsItemName.c_str());

	CResText::ResShopEquiText ShopEquiText;
	if (!MyTools::InvokeClassPtr<CBagItemExtend>()->GetShopEquiResText(wsItemName, ShopEquiText))
	{
		LOG_MSG_CF(L"物品:[%s]还没记录~联系作者添加该物品!", wsItemName.c_str());
		return FALSE;
	}

	std::wstring wsNpcName;
	if (!MoveToShopNpc(ShopEquiText.dwStoreIndex, wsNpcName))
	{
		LOG_CF_E(L"走到商店Npc[%s]边失败!", wsNpcName.c_str());
		return FALSE;
	}
	
	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	DWORD dwNpcId = 0;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(wsNpcName, [&bOpenDlg,&dwNpcId, &bClickOption](CONST CNpc& Npc)
	{
		if (Npc.GetDis() > 6)
			MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(Npc.GetPoint());
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
		LOG_CF_E(L"点击Npc选项[main1] 失败!");
		return FALSE;
	}

	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([dwNpcId, ShopEquiText]
	{
		MyTools::InvokeClassPtr<CGameCALL>()->Buy_By_Npc(dwNpcId, ShopEquiText.dwItemIndex, 1);
	});

	GameSleep(3000);
	MyTools::InvokeClassPtr<CGameUiExtend>()->CloseDlg(L"shop_npcbuy");
	return TRUE;
}

BOOL CPurifyWaterTask::MoveToShopNpc(_In_ DWORD dwStoreIndex, _Out_ std::wstring& wsNpcName) CONST
{
	switch (static_cast<CResText::em_StoreIndex>(dwStoreIndex))
	{
	case CResText::em_StoreIndex::em_StoreIndex_Ying_Weaspon:
		wsNpcName = L"【应】武器店老板";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"应天府", Point(390, 218), L"武器店"))
		{
			LOG_CF_E(L"走到[武器店]失败!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Ying_Defence:
		wsNpcName = L"【应】防具店老板";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"应天府", Point(441, 236), L"防具店"))
		{
			LOG_CF_E(L"走到[防具店]失败!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Ying_Jewelry:
		wsNpcName = L"【应】饰品店老板";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"应天府", Point(368, 279), L"饰品店"))
		{
			LOG_CF_E(L"走到[饰品店]失败!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Xing_Weaspon:
		wsNpcName = L"【星】武器店老板";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"星秀村", Point(252, 112), L"武器店"))
		{
			LOG_CF_E(L"走到[武器店]失败!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Xing_Defence:
		wsNpcName = L"【星】防具店老板";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"星秀村", Point(221, 179), L"防具店"))
		{
			LOG_CF_E(L"走到[防具店]失败!");
			return FALSE;
		}
		break;
	case CResText::em_StoreIndex::em_StoreIndex_Xing_Jewelry:
		wsNpcName = L"【星】饰品店老板";
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"星秀村", Point(31, 41), L"饰品店"))
		{
			LOG_CF_E(L"走到[饰品店]失败!");
			return FALSE;
		}
		break;
	default:
		LOG_MSG_CF(L"无效的商店索引[%d],联系作者修复!", dwStoreIndex);
		break;
	}
	GameSleep(3 * 1000);
	return TRUE;
}

BOOL CPurifyWaterTask::SumitTask() CONST
{
	return PickTask();
}
