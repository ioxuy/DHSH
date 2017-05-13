#include "stdafx.h"
#include "PlayerMove.h"
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "PersonAttribute.h"
#include "FarmMonster.h"

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
}

