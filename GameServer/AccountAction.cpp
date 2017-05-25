#include "AccountAction.h"
#include "Account.h"
#include "AccountExtend.h"

BOOL CAccountAction::Login(_In_ CONST std::wstring& wsAccountName, _In_ CONST std::wstring& wsAccountPass, _Out_ std::shared_ptr<CAccount>& Account) CONST
{
	BOOL bExist = FALSE;
	CAccountExtend::GetInstance().FindAccount_By_AccountName_And_Action(wsAccountName, [wsAccountPass, &Account, &bExist](std::shared_ptr<CAccount> itm)
	{
		if (itm->GetAccountPass() == wsAccountPass)
		{
			Account = itm;
			bExist = TRUE;
		}
	});
	return bExist;
}
