// UserConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "UserConfigDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <MyTools/Log.h>

#define _SELF L"UserConfigDlg.cpp"
// CUserConfigDlg dialog

IMPLEMENT_DYNAMIC(CUserConfigDlg, CDialogEx)

CUserConfigDlg::CUserConfigDlg(_In_ CONST std::wstring& wsPlayerName, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_USERCONFIG, pParent), 
	_wsPlayerName(wsPlayerName), 
	_CollectDlg(_wsPlayerName), 
	_FieldDlg(wsPlayerName),
	_BangTaskDlg(wsPlayerName),
	_bClose(FALSE)
{
	
}

CUserConfigDlg::~CUserConfigDlg()
{
	LOG_C_D(L"CUserConfigDlg::~CUserConfigDlg");
}

void CUserConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CUserConfigDlg::OnInitDialog()
{
	CTabCtrl* pTabCtrl = reinterpret_cast<CTabCtrl*>(this->GetDlgItem(IDC_TAB1));
	pTabCtrl->InsertItem(0, L"模式选择");
	pTabCtrl->InsertItem(1, L"野外刷怪");
	pTabCtrl->InsertItem(2, L"采集");
	pTabCtrl->InsertItem(3, L"胜木");

	_ConfigModeDlg.Create(IDD_DIALOG_CONFIGMODE, reinterpret_cast<CWnd *>(pTabCtrl));
	_FieldDlg.Create(IDD_DIALOG_CONFIG_FIELD, reinterpret_cast<CWnd *>(pTabCtrl));
	_CollectDlg.Create(IDD_DIALOG_CONFIG_COLLECT, reinterpret_cast<CWnd *>(pTabCtrl));
	_BangTaskDlg.Create(IDD_DIALOG_CONFIG_BANGTASK, reinterpret_cast<CWnd *>(pTabCtrl));

	_VecDlg.push_back(&_ConfigModeDlg);
	_VecDlg.push_back(&_FieldDlg);
	_VecDlg.push_back(&_CollectDlg);
	_VecDlg.push_back(&_BangTaskDlg);

	CRect tabRect;
	pTabCtrl->GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	for (auto& itm : _VecDlg)
	{
		itm->SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	}
	_ConfigModeDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);

	this->SetWindowTextW(_wsPlayerName.c_str());
	return TRUE;
}

BEGIN_MESSAGE_MAP(CUserConfigDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CUserConfigDlg::OnTcnSelchangeTab1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUserConfigDlg message handlers


void CUserConfigDlg::OnTcnSelchangeTab1(NMHDR *, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CRect tabRect;

	CTabCtrl* pTabCtrl = reinterpret_cast<CTabCtrl*>(this->GetDlgItem(IDC_TAB1));
	pTabCtrl->GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	auto CurSel = pTabCtrl->GetCurSel();
	if (CurSel != -1 && CurSel < static_cast<int>(_VecDlg.size()))
	{
		std::for_each(_VecDlg.begin(), _VecDlg.end(), [](auto& itm) { itm->ShowWindow(SW_HIDE); });
		_VecDlg.at(CurSel)->ShowWindow(SW_SHOW);
	}
}

CONST std::wstring& CUserConfigDlg::GetPlayerName() CONST
{
	return _wsPlayerName;
}


VOID CUserConfigDlg::MyDestoreWindows()
{
	_bClose = TRUE;
	this->PostMessageW(WM_CLOSE);
}

void CUserConfigDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (_bClose)
		CDialogEx::OnClose();
	else
		this->ShowWindow(SW_HIDE);
}
