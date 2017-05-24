#ifndef __DHSH_GAMESERVER_SOCKET_GAMESERVER_H__
#define __DHSH_GAMESERVER_SOCKET_GAMESERVER_H__

#include <MyTools/CLServer.h>
#include <MyTools/ClassInstance.h>
#include <MyTools/SocketClient.h>
#include <MyTools/SocketBuffer.h>

class CGameClient;
class CGameServer final : public MyTools::CLServer, public MyTools::CClassInstance<CGameServer>
{
public:
	CGameServer();
	~CGameServer();

	BOOL RunPrintThread();
protected:
	/////Virtual Method/////////////////////////////////////////////////////////////////////
	virtual std::shared_ptr<MyTools::CLSocketClient> GetNewClient(_In_ SOCKET ClientSock) override;

	virtual VOID DeleteClientPtr(_In_ MyTools::CLSocketClient* pSocketClientPtr) override;

	virtual VOID EchoRecive(_In_ std::shared_ptr<MyTools::CLSocketClient> pSocketClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) override;

	virtual BOOL FindClientSharePtr(_In_ MyTools::CLSocketClient* pSocketClient, _Out_ std::shared_ptr<MyTools::CLSocketClient>& SocketClientPtr) override;

private:
	static DWORD WINAPI _WorkThread(LPVOID lpParam);
protected:
	MyTools::CLLock _MapLock;

	std::map<MyTools::CLSocketClient*, std::shared_ptr<CGameClient>> _MapClient;
};



#endif // !__DHSH_GAMESERVER_SOCKET_GAMESERVER_H__
