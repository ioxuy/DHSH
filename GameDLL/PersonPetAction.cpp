#include "stdafx.h"
#include "PersonPetAction.h"
#include <MyTools/Log.h>
#include "PersonPetExtend.h"
#include "PersonPet.h"
#include "BagItemAction.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "GameVariable.h"

#define _SELF L"PersonPetAction.cpp"
BOOL CPersonPetAction::SupplementLoyalty() CONST
{
	BOOL bExist = TRUE;
	MyTools::InvokeClassPtr<CPersonPetExtend>()->Action_By_JoinWarPet([&bExist] (CONST CPersonPet& Pet)
	{
		if (Pet.GetLoyalty() < 90)
		{
			bExist = MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"欢悦铃");
		}
	});

	return bExist;
}

BOOL CPersonPetAction::SupplementHp() CONST
{
	BOOL bRetCode = FALSE;
	MyTools::InvokeClassPtr<CPersonPetExtend>()->Action_By_JoinWarPet([&bRetCode](CONST CPersonPet& Pet)
	{
		CONST auto dwValue = MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementHp);
		if (Pet.GetPercentHp() < dwValue)
		{
			LOG_CF_D(L"宠物的HP=%d, 低于设定的值=%d, 吃药!", Pet.GetPercentHp(), dwValue);
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] {MyTools::InvokeClassPtr<CGameCALL>()->AddHp(0x3); });
		}

		bRetCode = Pet.GetPercentHp() >= dwValue;
	});

	return bRetCode;
}

BOOL CPersonPetAction::SupplementMp() CONST
{
	BOOL bRetCode = FALSE;
	MyTools::InvokeClassPtr<CPersonPetExtend>()->Action_By_JoinWarPet([&bRetCode](CONST CPersonPet& Pet)
	{
		CONST auto dwValue = MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementMp);
		if (Pet.GetPercentMp() < dwValue)
		{
			LOG_CF_D(L"宠物的MP=%d, 低于设定的值=%d, 吃药!", Pet.GetPercentMp(), dwValue);
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] {MyTools::InvokeClassPtr<CGameCALL>()->AddHp(0x4); });
		}

		bRetCode = Pet.GetPercentMp() >= dwValue;
	});

	return bRetCode;
}

BOOL CPersonPetAction::SetPetEnterWar() CONST
{
	// Already Exist Pet Enter War
	if (MyTools::InvokeClassPtr<CPersonPetExtend>()->Action_By_JoinWarPet(nullptr))
		return TRUE;

	std::vector<CPersonPet> Vec;
	if (MyTools::InvokeClassPtr<CPersonPetExtend>()->GetVecPet(Vec, nullptr) == 0)
		return FALSE;

	LOG_CF_D(L"设置宠物[%s]参战!",Vec.begin()->GetName().c_str());
	Vec.begin()->EnterWar();
	GameSleep(2 * 1000);
	return TRUE;
}
