#include "stdafx.h"
#include "SkillExtend.h"
#include "Skill.h"
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include "PersonPetExtend.h"
#include "PersonPet.h"

UINT CSkillExtend::GetVecPlayerSkill(_Out_ std::vector<CPlayerSkill>& Vec, _In_ std::function<BOOL(CONST CPlayerSkill&)> fnFilter) CONST
{
	DWORD dwRoot = ReadDWORD(ReadDWORD(C_renwu_Skill_base) + C_Skill_office);

	int nCount = static_cast<int>(ReadDWORD(dwRoot + C_Skill_Num));
	for (int i = 0;i < nCount; ++i)
	{
		CPlayerSkill Skill_(ReadDWORD(dwRoot + C_state_erji) + i * 0x98);
		if(fnFilter == nullptr)
			Vec.push_back(std::move(Skill_));
		else if (fnFilter(Skill_))
		{
			Vec.push_back(std::move(Skill_));
			break;
		}
	}
	return Vec.size();
}

BOOL CSkillExtend::FindPlayerSkill_By_Name(_In_ CONST std::wstring& wsSkillName, _Out_opt_ CPlayerSkill* pPlayerSkill /*= nullptr*/) CONST
{
	std::vector<CPlayerSkill> Vec;
	if (GetVecPlayerSkill(Vec, [wsSkillName](CONST CPlayerSkill& PlayerSkill) { return PlayerSkill.GetName() == wsSkillName; }) == 0)
		return FALSE;

	MyTools::CLPublic::SetPtr(pPlayerSkill, Vec.at(0));
	return TRUE;
}

UINT CSkillExtend::GetVecPetSkill_By_ConditionPtr(_Out_ std::vector<CPetSkill>& Vec, _In_ std::function<BOOL(CONST CPersonPet&)> fnFilter) CONST
{
	std::vector<CPersonPet> VecPet;
	MyTools::InvokeClassPtr<CPersonPetExtend>()->GetVecPet(VecPet, fnFilter);
	for (CONST auto& itm : VecPet)
	{
		for (int k = 0; k < 10; ++k)
		{
			CPetSkill PetSkill(itm.GetNodeBase(), static_cast<UINT>(k));
			if (PetSkill.GetSkillId() > 0)
			{
				PetSkill.SetName();
				Vec.push_back(std::move(PetSkill));
			}
		}
	}

	return Vec.size();
}
