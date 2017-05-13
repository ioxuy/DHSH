#include "stdafx.h"
#include "LogicBagItemAction.h"
#include <MyTools/Log.h>
#include "PersonAttribute.h"
#include "GameVariable.h"
#include "BagItemAction.h"
#include "PersonPetAction.h"
#include "PersonAction.h"

#define _SELF L"LogicBagItemAction.cpp"
BOOL CLogicBagItemAction::AfterFight_Item() CONST
{
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	if (pPersonAttributePtr->IsWar())
		return TRUE;

	// 检查驱魔香
	if (!CheckExorcism())
	{
		LOG_CF_D(L"驱魔香不足!");
		return FALSE;
	}

	// 检查自己的HP MP
	CONST auto pPersonActionPtr = MyTools::InvokeClassPtr<CPersonAction>();
	if (!pPersonActionPtr->SupplementHp())
	{
		LOG_CF_D(L"HP药不足!");
		return FALSE;
	}
	else if (!pPersonActionPtr->SupplementMp())
	{
		LOG_CF_D(L"MP药不足!");
		return FALSE;
	}

	// 检查宠物的HP MP
	CONST auto pPersonPetAction = MyTools::InvokeClassPtr<CPersonPetAction>();
	if (!pPersonPetAction->SupplementHp())
	{
		LOG_CF_D(L"HP药不足!");
		return FALSE;
	}

	if (!pPersonPetAction->SupplementMp())
	{
		LOG_CF_D(L"MP药不足!");
		return FALSE;
	}

	// 检查宠物的忠诚度
	if (!pPersonPetAction->SupplementLoyalty())
	{
		LOG_CF_D(L"欢悦铃不足!");
		return FALSE;
	}

	return TRUE;
}

BOOL CLogicBagItemAction::CheckExorcism() CONST
{
	BOOL bExist = FALSE;
	MyTools::InvokeClassPtr<CGameVariable>()->Action_For_EqualValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 1, [&bExist]
	{
		if (!MyTools::InvokeClassPtr<CPersonAttribute>()->ExistPersonBuff_By_PartName(L"驱魔香剩余时间"))
			bExist = MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"驱魔香");
		else
			bExist = TRUE;
	});

	return bExist;
}

