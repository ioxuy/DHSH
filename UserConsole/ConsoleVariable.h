#ifndef __DHSH_USERCONSOLE_CODE_VARIABLE_CONSOLEVARIABLE_H__
#define __DHSH_USERCONSOLE_CODE_VARIABLE_CONSOLEVARIABLE_H__

#include <MyTools/ClassInstance.h>
#include "../GameDLL/GameBase.h"


enum class em_ConVar
{

};

class CConsoleVariable : public MyTools::CVariable<em_ConVar,DWORD>, public MyTools::CClassInstance<CConsoleVariable>
{
public:
	CConsoleVariable();
	~CConsoleVariable();


	PGameShareContent& GetSareContent() CONST;

	//
	VOID InitShareContent();
private:
	HANDLE _hFileMap;
};


#endif // !__DHSH_USERCONSOLE_CODE_VARIABLE_CONSOLEVARIABLE_H__
