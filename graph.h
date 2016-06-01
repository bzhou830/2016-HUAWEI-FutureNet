#ifndef _GRAPH_H
#define _GRAPH_H
#include <stdio.h>

#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <list>


#include "lib_record.h"
#include "lib_time.h"
#include "lib_io.h"

#define MY_INFINITY 65535

#include "global.h"


class Graph
{
public:
	struct Node{
		int ID;  int start;
		int end; int cost;
	};

public:
	Graph();
	~Graph();
	static std::vector<Node> m_Edges;					//边集合
	static std::list<sDmd>*  m_recDemand[2];			//备份必经点
	static std::vector<sDmd> m_Demand[2];
	static std::vector<int>  m_allDemand;
	
	static int*  m_Graph;								//图
	static int*  m_ID;									//图ID
	static int*  m_Floyd;

	static int	 m_size;								//图节点个数
	static int   m_start;								//起点
	static int   m_end;									//终点
	
	static int*  m_nNodeOut;							//各点的出边

	//static int  m_Thred

	static int*  m_nNodeIn;

	static int* m_Demand_ShortpathCost[2];

	/*创建图*/
	static void Create(char *topo[MAX_EDGE_NUM], int edge_num);
	static void CreateDmande(char *condition[MAX_DEMAND_NUM]);
	static void PrintGraph(int *p);
	static void PrintDemand_ShortpathCost();
	static void CreateFloyd();
private:
	static void removeCon();
	static void SortDemand();
};

#endif
