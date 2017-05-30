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
#include "MapSearch.h"
#include "GameUiExtend.h"
#include "LogicBagItemAction.h"
#include "GameUi.h"
#include "NpcExtend.h"
#include "Npc.h"
#include "ResNpcExtend.h"

#define _SELF L"PlayerMove.cpp"
BOOL CPlayerMove::MoveToPoint(_In_ CONST Point& TarPoint) CONST
{
	return MoveToMapPoint(MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName(), TarPoint);
}

BOOL CPlayerMove::MoveToMapPoint(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST
{
	MyTools::CTimeTick TimeTick;
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	CONST auto pGameUiExtend = MyTools::InvokeClassPtr<CGameUiExtend>();
	
	while (GameRun && (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != wsMapName || pPersonAttributePtr->GetDis(TarPoint) != 0.0f))
	{
		GameSleep(500);
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick_Second) >= 30 * 1000)
		{
			CONST auto CurPoint = pPersonAttributePtr->GetPoint();

			LOG_CF_E(L"10s之内都无法移动! 当前地图=%s,Tar=[%d,%d], Now=[%d,%d], dis=%.2f", pPersonAttributePtr->GetCurrentMapName().c_str(), TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, pPersonAttributePtr->GetDis(TarPoint));
			StopGame;
			return FALSE;
		}
		if (!pPersonAttributePtr->IsMoving())
		{
			if (!Action_When_UnMove(wsMapName, TarPoint))
				return FALSE;

			TimeTick.Reset();
			continue;
		}

		// 检查驱魔香
		MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism();
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
				LOG_CF_D(L"使用[%s]回城符", wsCityName.c_str());
				MyTools::InvokeClassPtr<CBagItemAction>()->UseItem(wsCityName + itmText);
				GameSleep(3000);
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

std::wstring CPlayerMove::GetRecentlyCityName() CONST
{
	UINT uCount1, uCount2, uCount3 = 0;
	std::wstring wsCurrentMapName = MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName();

	CONST static std::vector<std::wstring> VecMapName = { L"星秀村", L"应天府", L"汴京城"};
	auto pMapName = MyTools::CLPublic::Vec_find_if_Const(VecMapName, [wsCurrentMapName](CONST std::wstring& wsMapName) { return wsCurrentMapName == wsMapName; });
	if (pMapName != nullptr)
		return *pMapName;

	std::vector<std::wstring> VecToCity;
	uCount1 = MyTools::InvokeClassPtr<CMapSearch>()->Search(wsCurrentMapName, L"星秀村", VecToCity) ? VecToCity.size() : UINT_MAX;
	
	VecToCity.clear();
	uCount2 = MyTools::InvokeClassPtr<CMapSearch>()->Search(wsCurrentMapName, L"应天府", VecToCity) ? VecToCity.size() : UINT_MAX;

	VecToCity.clear();
	uCount3 = MyTools::InvokeClassPtr<CMapSearch>()->Search(wsCurrentMapName, L"汴京城", VecToCity) ? VecToCity.size() : UINT_MAX;

	if (uCount1 <= uCount2 && uCount1 <= uCount3)
		return std::wstring(L"星秀村");
	else if (uCount2 <= uCount1 && uCount2 <= uCount3)
		return std::wstring(L"应天府");;
	return std::wstring(L"汴京城");
}

BOOL CPlayerMove::Action_When_UnMove(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->IsWar())
	{
		LOG_CF_D(L"进入战斗!");
		if (!MyTools::InvokeClassPtr<CFarmMonster>()->Fight())
		{
			LOG_CF_D(L"Fight=FALSE, 跳出走路!");
			return FALSE;
		}
	}
	else if (FindDlg_By_BindMethod())
	{
		// 
		GameSleep(1000);
		return TRUE;
	}

	
	Move(wsMapName, TarPoint);
	GameSleep(1000);
	return  TRUE;
}

