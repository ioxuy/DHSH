#ifndef __DHSH_GAMESERVER_CONFIG_ACCOUNTCONFIGEXTEND_H__
#define __DHSH_GAMESERVER_CONFIG_ACCOUNTCONFIGEXTEND_H__

#include <MyTools/ClassInstance.h>
#include <MyTools/CLLock.h>


class CAccountConfigExtend : public MyTools::CClassInstance<CAccountConfigExtend>
{
public:
	struct AccountConfig
	{
		std::wstring wsConfigName;
		std::wstring wsConfigValue;
	};
public:
	CAccountConfigExtend();
	~CAccountConfigExtend() = default;

	// 
	BOOL Initialize();

	// 
	BOOL FindConfig(_In_ DWORD dwAccountId, _In_ CONST std::wstring& wsConfigName, _Out_ std::wstring& wsConfigValue) CONST;

	//
	VOID WriteConfig(_In_ DWORD dwAccountId, _In_ CONST std::vector<AccountConfig>& Vec);
private:
	MyTools::CLLock        _Lock;
	std::map<std::wstring, std::wstring> _MapConfig;
};



#endif // !__DHSH_GAMESERVER_CONFIG_ACCOUNTCONFIGEXTEND_H__
