#ifndef __DHSH_GAMEDLL_GAME_METHOD_CALL_GAMECALL_H__
#define __DHSH_GAMEDLL_GAME_METHOD_CALL_GAMECALL_H__

#include "GameBase.h"

class CGameCALL : public MyTools::CRelfexBaseClass
{
public:
	CGameCALL() = default;
	~CGameCALL() = default;
	
	// 给予
	BOOL Give(_In_ DWORD dwUiBase, _In_ DWORD dwIndex) CONST;
	
	// 打地鼠
	BOOL WhacAMole(_In_ DWORD dwIndex, _In_ DWORD dwCmd, _In_ DWORD dwNonw) CONST;

	// 打开给予的窗口
	BOOL OpenGiveGameUi(_In_ DWORD dwUiBase) CONST;

	// Send
	BOOL Send(_In_ CONST CHAR* Buffer) CONST;

	// 邮寄
	BOOL Mail(_In_ CONST std::wstring& wsBoxName, _In_ CONST std::wstring& wsPlayerName, _In_ DWORD dwId, _In_ DWORD dwPlayerId) CONST;

	// Close Game UI
	BOOL CloseGameUi(_In_ DWORD dwUiBase) CONST;

	// 给予
	BOOL Give_Send(_In_ DWORD dwNpcId, _In_ DWORD dwId) CONST;

	// 检查坐标点是否能降落(针对坐骑)
	BOOL CheckPointIsWork(_In_ CONST Point& Point_) CONST;

	// 逃跑
	BOOL RunAway() CONST;

	// 防御
	BOOL Defence() CONST;

	// 
	BOOL ClickMonster(_In_ DWORD dwObjAddr) CONST;

	// 卖
	BOOL SellToNpc(_In_ DWORD dwItemId, _In_ DWORD dwNpcId, _In_ DWORD dwCount) CONST;

	// 拾荒
	BOOL CollectScraps(_In_ DWORD dwId) CONST;

	// 放弃任务
	BOOL AbandonTask(_In_ DWORD dwTaskId) CONST;

	// 设置施法技能
	BOOL SetUseSkill(_In_ DWORD dwSkillId) CONST;

	// 关闭组队
	BOOL CloseTeam(_In_ DWORD dwBase) CONST;

	// 设置自动施法
	BOOL SetAutoUseSkill() CONST;

	// 捕抓
	BOOL CatchMonster() CONST;

	// 快捷键
	BOOL SetHotKey(_In_ DWORD dwKeyValue) CONST;

	// 打开Npc
	BOOL OpenNpc(_In_ DWORD dwNpcId) CONST;

	// 刷新验证码
	BOOL FreshVerCode() CONST;

	// Save
	BOOL Save(_In_ DWORD dwItemId, _In_ DWORD dwCount) CONST;

	// Add Hp
	BOOL AddHp(_In_ DWORD dwActionId) CONST;

	// 分解装备
	BOOL DecompostionEqui(_In_ DWORD dwId) CONST;

	// 参战
	BOOL EnterWar(_In_ DWORD dwPetId) CONST;

	// 销毁
	BOOL Destory(_In_ DWORD dwItemId, _In_ DWORD dwCount) CONST;

	// 唤神
	BOOL CallGod() CONST;

	// 合成宝石
	BOOL SynthesisGem(_In_ DWORD dwResId1, _In_ DWORD dwResId2) CONST;

	// 包裹提取
	BOOL ExtractItem(_In_ DWORD dwItemId) CONST;

	// 使用箱子
	BOOL UseItem(_In_ DWORD dwItemId) CONST;

	// 使用法宝
	BOOL UseMagicWeapon(_In_ DWORD dwItemId) CONST;

	// 猜拳
	BOOL Mora(_In_ DWORD dwIndex) CONST;

	// 自动
	BOOL SetAutoAction() CONST;

	// 购买法宝
	BOOL Buy_By_Npc(_In_ DWORD dwNpcId, _In_ DWORD dwLocation, _In_ DWORD dwCount) CONST;

	// 答题
	BOOL AnswerQuestion(_In_ DWORD dwAnswer, _In_ DWORD dwBase) CONST;

	// 关闭Npc对话
	BOOL CloseNpcDlg() CONST;

	// 寻路
	BOOL MoveToNpc(_In_ LPCSTR pszMapName, _In_ LPCSTR pszNpcName) CONST;
	BOOL MoveToPoint(_In_ CONST Point& Point_) CONST; // Current Map
	BOOL MoveToPoint(_In_ LPCSTR pszMapName, _In_ CONST Point& Point_) CONST;
	BOOL MoveToPoint_Mouse(_In_ CONST Point& TarPoint) CONST;

	// 采集
	BOOL CollectItem(_In_ DWORD dwObjAddr) CONST;
	BOOL CollectFurniture(_In_ DWORD dwId) CONST;


	// 点击
	BOOL ClickNpcOption(_In_ DWORD dwNpcId, _In_ LPCSTR pszOptionText) CONST;

	// 返回宠物名字
	CONST CHAR* GetPetName_By_SkillId(_In_ DWORD dwSkillId) CONST;

	// AutoMove
	BOOL SetAutoMove(_In_ BOOL bRun) CONST;

	//
public:
	static CGameCALL* CreateInstance()
	{
		return new CGameCALL;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CGameCALL*>(lpObjectAddr);
	}
};

#endif // !__DHSH_GAMEDLL_GAME_METHOD_CALL_GAMECALL_H__
