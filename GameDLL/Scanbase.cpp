#include "stdafx.h"
#include "Scanbase.h"
#include <MyTools/TimeTick.h>
#include <MyTools/CLSearchBase.h>
#include <MyTools/Log.h>

#define _SELF L"ScanBase.cpp"
VOID CScanBase::Start() CONST
{
	DWORD dwBase = 0x0;
	DWORD dwCALL = 0x0;
	DWORD dwAddr = 0x0;
	DWORD dwClient = (DWORD)::GetModuleHandleW(L"Game.exe");


	MyTools::CTimeTick TimeTick;

	dwCALL = MyTools::CLSearchBase::FindCALL("8B??????????85??74??E8????????8B??????????F7", 0x45AB80 - 0x45AB9B, dwClient, 1, 0, L"Game.exe");
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	自动走路CALL	0x%X", dwCALL);

	dwBase = MyTools::CLSearchBase::FindBase("8BC466C700E0FF8B4424", 0x47FB0B - 0x47FB22, 0x2, 0, L"Game.exe");
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	NPC选项文本偏移	0x%X", dwBase);

	dwBase = MyTools::CLSearchBase::FindBase("8BC466C700E0FF8B4424", 0x47FB0B - 0x47FB2A, 0x2, 0, L"Game.exe", 0xFF);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	NPC选项发包偏移	0x%X", dwBase);

	dwBase = MyTools::CLSearchBase::FindBase_ByCALL("E8????????83??3189", 0x65151E - 0x651544, dwClient, 0x1, 0, L"Game.exe", 0x4DDC15 - 0x4DDBC0 + 0x1);
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	家园基址	0x%X", dwBase);

	dwBase = MyTools::CLSearchBase::FindBase("6A016A018B??FF????E9????????????6A", 0x4DE09F - 0x4DE0C0, 0x2, 0, L"Game.exe");
	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	家园偏移	0x%X", dwBase);

	MyTools::CLog::GetInstance().Print(__FUNCTIONW__, _SELF, 0, LOG_TYPE_CONSOLE, MyTools::CLog::em_Log_Type_Debug, FALSE, L"#define	家园活力偏移	0x2B");

	LOG_CF_D(L"SpendTime/s = %d", static_cast<DWORD>(TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second)));
}
