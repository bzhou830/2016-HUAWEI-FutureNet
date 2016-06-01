#include "PathSearch.h"
#include "graph.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include "global.h"
#include <algorithm>

#define TEST

//传入需要计算的路径，0表示主路径，1表示备路径
PathSearch::PathSearch(int RouteID)
:m_RouteID(RouteID), m_isPlanB(false)
{
	m_nAllowedNode = new bool[Graph::m_size];
	memset(m_nAllowedNode, 0, sizeof(bool)*Graph::m_size);

	m_nPath = new int[Graph::m_size];
	memset(m_nPath, 0, sizeof(int)*Graph::m_size);

	m_nSegment = new int[Graph::m_size];
	memset(m_nSegment, 0, sizeof(int)*Graph::m_size);

	dij_final = new bool[Graph::m_size];
	memset(dij_final, 0, sizeof(bool)* Graph::m_size);

	dist = new int[Graph::m_size];
	memset(dist, 0, sizeof(int)* Graph::m_size);

	dij_path = new int[Graph::m_size];
	memset(dij_path, 0, sizeof(int)* Graph::m_size);

	temppath = new int[Graph::m_size];
	memset(dij_path, 0, sizeof(int)* Graph::m_size);

	path = new int[Graph::m_size];
	memset(path, 0, sizeof(int)* Graph::m_size);

	BestPath = new int[Graph::m_size];
	memset(BestPath, 0, sizeof(int)* Graph::m_size);

	m_steps = 0;
	m_nSegmentNum = 0;
}


PathSearch::~PathSearch()
{
	/*
	if (m_nAllowedNode != NULL)
	{
	delete[] m_nAllowedNode;
	m_nAllowedNode = NULL;
	}
	if (m_nPath != NULL)
	{
	delete[] m_nPath;
	m_nPath = NULL;
	}
	if (m_nSegment != NULL)
	{
	delete[] m_nSegment;
	m_nSegment = NULL;
	}
	if (dij_final != NULL)
	{
	delete[] dij_final;
	dij_final = NULL;
	}
	if (dist != NULL)
	{
	delete[] dist;
	dist = NULL;
	}
	if (dij_path != NULL)
	{
	delete[] dij_path;
	dij_path = NULL;
	}
	if (temppath != NULL)
	{
	delete[] temppath;
	temppath = NULL;
	}
	if (path != NULL)
	{
	delete[] path;
	path = NULL;
	}
	if (BestPath != NULL)
	{
	delete[] BestPath;
	BestPath = NULL;
	}
	*/
}


void PathSearch::ClearDijkstraMsg()
{
	memset(dij_final, 0, sizeof(bool)* Graph::m_size);				//已经求得最优路径的点
	memset(dist, 0, sizeof(int)* Graph::m_size);					//
	memset(dij_path, 0, sizeof(int)* Graph::m_size);				//
	memset(temppath, 0, sizeof(int)* Graph::m_size);
}


bool PathSearch::Dijkstra(int start, int end)
{
	if (m_nAllowedNode[end] == true)
		return false;

	ClearDijkstraMsg();												//清零

	for (int i = 0; i < Graph::m_size; i++)
	{
		dist[i] = Graph::m_Graph[start*Graph::m_size + i];			
		if (m_nAllowedNode[i] == true){								//避开已经走过的点，防止成环
			dij_final[i] = true;
		}
	}

	dist[start] = 0;
	dij_final[start] = true;

	int  min = MY_INFINITY;
	int tempCurs = 0;
	int i = 1, j = 0;
	for (i = 1; i < Graph::m_size; i++)
	{
		min = MY_INFINITY;
		for (j = 0; j < Graph::m_size; j++)
		{
			if ((!dij_final[j]) && (dist[j] < min))
			{
				min = dist[j];
				tempCurs = j;
			}
		}
		dij_final[tempCurs] = true;										//更新最优路径表
		for (j = 0; j < Graph::m_size; ++j)
		{
			if ((!dij_final[j]) && 
				(min + Graph::m_Graph[tempCurs*Graph::m_size + j] < dist[j]))
			{
				dist[j] = min + Graph::m_Graph[tempCurs*Graph::m_size + j];
				dij_path[j] = tempCurs;
			}
		}
		if (dij_final[end] == true)
			break;
	}

	if (dist[end] == MY_INFINITY) return false;

	int m = end;
	int count = 0;
	while ((dij_path[m] != 0) && (count != Graph::m_size))
	{
		temppath[count] = m;
		count++;
		m = dij_path[m];
	}
	temppath[count++] = m;

	if (count == Graph::m_size)							//未生成start--end最短路径
		return false;

	temppath[count] = start;
	while (count > 0)									//生成start--end最短路径，记录走过的点
	{
		m_nPath[temppath[count]] = temppath[count - 1];
		m_nAllowedNode[temppath[count]] = true;
		count--;
	}
	m_nAllowedNode[end] = true;							//走过end点 true
	return true;
}


