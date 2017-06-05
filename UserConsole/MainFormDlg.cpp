// MainFormDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "MainFormDlg.h"
#include "afxdialogex.h"
#include <mutex>
#include <future>
#include <MyTools/CLPublic.h>
#include <MyTools/CLProcess.h>
#include <MyTools/CLNoModuleDLL.h>
#include <MyTools/Log.h>
#include "ConsoleVariable.h"
#include "ConsoleClient.h"
#include "RechargeCardDlg.h"
#include "RechargeCardLogDlg.h"
#include "UserConsoleDlg.h"
#include "UserConfigDlg.h"
#include "RunGame.h"
#include "../GameDLL/GameBase.h"

// CMainFormDlg dialog
#define _SELF L"MainFormDlg.cpp"
IMPLEMENT_DYNAMIC(CMainFormDlg, CDialogEx)

CMainFormDlg::CMainFormDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MAINFORM, pParent)
{
	_hKeepALiveThread = INVALID_HANDLE_VALUE;
	_nHour = 0;
}

CMainFormDlg::~CMainFormDlg()
{
}

void CMainFormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BOOL CMainFormDlg::OnInitDialog()
{
	CUserConsoleDlg dlg;
	dlg.DoModal();

	if (CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.dwToken == NULL)
	{
		this->PostMessageW(WM_CLOSE);
		return TRUE;
	}

	HRSRC	hRsrc = FindResourceW(NULL, reinterpret_cast<CONST WCHAR*>(IDR_DLL1), L"DLL");
	if (hRsrc)
	{
		_RunGame.InitializeGameDLLMemory(LockResource(LoadResource(NULL, hRsrc)), SizeofResource(NULL, hRsrc));
	}
	else
	{
		LOG_MSG_CF(L"加载资源1失败!");
		ExitProcess(0);
	}

	hRsrc = FindResourceW(NULL, reinterpret_cast<CONST WCHAR*>(IDR_INJECTORDLL1), L"InjectorDLL");
	if (hRsrc)
	{
		_RunGame.InitializeInjectorDLLMemory(LockResource(LoadResource(NULL, hRsrc)), SizeofResource(NULL, hRsrc));
	}
	else
	{
		LOG_MSG_CF(L"加载资源2失败!");
		ExitProcess(0);
	}

	CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.dwConsoleTitle = MyTools::CCharacter::GetRand_For_DWORD();

	std::wstring wsTitleText;
	this->SetWindowTextW(MyTools::CCharacter::FormatText(wsTitleText, L"%08X", CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.dwConsoleTitle).c_str());

	_bRun = TRUE;
	_hKeepALiveThread = cbBEGINTHREADEX(NULL, NULL, _KeepALiveThread, this, NULL, NULL);
	return TRUE;
}

DWORD WINAPI CMainFormDlg::_KeepALiveThread(LPVOID lpParam)
{
	auto dlg = static_cast<CMainFormDlg*>(lpParam);
	while (dlg->_bRun)
	{
		dlg->SetAccountHour_To_Dlg();
		for (int i = 0; i < 10 && dlg->_bRun; ++i)
			::Sleep(500);
		
	}
	return 0;
}

VOID CMainFormDlg::SetAccountHour_To_Dlg()
{
	static std::mutex mutex_;
	std::lock_guard<std::mutex> lock_(mutex_);

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_KeepLive);

	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.dwHour;
		CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.ulTimeTick = ::GetTickCount64();
	});

	_nHour = CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.dwHour;

	CStringW strHour;
	CStatic* pStatic = reinterpret_cast<CStatic *>(GetDlgItem(IDC_STATIC_HOUR));
	
	strHour.Format(L"%d", _nHour);
	pStatic->SetWindowTextW(strHour);
}

CUserConfigDlg* CMainFormDlg::FindDlg_By_PlayerName(_In_ CONST std::wstring& wsPlayerName)
{
	auto pElement = MyTools::CLPublic::Vec_find_if(_VecDlgContent, [wsPlayerName](CUserConfigDlg* pUserConfigDlg) { return pUserConfigDlg->GetPlayerName() == wsPlayerName; });
	return pElement == nullptr ? nullptr : *pElement;
}

