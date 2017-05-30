#include "stdafx.h"
#include "ConsoleVariable.h"
#include <MyTools/Character.h>

CConsoleVariable::CConsoleVariable() : _hFileMap(INVALID_HANDLE_VALUE)
{

}

CConsoleVariable::~CConsoleVariable()
{
	if (_hFileMap != INVALID_HANDLE_VALUE)
	{
		::UnmapViewOfFile(GetSareContent());
		GetSareContent() = nullptr;

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
	_hFileMap = ::OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SZSHARENAME_CONSOLE);
	if (_hFileMap == NULL || _hFileMap == INVALID_HANDLE_VALUE)
		_hFileMap = ::CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, sizeof(GameShareContent), SZSHARENAME_CONSOLE);
	
	p = reinterpret_cast<GameShareContent*>(::MapViewOfFile(_hFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, sizeof(GameShareContent)));
	ZeroMemory(p, sizeof(GameShareContent));
	::GetCurrentDirectoryW(MAX_PATH, p->wszConsolePath);
}
