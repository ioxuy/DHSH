#pragma once

#include "UserConfigDlg.h"
#include "RunGame.h"
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
	// 心跳刷新剩余时间
	VOID SetAccountHour_To_Dlg();

	// 根据玩家名称查找配置窗口指针
	CUserConfigDlg* FindDlg_By_PlayerName(_In_ CONST std::wstring& wsPlayerName);

	//
	BOOL FindPlayerName_By_Index(_In_ UINT uIndex, _Out_ std::wstring& wsPlayername) CONST;

	// Add
	VOID AddDlgContent(_In_ CUserConfigDlg* pUserConfigDlg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRechargecard();
	afx_msg void OnBnClickedButtonRechargecardlog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonRungame();

	BOOL ChoiceGamePath(_Out_ std::wstring& wsGamePath) CONST;

	BOOL SaveGamePath(_In_ CONST std::wstring& wsGamePath) CONST;

	BOOL SyncGamePath();
protected:
	BOOL                       _bRun;
	HANDLE                     _hKeepALiveThread;

	std::vector<CUserConfigDlg*> _VecDlgContent;
	CRunGame	               _RunGame;
	int                        _nHour;
public:
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL CloseConfigDlg();

	VOID KillGame();
};
