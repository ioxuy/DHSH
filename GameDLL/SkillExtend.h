#ifndef __DHSH_GAMEDLL_GAME_OBJECT_SKILL_SKILLEXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_SKILL_SKILLEXTEND_H__

#include "GameBase.h"

class CPlayerSkill;
class CPetSkill;
class CSkillExtend : public MyTools::CRelfexBaseClass
{
public:
	CSkillExtend() = default;
	~CSkillExtend() = default;
	
	UINT GetVecPlayerSkill(_Out_ std::vector<CPlayerSkill>& Vec) CONST;
	
	UINT GetVecPetSkill(_Out_ std::vector<CPetSkill>& Vec) CONST;
public:
	static CSkillExtend* CreateInstance()
	{
		return new CSkillExtend;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CSkillExtend*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CSkillExtend";
		return wsClassName;
	}
};

#endif // !__DHSH_GAMEDLL_GAME_OBJECT_SKILL_SKILLEXTEND_H__
