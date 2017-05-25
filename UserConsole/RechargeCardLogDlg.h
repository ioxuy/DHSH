#pragma once


// CRechargeCardLogDlg dialog

class CRechargeCardLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRechargeCardLogDlg)

public:
	CRechargeCardLogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRechargeCardLogDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RECHARGECARDLOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	BOOL bReading;
public:
	afx_msg void OnClose();
	virtual void OnOK();
};
