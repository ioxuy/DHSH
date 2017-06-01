#include "stdafx.h"
#include "Task.h"
#include <MyTools/Character.h>
#include <MyTools/CLPublic.h>

CTaskObject::CTaskObject(_In_ DWORD dwNodeBase)
{
	_dwNodeBase = dwNodeBase;
}

VOID CTaskObject::SetName()
{
	_wsName = MyTools::CCharacter::ASCIIToUnicode(reinterpret_cast<CHAR*>(ReadDWORD(GetNodeBase() + C_task_name)));
}

std::wstring CTaskObject::GetTaskContent() CONST
{
	std::wstring wsTaskContent = MyTools::CCharacter::ASCIIToUnicode(reinterpret_cast<CHAR*>(ReadDWORD(ReadDWORD(GetNodeBase() + C_task_tishi) + 0x210)));
	return std::move(wsTaskContent);
}

CTaskObject::em_Task_Gang_Type CTaskObject::GetBangTaskType() CONST
{
	struct Task_Bang_Type_Text
	{
		std::wstring wsTaskName;
		em_Task_Gang_Type emType;
	};

	CONST static std::vector<Task_Bang_Type_Text> Vec = 
	{
		{ L"#C17[帮派]完璧归赵", em_Task_Gang_Type::em_Task_Gang_Type_KillMonster },
		{ L"#C17[帮派]打造精品", em_Task_Gang_Type::em_Task_Gang_Type_MakeEqui },
	};

	auto p = MyTools::CLPublic::Vec_find_if_Const(Vec, [this](CONST Task_Bang_Type_Text& TaskText) { return TaskText.wsTaskName == GetName(); });
	return p == nullptr ? em_Task_Gang_Type::em_Task_Gang_Type_None : p->emType;
}
