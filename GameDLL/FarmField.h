#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_FIELD_FARMFIELD_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_FIELD_FARMFIELD_H__

#include "GameBase.h"

class CFarmField : public MyTools::CRelfexBaseClass
{
public:
	CFarmField() = default;
	~CFarmField() = default;
	
	// 
	BOOL Run(_In_ CONST std::wstring& wsMapName, _In_ CONST Point& TarPoint) CONST;
	
	// 
	BOOL Check() CONST;
public:
	static CFarmField* CreateInstance()
	{
		return new CFarmField;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CFarmField*>(lpObjectAddr);
	}

	CFarmField(CONST CFarmField&) = delete;
	
	CFarmField& operator=(CONST CFarmField&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_FIELD_FARMFIELD_H__
