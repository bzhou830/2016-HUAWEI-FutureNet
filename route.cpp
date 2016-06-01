#include "route.h"
#include "lib_record.h"
#include "lib/lib_time.h"

#include <stdio.h>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "graph.h"
#include "PathSearch.h"
#include "Rule.h"



void search_route(char *topo[MAX_EDGE_NUM], int edge_num, char *demand[MAX_DEMAND_NUM], int demand_num)
{
	Graph::Create(topo, edge_num);						//构建图
	Graph::CreateDmande(demand);						//读入需求点

	Rule::Search();
	/*
	PathSearch SP1(0);
	
	bool *cb = new bool[Graph::m_Edges.size()];			//重边检查
	memset(cb, 0, Graph::m_Edges.size()*sizeof(bool));

	int cb_count = 0;									//重边数量
	int cost1 = 0;										//权重1
	int cost2 = 0;										//权重2

	while (!search_path(SP1, 0))						//找不到插入位置，则将插不进的点最先插入
	{
		mm.push_back(Graph::m_recDemand[0]->back());
		if (mm.size() > 4)
		{
			mm.erase(mm.begin());
			if (mm[0].id == mm[2].id && mm[1].id == mm[3].id)
			{
				cout<< "无解" << endl;
				break;
			}
		}
		Graph::m_Demand[0].clear();	
		for (reItr i = Graph::m_recDemand[0]->begin(); i != Graph::m_recDemand[0]->end(); ++i)
			Graph::m_Demand[0].push_back(*i);
	}

	int idex = Graph::m_start;
	while (idex != Graph::m_end)													//打印位置			
	{
		record_result(WORK_PATH, Graph::m_ID[idex * Graph::m_size + SP1.m_nPath[idex]]);
		cout << Graph::m_ID[idex * Graph::m_size + SP1.m_nPath[idex]] << " | ";
		cb[Graph::m_ID[idex * Graph::m_size + SP1.m_nPath[idex]]] = true;			//标定边已经走过
		cost1 += Graph::m_Graph[idex * Graph::m_size + SP1.m_nPath[idex]];		
		idex = SP1.m_nPath[idex];
	}

	idex = Graph::m_start;
	while (idex != Graph::m_end)
	{
		Graph::m_Graph[idex * Graph::m_size + SP1.m_nPath[idex]] = MY_INFINITY;			//第一条路上加权重，减少重边
		idex = SP1.m_nPath[idex];
	}
	

	cout << endl;
	cout << endl;
	cout << "权重" << cost1 << endl;

	//第二条路径
	mm.clear();
	//ShortpathSearch SP2(1);
	SP1.m_RouteID = 1;
	while (!search_path(SP1, 1))		//找不到插入位置，则将插不进的点最先插入
	{
		mm.push_back(Graph::m_recDemand[0]->back());
		if (mm.size() > 4)
		{
			mm.erase(mm.begin());
			if (mm[0].id == mm[2].id && mm[1].id == mm[3].id)
			{
				cout<< "无解" << endl;
				break;
			}
		}
		Graph::m_Demand[1].clear();	
		for (reItr i = Graph::m_recDemand[1]->begin(); i != Graph::m_recDemand[1]->end(); ++i)
			Graph::m_Demand[1].push_back(*i);
	}

	idex = Graph::m_start;
	while (idex != Graph::m_end)		//打印位置			
	{
		record_result(BACK_PATH, Graph::m_ID[idex*Graph::m_size + SP1.m_nPath[idex]]);
		cout << Graph::m_ID[idex*Graph::m_size + SP1.m_nPath[idex]] << " | ";

		if (cb[Graph::m_ID[idex * Graph::m_size + SP1.m_nPath[idex]]])
			++cb_count;

		cost2 += Graph::m_Graph[idex * Graph::m_size + SP1.m_nPath[idex]];

		idex = SP1.m_nPath[idex];
	}
	cout<<endl;
	cout<<endl;
	cout << "权重 " << cost2 << endl;
	cout << "重边 " << cb_count << endl;
	print_time("~A");
	*/
}








