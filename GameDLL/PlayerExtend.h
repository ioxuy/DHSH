#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PLAYER_PLAYEREXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PLAYER_PLAYEREXTEND_H__

#include "Player.h"
#include <MyTools/Character.h>
#include <MyTools/Log.h>

class CPlayerExtend : public MyTools::CRelfexBaseClass
{
public:
	CPlayerExtend() = default;
	~CPlayerExtend() = default;
	
	template<typename T>
	UINT GetAroundObject(_In_ CPlayer::em_PlayerType emPlayerType, _Out_ std::vector<T>& Vec)
	{
		DWORD dwRoot = ReadDWORD(ReadDWORD(C_huan_base) + C_huan_yiji);
		DWORD dwNextNode = ReadDWORD(dwRoot + 0x0);
		while (dwRoot != dwNextNode)
		{
			DWORD dwNodeBase = ReadDWORD(dwNextNode + 0x8);
			if (dwNodeBase > 0)
			{
				T Player(dwNodeBase);
				if (emPlayerType == CPlayer::em_PlayerType::em_PlayerType_ALL)
				{
					DWORD dwType = static_cast<DWORD>(Player.GetType());
					if (dwType != 0x6 && dwType != 0x7)
					{
						Player.SetName();
						Vec.push_back(std::move(Player));
					}
				}
				else if (Player.GetType() == emPlayerType)
				{
					Player.SetName();
					Vec.push_back(std::move(Player));
				}
			}
			dwNextNode = ReadDWORD(dwNextNode);
		}
		return Vec.size();
	}
	
public:
	static CPlayerExtend* CreateInstance()
	{
		return new CPlayerExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPlayerExtend*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CPlayerExtend";
		return wsClassName;
	}
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PLAYER_PLAYEREXTEND_H__
