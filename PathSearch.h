#ifndef __SHORTPATHSEARCH_H__
#define __SHORTPATHSEARCH_H__


#define		MINBESTLENGTH 1000000

#include "global.h"

using namespace std;

class PathSearch
{
public:
	PathSearch(int RouteID);
	~PathSearch();
public:	
	int*	 m_nPath;
	bool*    m_nAllowedNode;				//没过去标记为false
	int*     m_nSegment;
	int      m_steps;

public:
	bool Dijkstra(int start, int end);
	bool Updata_Dem(int start, int end, bool flag);
	bool InsertDNode(int Node);
	int  Calculatepath_Dem(void);

	void printPath(int end);
	void Clear();
	int m_RouteID;

private:
	bool	*dij_final;
	int		*dist;
	int		*dij_path;
	int		*temppath;
	int		*path;
	int		*BestPath;
	int      m_nSegmentNum;
	bool     m_isPlanB;

	sDmd	badNode;		//弹出点

	void ClearDijkstraMsg();
	void ChangeDemand();
	bool BestSegment(int start, int end, int node, bool find);
	bool PlanA(int Node);
	bool PlanB(int Node);
};

#endif
