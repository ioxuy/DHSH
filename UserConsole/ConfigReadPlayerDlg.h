#pragma once

#include "../GameDLL/GameBase.h"
// CConfigReadPlayerDlg dialog

class CConfigReadPlayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigReadPlayerDlg)

public:
	CConfigReadPlayerDlg(CONST std::vector<std::wstring>& VecConfigName, _In_ CONST std::wstring& wsTypeText, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigReadPlayerDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_READCONFIG_OTHERPLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	std::wstring _wsTypeText;
	std::vector<ConfigText> _VecConfig;
	std::vector<std::wstring> _VecConfigName;
public:
	afx_msg void OnBnClickedButtonReadconfig();
	CONST std::vector<ConfigText>& GetVecConfig() CONST;
};
