#ifndef __DHSH_GAMESERVER_ACCOUNT_ACCOUNTEXTEND_H__
#define __DHSH_GAMESERVER_ACCOUNT_ACCOUNTEXTEND_H__

#include <MyTools/ClassInstance.h>
#include <MyTools/CLLock.h>

class CAccount;
class CAccountExtend : public MyTools::CClassInstance<CAccountExtend>
{
public:
	CAccountExtend();
	~CAccountExtend() = default;

	// Init
	BOOL Initialize();

	//
	BOOL FindAccount_By_AccountName_And_Action(_In_ CONST std::wstring& wsAccountName, _In_ std::function<VOID(std::shared_ptr<CAccount>)> ActionPtr) CONST;
private:
	std::map<std::wstring,std::shared_ptr<CAccount>> _MapAccount;
	MyTools::CLLock _LockMapAccount;
};



#endif // !__DHSH_GAMESERVER_ACCOUNT_ACCOUNTEXTEND_H__
