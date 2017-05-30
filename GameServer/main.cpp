#include <iostream>
#include <MyTools/Log.h>
#include "AccountExtend.h"
#include "AccountConfigExtend.h"
#include "DbManager.h"
#include "EchoPacket.h"
#include "GameServer.h"

#define _SELF L"main.cpp"

int main()
{
	CONST static std::wstring wsMutexName = L"CL_GAMESERVER";
	if (OpenMutexW(MUTEX_ALL_ACCESS, FALSE, wsMutexName.c_str()) != NULL)
	{
		std::wcout << L"Server Already Exist!" << std::endl;
		::Sleep(3000);
		return 0;
	}
	::CreateMutexW(NULL, FALSE, wsMutexName.c_str());

	MyTools::CLog::GetInstance().SetClientName(L"GameServer", L"C:\\", FALSE, 100 * 1024 * 1024);
	if (!CAccountExtend::GetInstance().Initialize())
	{
		std::wcout << L"CAccountExtend::Initialize Faild!" << std::endl;
		goto lb_Exit;
	}
	if (!CAccountConfigExtend::GetInstance().Initialize())
	{
		std::wcout << L"CAccountConfigExtend::Initialize Faild!" << std::endl;
		goto lb_Exit;
	}
	if (!CEchoPacket::GetInstance().ReadLocalVersion())
	{
		std::wcout << L"CEchoPacket::ReadLocalVersion Faild!" << std::endl;
		goto lb_Exit;
	}

	CGameServer::GetInstance().Run(12345, 3000);
	CGameServer::GetInstance().RunPrintThread();
	std::wcout << L"Runing..." << std::endl;
lb_Exit:;
	::Sleep(INFINITE);
	return 0;
}