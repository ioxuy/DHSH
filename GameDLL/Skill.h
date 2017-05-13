#ifndef __DHSH_GAMEDLL_GAME_OBJECT_SKILL_SKILL_H__
#define __DHSH_GAMEDLL_GAME_OBJECT_SKILL_SKILL_H__

#include "BaseObject.h"

class CPlayerSkill : public CBaseObject
{
public:
	CPlayerSkill();
	explicit CPlayerSkill(_In_ DWORD dwObjAddr);
	~CPlayerSkill() = default;

	virtual VOID SetName();

	virtual DWORD GetSkillId() CONST;

private:
};

class CPetSkill : public CPlayerSkill
{
public:
	CPetSkill();
	explicit CPetSkill(_In_ DWORD dwObjAddr, _In_ UINT uIndex);
	~CPetSkill() = default;

	virtual VOID SetName();

	virtual DWORD GetSkillId() CONST;

	DWORD GetId() CONST;
private:
	UINT _uIndex;
};



#endif // !__DHSH_GAMEDLL_GAME_OBJECT_SKILL_SKILL_H__
