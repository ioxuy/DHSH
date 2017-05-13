#include "stdafx.h"
#include "GameVariable.h"

VOID CGameVariable::InitVariable() CONST
{
	// Ä¬ÈÏÊ¹ÓÃÇýÄ§Ïã
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 1);

	// 
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Person_SupplementHp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Person_SupplementMp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementHp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementMp, 70);
}
