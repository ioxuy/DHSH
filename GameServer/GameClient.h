#ifndef __DHSH_GAMESERVER_CLIENT_GAMECLIENT_H__
#define __DHSH_GAMESERVER_CLIENT_GAMECLIENT_H__

#include <MyTools/SocketClient.h>

class CAccount;
class CGameClient : public MyTools::CLSocketClient
{
public:
	CGameClient(_In_ SOCKET Sock);
	~CGameClient() = default;

	virtual VOID Remove() CONST override;

	virtual VOID Add() override;

public:
	// 设置心跳中……
	VOID SetKeepALiveTick() _NOEXCEPT;

	// 判断是否在线
	virtual BOOL IsOnLine() CONST _NOEXCEPT override;

	// 设置当前登录的IP
	VOID SetIP(_In_ CONST std::wstring& wsClientIP) _NOEXCEPT;

	// 获取当前的IP
	CONST std::wstring& GetIP() CONST _NOEXCEPT;

	// 获取Account
	std::shared_ptr<CAccount>&  GetAccount() _NOEXCEPT;

	VOID SetToken(_In_ DWORD ulToken) _NOEXCEPT;

	DWORD GetToken() CONST _NOEXCEPT;

	BOOL IsExistClient() CONST _NOEXCEPT;
private:
	std::shared_ptr<CAccount> _Account;
	ULONGLONG                 _ulKeepALiveTick;
	std::wstring              _wsClientIP;
	DWORD		              _dwToken;
};



#endif // !__DHSH_GAMESERVER_CLIENT_GAMECLIENT_H__
