#include "EchoPacket.h"
#include <MyTools/CLPublic.h>
#include <MyTools/CLFile.h>
#include <MyTools/Log.h>
#include "AccountAction.h"
#include "GameClient.h"
#include "Account.h"
#include "../GameDLL/GameBase.h"
#include "DbManager.h"
#include "AccountConfigExtend.h"
#include "AccountExtend.h"

#define _SELF L"EchoPacket.cpp"

CEchoPacket::CEchoPacket() : _LockVersion(L"CEchoPacket._LockVersion")
{

}

BOOL CEchoPacket::AccountLogin(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	struct LoginPacket
	{
		std::wstring wsAccountName;
		std::wstring wsAccountPass;
		std::wstring wsGameName;
		float		 fVersion;
	};
	
	auto LoginContent_ = ExtractPacket<LoginPacket>([&pSocketBuffer]
	{
		LoginPacket LoginContent_;
		*pSocketBuffer >> LoginContent_.wsAccountName >> LoginContent_.wsAccountPass >> LoginContent_.wsGameName >> LoginContent_.fVersion;
		return LoginContent_;
	});

	pSocketBuffer->clear();
	if (!CAccountAction::GetInstance().Login(LoginContent_.wsAccountName, LoginContent_.wsAccountPass, pGameClient->GetAccount()))
	{
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_AccountLogin);
		*pSocketBuffer << FALSE;
		return TRUE;
	}
	else if (!CheckVersion(LoginContent_.wsGameName, LoginContent_.fVersion))
	{
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"版本号不对……请获取最新的版本!";
		return TRUE;
	}
	
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_AccountLogin);
	pGameClient->SetToken(static_cast<ULONGLONG>(MyTools::CCharacter::GetRand_For_DWORD()));
	pGameClient->GetAccount()->SetToken(pGameClient->GetToken());

	*pSocketBuffer << TRUE << pGameClient->GetToken();
	return TRUE;
}

BOOL CEchoPacket::GameLogin(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	struct GameLoginPacket
	{
		std::wstring wsAccountName;
		std::wstring wsAccountPass;
		DWORD		 dwToken;
	};

	auto LoginContent_ = ExtractPacket<GameLoginPacket>([&pSocketBuffer]
	{
		GameLoginPacket LoginContent_;
		*pSocketBuffer >> LoginContent_.wsAccountName >> LoginContent_.wsAccountPass << LoginContent_.dwToken;;
		return LoginContent_;
	});

	pSocketBuffer->clear();
	if (!CAccountAction::GetInstance().Login(LoginContent_.wsAccountName, LoginContent_.wsAccountPass, pGameClient->GetAccount()))
	{
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_GameLogin);
		*pSocketBuffer << FALSE;
		return TRUE;
	}
	else if (!CheckIsLogin(pGameClient, pSocketBuffer))
		return TRUE;
	else if (pGameClient->GetAccount()->GetTokenCount() >= 10)
	{
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"登录已达到最大的上限10个了!";
		return TRUE;
	}


	pGameClient->GetAccount()->SetTokenCount(CAccount::em_Token_Action::em_Token_Action_Add);
	return TRUE;
}

BOOL CEchoPacket::Register(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	struct RegisterPacket
	{
		std::wstring wsAccountName;
		std::wstring wsAccountPass;
	};

	auto RegisterContent = ExtractPacket<RegisterPacket>([&pSocketBuffer]
	{
		RegisterPacket LoginContent_;
		*pSocketBuffer >> LoginContent_.wsAccountName >> LoginContent_.wsAccountPass;
		return LoginContent_;
	});

	

	std::wstring wsText;
	if (!CDbManager::GetInstance().RegisterAccount(RegisterContent.wsAccountName, RegisterContent.wsAccountPass, pGameClient->GetIP(), wsText))
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"创建帐号失败……联系作者查询详细原因!";
		return TRUE;
	}
	else if (wsText.find(L"创建成功!") != -1)
	{
		DWORD dwAccountId = _wtoi(wsText.substr(wsText.find(L"=") + 1).c_str());
		CAccount Account(dwAccountId, RegisterContent.wsAccountName, RegisterContent.wsAccountPass, FALSE, 0);
		CAccountExtend::GetInstance().AddAccount(Account);

		wsText = L"创建成功!";
	}
	
	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_Register);
	*pSocketBuffer << wsText;
	return TRUE;
}

BOOL CEchoPacket::KeepALive(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	pGameClient->SetKeepALiveTick();
	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_KeepLive);
	*pSocketBuffer << pGameClient->GetAccount()->GetEffectiveHour();
	return TRUE;
}

BOOL CEchoPacket::QueryAccount(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	std::vector<std::wstring> Vec;
	CDbManager::GetInstance().GetAccountCardLog(pGameClient->GetAccount()->GetAccountId(), Vec);

	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_QueryCard);

	
	*pSocketBuffer << Vec.size();
	for (CONST auto& itm : Vec)
		*pSocketBuffer << itm;

	return TRUE;
}

BOOL CEchoPacket::RechargeCard(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	std::wstring wsCardNo;
	*pSocketBuffer >> wsCardNo;

	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_Recharge);

	std::wstring wsText;
	DWORD dwNewTime = 0;
	if (CDbManager::GetInstance().RechargeCard(pGameClient->GetAccount()->GetAccountId(), wsCardNo, wsText))
	{
		if (wsText == L"充值完毕!" && CDbManager::GetInstance().GetTime_By_AccountId(pGameClient->GetAccount()->GetAccountId(), dwNewTime))
		{
			pGameClient->GetAccount()->SetNewTime(dwNewTime);
		}
		*pSocketBuffer << wsText;
	}
	else
		*pSocketBuffer << L"充值失败……请咨询作者查询详细原因!";

	return TRUE;
}

