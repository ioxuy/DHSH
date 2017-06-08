#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_FIELD_FARMFIELD_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_FIELD_FARMFIELD_H__

#include "GameBase.h"

class CFarmField
{
private:
	struct FieldConfig
	{
		std::wstring wsMapName;
		Point MapPoint;
	};
public:
	CFarmField() = default;
	~CFarmField() = default;
	
	// 
	BOOL Run() CONST;
	
	// 
	BOOL Check() CONST;

private:
	BOOL GetFieldConfig(_Out_ FieldConfig& FieldConfig_) CONST;
};

#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_FIELD_FARMFIELD_H__
