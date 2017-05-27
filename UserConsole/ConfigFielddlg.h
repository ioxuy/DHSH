#pragma once

#include "../GameDLL/GameBase.h"
// CConfigFielddlg dialog

class CConfigFielddlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigFielddlg)

public:
	CConfigFielddlg(_In_ CONST std::wstring& wsPlayerName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigFielddlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONFIG_FIELD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFieldSumit();
private:
	em_Button_Config_Status _emButtonConfigStatus;
	std::vector<CWnd*>		_VecFieldConfigCtrl;
	std::vector<ConfigText> _VecConfigText;
	std::wstring			_wsPlayerName;
	CONST static std::vector<std::wstring> _VecConfigName;
private:
	VOID SwapButtonFieldSumit();

	VOID AsyncSaveConfig();
public:
	afx_msg void OnBnClickedCheckFieldDefaultconfig();
	afx_msg void OnBnClickedButtonFieldSaveas();
private:
	DWORD GetValue_PersonHp() CONST;
	DWORD GetValue_PersonMp() CONST;
	DWORD GetValue_PetHp() CONST;
	DWORD GetValue_PetMp() CONST;

	int GetPersonFightMode() CONST;
	int GetPerFightMode() CONST;
	BOOL GetValue_AutoBuyHappyBell() CONST;

	VOID SyncReadConfig();

	VOID SyncServerConfigToLocal();

	int FindConfigValue_By_ConfigName_In_VecConfig(_In_ CONST std::wstring& wsConfigName) CONST;

	VOID AddCompVecConfig(_In_ int nValue, _In_ CONST std::wstring& wsConfigName, _Out_ std::vector<ConfigText>& Vec) CONST;
public:
	afx_msg void OnBnClickedButtonFieldReadconfigOtherplayer();
};
