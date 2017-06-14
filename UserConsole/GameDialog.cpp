#include "stdafx.h"
#include "GameDialog.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>

#define _SELF L"GameDialog.cpp"

CGameDialog::CGameDialog() : _hWnd(NULL), _hFont(NULL)
{
	InitFont();
}

BOOL CGameDialog::Create()
{
	HINSTANCE hThisInstance = NULL;
	MSG messages;
	WNDCLASSEX wincl;
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = GetRandomClassName().c_str();
	wincl.lpfnWndProc = CGameDialog::WindowProc;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof(WNDCLASSEX);
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//(HBRUSH)COLOR_BACKGROUND;
															   //wincl.hbrBackground=(HBRUSH)COLOR_BACKGROUND;
	if (!RegisterClassExW(&wincl))
	{
		LOG_MSG_CF(L"注册窗口类失败!");
		return FALSE;
	}
	
	_hWnd = CreateWindowEx(WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME | WS_EX_CONTROLPARENT, wincl.lpszClassName, L"Dialog",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | DS_FIXEDSYS | DS_MODALFRAME | DS_3DLOOK | DS_SHELLFONT | WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 520, 100, NULL/*HWND_DESKTOP*/, NULL, hThisInstance, NULL);
	if (!_hWnd)
	{
		LOG_MSG_CF(L"创建窗口失败!");
		return FALSE;
	}

	ShowWindow(_hWnd, SW_SHOW);
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

	if (!UnregisterClass(wincl.lpszClassName, hThisInstance))
	{
		LOG_MSG_CF(L"卸载窗口类失败!");
		return FALSE;
	}

	return TRUE;
}

HWND CGameDialog::GetDialogWnd() CONST _NOEXCEPT
{
	return _hWnd;
}

std::wstring CGameDialog::GetRandomClassName() CONST
{
	std::wstring wsClassName;
	MyTools::CCharacter::FormatText(wsClassName, L"%X", MyTools::CCharacter::GetRand_For_DWORD());
	
	return std::move(wsClassName);
}

LRESULT CALLBACK CGameDialog::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		CGameDialog::GetInstance().CreateControl();
		break;
	case WM_KEYDOWN:
		if (wParam == VK_HOME)
			::ShowWindow(CGameDialog::GetInstance().GetDialogWnd(), SW_SHOW);
		else if (wParam == VK_END)
			::ShowWindow(CGameDialog::GetInstance().GetDialogWnd(), SW_HIDE);
		break;
	case WM_CLOSE:
		::ShowWindow(CGameDialog::GetInstance().GetDialogWnd(), SW_HIDE);
	case WM_DESTROY:
		::ShowWindow(CGameDialog::GetInstance().GetDialogWnd(), SW_HIDE);
		::DestroyWindow(CGameDialog::GetInstance().GetDialogWnd());
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProcW(hwnd, NULL, wParam, lParam);
	}
	return 0;
}

VOID CGameDialog::InitFont()
{
	_hFont = CreateFont(11, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("宋体"));
}

VOID CGameDialog::CreateControl()
{
	_hBtnConfig = ::CreateWindowExW(NULL, L"Button", L"选择配置文件", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 5, 75, 20, _hWnd, reinterpret_cast<HMENU>(1), NULL, NULL);
}
