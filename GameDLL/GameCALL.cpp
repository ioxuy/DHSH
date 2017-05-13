#include "stdafx.h"
#include "GameCALL.h"
#include <MyTools/Log.h>
#include <MyTools/Character.h>

#define _SELF L"GameCALL.cpp"
BOOL CGameCALL::Give(_In_ DWORD dwUiBase, _In_ DWORD dwIndex) CONST
{
	__try
	{
		__asm
		{
			MOV ESI, dwUiBase;
			MOV ECX, DWORD PTR DS : [ESI + 0x498];

			PUSH dwIndex;
			MOV EDX, C_CALL_Give_Set;
			CALL EDX;

			MOV EBX, EAX;
			MOV ECX, DWORD PTR DS : [ESI + 0x4A0];
			MOV EDX, C_CALL_Give_Set1;
			CALL EDX;

			MOV ESI, EAX;
			PUSH EBX;
			PUSH ESI;
			MOV EDX, C_CALL_Give_Set2;
			CALL EDX;

			MOV EDX, DWORD PTR DS : [ESI + 0x1C];
			MOV DWORD PTR DS : [ESI + 0x78], EDX;
			ADD ESP, 0x8;

			PUSH 0;
			MOV ECX, dwUiBase;
			MOV EDX, C_CALL_give_Enter;
			CALL EDX;

			PUSH 1;
			MOV ECX, dwUiBase;
			MOV EDX, C_CALL_give_Enter;
			CALL EDX;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"Give Exception. Index=%X", dwIndex);
	}
	return FALSE;
}

BOOL CGameCALL::WhacAMole(_In_ DWORD dwIndex, _In_ DWORD dwCmd, _In_ DWORD dwNonw) CONST
{
	__try
	{
		DWORD dwBuffer[20] = { 0 };
		__asm
		{
			PUSH dwIndex;
			PUSH dwNonw;
			PUSH dwCmd;
			PUSH 1;

			LEA EAX, dwBuffer;
			PUSH EAX;
			PUSH 0x4606;

			MOV EDX, C_CALL_Start_打地鼠;
			CALL EDX;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"WhacAMole Exception. Index=%X, dwCmd=%X, dwNonw=%X", dwIndex, dwCmd, dwNonw);
	}
	return FALSE;
}

BOOL CGameCALL::OpenGiveGameUi(_In_ DWORD dwUiBase) CONST
{
	__try
	{
		__asm
		{
			PUSH 0;
			PUSH 0;
			PUSH 0;
			PUSH dwUiBase;

			MOV EDX, C_CALL_Open_give;
			CALL EDX;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"OpenGiveGameUi Exception. dwUiBase=%X", dwUiBase);
	}
	return FALSE;
}

BOOL CGameCALL::Send(_In_ CONST CHAR* Buffer) CONST
{
	__try
	{
		__asm
		{
			MOV ECX, C_CALL_fabao_base;
			MOV ECX, DWORD PTR DS : [ECX];
			 
			PUSH Buffer;
			PUSH ECX;

			MOV EDX, C_CALL_fabao;
			CALL EDX;
			ADD ESP, 0x8;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"Send Exception. Buffer=%X", Buffer);
	}
	return FALSE;
}

