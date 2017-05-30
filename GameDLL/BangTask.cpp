#include "stdafx.h"
#include "BangTask.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include "BagItemExtend.h"
#include "LogicBagItemAction.h"
#include "GameVariable.h"
#include "PlayerMove.h"
#include "NpcExtend.h"
#include "Npc.h"
#include "PersonAttribute.h"
#include "GameUiExtend.h"
#include "Task.h"
#include "TaskExtend.h"

#define _SELF L"BangTask.cpp"
BOOL CBangTask::Check() CONST
{
	if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"驱魔香") == 0)
	{
		LOG_CF_D(L"开启了自动购买驱魔香, 但是身上并不存在驱魔香! 去买驱魔香");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"驱魔香", 10))
		{
			LOG_CF_E(L"自动购买驱魔香失败……");
			return FALSE;
		}
	}

	if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemPartName(L"超程符") == 0 && MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyReturnSymbol))
	{
		LOG_CF_D(L"开启了自动购买超程符, 身上不足, 去购买超程符!");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"应天府超程符", 1))
		{
			LOG_CF_E(L"自动购买超程符失败……");
			return FALSE;
		}
	}
	
	// 
	if (!MyTools::InvokeClassPtr<CBagItemExtend>()->IsBagFull(2))
	{
		LOG_MSG_CF(L"背包至少要有2个格子!");
		return FALSE;
	}
	return TRUE;
}

BOOL CBangTask::MoveToBang() CONST
{
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToResNpc(L"帮派传送员"))
	{
		LOG_CF_E(L"走到帮派传送员失败!");
		return FALSE;
	}

	DWORD dwNpcId = 0;
	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"帮派传送员", [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			bClickOption = Npc.CLickOption_DisableDlg(L"enter_HQ", L"npcdlg");
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"打开Npc[帮派传送员] 失败!");
		return FALSE;
	}

	if (!bClickOption)
	{
		LOG_CF_E(L"点击Npc选项[回到自己帮派] 失败!");
		return FALSE;
	}

	GameSleep(3 * 1000);
	return MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == L"帮派";
}

BOOL CBangTask::MoveToManagerNpc() CONST
{
	if (!MoveToBang())
		return FALSE;

	return MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"帮派", Point(21, 30), L"金库");
}

BOOL CBangTask::ExistBangTask() CONST
{
	return MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"[帮派]", nullptr);
}

BOOL CBangTask::PickBangTask() CONST
{
	if (!MoveToManagerNpc())
		return FALSE;

	MyTools::CTimeTick TimeTick;
	while (GameRun && TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Minute) < 7 && !ExistBangTask())
	{
		DWORD dwNpcId = 0;
		BOOL bOpenDlg = FALSE;
		BOOL bClickOption = FALSE;
		MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"金库管理员", [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
		{
			if (Npc.OpenNpcDlg())
			{
				bOpenDlg = TRUE;
				if (!Npc.CLickOption_DisableDlg(L"AcceptTask", L"npcdlg") && MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
					MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
			}
		});
		if (!bOpenDlg)
		{
			LOG_CF_E(L"打开Npc[金库管理员] 失败!");
		}

		GameSleep(5 * 1000);
	}

	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	return ExistBangTask();
}

BOOL CBangTask::Run()
{
	while (GameRun)
	{
		if (!PickBangTask())
		{
			LOG_MSG_CF(L"接帮派任务失败!");
			break;
		}


	}

	return TRUE;
}

