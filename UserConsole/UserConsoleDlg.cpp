
// UserConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "UserConsoleDlg.h"
#include "afxdialogex.h"
#include "ConsoleClient.h"
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include <MyTools/CLFile.h>
#include <MyTools/CLProcess.h>
#include "ConsoleVariable.h"
#include "MainFormDlg.h"
#include "RunGame.h"

#define _SELF L"UserConsoleDlg.cpp"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUserConsoleDlg dialog



CUserConsoleDlg::CUserConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USERCONSOLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUserConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUserConsoleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CUserConsoleDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CUserConsoleDlg::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CUserConsoleDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CUserConsoleDlg message handlers

BOOL CUserConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}



	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	if (MyTools::CLProcess::Is_Exist_Process_For_ProcName(L"Game.exe"))
	{
		AfxMessageBox(L"检查进程存在游戏'Game.exe', 请先关闭游戏再打开本程序!");
		ExitProcess(0);
		return TRUE;
	}
	
#ifndef _DEBUG
	HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_DHSH_USERCONSOLE");
	if (hMutex != NULL && hMutex != INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(L"进程还存在'UserConsole.exe', 不能同时开2个本程序!");
		::CloseHandle(hMutex);
		ExitProcess(0);
		return TRUE;
	}
#endif // _DEBUG

	CreateMutexW(NULL, FALSE, L"CL_DHSH_USERCONSOLE");

	WCHAR wszLogPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszLogPath);
	::lstrcatW(wszLogPath, LR"(\Log\)");
	if (!MyTools::CLPublic::FileExist(wszLogPath))
		MyTools::CLFile::CreateMyDirectory(wszLogPath);

	MyTools::CLog::GetInstance().SetClientName(L"UserConsole", wszLogPath, FALSE, 20 * 1024 * 1024);

	// TODO: Add extra initialization here
	CConsoleVariable::GetInstance().InitShareContent();
	CConsoleClient::GetInstance().SetEchoErrorPtr([] 
	{
		ExitProcess(0);
	});

	reinterpret_cast<CButton *>(GetDlgItem(IDC_BUTTON_TEST))->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUserConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUserConsoleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUserConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUserConsoleDlg::OnBnClickedButtonLogin()
{
	CEdit* pAccountEdit = reinterpret_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_ACCOUNT));
	CEdit* pPassEdit = reinterpret_cast<CEdit *>(this->GetDlgItem(IDC_EDIT_PASS));

	CStringW strAccountName;
	CStringW strAccountPass;
	pAccountEdit->GetWindowTextW(strAccountName);
	pPassEdit->GetWindowTextW(strAccountPass);

	if (strAccountName == L"")
	{
		strAccountName = L"admin";
		strAccountPass = L"admin";
	}
	

	if (strAccountName.GetLength() > 16 || strAccountPass.GetLength() > 16)
	{
		AfxMessageBox(L"长度不可超过16位!");
		return;
	}
	if (strAccountName.GetLength() <= 4 || strAccountPass.GetLength() <= 4)
	{
		AfxMessageBox(L"长度不可小于5位!");
		return;
	}

	if (!CConsoleClient::GetInstance().IsConnect())
	{
		if (!CConsoleClient::GetInstance().Run(SERVERIP, SERVERPORT, 3 * 1000))
		{
			LOG_MSG_CF(L"连接服务器失败!");
			this->PostMessageW(WM_CLOSE);
			return;
		}
	}

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_AccountLogin);
	SocketBuffer << strAccountName.GetBuffer() << strAccountPass.GetBuffer() << L"DHSH" << GAMEVERSION;

	DWORD bLogin = FALSE;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&bLogin](MyTools::CLSocketBuffer SocketBuffer)
	{
		SocketBuffer >> bLogin >> CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig.dwToken;
	});

	if (!bLogin)
	{
		AfxMessageBox(L"登录失败……帐号密码错误!");
		return;
	}
	
	auto& GlobalConfig = CConsoleVariable::GetInstance().GetSareContent()->GlobalConfig;
	MyTools::CCharacter::wstrcpy_my(GlobalConfig.wszAccountName, strAccountName.GetBuffer());
	MyTools::CCharacter::wstrcpy_my(GlobalConfig.wszAccountPass, strAccountPass.GetBuffer());
	this->PostMessageW(WM_CLOSE);
}


void CUserConsoleDlg::OnBnClickedButtonRegister()
{
	CEdit* pAccountEdit = reinterpret_cast<CEdit*>(this->GetDlgItem(IDC_EDIT_ACCOUNT));
	CEdit* pPassEdit = reinterpret_cast<CEdit *>(this->GetDlgItem(IDC_EDIT_PASS));

	CStringW strAccountName;
	CStringW strAccountPass;
	pAccountEdit->GetWindowTextW(strAccountName);
	pPassEdit->GetWindowTextW(strAccountPass);

	if (strAccountName.GetLength() > 16 || strAccountPass.GetLength() > 16)
	{
		AfxMessageBox(L"长度不可超过16位!");
		return;
	}
	if (strAccountName.GetLength() <= 4 || strAccountPass.GetLength() <= 4)
	{
		AfxMessageBox(L"长度不可小于5位!");
		return;
	}

	if (!CConsoleClient::GetInstance().IsConnect())
	{
		if (!CConsoleClient::GetInstance().Run(SERVERIP, SERVERPORT, 3 * 1000))
		{
			LOG_MSG_CF(L"连接服务器失败!");
			this->PostMessageW(WM_CLOSE);
			return;
		}
	}

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_Register);
	SocketBuffer << strAccountName.GetBuffer() << strAccountPass.GetBuffer();

	std::wstring wsText;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&wsText](MyTools::CLSocketBuffer SocketBuffer)
	{
		SocketBuffer >> wsText;
	});

	AfxMessageBox(wsText.c_str());
}


void CUserConsoleDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}


void CUserConsoleDlg::OnBnClickedButtonTest()
{
	CFileDialog Dialog_(TRUE, L"exe", NULL, NULL, L"Game.exe", NULL, NULL, NULL);

	if (Dialog_.DoModal() == IDOK)
	{
		AfxMessageBox(Dialog_.GetPathName().GetBuffer());
	}
}