BOOL CPlayerMove::ShowNpcDlg_When_Move(CONST CGameUi& npcdlg) CONST
{
	BOOL bFind = npcdlg.FindNpcdlgOption_By_PartText_Excute_ActionPtr(L"(寻路)", [](CONST CGameUi::DlgOpionText& dlgText)
	{
		LOG_CF_D(L"走路过程中出现了Npc 寻路 对话框");
		BOOL bFind = MyTools::InvokeClassPtr<CNpcExtend>()->FindPlayer_By_Condition_ExcuteAction([](CONST CPlayer& Player)
		{
			return Player.GetDis() == 0 && Player.GetType() == CPlayer::em_PlayerType_Npc;
		}, [dlgText](CONST CNpc& Npc)
		{
			LOG_CF_D(L"点击Npc选项[%s]->[%s]", dlgText.wsOptionText.c_str(), dlgText.wsClickText.c_str());
			Npc.ClickOption_By_Condition(dlgText.wsClickText, [] { return !MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg(); });
		});

		if (!bFind)
			LOG_MSG_CF(L"不太可能……出现了寻路的对话框, 但是距离Npc距离过远……真的吗?");
	});

	if (!bFind)
	{
		LOG_CF_D(L"走路过程中出现了Npc对话框, 关闭!");
		MyTools::InvokeClassPtr<CGameUiExtend>()->CloseNpcDlg();
	}

	return TRUE;
}

BOOL CPlayerMove::FindDlg_By_BindMethod() CONST
{
	struct UiDlgMethod
	{
		std::wstring wsDlgText;
		std::function<BOOL(CONST CGameUi&)> MethodPtr;
	};

	CONST static std::vector<UiDlgMethod> Vec = 
	{
		{ L"npcdlg", std::bind(&CPlayerMove::ShowNpcDlg_When_Move, this, std::placeholders::_1) },
		{ L"Question", std::bind(&CPlayerMove::ShowQuestion_When_UnMove, this, std::placeholders::_1) },
	};

	std::vector<CGameUi> VecGameUi;
	MyTools::InvokeClassPtr<CGameUiExtend>()->GetVecGameUi(VecGameUi);

	for (CONST auto& itm : VecGameUi)
	{
		for (CONST auto& DlgMethodPtr : Vec)
		{
			if (itm.GetName().find(DlgMethodPtr.wsDlgText) != -1)
				return DlgMethodPtr.MethodPtr(itm);
		}
	}

	return FALSE;
}

BOOL CPlayerMove::MoveToHome() CONST
{
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != L"应天府")
	{
		if (!TransferToCity(L"应天府"))
		{
			LOG_CF_E(L"没有应天府的传送符……怎么回家园?");
			return FALSE;
		}
	}

	CResText::ResNpcMapPointText ResNpcPoint;
	if (!MyTools::InvokeClassPtr<CResNpcExtend>()->GetResNpc_By_MapName_NpcName(L"应天府",L"房屋管理员", ResNpcPoint))
	{
		LOG_CF_E(L"Npc资源里面竟然不存在[%s:%s],联系老夫!", L"应天府", L"房屋管理员");
		return FALSE;
	}
	

	if (!MoveToPoint(ResNpcPoint.FixPoint))
		return FALSE;


	DWORD dwNpcId = 0;
	BOOL bOpenDlg = FALSE;
	BOOL bClickOption = FALSE;
	MyTools::InvokeClassPtr<CNpcExtend>()->FindNpc_By_Name_ExcutePtr(L"房屋管理员", [&bOpenDlg, &bClickOption, &dwNpcId](CONST CNpc& Npc)
	{
		if (Npc.OpenNpcDlg())
		{
			bOpenDlg = TRUE;
			bClickOption = Npc.CLickOption_DisableDlg(L"DoEnterMyHome", L"npcdlg");
		}
	});
	if (!bOpenDlg)
	{
		LOG_CF_E(L"打开Npc[房屋管理员] 失败!");
		return FALSE;
	}

	if (!bClickOption)
	{
		LOG_CF_E(L"点击Npc选项[房屋管理员] 失败!");
		return FALSE;
	}

	return TRUE;
}

