// ConfigPurifyWaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "ConfigPurifyWaterDlg.h"
#include "afxdialogex.h"
#include <MyTools/CLPublic.h>
#include "ConsoleClient.h"

// CConfigPurifyWaterDlg dialog

IMPLEMENT_DYNAMIC(CConfigPurifyWaterDlg, CDialogEx)

CConfigPurifyWaterDlg::CConfigPurifyWaterDlg(_In_ CONST std::wstring& wsPlayerName, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONFIG_PURIFYWATER, pParent),
	_wsPlayerName(wsPlayerName)
{
	_emButtonConfigStatus = em_Button_Config_Status::em_Button_Config_Status_Update;
	_dwChoiceType = NULL;
}

CConfigPurifyWaterDlg::~CConfigPurifyWaterDlg()
{
}

void CConfigPurifyWaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CConfigPurifyWaterDlg::OnInitDialog()
{
	SyncReadConfig();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CConfigPurifyWaterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PURIFYWATER_SUMIT, &CConfigPurifyWaterDlg::OnBnClickedButtonPurifywaterSumit)
END_MESSAGE_MAP()


// CConfigPurifyWaterDlg message handlers


void CConfigPurifyWaterDlg::OnBnClickedButtonPurifywaterSumit()
{
	_emButtonConfigStatus = _emButtonConfigStatus == em_Button_Config_Status::em_Button_Config_Status_Lock ? em_Button_Config_Status::em_Button_Config_Status_Update : em_Button_Config_Status::em_Button_Config_Status_Lock;
	SwapButtonFieldSumit();

	if (_emButtonConfigStatus == em_Button_Config_Status::em_Button_Config_Status_Lock)
		AsyncSaveConfig();
}

VOID CConfigPurifyWaterDlg::SyncReadConfig()
{
	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg_ReadConfig);
	SocketBuffer << _wsPlayerName << L"PurifyWater";
	SocketBuffer << 1 << L"ChoiceType";

	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [this](MyTools::CLSocketBuffer& SocketBuffer)
	{
		DWORD dwSize = 0;
		SocketBuffer >> dwSize;

		for (decltype(dwSize) i = 0; i < dwSize; ++i)
		{
			ConfigText ConfigText_;
			SocketBuffer >> ConfigText_.wsConfigName >> ConfigText_.wsConfigValue;

			_dwChoiceType = std::stoi(ConfigText_.wsConfigValue);
		}
	});
	SyncServerConfigToLocal();
}

VOID CConfigPurifyWaterDlg::SyncServerConfigToLocal()
{
	reinterpret_cast<CButton*>(GetDlgItem(IDC_RADIO_PURIFYWATER_MONEY))->SetCheck(_dwChoiceType == 0);
	reinterpret_cast<CButton*>(GetDlgItem(IDC_RADIO_PURIFYWATER_BANG))->SetCheck(_dwChoiceType == 1);
}

VOID CConfigPurifyWaterDlg::SwapButtonFieldSumit()
{
	CButton* pButton = reinterpret_cast<CButton *>(this->GetDlgItem(IDC_BUTTON_PURIFYWATER_SUMIT));
	switch (_emButtonConfigStatus)
	{
	case em_Button_Config_Status_Lock:
		pButton->SetWindowTextW(L"修改使用该配置");
		reinterpret_cast<CButton*>(GetDlgItem(IDC_RADIO_PURIFYWATER_MONEY))->EnableWindow(FALSE);
		reinterpret_cast<CButton*>(GetDlgItem(IDC_RADIO_PURIFYWATER_BANG))->EnableWindow(FALSE);
		break;
	case em_Button_Config_Status_Update:
		pButton->SetWindowTextW(L"确认使用该配置");
		reinterpret_cast<CButton*>(GetDlgItem(IDC_RADIO_PURIFYWATER_MONEY))->EnableWindow(TRUE);
		reinterpret_cast<CButton*>(GetDlgItem(IDC_RADIO_PURIFYWATER_BANG))->EnableWindow(TRUE);
		break;
	default:
		break;
	}
}

VOID CConfigPurifyWaterDlg::AsyncSaveConfig()
{
	if (reinterpret_cast<CButton*>(GetDlgItem(IDC_RADIO_PURIFYWATER_MONEY))->GetCheck() && _dwChoiceType != 0x0)
		_dwChoiceType = 0x0;
	else if (reinterpret_cast<CButton*>(GetDlgItem(IDC_RADIO_PURIFYWATER_BANG))->GetCheck() && _dwChoiceType != 0x1)
		_dwChoiceType = 0x0;
	else
		return;

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_WriteConfig);

	SocketBuffer << _wsPlayerName << L"PurifyWater";
	SocketBuffer << 1 << L"ChoiceType" << std::to_wstring(_dwChoiceType);

	CConsoleClient::GetInstance().AsyncSend(SocketBuffer);
}
