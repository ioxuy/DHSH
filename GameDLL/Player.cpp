#include "stdafx.h"
#include "Player.h"
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>
#include "PersonAttribute.h"

CPlayer::CPlayer(_In_ DWORD dwNodeBase)
{ 
	_dwNodeBase = dwNodeBase;
}

CPlayer::CPlayer()
{
	_dwNodeBase = NULL;
}

VOID CPlayer::SetName()
{
	_wsName = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CHAR*>(GetNodeBase() + C_huan_name)));
}

DWORD CPlayer::GetId() CONST
{
	return ReadDWORD(GetNodeBase() + C_huan_ID);
}

Point CPlayer::GetPoint() CONST
{
	Point CurPoint = Point(ReadDWORD(GetNodeBase() + C_huan_X) >> 4, ReadDWORD(GetNodeBase() + C_huan_Y) >> 4);
	return std::move(CurPoint);
}

CPlayer::em_PlayerType CPlayer::GetType() CONST
{
	return static_cast<em_PlayerType>(ReadDWORD(GetNodeBase() + C_huan_tiaojian) & 0xFF);
}

CONST std::wstring& CPlayer::GetTextType() CONST
{
	struct PlayerTypeText
	{
		std::wstring wsText;
		em_PlayerType emType;
	};

	CONST static std::vector<PlayerTypeText> Vec = 
	{
		{ L"怪物", em_PlayerType::em_PlayerType_Monster },
		{ L"Npc", em_PlayerType::em_PlayerType_Npc },
		{ L"宠物", em_PlayerType::em_PlayerType_Pet },
		{ L"玩家", em_PlayerType::em_PlayerType_Player },
		{ L"自己", em_PlayerType::em_PlayerType_Self },
	};
	CONST static std::wstring wsUnKnowText = L"未知";

	auto p = MyTools::CLPublic::Vec_find_if_Const(Vec, [this](CONST auto& itm) { return itm.emType == GetType(); });
	return p != nullptr ? p->wsText : wsUnKnowText;
}

float CPlayer::GetDis() CONST
{
	return MyTools::CLPublic::GetDisBy2D(MyTools::InvokeClassPtr<CPersonAttribute>()->GetPoint(), GetPoint());
}

DWORD CPlayer::GetResId() CONST
{
	return ReadDWORD(GetNodeBase() + C_huan_ID + 0x4) & 0xFFFF;
}

