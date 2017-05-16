#ifndef __DHSH_GAMEDLL_GAME_ALGORITHM_PATH_MAPSEARCH_H__
#define __DHSH_GAMEDLL_GAME_ALGORITHM_PATH_MAPSEARCH_H__

#include "GameBase.h"

class CMapSearch : public MyTools::CRelfexBaseClass
{
public:
	CMapSearch() = default;
	~CMapSearch() = default;
	
	BOOL Search(_In_ CONST std::wstring& wsStartMapName, _In_ CONST std::wstring& wsEndMapName, _Out_ std::vector<std::wstring>& VecPath);

	BOOL Exist(_In_ CONST std::wstring& wsMapName);
public:
	struct Vertex
	{
		std::wstring wsMapName;
	};

	struct Edge
	{
		Vertex Vertex1;
		Vertex Vertex2;

		struct Edge* Father;
		int nWeight;
		bool bVisited;
		Edge(_In_ CONST std::wstring&& wsMapName1, _In_ CONST std::wstring&& wsMapName2)
		{
			nWeight = 1;
			bVisited = false;
			Father = nullptr;
			Vertex1.wsMapName = std::move(wsMapName1);
			Vertex2.wsMapName = std::move(wsMapName2);
		}

		BOOL Contain(_In_ CONST std::wstring& wsMapName) CONST
		{
			return Vertex1.wsMapName == wsMapName || Vertex2.wsMapName == wsMapName;
		}
	};
private:
	// 
	BOOL Find(_In_ CONST std::wstring& wsStartMapName, _In_ CONST std::wstring& wsEndMapName, _Out_ std::vector<std::wstring>& VecPath);
	
	//
	Edge* FindEdge(_In_ CONST std::wstring& wsMapName);

	//
	VOID FindContainVertex(_In_ Edge* pEdge);
	
private:
	std::vector<Edge> VecEdge;
	std::vector<Edge*> VecWaitSearchEdge;
public:
	static CMapSearch* CreateInstance()
	{
		return new CMapSearch;
	}
	
	virtual VOID ReleaseInstance(_In_ LPVOID lpObjectAddr) CONST
	{
		delete reinterpret_cast<CMapSearch*>(lpObjectAddr);
	}

	static CONST std::wstring& GetClassName_()
	{
		static CONST std::wstring wsClassName = L"CMapSearch";
		return wsClassName;
	}
	
	CMapSearch(CONST CMapSearch&) = delete;
	
	CMapSearch& operator=(CONST CMapSearch&) = delete;
};

#endif // !__DHSH_GAMEDLL_GAME_ALGORITHM_MAPSEARCH_H__
