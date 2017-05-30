#ifndef __DHSH_GAMEDLL_GAME_METHOD_SCANBASE_H__
#define __DHSH_GAMEDLL_GAME_METHOD_SCANBASE_H__

#include "GameVariable.h"

class CScanBase : public MyTools::CRelfexBaseClass
{
public:
	CScanBase() = default;
	~CScanBase() = default;
	
	VOID Start() CONST;
	
	
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CScanBase*>(lpObjectAddr);
	}
	
	CScanBase(CONST CScanBase&) = delete;
	
	CScanBase& operator=(CONST CScanBase&) = delete;
};



#endif // !__DHSH_GAMEDLL_GAME_METHOD_SCANBASE_H__
