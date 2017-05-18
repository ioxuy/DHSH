#ifndef __DHSH_GAMEDLL_TEST_TESTFUNCTION_H__
#define __DHSH_GAMEDLL_TEST_TESTFUNCTION_H__

#include "GameBase.h"

class CTestFunction : public MyTools::CRelfexBaseClass
{
public:
	CTestFunction() = default;
	~CTestFunction() = default;
	
	// 
	VOID InitTestShareContent() CONST;
	
	
public:
	static CTestFunction* CreateInstance()
	{
		return new CTestFunction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CTestFunction*>(lpObjectAddr);
	}

	CTestFunction(CONST CTestFunction&) = delete;
	
	CTestFunction& operator=(CONST CTestFunction&) = delete;
};

#endif // !__DHSH_GAMEDLL_TEST_TESTFUNCTION_H__
