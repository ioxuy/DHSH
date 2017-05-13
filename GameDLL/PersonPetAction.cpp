#include "stdafx.h"
#include "PersonPetAction.h"
#include "PersonPetExtend.h"
#include "PersonPet.h"
#include "BagItemAction.h"

BOOL CPersonPetAction::SupplementLoyalty() CONST
{
	return MyTools::InvokeClassPtr<CPersonPetExtend>()->Action_By_JoinWarPet([] (CONST CPersonPet& Pet)
	{
		if (Pet.GetLoyalty() < 90)
		{
			MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(L"ª∂‘√¡Â");
		}
	});
}

BOOL CPersonPetAction::SupplementHp() CONST
{
	return FALSE;
}

BOOL CPersonPetAction::SupplementMp() CONST
{
	return FALSE;
}
