// RechargeCardLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "RechargeCardLogDlg.h"
#include "afxdialogex.h"
#include <thread>
#include "ConsoleClient.h"
#include "ConsoleVariable.h"

// CRechargeCardLogDlg dialog

IMPLEMENT_DYNAMIC(CRechargeCardLogDlg, CDialogEx)

CRechargeCardLogDlg::CRechargeCardLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RECHARGECARDLOG, pParent)
{
	bReading = FALSE;
}

CRechargeCardLogDlg::~CRechargeCardLogDlg()
{
}

void CRechargeCardLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CRechargeCardLogDlg::OnInitDialog()
{
	std::thread t([this] 
	{
		CEdit* pEdit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_CARDLOG));
		pEdit->SetWindowTextW(L"读取中……");


		MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_QueryCard);
		CConsoleClient::GetInstance().SyncSend(SocketBuffer, [pEdit](MyTools::CLSocketBuffer& SocketBuffer)
		{
			DWORD dwSize = 0;
			SocketBuffer >> dwSize;

			std::wstring wsEditText;
			for (decltype(dwSize) i = 0;i < dwSize; ++i)
			{
				std::wstring wsText;
				SocketBuffer >> wsText;
				wsEditText += wsText + L"\r\n";
			}
			pEdit->SetWindowTextW(wsEditText.c_str());
		});

		bReading = TRUE;
	});
	t.detach();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CRechargeCardLogDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRechargeCardLogDlg message handlers


void CRechargeCardLogDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (!bReading)
	{
		AfxMessageBox(L"正在读取中……");
		return;
	}
	CDialogEx::OnClose();
}


void CRechargeCardLogDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}
