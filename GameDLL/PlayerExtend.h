#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PLAYER_PLAYEREXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PLAYER_PLAYEREXTEND_H__

#include "GameVariable.h"
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>

class CPlayer;
class CPlayerExtend : public MyTools::CRelfexBaseClass
{
public:
	CPlayerExtend() = default;
	~CPlayerExtend() = default;
	
	template<typename T>
	UINT GetAroundObject(_Out_ std::vector<T>& Vec, _In_ std::function<BOOL(CONST CPlayer&)> fnFilter) CONST
	{
		DWORD dwRoot = ReadDWORD(ReadDWORD(C_huan_base) + C_huan_yiji);
		DWORD dwNextNode = ReadDWORD(dwRoot + 0x0);
		while (dwRoot != dwNextNode)
		{
			DWORD dwNodeBase = ReadDWORD(dwNextNode + 0x8);
			if (dwNodeBase > 0)
			{
				T Player(dwNodeBase);
				DWORD dwType = static_cast<DWORD>(Player.GetType());
				if (dwType != 0x6 && dwType != 0x7)
				{
					Player.SetName();
					if (fnFilter == nullptr)
						Vec.push_back(std::move(Player));
					else if (fnFilter(Player))
					{
						Vec.push_back(std::move(Player));
						break;
					}
				}
				
			}
			dwNextNode = ReadDWORD(dwNextNode);
		}
		return Vec.size();
	}

	BOOL GetPerson(_Out_ CPlayer* pPlayer) CONST;
public:
	static CPlayerExtend* CreateInstance()
	{
		return new CPlayerExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPlayerExtend*>(lpObjectAddr);
	}
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PLAYER_PLAYEREXTEND_H__
