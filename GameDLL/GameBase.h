#ifndef __DHSH_GAMEDLL_GAME_COMMON_GAMEBASE_H__
#define __DHSH_GAMEDLL_GAME_COMMON_GAMEBASE_H__

#include <MyTools/RelfexClass.h>

#define C_renwu_zhanghao  0x91C644
#define C_renwu_qu        0x91C6B0
#define C_renwu_name      0x91C678
#define C_renwu_base      0x8F6EA8
#define C_renwu_DQHP      0x5E8
#define C_renwu_职业      0x4F8
#define C_renwu_MHP       C_renwu_DQHP + 0x0C
#define C_renwu_DQMP      C_renwu_DQHP + 04 
#define C_renwu_MMP       C_renwu_DQHP + 0x10 
#define C_renwu_lev       C_renwu_DQHP - 8 
#define C_renwu_ID        C_renwu_DQHP - 0x0F4
#define C_renwu_qianli    C_renwu_DQHP - 0x0C
///////////////////////////////////////////////////////技能
#define C_renwu_Skill_base   0x8F70E0
#define C_Skill_office       0x498
#define C_Skill_Num          0x264

#define C_Pet_Skill_base     0x944808
#define C_Get_Skill_name_CALL  0x006CDBC0
////////////////////////////////////////////////////////
//宠物
///////////////////////////////////////////////////////
#define C_chong_base     0x8FA8C0
#define C_chong_yiji     0x598
#define C_chong_shu      C_chong_yiji + 4
#define C_chong_DQHP     0x46
#define C_chong_MHP      0x52
#define C_chong_DQMP     0x4A
#define C_chong_MMP      0x56
#define C_chong_ID       0x0
#define C_chong_zhongcheng 0x1D
#define C_chong_name     0x9
#define C_chong_qianli   0x40
#define C_chong_chuzhan  0x7C
/////////////////////////////////////////////////////
//当前出战宠物数据
/////////////////////////////////////////////////////
#define C_chuzhan_base   0x8FADF4//?
#define C_chuzhan_dq     0x5Bc
#define C_EXP            0x588//[588]+258
#define C_chuzhan_MAX    0x5C4
////////////////////////////////////////////////////
//地图数据
///////////////////////////////////////////////////
#define C_map_base       0x8F594C    //+10 窗口句柄
#define C_map_name       0x184
#define C_map_Addr       0x910
#define C_map_AddX       0x2a
#define C_map_AddY       0x2e
#define C_ZOU_pi_1       0xA8
#define C_ZOU_pi_2       0x20

