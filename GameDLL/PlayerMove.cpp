#include "stdafx.h"
#include "PlayerMove.h"
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include <MyTools/CLPublic.h>
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "PersonAttribute.h"
#include "FarmMonster.h"
#include "BagItemAction.h"
#include "BagItemExtend.h"
#include "ResText.h"

#define _SELF L"PlayerMove.cpp"
BOOL CPlayerMove::MoveToPoint(_In_ CONST Point& TarPoint) CONST
{
	return MoveToMapPoint(MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName(), TarPoint);
}

BOOL CPlayerMove::MoveToMapPoint(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST
{
	MyTools::CTimeTick TimeTick;
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();

	while (GameRun && pPersonAttributePtr->GetDis(TarPoint) != 0.0f)
	{
		::Sleep(500);
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick_Second) >= 10 * 1000)
		{
			CONST auto CurPoint = pPersonAttributePtr->GetPoint();

			LOG_CF_E(L"10s之内都无法移动! 当前地图=%s,Tar=[%d,%d], Now=[%d,%d], dis=%.2f", pPersonAttributePtr->GetCurrentMapName().c_str(), TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, pPersonAttributePtr->GetDis(TarPoint));
			StopGame;
			return FALSE;
		}
		if (!pPersonAttributePtr->IsMoving())
		{
			if (pPersonAttributePtr->IsWar())
			{
				LOG_CF_D(L"进入战斗!");
				if (!MyTools::InvokeClassPtr<CFarmMonster>()->Fight())
				{
					LOG_CF_D(L"Fight=FALSE, 跳出走路!");
					return FALSE;
				}

				TimeTick.Reset();
				continue;
			}
			CONST auto CurPoint = pPersonAttributePtr->GetPoint();
			LOG_CF_D(L"MoveToPoint, Tar=[%d,%d], Now=[%d,%d], dis=%.2f", TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, pPersonAttributePtr->GetDis(TarPoint));
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([wsMapName,TarPoint] 
			{ 
				MyTools::InvokeClassPtr<CGameCALL>()->MoveToPoint(MyTools::CCharacter::UnicodeToASCII(wsMapName).c_str(), TarPoint); 
			});
			continue;
		}

		TimeTick.Reset();
	}
	return TRUE;
}

BOOL CPlayerMove::TransferToCity(_In_ CONST std::wstring& wsCityName) CONST
{
	auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();

	CONST static std::vector<std::wstring> VecItemText = { L"回城符",L"超程符" };
	while (GameRun && pPersonAttributePtr->GetCurrentMapName() != wsCityName)
	{
		BOOL bExist = FALSE;
		for (CONST auto& itmText : VecItemText)
		{
			if (MyTools::InvokeClassPtr<CBagItemExtend>()->GetCount_By_ItemName(wsCityName + itmText) != 0)
			{
				MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(wsCityName + itmText);
				::Sleep(1000);
				bExist = TRUE;
				break;
			}
		}

		if (!bExist)
		{
			LOG_CF_D(L"不存在回城符物品[%s]", wsCityName.c_str());
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CPlayerMove::MoveToGeneralStore(_Out_ std::wstring& wsNpcName) CONST
{
	if (TransferToCity(L"星秀村"))
		wsNpcName = L"【星】杂货摊老板";
	else if (TransferToCity(L"应天府"))
		wsNpcName = L"【应】杂货摊老板";
	else if (TransferToCity(L"汴京城"))
		wsNpcName = L"【汴】杂货摊老板";
	else
		return FALSE;

	return MoveToGeneralStoreNpc(wsNpcName);
}

BOOL CPlayerMove::MoveToGeneralStoreNpc(_In_ CONST std::wstring& wsNpcName) CONST
{
	auto pNpcVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<ResNpcMapPointText>*>(L"ResNpcPointText");
	if (pNpcVec == nullptr)
		return FALSE;

	auto pResNpc = MyTools::CLPublic::Vec_find_if_Const(*pNpcVec, [wsNpcName](CONST ResNpcMapPointText& ResNpc) { return ResNpc.wsNpcName == wsNpcName; });
	if (pResNpc == nullptr)
	{
		StopGame;
		LOG_MSG_CF(L"当前资源文件竟然不存在Npc[%s]", wsNpcName.c_str());
		return FALSE;
	}

	if (!MoveToPoint(pResNpc->FixPoint))
	{
		LOG_CF_E(L"走到%s失败!", wsNpcName.c_str());
		return FALSE;
	}

	return TRUE;
}

