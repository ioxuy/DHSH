#include "stdafx.h"
#include "MapSearch.h"
#include <MyTools/CLPublic.h>
#include "ResText.h"

BOOL CMapSearch::Search(_In_ CONST std::wstring& wsStartMapName, _In_ CONST std::wstring& wsEndMapName, _Out_ std::vector<std::wstring>& VecPath)
{
	return Find(wsStartMapName, wsEndMapName, VecPath);
}

BOOL CMapSearch::Exist(_In_ CONST std::wstring& wsMapName)
{
	auto pVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<Edge>*>(L"EdgeText");
	if (pVec == nullptr)
		return FALSE;

	return MyTools::CLPublic::Vec_find_if_Const(*pVec, [wsMapName](CONST Edge& Edge_) { return Edge_.Contain(wsMapName); }) != nullptr;
}

BOOL CMapSearch::Find(_In_ CONST std::wstring& wsStartMapName, _In_ CONST std::wstring& wsEndMapName, _Out_ std::vector<std::wstring>& VecPath)
{
	// // Dijkstra
	auto pVec = MyTools::InvokeClassPtr<CResText>()->GetStructPtr<CONST std::vector<Edge>*>(L"EdgeText");
	if (pVec == nullptr)
		return FALSE;

	VecEdge.clear();
	VecEdge = *pVec;

	Edge* pCurrentEdge = FindEdge(wsStartMapName);
	if (pCurrentEdge == nullptr)
		return FALSE;

	pCurrentEdge->bVisited = true;
	FindContainVertex(pCurrentEdge);

	Edge** pEndEdge = nullptr;
	for (;;)
	{
		if (VecWaitSearchEdge.size() == 0)
			break;

		pEndEdge = MyTools::CLPublic::Vec_find_if(VecWaitSearchEdge, [wsEndMapName](CONST Edge* pEdge) { return pEdge->Contain(wsEndMapName); });
		if (pEndEdge != nullptr)
			break;

		std::sort(VecWaitSearchEdge.begin(), VecWaitSearchEdge.end(), [](CONST Edge* Edge1, CONST Edge* Edge2) { return Edge1->nWeight < Edge2->nWeight; });
		pCurrentEdge = VecWaitSearchEdge.at(0);
		VecWaitSearchEdge.erase(VecWaitSearchEdge.begin());

		pCurrentEdge->bVisited = true;
		FindContainVertex(pCurrentEdge);
	}

	if (pEndEdge == nullptr)
		return FALSE;

	Edge* pEdge = *pEndEdge;
	std::wstring wsMapName = wsEndMapName;
	while (pEdge != nullptr)
	{
		VecPath.push_back(wsMapName);
		wsMapName = wsMapName == pEdge->Vertex1.wsMapName ? pEdge->Vertex2.wsMapName : pEdge->Vertex1.wsMapName;
		pEdge = pEdge->Father;
	}

	return TRUE;
}

CMapSearch::Edge* CMapSearch::FindEdge(_In_ CONST std::wstring& wsMapName)
{
	return MyTools::CLPublic::Vec_find_if(VecEdge, [wsMapName](Edge& Edge_) { return Edge_.Contain(wsMapName); });
}

VOID CMapSearch::FindContainVertex(_In_ Edge* pEdge)
{
	for (auto& itm : VecEdge)
	{
		if (itm.bVisited)
			continue;

		if (itm.Contain(pEdge->Vertex1.wsMapName) || itm.Contain(pEdge->Vertex2.wsMapName))
		{
			itm.bVisited = true;
			itm.Father = pEdge;
			itm.nWeight += pEdge->nWeight;
			VecWaitSearchEdge.push_back(&itm);
		}
	}
}
