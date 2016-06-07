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
	char line[100] = { 0 };					//һ������
	char* pT = NULL;
	set<int> vertexIDs;
	sEdge tNode;
	m_edge_num = edge_num;
	for (int i = 0; i < edge_num; ++i)
	{
		strcpy(line, topo[i]);				//����һ��
		pT = strtok(line, ",");				//ID
		tNode.ID = atoi(pT);

		pT = strtok(NULL, ",");				//���
		tNode.start = atoi(pT);
		vertexIDs.insert(tNode.start);

		pT = strtok(NULL, ",");				//�յ�
		tNode.end = atoi(pT);
		vertexIDs.insert(tNode.end);

		pT = strtok(NULL, ",");				//Ȩ��
		tNode.cost = atoi(pT);
		Edges.push_back(tNode);
	}
	m_size = vertexIDs.size();				//ͼ�ڵ����
	//�������ű�
	edge_id = new unsigned int[m_size * m_size];
	edge_cost = new unsigned int[m_size * m_size];
	rtGrph = new unsigned int[m_size * m_size];
	//node_degree = new int[m_size * m_size];
	memset(edge_id, 0xff, sizeof(unsigned int)*m_size * m_size);
	memset(edge_cost, 0xff, sizeof(unsigned int)*m_size * m_size);
	memset(rtGrph, 0x00, sizeof(unsigned int)*m_size * m_size);
	//memset(node_degree, 0x00, sizeof(int)*m_size * m_size);
	for (int i = 0; i < m_size; ++i)				//�Խ���ȫ�����ó�0
		edge_cost[i*m_size + i] = 0;
	for (size_t i = 0; i < Edges.size(); ++i)		//��ÿһ����
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

/*�ڽ���������ҽڵ����һ��ֵ*/
int Graph::GetNodeIn(int NodeNum)
{
	for(int i = 0; i < m_size; ++i)
	{
		if(rtGrph[i*m_size + NodeNum] != 0)
			return i;
	}
	return -1;
}

/*�ڽ������������һ��ֵ*/
int Graph::GetNodeOut(int NodeNum)
{
	for(int i = 0; i < m_size; ++i)
	{
		if(rtGrph[m_size*NodeNum + i] != 0)
			return i;
	}
	return -1;
}


