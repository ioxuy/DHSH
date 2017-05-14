#include "stdafx.h"
#include "TestFunction.h"
#include "GameVariable.h"

VOID CTestFunction::InitTestShareContent() CONST
{
	auto pGameVariable = MyTools::InvokeClassPtr<CGameVariable>();
	pGameVariable->GetGameShareContent() = new GameShareContent;
	pGameVariable->GetAccountShareContent() = &pGameVariable->GetGameShareContent()->arrGameArrount[0];

	auto pAccountShareContent = pGameVariable->GetAccountShareContent();

	pGameVariable->SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_AutoBuyExorcism, 1);
	pGameVariable->SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 1);
}