bool PathSearch::Updata_Dem(int start, int end, bool flag)
{
	int Cur = start;
	int tempCur = start;
	int tLength = 0;
	if (Dijkstra(start, end) == true)
	{
		if (flag == true)
		{
			while (Cur != end)
			{
				tLength += Graph::m_Graph[Cur*Graph::m_size + m_nPath[Cur]];
				Cur = m_nPath[Cur];
				if (Cur != end)
				{
					for (deItr it = Graph::m_Demand[m_RouteID].begin();
						it != Graph::m_Demand[m_RouteID].end(); it++)
					{
						if (Cur == it->id)
						{
							++m_nSegmentNum;
							m_nSegment[tempCur] = Cur;
							Graph::m_Demand_ShortpathCost[m_RouteID][tempCur*Graph::m_size + Cur] = tLength;
							tLength = 0;
							tempCur = Cur;
							it = Graph::m_Demand[m_RouteID].erase(it);
							break;
						}
					}
				}
				else
				{
					++m_nSegmentNum;
					m_nSegment[tempCur] = Cur;
					Graph::m_Demand_ShortpathCost[m_RouteID][tempCur*Graph::m_size + Cur] = tLength;
					tLength = 0;
				}
			}
		}
		else
		{
			while (Cur != end)
			{
				tLength += Graph::m_Graph[Cur*Graph::m_size + m_nPath[Cur]];
				Cur = m_nPath[Cur];
				if (Cur == end)
				{
					m_nSegment[tempCur] = Cur;
					Graph::m_Demand_ShortpathCost[m_RouteID][tempCur*Graph::m_size + Cur] = tLength;
				}
			}
		}
		return true;
	}
	return false;
}


bool SortLen(const sSegment& i, const sSegment& j)
{
	return i.cost < j.cost;
}


bool PathSearch::InsertDNode(int Node)
{
	if (!m_isPlanB)
		return PlanA(Node);
	else
		return PlanB(Node);
}

/**/
bool PathSearch::BestSegment(int start, int end, int node, bool find)
{
	int bestStart = start;
	int bestEnd = end;
	int bestNode = node;
	int Cur = 0;
	int tempCur = 0;
	int templength = 0;
	if (find)
	{
		m_nSegment[bestStart] = 0;
		Graph::m_Demand_ShortpathCost[m_RouteID][bestStart * Graph::m_size + bestEnd] = MY_INFINITY;
		Cur = bestStart;
		while (m_nPath[Cur] != bestEnd)
		{
			m_nAllowedNode[m_nPath[Cur]] = false;
			Cur = m_nPath[Cur];
		}
		Cur = bestStart;
		while (Cur != bestEnd)
		{
			m_nPath[Cur] = BestPath[Cur];
			m_nAllowedNode[BestPath[Cur]] = true;
			Cur = BestPath[Cur];
		}
		Cur = bestStart;
		tempCur = bestStart;
		while (Cur != bestEnd)
		{
			templength += Graph::m_Graph[Cur*Graph::m_size + m_nPath[Cur]];
			Cur = m_nPath[Cur];
			if (Cur != bestEnd)
			{
				if (Cur == bestNode)
				{
					++m_nSegmentNum;
					m_nSegment[tempCur] = Cur;
					Graph::m_Demand_ShortpathCost[m_RouteID][tempCur * Graph::m_size + Cur] = templength;
					templength = 0;
					tempCur = Cur;
				}
				else
				{
					for (deItr it = Graph::m_Demand[m_RouteID].begin(); it != Graph::m_Demand[m_RouteID].end(); it++)
					{
						if (Cur == it->id)
						{
							m_nSegment[tempCur] = Cur;
							Graph::m_Demand_ShortpathCost[m_RouteID][tempCur*Graph::m_size + Cur] = templength;
							templength = 0;
							tempCur = Cur;
							it = Graph::m_Demand[m_RouteID].erase(it);
							break;
						}
					}
				}
			}
			else
			{
				m_nSegment[tempCur] = Cur;
				Graph::m_Demand_ShortpathCost[m_RouteID][tempCur * Graph::m_size + Cur] = templength;
				templength = 0;
			}
		}
		return true;
	}
	else //回溯
	{
		badNode.id = bestNode;
		badNode.sum = Graph::m_nNodeOut[badNode.id] + Graph::m_nNodeIn[badNode.id];
		badNode.in = Graph::m_nNodeIn[badNode.id];
		badNode.out = Graph::m_nNodeOut[badNode.id];

#ifdef _DEBUG
		cout << "弹回Node " << bestNode << endl;
		cout << "剩余 " << Graph::m_Demand[m_RouteID].size() << endl;
#endif
		ChangeDemand();
		return false;
	}
}