#define C_map_isRun      0x74//地图是否能寻路到
///////////////////////////////////////////////////
//加速
///////////////////////////////////////////////////
#define C_jiasu          0x1BC
#define C_dizhi          0x00666B00
#define C_duiyuan_HOOK   0x005FA556
///////////////////////////////////////////////////
//金钱
///////////////////////////////////////////////////
#define C_money_base     0xAE8C88
#define C_money_yiji     0x10
#define C_money_erji     0x14
#define C_money_siji     0x10
#define C_money_wuji     0x10
#define C_money_liuji    4
#define C_money_jiao     8
#define C_money_money    0
//////////////////////////////////////////////////
//背包
/////////////////////////////////////////////////
#define C_Box_base      0x8F9C08//?
#define C_Box_yiji      4
#define C_Box_num       8
#define C_Box_jia       0xc
#define C_BOX_name1     0x4b
#define C_box_name      8
#define C_Box_lei       4
#define C_Box_ID        0
#define C_BOX_shuxing1  0x33
//////////////////////////////////////////////////
//环境
//////////////////////////////////////////////////
#define	C_huan_base     0x922190
#define C_huan_yiji     4
#define C_huan_name     4
#define C_huan_ID       0x24
#define C_huan_X        C_map_AddX
#define C_huan_Y        C_map_AddY
#define C_huan_tiaojian 0x33
#define C_huan_si       0x78 //1为死亡
//////////////////////////////////////////////
//状态
//////////////////////////////////////////////
#define C_state_base    0x8F6EF0//?
#define C_state_yiji    0x4a0
#define C_state_erji    0x290
#define C_state_shu     0xac   //290+ac 是数量
#define C_state_surp1   0x78
#define C_state_surp    0x24
#define C_state_name    0x8
////////////////////////////////////////////
//NPC 状态
////////////////////////////////////////////
#define C_NPC_state    0x8F7070// ?
#define C_NPC_xuan_base  0x454
#define C_NPC_xuan_shu   0x458
#define C_NPC_xuan_str   0x1c0
#define C_NPC_xuan_dianji 0x50
#define C_NPC_state1      0x16C
#define C_NPC_neirong     0x148
////////////////////////////////////////////
//任务
////////////////////////////////////////////
#define C_task_base      0x8FADDC
#define C_task_yiji      0x149C
#define C_task_erji      0x390
#define C_task_name      0x54
#define C_task_shu       C_task_erji + 4
#define C_task_tishi     0x104
#define C_task_tishi1    0x24
////////////////////////////////////////////
//答题
////////////////////////////////////////////
//#define C_dati_base      0x9D7100
//#define C_yanzheng_yiji  0xc0
//#define C_zudui_kuang    0x1a0
//#define C_yanzheng_erji  0xc4
#define C_dati_yiji      0x4A8
#define C_dati_erji      C_state_erji
//
//#define C_dati_state     0x83709C
//#define C_dati_kuang     0x210
///////////////////////////////////////////
//商会
///////////////////////////////////////////
#define C_shanghui_base  0x83DEA8
#define C_shanghui_Offset 0x484
#define C_shanghui_Offset1 0x278
#define C_shanghui_Offset2 0x9c
#define C_shanghui_name    0x4
#define C_shanghui_jiage   0x2b
#define C_shanghui_win_zhuangtai   0x160
///////////////////////////////////////////
//采集
///////////////////////////////////////////
#define C_caiji_base        0x8F9DA4//?
#define C_caiji_offset      0x8
#define C_caiji_chong_base   0x8F9D94

//#define C_chongwu_chushi    0x0050DEB0
//#define C_chongwu_base      0x0050CE40 
#define C_chongwu_tili      0x26
#define C_caiji_CALL_chushi 0x0051C200
#define C_caiji_CALL        0x0051C490
////////////////////////////////////////////
//UI 怪物数组
////////////////////////////////////////////
#define C_Monster_base           0x8F7110
#define C_Monster_UI             0x49C
#define C_Monster_UI_1           0x290
///////////////////////////////////////////////
/////遍历窗口
///////////////////////////////////////////////
#define C_window_base         0xAE8BA0//?
#define C_window_offset       0x0C8


/////////////////////////////////////////////
//队伍
////////////////////////////////////////////
#define C_Team_base               0x8F95A4
#define C_Team_暂离               0x75 //1为暂离 0为组队
#define C_Team_Lev                0x14
#define C_Team_Map                0x7E
#define C_Team_ID                 0x18
#define C_Team_申请               0x2F0
////////////////////////////////////////////
//CALL 数据
////////////////////////////////////////////

//#define C_CALL_dati         0x007491D0  0x5332B0
#define C_CALL_dati         0x533B10 //答题三CALL 上层
#define C_CALL_dati1        0x007D4915
#define C_CALL_dati2        0x00533AA0
#define C_CALL_fabao_base   0x8F5950
#define C_CALL_fabao        0x0064FBE0
#define C_CALL_close_maimai  C_NPC_state
#define C_CALL_CloseNPC     0x00752970
//#define C_CALL_hanhua       0x005FEAD0
#define C_CALL_zoulu        0x0042F570
#define  C_xunlu_base       0x8F6E64 //C_CALL_xunlu1 内
#define C_CALL_xunlu1       0x0042F4C0
#define C_CALL_xunlu        0x0042FD50
#define C_call_dianji       0x006506D0//?
#define C_Set_Skill_base    0x8F70B8
#define C_CALL_Set_Skill    0x00489340
#define C_base_kuaijie      0x8F9CBC// 快捷键基址 ??
#define C_CALL_kuaijie      0x004F9DE0// 快捷键CALL ??
#define C_base_dianguai     0x8FC14C//战斗点击怪物基址 //?
#define C_zhandou_huihe     0x0c      //战斗回合数
#define C_base_xing         0x40      // 快捷键形态
#define C_base_baobaoxing   0x84     //宝宝快捷键形态
#define	C_base_zhuangtai    0x10      // 战斗场景人物状态 5,人物未攻击  6宠物未攻击 0 未进入战斗  3人物正在攻击
#define C_CALL_dianguai     0x00642550// 技能点击怪物
#define C_CALL_ADDHP        0x006507F0
#define C_CALL_maichu      C_CALL_ADDHP
#define C_chaxun_Offset     0x6E4 //--
#define C_chushi_Offset     0x4C8 //--
#define C_CALL_chushi       0x0053A130 //--
#define C_CALL_chaxun       0x00539EE0 //--
#define C_call_jianwu       C_CALL_ADDHP   //
#define C_CALL_taopao       0x00642920