BOOL CGameCALL::Mail(_In_ CONST std::wstring& wsBoxName, _In_ CONST std::wstring& wsPlayerName, _In_ DWORD dwId, _In_ DWORD dwPlayerId) CONST
{
	struct MyStruct
	{
		char boxName[534] = { 0 };
		BYTE RoleName[20] = { 0 };
		DWORD RoleID = NULL;
		BYTE one = 0x1;
		DWORD dwBoxID = NULL;
		char Know[9] = { 0 };
		char CapText[534] = { 0 };
		BYTE RoleName1[20] = { 0 };
		DWORD RoleID1 = NULL;
		BYTE one1 = 1;
		DWORD dwBoxID1 = NULL;
		char Know1[9] = { 0 };
		DWORD one2 = 1;
		DWORD one3 = 1;
		DWORD zero1 = NULL;
		DWORD one4 = NULL;
		char know2[64] = { 0 };
		DWORD Mon = 0x3E8;//0x3E8邮费
		char know[100] = { 0 };
	};

	MyStruct MailContent;

	MyTools::CCharacter::strcpy_my(MailContent.boxName, MyTools::CCharacter::UnicodeToASCII(wsBoxName).c_str(), _countof(MailContent.boxName) - 1);
	MyTools::CCharacter::strcpy_my(MailContent.CapText, MyTools::CCharacter::UnicodeToASCII(wsBoxName).c_str(), _countof(MailContent.boxName) - 1);
	MyTools::CCharacter::strcpy_my(reinterpret_cast<CHAR*>(MailContent.RoleName), MyTools::CCharacter::UnicodeToASCII(wsPlayerName).c_str(), _countof(MailContent.RoleName) - 1);
	MyTools::CCharacter::strcpy_my(reinterpret_cast<CHAR*>(MailContent.RoleName1), MyTools::CCharacter::UnicodeToASCII(wsPlayerName).c_str(), _countof(MailContent.RoleName) - 1);

	MailContent.RoleID = MailContent.RoleID1 = dwPlayerId;
	MailContent.dwBoxID = MailContent.dwBoxID1 = dwId;

	auto fnMailMethodPtr = [wsBoxName, wsPlayerName, dwId, dwPlayerId](CONST MyStruct& MailContent)
	{
		__try
		{
			__asm
			{
				PUSH 0;
				PUSH 0;
				PUSH dwPlayerId;
				PUSH 0x4229;

				MOV EDX, C_CALL_ADDHP;
				CALL EDX;
				ADD ESP, 0x10;

				PUSH 0;
				PUSH 0;
				PUSH 0;
				PUSH 0x23C;
				LEA EAX, MailContent;
				PUSH EAX;
				PUSH 0x4223;

				MOV EDX, C_CALL_Start_打地鼠;
				CALL EDX;
				ADD ESP, 0x18;
			}
			return TRUE;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LOG_CF_E(L"Mail Exception. wsBoxName=%s,wsPlayerName=%s, dwId=%X, dwPlayerId=%X", wsBoxName.c_str(), wsPlayerName.c_str(), dwId, dwPlayerId);
		}
		return FALSE;
	};

	
	return fnMailMethodPtr(MailContent);
}

BOOL CGameCALL::CloseGameUi(_In_ DWORD dwUiBase) CONST
{
	__try
	{
		__asm
		{
			PUSH 1;
			PUSH 0;
			MOV ECX, dwUiBase;
			MOV ECX, DWORD PTR DS : [ECX];
			MOV EAX, ECX;
			MOV EDX, DWORD PTR DS : [EAX + 0x28];
			CALL EDX;

			MOV ECX, dwUiBase;
			MOV EAX, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [EAX + 0x74];
			CALL EDX;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"CloseGameUi Exception. dwUiBase=%X", dwUiBase);
	}
	return FALSE;
}

BOOL CGameCALL::Give_Send(_In_ DWORD dwNpcId, _In_ DWORD dwId) CONST
{
#pragma pack(1)
	struct _jiegou
	{
		WORD Cmd = 0x2E;
		WORD CMd1 = 0x4064;
		DWORD npc = Num;
		DWORD dwIndex[3] = { 0 };
		DWORD dw17 = 0x0018C778;
		DWORD dwID = Num;
		DWORD ling[2] = { 0 };
		DWORD Num = 0x1;
		DWORD unknow[3] = { 0 };
		DWORD dw18 = 0x0018C808;
	};
#pragma pack()

	_jiegou Buffer;
	Buffer.npc = dwNpcId;
	Buffer.dwID = dwId;

	return Send(reinterpret_cast<CONST CHAR*>(&Buffer));
}

