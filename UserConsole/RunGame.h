#ifndef __DHSH_USERCONSOLE_CODE_RUNGAME_RUNGAME_H__
#define __DHSH_USERCONSOLE_CODE_RUNGAME_RUNGAME_H__

#include "ConsoleVariable.h"

class CRunGame
{
public:
	CRunGame();
	~CRunGame();

	BOOL Run();

	BOOL InitializeGameDLLMemory(_In_ LPVOID lpGameDLLContent, _In_ UINT uSize);

	BOOL InitializeInjectorDLLMemory(_In_ LPVOID lpInjectorDLLContent, _In_ UINT uSize);
private:
	//
	BOOL ExistGameProtectt() CONST;

	// 
	BOOL RunGameProtect() CONST;

	//
	BOOL RunMainGame() CONST;

	//
	CONST std::wstring& GetGameProcPath() CONST;
private:
	//////////////////////////////////////////////////////////////////////////
	HANDLE _hFileMap;
	LPVOID _lpGameDLLMemory;

	//////////////////////////////////////////////////////////////////////////
	HANDLE _hInjectorFileMap;
	LPVOID _lpInjectorDLLMemory;
};


#endif // !__DHSH_USERCONSOLE_CODE_RUNGAME_RUNGAME_H__
