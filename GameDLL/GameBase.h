#ifndef __DHSH_GAMEDLL_GAME_COMMON_GAMEBASE_H__
#define __DHSH_GAMEDLL_GAME_COMMON_GAMEBASE_H__

#include <MyTools/RelfexClass.h>

#define C_renwu_zhanghao  0x919618
#define C_renwu_qu        0x919684
#define C_renwu_name      0x91964C
#define C_renwu_base      0x8F3E88
#define C_renwu_DQHP      0x5E8
#define C_renwu_职业      0x4F8
#define C_renwu_MHP       C_renwu_DQHP + 0x0C
#define C_renwu_DQMP      C_renwu_DQHP + 04 
#define C_renwu_MMP       C_renwu_DQHP + 0x10 
#define C_renwu_lev       C_renwu_DQHP - 8 
#define C_renwu_ID        C_renwu_DQHP - 0x0F4
#define C_renwu_qianli    C_renwu_DQHP - 0x0C
///////////////////////////////////////////////////////技能
#define C_renwu_Skill_base   0x8F40C0
#define C_Skill_office       0x498
#define C_Skill_Num          0x264

#define C_Pet_Skill_base     0x9417E0
#define C_Get_Skill_name_CALL  0x006CC1A0
////////////////////////////////////////////////////////
//宠物
///////////////////////////////////////////////////////
#define C_chong_base     0x8F7898
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
#define C_chuzhan_base   0x8F7DCC//?
#define C_chuzhan_dq     0x5Bc
#define C_EXP            0x588//[588]+258
#define C_chuzhan_MAX    0x5C4
////////////////////////////////////////////////////
//地图数据
///////////////////////////////////////////////////
#define C_map_base       0x8F292C    //+10 窗口句柄
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
#define C_money_base     0xAE5C64
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
#define C_Box_base      0x8F6BE8//?
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
#define	C_huan_base     0x91F168
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
#define C_state_base    0x8F3ED0//?
#define C_state_yiji    0x4a0
#define C_state_erji    0x290
#define C_state_shu     0xac   //290+ac 是数量
#define C_state_surp1   0x78
#define C_state_surp    0x24
#define C_state_name    0x8
////////////////////////////////////////////
//NPC 状态
////////////////////////////////////////////
#define C_NPC_state    0x8F4050// ?
#define C_NPC_xuan_base  0x454
#define C_NPC_xuan_shu   0x458
#define C_NPC_xuan_str   0x1c0
#define C_NPC_xuan_dianji 0x50
#define C_NPC_state1      0x16C
#define C_NPC_neirong     0x148
////////////////////////////////////////////
//任务
////////////////////////////////////////////
#define C_task_base      0x8F7DB4
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
#define C_caiji_base        0x8F6D84 //?
#define C_caiji_offset      0x8
#define C_caiji_chong_base   0x8F6D74

//#define C_chongwu_chushi    0x0050DEB0
//#define C_chongwu_base      0x0050CE40 
#define C_chongwu_tili      0x26
#define C_caiji_CALL_chushi 0x0051BF60
#define C_caiji_CALL        0x0051C1F0
////////////////////////////////////////////
//UI 怪物数组
////////////////////////////////////////////
#define C_Monster_base           0x8F40F0
#define C_Monster_UI             0x49C
#define C_Monster_UI_1           0x290
///////////////////////////////////////////////
/////遍历窗口
///////////////////////////////////////////////
#define C_window_base         0xAE5B78 //?
#define C_window_offset       0x0C8


/////////////////////////////////////////////
//队伍
////////////////////////////////////////////
#define C_Team_base               0x8F7DE4
#define C_Team_暂离               0x75 //1为暂离 0为组队
#define C_Team_Lev                0x14
#define C_Team_Map                0x7E
#define C_Team_ID                 0x18
#define C_Team_申请               0x2F0
////////////////////////////////////////////
//CALL 数据
////////////////////////////////////////////