BOOL CGameCALL::CheckPointIsWork(_In_ CONST Point& Point_) CONST
{
	__try
	{
		DWORD dwX = Point_.X;
		DWORD dwY = Point_.Y;
		__asm
		{
			PUSH dwX;
			PUSH dwY;

			MOV ECX, C_map_base;
			MOV ECX, DWORD PTR DS : [ECX];
			MOV ECX, DWORD PTR DS : [ECX + C_map_isRun];
			MOV EDX, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [EDX + 0xC];
			CALL EDX;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"CheckPointIsWork Exception. Point_=[%d,%d]", Point_.X, Point_.Y);
	}
	return FALSE;
}

BOOL CGameCALL::RunAway() CONST
{
	__try
	{
		__asm
		{
			MOV ECX, C_base_dianguai;
			MOV ECX, DWORD PTR DS : [ECX];
			PUSH 0;
			PUSH 0xA;
			MOV EDX, C_CALL_taopao;
			CALL EDX;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"RunAway Exception",);
	}
	return FALSE;
}

BOOL CGameCALL::Defence() CONST
{
	__try
	{
		__asm
		{
			MOV ECX, C_base_dianguai;
			MOV ECX, DWORD PTR DS : [ECX];
			PUSH 0;
			PUSH 0x3;
			MOV EDX, C_CALL_taopao;
			CALL EDX;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"Defence Exception", );
	}
	return FALSE;
}

BOOL CGameCALL::ClickMonster(_In_ DWORD dwObjAddr) CONST
{
	__try
	{
		DWORD Buffer = { 0 };
		__asm
		{
			PUSH dwObjAddr;
			MOV ECX, C_map_base;
			MOV ECX, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [ECX + 0x90C];
			PUSH EDX;
			MOV EDX, DWORD PTR DS : [ECX + 0x910];
			PUSH EDX;
			LEA EAX, Buffer;
			PUSH EAX;

			MOV EDX, C_CALL_dianguai;
			MOV ECX, C_base_dianguai;
			MOV ECX, DWORD PTR DS : [ECX];
			CALL EDX;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"ClickMonster Exception");
	}
	return FALSE;
}

BOOL CGameCALL::SellToNpc(_In_ DWORD dwItemId, _In_ DWORD dwNpcId, _In_ DWORD dwCount) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;

			PUSH dwCount;
			PUSH dwNpcId;
			PUSH dwItemId;
			PUSH 0x4072;

			MOV EAX, C_CALL_maichu;
			CALL EAX;
			ADD ESP, 0x10;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"SellToNpc Exception, dwItemId=%X, dwNpcId=%X, dwCount=%d", dwItemId, dwNpcId, dwCount);
	}
	return FALSE;
}

BOOL CGameCALL::CollectScraps(_In_ DWORD dwId) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;

			PUSH 0;
			PUSH 0;
			PUSH dwId;
			PUSH 0x4062;

			MOV EAX, C_CALL_maichu;
			CALL EAX;
			ADD ESP, 0x10;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"CollectScraps Exception, dwId=%X", dwId);
	}
	return FALSE;
}

BOOL CGameCALL::AbandonTask(_In_ DWORD dwTaskId) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;

			PUSH 0;
			PUSH 0;
			PUSH dwTaskId;
			PUSH 0x4214;

			MOV EAX, C_CALL_maichu;
			CALL EAX;
			ADD ESP, 0x10;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"AbandonTask Exception, dwTaskId=%X", dwTaskId);
	}
	return FALSE;
}

BOOL CGameCALL::SetUseSkill(_In_ DWORD dwSkillId) CONST
{
	__try
	{
		__asm
		{
			MOV ECX, C_Set_Skill_base;
			MOV ECX, DWORD PTR DS : [ECX];

			PUSH dwSkillId;

			MOV EDX, C_CALL_Set_Skill;
			CALL EDX;

		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"SetUseSkill Exception, dwSkillId=%X", dwSkillId);
	}
	return FALSE;
}

