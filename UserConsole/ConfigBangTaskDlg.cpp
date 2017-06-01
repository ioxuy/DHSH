// ConfigBangTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "ConfigBangTaskDlg.h"
#include "afxdialogex.h"
#include <MyTools/CLPublic.h>
#include "ConsoleClient.h"
#include "ConfigReadPlayerDlg.h"

// CConfigBangTaskDlg dialog
CONST std::vector<std::wstring> CConfigBangTaskDlg::_VecConfigName =
{
	L"BuyExorcism", L"BuyReturnSymbol",L"UseReturnSymbol"
};

IMPLEMENT_DYNAMIC(CConfigBangTaskDlg, CDialogEx)

CConfigBangTaskDlg::CConfigBangTaskDlg(_In_ CONST std::wstring& wsPlayerName, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONFIG_BANGTASK, pParent),
	_wsPlayerName(wsPlayerName)
{
	_emButtonConfigStatus = em_Button_Config_Status::em_Button_Config_Status_Update;
}

CConfigBangTaskDlg::~CConfigBangTaskDlg()
{
}

void CConfigBangTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CConfigBangTaskDlg::OnInitDialog()
{
	_VecCtrl.push_back(GetDlgItem(IDC_CHECK_BANGTASK_AUTOBUY_RETURNSYMBOL));
	_VecCtrl.push_back(GetDlgItem(IDC_CHECK_BANGTASK_AUTOBUYEXORCISM));
	_VecCtrl.push_back(GetDlgItem(IDC_CHECK_BANGTASK_USE_RETURNSYMBOL));

	SwapButtonFieldSumit();
	SyncReadConfig();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CConfigBangTaskDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BANGTASK_READCONFIG_OTHERPLAYER, &CConfigBangTaskDlg::OnBnClickedButtonBangtaskReadconfigOtherplayer)
	ON_BN_CLICKED(IDC_CHECK_BANGTASK_DEFAULTCONFIG, &CConfigBangTaskDlg::OnBnClickedCheckBangtaskDefaultconfig)
	ON_BN_CLICKED(IDC_BUTTON_BANGTASK_SUMIT, &CConfigBangTaskDlg::OnBnClickedButtonBangtaskSumit)
END_MESSAGE_MAP()


// CConfigBangTaskDlg message handlers


void CConfigBangTaskDlg::OnBnClickedButtonBangtaskReadconfigOtherplayer()
{
	CConfigReadPlayerDlg dlg(_VecConfigName, L"BangTask");
	dlg.DoModal();

	if (dlg.GetVecConfig().size() > 0)
	{
		_VecConfig.clear();
		_VecConfig = dlg.GetVecConfig();
		SyncServerConfigToLocal();
	}
}

VOID CConfigBangTaskDlg::SwapButtonFieldSumit()
{
	CButton* pButton = reinterpret_cast<CButton *>(this->GetDlgItem(IDC_BUTTON_BANGTASK_SUMIT));
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

VOID CConfigBangTaskDlg::SyncReadConfig()
{
	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg_ReadConfig);
	SocketBuffer << _wsPlayerName << L"BangTask" << _VecConfigName.size();

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
	SyncServerConfigToLocal();
}

VOID CConfigBangTaskDlg::SyncServerConfigToLocal()
{
	// L"BuyExorcism", L"BuyReturnSymbol",L"UseReturnSymbol"
	int nValue = 0;
	std::wstring wsFormatText;

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"BuyExorcism");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_AUTOBUYEXORCISM))->SetCheck(nValue == 1);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"BuyReturnSymbol");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_AUTOBUY_RETURNSYMBOL))->SetCheck(nValue == 1);

	nValue = FindConfigValue_By_ConfigName_In_VecConfig(L"UseReturnSymbol");
	reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_USE_RETURNSYMBOL))->SetCheck(nValue == 1);
}

int CConfigBangTaskDlg::FindConfigValue_By_ConfigName_In_VecConfig(_In_ CONST std::wstring& wsConfigName) CONST
{
	auto pElement = MyTools::CLPublic::Vec_find_if_Const(_VecConfig, [wsConfigName](CONST auto& itm) { return itm.wsConfigName == wsConfigName; });
	if (pElement == nullptr)
		return -1;

	return pElement->wsConfigValue == L"Empty" ? -1 : _wtoi(pElement->wsConfigValue.c_str());
}

VOID CConfigBangTaskDlg::AsyncSaveConfig()
{
	std::vector<ConfigText> Vec;

	AddCompVecConfig(GetValue_BuyExcorism(), L"BuyExorcism", Vec);
	AddCompVecConfig(GetValue_BuyReturnSymbol(), L"BuyReturnSymbol", Vec);
	AddCompVecConfig(GetValue_UseReturnSymbol(), L"UseReturnSymbol", Vec);

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_WriteConfig);

	SocketBuffer << _wsPlayerName << L"BangTask" << Vec.size();
	for (CONST auto& itm : Vec)
		SocketBuffer << itm.wsConfigName << itm.wsConfigValue;

	CConsoleClient::GetInstance().AsyncSend(SocketBuffer);
}

VOID CConfigBangTaskDlg::AddCompVecConfig(_In_ int nValue, _In_ CONST std::wstring& wsConfigName, _Out_ std::vector<ConfigText>& Vec) CONST
{
	if (nValue != FindConfigValue_By_ConfigName_In_VecConfig(wsConfigName))
	{
		ConfigText ConfigText_;
		ConfigText_.wsConfigName = wsConfigName;
		MyTools::CCharacter::FormatText(ConfigText_.wsConfigValue, L"%d", nValue);
		Vec.push_back(std::move(ConfigText_));
	}
}

int CConfigBangTaskDlg::GetValue_BuyExcorism()
{
	return reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_AUTOBUYEXORCISM))->GetCheck() == 1;
}

int CConfigBangTaskDlg::GetValue_BuyReturnSymbol()
{
	return reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_AUTOBUY_RETURNSYMBOL))->GetCheck() == 1;
}

int CConfigBangTaskDlg::GetValue_UseReturnSymbol()
{
	return reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_USE_RETURNSYMBOL))->GetCheck() == 1;
}

void CConfigBangTaskDlg::OnBnClickedCheckBangtaskDefaultconfig()
{
	if (reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_DEFAULTCONFIG))->GetCheck() == 1)
	{
		reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_AUTOBUYEXORCISM))->SetCheck(TRUE);
		reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_AUTOBUY_RETURNSYMBOL))->SetCheck(TRUE);
		reinterpret_cast<CButton *>(GetDlgItem(IDC_CHECK_BANGTASK_USE_RETURNSYMBOL))->SetCheck(TRUE);
	}
}


void CConfigBangTaskDlg::OnBnClickedButtonBangtaskSumit()
{
	_emButtonConfigStatus = _emButtonConfigStatus == em_Button_Config_Status::em_Button_Config_Status_Lock ? em_Button_Config_Status::em_Button_Config_Status_Update : em_Button_Config_Status::em_Button_Config_Status_Lock;
	SwapButtonFieldSumit();

	if (_emButtonConfigStatus == em_Button_Config_Status::em_Button_Config_Status_Lock)
		AsyncSaveConfig();
}
