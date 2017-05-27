#include "AccountConfigExtend.h"
#include <future>
#include <MyTools/Character.h>
#include "DbManager.h"

CAccountConfigExtend::CAccountConfigExtend() : _Lock(L"CAccountConfigExtend._Lock")
{

}

BOOL CAccountConfigExtend::Initialize()
{
	std::vector<CDbManager::DbAccountConfig> Vec;
	if (!CDbManager::GetInstance().GetVecAccountConfog(Vec))
		return FALSE;

	_Lock.Access([&] 
	{
		for (CONST auto& itm : Vec)
		{
			std::wstring wsKey;
			_MapConfig.insert(std::make_pair(MyTools::CCharacter::FormatText(wsKey, L"%d-%s", itm.dwAccountId, itm.wsConfigName.c_str()), itm.wsConfigValue));;
		}
	});

	return TRUE;
}

BOOL CAccountConfigExtend::FindConfig(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsConfigName, _Out_ std::wstring& wsConfigValue) CONST
{
	BOOL bExist = FALSE;
	_Lock.Access([&] 
	{
		std::wstring wsKey;
		auto itr = _MapConfig.find(MyTools::CCharacter::FormatText(wsKey, L"%d-%s", dwAccountId, wsConfigName.c_str()));
		if (itr != _MapConfig.end())
		{
			wsConfigValue = itr->second;
			bExist = TRUE;
		}
	});

	return bExist;
}

VOID CAccountConfigExtend::WriteConfig(_In_ DWORD dwAccountId, _In_ CONST WriteConfigText& WriteConfigText_)
{
	_Lock.Access([&]
	{
		std::wstring wsKey;
		for (CONST auto& itm : WriteConfigText_.VecConfig)
		{
			auto itr = _MapConfig.find(MyTools::CCharacter::FormatText(wsKey, L"%d-%s", dwAccountId, itm.wsConfigName.c_str()));
			if (itr == _MapConfig.end())
				continue;

			if (itr->second == itm.wsConfigValue)
				continue;

			itr->second = itm.wsConfigValue;

			std::wstring wsSQL;
			CDbManager::GetInstance().AsyncExcuteSQL(MyTools::CCharacter::FormatText(wsSQL, L"exec [proc_SetAccountConfig] %d,N'%s',N'%s'", dwAccountId, itm.wsConfigName.c_str(), itm.wsConfigValue.c_str()));
		}
		
	});
}
