#include "stdafx.h"
#include "GameBase.h"

em_GameStatus g_GameStatus = em_GameStatus::em_GameStatus_None;

extern VOID GameSleep(_In_ DWORD dwTime)
{
	for (decltype(dwTime) i = 0; i < dwTime; i += 100)
		::Sleep(100);
}
