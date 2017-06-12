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
#include "BagItemExtend.h"
#include "LogicBagItemAction.h"
#include "PersonAttribute.h"

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
	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyExorcism) && MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"驱魔香") == 0)
	{
		LOG_CF_D(L"开启了自动购买驱魔香, 但是身上并不存在驱魔香! 去买驱魔香");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"驱魔香", 10))
			return FALSE;
	}

	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_UseExorcism))
	{
		MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism();
	}

	if(MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_UseExorcism) == 0 && MyTools::InvokeClassPtr<CPersonAttribute>()->ExistPersonBuff_By_PartName(L"驱魔香"))
	{
		LOG_CF_D(L"设置不使用驱魔香, 但是却检测到身上有驱魔香BUFF, 放弃驱魔香任务!");
		MyTools::InvokeClassPtr<CLogicBagItemAction>()->RemoveExorcism();
	};

	if (MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyReturnSymbol) && MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemPartName(L"超程符") == 0)
	{
		LOG_CF_D(L"开启了自动购买超程符, 身上不足, 去购买超程符!");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"应天府超程符", 1))
		{
			LOG_CF_E(L"自动购买超程符失败……");
			return FALSE;
		}
	}

	if(MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_AutoBuyHappyBell) && MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(L"欢悦铃") == 0)
	{
		LOG_CF_D(L"身上并不存在欢悦铃! 去买欢悦铃");
		if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->SupplementItem(L"欢悦铃", 10))
		{
			LOG_CF_E(L"自动购买欢悦铃失败……");
			return FALSE;
		}
	};

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
