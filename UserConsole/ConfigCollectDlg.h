#pragma once

#include "../GameDLL/GameBase.h"

class CConfigCollectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigCollectDlg)

public:
	CConfigCollectDlg(_In_ CONST std::wstring& wsPlayerName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigCollectDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONFIG_COLLECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckAutobuyPetdrink();
private:
	VOID SetEnableFurniture(_In_ BOOL bEnable);
public:
	afx_msg void OnCbnSelchangeComboCollectMap();
	afx_msg void OnBnClickedButtonCollectSumit();
private:
	// 
	VOID SwapButtonFieldSumit();

	// 
	VOID AsyncSaveConfig();

	//
	VOID SyncReadConfig();

	//
	VOID SyncServerConfigToLocal();

	// 
	int FindConfigValue_By_ConfigName_In_VecConfig(_In_ CONST std::wstring& wsConfigName) CONST;

	//
	VOID AddCompVecConfig(_In_ int nValue, _In_ CONST std::wstring& wsConfigName, _Out_ std::vector<ConfigText>& Vec) CONST;
private:
	// 
	BOOL GetValue_UseExcorism() CONST;

	// 
	BOOL GetValue_BuyExcorism() CONST;

	//
	BOOL GetValue_BuyPetDrink() CONST;

	//
	int GetValue_PetDrinkCount() CONST;

	//
	int GetValue_CollectMap() CONST;

	//
	BOOL GetValue_CollectResItem() CONST;

	//
	BOOL GetValue_CollectReadBean() CONST;

	//
	int GetValue_FurnitureIndex() CONST;

private:
	em_Button_Config_Status _emButtonConfigStatus;
	std::vector<CWnd *> _VecCtrl;
	std::vector<ConfigText> _VecConfig;
	std::wstring _wsPlayerName;
	CONST static std::vector<std::wstring> _VecConfigName;
public:
	afx_msg void OnBnClickedCheckCollectDefaultconfig();
	afx_msg void OnBnClickedButtonCollectReadconfigOtherplayer();
};
