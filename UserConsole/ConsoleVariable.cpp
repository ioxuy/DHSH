#include "stdafx.h"
#include "ConsoleVariable.h"

CConsoleVariable::CConsoleVariable() : _hFileMap(INVALID_HANDLE_VALUE)
{

}

CConsoleVariable::~CConsoleVariable()
{
	if (_hFileMap != INVALID_HANDLE_VALUE)
	{
		::UnmapViewOfFile(GetSareContent());
		::CloseHandle(_hFileMap);
		_hFileMap = INVALID_HANDLE_VALUE;
	}
}

PGameShareContent& CConsoleVariable::GetSareContent() CONST
{
	static PGameShareContent p = nullptr;
	return p;
}

VOID CConsoleVariable::InitShareContent()
{
	auto& p = GetSareContent();
	_hFileMap = ::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, sizeof(GameShareContent), L"CL_SHARE_DHSH");
	p = reinterpret_cast<GameShareContent*>(::MapViewOfFile(_hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, sizeof(GameShareContent)));
	p = new GameShareContent;
	ZeroMemory(p, sizeof(GameShareContent));
	::GetCurrentDirectoryW(MAX_PATH, p->wszConsolePath);
}
