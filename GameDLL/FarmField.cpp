#include "stdafx.h"
#include "FarmField.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include "PersonAttribute.h"
#include "PlayerMove.h"
#include "FarmMonster.h"
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "GameVariable.h"
#include "BagItemAction.h"
#include "BagItemExtend.h"
#include "LogicBagItemAction.h"
#include "PersonPetAction.h"
#include "TextVariable.h"
#include "ScriptServices.h"

#define _SELF L"FarmField.cpp"
BOOL CFarmField::Run() CONST
{
	FieldConfig Config_;
	if (!GetFieldConfig(Config_) || !Check())
		return FALSE;

	LOG_CF_D(L"野外刷怪……目的地是[%s,%d,%d]", Config_.wsMapName.c_str(), Config_.MapPoint.X, Config_.MapPoint.Y);
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	while (GameRun)
	{
		GameSleep(500);
		if (pPersonAttributePtr->GetCurrentMapName() != Config_.wsMapName)
		{
			LOG_CF_D(L"当前地图=[%s], !=目的地[%s], 重新寻路!", pPersonAttributePtr->GetCurrentMapName().c_str(), Config_.wsMapName.c_str());
			if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(Config_.wsMapName, Config_.MapPoint))
			{
				LOG_CF_D(L"MoveToMapPoint Faild!");
				break;
			}

			continue;
		}

		if (!pPersonAttributePtr->IsMoving())
		{
			LOG_CF_D(L"停止走路了……");
			if (pPersonAttributePtr->IsWar())
			{
				LOG_CF_D(L"遭遇怪了!开始战斗!");
				if (!MyTools::InvokeClassPtr<CFarmMonster>()->Fight())
				{
					LOG_CF_D(L"Fight Faild!");
					break;
				}

				GameSleep(2 * 1000);
				continue;
			}

			// Check Answer!
			MyTools::InvokeClassPtr<CPlayerMove>()->FindDlg_By_BindMethod();

			// ALT+Z
			LOG_CF_D(L"发现停止不动了,按ALT+Z开始自动走路!");
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] { MyTools::InvokeClassPtr<CGameCALL>()->SetAutoMove(TRUE); });
			continue;
		}
	}

	MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(pPersonAttributePtr->GetPoint());
	return TRUE;
}

BOOL CFarmField::Check() CONST
{
	return !CScriptServices::CommonCheck() ? FALSE : MyTools::InvokeClassPtr<CLogicBagItemAction>()->AfterFight_Item();
}

BOOL CFarmField::GetFieldConfig(_Out_ FieldConfig& FieldConfig_) CONST
{
	FieldConfig_.wsMapName = MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Field_MapName);
	
	std::vector<std::wstring> VecPoint;
	if (MyTools::CCharacter::Split(MyTools::InvokeClassPtr<CTextVariable>()->GetRefValue_By_Id(em_TextVar::em_TextVar_Field_MapPoint), L",", VecPoint, Split_Option_RemoveEmptyEntries) != 2)
	{
		LOG_MSG_CF(L"坐标格式错误!");
		return FALSE;
	}

	FieldConfig_.MapPoint.X = std::stoi(VecPoint.at(0));
	FieldConfig_.MapPoint.Y = std::stoi(VecPoint.at(1));
	return TRUE;
}

