#include "stdafx.h"
#include "SkillExtend.h"
#include "Skill.h"
#include <MyTools/Character.h>
#include "PersonPetExtend.h"
#include "PersonPet.h"

UINT CSkillExtend::GetVecPlayerSkill(_Out_ std::vector<CPlayerSkill>& Vec) CONST
{
	DWORD dwRoot = ReadDWORD(ReadDWORD(C_renwu_Skill_base) + C_Skill_office);

	int nCount = static_cast<int>(ReadDWORD(dwRoot + C_Skill_Num));
	for (int i = 0;i < nCount; ++i)
	{
		CPlayerSkill Skill_(ReadDWORD(dwRoot + C_state_erji) + i * 0x98);
		Vec.push_back(std::move(Skill_));
	}
	return Vec.size();
}

UINT CSkillExtend::GetVecPetSkill(_Out_ std::vector<CPetSkill>& Vec) CONST
{
	std::vector<CPersonPet> VecPet;
	MyTools::InvokeClassPtr<CPersonPetExtend>()->GetVecPet(VecPet, [](CONST auto&) { return TRUE; });
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
