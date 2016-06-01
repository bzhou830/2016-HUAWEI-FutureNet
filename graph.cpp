#include <iostream>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <iomanip>
#include <ctime>


#include "graph.h"


Graph::Graph()
{
}


/*析构函数中，销毁数据*/
Graph::~Graph()
{
	/*
	if (m_Graph != NULL)
	{
		delete[] m_Graph;
		m_Graph = NULL;
	}
	if (m_Demand_ShortpathCost[0] != NULL)
	{
		delete[] m_Demand_ShortpathCost[0];
		m_Demand_ShortpathCost[0] = NULL;
	}
	if (m_Demand_ShortpathCost[1] != NULL)
	{
		delete[] m_Demand_ShortpathCost[1];
		m_Demand_ShortpathCost[1] = NULL;
	}
	if (m_nNodeOut != NULL)
	{
		delete[] m_nNodeOut;
		m_nNodeOut = NULL;
	}
	*/
}

/*静态变量声明并赋初值*/
std::vector<Graph::Node> Graph::m_Edges(5000);					//初始创建5000个空间
std::vector<int>  Graph::m_allDemand(202);						//初始创建

std::vector<sDmd> Graph::m_Demand[2];							//必经点
std::list<sDmd>* Graph::m_recDemand[2];							//备份必经点

int* Graph::m_Graph = 0;										//邻接矩阵图
int* Graph::m_ID = 0;											//ID图
int* Graph::m_Floyd = 0;
int* Graph::m_Demand_ShortpathCost[2];

int* Graph::m_nNodeOut = 0;										//各个点的出边数目
int* Graph::m_nNodeIn = 0;										//各个点的入边数目

int	 Graph::m_size = 0;											//图大小
int  Graph::m_start = 0;										//起始点
int  Graph::m_end = 0;											//终点


/*用邻接矩阵表示一个图*/
void Graph::Create(char *topo[MAX_EDGE_NUM], int edge_num)
{
	char *split = NULL;											//分割符
	m_Edges.clear();
	Node tmp;

	for (int i = 0; i < edge_num; ++i)
	{
		split = strtok(topo[i], ",");							//截取
		tmp.ID = atoi(split); split = strtok(NULL, ",");		//边Id
		tmp.start = atoi(split); split = strtok(NULL, ",");		//起点
		tmp.end = atoi(split); split = strtok(NULL, ",");		//终点
		tmp.cost = atoi(split);									//权重
		m_Edges.push_back(tmp);									//存到表中
		
		if (m_size < tmp.start) m_size = tmp.start;				
		if (m_size < tmp.end) m_size = tmp.end;
	}

	++m_size;
	m_Graph = new int[m_size * m_size];							//分配图内存
	memset(m_Graph, 0, (m_size * m_size) * sizeof(int));
	
	m_ID = new int[m_size * m_size];
	memset(m_ID, 0, (m_size * m_size) * sizeof(int));

	m_nNodeOut = new int[m_size];								//各点的出边数目
	memset(m_nNodeOut, 0, m_size * sizeof(int));

	m_nNodeIn = new int[m_size];
	memset(m_nNodeIn, 0, m_size * sizeof(int));
	
	for(int i = 0; i < m_size; i++)	
	{
		for(int j = 0; j < m_size; j++)
		{
			if(i == j) m_Graph[i*m_size + j] = 0;
			else m_Graph[i*m_size + j] = MY_INFINITY;
		}
	}

	for (unsigned int i = 0; i < m_Edges.size(); ++i)
	{
		m_Graph[m_Edges[i].start * m_size + m_Edges[i].end] = m_Edges[i].cost;
		m_ID[m_Edges[i].start * m_size + m_Edges[i].end] = m_Edges[i].ID;
		m_nNodeOut[m_Edges[i].start]++;								//tmp.start的出边加
		m_nNodeIn[m_Edges[i].end]++;
	}

#ifdef _DEBUG
	std::cout << "m_size = " << m_size << std::endl;
#endif // DEBUG
	return;
}


/*排序规则*/
bool Sortfunction(const sDmd& i, const sDmd& j)
{
	if (i.sum > j.sum)
		return true;
	else if (i.sum < j.sum)
		return false;

	if (i.in > j.in)
		return true;
	else if (i.in < j.in)
		return false;

	if (i.out > j.out)
		return true;
	return false;
}


