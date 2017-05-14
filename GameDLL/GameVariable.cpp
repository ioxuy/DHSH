#include "stdafx.h"
#include "GameVariable.h"

VOID CGameVariable::InitVariable() CONST
{
	// Ä¬ÈÏÊ¹ÓÃÇýÄ§Ïã
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 0);

	// 
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Person_SupplementHp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Person_SupplementMp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementHp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementMp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_PersonFightMode, static_cast<DWORD>(em_PersonFightMode::em_PersonFightMode_FixF1));

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_PetFightMode, static_cast<DWORD>(em_PetFightMode::em_PersonFightMode_Denfence));

	// 
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_AutoBuyExorcism, 0);
}

GameShareContent*& CGameVariable::GetGameShareContent() CONST
{
	static GameShareContent* pShareContent = nullptr;
	return pShareContent;
}

GameAccountShareContent*& CGameVariable::GetAccountShareContent() CONST
{
	static GameAccountShareContent* pShareAccount = nullptr;
	return pShareAccount;
}
