#include "stdafx.h"
#include "HotFireTask.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
#include "Task.h"
#include "TaskExtend.h"
#include "PlayerMove.h"
#include "Npc.h"
#include "NpcExtend.h"
#include "GameUiExtend.h"
#include "FarmMonster.h"
#include "ScriptServices.h"

#define _SELF L"HotFireTask.cpp"
BOOL CHotFireTask::Run()
{
	MyTools::InvokeClassPtr<CGameVariable>()->SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 1);
	while (GameRun && CScriptServices::CommonCheck())
	{
		CTaskObject TaskObject(NULL);
		if (!ExistTask(&TaskObject))
		{
			if (!PickTask())
			{
				LOG_CF_E(L"接旺火任务失败!");
				break;
			}
			continue;
		}
		else if(TaskObject.GetTaskContent().find(L"已经干掉") != -1)
		{
			if (!SumitTask())
			{
				LOG_CF_E(L"交旺火任务失败!");
				break;
			}
			continue;
		}

		RobberContent RobberContent_;
		if (!GetMonsterPoint(TaskObject, RobberContent_))
		{
			LOG_CF_E(L"获取旺火怪物坐标失败!");
			break;
		}
		if (!KillRobber(RobberContent_))
		{
			LOG_CF_E(L"击杀旺火怪物失败!");
			break;
		}
	}
	return TRUE;
}

BOOL CHotFireTask::PickTask() CONST
{
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToResNpc(L"阳谷县", L"预备军官"))
	{
		LOG_CF_E(L"走到预备军官旁边失败!");
		return FALSE;
	}

	BOOL bOpenDlg = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"预备军官", [&bOpenDlg](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			if (MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"奖励"))
			{
				LOG_CF_D(L"旺火任务完成!");
				return;
			}
			else if (MyTools::InvokeClassPtr<CGameUiExtend>()->FindText_In_NpcDlg(L"不够"))
			{
				StopGame;
				LOG_MSG_CF(L"%s", MyTools::InvokeClassPtr<CGameUiExtend>()->GetNpcDlgText().c_str());
				return;
			}

			Npc.ClickOption_Once(L"YX,2");
			if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_HotFire_PromiseType) == 0)
			{
				LOG_CF_D(L"花钱去接旺火任务");
				Npc.ClickOption_Once(L"YX,4");
			}
			else
			{
				LOG_CF_D(L"花帮贡去接旺火任务");
				Npc.ClickOption_Once(L"YX,3");
			}
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"打开Npc[预备军官] 失败!");
		return FALSE;
	}

	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	return TRUE;
}

BOOL CHotFireTask::GetMonsterPoint(_In_ CONST CTaskObject& TaskObject, _Out_ RobberContent& RobberContent_) CONST
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
	RobberContent_.FixPoint = Point(std::stoi(VecRobberContent.at(2)), std::stoi(VecRobberContent.at(3)));
	LOG_CF_D(L"眼线名称[%s], 所在地图[%s],坐标[%d,%d]", RobberContent_.wsMonsterName.c_str(), RobberContent_.wsMapName.c_str(), RobberContent_.FixPoint.X, RobberContent_.FixPoint.Y);
	return TRUE;
}

BOOL CHotFireTask::ExistTask(_Out_ CTaskObject* pTaskObject) CONST
{
	return MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"[活动]铲除眼线", [&pTaskObject](CONST CTaskObject& TaskObject)
	{
		MyTools::CLPublic::SetPtr(pTaskObject, TaskObject);
	});;
}

BOOL CHotFireTask::KillRobber(_In_ CONST RobberContent& RobberContent_) CONST
{
	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(RobberContent_.wsMapName, RobberContent_.FixPoint))
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

	LOG_CF_D(L"Fight Done");
	GameSleep(2 * 1000);
	if (MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();

	LOG_CF_D(L"KillRobber Finish");
	return TRUE;
}

BOOL CHotFireTask::SumitTask() CONST
{
	return PickTask();
}
