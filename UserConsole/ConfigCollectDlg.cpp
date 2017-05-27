// ConfigCollectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "ConfigCollectDlg.h"
#include "afxdialogex.h"
#include <string.h>
#include <algorithm>
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
#include "ConsoleClient.h"
#include "ConfigReadPlayerDlg.h"

CONST std::vector<std::wstring> CConfigCollectDlg::_VecConfigName =
{
	L"UseExorcism", L"BuyExorcism", L"BuyPetDrink",L"PetDrinkCount"
	L"CollectMap",L"CollectResItem",L"CollectRedBean",L"FurnitureIndex"
};

IMPLEMENT_DYNAMIC(CConfigCollectDlg, CDialogEx)

CConfigCollectDlg::CConfigCollectDlg(_In_ CONST std::wstring& wsPlayerName,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONFIG_COLLECT, pParent), _wsPlayerName(wsPlayerName)
{
	_emButtonConfigStatus = em_Button_Config_Status::em_Button_Config_Status_Update;
}

CConfigCollectDlg::~CConfigCollectDlg()
{
}

void CConfigCollectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CConfigCollectDlg::OnInitDialog()
{
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_COLLECT_PETDRINK_COUNT))->EnableWindow(FALSE);

	CComboBox* pComboBox = reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_COLLECT_MAP));
	if (pComboBox->GetCount() == 0)
	{
		CONST std::vector<std::wstring> Vec = 
		{
			L"芒砀山麓",L"黄泥岗",L"应天府东郊",L"应天府西郊",L"家园",
		};

		std::for_each(Vec.begin(), Vec.end(), [&pComboBox](CONST auto& itm) { pComboBox->InsertString(pComboBox->GetCount(), itm.c_str()); });
	}

	_VecCtrl.push_back(GetDlgItem(IDC_CHECK_USEEXORCISM));
	_VecCtrl.push_back(GetDlgItem(IDC_CHECK_AUTOBUY_EXORCISM));
	_VecCtrl.push_back(GetDlgItem(IDC_CHECK_AUTOBUY_PETDRINK));
	_VecCtrl.push_back(GetDlgItem(IDC_EDIT_COLLECT_PETDRINK_COUNT));
	_VecCtrl.push_back(GetDlgItem(IDC_COMBO_COLLECT_MAP));
	_VecCtrl.push_back(GetDlgItem(IDC_CHECK_COLLECT_ITEMNAME_RES));
	_VecCtrl.push_back(GetDlgItem(IDC_CHECK_COLLECT_ITEMNAME_REDBEAD));
	_VecCtrl.push_back(GetDlgItem(IDC_CHECK_COLLECT_DEFAULTCONFIG));

	SwapButtonFieldSumit();
	SetEnableFurniture(FALSE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CConfigCollectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_AUTOBUY_PETDRINK, &CConfigCollectDlg::OnBnClickedCheckAutobuyPetdrink)
	ON_CBN_SELCHANGE(IDC_COMBO_COLLECT_MAP, &CConfigCollectDlg::OnCbnSelchangeComboCollectMap)
	ON_BN_CLICKED(IDC_BUTTON_COLLECT_SUMIT, &CConfigCollectDlg::OnBnClickedButtonCollectSumit)
	ON_BN_CLICKED(IDC_CHECK_COLLECT_DEFAULTCONFIG, &CConfigCollectDlg::OnBnClickedCheckCollectDefaultconfig)
	ON_BN_CLICKED(IDC_BUTTON_COLLECT_READCONFIG_OTHERPLAYER, &CConfigCollectDlg::OnBnClickedButtonCollectReadconfigOtherplayer)
END_MESSAGE_MAP()



void CConfigCollectDlg::OnBnClickedCheckAutobuyPetdrink()
{
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_COLLECT_PETDRINK_COUNT))->EnableWindow(reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_AUTOBUY_PETDRINK))->GetCheck() == 1 ? TRUE : FALSE);
}

VOID CConfigCollectDlg::SetEnableFurniture(_In_ BOOL bEnable)
{
	CONST static std::vector<UINT> VecId = 
	{
		IDC_RADIO_COLLECT_FURNITURE1,IDC_RADIO_COLLECT_FURNITURE2,
		IDC_RADIO_COLLECT_FURNITURE3,IDC_RADIO_COLLECT_FURNITURE4,
	};

	std::for_each(VecId.begin(), VecId.end(), [this, bEnable](UINT uId) { reinterpret_cast<CButton *>(GetDlgItem(uId))->EnableWindow(bEnable); });
}


void CConfigCollectDlg::OnCbnSelchangeComboCollectMap()
{
	CComboBox* pComboBox = reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_COLLECT_MAP));

	auto Cursel = pComboBox->GetCurSel();
	if (Cursel == -1)
		return;

	CStringW strText;
	pComboBox->GetLBText(Cursel, strText);
	if (strText == L"家园")
		SetEnableFurniture(TRUE);
	else
		SetEnableFurniture(FALSE);
}


