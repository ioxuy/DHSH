#include "stdafx.h"
#include "BangTask.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
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
#include "BagItem.h"
#include "FarmMonster.h"

#define _SELF L"BangTask.cpp"
BOOL CBangTask::Check() CONST
{
	MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 1);

	if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"驱魔香") == 0)
	{
		LOG_CF_D(L"开启了自动购买驱魔香, 但是身上并不存在驱魔香! 去买驱魔香");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"驱魔香", 10))
		{
			LOG_CF_E(L"自动购买驱魔香失败……");
			return FALSE;
		}
	}

	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyReturnSymbol) && MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemPartName(L"超程符") == 0)
	{
		LOG_CF_D(L"开启了自动购买超程符, 身上不足, 去购买超程符!");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"应天府超程符", 1))
		{
			LOG_CF_E(L"自动购买超程符失败……");
			return FALSE;
		}
	}

	// 使用驱魔香
	MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism();
	
	// 
	if (MyTools::InvokeClassPtr<CBagItemExtend>()->IsBagFull(2))
	{
		LOG_MSG_CF(L"背包至少要有2个格子!");
		return FALSE;
	}
	return TRUE;
}

BOOL CBangTask::MoveToBang() CONST
{
	if (!Check())
	{
		LOG_CF_E(L"检查补给失败!");
		return FALSE;
	}

	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToResNpc(L"应天府", L"帮派传送员"))
	{
		LOG_CF_E(L"走到帮派传送员失败!");
		return FALSE;
	}

	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"帮派传送员", [&bOpenDlg, &bClickOption](CONST CNpc& Npc)
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

	GameSleep(2 * 1000);
	return MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == L"帮派";
}

BOOL CBangTask::MoveToManagerNpc() CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != L"帮派" && !MoveToBang())
	{
		LOG_CF_E(L"回帮派失败!");
		return FALSE;
	}

	LOG_C_D(L"走到[虎虎家园传送员]");
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(Point(91, 93)))
		return FALSE;

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"虎虎家园传送员", [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			if (!Npc.CLickOption_DisableDlg(L"moveto,6", L"npcdlg") && MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
				MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"打开Npc[虎虎家园传送员] 失败!");
		return FALSE;
	}
	return MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(Point(29, 34));
}

BOOL CBangTask::ExistBangTask(_Out_ CTaskObject* pTaskObject) CONST
{
	return MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"[帮派]", [&pTaskObject](CONST CTaskObject& TaskObject)
	{
		MyTools::CLPublic::SetPtr(pTaskObject, TaskObject);
	});;
}

BOOL CBangTask::MoveToBangDelivery() CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == L"金库")
	{
		LOG_C_D(L"走出金库");
		if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToSpecialMap(L"金库", Point(43, 37), L"帮派"))
			return FALSE;
	}

	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == L"帮派")
	{
		LOG_C_D(L"走到[虎虎家园传送员]");
		return MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(Point(91, 93));
	}
	return TRUE;
}

std::wstring CBangTask::GetBangTaskRequestItemQuality(_In_ CONST CTaskObject& TaskObject) CONST
{
	std::wstring wsItemQuality = TaskObject.GetTaskContent();
	if (wsItemQuality.find(L"打造成") == -1)
		return std::wstring();

	// 把帮派的混天剑打造成神工品质交给金库管理员，各地的铁匠可以帮助您。（当前第2次）
	wsItemQuality = wsItemQuality.substr(wsItemQuality.find(L"打造成") + 3);
	wsItemQuality = wsItemQuality.substr(0, wsItemQuality.find(L"交给"));
	LOG_CF_D(L"帮派任务需要打造的品质[%s]",wsItemQuality.c_str());
	return wsItemQuality;
}

