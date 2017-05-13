#include "stdafx.h"
#include "Skill.h"
#include <MyTools/Character.h>
#include "GameCALL.h"

CPlayerSkill::CPlayerSkill()
{
	_dwNodeBase = NULL;
}

CPlayerSkill::CPlayerSkill(_In_ DWORD dwObjAddr)
{
	_dwNodeBase = dwObjAddr;
	SetName();
}

VOID CPlayerSkill::SetName()
{
	_wsName = MyTools::CCharacter::ASCIIToUnicode(reinterpret_cast<CONST CHAR*>(ReadDWORD(ReadDWORD(GetNodeBase() + 0x78) + 0x4) + 0x2));
}

DWORD CPlayerSkill::GetSkillId() CONST
{
	return ReadDWORD(GetNodeBase() + 0x20);
}

CPetSkill::CPetSkill() : _uIndex(NULL)
{
	_dwNodeBase = NULL;
}

CPetSkill::CPetSkill(_In_ DWORD dwObjAddr, _In_ UINT uIndex)
{
	_dwNodeBase = dwObjAddr;
	_uIndex = uIndex;
}

VOID CPetSkill::SetName()
{
	auto pszName = MyTools::InvokeClassPtr<CGameCALL>()->GetPetName_By_SkillId(GetSkillId());
	if (pszName != nullptr)
		_wsName = MyTools::CCharacter::ASCIIToUnicode(std::string(pszName));
}

DWORD CPetSkill::GetSkillId() CONST
{
	return ReadDWORD(GetNodeBase() + 0x80 + _uIndex * 4);
}

DWORD CPetSkill::GetId() CONST
{
	return ReadDWORD(GetNodeBase() + C_chong_ID);
}
