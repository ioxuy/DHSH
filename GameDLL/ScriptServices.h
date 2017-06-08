#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_SCRIPTSERVICES_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_SCRIPTSERVICES_H__

#include "GameBase.h"
#include "TextVariable.h"

class CScriptServices
{
public:
	CScriptServices() = default;
	~CScriptServices() = default;

	BOOL Run();

private:
	template<typename T>
	BOOL RunPtr()
	{
		T Ptr;
		return Ptr.Run();
	}

};



#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_SCRIPTSERVICES_H__
