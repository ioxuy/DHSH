#ifndef __DHSH_GAMEDLL_USERCONSOLE_CODE_DIALOG_BASECONFIGDIALOG_H__
#define __DHSH_GAMEDLL_USERCONSOLE_CODE_DIALOG_BASECONFIGDIALOG_H__

#include "../GameDLL/GameBase.h"

class CBaseConfigDialog
{
public:
	CBaseConfigDialog(_In_ CDialogEx* pCDialogEx, _In_ UINT uSumitButtonId);
	~CBaseConfigDialog() = default;

	// 
	BOOL IsButtonLock() CONST;

protected:
	// 
	VOID SwapButtonFieldSumit();

	// 
	virtual VOID AsyncSaveConfig() = NULL;

	// 
	virtual VOID SyncReadConfig() = NULL;

	//
	virtual VOID SyncServerConfigToLocal() = NULL;

protected:
	//
	int FindConfigValue_By_ConfigName_In_VecConfig(_In_ CONST std::wstring& wsConfigName) CONST;

	//
	VOID AddCompVecConfig(_In_ int nValue, _In_ CONST std::wstring& wsConfigName, _Out_ std::vector<ConfigText>& Vec) CONST;
private:
	em_Button_Config_Status _emButtonConfigStatus;
	std::wstring _wsPlayerName;
	std::vector<CWnd *> _VecCtrl;
	std::vector<ConfigText> _VecConfig;
	std::vector<std::wstring> _VecConfigName;
	UINT _uSumitButtonId;
	CDialogEx* _pDialog;
};



#endif // !__DHSH_GAMEDLL_USERCONSOLE_CODE_DIALOG_BASECONFIGDIALOG_H__