bool operator==(const sDmd &a, const sDmd &b)
{
	if (a.id == b.id)
		return true;
	return false;
}


int PathSearch::Calculatepath_Dem()
{
	int tPathLen = 0;
	int i = Graph::m_start;
	while (i != Graph::m_end)
	{
		tPathLen += Graph::m_Demand_ShortpathCost[m_RouteID][i * Graph::m_size + m_nSegment[i]];
		i = m_nSegment[i];
	}
	return tPathLen;
}


void PathSearch::printPath(int end)
{
#ifdef TEST
	int i = Graph::m_start;
	while (i != end)
	{
		cout << i << "-->";
		i = m_nPath[i];
	}
	cout << i << endl;
#endif // TEST
}


void PathSearch::Clear()
{
	memset(m_nAllowedNode, 0, sizeof(bool)*Graph::m_size);
	memset(m_nPath, 0, sizeof(int)*Graph::m_size);
	memset(m_nSegment, 0, sizeof(int)*Graph::m_size);
	memset(dij_final, 0, sizeof(bool)* Graph::m_size);
	memset(dist, 0, sizeof(int)* Graph::m_size);
	memset(dij_path, 0, sizeof(int)* Graph::m_size);
	memset(dij_path, 0, sizeof(int)* Graph::m_size);
	memset(path, 0, sizeof(int)* Graph::m_size);
	memset(BestPath, 0, sizeof(int)* Graph::m_size);
}


void PathSearch::ChangeDemand()
{
	(Graph::m_recDemand[m_RouteID])->remove(badNode);
	/*if (Graph::m_Demand[m_RouteID].size() * 3 < (Graph::m_recDemand[m_RouteID])->size())
	{
	int i = Graph::m_recDemand[m_RouteID]->size() / 2;
	reItr itr = (Graph::m_recDemand[m_RouteID])->begin();
	while (i > 0)
	{
	++itr; ++i;
	}
	(Graph::m_recDemand[m_RouteID])->insert(itr, badNode);
	}
	else*/
	{
		(Graph::m_recDemand[m_RouteID])->push_back(badNode);
	}
	Graph::CreateFloyd();
	m_isPlanB = true;
}

/*原方案*/
bool PathSearch::PlanA(int Node)
{
	int nextDemendNode = Graph::m_start;
	memset(path, 0, sizeof(int)* Graph::m_size);
	memset(BestPath, 0, sizeof(int)* Graph::m_size);

	int BestStart = 0;
	int BestEnd = 0;
	int dir = 0;
	int length = MINBESTLENGTH;
	int Cur = 0;

	Cur = Graph::m_start;
	while (Cur != Graph::m_end)
	{
		path[Cur] = m_nPath[Cur];
		Cur = m_nPath[Cur];
	}

	bool hasRout = false;

	while (nextDemendNode != Graph::m_end)
	{
		Cur = nextDemendNode;
		int start = Cur;
		nextDemendNode = m_nSegment[nextDemendNode];
		int end = nextDemendNode;
		dir = Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + end];
		Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + end] = MY_INFINITY;
		m_nSegment[start] = 0;
		while (m_nPath[Cur] != end)
		{
			m_nAllowedNode[m_nPath[Cur]] = false;
			Cur = m_nPath[Cur];
		}

		if (false == Updata_Dem(start, Node, false))
		{
			Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + end] = dir;
			m_nSegment[start] = end;
			Cur = start;
			while (m_nPath[Cur] != end)
			{
				m_nAllowedNode[m_nPath[Cur]] = true;
				Cur = m_nPath[Cur];
			}
			continue;
		}
		m_nAllowedNode[end] = false;
		if (false == Updata_Dem(Node, end, false))
		{
			Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + Node] = MY_INFINITY;
			m_nSegment[start] = 0;
			m_nSegment[start] = end;
			Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + end] = dir;
			Cur = start;
			while (Cur != Node)
			{
				m_nAllowedNode[m_nPath[Cur]] = false;
				Cur = m_nPath[Cur];
			}
			Cur = start;
			while (Cur != end)
			{
				m_nPath[Cur] = path[Cur];
				m_nAllowedNode[path[Cur]] = true;
				Cur = path[Cur];
			}
			continue;
		}

		int lgh = Calculatepath_Dem();
		if (lgh < length)
		{
			hasRout = true;
			length = lgh;
			BestStart = start;
			BestEnd = end;
			Cur = BestStart;
			while (Cur != BestEnd)
			{
				BestPath[Cur] = m_nPath[Cur];
				Cur = m_nPath[Cur];
			}
		}
		Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + Node] = MY_INFINITY;
		m_nSegment[start] = 0;
		Graph::m_Demand_ShortpathCost[m_RouteID][Node * Graph::m_size + end] = MY_INFINITY;
		m_nSegment[Node] = 0;
		m_nSegment[start] = end;
		Cur = start;
		while (m_nPath[Cur] != end)
		{
			m_nAllowedNode[m_nPath[Cur]] = false;
			Cur = m_nPath[Cur];
		}
		Cur = start;
		while (Cur != end)
		{
			m_nPath[Cur] = path[Cur];
			m_nAllowedNode[path[Cur]] = true;
			Cur = path[Cur];
		}
		Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + end] = dir;

		//if (hasRout)
		//break;
		if (hasRout && m_nSegmentNum > 20 && Graph::m_size > 1400)
			break;
	}
	return BestSegment(BestStart, BestEnd, Node, hasRout);
}


