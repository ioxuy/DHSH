#ifndef __DHSH_GAMEDLL_GAME_METHOD_ACTION_LOGICBAGITEMACTION_H__
#define __DHSH_GAMEDLL_GAME_METHOD_ACTION_LOGICBAGITEMACTION_H__

#include "GameBase.h"

class CLogicBagItemAction : public MyTools::CRelfexBaseClass
{
public:
	CLogicBagItemAction() = default;
	~CLogicBagItemAction() = default;
	
	// 战斗后的物品处理
	BOOL AfterFight_Item() CONST;

	// 驱魔香检查
	BOOL CheckExorcism() CONST;

	// 装备持久度检查?


	// 先检查物品是否充足, 背包位置是否足够之类的……!
	BOOL Check() CONST;

	// 购买补充物品
	BOOL SupplementItem(_In_ CONST std::wstring& wsItemName, _In_ UINT uCount) CONST;

	// 去除驱魔香效果
	VOID RemoveExorcism() CONST;
public:
	static CLogicBagItemAction* CreateInstance()
	{
		return new CLogicBagItemAction;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CLogicBagItemAction*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CLogicBagItemAction";
		return wsClassName;
	}

	CLogicBagItemAction(CONST CLogicBagItemAction&) = delete;

	CLogicBagItemAction& operator=(CONST CLogicBagItemAction&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_ACTION_LOGICBAGITEMACTION_H__
