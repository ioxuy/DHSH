#include "stdafx.h"
#include "PersonPetExtend.h"
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#include "PersonPet.h"

#define _SELF L"PersonPetExtend.cpp"

UINT CPersonPetExtend::GetVecPet(_Out_ std::vector<CPersonPet>& Vec, _In_ std::function<BOOL(CONST CPersonPet&)> FilterPtr) CONST
{
	DWORD dwArrayHead = ReadDWORD(ReadDWORD(C_chong_base) + C_chong_yiji);
	DWORD dwArrayEnd = ReadDWORD(ReadDWORD(C_chong_base) + C_chong_shu);
	if (dwArrayEnd <= dwArrayHead)
		return 0;

	int nCount = static_cast<int>(dwArrayEnd - dwArrayHead) >> 0x2;
	for (int i = 0;i < nCount; ++i)
	{
		CPersonPet PersonPet(ReadDWORD(dwArrayHead + i * 4));
		if(FilterPtr == nullptr)
			Vec.push_back(std::move(PersonPet));
		else if (FilterPtr(PersonPet))
		{
			Vec.push_back(std::move(PersonPet));
			break;
		}
	}
	return Vec.size();
}

BOOL CPersonPetExtend::Action_By_JoinWarPet(_In_ std::function<VOID(CONST CPersonPet&)> ActionPtr) CONST
{
	std::vector<CPersonPet> Vec;
	if (GetVecPet(Vec, [](CONST CPersonPet& PersonPet) { return PersonPet.IsJoinWar(); }) == NULL)
		return FALSE;
	
	if(ActionPtr != nullptr)
		ActionPtr(*Vec.begin());
	return TRUE;
}

