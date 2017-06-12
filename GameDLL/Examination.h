#ifndef __DHSH_GAMEDLL_GAME_SCRIPT_EXAM_EXAMINATION_H__
#define __DHSH_GAMEDLL_GAME_SCRIPT_EXAM_EXAMINATION_H__

#include "GameBase.h"

class CTaskObject;
class CExamination : public MyTools::CRelfexBaseClass
{
private:
	struct MapLocation
	{
		std::wstring wsMapName;
		std::wstring wsNpcName;
		Point TarPoint;
	};

public:
	CExamination() = default;
	~CExamination() = default;
	
	BOOL Check() CONST;
	
	BOOL Run();
private:
	//
	BOOL Signup();

	//
	BOOL ExistTask(_Out_ CTaskObject* pTaskObject) CONST;

	//
	BOOL GetTaskMapLocation(_In_ CONST CTaskObject* pTaskObject, _Out_ MapLocation& MapLocation_) CONST;

	//
	BOOL AnswerTaskQuestion(_In_ CONST MapLocation& MapLocation_) CONST;
public:
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CExamination*>(lpObjectAddr);
	}
	
	CExamination(CONST CExamination&) = delete;
	
	CExamination& operator=(CONST CExamination&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_SCRIPT_EXAM_EXAMINATION_H__