/*必经点*/
void Graph::CreateDmande(char *condition[MAX_DEMAND_NUM])
{
	char *split = NULL;											//分割符
	m_Demand[0].clear();
	m_Demand[1].clear();
	m_allDemand.clear();
	split = strtok(condition[0], ",");
	split = strtok(NULL, ",");
	m_start = atoi(split);   split = strtok(NULL, ",");			//起点
	m_end = atoi(split);     split = strtok(NULL, "|");			//终点
	
	sDmd dmnd = {0, 0};
	while (split)
	{
		dmnd.id = atoi(split);
		dmnd.sum = m_nNodeOut[dmnd.id] + m_nNodeIn[dmnd.id];
		dmnd.out = m_nNodeOut[dmnd.id];
		dmnd.in = m_nNodeIn[dmnd.id];

		m_Demand[0].push_back(dmnd);							//Demand_1必须要经过的点
		m_allDemand.push_back(dmnd.id);							//所有必经点
		
		split = strtok(NULL, "|");
	}

	m_Demand_ShortpathCost[0] = new int[m_size*m_size];
	memset(m_Demand_ShortpathCost[0], 0, (m_size*m_size) * sizeof(int));

	for(int i = 0; i < m_size; i++)	
	{
		for(int j = 0; j < m_size; j++)
		{
			m_Demand_ShortpathCost[0][i*m_size + j] = MY_INFINITY;
		}
	}

	//备路径
	split = strtok(condition[1], ",");							//跳过前面起始点，终点
	split = strtok(NULL, ","); split = strtok(NULL, ","); 
	split = strtok(NULL, "|");
	while (split)
	{
		dmnd.id = atoi(split);
		dmnd.sum = m_nNodeOut[dmnd.id] + m_nNodeIn[dmnd.id];
		dmnd.out = m_nNodeOut[dmnd.id];
		dmnd.in = m_nNodeIn[dmnd.id];

		m_Demand[1].push_back(dmnd);							//Demand_2必须要经过的点
		m_allDemand.push_back(dmnd.id);							//所有必经点

		split = strtok(NULL, "|");
	}
	m_Demand_ShortpathCost[1] = new int[m_size*m_size];
	memset(m_Demand_ShortpathCost[1], 0, (m_size*m_size) * sizeof(int));
	
	for(int i = 0; i < m_size; i++)	
	{
		for(int j = 0; j < m_size; j++)
		{
			m_Demand_ShortpathCost[1][i*m_size + j] = MY_INFINITY;
		}
	}

	SortDemand();											//对必经点进行排序
	//CreateFloyd();											//Floyd表
	removeCon();
	//PrintGraph(m_Graph);
}


void Graph::SortDemand()
{
	m_allDemand.push_back(m_start);
	m_allDemand.push_back(m_end);
	std::sort(m_allDemand.begin(), m_allDemand.end());
	nIter iter = std::unique(m_allDemand.begin(), m_allDemand.end());
	m_allDemand.erase(iter, m_allDemand.end());

	std::sort(m_Demand[0].begin(), m_Demand[0].end(), Sortfunction);
	std::sort(m_Demand[1].begin(), m_Demand[1].end(), Sortfunction);
	m_recDemand[0] = new std::list<sDmd>(m_Demand[0].begin(), m_Demand[0].end());
	m_recDemand[1] = new std::list<sDmd>(m_Demand[1].begin(), m_Demand[1].end());
}


/*创建floyd表*/
void Graph::CreateFloyd()
{
	m_Floyd = new int[m_size * m_size];							//分配图内存
	memcpy(m_Floyd, m_Graph, m_size * m_size * sizeof(int));	//直接拷贝原图
	
	int k = 0, v = 0, w = 0;
 	int Ex_a = 0;
 	int *pEx_b = 0;
	int *pEx_c = 0;
	int count = m_allDemand.size();
	int id = 0;
	for (k = 0; k < m_size; ++k)
	{
		for (v = 0; v < count; ++v)
		{
			id = m_allDemand[v];
			Ex_a = m_Floyd[id * m_size + k];
			for (w = 0; w < m_size; ++w)
			{
				pEx_b = &m_Floyd[k*m_size + w];
				pEx_c = &m_Floyd[id*m_size + w];
				if (*pEx_c > Ex_a + *pEx_b)
				{
					*pEx_c = Ex_a + *pEx_b;
				}
			}
		}
	}
	/*
	int k = 0, v = 0, w = 0;
	for (k = 0; k < m_size; ++k)
	{
		for (v = 0; v < m_size; ++v)
		{
			for (w = 0; w < m_size; ++w)
			{
				if (m_Floyd[v*m_size + w] > m_Floyd[v * m_size + k] + m_Floyd[k*m_size + w])
				{
					m_Floyd[v*m_size + w] = m_Floyd[v * m_size + k] + m_Floyd[k*m_size + w];
				}
			}
		}
	}
	*/
}


/*预处理图*/
void Graph::removeCon()
{
	for (unsigned int i = 0; i < m_Demand[0].size(); ++i)
	{
		for (unsigned int j = 0; j < m_Demand[1].size(); ++j)
		{

			if (m_Graph[m_Demand[0][i].id*m_size + m_Demand[1][j].id] != MY_INFINITY)
				m_Graph[m_Demand[0][i].id*m_size + m_Demand[1][j].id] += 100;

			if (m_Graph[m_Demand[1][j].id*m_size + m_Demand[0][i].id] != MY_INFINITY)
				m_Graph[m_Demand[1][j].id*m_size + m_Demand[0][i].id] += 100;
		}
	}
}



/*打印图*/
void Graph::PrintGraph(int* p)
{
	for (int i = 0; i < m_size; ++i)
	{
		for (int j = 0; j < m_size; ++j)
		{
			std::cout << std::setw(6) << (int)(p[i*m_size + j]);
		}
		std::cout << std::endl;
	}
}


void Graph::PrintDemand_ShortpathCost()
{
	for (int i = 0; i < m_size; ++i)
	{
		for (int j = 0; j < m_size; ++j)
		{
			std::cout <<std::setw(6) << (int)(m_Demand_ShortpathCost[0][i*m_size + j]);
		}
		std::cout << std::endl;
	}
}




