#include "stdafx.h"
#include "FarmMonster.h"
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#include "LogicBagItemAction.h"
#include "PersonAttribute.h"
#include "GameVariable.h"
#include "MonsterExtend.h"
#include "ExcuteAction.h"
#include "GameCALL.h"

#define _SELF L"FarmMonster.cpp"
BOOL CFarmMonster::Fight() CONST
{
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	while (GameRun && pPersonAttributePtr->IsWar())
	{
		::Sleep(500);
		switch (GetFightSwitch())
		{
		case em_FightSwitch::em_FightSwitch_None:
			break;
		case em_FightSwitch::em_FightSwitch_Person:
			Fight_Person();
			break;
		case em_FightSwitch::em_FightSwitch_Pet:
			Fight_Pet();
			break;
		default:
			break;
		}
	}

	return MyTools::InvokeClassPtr<CLogicBagItemAction>()->AfterFight_Item();
}

CFarmMonster::em_FightSwitch CFarmMonster::GetFightSwitch() CONST
{
	DWORD dwValue = ReadDWORD(ReadDWORD(C_base_dianguai) + C_base_zhuangtai);
	switch (dwValue)
	{
	case 5:
		return em_FightSwitch::em_FightSwitch_Person;
	case 6:
		return em_FightSwitch::em_FightSwitch_Pet;
	default:
		return em_FightSwitch::em_FightSwitch_None;
	}
}

VOID CFarmMonster::Fight_Person() CONST
{
	LOG_CF_D(L"轮到人物行动");
	em_MouseShape emMouseShape = GetPersonMouseShape();
	switch (static_cast<em_PersonFightMode>(MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_PersonFightMode)))
	{
	case em_PersonFightMode::em_PersonFightMode_FixF1:
		if (emMouseShape == em_MouseShape::em_MouseShape_None)
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] {MyTools::InvokeClassPtr<CGameCALL>()->SetHotKey(VK_F1); });
		else
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] { MyTools::InvokeClassPtr<CGameCALL>()->ClickMonster(MyTools::InvokeClassPtr<CMonsterExtend>()->GetAttackMonsterNodeBase()); });
		break;
	case em_PersonFightMode::em_PersonFightMode_NormalAttack:
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] { MyTools::InvokeClassPtr<CGameCALL>()->ClickMonster(MyTools::InvokeClassPtr<CMonsterExtend>()->GetAttackMonsterNodeBase()); });
		break;
	case em_PersonFightMode::em_PersonFightMode_SwitchF1F2:

		break;
	default:
		break;
	}
}

VOID CFarmMonster::Fight_Pet() CONST
{
	LOG_CF_D(L"轮到宠物行动");
	em_MouseShape emMouseShape = GetPetMouseShape();
	
	switch (static_cast<em_PetFightMode>(MyTools::InvokeClassPtr<CGameVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_PetFightMode)))
	{
	case em_PetFightMode::em_PersonFightMode_Denfence:
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] { MyTools::InvokeClassPtr<CGameCALL>()->Defence(); });
		break;
	case em_PetFightMode::em_PersonFightMode_Skill:
		if (GetPetMouseShape() == em_MouseShape::em_MouseShape_None)
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] {MyTools::InvokeClassPtr<CGameCALL>()->SetHotKey(VK_F2); });
		else
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] { MyTools::InvokeClassPtr<CGameCALL>()->ClickMonster(MyTools::InvokeClassPtr<CMonsterExtend>()->GetAttackMonsterNodeBase()); });
		break;
	default:
		break;
	}
}

CFarmMonster::em_MouseShape CFarmMonster::GetPersonMouseShape() CONST
{
	DWORD dwAddr = ReadDWORD(C_base_dianguai) + C_base_xing;
	return (ReadDWORD(dwAddr) == 5 && ReadDWORD(dwAddr + 0x1C) != NULL) ? em_MouseShape::em_MouseShape_Skill : em_MouseShape::em_MouseShape_None;
}

CFarmMonster::em_MouseShape CFarmMonster::GetPetMouseShape() CONST
{
	// = ReadDWORD(C_base_dianguai) + 0x98
	DWORD dwAddr = ReadDWORD(C_base_dianguai) + C_base_xing + 0x58;

	return (ReadDWORD(dwAddr) == 5 && ReadDWORD(dwAddr + 0x1C) != NULL) ? em_MouseShape::em_MouseShape_Skill : em_MouseShape::em_MouseShape_None;
}

DWORD CFarmMonster::GetRoundOrder() CONST
{
	return ReadDWORD(ReadDWORD(C_base_dianguai) + C_zhandou_huihe) & 0xFF;
}
