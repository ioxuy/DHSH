// ConfigFielddlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "ConfigFielddlg.h"
#include "afxdialogex.h"
#include <vector>
#include <string.h>
#include <algorithm>
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include "ConsoleClient.h"
#include "ConfigReadPlayerDlg.h"

// CConfigFielddlg dialog

CONST std::vector<std::wstring> CConfigFielddlg::_VecConfigName =
{
	L"PersonHP<", L"PersonMP<", L"PetHP<",L"PetMP<",
	L"PersonFightMode",L"PetFightMode",L"AutoBuyHappyBell"
};

IMPLEMENT_DYNAMIC(CConfigFielddlg, CDialogEx)

CConfigFielddlg::CConfigFielddlg(_In_ CONST std::wstring& wsPlayerName, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONFIG_FIELD, pParent), _wsPlayerName(wsPlayerName)
{
	_emButtonConfigStatus = em_Button_Config_Status::em_Button_Config_Status_Update;
}

CConfigFielddlg::~CConfigFielddlg()
{
}

void CConfigFielddlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CConfigFielddlg::OnInitDialog()
{
	CComboBox* pPersonFightMode = reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_FIELD_PERSON_FIGHTMODE));
	CComboBox* pPetFightMode = reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_FIELD_PET_FIGHTMODE));


	if (pPersonFightMode->GetCount() == 0)
	{
		CONST static std::vector<std::wstring> Vec = { L"固定使用F1",L"固定普通攻击",L"先使用F2后固定用F1" };
		std::for_each(Vec.begin(), Vec.end(), [&pPersonFightMode](CONST auto& itm) { pPersonFightMode->InsertString(pPersonFightMode->GetCount(), itm.c_str()); });
	}
	if (pPetFightMode->GetCount() == 0)
	{
		CONST static std::vector<std::wstring> Vec = { L"防御",L"固定用F3" };
		std::for_each(Vec.begin(), Vec.end(), [&pPetFightMode](CONST auto& itm) { pPetFightMode->InsertString(pPetFightMode->GetCount(), itm.c_str()); });
	}

	_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_EDIT_FIELD_PERSON_HP));
	_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_EDIT_FIELD_PERSON_MP));
	_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_EDIT_FIELD_PET_HP));
	_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_EDIT_FIELD_PET_MP));

	_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_COMBO_FIELD_PERSON_FIGHTMODE));
	_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_COMBO_FIELD_PET_FIGHTMODE));
	//_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_CHECK_USEEXORCISM));
	//_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_CHECK_AUTOBUY_EXORCISM));
	_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_CHECK_AUTOBUY_HAPPYBELL));
	_VecFieldConfigCtrl.push_back(GetDlgItem(IDC_CHECK_FIELD_DEFAULTCONFIG));

	SwapButtonFieldSumit();
	SyncReadConfig();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CConfigFielddlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_SUMIT, &CConfigFielddlg::OnBnClickedButtonFieldSumit)
	ON_BN_CLICKED(IDC_CHECK_FIELD_DEFAULTCONFIG, &CConfigFielddlg::OnBnClickedCheckFieldDefaultconfig)
	ON_BN_CLICKED(IDC_BUTTON_FIELD_READCONFIG_OTHERPLAYER, &CConfigFielddlg::OnBnClickedButtonFieldReadconfigOtherplayer)
END_MESSAGE_MAP()


// CConfigFielddlg message handlers


void CConfigFielddlg::OnBnClickedButtonFieldSumit()
{
	if (GetPerFightMode() == em_PetFightMode::em_PetFightMode_None)
	{
		AfxMessageBox(L"必须要选择宠物的战斗模式!");
		return;
	}
	else if (GetPersonFightMode() == em_PersonFightMode::em_PersonFightMode_None)
	{
		AfxMessageBox(L"必须要选择人物的战斗模式!");
		return;
	}

	_emButtonConfigStatus = _emButtonConfigStatus == em_Button_Config_Status::em_Button_Config_Status_Lock ? em_Button_Config_Status::em_Button_Config_Status_Update : em_Button_Config_Status::em_Button_Config_Status_Lock;
	SwapButtonFieldSumit();

	if (_emButtonConfigStatus == em_Button_Config_Status::em_Button_Config_Status_Lock)
		AsyncSaveConfig();
}

