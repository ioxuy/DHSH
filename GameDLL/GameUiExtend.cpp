#include "stdafx.h"
#include "GameUiExtend.h"
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include "GameUi.h"
#include "ExcuteAction.h"
#include "GameCALL.h"

#define _SELF L"GameUiExtend.cpp"
UINT CGameUiExtend::GetVecGameUi(_Out_ std::vector<CGameUi>& Vec) CONST
{
	return TraverseGameUi(Vec, nullptr);
}

BOOL CGameUiExtend::FindGameUi_By_Name(_In_ CONST std::wstring& wsUiName, _Out_ CGameUi& GameUi) CONST
{
	std::vector<CGameUi> Vec;
	if (TraverseGameUi(Vec, [wsUiName](CONST CGameUi& GameUi_) { return GameUi_.GetName() == wsUiName; }) > 0)
	{
		GameUi = std::move(Vec.at(0));
		return TRUE;
	}

	return FALSE;
}

BOOL CGameUiExtend::IsShowNpcDlg() CONST
{
	return IsShowDlg(L"npcdlg");
}

VOID CGameUiExtend::CloseNpcDlg() CONST
{
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([this] {MyTools::InvokeClassPtr<CGameCALL>()->CloseNpcDlg(); });
}

BOOL CGameUiExtend::FindText_In_NpcDlg(_In_ CONST std::wstring& wsText) CONST
{
	DWORD dwBase = ReadDWORD(ReadDWORD(C_NPC_state) + C_NPC_neirong);
	if (dwBase == NULL)
		return FALSE;

	std::wstring wsNpcDlgText = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CHAR *>(dwBase)));
	return wsNpcDlgText.find(wsText) != -1 ? TRUE : FALSE;
}

BOOL CGameUiExtend::IsShowDlg(_In_ CONST std::wstring& wsDlgName) CONST
{
	CGameUi GameUi;
	return !FindGameUi_By_Name(wsDlgName, GameUi) ? FALSE : GameUi.IsShow();
}

BOOL CGameUiExtend::CloseDlg(_In_ CONST std::wstring& wsDlgName) CONST
{
	MyTools::CTimeTick TimeTick;
	CGameUi dlg;
	while (GameRun && FindGameUi_By_Name(wsDlgName, dlg) && dlg.IsShow())
	{
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) > 10)
		{
			LOG_CF_E(L"πÿ±’dlg[%s] ß∞‹ ≥¨ ±10√Î", wsDlgName.c_str());
			return FALSE;
		}

		LOG_CF_D(L"πÿ±’dlg[%s]", wsDlgName.c_str());
		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([dlg]
		{
			MyTools::InvokeClassPtr<CGameCALL>()->CloseGameUi(dlg.GetNodeBase());
		});
		GameSleep(1000);
	}

	return TRUE;
}

VOID CGameUiExtend::PrintGameUi() CONST
{
	std::vector<CGameUi> Vec;
	TraverseGameUi(Vec, nullptr);
	for (CONST auto& itm : Vec)
	{
		LOG_C_D(L"Ui.Name=[%s],Show=%X,dwAddr=%X", itm.GetName().c_str(), itm.IsShow(), itm.GetNodeBase());
		itm.PrintOpionText();
	}
}

BOOL CGameUiExtend::Action_By_DlgName_When_ShowDlg(_In_ CONST std::wstring& wsDlgName, std::function<VOID(CONST CGameUi&)> ActionPtr) CONST
{
	CGameUi GameUi;
	if (FindGameUi_By_Name(wsDlgName, GameUi) && GameUi.IsShow())
	{
		if (ActionPtr != nullptr)
			ActionPtr(GameUi);

		return TRUE;
	}
	return FALSE;
}

UINT CGameUiExtend::TraverseGameUi(_Out_ std::vector<CGameUi>& Vec, _In_ std::function<BOOL(CONST CGameUi&)> fnExprPtr) CONST
{
	DWORD dwArrayHead = ReadDWORD(ReadDWORD(C_window_base) + C_window_offset);
	DWORD dwArrayEnd = ReadDWORD(ReadDWORD(C_window_base) + C_window_offset + 0x4);

	int nCount = (dwArrayEnd - dwArrayHead) / 4;
	for (int i = 0;i < nCount; ++i)
	{
		DWORD dwAddr = ReadDWORD(dwArrayHead + i * 4);
		CGameUi GameUi(dwAddr);
		if (fnExprPtr == nullptr)
		{
			Vec.push_back(std::move(GameUi));
			continue;
		}
		else if (fnExprPtr(GameUi))
		{
			Vec.push_back(std::move(GameUi));
			break;
		}
	}
	return Vec.size();
}
