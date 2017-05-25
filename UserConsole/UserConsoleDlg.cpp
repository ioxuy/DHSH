
// UserConsoleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "UserConsoleDlg.h"
#include "afxdialogex.h"
#include "ConsoleClient.h"
#include <MyTools/Log.h>
#include "ConsoleVariable.h"
#include "MainFormDlg.h"

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

	// TODO: Add extra initialization here
	CConsoleVariable::GetInstance().InitShareContent();
	CConsoleClient::GetInstance().SetEchoErrorPtr([] 
	{
		ExitProcess(0);
	});

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

	if (strAccountName.GetLength() > 16 || strAccountPass.GetLength() > 16)
	{
		AfxMessageBox(L"长度不可超过16位!");
		return;
	}

	if (!CConsoleClient::GetInstance().IsConnect())
	{
		if (!CConsoleClient::GetInstance().Run(L"127.0.0.1", 12345))
		{
			LOG_MSG_CF(L"连接服务器失败!");
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

	if (!CConsoleClient::GetInstance().IsConnect())
	{
		if (!CConsoleClient::GetInstance().Run(L"127.0.0.1", 12345))
		{
			LOG_MSG_CF(L"连接服务器失败!");
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
