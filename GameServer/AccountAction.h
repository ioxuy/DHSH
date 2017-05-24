#ifndef __DHSH_GAMESERVER_ACCOUNT_ACCOUNTACTION_H__
#define __DHSH_GAMESERVER_ACCOUNT_ACCOUNTACTION_H__

#include <MyTools/ClassInstance.h>

class CAccount;
class CAccountAction : public MyTools::CClassInstance<CAccountAction>
{
public:
	CAccountAction() = default;
	~CAccountAction() = default;

	BOOL Login(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass, _Out_ std::shared_ptr<CAccount>& Account) CONST;
private:

};



#endif // !__DHSH_GAMESERVER_ACCOUNT_ACCOUNTACTION_H__
