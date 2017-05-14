#include "stdafx.h"
#include "FarmField.h"
#include <MyTools/Log.h>
#include "PersonAttribute.h"
#include "PlayerMove.h"
#include "FarmMonster.h"
#include "ExcuteAction.h"
#include "GameCALL.h"

#define _SELF L"FarmField.cpp"
BOOL CFarmField::Run(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST
{
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	while (GameRun)
	{
		::Sleep(500);
		if (pPersonAttributePtr->GetCurrentMapName() != wsMapName)
		{
			if (!MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(wsMapName, TarPoint))
			{
				LOG_CF_D(L"MoveToMapPoint Faild!");
				break;
			}

			continue;
		}

		if (!pPersonAttributePtr->IsMoving())
		{
			if (pPersonAttributePtr->IsWar())
			{
				if (!MyTools::InvokeClassPtr<CFarmMonster>()->Fight())
				{
					LOG_CF_D(L"Fight Faild!");
					break;
				}
				continue;
			}

			// ALT+Z
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([] { MyTools::InvokeClassPtr<CGameCALL>()->SetAutoMove(TRUE); });
			continue;
		}
	}

	MyTools::InvokeClassPtr<CPlayerMove>()->MoveToPoint(pPersonAttributePtr->GetPoint());
	return TRUE;
}

