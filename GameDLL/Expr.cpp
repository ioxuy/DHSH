#include "stdafx.h"
#include "Expr.h"
#include <future>
#include <MyTools/Log.h>
#include <MyTools/CLThread.h>
#include "PlayerExtend.h"
#include "Player.h"
#include "Npc.h"
#include "PlayerMove.h"
#include "GameCALL.h"
#include "PersonAttribute.h"
#include "Skill.h"
#include "SkillExtend.h"
#include "PersonPet.h"
#include "PersonPetExtend.h"
#include "HotKey.h"
#include "HotKeyExtend.h"
#include "TestFunction.h"
#include "GameVariable.h"
#include "FarmField.h"
#include "LogicBagItemAction.h"
#include "GameUi.h"
#include "GameUiExtend.h"
#include "BagItem.h"
#include "BagItemExtend.h"
#include "ExcuteAction.h"
#include "NpcExtend.h"
#include "MonsterExtend.h"
#include "CollectItem.h"
#include "ItemFilter.h"
#include "BagItemAction.h"
#include "Scanbase.h"
#include "Task.h"
#include "TaskExtend.h"
#include "BangTask.h"
#include "PurifyWaterTask.h"
#include "HotFireTask.h"

#define _SELF L"Expr.cpp"
CExpr::CExpr()
{

}

CExpr::~CExpr()
{

}

VOID CExpr::Release()
{

}

std::vector<MyTools::ExpressionFunPtr>& CExpr::GetVec()
{
	static std::vector<MyTools::ExpressionFunPtr> Vec = 
	{
		{ std::bind(&CExpr::Help,this, std::placeholders::_1),L"Help" },
		{ std::bind(&CExpr::AroundObject,this, std::placeholders::_1),L"AroundObject" },
		{ std::bind(&CExpr::SetGameRun,this, std::placeholders::_1),L"SetGameRun" },
		{ std::bind(&CExpr::TestPtr,this, std::placeholders::_1),L"TestPtr" },
		{ std::bind(&CExpr::PrintUiDlg,this, std::placeholders::_1),L"PrintUiDlg" },
		{ std::bind(&CExpr::PrintBuff,this, std::placeholders::_1),L"PrintBuff" },
		{ std::bind(&CExpr::ScanBase,this, std::placeholders::_1),L"ScanBase" },
		{ std::bind(&CExpr::PrintTask,this, std::placeholders::_1),L"PrintTask" },
		{ std::bind(&CExpr::PrintCurMap,this, std::placeholders::_1),L"PrintCurMap" },
		{ std::bind(&CExpr::PrintBag,this, std::placeholders::_1),L"PrintBag" },
	};
	
	return Vec;
}

VOID CExpr::Help(CONST std::vector<std::wstring>&)
{
	auto& Vec = GetVec();
	for (CONST auto& itm : Vec)
		LOG_C_D(L"MethodName=%s", itm.wsFunName.c_str());
}

VOID CExpr::AroundObject(CONST std::vector<std::wstring>&)
{
	std::vector<CPlayer> Vec;
	MyTools::InvokeClassPtr<CPlayerExtend>()->GetAroundObject(Vec, nullptr);

	for (CONST auto& itm : Vec)
		LOG_C_D(L"ID=%X,ResId=%X,Addr=%X, Name=%s,Point=[%d,%d],dis=%.2f,Type=[%X,%s]", itm.GetId(), itm.GetResId(), itm.GetNodeBase(), itm.GetName().c_str(), itm.GetPoint().X, itm.GetPoint().Y, itm.GetDis(), static_cast<DWORD>(itm.GetType()), itm.GetTextType().c_str());
}

VOID CExpr::SetGameRun(CONST std::vector<std::wstring>& VecParm)
{
	if (VecParm.size() == 0)
		return;

	g_GameStatus = VecParm.at(0) == L"1" ? em_GameStatus::em_GameStatus_Running : em_GameStatus::em_GameStatus_Stop;
}



VOID CExpr::PrintUiDlg(CONST std::vector<std::wstring>& VecParm)
{
	MyTools::InvokeClassPtr<CGameUiExtend>()->PrintGameUi();
}

VOID CExpr::PrintBuff(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<CPersonAttribute::PersonBuff> Vec;
	MyTools::InvokeClassPtr<CPersonAttribute>()->GetVecPersonBuff(Vec, nullptr);
	for (CONST auto& itm : Vec)
		LOG_C_D(L"Text=%s",itm.wsName.c_str());
}

VOID CExpr::ScanBase(CONST std::vector<std::wstring>&)
{
	std::async(std::launch::async, &CScanBase::Start, MyTools::InvokeClassPtr<CScanBase>());
}

VOID CExpr::TestPtr(CONST std::vector<std::wstring>& VecParm)
{
	MyTools::InvokeClassPtr<CGameVariable>()->InitVariable();

	StartGame;
	MyTools::InvokeClassPtr<CHotFireTask>()->Run();
}

VOID CExpr::PrintTask(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<CTaskObject> Vec;
	MyTools::InvokeClassPtr<CTaskExtend>()->GetTask(Vec, nullptr);

	for (CONST auto& itm : Vec)
		LOG_C_D(L"dwAddr=%X, Name=%s, Content=%s", itm.GetNodeBase(), itm.GetName().c_str(), itm.GetTaskContent().c_str());
}

VOID CExpr::PrintCurMap(CONST std::vector<std::wstring>& VecParm)
{
	LOG_C_D(L"%s",MyTools::InvokeClassPtr<CPersonAttribute>()->GetCurrentMapName().c_str());
}

VOID CExpr::PrintBag(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<CBagItem> Vec;
	MyTools::InvokeClassPtr<CBagItemExtend>()->GetVecBagItem(Vec, nullptr);

	for (CONST auto& itm : Vec)
	{
		LOG_C_D(L"Addr=[%X],ID=[%X],Name=[%s],Count=[%d],Quality=[%s]", \
			itm.GetNodeBase(), itm.GetItemId(), itm.GetName().c_str(), itm.GetCount(), itm.GetItemQuality().c_str());
	}
}
