// ConfigReadPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "ConfigReadPlayerDlg.h"
#include "afxdialogex.h"
#include "ConsoleClient.h"

// CConfigReadPlayerDlg dialog

IMPLEMENT_DYNAMIC(CConfigReadPlayerDlg, CDialogEx)

CConfigReadPlayerDlg::CConfigReadPlayerDlg(CONST std::vector<std::wstring>& VecConfigName, _In_ CONST std::wstring& wsTypeText, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_READCONFIG_OTHERPLAYER, pParent),  _wsTypeText(wsTypeText)
{
	_VecConfigName = VecConfigName;
}

CConfigReadPlayerDlg::~CConfigReadPlayerDlg()
{
}

void CConfigReadPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CConfigReadPlayerDlg::OnInitDialog()
{
	this->SetWindowTextW((_wsTypeText).c_str());
	return TRUE;
}

BEGIN_MESSAGE_MAP(CConfigReadPlayerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_READCONFIG, &CConfigReadPlayerDlg::OnBnClickedButtonReadconfig)
END_MESSAGE_MAP()


// CConfigReadPlayerDlg message handlers


void CConfigReadPlayerDlg::OnBnClickedButtonReadconfig()
{
	CStringW strPlayerName;
	reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_PLAYERNAME))->GetWindowTextW(strPlayerName);
	if (strPlayerName.Trim().GetLength() == 0)
	{
		AfxMessageBox(L"你确定有角色名是空的配置吗?");
		return;
	}


	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_ReadConfig);
	SocketBuffer << strPlayerName.Trim().GetBuffer() << _VecConfigName.size();
	for (CONST auto& itm : _VecConfigName)
		SocketBuffer << itm;

	DWORD dwSize = 0;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [this,&dwSize](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> dwSize;

		for (DWORD i = 0;i < dwSize; ++i)
		{
			ConfigText ConfigText_;
			SocketBuffer >> ConfigText_.wsConfigName >> ConfigText_.wsConfigValue;;
			_VecConfig.push_back(std::move(ConfigText_));
		}
	});

	AfxMessageBox(dwSize == 0 ? L"不存在该角色名的配置！" : L"读取完毕!");
	this->PostMessageW(WM_CLOSE);
}

CONST std::vector<ConfigText>& CConfigReadPlayerDlg::GetVecConfig() CONST
{
	return _VecConfig;
}
