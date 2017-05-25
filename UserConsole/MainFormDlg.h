#pragma once


// CMainFormDlg dialog

class CMainFormDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainFormDlg)

public:
	CMainFormDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainFormDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAINFORM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	static DWORD WINAPI _KeepALiveThread(LPVOID lpParam);
private:
	VOID SetAccountHour_To_Dlg();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRechargecard();
	afx_msg void OnBnClickedButtonRechargecardlog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonRungame();
protected:
	BOOL bRun;
	HANDLE hThread;
public:
	afx_msg void OnClose();
};
