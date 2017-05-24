#ifndef __DHSH_GAMESERVER_ECHO_ECHOPACKET
#define __DHSH_GAMESERVER_ECHO_ECHOPACKET

#include <MyTools/ClassInstance.h>
#include <MyTools/SocketBuffer.h>

class CGameClient;
class CAccount;
class CEchoPacket : public MyTools::CClassInstance<CEchoPacket>
{
public:
	struct VersionContent
	{
		std::wstring wsGameName;
		float fVersion;
	};
public:
	CEchoPacket();
	~CEchoPacket() = default;

	// Login
	BOOL AccountLogin(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	BOOL GameLogin(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// Register
	BOOL Register(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// KeepLive
	BOOL KeepALive(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;
	
	// QueryCard
	BOOL QueryAccount(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// RechargeCard
	BOOL RechargeCard(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// Read Config
	BOOL ReadConfig(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// Write config
	BOOL WriteConfig(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// Query Time
	BOOL QueryTime(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 检查登录
	BOOL CheckIsLogin(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST;

	// 获取本地的版本号
	BOOL ReadLocalVersion();

	// 检查版本号
	BOOL CheckVersion(_In_ CONST std::wstring& wsGameName, _In_ float fVersion) CONST;

private:
	// 解包
	template<typename T>
	static T ExtractPacket(_In_ std::function<T()> MethodPtr)
	{
		return std::move(MethodPtr());
	}

private:
	std::vector<VersionContent> _VecVersion;
	MyTools::CLLock _LockVersion;
};



#endif // !__DHSH_GAMESERVER_ECHO_ECHOPACKET
