#ifndef __DHSH_GAMEDLL_GAME_OBJECT_NPC_NPC_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_NPC_NPC_H__

#include "Player.h"

class CNpc : public CPlayer
{
public:
	CNpc();
	CNpc(_In_ DWORD dwNodeBase);
	~CNpc() = default;

	BOOL OpenNpcDlg() CONST;

	// npcdlg visiable
	BOOL ClickOption(_In_ CONST std::wstring& wsOptionText, _In_ CONST std::wstring& wsNewDlg) CONST;

	//
	BOOL ClickOption_Once(_In_ CONST std::wstring& wsOptionText) CONST;

	//
	BOOL CLickOption_DisableDlg(_In_ CONST std::wstring& wsOptionText, _In_ CONST std::wstring& wsDlg) CONST;

	BOOL ClickOption_By_Condition(_In_ CONST std::wstring& wsOptionText, _In_ std::function<BOOL(VOID)> fnExitPtr) CONST;

	// 
	BOOL Collect() CONST;
	BOOL CollectFurniture() CONST;

private:
	BOOL Collect_By_ActionPtr(_In_ std::function<DWORD(VOID)> ActionPtr) CONST;
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_NPC_NPC_H__
