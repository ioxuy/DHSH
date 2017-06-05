#include "stdafx.h"
#include "BaseConfigDialog.h"
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>

CBaseConfigDialog::CBaseConfigDialog(_In_ CDialogEx* pCDialogEx, _In_ UINT uSumitButtonId) : _uSumitButtonId(uSumitButtonId)
{
	_pDialog = pCDialogEx;
	_emButtonConfigStatus = em_Button_Config_Status::em_Button_Config_Status_Update;
}

BOOL CBaseConfigDialog::IsButtonLock() CONST
{
	return _emButtonConfigStatus == em_Button_Config_Status::em_Button_Config_Status_Lock;
}

VOID CBaseConfigDialog::SwapButtonFieldSumit()
{
	CButton* pButton = reinterpret_cast<CButton *>(_pDialog->GetDlgItem(_uSumitButtonId));
	if (pButton == nullptr)
		return;
	switch (_emButtonConfigStatus)
	{
	case em_Button_Config_Status_Lock:
		pButton->SetWindowTextW(L"修改使用该配置");
		std::for_each(_VecCtrl.begin(), _VecCtrl.end(), [](CWnd* pWnd) { pWnd->EnableWindow(FALSE); });
		break;
	case em_Button_Config_Status_Update:
		pButton->SetWindowTextW(L"确认使用该配置");
		std::for_each(_VecCtrl.begin(), _VecCtrl.end(), [](CWnd* pWnd) { pWnd->EnableWindow(TRUE); });
		break;
	default:
		break;
	}
}

int CBaseConfigDialog::FindConfigValue_By_ConfigName_In_VecConfig(_In_ CONST std::wstring& wsConfigName) CONST
{
	auto pElement = MyTools::CLPublic::Vec_find_if_Const(_VecConfig, [wsConfigName](CONST auto& itm) { return itm.wsConfigName == wsConfigName; });
	if (pElement == nullptr)
		return -1;

	return pElement->wsConfigValue == L"Empty" ? -1 : _wtoi(pElement->wsConfigValue.c_str());
}

VOID CBaseConfigDialog::AddCompVecConfig(_In_ int nValue, _In_ CONST std::wstring& wsConfigName, _Out_ std::vector<ConfigText>& Vec) CONST
{
	if (nValue != FindConfigValue_By_ConfigName_In_VecConfig(wsConfigName))
	{
		ConfigText ConfigText_;
		ConfigText_.wsConfigName = wsConfigName;
		MyTools::CCharacter::FormatText(ConfigText_.wsConfigValue, L"%d", nValue);
		Vec.push_back(std::move(ConfigText_));
	}
}
