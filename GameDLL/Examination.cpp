#include "stdafx.h"
#include "Examination.h"
#include <MyTools/Log.h>
#include "Task.h"
#include "TaskExtend.h"
#include "PlayerMove.h"
#include "Npc.h"
#include "NpcExtend.h"

#define _SELF L"Examination.cpp"
BOOL CExamination::Run()
{
	while (GameRun)
	{
		CTaskObject TaskObject(NULL);
		if (!Signup(&TaskObject))
		{
			LOG_MSG_CF(L"报名失败!");
			break;
		}

		MapLocation MapLocation_;
		if (!GetTaskMapLocation(&TaskObject, MapLocation_));
		{
			LOG_MSG_CF(L"获取考试地点失败!");
			break;
		}
	}
}

BOOL CExamination::Signup(_Out_ CTaskObject* pTaskObject)
{
	// Exist Task
	if (ExistTask(pTaskObject))
		return TRUE;

	if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(L"衙门", Point(35, 32)))
		return FALSE;

	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"科举报名官员", [&bOpenDlg, &bClickOption](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			Npc.ClickOption_Once(L"XiangShi");
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"打开Npc[帮派传送员] 失败!");
		return FALSE;
	}

	return ExistTask(pTaskObject);
}

BOOL CExamination::ExistTask(_Out_ CTaskObject* pTaskObject) CONST
{
	return MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"#C20[活动]乡试", nullptr) || MyTools::InvokeClassPtr<CTaskExtend>()->ExistTask_By_PartName_Action(L"#C20[活动]会试", nullptr);
}

BOOL CExamination::GetTaskMapLocation(_In_ CONST CTaskObject* pTaskObject, _Out_ MapLocation& MapLocation_) CONST
{
	return TRUE;
}
