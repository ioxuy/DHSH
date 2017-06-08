#include "stdafx.h"
#include "ScriptServices.h"
#include <MyTools/Log.h>
#include "FarmField.h"
#include "CollectItem.h"
#include "HotFireTask.h"
#include "PurifyWaterTask.h"
#include "BangTask.h"

#define _SELF L"ScriptServices.cpp"
BOOL CScriptServices::Run()
{
	StartGame;
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

	LOG_MSG_CF(L"功能:[%s] 还没做……", wsText.c_str());
	return FALSE;
}
