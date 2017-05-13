#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONATTRIBUTE_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONATTRIBUTE_H__

#include "GameBase.h"

class CPersonAttribute : public MyTools::CRelfexBaseClass
{
	struct PersonBuff
	{
		std::wstring wsName;
		DWORD dwSurpTime;
		DWORD dwObjAddr;
	};
public:
	CPersonAttribute() = default;
	~CPersonAttribute() = default;
	
	// 当前所在的地图名
	CONST std::wstring GetCurrentMapName() CONST;
	
	// 等级
	DWORD GetLevel() CONST;

	// HP
	DWORD GetHp() CONST;
	DWORD GetMaxHp() CONST;
	DWORD GetPercentHp() CONST;

	// MP
	DWORD GetMp() CONST;
	DWORD GetMaxMp() CONST;
	DWORD GetPercentMp() CONST;

	// Exp
	DWORD GetExp() CONST;

	// InWar?
	BOOL IsWar() CONST;

	// Point
	Point GetPoint() CONST;

	// 
	DWORD GetMoney() CONST;
	DWORD GetJiaoZiMoney() CONST;

	//
	UINT GetVecPersonBuff(_Out_ std::vector<PersonBuff>& Vec, _In_ std::function<BOOL(CONST PersonBuff&)> FilterPtr) CONST;
	BOOL ExistPersonBuff_By_Name(_In_ CONST std::wstring& wsName) CONST;
public:
	static CPersonAttribute* CreateInstance()
	{
		return new CPersonAttribute;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPersonAttribute*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CPersonAttribute";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PERSON_PERSONATTRIBUTE_H__
