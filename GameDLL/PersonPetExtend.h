#ifndef __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETEXTPEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETEXTPEND_H__

#include "GameBase.h"

class CPersonPet;
class CPersonPetExtend : public MyTools::CRelfexBaseClass
{
public:
	CPersonPetExtend() = default;
	~CPersonPetExtend() = default;
	
	UINT GetVecPet(_Out_ std::vector<CPersonPet>& Vec, _In_ std::function<BOOL(CONST CPersonPet&)> FilterPtr) CONST;
	
	BOOL Action_By_JoinWarPet(_In_ std::function<VOID(CONST CPersonPet&)> ActionPtr) CONST;
public:
	static CPersonPetExtend* CreateInstance()
	{
		return new CPersonPetExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CPersonPetExtend*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CPersonPetExtend";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_PET_PERSONPETEXTPEND_H__
