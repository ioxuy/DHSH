#include "stdafx.h"
#include "Npc.h"
#include <MyTools/Log.h>
#include <MyTools/TimeTick.h>
#include <MyTools/Character.h>
#include "ExcuteAction.h"
#include "GameCALL.h"
#include "GameUiExtend.h"

#define _SELF L"Npc.cpp"
CNpc::CNpc() : CPlayer(NULL)
{

}

CNpc::CNpc(_In_ DWORD dwNodeBase) : CPlayer(dwNodeBase)
{

}

BOOL CNpc::OpenNpcDlg() CONST
{
	if (GetDis() > 15)
	{
		LOG_CF_E(L"与Npc[%s] 距离[%.2f]过远, 打开对话失败!", GetName().c_str(), GetDis());
		return FALSE;
	}

	MyTools::CTimeTick TimeTick;
	while (GameRun && !MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowNpcDlg())
	{
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) > 10)
		{
			LOG_CF_E(L"与Npc[%s] 距离[%.2f], 打开对话失败! 超时10秒", GetName().c_str(), GetDis());
			return FALSE;
		}

		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([this]
		{
			MyTools::InvokeClassPtr<CGameCALL>()->OpenNpc(GetId());
		});
		::Sleep(1000);
	}
	
	return TRUE;
}

BOOL CNpc::ClickOption(_In_ CONST std::wstring& wsOptionText, _In_ CONST std::wstring& wsNewDlg) CONST
{
	return ClickOption_By_Condition(wsOptionText, [wsNewDlg] { return MyTools::InvokeClassPtr<CGameUiExtend>()->IsShowDlg(wsNewDlg); });
}

BOOL CNpc::ClickOption_By_Condition(_In_ CONST std::wstring& wsOptionText, _In_ std::function<BOOL(VOID)> fnExitPtr) CONST
{
	MyTools::CTimeTick TimeTick;
	while (GameRun && !fnExitPtr())
	{
		if (TimeTick.GetSpentTime(MyTools::CTimeTick::em_TimeTick::em_TimeTick_Second) > 10)
		{
			LOG_CF_E(L"与Npc[%s] 距离[%.2f], 点击选项:[%s]! 超时10秒", GetName().c_str(), GetDis(), wsOptionText.c_str());
			return FALSE;
		}

		MyTools::InvokeClassPtr<CExcuteAction>()->PushPtrToMainThread([this, wsOptionText]
		{
			MyTools::InvokeClassPtr<CGameCALL>()->ClickNpcOption(GetId(), MyTools::CCharacter::UnicodeToASCII(wsOptionText).c_str());
		});
		::Sleep(1000);
	}
	return fnExitPtr();
}

