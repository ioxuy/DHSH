// ConfigModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "ConfigModeDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <string.h>
#include <algorithm>
#include "UserConfigDlg.h"

// CConfigModeDlg dialog

IMPLEMENT_DYNAMIC(CConfigModeDlg, CDialogEx)

CConfigModeDlg::CConfigModeDlg(_In_ GameAccountShareContent* pGameAccountShareContent, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONFIGMODE, pParent),
	_pGameAccountShareContent(pGameAccountShareContent)
{

}

CConfigModeDlg::~CConfigModeDlg()
{
}

void CConfigModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CConfigModeDlg::OnInitDialog()
{
	CComboBox* pComBoBox = reinterpret_cast<CComboBox *>(this->GetDlgItem(IDC_COMBO_CONFIGMODE));
	if (pComBoBox->GetCount() == 0)
	{
		CONST static std::vector<std::wstring> VecText = 
		{
			L"野外",L"采集",L"阵营",L"净水",L"胜木",L"考试",L"分解",
			L"旺火",L"小弟",L"护送",L"五行",L"试练",L"乌龟",L"拾荒"
		};

		std::for_each(VecText.begin(), VecText.end(), [&pComBoBox](CONST auto& itm) { pComBoBox->InsertString(pComBoBox->GetCount(), itm.c_str());  });
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CConfigModeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CConfigModeDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CConfigModeDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CConfigModeDlg message handlers


void CConfigModeDlg::OnBnClickedButtonRun()
{
	// Check Save Config?
	CStringW strText;
	CComboBox* pComBoBox = reinterpret_cast<CComboBox *>(this->GetDlgItem(IDC_COMBO_CONFIGMODE));
	if (pComBoBox->GetCurSel() == -1)
	{
		AfxMessageBox(L"你一定知道我想要说什么的!");
		return;
	}

	pComBoBox->GetLBText(pComBoBox->GetCurSel(), strText);

	CUserConfigDlg* dlg = reinterpret_cast<CUserConfigDlg *>(this->GetParent());
	if (dlg == nullptr)
	{
		AfxMessageBox(L"dlg = nullptr!");
		return;
	}

	if (!dlg->CheckConfig(strText.GetBuffer()))
		return;

	// Post Message
	::PostMessage(_pGameAccountShareContent->AccountStatus.hGameWnd, WM_CUSTOME_READCONFIG, NULL, NULL);
}

void CConfigModeDlg::OnBnClickedButtonStop()
{
	::PostMessage(_pGameAccountShareContent->AccountStatus.hGameWnd, WM_CUSTOME_STOP, NULL, NULL);
}
