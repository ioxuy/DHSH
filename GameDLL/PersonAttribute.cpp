#include "stdafx.h"
#include "PersonAttribute.h"
#include <MyTools/Character.h>

CONST std::wstring CPersonAttribute::GetCurrentMapName() CONST
{
	return MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CONST CHAR*>(ReadDWORD(C_map_base) + C_map_name)));
}

DWORD CPersonAttribute::GetLevel() CONST
{
	return ReadDWORD(ReadDWORD(C_renwu_base) + C_renwu_lev);
}

DWORD CPersonAttribute::GetHp() CONST
{
	return ReadDWORD(ReadDWORD(C_renwu_base) + C_renwu_DQHP);
}

DWORD CPersonAttribute::GetMaxHp() CONST
{
	return ReadDWORD(ReadDWORD(C_renwu_base) + C_renwu_MHP);
}

DWORD CPersonAttribute::GetPercentHp() CONST
{
	return GetMaxHp() == 0 ? 0 : (GetHp() * 100 / GetMaxHp());
}

DWORD CPersonAttribute::GetMp() CONST
{
	return ReadDWORD(ReadDWORD(C_renwu_base) + C_renwu_DQMP);
}

DWORD CPersonAttribute::GetMaxMp() CONST
{
	return ReadDWORD(ReadDWORD(C_renwu_base) + C_renwu_MMP);
}

DWORD CPersonAttribute::GetPercentMp() CONST
{
	return GetMaxMp() == 0 ? 0 : (GetMp() * 100 / GetMaxMp());
}

DWORD CPersonAttribute::GetExp() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(C_renwu_base) + C_EXP + 0x4) + 0x25C);
}

BOOL CPersonAttribute::IsWar() CONST
{
	return (ReadDWORD(ReadDWORD(C_base_dianguai) + 0x0) & 0xFF) != NULL;
}

Point CPersonAttribute::GetPoint() CONST
{
	DWORD dwAddr = ReadDWORD(ReadDWORD(C_map_base) + C_map_Addr);
	return Point(ReadDWORD(dwAddr + C_map_AddX) >> 0x4, ReadDWORD(dwAddr + C_map_AddY) >> 0x4);
}

DWORD CPersonAttribute::GetMoney() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(C_money_base) + C_money_yiji + C_money_erji) + 0) + C_money_siji) + C_money_wuji) + C_money_liuji) + 0);
}

DWORD CPersonAttribute::GetJiaoZiMoney() CONST
{
	return ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(C_money_base) + C_money_yiji + C_money_erji) + 0) + C_money_siji) + C_money_wuji) + C_money_liuji) + C_money_jiao);
}

UINT CPersonAttribute::GetVecPersonBuff(_Out_ std::vector<PersonBuff>& Vec, _In_ std::function<BOOL(CONST PersonBuff&)> FilterPtr) CONST
{
	DWORD dwBase = ReadDWORD(ReadDWORD(C_state_base) + C_state_yiji);
	DWORD dwHead = ReadDWORD(dwBase + C_state_erji);

	int nCount = static_cast<int>(ReadDWORD(dwBase + C_state_shu + C_state_erji));
	for (int i = 0;i < nCount; ++i)
	{
		PersonBuff PersonBuff_;
		PersonBuff_.dwObjAddr = dwHead + i * 0x98;
		PersonBuff_.wsName = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CHAR*>(ReadDWORD(PersonBuff_.dwObjAddr + 0x18))));
		PersonBuff_.dwSurpTime = ReadDWORD(PersonBuff_.dwObjAddr + C_state_surp1);
		if (FilterPtr(PersonBuff_))
			Vec.push_back(std::move(PersonBuff_));
	}
	return Vec.size();
}

BOOL CPersonAttribute::ExistPersonBuff_By_Name(_In_ CONST std::wstring& wsName) CONST
{
	std::vector<PersonBuff> Vec;
	return GetVecPersonBuff(Vec, [wsName](CONST PersonBuff& PersonBuff_) { return PersonBuff_.wsName == wsName; }) != NULL;
}
