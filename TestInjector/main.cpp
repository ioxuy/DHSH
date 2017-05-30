#include <MyTools/CLProcess.h>
#include <MyTools/CLNoModuleDLL.h>
#include <MyTools/Log.h>
#include <iostream>
#include <MyTools/CLFile.h>
#include <MyTools/CLThread.h>
#include <MyTools/CLPublic.h>
#include <MyTools/Character.h>
#include <MyTools/CmdLog.h>
#include <MyTools/CLExpression.h>
#include <future>
#include "resource.h"

#define _SELF L"main.cpp"


int main()
{
	WCHAR wszResourceType[] = { L"kernel" };
	HMODULE hm = NULL;
	HRSRC hRsrc = ::FindResourceW(hm, reinterpret_cast<LPCWSTR>(IDR_GAMEDLL), wszResourceType);
	if (hRsrc)
	{
		DWORD dwSize = ::SizeofResource(hm, hRsrc);
		dwSize = 0;
	}
	::Sleep(-1);
	return 0;
	if (!MyTools::CLProcess::Is_Exist_Process_For_ProcName(L"CProtect1.exe"))
	{
		STARTUPINFOW		si = { sizeof(STARTUPINFOW) };
		PROCESS_INFORMATION pi = { 0 };
		WCHAR				wszProcPath[MAX_PATH] = { L"C:\\dhsh\\shdata\\CProtect1.exe Game.exe A89DD7AD dhsh1 CYOU Space zy@fs" };

		if (!::CreateProcessW(NULL, wszProcPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, L"C:\\dhsh\\shdata\\", &si, &pi))
		{
			return FALSE;
		}

		LPVOID lpFileContent = nullptr;
		UINT uSize = 0;
		if (MyTools::CLFile::ReadFileContent(L"C:\\Share\\InjectorDLL.dll", lpFileContent, uSize))
		{
			MyTools::CLNoModuleDLL NoModuleDLL;
			BOOL bRetCode = NoModuleDLL.RemoteLoadMemoryDLL(lpFileContent, uSize, pi.hProcess, pi.hThread);
			std::wcout << bRetCode << std::endl;
			//CLProcess::LoadRemoteDLL(pi.dwProcessId, L"C:\\Visual Studio 2013\\Projects\\DHSH\\Release\\InjectorDLL.dll");
			::ResumeThread(pi.hThread);
			::CloseHandle(pi.hProcess);
			::VirtualFree(lpFileContent, 0, MEM_RELEASE);
		}
		
	}
	else
	{
		STARTUPINFOW		si = { sizeof(STARTUPINFOW) };
		PROCESS_INFORMATION pi = { 0 };
		WCHAR				wszProcPath[MAX_PATH] = { L"C:\\dhsh\\shdata\\CProtect1.exe Game.exe A89DD7AD dhsh1 CYOU Space zy@fs" };

		if (!::CreateProcessW(NULL, wszProcPath, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, L"C:\\dhsh\\shdata\\", &si, &pi))
		{
			return FALSE;
		}
	}
	system("pause");
	return 0;
}