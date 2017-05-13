#include "stdafx.h"
#include "FarmMonster.h"
#include "LogicBagItemAction.h"

BOOL CFarmMonster::Fight() CONST
{
	// ...........


	return MyTools::InvokeClassPtr<CLogicBagItemAction>()->AfterFight_Item();
}