BOOL CBangTask::Task_MakeEquiment(_In_ CONST CTaskObject& TaskObject, _In_ CONST std::wstring& wsItemQuality) CONST
{
	if (!MoveToBangDelivery())
		return FALSE;


	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == L"帮派")
	{
		BOOL bOpenDlg = FALSE;
		MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"虎虎家园传送员", [&bOpenDlg](CONST CNpc& Npc)
		{
			if (Npc.OpenNpcDlg())
			{
				bOpenDlg = TRUE;
				if (!Npc.CLickOption_DisableDlg(L"moveto,1", L"npcdlg") && MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
					MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
			}
		});
		if (!bOpenDlg)
		{
			LOG_CF_E(L"打开Npc[虎虎家园传送员] 失败!");
			return FALSE;
		}
	}
	

	struct BangTask_MakeEquiment_Text
	{
		std::wstring wsMapName;
		std::wstring wsNpcName;
		std::wstring wsClickText;
	};

	CONST static std::vector<BangTask_MakeEquiment_Text> Vec = 
	{
		{ L"应天府",L"应天府铁匠",L"doMake" },
		{ L"星秀村",L"星秀村铁匠",L"doMake" },
		{ L"林中小居",L"林中小居铁匠",L"doMake" },
		{ L"景阳岗",L"景阳岗铁匠",L"doMake" },
		{ L"柴家庄",L"柴家庄铁匠",L"doMake" },
	};

	while (GameRun)
	{
		for (CONST auto& itm : Vec)
		{
			if (!Task_MoveToBlackSmith(itm.wsMapName, itm.wsNpcName, itm.wsClickText))
				return FALSE;

			if (_bFinishTask || BangTask_IsFinishMakeEquiment(wsItemQuality))
			{
				LOG_CF_D(L"帮派任务[打造精品]完成!");
				return TRUE;
			}
		}
		GameSleep(1000);
	}
	return TRUE;
}

BOOL CBangTask::Task_MoveToBlackSmith(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsNpcName, _In_ CONST std::wstring& wsClickText) CONST
{
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToResNpc(wsMapName, wsNpcName))
		return FALSE;

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(wsNpcName, [&bOpenDlg, wsClickText](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			Npc.ClickOption_Once(L"CheckMake");
			Npc.ClickOption_Once(wsClickText);
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"打开Npc[%s] 失败!", wsNpcName.c_str());
		return FALSE;
	}

	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
	{
		if (MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"回复金库管理员吧"))
		{
			LOG_CF_D(L"打造精品任务完成!");
			_bFinishTask = TRUE;
		}
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
	}

	return TRUE;
}

BOOL CBangTask::BangTask_IsFinishMakeEquiment(_In_ CONST std::wstring& wsItemQuality) CONST
{
	BOOL bFinish = FALSE;
	MyTools::InvokeClassPtr<CBagItemExtend>()->FindItem_By_PartName_To_ExcutePtr(L"帮派的", [wsItemQuality, &bFinish](CONST CBagItem& itm)
	{
		bFinish = itm.GetItemQuality() == wsItemQuality;
	});

	return bFinish;
}

BOOL CBangTask::KillRobber(_In_ CONST CTaskObject& TaskObject) CONST
{
	RobberContent RobberContent_;
	if (!GetRobberPoint(TaskObject, RobberContent_))
		return FALSE;

	if (!MoveToBangDelivery())
		return FALSE;

	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == L"帮派")
	{
		BOOL bOpenDlg = FALSE;
		MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"虎虎家园传送员", [&bOpenDlg](CONST CNpc& Npc)
		{
			if (Npc.OpenNpcDlg())
			{
				bOpenDlg = TRUE;
				if (!Npc.CLickOption_DisableDlg(L"moveto,1", L"npcdlg") && MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
					MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
			}
		});
		if (!bOpenDlg)
		{
			LOG_CF_E(L"打开Npc[虎虎家园传送员] 失败!");
			return FALSE;
		}
	}

	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(RobberContent_.wsMapName, RobberContent_.TarPoint))
		return FALSE;

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(RobberContent_.wsMonsterName, [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			Npc.CLickOption_DisableDlg(L"DoFight", L"npcdlg");
		}
	});

	if (!bOpenDlg)
	{
		LOG_CF_E(L"打开Monster[%s] 失败!", RobberContent_.wsMonsterName.c_str());
		return FALSE;
	}

	GameSleep(2 * 1000);
	if (!MyTools::InvokeClassPtr<CFarmMonster>()->Fight())
		return FALSE;

	GameSleep(2 * 1000);
	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	return TRUE;
}

