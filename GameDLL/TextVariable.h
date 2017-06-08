#ifndef __DHSH_GAMEDLL_GAME_TEXT_VARIABLE_TEXTVARIABLE_H__
#define __DHSH_GAMEDLL_GAME_TEXT_VARIABLE_TEXTVARIABLE_H__

#include "GameBase.h"

class CTextVariable : public MyTools::CRelfexBaseClass, public MyTools::CVariable<em_TextVar, std::wstring>
{
public:
	CTextVariable() = default;
	~CTextVariable() = default;
	
	virtual std::wstring GetEmptyValue() CONST throw() override;
public:
	static CTextVariable* CreateInstance()
	{
		return new CTextVariable;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CTextVariable*>(lpObjectAddr);
	}
	
	CTextVariable(CONST CTextVariable&) = delete;
	
	CTextVariable& operator=(CONST CTextVariable&) = delete;
};



#endif // !__DHSH_GAMEDLL_GAME_TEXT_VARIABLE_TEXTVARIABLE_H__
