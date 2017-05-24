#include "GameClient.h"
#include "Account.h"

CGameClient::CGameClient(_In_ SOCKET Sock) : MyTools::CLSocketClient(Sock), _Account(nullptr), _dwToken(NULL)
{
	_ulKeepALiveTick = NULL;
}

VOID CGameClient::Remove() CONST
{
	// 将Toek标志位-1
	_Account->SetTokenCount(CAccount::em_Token_Action::em_Token_Action_Dec);
}

VOID CGameClient::Add()
{

}

VOID CGameClient::SetKeepALiveTick() _NOEXCEPT
{
	_ulKeepALiveTick = ::GetTickCount64();
}

BOOL CGameClient::IsOnLine() CONST _NOEXCEPT
{
	return _ulKeepALiveTick != NULL && (::GetTickCount64() - _ulKeepALiveTick >= 30 * 1000);
}

VOID CGameClient::SetIP(_In_ CONST std::wstring& wsClientIP) _NOEXCEPT
{
	_wsClientIP = wsClientIP;
}

CONST std::wstring& CGameClient::GetIP() CONST _NOEXCEPT
{
	return _wsClientIP;
}

std::shared_ptr<CAccount>&  CGameClient::GetAccount() _NOEXCEPT
{
	return _Account;
}

VOID CGameClient::SetToken(_In_ DWORD ulToken) _NOEXCEPT
{
	_dwToken = ulToken;
}

DWORD CGameClient::GetToken() CONST _NOEXCEPT
{
	return _dwToken;
}