//#define C_jiasu_xuhanshu    0x7E8644
#define C_zidong_base       0x8FC148
#define C_CALL_zidong       0x0063F820

#define C_Auto_Skill        C_CALL_taopao

#define C_Auto_Skill_      0x006457E0
#define C_CALL_IsFly       0x006AC350//0x00693580

#define C_CALL_NPC_寻路CALL  C_CALL_xunlu1
#define C_CALL_NPC_寻路CALL1  0x0042FFE0


#define C_CALL_猜拳          0x00641B40

#define C_CALL_Open_give    0x004C9950
#define C_CALL_Start_打地鼠  0x00650850
#define C_CALL_give_Enter            0x004C9690//确认给予
#define C_CALL_Give_Set            0x0076EE80
#define C_CALL_Give_Set1            0x00772290
#define C_CALL_Give_Set2           0x0076F140



#define C_verify_Base           0x90C630
#define C_CALL_Key_InPut        0x00774460
#define C_CALL_Login_Base       0x8F9CCC
#define C_CALL_Login             0x004FC480
#define C_CALL_Check_Btn        0x00780F50

#define 自动走路CALL 0x409130
#define NPC选项文本偏移 0x24C
#define NPC选项发包偏移 0x58
#define 家园基址 0x8F9B60
#define 家园偏移 0x498
#define 家园活力偏移 0x2B
#define 是否屏蔽玩家基址 0x9221A4
#define 采集家具CALL 0x51C3A0
#define 家具采集偏移 0x8104E4

#define ReadDWORD(Addr) MyTools::CCharacter::ReadDWORD(Addr)

#define WM_CUSTOME_HOME 0x400 + 0x52
#define WM_CUSTOME_END  WM_CUSTOME_HOME + 0x1
#define WM_CUSTOME_READCONFIG WM_CUSTOME_END + 0x1
#define WM_CUSTOME_STOP WM_CUSTOME_READCONFIG + 0x1

#define SERVERIP L"192.168.0.104"
#define SERVERPORT 12345

#define SZSHARENAME_CONSOLE L"CL_SHARE_DHSH"

enum class em_GameStatus
{
	em_GameStatus_None,
	em_GameStatus_Running,
	em_GameStatus_Stop
};

extern em_GameStatus g_GameStatus;
#define GameRun (g_GameStatus == em_GameStatus::em_GameStatus_Running)
#define StopGame  g_GameStatus = em_GameStatus::em_GameStatus_Stop
#define StartGame g_GameStatus = em_GameStatus::em_GameStatus_Running

extern VOID GameSleep(_In_ DWORD dwTime);

