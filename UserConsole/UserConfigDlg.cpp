// UserConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "UserConfigDlg.h"
#include "afxdialogex.h"
#include <algorithm>

// CUserConfigDlg dialog

IMPLEMENT_DYNAMIC(CUserConfigDlg, CDialogEx)

CUserConfigDlg::CUserConfigDlg(_In_ CONST std::wstring& wsPlayerName, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_USERCONFIG, pParent), 
	_wsPlayerName(wsPlayerName), 
	_CollectDlg(_wsPlayerName), 
	_FieldDlg(wsPlayerName)
{

}

CUserConfigDlg::~CUserConfigDlg()
{
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

	_ConfigModeDlg.Create(IDD_DIALOG_CONFIGMODE, reinterpret_cast<CWnd *>(pTabCtrl));
	_FieldDlg.Create(IDD_DIALOG_CONFIG_FIELD, reinterpret_cast<CWnd *>(pTabCtrl));
	_CollectDlg.Create(IDD_DIALOG_CONFIG_COLLECT, reinterpret_cast<CWnd *>(pTabCtrl));

	_VecDlg.push_back(&_ConfigModeDlg);
	_VecDlg.push_back(&_FieldDlg);
	_VecDlg.push_back(&_CollectDlg);

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

	return TRUE;
}

BEGIN_MESSAGE_MAP(CUserConfigDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CUserConfigDlg::OnTcnSelchangeTab1)
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
