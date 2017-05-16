#include "stdafx.h"
#include "GameUi.h"
#include <MyTools/Character.h>
#include <MyTools/Log.h>

#define _SELF L"GameUi.cpp"
CGameUi::CGameUi()
{

}

CGameUi::CGameUi(_In_ DWORD dwNodeBase)
{
	_dwNodeBase = dwNodeBase;
	SetName();
}

VOID CGameUi::SetName()
{
	_wsName = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CHAR*>(GetNodeBase() + 0x60)));
}

BOOL CGameUi::IsShow() CONST
{
	return (ReadDWORD(GetNodeBase() + 0x16C) & 0xFF) > 0;
}

BOOL CGameUi::FindNpcdlgOption_By_PartText_Excute_ActionPtr(_In_ CONST std::wstring& wsPartText, _In_ std::function<VOID(CONST DlgOpionText&)> ActionPtr) CONST
{
	std::vector<DlgOpionText> Vec;
	if (GetVecOptionText(Vec, [wsPartText](CONST std::wstring& wsFullText) { return wsFullText.find(wsPartText) != -1; }) == 0)
		return FALSE;

	if (ActionPtr != nullptr)
		ActionPtr(Vec.at(0));
	return TRUE;
}

VOID CGameUi::PrintOpionText() CONST
{
	if (!IsShow())
		return;

	std::vector<DlgOpionText> Vec;
	GetVecOptionText(Vec, nullptr);

	LOG_C_D(L"dlg=[%s]",GetName().c_str());
	for (CONST auto& itm : Vec)
		LOG_C_D(L"NodeBase=%X,OptionText=[%s],ClickText=[%s]", itm.dwNodeBase, itm.wsOptionText.c_str(), itm.wsClickText.c_str());
}

UINT CGameUi::GetVecOptionText(_Out_ std::vector<DlgOpionText>& Vec, _In_ std::function<BOOL(CONST std::wstring&)> FilterPtr) CONST
{
	DWORD dwHead = ReadDWORD(GetNodeBase() + 0x14);
	DWORD dwEnd = ReadDWORD(GetNodeBase() + 0x18);

	int nCount = static_cast<int>(dwEnd - dwHead) >> 2;
	for (int i = 0; i < nCount; ++i)
	{
		DWORD dwAddr = ReadDWORD(dwHead + i * 4);
		if (ReadDWORD(dwAddr + NPC选项文本偏移) == 0)
			continue;

		DlgOpionText dlgText;
		dlgText.dwNodeBase = dwAddr;
		dlgText.wsOptionText = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CHAR *>(ReadDWORD(dwAddr + NPC选项文本偏移))));
		dlgText.wsClickText = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CHAR *>(ReadDWORD(dwAddr + NPC选项发包偏移))));

		if (FilterPtr == nullptr)
			Vec.push_back(std::move(dlgText));
		else if (FilterPtr(dlgText.wsOptionText))
		{
			Vec.push_back(std::move(dlgText));
			break;
		}
	}
	return Vec.size();
}