BOOL CPlayerMove::MoveToResNpc(_In_ CONST std::wstring& wsNpcName) CONST
{
	CResText::ResNpcMapPointText ResNpcPoint;
	if (!MyTools::InvokeClassPtr<CResNpcExtend>()->GetResNpc_By_MapName_NpcName(L"应天府", L"帮派传送员", ResNpcPoint))
	{
		LOG_MSG_CF(L"Npc资源里面竟然不存在[%s:%s],联系老夫!", L"应天府", L"帮派传送员");
		return FALSE;
	}

	std::wstring wsCurrentMapName = MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName();
	if (!MyTools::InvokeClassPtr<CMapSearch>()->Exist(wsCurrentMapName))
	{
		if (TransferToCity(L"应天府") || TransferToCity(L"星秀村") || TransferToCity(L"汴京城"))
			LOG_CF_D(L"由于处于非大地图的范围……所以先使用回城符!");
		else
		{
			LOG_MSG_CF(L"当前地图:[%s] 不可识别,并且身上不存在回城符……先跑到大地图再开始好吗?");
			StopGame;
			return FALSE;
		}
	}

	if (!TransferToCity(L"应天府"))
	{
		std::wstring wsCityName = GetRecentlyCityName();
		LOG_CF_D(L"当前离你最近的城市是:%s", wsCityName.c_str());
		TransferToCity(wsCityName);
	}

	return MoveToMapPoint(L"应天府", ResNpcPoint.FixPoint);

}

BOOL CPlayerMove::MoveToSpecialMap(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint, _In_ CONST std::wstring wsSpecialMapName) CONST
{
	MyTools::CTimeTick TimeTick;
	CONST auto pPersonAttributePtr = MyTools::InvokeClassPtr<CPersonAttribute>();
	CONST auto pGameUiExtend = MyTools::InvokeClassPtr<CGameUiExtend>();

	while (GameRun && (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() != wsSpecialMapName))
	{
		GameSleep(500);
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick_Second) >= 10 * 1000)
		{
			CONST auto CurPoint = pPersonAttributePtr->GetPoint();

			LOG_CF_E(L"10s之内都无法移动! 当前地图=%s,Tar=[%d,%d], Now=[%d,%d], dis=%.2f", pPersonAttributePtr->GetCurrentMapName().c_str(), TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, pPersonAttributePtr->GetDis(TarPoint));
			StopGame;
			return FALSE;
		}
		if (!pPersonAttributePtr->IsMoving())
		{
			if (!Action_When_UnMove(wsMapName, TarPoint))
				return FALSE;

			TimeTick.Reset();
			continue;
		}

		// 检查驱魔香
		MyTools::InvokeClassPtr<CLogicBagItemAction>()->CheckExorcism();
		TimeTick.Reset();
	}
	return TRUE;
}

BOOL CPlayerMove::ShowQuestion_When_UnMove(CONST CGameUi& Question) CONST
{
	CHAR Buffer1[50], Buffer2[50], Buffer3[50], Buffer4[50] = { 0 };

	DWORD dwBase = ReadDWORD(Question.GetNodeBase() + C_dati_yiji);
	if (ReadDWORD(dwBase + 0x264) == 0)
		return FALSE;

	dwBase = ReadDWORD(dwBase + C_dati_erji);

	MyTools::CCharacter::strcpy_my(Buffer1, reinterpret_cast<CHAR*>(ReadDWORD(dwBase + 0 * 0x98 + 0x8)));
	MyTools::CCharacter::strcpy_my(Buffer2, reinterpret_cast<CHAR*>(ReadDWORD(dwBase + 1 * 0x98 + 0x8)));
	MyTools::CCharacter::strcpy_my(Buffer3, reinterpret_cast<CHAR*>(ReadDWORD(dwBase + 2 * 0x98 + 0x8)));
	MyTools::CCharacter::strcpy_my(Buffer4, reinterpret_cast<CHAR*>(ReadDWORD(dwBase + 3 * 0x98 + 0x8)));

	DWORD dwAnswer = 0xFF;
	if (strlen(Buffer1) > 0)
	{
		if (strstr(Buffer1, Buffer2) == NULL)
			dwAnswer = strstr(Buffer2, Buffer3) == NULL ? 1 : 0;
		else if (strstr(Buffer1, Buffer3) == NULL)
			dwAnswer = 2;
		else
			dwAnswer = 3;

		if (dwAnswer != 0xFF)
		{
			LOG_CF_D(L"答题, 答案是第%d个", dwAnswer + 1);
			MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([dwAnswer, Question]
			{
				MyTools::InvokeClassPtr<CGameCALL>()->AnswerQuestion(dwAnswer, Question.GetNodeBase());
			});
			GameSleep(1000);
		}
	}

	return TRUE;
}

