#pragma once


// CRechargeCardDlg dialog

class CRechargeCardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRechargeCardDlg)

public:
	CRechargeCardDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRechargeCardDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RECHARGECARD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonSumitRecharge();

	BOOL VerCardNo(_In_ LPCWSTR pwszCardNo ) CONST;
	virtual void OnOK();
};