bool PathSearch::PlanB(int Node)
{
	memset(path, 0, sizeof(int)* Graph::m_size);
	memset(BestPath, 0, sizeof(int)* Graph::m_size);

	int dir = 0;
	int length = MINBESTLENGTH;
	int index = Graph::m_start;
	
	while (index != Graph::m_end)
	{
		path[index] = m_nPath[index];
		index = m_nPath[index];
	}

	vector<sSegment> rec;
	sSegment seg;

	index = Graph::m_start;
	while (index != Graph::m_end)
	{
		seg.start = index;
		seg.end = m_nSegment[index];
		seg.cost = Graph::m_Floyd[index * Graph::m_size + Node]
			+ Graph::m_Floyd[Node * Graph::m_size + seg.end]
			- Graph::m_Demand_ShortpathCost[m_RouteID][seg.start * Graph::m_size + seg.end];
		rec.push_back(seg);
		index = m_nSegment[index];
	}
	std::sort(rec.begin(), rec.end(), SortLen);
	
	//带入起点终点
	sSegIter itr = rec.begin();
	bool con = false;
	int start = 0;
	int end = 0;
	while ( !con )
	{
		if (itr == rec.end())
			break;
		start = itr->start;
		end = itr->end;

		dir = Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + end];			//临时保存片段（start---end）试探前的权值
		Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + end] = MY_INFINITY;	//片段（start---end）权值设为最大
		m_nSegment[start] = 0;																	//将原来start的后继end修改改为0
		
		index = start;
		while (m_nPath[index] != end)															//片段（start---end）中间经过的点设为false，表示没走过
		{
			m_nAllowedNode[m_nPath[index]] = false;
			index = m_nPath[index];
		}

		if (false == Updata_Dem(start, Node, false))											//前一个片段（start----Node）没插入成功
		{																						//恢复片段（start---end）试探前的权值			
			Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + end] = dir;
			m_nSegment[start] = end;			//恢复start的后继end								
			index = start;
			while (m_nPath[index] != end)		//恢复片段（start---end）中间经过的点true
			{
				m_nAllowedNode[m_nPath[index]] = true; 
				index = m_nPath[index];
			}
			++itr;
			continue;
		}
		
		m_nAllowedNode[end] = false;

		if (false == Updata_Dem(Node, end, false))												//后一个片段（Node----end）没有插入成功
		{
			Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + Node] = MY_INFINITY;
			m_nSegment[start] = end;
			Graph::m_Demand_ShortpathCost[m_RouteID][start * Graph::m_size + end] = dir;
			index = start;
			while (index != Node)
			{
				m_nAllowedNode[m_nPath[index]] = false;
				index = m_nPath[index];
			}

			index = start;
			while (index != end)
			{
				m_nPath[index] = path[index];
				m_nAllowedNode[path[index]] = true;
				index = path[index];
			}
			++itr;
			continue;
		}
		index = start;
		while (index != end)
		{
			BestPath[index] = m_nPath[index];
			index = m_nPath[index];
		}
		con = true;
	}
	return BestSegment(start, end, Node, con);
}

