#ifndef _GRAPH_H
#define _GRAPH_H
#include <stdio.h>
#include <vector>
#include "lp_lib.h"
#include "constraint_expression.h"
#include "global.h"
#define nMax 20

using namespace std;

class expression;

class Graph
{
public:
	Graph();								//构造函数
	~Graph();								//析构函数
	unsigned int *edge_id;					//ID图
	unsigned int *edge_cost;				//权重图
	unsigned int *rtGrph;					//求解结果图

	void CreateTables(char *topo[5000], int edge_num);
	void DealDemands(char *demand);
	
	int IDGetCost(unsigned int id);			//通过边的ID获取cost
	int IDGetEnd(unsigned int id);			//通过边的ID获取end
	int IDGetStart(unsigned int id);		//通过边的ID获取start
	void IDSetCost(unsigned int id, int x);	//通过边的ID设置该ID的cost
	
	int GetNodeIn(int NodeNum);				//该点入边的起点
	int GetNodeOut(int NodeNum);			//该点出边的终点
	
	vector<int> demand_node;				//必须经过的点
	vector<sEdge> Edges;					//边列表
	int m_start;							//起点
	int m_end;								//终点
	int	m_size;								//图规模
	int m_edge_num;
	expression* ex;							//表达式
	void Search();							//搜索
};

#endif


