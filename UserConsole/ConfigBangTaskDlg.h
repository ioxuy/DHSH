#pragma once

#include "../GameDLL/GameBase.h"
// CConfigBangTaskDlg dialog

class CConfigBangTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigBangTaskDlg)

public:
	CConfigBangTaskDlg(_In_ CONST std::wstring& wsPlayerName ,CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigBangTaskDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONFIG_BANGTASK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBangtaskReadconfigOtherplayer();
private:
	// 
	VOID SwapButtonFieldSumit();

	//
	VOID SyncReadConfig();

	//
	VOID SyncServerConfigToLocal();

	//
	int FindConfigValue_By_ConfigName_In_VecConfig(_In_ CONST std::wstring& wsConfigName) CONST;

	//
	VOID AsyncSaveConfig();

	//
	VOID AddCompVecConfig(_In_ int nValue, _In_ CONST std::wstring& wsConfigName, _Out_ std::vector<ConfigText>& Vec) CONST;

private:
	// 
	int GetValue_BuyExcorism();

	// 
	int GetValue_BuyReturnSymbol();

	//
	int GetValue_UseReturnSymbol();
private:
	std::wstring _wsPlayerName;
	std::vector<CWnd *> _VecCtrl;
	em_Button_Config_Status _emButtonConfigStatus;
	static CONST std::vector<std::wstring> _VecConfigName;
	std::vector<ConfigText> _VecConfig;
public:
	afx_msg void OnBnClickedCheckBangtaskDefaultconfig();
	afx_msg void OnBnClickedButtonBangtaskSumit();
};
