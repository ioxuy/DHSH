#ifndef __DHSH_GAMEDLL_GAME_OBJECT_GAMEUI_GAMEUI_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_GAMEUI_GAMEUI_H__

#include "BaseObject.h"

class CGameUi : public CBaseObject
{
public:
	struct DlgOpionText
	{
		DWORD dwNodeBase;
		std::wstring wsOptionText;
		std::wstring wsClickText;
	};
public:
	CGameUi();
	explicit CGameUi(_In_ DWORD dwNodeBase);
	~CGameUi() = default;

	virtual VOID SetName();

	BOOL IsShow() CONST;

	BOOL FindNpcdlgOption_By_PartText_Excute_ActionPtr(_In_ CONST std::wstring& wsPartText,_In_ std::function<VOID(CONST DlgOpionText&)> ActionPtr) CONST;

	VOID PrintOpionText() CONST;
private:
	UINT GetVecOptionText(_Out_ std::vector<DlgOpionText>& Vec, _In_ std::function<BOOL(CONST std::wstring&)> FilterPtr) CONST;
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_GAMEUI_GAMEUI_H__
