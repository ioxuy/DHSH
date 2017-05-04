// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLHook.h>
#include <MyTools/CLNoModuleDLL.h>
#include <MyTools/CLProcess.h>
#include "LdrHeader.h"

#define _SELF L"dllmain.cpp"

BOOL LdrLoadMemDll(PVOID DllMem, SIZE_T dllSize, LPCWSTR szDllName, HMODULE *pModule);

VOID LoadGameDLL(_In_ CONST LPPROCESS_INFORMATION lpProcessInformation)
{
	LOG_C_D(L"lpProcessInformation=%X", lpProcessInformation);
	if (lpProcessInformation != nullptr)
	{
		CLProcess::LoadRemoteDLL(lpProcessInformation->dwProcessId, L"C:\\Visual Studio 2013\\Projects\\DHSH\\Release\\InjectorDLL.dll");
		ResumeThread(lpProcessInformation->hThread);
	}

}

DWORD dwHookAddr = 0;
__declspec(naked) void HookCreateProcessA()
{
	static DWORD dwESP = 0;
	static BOOL bHook = FALSE;
	static LPCSTR lpApplicationName = NULL;
	static LPSTR lpCommandLine = NULL;
	static LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
	static LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
	static BOOL bInheritHandles = FALSE;
	static DWORD dwCreationFlags = NULL;
	static LPVOID lpEnvironment = NULL;
	static LPCSTR lpCurrentDirectory = NULL;
	static LPSTARTUPINFOA lpStartupInfo = NULL;
	static LPPROCESS_INFORMATION lpProcessInformation = NULL;

	if (bHook)
	{
		__asm PUSH EBP;
		__asm MOV EBP, ESP;
		__asm MOV EAX, dwHookAddr;
		__asm ADD EAX, 0x5;
		__asm JMP EAX;
	}
	else
	{
		__asm
		{
			MOV EAX, DWORD PTR DS : [ESP + 0x4];
			MOV lpApplicationName, EAX;

			MOV EAX, DWORD PTR DS : [ESP + 0x8];
			MOV lpCommandLine, EAX;

			MOV EAX, DWORD PTR DS : [ESP + 0xC];
			MOV lpProcessAttributes, EAX;

			MOV EAX, DWORD PTR DS : [ESP + 0x10];
			MOV lpThreadAttributes, EAX;

			MOV EAX, DWORD PTR DS : [ESP + 0x14];
			MOV bInheritHandles, EAX;

			MOV EAX, DWORD PTR DS : [ESP + 0x18];
			MOV dwCreationFlags, EAX;

			MOV EAX, DWORD PTR DS : [ESP + 0x1C];
			MOV lpEnvironment, EAX;

			MOV EAX, DWORD PTR DS : [ESP + 0x20];
			MOV lpCurrentDirectory, EAX;

			MOV EAX, DWORD PTR DS : [ESP + 0x24];
			MOV lpStartupInfo, EAX;

			MOV EAX, DWORD PTR DS : [ESP + 0x28];
			MOV lpProcessInformation, EAX;

		}

		if (CCharacter::strstr_my(lpCommandLine, "zy@fs"))
		{
			bHook = TRUE;
			//CLProcess::CreateProcess_Injector_DLL(lpCommandLine, L"", lpProcessInformation);
			::CreateProcessA(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags/* | CREATE_SUSPENDED*/, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
			LoadGameDLL(lpProcessInformation);
			bHook = FALSE;
			__asm RETN 0x28;
		}
		else
		{
			__asm
			{
				__asm PUSH EBP;
				__asm MOV EBP, ESP;
				__asm MOV EAX, dwHookAddr;
				__asm ADD EAX, 0x5;
				__asm JMP EAX;
			}
		}


	}

}


HANDLE hWorkThread = NULL;
DWORD WINAPI _WorkThread(LPVOID lpParm)
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetModuleFileNameW(NULL, wszPath, MAX_PATH);

	std::wstring wsPath = CCharacter::MakeTextToLower(wszPath);
	if (wsPath.find(L"cprotect1.exe") != -1)
	{
		MYHOOK_CONTENT HookContent;
		HookContent.dwFunAddr = reinterpret_cast<DWORD>(HookCreateProcessA);
		HookContent.dwHookAddr = dwHookAddr = reinterpret_cast<DWORD>(::CreateProcessA);
		HookContent.uNopCount = 0x0;
		CLHook::Hook_Fun_Jmp_MyAddr(&HookContent);
	}
	else if (wsPath.find(L"game.exe") != -1)
	{
		//CLog::GetInstance().SetClientName(L"InjectorClient", wszPath, TRUE, 20 * 1024 * 1024);
		std::wstring _wsModuleName = L"C:\\Visual Studio 2013\\Projects\\DHSH\\Release\\GameDLL.dll";
		HANDLE hFile = CreateFileW(_wsModuleName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			//LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"hFile = NULL, strFileName = %s", _wsModuleName.c_str());
			return FALSE;
		}

		DWORD _dwMemSize = GetFileSize(hFile, NULL);
		if (_dwMemSize == 0)
		{
			//LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"nFileSize = 0");
			return FALSE;
		}
		LPVOID _pRelocMemPtr = VirtualAlloc(NULL, _dwMemSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (_pRelocMemPtr == nullptr)
		{
			//LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"VirtualAlloc = 0");
			return FALSE;
		}

		DWORD dwReadSize = 0;
		if (!ReadFile(hFile, _pRelocMemPtr, _dwMemSize, &dwReadSize, NULL))
		{
			//LOG_CF(CLog::em_Log_Type::em_Log_Type_Exception, L"ReadFile = 0");
			return FALSE;
		}

		TCHAR szSysDll[MAX_PATH] = { 0 };
		TCHAR szDllName[] = { '\\', 'a', 's', 'y', 'c', 'f', 'i', 'l', 't', '.', 'd', 'l', 'l', 0 };
		GetSystemDirectory(szSysDll, MAX_PATH);
		lstrcat(szSysDll, szDllName);
		HMODULE hmDLL = NULL;
		LdrLoadMemDll(_pRelocMemPtr, _dwMemSize, szSysDll, &hmDLL);
	}

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	CLNoModuleDLL NoModuleDLL;
	NoModuleDLL.HideSelf(hModule, ul_reason_for_call, lpReserved);
	if (NoModuleDLL.GetNoModuleReasonValue() == ul_reason_for_call && hWorkThread == NULL)
	{
		hWorkThread = cbBEGINTHREADEX(NULL, NULL, _WorkThread, &hModule, NULL, NULL);
		::CloseHandle(hWorkThread);
	}

	return ul_reason_for_call == DLL_PROCESS_ATTACH ? FALSE : TRUE;
}

