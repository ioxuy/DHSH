#ifndef __GameDialog_H__
#define __GameDialog_H__

#include "../GameDLL/GameBase.h"
class CGameDialog : MyTools::CClassInstance<CGameDialog>
{
public:
	CGameDialog();
	~CGameDialog() = default;

	BOOL Create();

	HWND GetDialogWnd() CONST _NOEXCEPT;
private:
	std::wstring GetRandomClassName() CONST;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	VOID InitFont();

	VOID CreateControl();
private:
	HWND _hWnd;
	HFONT _hFont;
	HWND _hBtnConfig;
};



#endif // !__GameDialog_H__
