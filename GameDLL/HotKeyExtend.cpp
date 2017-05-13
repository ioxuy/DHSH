#include "stdafx.h"
#include "HotKeyExtend.h"
#include <MyTools/Character.h>
#include "HotKey.h"

UINT CHotKeyExtend::GetVecHotKey(_Out_ std::vector<CHotKey>& Vec) CONST
{
	DWORD dwRoot = ReadDWORD(C_base_kuaijie);
	for (int i = 0;i < 0x5A; ++i)
	{
		CHotKey HotKey_(dwRoot + i * 0x2C);
		Vec.push_back(std::move(HotKey_));
	}
	return Vec.size();
}