VOID CConfigFielddlg::SwapButtonFieldSumit()
{
	CButton* pButton = reinterpret_cast<CButton *>(this->GetDlgItem(IDC_BUTTON_FIELD_SUMIT));
	switch (_emButtonConfigStatus)
	{
	case em_Button_Config_Status_Lock:
		pButton->SetWindowTextW(L"修改使用该配置");
		std::for_each(_VecFieldConfigCtrl.begin(), _VecFieldConfigCtrl.end(), [](CWnd* pWnd) { pWnd->EnableWindow(FALSE); });
		break;
	case em_Button_Config_Status_Update:
		pButton->SetWindowTextW(L"确认使用该配置");
		std::for_each(_VecFieldConfigCtrl.begin(), _VecFieldConfigCtrl.end(), [](CWnd* pWnd) { pWnd->EnableWindow(TRUE); });
		break;
	default:
		break;
	}
}


VOID CConfigFielddlg::AsyncSaveConfig()
{
	std::vector<ConfigText> Vec;

	AddCompVecConfig(GetValue_PersonHp(), L"PersonHP<", Vec);
	AddCompVecConfig(GetValue_PersonMp(), L"PersonMP<", Vec);
	AddCompVecConfig(GetValue_PetHp(), L"PetHP<", Vec);
	AddCompVecConfig(GetValue_PetMp(), L"PetMP<", Vec);
	AddCompVecConfig(GetPersonFightMode(), L"PersonFightMode", Vec);
	AddCompVecConfig(GetPerFightMode(), L"PetFightMode", Vec);
	AddCompVecConfig(GetValue_AutoBuyHappyBell(), L"AutoBuyHappyBell", Vec);

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_WriteConfig);

	SocketBuffer << _wsPlayerName << L"Field" << Vec.size();
	for (CONST auto& itm : Vec)
		SocketBuffer << itm.wsConfigName << itm.wsConfigValue;

	CConsoleClient::GetInstance().AsyncSend(SocketBuffer);
}

void CConfigFielddlg::OnBnClickedCheckFieldDefaultconfig()
{
	CButton* pButton = reinterpret_cast<CButton *>(this->GetDlgItem(IDC_CHECK_FIELD_DEFAULTCONFIG));
	
	if (pButton->GetCheck() == TRUE)
	{
		reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_FIELD_PERSON_HP))->SetWindowTextW(L"70");
		reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_FIELD_PERSON_MP))->SetWindowTextW(L"70");
		reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_FIELD_PET_HP))->SetWindowTextW(L"70");
		reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_FIELD_PET_MP))->SetWindowTextW(L"70");

		reinterpret_cast<CComboBox*>(GetDlgItem(IDC_COMBO_FIELD_PERSON_FIGHTMODE))->SetCurSel(0);
		reinterpret_cast<CComboBox*>(GetDlgItem(IDC_COMBO_FIELD_PET_FIGHTMODE))->SetCurSel(0);
		reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_AUTOBUY_HAPPYBELL))->SetCheck(TRUE);
	}
}

DWORD CConfigFielddlg::GetValue_PersonHp() CONST
{
	CStringW strText;
	CEdit* pPersonHpEdit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_FIELD_PERSON_HP));
	pPersonHpEdit->GetWindowTextW(strText);
	return _wtoi(strText.GetBuffer());
}

DWORD CConfigFielddlg::GetValue_PersonMp() CONST
{
	CStringW strText;
	CEdit* pPersonMpEdit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_FIELD_PERSON_MP));
	pPersonMpEdit->GetWindowTextW(strText);
	return _wtoi(strText.GetBuffer());
}

DWORD CConfigFielddlg::GetValue_PetHp() CONST
{
	CStringW strText;
	CEdit* pPetHpEdit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_FIELD_PET_HP));
	pPetHpEdit->GetWindowTextW(strText);
	return _wtoi(strText.GetBuffer());
}

DWORD CConfigFielddlg::GetValue_PetMp() CONST
{
	CStringW strText;
	CEdit* pPetMpEdit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_FIELD_PET_MP));
	pPetMpEdit->GetWindowTextW(strText);
	return _wtoi(strText.GetBuffer());
}

