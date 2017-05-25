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

#define _SELF L"main.cpp"

class CExpr : public MyTools::CExprFunBase, virtual public MyTools::CClassInstance<CExpr>
{
public:
	CExpr() = default;
	virtual ~CExpr()
	{

	}

	virtual VOID Release()
	{

	}

	virtual std::vector<MyTools::ExpressionFunPtr>& GetVec()
	{
		static std::vector<MyTools::ExpressionFunPtr> Vec =
		{
			{ std::bind(&CExpr::Help,this, std::placeholders::_1),L"Help" }
		};

		return Vec;
	}

private:
	virtual VOID Help(CONST std::vector<std::wstring>& VecParm)
	{

	}
};


int main()
{
	std::wstring wsText = L"创建成功!ID=54";
	auto Text =  wsText.substr(wsText.find(L"="));
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