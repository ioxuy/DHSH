#pragma once

// CUserConfigDlg dialog
#include "ConsoleVariable.h"
#include "ConfigFielddlg.h"
#include "ConfigCollectDlg.h"
#include "ConfigModeDlg.h"
#include "ConfigBangTaskDlg.h"
#include "ConfigPurifyWaterDlg.h"


class CUserConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserConfigDlg)

public:
	CUserConfigDlg(_In_ GameAccountShareContent* pGameAccountShareContent, _In_ CONST std::wstring& wsPlayerName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserConfigDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_USERCONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CONST std::wstring& GetPlayerName() CONST;
	VOID  MyDestoreWindows();

	GameAccountShareContent* GetGameAccountShareContentPtr();

	BOOL CheckConfig(_In_ CONST std::wstring& wsText) CONST;
private:
	std::wstring _wsPlayerName;
	CConfigCollectDlg       _CollectDlg;
	CConfigFielddlg         _FieldDlg;
	CConfigModeDlg          _ConfigModeDlg;
	CConfigBangTaskDlg		_BangTaskDlg;
	CConfigPurifyWaterDlg	_PurifyWaterDlg;
	std::vector<CDialogEx*> _VecDlg;
	BOOL                    _bClose;
	GameAccountShareContent* _pGameAccountShareContent;
public:
	afx_msg void OnClose();
};