BOOL CGameCALL::CloseTeam(_In_ DWORD dwBase) CONST
{
	__try
	{
		__asm
		{
			PUSH 2;
			MOV ECX, dwBase;
			MOV ECX, DWORD PTR DS : [ECX];

			CALL DWORD PTR DS : [EAX + 0x184];
			MOV EAX, 0x1;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"CloseTeam Exception, dwBase=%X", dwBase);
	}
	return FALSE;
}

BOOL CGameCALL::SetAutoUseSkill() CONST
{
	__try
	{
		__asm
		{
			PUSHAD;

			MOV EDX, C_Auto_Skill_;
			CALL EDX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"SetAutoUseSkill Exception");
	}
	return FALSE;
}

BOOL CGameCALL::CatchMonster() CONST
{
	__try
	{
		DWORD Buffer[10] = { 0 };
		Buffer[0] = 0x1;

		__asm
		{
			PUSHAD;

			LEA EAX, Buffer;
			PUSH EAX;
			PUSH 0x1000044;
			PUSH 0x00000047;

			MOV ECX, C_base_kuaijie;
			MOV ECX, DWORD PTR DS : [ECX];
			MOV EAX, C_CALL_kuaijie;
			CALL EAX;

			LEA EAX, Buffer;
			PUSH EAX;

			PUSH 0x1000000;
			PUSH 0x0000012;

			MOV ECX, C_base_kuaijie;
			MOV ECX, DWORD PTR DS : [ECX];
			MOV EAX, C_CALL_kuaijie;
			CALL EAX;

		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"CatchMonster Exception");
	}
	return FALSE;
}

BOOL CGameCALL::SetHotKey(_In_ DWORD dwKeyValue) CONST
{
	__try
	{
		DWORD Buffer[10] = { 0 };
		Buffer[0] = 0x1;
		__asm
		{
			PUSHAD;
			LEA EAX, Buffer;
			PUSH EAX;

			PUSH 0x01000000;
			PUSH dwKeyValue;

			MOV ECX, C_base_kuaijie;
			MOV ECX, DWORD PTR DS : [ECX];

			MOV EAX, C_CALL_kuaijie;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"SetHotKey Exception, dwKeyValue=%X", dwKeyValue);
	}
	return FALSE;
}

BOOL CGameCALL::OpenNpc(_In_ DWORD dwNpcId) CONST
{
	__try
	{
		DWORD Buffer[20] = { 0 };
		__asm
		{
			PUSHAD;
			PUSH dwNpcId;
			PUSH 0;
			LEA EAX, Buffer;
			PUSH EAX;

			MOV EAX, C_CALL_fabao_base;
			MOV EAX, DWORD PTR DS : [EAX];
			PUSH EAX;
			MOV EAX, C_call_dianji;
			CALL EAX;
			ADD ESP, 0x10;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"OpenNpc Exception, dwNpcId=%X", dwNpcId);
	}
	return FALSE;
}

BOOL CGameCALL::FreshVerCode() CONST
{
	__try
	{
		__asm
		{
			PUSH 0;
			PUSH 0;
			PUSH 0;
			PUSH 0x100B;
			MOV EDX, C_CALL_ADDHP;
			CALL EDX;
			ADD ESP, 0x10;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"FreshVerCode Exception");
	}
	return FALSE;
}

BOOL CGameCALL::Save(_In_ DWORD dwItemId, _In_ DWORD dwCount) CONST
{
	__try
	{
		__asm 
		{
			PUSHAD;
			PUSH dwCount;
			PUSH 0x1F;
			PUSH dwItemId;
			PUSH 0x4068;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"Save Exception, dwItemId=%X, dwCount=%d", dwItemId, dwCount);
	}
	return FALSE;
}

BOOL CGameCALL::AddHp(_In_ DWORD dwLocation) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 0;
			PUSH dwLocation;
			PUSH 0x4075;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"AddHp Exception, dwLocation=%d", dwLocation);
	}
	return FALSE;
}

BOOL CGameCALL::DecompostionEqui(_In_ DWORD dwId) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			PUSH 1;
			PUSH dwId;
			PUSH 0x7;
			PUSH 0x4636;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"DecompostionEqui Exception, dwIndex=%d", dwId);
	}
	return FALSE;
}

