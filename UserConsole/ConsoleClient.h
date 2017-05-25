#ifndef __DHSH_USERCONSOLE_CODE_SOCKET_CONSOLECLIENT_H__
#define __DHSH_USERCONSOLE_CODE_SOCKET_CONSOLECLIENT_H__

#include <MyTools/CLClient.h>
#include <MyTools/ClassInstance.h>
#include <MyTools/SocketBuffer.h>

class CConsoleClient : public MyTools::CLClient, public MyTools::CClassInstance<CConsoleClient>
{
public:
	CConsoleClient();
	~CConsoleClient() = default;

	// 异步发送
	BOOL AsyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer);


	// 同步发送(等待接收消息)
	BOOL SyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer, std::function<VOID(MyTools::CLSocketBuffer& SocketBuffer)> ExtractPtr);


	VOID SetEchoErrorPtr(_In_ std::function<VOID(VOID)> EchoErrorPtr);
protected:

	// 处理收包
	virtual VOID EchoPacket(_In_ MyTools::CLSocketBuffer& SocketBuffer) override;


	// 与服务器断开连接回调
	virtual VOID DisConnectServer() CONST override;

private:
	VOID EchoError() CONST;
private:
	DWORD _dwMsgHead;
	HANDLE _hEvent;
	std::function<VOID(MyTools::CLSocketBuffer& SocketBuffer)> _ExtractActionPtr;
	std::function<VOID(VOID)> _EchoErrorPtr;
};



#endif // !__DHSH_USERCONSOLE_CODE_SOCKET_CONSOLECLIENT_H__
