#include "stdafx.h"
#include "GameUiExtend.h"
#include <MyTools/Character.h>
#include "GameUi.h"

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