int CConfigFielddlg::GetPersonFightMode() CONST
{
	CComboBox* pPersonFightMode = reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_FIELD_PERSON_FIGHTMODE));
	return pPersonFightMode->GetCurSel();
}

int CConfigFielddlg::GetPerFightMode() CONST
{
	CComboBox* pPetFightMode = reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_FIELD_PET_FIGHTMODE));
	return pPetFightMode->GetCurSel();
}

BOOL CConfigFielddlg::GetValue_AutoBuyHappyBell() CONST
{
	return reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_AUTOBUY_HAPPYBELL))->GetCheck() == 1;
}

VOID CConfigFielddlg::SyncReadConfig()
{
	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg_ReadConfig);
	SocketBuffer << _wsPlayerName << L"Field" << _VecConfigName.size();

	for (CONST auto& itm : _VecConfigName)
		SocketBuffer << itm;

	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [this](MyTools::CLSocketBuffer& SocketBuffer) 
	{
		DWORD dwSize = 0;
		SocketBuffer >> dwSize;

		for (decltype(dwSize) i = 0;i < dwSize; ++i)
		{
			ConfigText ConfigText_;
			SocketBuffer >> ConfigText_.wsConfigName >> ConfigText_.wsConfigValue;
			_VecConfigText.push_back(std::move(ConfigText_));
		}
	});
}

VOID CConfigFielddlg::SyncServerConfigToLocal()
{
	int nValue = 0;
	std::wstring wsFormatText;

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"PersonHP<");
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_FIELD_PERSON_HP))->SetWindowTextW(MyTools::CCharacter::FormatText(wsFormatText, L"%d", nValue == -1 ? 0 : nValue).c_str());

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"PersonMP<");
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_FIELD_PERSON_MP))->SetWindowTextW(MyTools::CCharacter::FormatText(wsFormatText, L"%d", nValue == -1 ? 0 : nValue).c_str());

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"PetHP<");
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_FIELD_PET_HP))->SetWindowTextW(MyTools::CCharacter::FormatText(wsFormatText, L"%d", nValue == -1 ? 0 : nValue).c_str());

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"PetMP<");
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_FIELD_PET_MP))->SetWindowTextW(MyTools::CCharacter::FormatText(wsFormatText, L"%d", nValue == -1 ? 0 : nValue).c_str());

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"PersonFightMode");
	reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_FIELD_PERSON_FIGHTMODE))->SetCurSel(nValue);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"PetFightMode");
	reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_FIELD_PET_FIGHTMODE))->SetCurSel(nValue);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"AutoBuyHappyBell");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_AUTOBUY_HAPPYBELL))->SetCheck(nValue == 1);
}

int CConfigFielddlg::FindConfigValue_By_ConfigName_In_VecConfig(_In_ CONST std::wstring& wsConfigName) CONST
{
	auto pElement = MyTools::CLPublic::Vec_find_if_Const(_VecConfigText, [wsConfigName](CONST auto& itm) { return itm.wsConfigName == wsConfigName; });
	if (pElement == nullptr)
		return -1;

	return pElement->wsConfigValue == L"Empty" ? -1 : _wtoi(pElement->wsConfigValue.c_str());
}

VOID CConfigFielddlg::AddCompVecConfig(_In_ int nValue, _In_ CONST std::wstring& wsConfigName, _Out_ std::vector<ConfigText>& Vec) CONST
{
	if (nValue != FindConfigValue_By_ConfigName_In_VecConfig(wsConfigName))
	{
		ConfigText ConfigText_;
		ConfigText_.wsConfigName = wsConfigName;
		MyTools::CCharacter::FormatText(ConfigText_.wsConfigValue, L"%d", nValue);
		Vec.push_back(std::move(ConfigText_));
	}
}


void CConfigFielddlg::OnBnClickedButtonFieldReadconfigOtherplayer()
{
	CConfigReadPlayerDlg dlg(_VecConfigName, L"Field");
	dlg.DoModal();

	if (dlg.GetVecConfig().size() > 0)
	{
		_VecConfigText.clear();
		_VecConfigText = dlg.GetVecConfig();
	}
}
