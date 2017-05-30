#include "stdafx.h"
#include "RunGame.h"
#include <MyTools/CLProcess.h>
#include <MyTools/Character.h>
#include <MyTools/CLNoModuleDLL.h>

CRunGame::CRunGame()
{
	_hFileMap = NULL;
	_lpGameDLLMemory = NULL;
	_hInjectorFileMap = NULL;
	_lpInjectorDLLMemory = NULL;
}

CRunGame::~CRunGame()
{
	if (_hFileMap != NULL)
	{
		::UnmapViewOfFile(_lpGameDLLMemory);
		::CloseHandle(_hFileMap);
		_hFileMap = NULL;
	}
	if (_hInjectorFileMap != NULL)
	{
		::UnmapViewOfFile(_lpInjectorDLLMemory);
		::CloseHandle(_hInjectorFileMap);
		_hInjectorFileMap = NULL;
	}
}

BOOL CRunGame::Run()
{
	//return ExistGameProtectt() ? RunMainGame() : RunGameProtect();
	return RunGameProtect();
}

BOOL CRunGame::InitializeGameDLLMemory(_In_ LPVOID lpGameDLLContent, _In_ UINT uSize)
{
	_hFileMap = ::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, 10 * 1024 * 1024, L"CL_SHARE_DHSH_GAMEDLL");
	_lpGameDLLMemory = ::MapViewOfFile(_hFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, 10 * 1024 * 1024);
	ZeroMemory(_lpGameDLLMemory, 10 * 1024 * 1024);

	*reinterpret_cast<DWORD *>(_lpGameDLLMemory) = uSize;
	memcpy(reinterpret_cast<CHAR*>(_lpGameDLLMemory) + 4, lpGameDLLContent, uSize);
	return TRUE;
}

BOOL CRunGame::InitializeInjectorDLLMemory(_In_ LPVOID lpInjectorDLLContent, _In_ UINT uSize)
{
	_hInjectorFileMap = ::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, 10 * 1024 * 1024, L"CL_SHARE_DHSH_INJECTORDLL");
	_lpInjectorDLLMemory = ::MapViewOfFile(_hInjectorFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, 10 * 1024 * 1024);
	ZeroMemory(_lpInjectorDLLMemory, 10 * 1024 * 1024);

	*reinterpret_cast<DWORD *>(_lpInjectorDLLMemory) = uSize;
	memcpy(reinterpret_cast<CHAR*>(_lpInjectorDLLMemory) + 4, lpInjectorDLLContent, uSize);
	return TRUE;
}

BOOL CRunGame::ExistGameProtectt() CONST
{
	return MyTools::CLProcess::Is_Exist_Process_For_ProcName(L"CProtect1.exe");
}

BOOL CRunGame::RunGameProtect() CONST
{
	STARTUPINFOW		si = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION pi = { 0 };

	WCHAR				wszProcPath[MAX_PATH] = { 0 };
	WCHAR				wszWorkDirectory[MAX_PATH] = { 0 };

	MyTools::CCharacter::wstrcpy_my(wszWorkDirectory, CConsoleVariable::GetInstance().GetSareContent()->wszGamePath);
	MyTools::CCharacter::wstrcpy_my(wszProcPath, GetGameProcPath().c_str());

	if (::CreateProcessW(NULL, wszProcPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, wszWorkDirectory, &si, &pi))
	{
		UINT uSize = *reinterpret_cast<DWORD*>(_lpInjectorDLLMemory);

		MyTools::CLNoModuleDLL NoModuleDLL;
		BOOL bRet = NoModuleDLL.RemoteLoadMemoryDLL(reinterpret_cast<CHAR*>(_lpInjectorDLLMemory) + 4, uSize, pi.hProcess, pi.hThread);
		::ResumeThread(pi.hThread);

		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
		return bRet;
	}

	return FALSE;
}

BOOL CRunGame::RunMainGame() CONST
{
	STARTUPINFOW		si = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION pi = { 0 };

	WCHAR				wszProcPath[MAX_PATH] = { 0 };
	WCHAR				wszWorkDirectory[MAX_PATH] = { 0 };

	MyTools::CCharacter::wstrcpy_my(wszWorkDirectory, CConsoleVariable::GetInstance().GetSareContent()->wszGamePath);
	MyTools::CCharacter::wstrcpy_my(wszProcPath, GetGameProcPath().c_str());

	if (::CreateProcessW(NULL, wszProcPath, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, wszWorkDirectory, &si, &pi))
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
		return TRUE;
	}

	return FALSE;
}

CONST std::wstring& CRunGame::GetGameProcPath() CONST
{
	static std::wstring wsProcPath;
	if (wsProcPath.empty())
		MyTools::CCharacter::FormatText(wsProcPath, L"%s\\CProtect1.exe Game.exe A89DD7AD dhsh1 CYOU Space zy@fs", CConsoleVariable::GetInstance().GetSareContent()->wszGamePath);

	return wsProcPath;
}