//#define C_CALL_dati         0x007491D0  0x5332B0
#define C_CALL_dati         0x5338F0 //答题三CALL 上层
#define C_CALL_dati1        0x007D28C5
#define C_CALL_dati2        0x00533880
#define C_CALL_fabao_base   0x8F2930
#define C_CALL_fabao        0x0064E360
#define C_CALL_close_maimai  C_NPC_state
#define C_CALL_CloseNPC     0x00750F80
//#define C_CALL_hanhua       0x005FEAD0
#define C_CALL_zoulu        0x0042FC30
#define  C_xunlu_base       0x8F3E44 //C_CALL_xunlu1 内
#define C_CALL_xunlu1       0x0042FB80
#define C_CALL_xunlu        0x00430450
#define C_call_dianji       0x0064EE10//?
#define C_Set_Skill_base    0x8F4098
#define C_CALL_Set_Skill    0x00489650
#define C_base_kuaijie      0x8F6C9C// 快捷键基址 ??
#define C_CALL_kuaijie      0x004F9CC0// 快捷键CALL ??
#define C_base_dianguai     0x8F9124//战斗点击怪物基址 //?
#define C_zhandou_huihe     0x0c      //战斗回合数
#define C_base_xing         0x40      // 快捷键形态
#define C_base_baobaoxing   0x84     //宝宝快捷键形态
#define	C_base_zhuangtai    0x10      // 战斗场景人物状态 5,人物未攻击  6宠物未攻击 0 未进入战斗  3人物正在攻击
#define C_CALL_dianguai     0x00640B40// 技能点击怪物
#define C_CALL_ADDHP        0x0064EF30
#define C_CALL_maichu      C_CALL_ADDHP
#define C_chaxun_Offset     0x6E4 //--
#define C_chushi_Offset     0x4C8 //--
#define C_CALL_chushi       0x0053A130 //--
#define C_CALL_chaxun       0x00539EE0 //--
#define C_call_jianwu       C_CALL_ADDHP   //
#define C_CALL_taopao       0x00640F10

//#define C_jiasu_xuhanshu    0x7E8644
#define C_zidong_base       0x8F9120
#define C_CALL_zidong       0x0063DE00

#define C_Auto_Skill        C_CALL_taopao

#define C_Auto_Skill_      0x00643E30
#define C_CALL_IsFly       0x006AAA70//0x00693580

#define C_CALL_NPC_寻路CALL  C_CALL_xunlu1
#define C_CALL_NPC_寻路CALL1  0x00430710


#define C_CALL_猜拳          0x00640130

#define C_CALL_Open_give    0x004C9630
#define C_CALL_Start_打地鼠  0x0064EF90
#define C_CALL_give_Enter            0x004C9320//确认给予
#define C_CALL_Give_Set            0x0076BF30
#define C_CALL_Give_Set1            0x0076F330
#define C_CALL_Give_Set2           0x0076C1E0



#define C_verify_Base           0x909608
#define C_CALL_Key_InPut        0x00771500
#define C_CALL_Login_Base       0x8F6CAC
#define C_CALL_Login             0x004FC380
#define C_CALL_Check_Btn        0x0077E020


#define ReadDWORD(Addr) MyTools::CCharacter::ReadDWORD(Addr)


enum em_GameStatus
{
	em_GameStatus_None,
	em_GameStatus_Running,
	em_GameStatus_Stop
};

extern em_GameStatus g_GameStatus;
#define GameRun (g_GameStatus == em_GameStatus::em_GameStatus_Running)
#define StopGame  g_GameStatus = em_GameStatus::em_GameStatus_Stop

struct Point
{
	DWORD X, Y;

	Point()
	{
		X = Y = NULL;
	}

	Point(_In_ DWORD dwX, _In_ DWORD dwY) : X(dwX), Y(dwY)
	{

	}

	bool operator == (CONST Point& Point_) CONST
	{
		return this->X == Point_.X && this->Y == Point_.Y;
	}

	bool operator != (CONST Point& Point_) CONST
	{
		return this->X != Point_.X || this->Y != Point_.Y;
	}
};

struct ResNpcMapPointText
{
	std::wstring wsNpcName;
	std::wstring wsMapName;
	std::wstring wsGlobleMapName;
	Point        FixPoint;
};


struct ResShopEquiText
{
	std::wstring wsEquiName;
	DWORD		 dwStoreIndex;
	DWORD		 dwItemIndex;
};

#endif
