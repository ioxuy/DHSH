#include "GameServer.h"
#include <future>
#include <MyTools/Log.h>
#include "GameClient.h"
#include "EchoPacket.h"
#include "../GameDLL/GameBase.h"

#define _SELF L"GameServer.cpp"
CGameServer::CGameServer() : _MapLock(L"_MapLock")
{

}

std::shared_ptr<MyTools::CLSocketClient> CGameServer::GetNewClient(_In_ SOCKET ClientSock)
{
	auto itm = std::make_shared<CGameClient>(ClientSock);
	_MapLock.Access([&] {_MapClient.insert(std::make_pair(itm.get(), itm)); });
	return itm;
}

VOID CGameServer::DeleteClientPtr(_In_ MyTools::CLSocketClient* pSocketClientPtr)
{
	_MapLock.Access([&]
	{
		auto itr = _MapClient.find(pSocketClientPtr);
		if (itr != _MapClient.end())
		{
			_MapClient.erase(itr);
		}
	});
}

VOID CGameServer::EchoRecive(_In_ std::shared_ptr<MyTools::CLSocketClient> pSocketClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer)
{
	em_Sock_Msg emSockMsg;
	auto Error = pSocketBuffer->VerDataPtr(emSockMsg); // Clean 
	if (Error)
	{
		LOG_CF_E(L"VerData Faild. Error=%d", Error);
		return;
	}

	auto pGameClient = dynamic_cast<CGameClient*>(pSocketClient.get());
	if (emSockMsg != em_Sock_Msg::em_Sock_Msg_AccountLogin && emSockMsg != em_Sock_Msg::em_Sock_Msg_Register && emSockMsg != em_Sock_Msg::em_Sock_Msg_GameLogin)
	{
		// 
		if (!CEchoPacket::GetInstance().CheckIsLogin(pGameClient, pSocketBuffer))
		{
			PostSend(pSocketClient, pSocketBuffer);
			return;
		}
	}

	switch (emSockMsg)
	{
	case em_Sock_Msg_KeepLive:
		CEchoPacket::GetInstance().KeepALive(pGameClient, pSocketBuffer);
		break;
	case em_Sock_Msg_QueryCard:
		CEchoPacket::GetInstance().QueryAccount(pGameClient, pSocketBuffer);
		break;
	case em_Sock_Msg_Recharge:
		CEchoPacket::GetInstance().RechargeCard(pGameClient, pSocketBuffer);
		break;
	case em_Sock_Msg_ReadConfig:
		CEchoPacket::GetInstance().ReadConfig(pGameClient, pSocketBuffer);
		break;
	case em_Sock_Msg_WriteConfig:
		CEchoPacket::GetInstance().WriteConfig(pGameClient, pSocketBuffer);
		break;
	case em_Sock_Msg_AccountLogin:
		CEchoPacket::GetInstance().AccountLogin(pGameClient, pSocketBuffer);
		break;
	case em_Sock_Msg_GameLogin:
		CEchoPacket::GetInstance().GameLogin(pGameClient, pSocketBuffer);
		break;
	case em_Sock_Msg_Register:
		CEchoPacket::GetInstance().Register(pGameClient, pSocketBuffer);
		break;
	case em_Sock_Msg_QueryHour:
		CEchoPacket::GetInstance().QueryTime(pGameClient, pSocketBuffer);
		break;
	default:
		break;
	}

	if (pSocketBuffer->size() > 0)
	{
		PostSend(pSocketClient, pSocketBuffer);
	}
}

BOOL CGameServer::FindClientSharePtr(_In_ MyTools::CLSocketClient* pSocketClient, _Out_ std::shared_ptr<MyTools::CLSocketClient>& SocketClientPtr)
{
	BOOL bExist = FALSE;
	_MapLock.Access([&]
	{
		auto itr = _MapClient.find(pSocketClient);
		if (itr != _MapClient.end())
		{
			bExist = TRUE;
			SocketClientPtr = itr->second;
		}
	});
	return bExist;
}

DWORD WINAPI CGameServer::_WorkThread(LPVOID lpParam)
{
	auto pGameServer = static_cast<CGameServer *>(lpParam);

	for (;;)
	{
		::Sleep(30 * 1000);

		DWORD dwCount = 0;
		pGameServer->_MapLock.Access([pGameServer, &dwCount]
		{
			for (CONST auto& itm : pGameServer->_MapClient)
			{
				if (itm.second->GetAccount() != nullptr)
				{
					//BOOL bOnLine = itm.second->IsOnLine();
					//BOOL bExistClient = itm.second->IsExistClient();
				}
				if (!itm.second->IsExistClient())
					continue;
				else if (itm.second->IsOnLine())
				{
					dwCount += 1;
					continue;
				}

				std::async(std::launch::async, &CGameServer::DisClientConnect, pGameServer, itm.second.get());
			}
				
		});

		LOG_CF_D(L"1分钟在线数量=%d个", dwCount);
	}
	return 0;
}

BOOL CGameServer::RunPrintThread()
{
	cbBEGINTHREADEX(NULL, NULL, _WorkThread, this, NULL, NULL);
	return TRUE;
}

CGameServer::~CGameServer()
{

}

VOID CGameServer::DisClientConnect(_In_ MyTools::CLSocketClient* pSocketClientPt)
{
	pSocketClientPt->DisConnect();
}
