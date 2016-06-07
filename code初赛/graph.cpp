#include <iostream>
#include <set>
#include <string.h>
#include <stdlib.h>
#include "graph.h"
#include <malloc.h>
#include <vector>
#include <stdio.h>
#include "constraint_expression.h"
using namespace std;

Graph::Graph()
{
}

Graph::~Graph()
{
	delete[]edge_id;
	delete[]edge_cost;
	delete[]rtGrph;
}

void Graph::CreateTables(char *topo[5000], int edge_num)
{
	char line[100] = { 0 };					//一行内容
	char* pT = NULL;
	set<int> vertexIDs;
	sEdge tNode;
	m_edge_num = edge_num;
	for (int i = 0; i < edge_num; ++i)
	{
		strcpy(line, topo[i]);				//读入一行
		pT = strtok(line, ",");				//ID
		tNode.ID = atoi(pT);

		pT = strtok(NULL, ",");				//起点
		tNode.start = atoi(pT);
		vertexIDs.insert(tNode.start);

		pT = strtok(NULL, ",");				//终点
		tNode.end = atoi(pT);
		vertexIDs.insert(tNode.end);

		pT = strtok(NULL, ",");				//权重
		tNode.cost = atoi(pT);
		Edges.push_back(tNode);
	}
	m_size = vertexIDs.size();				//图节点个数
	//建立三张表
	edge_id = new unsigned int[m_size * m_size];
	edge_cost = new unsigned int[m_size * m_size];
	rtGrph = new unsigned int[m_size * m_size];
	//node_degree = new int[m_size * m_size];
	memset(edge_id, 0xff, sizeof(unsigned int)*m_size * m_size);
	memset(edge_cost, 0xff, sizeof(unsigned int)*m_size * m_size);
	memset(rtGrph, 0x00, sizeof(unsigned int)*m_size * m_size);
	//memset(node_degree, 0x00, sizeof(int)*m_size * m_size);
	for (int i = 0; i < m_size; ++i)				//对角线全部设置成0
		edge_cost[i*m_size + i] = 0;
	for (size_t i = 0; i < Edges.size(); ++i)		//对每一条边
	{
		edge_id[Edges[i].start*m_size + Edges[i].end] = Edges[i].ID;
		edge_cost[Edges[i].start*m_size + Edges[i].end] = Edges[i].cost;
	}
#ifdef TEST
	cout<< "points:" << m_size <<endl;
	cout<< "edges:" << Edges.size() << endl;
	cout<<"per NEW"<<endl;
#endif
	ex = new expression(edge_id, edge_cost, (int)(Edges.size()), m_size);
	ex->pGrph = this;
}


void Graph::Search()
{
	/*
	cout << "cost Tables" << endl;
	for (int i = 0; i < m_size; ++i)
	{
		for (int j = 0; j < m_size; ++j)
		{
			cout << edge_cost[i*m_size + j] << " ";
		}
		cout << endl;
	}
	cout << endl << "ID Tables" << endl;
	for (int i = 0; i < m_size; ++i)
	{
		for (int j = 0; j < m_size; ++j)
		{
			cout << edge_id[i*m_size + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	*/
	ex->GetResult();
}

void Graph::DealDemands(char *demand)
{
#ifdef TEST
	cout<<"per Deal"<<endl;
#endif
	ex->DealDemands(demand);
#ifdef TEST
	cout<<"Deal OK!"<<endl;
#endif
}


int Graph::IDGetCost(unsigned int id)
{
	return Edges[id].cost;
}

int Graph::IDGetStart(unsigned int id)
{
	return Edges[id].start;
}

int Graph::IDGetEnd(unsigned int id)
{
	return Edges[id].end;
}

void Graph::IDSetCost(unsigned int id, int x)
{
	Edges[id].cost = x;
}

/*在结果表里面找节点的上一个值*/
int Graph::GetNodeIn(int NodeNum)
{
	for(int i = 0; i < m_size; ++i)
	{
		if(rtGrph[i*m_size + NodeNum] != 0)
			return i;
	}
	return -1;
}

/*在结果表里面找下一个值*/
int Graph::GetNodeOut(int NodeNum)
{
	for(int i = 0; i < m_size; ++i)
	{
		if(rtGrph[m_size*NodeNum + i] != 0)
			return i;
	}
	return -1;
}


