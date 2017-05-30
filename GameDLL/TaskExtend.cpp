#include "stdafx.h"
#include "TaskExtend.h"
#include <MyTools/Character.h>
#include <MyTools/Log.h>
#include <MyTools/CLPublic.h>
#include "Task.h"

#define _SELF L"TaskExtend.cpp"
UINT CTaskExtend::GetTask(_Out_ std::vector<CTaskObject>& Vec, _In_ std::function<BOOL(CONST CTaskObject&)> FilterPtr) CONST
{
	DWORD dwRoot = ReadDWORD(ReadDWORD(C_task_base) + C_task_yiji);
	DWORD dwHead = ReadDWORD(dwRoot + C_task_erji);
	DWORD dwCount = ReadDWORD(dwRoot + C_task_shu);

	LOG_C_D(L"dwRoot=%X,dwHead=%X, dwCount=%d", dwRoot, dwHead, dwCount);
	for (decltype(dwCount) i = 0;i < dwCount; ++i)
	{
		CTaskObject TaskObject(ReadDWORD(dwHead + i * 4));
		TaskObject.SetName();
		if (FilterPtr == nullptr)
		{
			Vec.push_back(std::move(TaskObject));
			continue;
		}
		else if (FilterPtr(TaskObject))
		{
			Vec.push_back(std::move(TaskObject));
			break;
		}
	}

	return Vec.size();
}

BOOL CTaskExtend::ExistTask_By_PartName_Action(_In_ CONST std::wstring wsPartName, _In_ std::function<VOID(CONST CTaskObject&)> ActionPtr) CONST
{
	std::vector<CTaskObject> Vec;
	GetTask(Vec, [wsPartName](CONST CTaskObject& TaskObject) { return TaskObject.GetName().find(wsPartName) != -1; });

	if (Vec.empty())
		return FALSE;

	if(ActionPtr != nullptr)
		ActionPtr(Vec.at(0));
	return TRUE;
}