BOOL CPlayerMove::MoveToGeneralStore(_Out_ std::wstring& wsNpcName) CONST
{
	std::wstring wsCurrentMapName = MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName();
	if (!MyTools::InvokeClassPtr<CMapSearch>()->Exist(wsCurrentMapName))
	{
		if (TransferToCity(L"应天府") || TransferToCity(L"星秀村") || TransferToCity(L"汴京城"))
		{
			LOG_CF_D(L"由于处于非大地图的范围……所以先使用回城符!");
			return MoveToGeneralStore(wsNpcName);
		}
		LOG_MSG_CF(L"当前地图:[%s] 不可识别,并且身上不存在回城符……先跑到大地图再开始好吗?");
		StopGame;
		return FALSE;
	}

	std::wstring wsCityName = GetRecentlyCityName();
	if (TransferToCity(wsCityName))
	{
		if(wsCityName == L"星秀村")
			wsNpcName = L"【星】杂货摊老板";
		else if (wsCityName == L"应天府")
			wsNpcName = L"【应】杂货摊老板";
		else
			wsNpcName = L"【汴】杂货摊老板";
	}
	else
	{
		LOG_CF_D(L"不存在城市[%s]的传送符, 尝试传送去别的城市!");
		if (TransferToCity(L"星秀村"))
			wsNpcName = L"【星】杂货摊老板";
		else if (TransferToCity(L"应天府"))
			wsNpcName = L"【应】杂货摊老板";
		else if (TransferToCity(L"汴京城"))
			wsNpcName = L"【汴】杂货摊老板";
		else
		{
			LOG_CF_D(L"不存在任何的城市传送符!");
			return FALSE;
		}
	}
	

	return MoveToGeneralStoreNpc(wsCityName, wsNpcName);
}

VOID CPlayerMove::Move(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST
{
	CONST auto CurPoint = MyTools::InvokeClassPtr<CPersonAttribute>()->GetPoint();
	
	if (MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName() == wsMapName)
	{
		LOG_CF_D(L"MoveToPoint, Tar=[%d,%d], Now=[%d,%d], dis=%.2f", TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, MyTools::InvokeClassPtr<CPersonAttribute>()->GetDis(TarPoint));
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([TarPoint]
		{
			MyTools::InvokeClassPtr<CGameCALL>()->MoveToPoint(TarPoint);
		});
	}
	else
	{
		LOG_CF_D(L"MoveToPoint, Tar=[%s,%d,%d], Now=[%d,%d], dis=%.2f", wsMapName.c_str(), TarPoint.X, TarPoint.Y, CurPoint.X, CurPoint.Y, MyTools::InvokeClassPtr<CPersonAttribute>()->GetDis(TarPoint));
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([wsMapName, TarPoint]
		{
			MyTools::InvokeClassPtr<CGameCALL>()->MoveToPoint(MyTools::CCharacter::UnicodeToASCII(wsMapName).c_str(), TarPoint);
		});
	}
}

BOOL CPlayerMove::MoveToGeneralStoreNpc(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsNpcName) CONST
{
	CResText::ResNpcMapPointText ResNpc;
	if (!MyTools::InvokeClassPtr<CResNpcExtend>()->GetResNpc_By_MapName_NpcName(wsMapName, wsNpcName, ResNpc))
	{
		StopGame;
		LOG_MSG_CF(L"当前资源文件竟然不存在Npc[%s]", wsNpcName.c_str());
		return FALSE;
	}
	
	LOG_CF_D(L"走到杂货摊Npc[%s]旁边", wsNpcName.c_str());
	if (!MoveToPoint(ResNpc.FixPoint))
	{
		LOG_CF_E(L"走到%s失败!", wsNpcName.c_str());
		return FALSE;
	}

	return TRUE;
}

