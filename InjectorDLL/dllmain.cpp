// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLHook.h>
#include <MyTools/CLNoModuleDLL.h>
#include <MyTools/CLProcess.h>
#include <MyTools/CLFile.h> 
#define _SELF L"dllmain.cpp"

struct DLL_Share_Memory
{
	LPVOID lpAddr;
	HANDLE hFileMap;

	UINT   uSize;
	LPVOID lpDLLContentAddr;

	DLL_Share_Memory()
	{
		lpAddr = lpDLLContentAddr = nullptr;
		hFileMap = NULL;
		uSize = 0;
	}
};

DLL_Share_Memory InjectorDLLMemory;
DLL_Share_Memory GameDLLMemory;

BOOL GetShareMemory(DLL_Share_Memory& ShareMemory, _In_ CONST std::wstring& wsText)
{
	if (ShareMemory.hFileMap != NULL && ShareMemory.hFileMap != INVALID_HANDLE_VALUE)
		return TRUE;

	ShareMemory.hFileMap = ::OpenFileMappingW(PAGE_READWRITE, FALSE, wsText.c_str());
	if (ShareMemory.hFileMap == NULL || ShareMemory.hFileMap == INVALID_HANDLE_VALUE)
		return FALSE;
	
	ShareMemory.lpAddr = ::MapViewOfFile(ShareMemory.hFileMap, FILE_MAP_READ, NULL, NULL, 10 * 1024 * 1024);
	ShareMemory.lpDLLContentAddr = reinterpret_cast<CHAR *>(ShareMemory.lpAddr) + 4;
	ShareMemory.uSize = *reinterpret_cast<DWORD *>(ShareMemory.lpAddr);
	return TRUE;
}

VOID LoadGameDLL(_In_ CONST LPPROCESS_INFORMATION lpProcessInformation)
{
	if (lpProcessInformation != nullptr)
	{
		if (GetShareMemory(InjectorDLLMemory, L"CL_SHARE_DHSH_INJECTORDLL"))
		{
			MyTools::CLNoModuleDLL NoModuleDLL;
			NoModuleDLL.RemoteLoadMemoryDLL(InjectorDLLMemory.lpDLLContentAddr, InjectorDLLMemory.uSize, lpProcessInformation->hProcess, lpProcessInformation->hThread);
			ResumeThread(lpProcessInformation->hThread);
		}
		else
			::MessageBoxW(NULL, L"Load Injector.dll Faild!", L"", NULL);
	}

}

#pragma warning(push)
#pragma warning(disable:4740)
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
		__asm PUSHAD;
		if (MyTools::CCharacter::strstr_my(lpCommandLine, "zy@fs"))
		{
			bHook = TRUE;
			//CLProcess::CreateProcess_Injector_DLL(lpCommandLine, L"", lpProcessInformation);
			::CreateProcessA(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags | CREATE_SUSPENDED, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
			LoadGameDLL(lpProcessInformation);
			bHook = FALSE;
			__asm POPAD;
			__asm RETN 0x28;
		}
		else
		{
			__asm POPAD;
			__asm PUSH EBP;
			__asm MOV EBP, ESP;
			__asm MOV EAX, dwHookAddr;
			__asm ADD EAX, 0x5;
			__asm JMP EAX;
		}
	}
} 
#pragma warning(pop)

HANDLE hWorkThread = NULL;
DWORD WINAPI _WorkThread(LPVOID)
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetModuleFileNameW(NULL, wszPath, MAX_PATH); 
	std::wstring wsPath = MyTools::CCharacter::MakeTextToLower(wszPath);
	if (wsPath.find(L"cprotect1.exe") != -1)  
	{
		MyTools::MYHOOK_CONTENT HookContent;
		HookContent.dwFunAddr = reinterpret_cast<DWORD>(HookCreateProcessA);
		HookContent.dwHookAddr = dwHookAddr = reinterpret_cast<DWORD>(::CreateProcessA);
		HookContent.uNopCount = 0x0;
		MyTools::CLHook::Hook_Fun_Jmp_MyAddr(&HookContent);
	}
	else if(wsPath.find(L"game.exe") != -1)
	{
		if (GetShareMemory(GameDLLMemory, L"CL_SHARE_DHSH_GAMEDLL"))
		{
			MyTools::CLNoModuleDLL NoModuleDLL;
			NoModuleDLL.LoadMemoryDLL(GameDLLMemory.lpDLLContentAddr, GameDLLMemory.uSize);
			::MessageBoxW(NULL, L"game", L"", NULL);
		}
		else
			::MessageBoxW(NULL, L"GetShareMemory Faild!", L"", NULL);
	}
	return 0;
}

BOOL APIENTRY DllMain(HMODULE ,DWORD  ,LPVOID )
{
	if (hWorkThread == NULL)
	{
		hWorkThread = cbBEGINTHREADEX(NULL, NULL, _WorkThread, NULL, NULL, NULL);
		::CloseHandle(hWorkThread);
	}
	return TRUE;
}

