#include "stdafx.h"
#include "GameClient.h"
#include <MyTools/Log.h>

#define _SELF L"GameClient.cpp"
CGameClient::CGameClient() : _ExtractActionPtr(nullptr), _EchoErrorPtr(nullptr), _dwMsgHead(NULL)
{
	_hEvent = INVALID_HANDLE_VALUE;
	_hKeepALiveThread = INVALID_HANDLE_VALUE;
}


CGameClient::~CGameClient()
{
	_bRun = FALSE;
	if (_hKeepALiveThread != INVALID_HANDLE_VALUE)
	{
		::WaitForSingleObject(_hKeepALiveThread, INFINITE);
		::CloseHandle(_hKeepALiveThread);
		_hKeepALiveThread = INVALID_HANDLE_VALUE;
	}
}

BOOL CGameClient::AsyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer)
{
	PostSend(SocketBuffer);
	return TRUE;
}

BOOL CGameClient::SyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer, std::function<VOID(MyTools::CLSocketBuffer& SocketBuffer)> ExtractPtr)
{
	std::lock_guard<std::mutex> LockMutex(_SendMutex);
	_dwMsgHead = SocketBuffer.GetMsgHead<DWORD>();

	_ExtractActionPtr = ExtractPtr;
	_hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	AsyncSend(SocketBuffer);

	if (::WaitForSingleObject(_hEvent, 30 * 1000) == WAIT_TIMEOUT)
	{
		::CloseHandle(_hEvent);
		_hEvent = INVALID_HANDLE_VALUE;
		return FALSE;
	}

	::CloseHandle(_hEvent);
	_hEvent = INVALID_HANDLE_VALUE;
	_ExtractActionPtr = nullptr;
	_dwMsgHead = NULL;
	return TRUE;
}

VOID CGameClient::SetEchoErrorPtr(_In_ std::function<VOID(VOID)> EchoErrorPtr)
{
	_EchoErrorPtr = EchoErrorPtr;
}

VOID CGameClient::RunKeepALive()
{
	if (_hKeepALiveThread == INVALID_HANDLE_VALUE)
	{
		_hKeepALiveThread = cbBEGINTHREADEX(NULL, NULL, CGameClient::_KeepALiveThread, this, NULL, NULL);
	}
}

BOOL CGameClient::GameLogin()
{
	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_GameLogin);

	auto& GlobalConfig = MyTools::InvokeClassPtr<CGameVariable>()->GetGameShareContent()->GlobalConfig;
	SocketBuffer << GlobalConfig.wszAccountName << GlobalConfig.wszAccountPass << GlobalConfig.dwToken;

	DWORD dwRetCode = FALSE;
	MyTools::InvokeClassPtr<CGameClient>()->SyncSend(SocketBuffer, [&dwRetCode](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> dwRetCode;
	});
	LOG_C_D(L"dwRetCode=%d", dwRetCode);
	return dwRetCode != 0;
}

VOID CGameClient::EchoPacket(_In_ MyTools::CLSocketBuffer& SocketBuffer)
{
	em_Sock_Msg emSockMsg;
	auto Error = SocketBuffer.VerDataPtr<em_Sock_Msg>(emSockMsg);
	if (Error != MyTools::CLSocketBuffer::em_SocketBuffer_Error::em_SocketBuffer_Error_None)
	{
		LOG_MSG_CF(L"解包错误, Error=%d", Error);
		EchoError();
		return;
	}

	if (emSockMsg == em_Sock_Msg::em_Sock_Msg_ServerText)
	{
		std::wstring wsText;
		SocketBuffer >> wsText;
		LOG_MSG_CF(L"服务器返回消息:%s", wsText.c_str());
		EchoError();
		return;
	}


	if (_hEvent == INVALID_HANDLE_VALUE || SocketBuffer.GetMsgHead<DWORD>() != _dwMsgHead)
	{
		//LOG_CF_E(L"服务器回发了一个包,Head=%X", emSockMsg);
		return;
	}

	// Exctrcat Packet
	_ExtractActionPtr(SocketBuffer);
	::SetEvent(_hEvent);
}

VOID CGameClient::DisConnectServer() CONST
{
	::MessageBoxW(NULL, L"与服务器失去连接……", L"Error", NULL);;
	EchoError();
}

VOID CGameClient::EchoError() CONST
{
	if (_EchoErrorPtr != nullptr)
		_EchoErrorPtr();
}

DWORD WINAPI CGameClient::_KeepALiveThread(LPVOID lpParam)
{
	auto pCGameClient = static_cast<CGameClient *>(lpParam);
	while (pCGameClient->_bRun)
	{
		MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_KeepLive);
		pCGameClient->AsyncSend(SocketBuffer);

		for (int i = 0; i < 5 && pCGameClient->_bRun; ++i)
			::Sleep(1000);
	}
	return 0;
}