void CConfigCollectDlg::OnBnClickedButtonCollectSumit()
{
	if (GetValue_CollectMap() == -1)
	{
		AfxMessageBox(L"必须选择一个采集地图!");
		return;
	}
	_emButtonConfigStatus = _emButtonConfigStatus == em_Button_Config_Status::em_Button_Config_Status_Lock ? em_Button_Config_Status::em_Button_Config_Status_Update : em_Button_Config_Status::em_Button_Config_Status_Lock;
	SwapButtonFieldSumit();

	if (_emButtonConfigStatus == em_Button_Config_Status::em_Button_Config_Status_Lock)
		AsyncSaveConfig();
}

VOID CConfigCollectDlg::SwapButtonFieldSumit()
{
	CButton* pButton = reinterpret_cast<CButton *>(this->GetDlgItem(IDC_BUTTON_COLLECT_SUMIT));
	switch (_emButtonConfigStatus)
	{
	case em_Button_Config_Status_Lock:
		pButton->SetWindowTextW(L"修改使用该配置");
		std::for_each(_VecCtrl.begin(), _VecCtrl.end(), [](CWnd* pWnd) { pWnd->EnableWindow(FALSE); });
		SetEnableFurniture(FALSE);
		break;
	case em_Button_Config_Status_Update:
		pButton->SetWindowTextW(L"确认使用该配置");
		std::for_each(_VecCtrl.begin(), _VecCtrl.end(), [](CWnd* pWnd) { pWnd->EnableWindow(TRUE); });
		break;
	default:
		break;
	}
}


VOID CConfigCollectDlg::AsyncSaveConfig()
{
	std::vector<ConfigText> Vec;

	AddCompVecConfig(GetValue_UseExcorism(), L"UseExorcism", Vec);
	AddCompVecConfig(GetValue_BuyExcorism(), L"BuyExorcism", Vec);
	AddCompVecConfig(GetValue_BuyPetDrink(), L"BuyPetDrink", Vec);
	AddCompVecConfig(GetValue_PetDrinkCount(), L"PetDrinkCount", Vec);
	AddCompVecConfig(GetValue_CollectMap(), L"CollectMap", Vec);
	AddCompVecConfig(GetValue_CollectResItem(), L"CollectResItem", Vec);
	AddCompVecConfig(GetValue_CollectReadBean(), L"CollectRedBean", Vec);
	AddCompVecConfig(GetValue_FurnitureIndex(), L"FurnitureIndex", Vec);

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_WriteConfig);

	SocketBuffer << _wsPlayerName << L"Collect" << Vec.size();
	for (CONST auto& itm : Vec)
		SocketBuffer << itm.wsConfigName << itm.wsConfigValue;

	CConsoleClient::GetInstance().AsyncSend(SocketBuffer);
}

VOID CConfigCollectDlg::SyncReadConfig()
{
	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg_ReadConfig);
	SocketBuffer << _wsPlayerName << L"Collect" << _VecConfigName.size();

	for (CONST auto& itm : _VecConfigName)
		SocketBuffer << itm;

	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [this](MyTools::CLSocketBuffer& SocketBuffer)
	{
		DWORD dwSize = 0;
		SocketBuffer >> dwSize;

		for (decltype(dwSize) i = 0; i < dwSize; ++i)
		{
			ConfigText ConfigText_;
			SocketBuffer >> ConfigText_.wsConfigName >> ConfigText_.wsConfigValue;
			_VecConfig.push_back(std::move(ConfigText_));
		}
	});
}

VOID CConfigCollectDlg::SyncServerConfigToLocal()
{
	int nValue = 0;
	std::wstring wsFormatText;

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"UseExorcism");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_USEEXORCISM))->SetCheck(nValue == 1);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"BuyExorcism");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_AUTOBUY_EXORCISM))->SetCheck(nValue == 1);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"BuyPetDrink");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_AUTOBUY_PETDRINK))->SetCheck(nValue == 1);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"PetDrinkCount");
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_COLLECT_PETDRINK_COUNT))->SetWindowTextW(MyTools::CCharacter::FormatText(wsFormatText, L"%d", nValue == -1 ? 0 : nValue).c_str());

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"CollectMap");
	reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_COLLECT_MAP))->SetCurSel(nValue);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"CollectResItem");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_COLLECT_ITEMNAME_RES))->SetCheck(nValue == 1);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"CollectRedBean");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_COLLECT_ITEMNAME_REDBEAD))->SetCheck(nValue == 1);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"FurnitureIndex");
	switch (nValue)
	{
	case 0:
		reinterpret_cast<CButton *>(GetDlgItem(IDC_RADIO_COLLECT_FURNITURE1))->SetCheck(TRUE);
		break;
	case 1:
		reinterpret_cast<CButton *>(GetDlgItem(IDC_RADIO_COLLECT_FURNITURE2))->SetCheck(TRUE);
		break;
	case 2:
		reinterpret_cast<CButton *>(GetDlgItem(IDC_RADIO_COLLECT_FURNITURE3))->SetCheck(TRUE);
		break;
	case 3:
		reinterpret_cast<CButton *>(GetDlgItem(IDC_RADIO_COLLECT_FURNITURE4))->SetCheck(TRUE);
		break;
	default:
		break;
	}
}

