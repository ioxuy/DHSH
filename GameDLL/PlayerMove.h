#ifndef __DHSH_GAMEDLL_GAME_METHOD_ACTION_PLAYERMOVE_H__
#define __DHSH_GAMEDLL_GAME_METHOD_ACTION_PLAYERMOVE_H__

#include "GameBase.h"

class CPlayerMove : public MyTools::CRelfexBaseClass
{
public:
	CPlayerMove() = default;
	~CPlayerMove() = default;
	
	// Current Map
	BOOL MoveToPoint(_In_ CONST Point& TarPoint) CONST;

	// different Map
	BOOL MoveToMapPoint(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST;

	// 用符传送
	BOOL TransferToCity(_In_ CONST std::wstring& wsCityName) CONST;

	// 去杂货摊
	BOOL MoveToGeneralStore(_Out_ std::wstring& wsNpcName) CONST;

	BOOL MoveToGeneralStoreNpc(_In_ CONST std::wstring& wsNpcName) CONST;

public:
	static CPlayerMove* CreateInstance()
	{
		return new CPlayerMove;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPlayerMove*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CPlayerMove";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_ACTION_PLAYERMOVE_H__
