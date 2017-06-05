#pragma once

#include "ConsoleVariable.h"
// CConfigModeDlg dialog

class CConfigModeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigModeDlg)

public:
	CConfigModeDlg(_In_ GameAccountShareContent* pGameAccountShareContent, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigModeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONFIGMODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRun();

private:
	GameAccountShareContent* _pGameAccountShareContent;
public:
	afx_msg void OnBnClickedButtonStop();
};
