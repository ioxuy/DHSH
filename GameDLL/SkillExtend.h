#ifndef __DHSH_GAMEDLL_GAME_OBJECT_SKILL_SKILLEXTEND_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_SKILL_SKILLEXTEND_H__

#include "GameBase.h"

class CPlayerSkill;
class CPetSkill;
class CPersonPet;
class CSkillExtend : public MyTools::CRelfexBaseClass
{
public:
	CSkillExtend() = default;
	~CSkillExtend() = default;
	
	// 当前人物的所有技能
	UINT GetVecPlayerSkill(_Out_ std::vector<CPlayerSkill>& Vec, _In_ std::function<BOOL(CONST CPlayerSkill&)> fnFilter) CONST;

	// 查找人物的某个技能
	BOOL FindPlayerSkill_By_Name(_In_ CONST std::wstring& wsSkillName, _Out_opt_ CPlayerSkill* pPlayerSkill = nullptr) CONST;
	
	// 宠物的技能
	UINT GetVecPetSkill_By_ConditionPtr(_Out_ std::vector<CPetSkill>& Vec, _In_ std::function<BOOL(CONST CPersonPet&)> fnFilter) CONST;


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