BOOL CMainFormDlg::FindPlayerName_By_Index(_In_ UINT uIndex, _Out_ std::wstring& wsPlayername) CONST
{
	auto p = CConsoleVariable::GetInstance().GetSareContent();
	for (UINT i = 0;i < p->uAccountCount; ++i)
	{
		if (p->arrGameArrount[i].AccountStatus.bExist && p->arrGameArrount[i].uIndex == uIndex)
		{
			wsPlayername = p->arrGameArrount[i].AccountContent.wszPlayerName;
			return TRUE;
		}
	}
	return FALSE;
}

VOID CMainFormDlg::AddDlgContent(_In_ CUserConfigDlg* pUserConfigDlg)
{
	_VecDlgContent.emplace_back(pUserConfigDlg);
}

BEGIN_MESSAGE_MAP(CMainFormDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RECHARGECARD, &CMainFormDlg::OnBnClickedButtonRechargecard)
	ON_BN_CLICKED(IDC_BUTTON_RECHARGECARDLOG, &CMainFormDlg::OnBnClickedButtonRechargecardlog)
	ON_BN_CLICKED(IDC_BUTTON_RUNGAME, &CMainFormDlg::OnBnClickedButtonRungame)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMainFormDlg message handlers


void CMainFormDlg::OnBnClickedButtonRechargecard()
{
	CRechargeCardDlg dlg;
	dlg.DoModal();
	SetAccountHour_To_Dlg();

	/*auto p = CConsoleVariable::GetInstance().GetSareContent();
	if (p->uAccountCount == 0)
	{
		p->uAccountCount += 1;
		p->arrGameArrount[0].uIndex = 0;
		p->arrGameArrount[0].AccountStatus.bExist = TRUE;
		//MyTools::CCharacter::wstrcpy_my(p->arrGameArrount[0].AccountContent.wszPlayerName, L"逆天童");
		MyTools::CCharacter::wstrcpy_my(p->arrGameArrount[0].AccountContent.wszPlayerName, L"Test");
	}

	_Point Point_(0, 0);
	this->PostMessageW(WM_CUSTOME_HOME, 0, Point_.Encode());*/
}


void CMainFormDlg::OnBnClickedButtonRechargecardlog()
{
	CRechargeCardLogDlg dlg;
	dlg.DoModal();
}


void CMainFormDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}


void CMainFormDlg::OnBnClickedButtonRungame()
{
	if (_nHour <= 0)
	{
		AfxMessageBox(L"有效时间不足!");
		return;
	}

	if (CConsoleVariable::GetInstance().GetSareContent()->wszGamePath[0] == L'\0')
		SyncGamePath();

	std::wstring wsGamePath;
	MyTools::CCharacter::FormatText(wsGamePath, L"%s\\Game.exe", CConsoleVariable::GetInstance().GetSareContent()->wszGamePath);
	if (!MyTools::CLPublic::FileExist(wsGamePath))
	{
		AfxMessageBox(L"游戏路径错误! 请选择游戏路径!");
		static std::wstring wsGamePath_;
		if(ChoiceGamePath(wsGamePath_))
			std::async(std::launch::async, &CMainFormDlg::SaveGamePath, this, wsGamePath_);

		return;
	}

	if (!_RunGame.Run())
	{
		AfxMessageBox(L"启动游戏失败!");
		return;
	}

	//HANDLE hEvent = ::CreateEventW(NULL, FALSE, FALSE, L"CL_MUTEX_DHSH_RUNGAME");
	reinterpret_cast<CButton *>(this->GetDlgItem(IDC_BUTTON_RUNGAME))->EnableWindow(FALSE);
	std::thread t([this] 
	{
		::Sleep(5 * 1000);
		reinterpret_cast<CButton *>(this->GetDlgItem(IDC_BUTTON_RUNGAME))->EnableWindow(TRUE);
	});
	t.detach();
}


BOOL CMainFormDlg::ChoiceGamePath(_Out_ std::wstring& wsGamePath) CONST
{
	CFileDialog Dialog_(TRUE,L"exe",NULL,NULL,L"Game.exe",NULL,NULL,NULL);

	if (Dialog_.DoModal() == IDOK)
	{
		wsGamePath = Dialog_.GetPathName().GetBuffer();
		return TRUE;
	}
	return FALSE;
}

BOOL CMainFormDlg::SaveGamePath(_In_ CONST std::wstring& wsGamePath) CONST
{
	MyTools::CCharacter::wstrcpy_my(CConsoleVariable::GetInstance().GetSareContent()->wszGamePath, wsGamePath.c_str());
	::PathRemoveFileSpecW(CConsoleVariable::GetInstance().GetSareContent()->wszGamePath);

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_WriteConfig);
	SocketBuffer << L"Config!" << L"Common!" << 1 << L"GamePath" << CConsoleVariable::GetInstance().GetSareContent()->wszGamePath;

	CConsoleClient::GetInstance().AsyncSend(SocketBuffer);
	return TRUE;
}

