#include "stdafx.h"
#include "Expr.h"
#include <MyTools/Log.h>
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
		{ std::bind(&CExpr::TestPtr,this, std::placeholders::_1),L"TestPtr" },
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

	int nCount = 0;
	for (CONST auto& itm : Vec)
	{
		LOG_C_D(L"Index=%d,Addr=%X, Name=%s,Point=[%d,%d],dis=%.2f,Type=[%X,%s]", ++nCount, itm.GetNodeBase(), itm.GetName().c_str(), itm.GetPoint().X, itm.GetPoint().Y, itm.GetDis(),static_cast<DWORD>(itm.GetType()),  itm.GetTextType().c_str());
	}
}

VOID CExpr::TestPtr(CONST std::vector<std::wstring>& VecParm)
{
	std::vector<CHotKey> Vec;
	MyTools::InvokeClassPtr<CHotKeyExtend>()->GetVecHotKey(Vec);
	for (CONST auto& itm : Vec)
	{
		LOG_C_D(L"HotKey Index=%d, KeyType=%d, KeyValue=%d, Name=%s, Office=%d", itm.GetIndex(), itm.GetKeyType(), itm.GetKeyValue(), itm.GetName().c_str(), itm.GetOffice());
	}

	
}