BOOL CBangTask::GetRobberPoint(_In_ CONST CTaskObject& TaskObject, _Out_ RobberContent& RobberContent_) CONST
{
	auto wsContent = TaskObject.GetTaskContent();

	static CONST std::vector<std::wstring> VecText = { L"{",L"}" };

	std::vector<std::wstring> VecResult;
	MyTools::CCharacter::GetSplit_By_List(wsContent, VecText, VecResult, Split_Option_RemoveEmptyEntries);
	if (VecResult.size() == 0)
	{
		LOG_CF_E(L"GetRobberPoint Faild! Text=%s", wsContent.c_str());
		return FALSE;
	}

	// 强盗雷富昌|POS应天府西|12|14
	std::vector<std::wstring> VecRobberContent;
	if (MyTools::CCharacter::Split(VecResult.at(0), L"|", VecRobberContent, Split_Option_RemoveEmptyEntries) != 4)
	{
		LOG_CF_E(L"VecResult=%s", VecResult.at(0).c_str());
		return FALSE;
	}

	RobberContent_.wsMonsterName = VecRobberContent.at(0);
	RobberContent_.wsMapName = VecRobberContent.at(1).substr(3);
	RobberContent_.TarPoint = Point(std::stoi(VecRobberContent.at(2)), std::stoi(VecRobberContent.at(3)));
	LOG_CF_D(L"强盗名称[%s], 所在地图[%s],坐标[%d,%d]", RobberContent_.wsMonsterName.c_str(), RobberContent_.wsMapName.c_str(), RobberContent_.TarPoint.X, RobberContent_.TarPoint.Y);
	return TRUE;
}

BOOL CBangTask::PickBangTask(_Out_ CTaskObject* pTaskObject)
{
	if (!_bFinishTask && ExistBangTask(pTaskObject))
	{
		LOG_CF_D(L"帮派任务已经存在了……");
		return TRUE;
	}

	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != L"金库" && !MoveToManagerNpc())
	{
		LOG_CF_D(L"走到金库管理员失败!");
		return FALSE;
	}
	GameSleep(5 * 1000);

	MyTools::CTimeTick TimeTick;
	do
	{
		DWORD dwNpcId = 0;
		BOOL bOpenDlg = FALSE;
		BOOL bClickOption = FALSE;
		MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"金库管理员", [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
		{
			LOG_CF_D(L"打开金库管理员对话框");
			if (Npc.OpenNpcDlg())
			{
				bOpenDlg = TRUE;

				// get dlg Text = '这是给您的奖励？要多为帮派经营做贡献哦'
				if (MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"这是给您的奖励") || MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"您来得正好")) // Show 
				{
					Npc.ClickOption_Once(L"AcceptTask");
				}
				else
				{
					LOG_C_D(L"放弃任务后需要等待一段时间……设置等待30s后再接任务");
					GameSleep(30 * 1000);
				}
			}
		});
		if (!bOpenDlg)
		{
			LOG_CF_E(L"打开Npc[金库管理员] 失败!");
		}

		GameSleep(1000);
	} while (GameRun && TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Minute) < 7 && !ExistBangTask(nullptr));

	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	_bFinishTask = FALSE;
	return ExistBangTask(pTaskObject);
}

BOOL CBangTask::Run()
{
	_bFinishTask = FALSE;
	while (GameRun)
	{
		CTaskObject CurrentTaskObject(NULL);
		if (!PickBangTask(&CurrentTaskObject))
		{
			LOG_MSG_CF(L"接帮派任务失败!");
			break;
		}

		switch (CurrentTaskObject.GetBangTaskType())
		{
		case CTaskObject::em_Task_Gang_Type::em_Task_Gang_Type_MakeEqui:	
			LOG_CF_D(L"打造精品类任务");
			Task_MakeEquiment(CurrentTaskObject, GetBangTaskRequestItemQuality(CurrentTaskObject));
			break;
		case CTaskObject::em_Task_Gang_Type::em_Task_Gang_Type_KillMonster:
			LOG_CF_D(L"杀强盗类任务");
			KillRobber(CurrentTaskObject);
			break;
		default:
			LOG_MSG_CF(L"未知的任务:[%s]",CurrentTaskObject.GetTaskContent().c_str());
			break;
		}
		_bFinishTask = TRUE;
	}

	return TRUE;
}