BOOL CMainFormDlg::SyncGamePath()
{
	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_ReadConfig);
	SocketBuffer << L"Config!" << L"Common!" << 1 << L"GamePath";

	return CConsoleClient::GetInstance().SyncSend(SocketBuffer, [](MyTools::CLSocketBuffer& SocketBuffer)
	{
		DWORD dwSize = 0;
		SocketBuffer >> dwSize;
		if (dwSize == 0)
			return;

		std::wstring wsGamePath;
		std::wstring wsConfigName;

		SocketBuffer >> wsConfigName >> wsGamePath;
		if (wsConfigName == L"GamePath")
		{
			LOG_C_D(L"GamePath=%s", wsGamePath.c_str());
			MyTools::CCharacter::wstrcpy_my(CConsoleVariable::GetInstance().GetSareContent()->wszGamePath, wsGamePath.c_str());
		}
	});
}

void CMainFormDlg::OnClose()
{
	LOG_C_D(L"Kill Game~");
	KillGame();
	auto futurePtr = std::async(std::launch::async, &CMainFormDlg::CloseConfigDlg, this);
	_bRun = FALSE;

	if (_hKeepALiveThread != INVALID_HANDLE_VALUE)
	{
		::WaitForSingleObject(_hKeepALiveThread, INFINITE);
		_hKeepALiveThread = INVALID_HANDLE_VALUE;
	}
	LOG_C_D(L"Stop KeepAlive Thread");
	futurePtr.wait();
	LOG_C_D(L"futurePtr.get()");
	CDialogEx::OnClose();
}


BOOL CMainFormDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_CUSTOME_HOME)
	{
		/*UINT uIndex = static_cast<UINT>(pMsg->wParam);
		DWORD dwPointValue = static_cast<DWORD>(pMsg->lParam);
		_Point TarPoint = _Point::Decode(dwPointValue);

		std::wstring wsPlayerName;
		if (FindPlayerName_By_Index(uIndex, wsPlayerName))
		{
			LOG_C_D(L"Index=%d, TarPoint=[%d,%d], PlayerName=%s", uIndex, TarPoint.X,TarPoint.Y, wsPlayerName.c_str());
			auto dlg = FindDlg_By_PlayerName(wsPlayerName);
			if (dlg == nullptr)
			{
				CUserConfigDlg* pUserConfigDlg = new CUserConfigDlg(wsPlayerName);
				pUserConfigDlg->Create(IDD_DIALOG_USERCONFIG, this);
				pUserConfigDlg->ShowWindow(SW_SHOW);
				::SetWindowPos(pUserConfigDlg->GetSafeHwnd(), HWND_TOPMOST, TarPoint.X, TarPoint.Y, 0, 0, SWP_NOSIZE);
				AddDlgContent(pUserConfigDlg);
			}
			else
			{
				dlg->ShowWindow(SW_SHOW);
			}
		}
		else
			::MessageBoxW(NULL, MyTools::CCharacter::FormatText(L"Invalid Index=", uIndex), L"", NULL);*/
	}
	else if (pMsg->message == WM_CUSTOME_END)
	{
		/*UINT uIndex = static_cast<UINT>(pMsg->wParam);

		std::wstring wsPlayerName;
		if (FindPlayerName_By_Index(uIndex, wsPlayerName))
		{
			auto dlg = FindDlg_By_PlayerName(wsPlayerName);
			if (dlg != nullptr)
			{
				dlg->ShowWindow(SW_HIDE);
			}
		}*/
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CMainFormDlg::CloseConfigDlg()
{
	for (auto& itm : _VecDlgContent)
	{
		itm->MyDestoreWindows();
		itm->DestroyWindow();
		delete itm;
	}

	_VecDlgContent.clear();
	LOG_C_D(L"Clear Vec Dlg");
	return TRUE;
}

VOID CMainFormDlg::KillGame()
{
	while (MyTools::CLProcess::Is_Exist_Process_For_ProcName(L"Game.exe"))
	{
		LOG_C_D(L"Killing, PID=%X", MyTools::CLProcess::GetPid_For_ProcName(L"Game.exe"));
		MyTools::CLProcess::TerminateProc_For_ProcName(L"Game.exe", 3000, 1);
	}
}
