#include "stdafx.h"
#include "Task.h"
#include <MyTools/Character.h>

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
	return em_Task_Gang_Type::em_Task_Gang_Type_None;
}