enum class em_TextVar
{
	// 是否运行在测试模式下
	em_TextVar_IsRunDlg,
	// 是否使用驱魔香 = 1
	em_TextVar_UseExorcism,
	// 人物HP<70 吃药
	em_TextVar_Person_SupplementHp,
	// 人物MP<70 吃药
	em_TextVar_Person_SupplementMp,
	// 宠物HP<70 吃药
	em_TextVar_Pet_SupplementHp,
	// 宠物MP<70 吃药
	em_TextVar_Pet_SupplementMp,
	// 人物的战斗模式
	em_TextVar_PersonFightMode,
	// 宠物的战斗模式
	em_TextVar_PetFightMode,
	// 自动购买驱魔香
	em_TextVar_AutoBuyExorcism,
	// 自动购买欢悦铃
	em_TextVar_AutoBuyHappyBell,
	// 购买宠物饮料数量
	em_TextVar_AutoBuyPetDrinksCount,
	// 是否使用超程符
	em_TextVar_UseReturnSymbol,
	// 是否自动购买超程符
	em_TextVar_AutoBuyReturnSymbol,
	// 净水的类型 0=金钱 1=帮贡
	em_TextVar_PurifyWater_PromiseType,
	// 旺火的类型 0=金钱 1=帮贡
	em_TextVar_HotFire_PromiseType,
	// 采集的家具名称
	em_TextVar_CollectHome_Furniture,
	// 采集的物品名称
	em_TextVar_Collect_ItemName,
	// 采集的地图名称
	em_TextVar_Collect_MapName,
	// 野外的地图
	em_TextVar_Field_MapName,
	// 野外的坐标
	em_TextVar_Field_MapPoint,
	// 挂机命令
	em_TextVar_ConfigAction,
	// 屏蔽玩家
	em_TextVar_ShieldPlayer
};


#define GAMEVERSION 1.01f
enum em_Sock_Msg
{
	em_Sock_Msg_KeepLive = 0x1,
	em_Sock_Msg_QueryCard,
	em_Sock_Msg_Recharge,
	em_Sock_Msg_ReadConfig,
	em_Sock_Msg_WriteConfig,
	em_Sock_Msg_AccountLogin,
	em_Sock_Msg_GameLogin,
	em_Sock_Msg_Register,
	em_Sock_Msg_QueryHour,
	em_Sock_Msg_ServerText,
};


typedef struct _Point
{
	DWORD X, Y;

	_Point()
	{
		X = Y = NULL;
	}

	_Point(_In_ DWORD dwX, _In_ DWORD dwY) : X(dwX), Y(dwY)
	{

	}

	bool operator == (CONST _Point& Point_) CONST
	{
		return this->X == Point_.X && this->Y == Point_.Y;
	}

	bool operator != (CONST _Point& Point_) CONST
	{
		return this->X != Point_.X || this->Y != Point_.Y;
	}

	DWORD Encode()
	{
		return X << 16 | Y;
	}

	static _Point Decode(_In_ DWORD dwValue)
	{
		_Point Pt;
		Pt.Y = dwValue & 0xFFFF;
		Pt.X = dwValue >> 16;
		return Pt;
	}

}Point;


struct ConfigText
{
	std::wstring wsConfigName;
	std::wstring wsConfigValue;
};


///////Account Share///////////////////////////////////////////////////////////////////

struct GameAccountContent
{
	WCHAR wszPlayerName[64];
};

struct GameAccountStatus
{
	BOOL bExist;
	BOOL bLogining;
	BOOL bClose;
	UINT uLoginFaildCount;
	ULONGLONG ulOnlineTick;
	BOOL bAlive;
	DWORD dwPid;
	HWND hGameWnd;
	em_GameStatus emGameStatus;
};

enum em_PersonFightMode : int
{
	em_PersonFightMode_None = -1,
	em_PersonFightMode_FixF1,
	em_PersonFightMode_NormalAttack,
	em_PersonFightMode_SwitchF1F2
};

enum em_PetFightMode : int
{
	em_PetFightMode_None = -1,
	em_PersonFightMode_Denfence,
	em_PersonFightMode_Skill
};

struct GameAccountConfig
{
	
};

typedef struct GameAccountShareContent
{
	GameAccountContent AccountContent;
	GameAccountStatus  AccountStatus;
	GameAccountConfig  AccountConfig;
	UINT               uIndex;
}*PGameAccountContent;

struct GameShareGlobalConfig
{
	WCHAR	  wszAccountName[32];
	WCHAR     wszAccountPass[32];
	DWORD     dwToken;
	DWORD     dwHour;
	ULONGLONG ulTimeTick;
	DWORD     dwConsoleTitle;
};

enum 
{
	em_Max_GameAccount_Count = 10
};
typedef struct GameShareContent
{
	WCHAR wszConsolePath[MAX_PATH];
	WCHAR wszGamePath[MAX_PATH];

	UINT  uAccountCount;
	GameAccountShareContent arrGameArrount[em_Max_GameAccount_Count];
	GameShareGlobalConfig  GlobalConfig;
}*PGameShareContent;

#endif
