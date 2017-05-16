#include "stdafx.h"
#include "Expr.h"
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
		LOG_C_D(L"ID=%X,Addr=%X, Name=%s,Point=[%d,%d],dis=%.2f,Type=[%X,%s]", itm.GetId(), itm.GetNodeBase(), itm.GetName().c_str(), itm.GetPoint().X, itm.GetPoint().Y, itm.GetDis(), static_cast<DWORD>(itm.GetType()), itm.GetTextType().c_str());
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

VOID CExpr::TestPtr(CONST std::vector<std::wstring>& VecParm)
{
	MyTools::InvokeClassPtr<CTestFunction>()->InitTestShareContent();

	StartGame;
	if (!MyTools::InvokeClassPtr<CLogicBagItemAction>()->Check())
		return;

	MyTools::InvokeClassPtr<CPlayerMove>()->MoveToMapPoint(L"汴京东郊", Point(39, 21));
	/*MyTools::InvokeClassPtr<CGameUiExtend>()->Action_By_DlgName_When_ShowDlg(L"npcdlg", [](CONST CGameUi& npcdlg) 
	{
		DWORD dwHead = ReadDWORD(npcdlg.GetNodeBase() + 0x14);
		DWORD dwEnd = ReadDWORD(npcdlg.GetNodeBase() + 0x18);

		int nCount = static_cast<int>(dwEnd - dwHead) >> 2;
		for (int i = 0;i < nCount; ++i)
		{
			DWORD dwAddr = ReadDWORD(dwHead + i * 4);
			if (ReadDWORD(dwAddr + 0x24C) == 0)
				continue;

			CHAR* pszText = reinterpret_cast<CHAR *>(ReadDWORD(dwAddr + 0x24C));
			LOG_C_D(L"Text=%s", MyTools::CCharacter::ASCIIToUnicode(pszText).c_str());
		}
	});*/
	
	//MyTools::InvokeClassPtr<CFarmField>()->Run(L"汴京东郊", Point(39,21));
	/**/
}
