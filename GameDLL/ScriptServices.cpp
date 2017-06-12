#include "stdafx.h"
#include "ScriptServices.h"
#include <MyTools/Log.h>
#include "FarmField.h"
#include "CollectItem.h"
#include "HotFireTask.h"
#include "PurifyWaterTask.h"
#include "BangTask.h"
#include "PersonAction.h"
#include "Examination.h"

#define _SELF L"ScriptServices.cpp"
BOOL CScriptServices::Run()
{
	StartGame;
	CommonAction();

	auto& wsText = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_ConfigAction);
	if (wsText == L"野外")
		return RunPtr<CFarmField>();
	else if (wsText == L"采集")
		return RunPtr<CCollectItem>();
	else if (wsText == L"旺火")
		return RunPtr<CHotFireTask>();
	else if (wsText == L"净水")
		return RunPtr<CPurifyWaterTask>();
	else if (wsText == L"胜木")
		return RunPtr<CBangTask>();
	else if (wsText == L"考试")
		return RunPtr<CExamination>();

	LOG_MSG_CF(L"功能:[%s] 还没做……", wsText.c_str());
	return FALSE;
}

BOOL CScriptServices::CommonCheck()
{
	return TRUE;
}

BOOL CScriptServices::CommonAction() CONST
{
	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_ShieldPlayer))
	{
		LOG_CF_D(L"屏蔽其他玩家!");
		MyTools::InvokeClassPtr<CPersonAction>()->DisableOtherPlayer();
	}

	return TRUE;
}
