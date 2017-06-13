#ifndef __DHSH_GAMEDLL_GAME_TEXT_CONFIG_GAMECONFIG_H__
#define __DHSH_GAMEDLL_GAME_TEXT_CONFIG_GAMECONFIG_H__

#include "GameBase.h"

class CGameConfig : public MyTools::CRelfexBaseClass
{
public:
	enum class em_Config_Type
	{
		em_Config_Type_Action,
		em_Config_Type_Field,
		em_Config_Type_Collect,
		em_Config_Type_HotFire,
		em_Config_Type_PurifyWater,
		em_Config_Type_BangTask,
		em_Config_Type_Exam,
		em_Config_Type_Fight,
		em_Config_Type_Common
	};

public:
	CGameConfig() = default;
	~CGameConfig() = default;
	
	//
	BOOL ExistConfigFile() CONST;

	//
	std::wstring GetConfigPath() CONST;

	//
	BOOL Initialize() CONST;
private:
	//
	BOOL GetConfigAppText(_In_ em_Config_Type emConfigType, _Out_ std::wstring& wsText) CONST;
	
	//
	BOOL SetConfigValue(_In_ em_Config_Type emConfigType, _In_ CONST std::wstring& wsConfigName, _In_ em_TextVar emTextVar, _In_ std::function<BOOL(em_TextVar, CONST std::wstring&)> SetValuePtr) CONST;

	//
	BOOL ReadConfig(_In_ em_Config_Type emConfigType, _In_ CONST std::wstring& wsConfigName, _Out_ std::wstring& wsConfigValue) CONST;


	BOOL ReadConfig_Field() CONST;

	//
	BOOL ReadConfig_Collect() CONST;

	//
	BOOL ReadConfig_BangTask() CONST;

	//
	BOOL ReadConfig_PurifyWater() CONST;

	//
	BOOL ReadConfig_HotFire() CONST;

	//
	BOOL ReadConfig_Exam() CONST;

	//
	BOOL SetFightConfig() CONST;

	//
	BOOL SetCommonConfig() CONST;
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CGameConfig*>(lpObjectAddr);
	}
	
	CGameConfig(CONST CGameConfig&) = delete;
	
	CGameConfig& operator=(CONST CGameConfig&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_TEXT_CONFIG_GAMECONFIG_H__
