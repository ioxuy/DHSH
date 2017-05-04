#include <MyTools/CLProcess.h>
#include <MyTools/CLNoModuleDLL.h>
#include <MyTools/Log.h>
#include <iostream>
#include "LdrHeader.h"

BOOL LdrLoadMemDll(PVOID DllMem, SIZE_T dllSize, LPCWSTR szDllName, HMODULE *pModule);

#define _SELF L"main.cpp"

int main()
{
	//HMODULE hm = ::LoadLibraryW(L"C:\\Visual Studio 2013\\Projects\\DHSH\\Release\\GameDLL.dll");
	//::FreeLibrary(hm);

	/*std::wstring _wsModuleName = L"C:\\Visual Studio 2013\\Projects\\DHSH\\Release\\GameDLL.dll";
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
	system("pause");
	return 0;*/

	if (!CLProcess::Is_Exist_Process_For_ProcName(L"CProtect1.exe"))
	{
		STARTUPINFOW		si = { sizeof(STARTUPINFOW) };
		PROCESS_INFORMATION pi = { 0 };
		WCHAR				wszProcPath[MAX_PATH] = { L"C:\\dhsh\\shdata\\CProtect1.exe Game.exe A89DD7AD dhsh1 CYOU Space zy@fs" };

		if (!::CreateProcessW(NULL, wszProcPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, L"C:\\dhsh\\shdata\\", &si, &pi))
		{
			return FALSE;
		}

		CLProcess::LoadRemoteDLL(pi.dwProcessId, L"C:\\Visual Studio 2013\\Projects\\DHSH\\Release\\InjectorDLL.dll");
		::ResumeThread(pi.hThread);
		::CloseHandle(pi.hProcess);
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