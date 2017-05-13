#include "stdafx.h"
#include "PersonPet.h"
#include <MyTools/Character.h>
#include "ExcuteAction.h"
#include "GameCALL.h"

CPersonPet::CPersonPet()
{
	_dwNodeBase = NULL;
}

CPersonPet::CPersonPet(_In_ DWORD dwNodeBase)
{
	_dwNodeBase = dwNodeBase;
	SetName();
}

VOID CPersonPet::SetName()
{
	_wsName = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CONST CHAR*>(GetNodeBase() + C_chong_name)));
}

DWORD CPersonPet::GetLoyalty() CONST
{
	return ReadDWORD(GetNodeBase() + C_chong_zhongcheng);
}

BOOL CPersonPet::IsJoinWar() CONST
{
	return (ReadDWORD(GetNodeBase() + C_chong_chuzhan) & 0xFF) > 0 ? TRUE : FALSE;
}

DWORD CPersonPet::GetHp() CONST
{
	return ReadDWORD(GetNodeBase() + C_chong_DQHP);
}

DWORD CPersonPet::GetMaxHp() CONST
{
	return ReadDWORD(GetNodeBase() + C_chong_MHP);
}

DWORD CPersonPet::GetPercentHp() CONST
{
	float fMaxHp = static_cast<float>(GetMaxHp());
	if (fMaxHp == 0)
		return 0;

	return static_cast<DWORD>(static_cast<float>(GetHp()) / fMaxHp * 100);
}

DWORD CPersonPet::GetMp() CONST
{
	return ReadDWORD(GetNodeBase() + C_chong_DQMP);
}

DWORD CPersonPet::GetMaxMp() CONST
{
	return ReadDWORD(GetNodeBase() + C_chong_MMP);
}

DWORD CPersonPet::GetPercentMp() CONST
{
	float fMaxMp = static_cast<float>(GetMaxMp());
	if (fMaxMp == 0)
		return 0;

	return static_cast<DWORD>(static_cast<float>(GetMp()) / fMaxMp * 100);
}

DWORD CPersonPet::GetId() CONST
{
	return ReadDWORD(GetNodeBase() + C_chong_ID);
}

VOID CPersonPet::EnterWar() CONST
{
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([this] 
	{
		MyTools::InvokeClassPtr<CGameCALL>()->EnterWar(GetId());
	});
}

