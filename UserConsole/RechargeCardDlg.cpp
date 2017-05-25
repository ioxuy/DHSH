// RechargeCardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserConsole.h"
#include "RechargeCardDlg.h"
#include "afxdialogex.h"
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include "ConsoleClient.h"
#include "ConsoleVariable.h"
// CRechargeCardDlg dialog

IMPLEMENT_DYNAMIC(CRechargeCardDlg, CDialogEx)

CRechargeCardDlg::CRechargeCardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RECHARGECARD, pParent)
{

}

CRechargeCardDlg::~CRechargeCardDlg()
{
}

void CRechargeCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CRechargeCardDlg::OnInitDialog()
{
	return TRUE;
}

BEGIN_MESSAGE_MAP(CRechargeCardDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SUMIT_RECHARGE, &CRechargeCardDlg::OnBnClickedButtonSumitRecharge)
END_MESSAGE_MAP()


// CRechargeCardDlg message handlers


void CRechargeCardDlg::OnBnClickedButtonSumitRecharge()
{
	CEdit* pEdit = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDIT_CARD));

	CStringW strCardNo;
	pEdit->GetWindowTextW(strCardNo);

	if (!VerCardNo(strCardNo.GetBuffer()))
	{
		AfxMessageBox(L"¿¨ºÅ²»ºÏ·¨!");
		return;
	}

	MyTools::CLSocketBuffer SocketBuffer(em_Sock_Msg::em_Sock_Msg_Recharge);
	SocketBuffer << strCardNo.GetBuffer();

	std::wstring wsText;
	CConsoleClient::GetInstance().SyncSend(SocketBuffer, [&wsText](MyTools::CLSocketBuffer& SocketBuffer)
	{
		SocketBuffer >> wsText;
	});

	pEdit->SetWindowTextW(wsText.c_str());
}

BOOL CRechargeCardDlg::VerCardNo(_In_ LPCWSTR pwszCardNo) CONST
{
	auto fnGetHour = [](LPCSTR pwszCard)
	{
		CHAR wszHour[32] = { 0 };
		MyTools::CCharacter::strcpy_my(wszHour, pwszCard, 4);
		return (DWORD)atoi(wszHour);
	};

	auto fnGetDate = [](LPCSTR pwszCard)
	{
		CHAR wszDate[32] = { 0 };
		MyTools::CCharacter::strcpy_my(wszDate, pwszCard + 4, 8);
		return (DWORD)atoi(wszDate);
	};

	auto fnGetRand = [](LPCSTR pwszCard)
	{
		CHAR wszRand[32] = { 0 };
		MyTools::CCharacter::strcpy_my(wszRand, pwszCard + 4 + 8, 8);
		return (DWORD)strtol(wszRand, NULL, 16);
	};

	auto fnGetCrc = [](LPCSTR pwszCard)
	{
		CHAR wszCrc[32] = { 0 };
		MyTools::CCharacter::strcpy_my(wszCrc, pwszCard + 4 + 8 + 8, 8);
		return (DWORD)strtoll(wszCrc, NULL, 16);
	};

	auto fnConvertNumber = [](LPCSTR pszFormat, DWORD n)
	{
		static CHAR wszText[128];
		wsprintfA(wszText, pszFormat, n);
		return wszText;
	};

	if (wcslen(pwszCardNo) != 0x1C)
		return 0;

	CHAR szCard[128] = { 0 };
	MyTools::CCharacter::UnicodeToASCII(pwszCardNo, szCard);

	auto nHour = fnGetHour(szCard);
	auto nDate = fnGetDate(szCard);
	auto nRand = fnGetRand(szCard);
	//auto nCrc = fnGetCrc(szCard);

	CHAR wszText[128] = { 0 };
	lstrcatA(wszText, fnConvertNumber("%04d", nHour));
	lstrcatA(wszText, fnConvertNumber("%08d", nDate));
	lstrcatA(wszText, fnConvertNumber("%08X", nRand));
	auto dwCrc = MyTools::CLPublic::GetCRC32_DWORD(wszText, strlen(wszText));
	lstrcatA(wszText, fnConvertNumber("%08X", dwCrc));
	return MyTools::CCharacter::strcmp_my(wszText, szCard) ? 1 : 0;
}


void CRechargeCardDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}
