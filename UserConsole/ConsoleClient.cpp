#include "stdafx.h"
#include "ConsoleClient.h"
#include "../GameDLL/GameBase.h"
#include <MyTools/Log.h>

#define _SELF L"ConsoleClient.cpp"
CConsoleClient::CConsoleClient() : _ExtractActionPtr(nullptr), _EchoErrorPtr(nullptr), _dwMsgHead(NULL)
{
	_hEvent = INVALID_HANDLE_VALUE;
}


BOOL CConsoleClient::AsyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer)
{
	PostSend(SocketBuffer);
	return TRUE;
}

BOOL CConsoleClient::SyncSend(_In_ MyTools::CLSocketBuffer& SocketBuffer, std::function<VOID(MyTools::CLSocketBuffer& SocketBuffer)> ExtractPtr)
{
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

VOID CConsoleClient::SetEchoErrorPtr(_In_ std::function<VOID(VOID)> EchoErrorPtr)
{
	_EchoErrorPtr = EchoErrorPtr;
}

VOID CConsoleClient::EchoPacket(_In_ MyTools::CLSocketBuffer& SocketBuffer)
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
		LOG_CF_E(L"服务器回发了一个包,Head=%X", emSockMsg);
		return;
	}

	// Exctrcat Packet
	_ExtractActionPtr(SocketBuffer);
	::SetEvent(_hEvent);
}

VOID CConsoleClient::DisConnectServer() CONST
{
	AfxMessageBox(L"与服务器失去连接……");
	EchoError();
}

VOID CConsoleClient::EchoError() CONST
{
	if (_EchoErrorPtr != nullptr)
		_EchoErrorPtr();
}

