#include "stdafx.h"
#include "GameVariable.h"
#include <MyTools/Character.h>

CGameVariable::CGameVariable()
{
	_hFileMap = INVALID_HANDLE_VALUE;
	_pGameShareContent = nullptr;
	_pGameAccountShareContent = nullptr;
}

CGameVariable::~CGameVariable()
{
	if (_hFileMap != INVALID_HANDLE_VALUE && _hFileMap != NULL)
	{
		::UnmapViewOfFile(_pGameShareContent);
		_pGameShareContent = nullptr;
		_pGameAccountShareContent = nullptr;

		::CloseHandle(_hFileMap);
		_hFileMap = INVALID_HANDLE_VALUE;
	}

	if(_pGameShareContent != nullptr)
	{
		delete _pGameShareContent;
		_pGameShareContent = nullptr;
	}
	if (_pGameAccountShareContent != nullptr)
	{
		delete _pGameAccountShareContent;
		_pGameAccountShareContent = nullptr;
	}
}


VOID CGameVariable::InitVariable() CONST
{
	// Ä¬ÈÏÊ¹ÓÃÇýÄ§Ïã
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_UseExorcism, 0);

	// 
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Person_SupplementHp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Person_SupplementMp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementHp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_Pet_SupplementMp, 70);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_PersonFightMode, static_cast<DWORD>(em_PersonFightMode::em_PersonFightMode_FixF1));

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_PetFightMode, static_cast<DWORD>(em_PetFightMode::em_PersonFightMode_Denfence));

	// 
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_AutoBuyExorcism, 0);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_AutoBuyPetDrinksCount, 30);

	//
	SetValueAndGetOldValue_By_Id(em_TextVar::em_TextVar_AutoBuyReturnSymbol, 0);
}

GameShareContent*& CGameVariable::GetGameShareContent()
{
	return _pGameShareContent;
}

GameAccountShareContent*& CGameVariable::GetAccountShareContent()
{
	return _pGameAccountShareContent;
}

BOOL CGameVariable::SetGameSharePtr()
{
	_hFileMap = ::OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SZSHARENAME_CONSOLE);
	if (_hFileMap == NULL || _hFileMap == INVALID_HANDLE_VALUE)
		return FALSE;

	_pGameShareContent = static_cast<GameShareContent *>(::MapViewOfFile(_hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, sizeof(GameShareContent)));
	return _pGameShareContent != nullptr;
}

BOOL CGameVariable::SetAccountGameShare(_In_ CONST std::wstring& wsPlayerName)
{
	// Is Exist 
	for (UINT i = 0;i < _pGameShareContent->uAccountCount; ++i)
	{
		if (_pGameShareContent->arrGameArrount[i].AccountStatus.bExist)
		{
			if (wsPlayerName == std::wstring(_pGameShareContent->arrGameArrount[i].AccountContent.wszPlayerName))
			{
				SetAccountGameShare(i, wsPlayerName);
				return TRUE;
			}
		}
	}

	// Create New
	for (int i = 0;i < em_Max_GameAccount_Count; ++i)
	{
		// Fill UnExist
		if (!_pGameShareContent->arrGameArrount[i].AccountStatus.bExist)
		{
			_pGameShareContent->uAccountCount += 1;
			SetAccountGameShare(i, wsPlayerName);
			return TRUE;
		}

		// Fill Exist!
		if (::GetTickCount64() - _pGameAccountShareContent->AccountStatus.ulOnlineTick >= 20 * 1000)
		{
			SetAccountGameShare(i, wsPlayerName);
			return TRUE;
		}

	}

	return FALSE;
}

VOID CGameVariable::SetAccountGameShare(_In_ UINT uIndex, _In_ CONST std::wstring& wsPlayerName)
{
	_pGameAccountShareContent = &_pGameShareContent->arrGameArrount[uIndex];
	_pGameAccountShareContent->uIndex = uIndex;
	_pGameAccountShareContent->AccountStatus.bExist = TRUE;
	_pGameAccountShareContent->AccountStatus.dwPid = ::GetCurrentProcessId();
	MyTools::CCharacter::wstrcpy_my(_pGameAccountShareContent->AccountContent.wszPlayerName, wsPlayerName.c_str());
}

