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
	Graph();								//���캯��
	~Graph();								//��������
	unsigned int *edge_id;					//IDͼ
	unsigned int *edge_cost;				//Ȩ��ͼ
	unsigned int *rtGrph;					//�����ͼ

	void CreateTables(char *topo[5000], int edge_num);
	void DealDemands(char *demand);
	
	int IDGetCost(unsigned int id);			//ͨ���ߵ�ID��ȡcost
	int IDGetEnd(unsigned int id);			//ͨ���ߵ�ID��ȡend
	int IDGetStart(unsigned int id);		//ͨ���ߵ�ID��ȡstart
	void IDSetCost(unsigned int id, int x);	//ͨ���ߵ�ID���ø�ID��cost
	
	int GetNodeIn(int NodeNum);				//�õ���ߵ����
	int GetNodeOut(int NodeNum);			//�õ���ߵ��յ�
	
	vector<int> demand_node;				//���뾭���ĵ�
	vector<sEdge> Edges;					//���б�
	int m_start;							//���
	int m_end;								//�յ�
	int	m_size;								//ͼ��ģ
	int m_edge_num;
	expression* ex;							//���ʽ
	void Search();							//����
};

#endif