BOOL CGameCALL::EnterWar(_In_ DWORD dwPetId) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 3;
			PUSH dwPetId;
			PUSH 0x4090;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"EnterWar Exception, dwPetId=%d", dwPetId);
	}
	return FALSE;
}

BOOL CGameCALL::Destory(_In_ DWORD dwItemId, _In_ DWORD dwCount) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH dwCount;
			PUSH dwItemId;
			PUSH 0x4063;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"Destory Exception, dwItemId=%d,dwCount=%d", dwItemId, dwCount);
	}
	return FALSE;
}

BOOL CGameCALL::CallGod() CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			
			PUSH 0;
			PUSH 0;
			PUSH 0xE;
			PUSH 0x462D;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"CallGod Exception");
	}
	return FALSE;
}

BOOL CGameCALL::SynthesisGem(_In_ DWORD dwResId1, _In_ DWORD dwResId2) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;

			PUSH 1;
			PUSH dwResId1;
			PUSH dwResId2;
			PUSH 0x406C;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;
			
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"SynthesisGem Exception,dwResId=[%X,%X]", dwResId1, dwResId2);
	}
	return FALSE;
}

BOOL CGameCALL::ExtractItem(_In_ DWORD dwItemId) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;

			PUSH 0x1;
			PUSH 0x1E;
			PUSH dwItemId;
			PUSH 0x4068;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"ExtractItem Exception,dwItemId=[%X]", dwItemId);
	}
	return FALSE;
}

BOOL CGameCALL::UseItem(_In_ DWORD dwItemId) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;

			PUSH 0x0;
			PUSH 0x0;
			PUSH dwItemId;
			PUSH 0x4060;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"UseItem Exception,dwItemId=[%X]", dwItemId);
	}
	return FALSE;
}

BOOL CGameCALL::UseMagicWeapon(_In_ DWORD dwItemId) CONST
{
	return UseItem(dwItemId);
}

BOOL CGameCALL::Mora(_In_ DWORD dwIndex) CONST
{
	__try
	{
		__asm
		{
			MOV ECX, C_base_dianguai;
			MOV ECX, DWORD PTR DS : [ECX];
			PUSH 0;
			PUSH dwIndex;
			MOV EDX, C_CALL_猜拳;
			CALL EDX;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"Mora Exception,dwIndex=[%X]", dwIndex);
	}
	return FALSE;
}

BOOL CGameCALL::SetAutoAction() CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			MOV ECX, C_base_dianguai;
			MOV ECX, DWORD PTR DS : [ECX];
			PUSH 0x19;
			MOV EDX, C_CALL_zidong;
			CALL EDX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"SetAutoAction Exception");
	}
	return FALSE;
}

BOOL CGameCALL::ShopMagicWeapon(_In_ DWORD dwNpcId, _In_ DWORD dwLocation, _In_ DWORD dwCount) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			PUSH dwCount;
			PUSH dwNpcId;
			PUSH dwLocation;
			PUSH 0x4070;
			MOV EAX, C_CALL_ADDHP;
			CALL EAX;
			ADD ESP, 0x10;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"ShopMagicWeapon Exception, dwNpcId=%X, dwLocation=%d, dwCount=%d", dwNpcId, dwLocation, dwCount);
	}
	return FALSE;
}

BOOL CGameCALL::AnswerQuestion(_In_ DWORD dwAnswer, _In_ DWORD dwBase) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH 1;
			MOV ECX, dwBase;
			MOV EDX, C_CALL_dati;
			CALL EDX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"AnswerQuestion Exception, dwAnswer=%X, dwBase=%X", dwAnswer, dwBase);
	}
	return FALSE;
}

BOOL CGameCALL::CloseNpcDlg() CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			
			MOV ESI, C_NPC_state;
			MOV ESI, DWORD PTR DS : [ESI];
			MOV ECX, ESI;
			MOV EDX, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [EDX + 0x74];
			CALL EDX;

			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"CloseNpcDlg Exception");
	}
	return FALSE;
}

