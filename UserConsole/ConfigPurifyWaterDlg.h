#pragma once

#include "../GameDLL/GameBase.h"
// CConfigPurifyWaterDlg dialog

class CConfigPurifyWaterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigPurifyWaterDlg)

public:
	CConfigPurifyWaterDlg(_In_ CONST std::wstring& wsPlayerName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigPurifyWaterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPurifywaterSumit();
private:
	// 
	VOID SyncReadConfig();

	//
	VOID SyncServerConfigToLocal();

	//
	VOID SwapButtonFieldSumit();

	//
	VOID AsyncSaveConfig();
private:
	std::wstring _wsPlayerName;
	em_Button_Config_Status _emButtonConfigStatus;
	DWORD _dwChoiceType;
};
