#include "AccountExtend.h"
#include <MyTools/CLPublic.h>
#include "DbManager.h"
#include "Account.h"

CAccountExtend::CAccountExtend() : _LockMapAccount(L"CAccountExtend._LockVecAccount")
{

}

BOOL CAccountExtend::Initialize()
{
	std::vector<CAccount> Vec;
	if (!CDbManager::GetInstance().GetAccount(Vec))
		return FALSE;

	_LockMapAccount.Access([&] 
	{
		_MapAccount.clear();
		std::for_each(Vec.begin(), Vec.end(), [&](CAccount& itm)
		{ 
			_MapAccount.insert(std::make_pair(itm.GetAccountName(), std::make_shared<CAccount>(itm))); 
		});
	});
	return TRUE;
}

BOOL CAccountExtend::FindAccount_By_AccountName_And_Action(_In_ CONST std::wstring& wsAccountName, _In_ std::function<VOID(std::shared_ptr<CAccount>)> ActionPtr) CONST
{
	BOOL bRetCode = FALSE;
	_LockMapAccount.Access([&]
	{
		auto itr = _MapAccount.find(wsAccountName);
		if (itr != _MapAccount.end())
		{
			if (ActionPtr != nullptr)
				ActionPtr(itr->second);

			bRetCode = TRUE;
		}
	});

	return bRetCode;
}
