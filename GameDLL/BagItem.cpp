#include "stdafx.h"
#include "BagItem.h"
#include <MyTools/Character.h>
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "Npc.h"

CBagItem::CBagItem()
{
	_dwNodeBase = NULL;
}

CBagItem::CBagItem(_In_ DWORD dwObjAddr)
{
	_dwNodeBase = dwObjAddr;
	SetName();
}

VOID CBagItem::SetName()
{
	_wsName = MyTools::CCharacter::ASCIIToUnicode(std::string(reinterpret_cast<CONST CHAR*>(ReadDWORD(GetNodeBase() + C_BOX_name1) + C_box_name)));
}

DWORD CBagItem::GetItemId() CONST
{
	return ReadDWORD(GetNodeBase() + C_Box_ID);
}

DWORD CBagItem::GetCount() CONST
{
	return ReadDWORD(GetNodeBase() + C_Box_num);
}

BOOL CBagItem::UseItem() CONST
{
	BOOL bRetCode = FALSE;
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([this, &bRetCode] 
	{
		bRetCode = MyTools::InvokeClassPtr<CGameCALL>()->UseItem(GetItemId());
	});
	return bRetCode;
}

BOOL CBagItem::Sell(_In_ CONST CNpc& Npc) CONST
{
	BOOL bRetCode = FALSE;
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([this, &bRetCode, Npc]
	{
		bRetCode = MyTools::InvokeClassPtr<CGameCALL>()->SellToNpc(GetItemId(), Npc.GetId(), GetCount());
	});
	return bRetCode;
}

VOID CBagItem::Drop() CONST
{
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([this] 
	{
		MyTools::InvokeClassPtr<CGameCALL>()->Destory(GetItemId(), GetCount());
	});
}

VOID CBagItem::Decompose() CONST
{
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([this]
	{
		MyTools::InvokeClassPtr<CGameCALL>()->DecompostionEqui(GetItemId());
	});
}

VOID CBagItem::Save() CONST
{
	MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([this]
	{
		MyTools::InvokeClassPtr<CGameCALL>()->Save(GetItemId(), GetCount());
	});
}

std::wstring CBagItem::GetItemQuality() CONST
{
	std::wstring wsItemName = MyTools::CCharacter::ASCIIToUnicode(reinterpret_cast<CHAR*>(ReadDWORD(GetNodeBase() + C_BOX_shuxing1)));
	return std::move(wsItemName);
}