int CConfigCollectDlg::FindConfigValue_By_ConfigName_In_VecConfig(_In_ CONST std::wstring& wsConfigName) CONST
{
	auto pElement = MyTools::CLPublic::Vec_find_if_Const(_VecConfig, [wsConfigName](CONST auto& itm) { return itm.wsConfigName == wsConfigName; });
	if (pElement == nullptr)
		return -1;

	return pElement->wsConfigValue == L"Empty" ? -1 : _wtoi(pElement->wsConfigValue.c_str());
}

VOID CConfigCollectDlg::AddCompVecConfig(_In_ int nValue, _In_ CONST std::wstring& wsConfigName, _Out_ std::vector<ConfigText>& Vec) CONST
{
	if (nValue != FindConfigValue_By_ConfigName_In_VecConfig(wsConfigName))
	{
		ConfigText ConfigText_;
		ConfigText_.wsConfigName = wsConfigName;
		MyTools::CCharacter::FormatText(ConfigText_.wsConfigValue, L"%d", nValue);
		Vec.push_back(std::move(ConfigText_));
	}
}

BOOL CConfigCollectDlg::GetValue_UseExcorism() CONST
{
	return reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_USEEXORCISM))->GetCheck() == 1;
}

BOOL CConfigCollectDlg::GetValue_BuyExcorism() CONST
{
	return reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_AUTOBUY_EXORCISM))->GetCheck() == 1;
}

BOOL CConfigCollectDlg::GetValue_BuyPetDrink() CONST
{
	return reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_AUTOBUY_PETDRINK))->GetCheck() == 1;
}

int CConfigCollectDlg::GetValue_PetDrinkCount() CONST
{
	CStringW strValue;
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_COLLECT_PETDRINK_COUNT))->GetWindowTextW(strValue);

	return _wtoi(strValue.GetBuffer());
}

int CConfigCollectDlg::GetValue_CollectMap() CONST
{
	return reinterpret_cast<CComboBox *>(GetDlgItem(IDC_COMBO_COLLECT_MAP))->GetCurSel();
}

BOOL CConfigCollectDlg::GetValue_CollectResItem() CONST
{
	return reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_COLLECT_ITEMNAME_RES))->GetCheck() == 1;
}

BOOL CConfigCollectDlg::GetValue_CollectReadBean() CONST
{
	return reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_COLLECT_ITEMNAME_REDBEAD))->GetCheck() == 1;
}

int CConfigCollectDlg::GetValue_FurnitureIndex() CONST
{
	if (reinterpret_cast<CButton *>(GetDlgItem(IDC_RADIO_COLLECT_FURNITURE1))->GetCheck() == 1)
		return 0;
	else if (reinterpret_cast<CButton *>(GetDlgItem(IDC_RADIO_COLLECT_FURNITURE2))->GetCheck() == 1)
		return 1;
	else if (reinterpret_cast<CButton *>(GetDlgItem(IDC_RADIO_COLLECT_FURNITURE3))->GetCheck() == 2)
		return 2;
	else if (reinterpret_cast<CButton *>(GetDlgItem(IDC_RADIO_COLLECT_FURNITURE3))->GetCheck() == 3)
		return 4;
	return -1;
}

void CConfigCollectDlg::OnBnClickedCheckCollectDefaultconfig()
{
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_USEEXORCISM))->SetCheck(TRUE);
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_AUTOBUY_EXORCISM))->SetCheck(TRUE);
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_AUTOBUY_PETDRINK))->SetCheck(TRUE);
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_COLLECT_PETDRINK_COUNT))->SetWindowTextW(L"20");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_COLLECT_ITEMNAME_RES))->SetCheck(TRUE);
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_COLLECT_ITEMNAME_REDBEAD))->SetCheck(FALSE);
}


void CConfigCollectDlg::OnBnClickedButtonCollectReadconfigOtherplayer()
{
	CConfigReadPlayerDlg dlg(_VecConfigName, L"Collect");
	dlg.DoModal();

	if (dlg.GetVecConfig().size() > 0)
	{
		_VecConfig.clear();
		_VecConfig = dlg.GetVecConfig();
	}
}