BOOL CEchoPacket::ReadConfig(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	struct ReadConfigText
	{
		std::wstring wsPlayerName;
		std::wstring wsType;
		std::vector<std::wstring> VecText;
	};
	auto Vec = ExtractPacket<ReadConfigText>([&pSocketBuffer]
	{
		ReadConfigText ReadConfigText_;

		DWORD dwSize = 0;
		*pSocketBuffer >> ReadConfigText_.wsPlayerName >> ReadConfigText_.wsType >> dwSize;

		std::wstring wsConfigName;
		for (decltype(dwSize) i = 0; i < dwSize; ++i)
		{
			*pSocketBuffer >> wsConfigName;
			ReadConfigText_.VecText.push_back(wsConfigName);
		}

		return ReadConfigText_;
	});

	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ReadConfig);
	*pSocketBuffer << Vec.VecText.size();

	std::wstring wsFormatText;
	auto AccountId = pGameClient->GetAccount()->GetAccountId();
	for (CONST auto& itm : Vec.VecText)
	{
		std::wstring wsConfigValue = L"Empty";

		CAccountConfigExtend::GetInstance().FindConfig(AccountId, MyTools::CCharacter::FormatText(wsFormatText, L"%s-%s-%s", Vec.wsPlayerName.c_str(), Vec.wsType.c_str(), itm.c_str()), wsConfigValue);
		*pSocketBuffer << itm << wsConfigValue;
	}
	return TRUE;
}

BOOL CEchoPacket::WriteConfig(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	auto WriteConfigText_ = ExtractPacket<CAccountConfigExtend::WriteConfigText>([&pSocketBuffer]
	{
		CAccountConfigExtend::WriteConfigText WriteConfigText_;

		DWORD dwSize = 0;
		*pSocketBuffer >> WriteConfigText_.wsPlayerName >> WriteConfigText_.wsType >> dwSize;
		for (decltype(dwSize) i = 0;i < dwSize; ++i)
		{
			CAccountConfigExtend::AccountConfig ConfigPacket_;

			std::wstring wsConfigValue;
			*pSocketBuffer >> ConfigPacket_.wsConfigName >> wsConfigValue;

			MyTools::CCharacter::FormatText(ConfigPacket_.wsConfigValue, L"%s-%s-%s", WriteConfigText_.wsPlayerName.c_str(), WriteConfigText_.wsType.c_str(), wsConfigValue.c_str());
			WriteConfigText_.VecConfig.push_back(std::move(ConfigPacket_));
		}
	
		return WriteConfigText_;
	});

	pSocketBuffer->clear();
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_WriteConfig);

	auto AccountId = pGameClient->GetAccount()->GetAccountId();
	CAccountConfigExtend::GetInstance().WriteConfig(AccountId, WriteConfigText_);

	return TRUE;
}

BOOL CEchoPacket::QueryTime(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_QueryHour);
	*pSocketBuffer << pGameClient->GetAccount()->GetEffectiveHour();
	return TRUE;
}

BOOL CEchoPacket::CheckIsLogin(_In_ CGameClient* pGameClient, _In_ MyTools::CLSocketBuffer* pSocketBuffer) CONST
{
	if (pGameClient->GetAccount() == nullptr)
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"此帐号还没登录";
		return FALSE;
	}
	else if (pGameClient->GetToken() != pGameClient->GetAccount()->GetToken())
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"你已经被挤号下线了……";
		return FALSE;
	}
	else if (pGameClient->GetAccount()->IsForzen())
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"此帐号已经被冻结了!";
		return FALSE;
	}
	else if (pGameClient->GetAccount()->GetToken() != pGameClient->GetToken())
	{
		pSocketBuffer->clear();
		pSocketBuffer->InitializeHead(em_Sock_Msg::em_Sock_Msg_ServerText);
		*pSocketBuffer << L"你的帐号已经被重新登录了……";
		return FALSE;
	}

	return TRUE;
}

BOOL CEchoPacket::ReadLocalVersion()
{
	auto fnAddVersion = [this](CONST std::wstring& wsGameName, float fVersion)
	{
		_VecVersion.push_back(VersionContent{ wsGameName, fVersion });
	};

	std::wstring wsDHSHVersion;
	if (!MyTools::CLFile::ReadUnicodeConfig(L"\\Config.ini", L"Version", L"DHSH", wsDHSHVersion))
		return FALSE;

	_VecVersion.clear();
	fnAddVersion(L"DHSH",static_cast<float>(_wtof(wsDHSHVersion.c_str())));

	return TRUE;
}


BOOL CEchoPacket::CheckVersion(_In_ CONST std::wstring& wsGameName, _In_ float fVersion) CONST
{
	BOOL bExist = FALSE;
	_LockVersion.Access([=, &bExist]
	{
		bExist = MyTools::CLPublic::Vec_find_if_Const(_VecVersion, [=](CONST VersionContent& VC) { return VC.wsGameName == wsGameName && fVersion == VC.fVersion; }) != nullptr;
	});

	return bExist;
}