BOOL CGameCALL::MoveToNpc(_In_ LPCSTR pszMapName, _In_ LPCSTR pszNpcName) CONST
{
	__try
	{
		__asm
		{
			PUSH 1;
			PUSH 0;
			PUSH pszNpcName;
			PUSH pszMapName;
			MOV EDX, C_CALL_NPC_寻路CALL;
			CALL EDX;

			MOV ECX, EAX;
			MOV EDX, C_CALL_NPC_寻路CALL1;
			CALL EDX;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"MoveToNpc Exception");
	}
	return FALSE;
}

BOOL CGameCALL::MoveToPoint(_In_ CONST Point& Point_) CONST
{
	__try
	{
		DWORD dwX = Point_.X;
		DWORD dwY = Point_.Y;
		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH dwY;
			PUSH dwX;
			PUSH 0;

			MOV EAX, C_CALL_xunlu1;
			CALL EAX;

			MOV ECX, EAX;
			MOV EBX, C_CALL_xunlu;
			CALL EBX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"MoveToPoint Exception, Point_=[%d,%d]", Point_.X, Point_.Y);
	}
	return FALSE;
}

BOOL CGameCALL::MoveToPoint(_In_ LPCSTR pszMapName, _In_ CONST Point& Point_) CONST
{
	__try
	{
		DWORD dwX = Point_.X;
		DWORD dwY = Point_.Y;
		__asm
		{
			PUSHAD;
			PUSH 0;
			PUSH dwY;
			PUSH dwX;
			MOV EAX, pszMapName;
			PUSH EAX;
			MOV EAX, C_CALL_xunlu1;
			CALL EAX;
			MOV ECX, EAX;
			MOV EBX, C_CALL_xunlu;
			CALL EBX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"MoveToPoint Exception, Point_=[%d,%d]", Point_.X, Point_.Y);
	}
	return FALSE;
}

BOOL CGameCALL::CollectItem(_In_ DWORD dwObjAddr) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			MOV EAX, C_caiji_CALL_chushi;
			CALL EAX;
			PUSH dwObjAddr;
			MOV ECX, EAX;
			MOV EAX, C_caiji_CALL;
			CALL EAX;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"CollectItem Exception, dwObjAddr=[%X]", dwObjAddr);
	}
	return FALSE;
}

BOOL CGameCALL::ClickNpcOption(_In_ DWORD dwNpcId, _In_ LPCSTR pszOptionText) CONST
{
	__try
	{
		__asm
		{
			PUSHAD;
			MOV EAX, C_CALL_fabao_base;
			MOV EAX, DWORD PTR DS: [EAX];
			MOV ECX, dwNpcId;
			PUSH ECX;
			PUSH 0;
			MOV EDX, pszOptionText;
			PUSH EDX;
			PUSH EAX;
			MOV EAX, C_call_dianji;
			CALL EAX;
			ADD ESP, 0x10;
			POPAD;
		}
		return TRUE;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"ClickNpcOption Exception, dwNpcId=[%X]", dwNpcId);
	}
	return FALSE;
}

CONST CHAR* CGameCALL::GetPetName_By_SkillId(_In_ DWORD dwSkillId) CONST
{
	__try
	{
		DWORD dwValue = 0;
		__asm
		{
			PUSHAD;
			MOV ECX, C_Pet_Skill_base;
			MOV ECX, DWORD PTR DS : [ECX];
			PUSH dwSkillId;
			MOV EDX, C_Get_Skill_name_CALL;
			CALL EDX;
			MOV dwValue, EAX;
			POPAD;
		}
		return dwValue == NULL ? nullptr : reinterpret_cast<CONST CHAR*>(dwValue + 0x2);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		LOG_CF_E(L"GetPetName_By_SkillId Exception, dwSkillId=[%X]", dwSkillId);
	}
	return nullptr;
}

