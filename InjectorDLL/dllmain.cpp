// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>
#include <MyTools/CLHook.h>
#include <MyTools/CLNoModuleDLL.h>
#include <MyTools/CLProcess.h>
#include <MyTools/CLFile.h>
#define _SELF L"dllmain.cpp"

VOID LoadGameDLL(_In_ CONST LPPROCESS_INFORMATION lpProcessInformation)
{
	if (lpProcessInformation != nullptr)
	{
		LPVOID lpFileContent = nullptr;
		UINT uSize = 0;
		if (MyTools::CLFile::ReadFileContent(L"C:\\Share\\InjectorDLL.dll", lpFileContent, uSize))
		{
			MyTools::CLNoModuleDLL NoModuleDLL;
			NoModuleDLL.RemoteLoadMemoryDLL(lpFileContent, uSize, lpProcessInformation->hProcess, lpProcessInformation->hThread);
			ResumeThread(lpProcessInformation->hThread);
			::VirtualFree(lpFileContent, 0, MEM_RELEASE);
		}
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
		LPVOID lpFileContent = nullptr;
		UINT uSize = 0;
		if (MyTools::CLFile::ReadFileContent(L"C:\\Share\\GameDLL.dll", lpFileContent, uSize))
		{
			MyTools::CLNoModuleDLL NoModuleDLL;
			BOOL bRet = NoModuleDLL.LoadMemoryDLL(lpFileContent, uSize);
			::VirtualFree(lpFileContent, 0, MEM_RELEASE);
		}
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

