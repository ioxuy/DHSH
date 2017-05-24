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
	if (!CDbManager::GetInstance().RunThread())
	{
		std::wcout << L"CDbManager::RunThread Faild!" << std::endl;
		goto lb_Exit;
	}
	if (!CEchoPacket::GetInstance().ReadLocalVersion())
	{
		std::wcout << L"CEchoPacket::ReadLocalVersion Faild!" << std::endl;
		goto lb_Exit;
	}

	CGameServer::GetInstance().Run(12345, 3000);
	std::wcout << L"Runing..." << std::endl;
lb_Exit:;
	system("pause");
	return 0;
}