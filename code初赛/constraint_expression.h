#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__
#include "lp_lib.h"
#include "global.h"
#include "graph.h"
#include <vector>
using namespace std;

class Graph; 

class expression
{
public:
	expression(unsigned int *id, unsigned int *cost, int num, int sz);
	~expression();
private:
	unsigned int  *edge_id;
	unsigned int  *edge_cost;
	unsigned int edge_num;
	int size;
	vector<int> special_node;	//必经点
	vector<int> edges; 			//边集，判断环使用
	vector<int> nodes; 			//点集，判断环使用
	vector<int> vt_rt;				//结果
	int m_start;
	int m_end;
	lprec *lp;
	REAL *row;
	int *colno;
	char *pArray;
public:
	void DealDemands(char *demand);
	void handle_expression();
	void AddExtraConstrain();
	//bool HasCircle(REAL *row, int& Enum);
	void CreateObj();
	void GetResult();
	bool SearchCircle();
	bool SolveMD();
	void PrintRt();
	Graph* pGrph;
};



#endif
