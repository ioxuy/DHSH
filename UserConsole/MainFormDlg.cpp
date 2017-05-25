// MainFormDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "MainFormDlg.h"
#include "afxdialogex.h"
#include "ConsoleVariable.h"
#include "ConsoleClient.h"
#include "RechargeCardDlg.h"
#include "RechargeCardLogDlg.h"
#include "UserConsoleDlg.h"
#include <mutex>
// CMainFormDlg dialog

IMPLEMENT_DYNAMIC(CMainFormDlg, CDialogEx)

CMainFormDlg::CMainFormDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MAINFORM, pParent)
{

}

CMainFormDlg::~CMainFormDlg()
{
}

void CMainFormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BOOL CMainFormDlg::OnInitDialog()
{
	CUserConsoleDlg dlg;
	dlg.DoModal();

	if (CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.dwToken == NULL)
	{
		this->PostMessageW(WM_CLOSE);
		return TRUE;
	}
	SetAccountHour_To_Dlg();

	bRun = TRUE;
	hThread = cbBEGINTHREADEX(NULL, NULL, _KeepALiveThread, this, NULL, NULL);
	return TRUE;
}

DWORD WINAPI CMainFormDlg::_KeepALiveThread(LPVOID lpParam)
{
	auto dlg = static_cast<CMainFormDlg*>(lpParam);
	while (dlg->bRun)
	{
		::Sleep(10 * 1000);
		dlg->SetAccountHour_To_Dlg();
	}
	return 0;
}

VOID CMainFormDlg::SetAccountHour_To_Dlg()
{
	static std::mutex mutex_;
	std::lock_guard<std::mutex> lock_(mutex_);

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_KeepLive);

	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.dwHour;
		CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.ulTimeTick = ::GetTickCount64();
	});

	CStatic* pStatic = reinterpret_cast<CStatic *>(GetDlgItem(IDC_STATIC_HOUR));

	CStringW strHour;
	strHour.Format(L"%d", CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.dwHour);
	pStatic->SetWindowTextW(strHour);
}

BEGIN_MESSAGE_MAP(CMainFormDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RECHARGECARD, &CMainFormDlg::OnBnClickedButtonRechargecard)
	ON_BN_CLICKED(IDC_BUTTON_RECHARGECARDLOG, &CMainFormDlg::OnBnClickedButtonRechargecardlog)
	ON_BN_CLICKED(IDC_BUTTON_RUNGAME, &CMainFormDlg::OnBnClickedButtonRungame)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMainFormDlg message handlers


void CMainFormDlg::OnBnClickedButtonRechargecard()
{
	CRechargeCardDlg dlg;
	dlg.DoModal();
	SetAccountHour_To_Dlg();
}


void CMainFormDlg::OnBnClickedButtonRechargecardlog()
{
	CRechargeCardLogDlg dlg;
	dlg.DoModal();
}


void CMainFormDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}


void CMainFormDlg::OnBnClickedButtonRungame()
{
	// TODO: Add your control notification handler code here
}


void CMainFormDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	bRun = FALSE;
	::WaitForSingleObject(hThread, INFINITE);
	hThread = NULL;
	CDialogEx::OnClose();
}
