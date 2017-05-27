// ConfigModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "ConfigModeDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <string.h>
#include <algorithm>

// CConfigModeDlg dialog

IMPLEMENT_DYNAMIC(CConfigModeDlg, CDialogEx)

CConfigModeDlg::CConfigModeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONFIGMODE, pParent)
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
			L"Ò°Íâ",L"²É¼¯",L"ÕóÓª",L"¾»Ë®",L"Ê¤Ä¾",L"¿¼ÊÔ",L"·Ö½â",
			L"Íú»ð",L"Ð¡µÜ",L"»¤ËÍ",L"ÎåÐÐ",L"ÊÔÁ·",L"ÎÚ¹ê",L"Ê°»Ä"
		};

		std::for_each(VecText.begin(), VecText.end(), [&pComBoBox](CONST auto& itm) { pComBoBox->InsertString(pComBoBox->GetCount(), itm.c_str());  });
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CConfigModeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CConfigModeDlg::OnBnClickedButtonRun)
END_MESSAGE_MAP()


// CConfigModeDlg message handlers


void CConfigModeDlg::OnBnClickedButtonRun()
{
	// TODO: Add your control notification handler code here
}
