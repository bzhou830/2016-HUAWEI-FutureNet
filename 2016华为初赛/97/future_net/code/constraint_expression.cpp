#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "constraint_expression.h"
#include <algorithm>
#include "graph.h"
#include <iostream>
#include "lib_record.h"
#include "lp_lib.h"

using namespace std;

expression::expression(unsigned int *id, unsigned int *cost, int num, int sz)
	:edge_id(id), edge_cost(cost), edge_num(num), size(sz)
{
	pArray = new char[601];			//标记已经经过的点
}

expression::~expression()
{
	if(row != NULL)
		free(row);
	if(colno != NULL)
		free(colno);
	if(lp != NULL)
		delete_lp(lp);
	if(pArray != NULL)
		delete[] pArray;
}


void expression::DealDemands(char *demand)
{
	char* p = NULL;
	char* q = NULL;
	char *line = (char*)malloc(sizeof(char) * 1024);
	strcpy(line, demand);
	p = strtok(line, ",");
	m_start = atoi(p);					//起点
	p = strtok(NULL, ",");
	m_end = atoi(p);					//终点
	q = strtok(NULL, ",");
	p = strtok(q, "|");
	while (p)
	{
		int id = atoi(p);
		special_node.push_back(id);		//必须要经过的点
		p = strtok(NULL, "|");
	}
	free(line);
}

/*判断是否存在环,返回必经点为起点的那一条边*/
/*
bool expression::HasCircle(REAL *row, int& Enum)
{
	memset(pArray, 0, 601*sizeof(char));			//清空标记数组
	int start = 700;
	int end = 700;
	for(unsigned int i = 0; i < edge_num; ++i)		//对于每一条边
	{
		if(row[i] == 0) continue;					//如果存在该解
		start = pGrph->IDGetStart(i);				//由边ID获得改变的起点和终点
		end = pGrph->IDGetEnd(i);					//
		if(pArray[start] >= 1 || pArray[end] >= 1)	//该边已经标记过，表示形成了环
		{
			while( !any_of(special_node.begin(), special_node.end(), start)) //判断是否是必须经过的点
			{
				//在结果里面去遍历
				
				Enum = i;
			}
				
			//cout<< "HasCircle: edge_num" << i << endl;
			//pGrph->IDSetCost(row[i], 1000)
			return true;
		}
		pArray[start] = 1;
		pArray[end] = 1;
	}
}
*/

/*
void expression::next(int start)
{
}
*/


void expression::handle_expression()
{
    int Ncol, j, ret = 0;
    Ncol = edge_num; 
    lp = make_lp(0, Ncol);				//产生一个线性模型
    if(lp == NULL)
        ret = 1; 
    if(ret == 0) 
	{
        //设置变量，维数
        for(int i = 0; i < Ncol; i++)
        {
            char pStr[10] = {0}; 
            sprintf(pStr, "X%d", i);
            set_col_name(lp, i + 1, pStr);
        }
        colno = (int *) malloc(Ncol * sizeof(*colno));
        row = (REAL *) malloc(Ncol * sizeof(*row));
        if((colno == NULL) || (row == NULL))
            ret = 2;
    }
	
    if(ret == 0) 
	{
        set_add_rowmode(lp, TRUE); 
		j = 0;
		/*起点的出度=1*/
        for(int i = 0; i < size; i++)
        {    
            if(edge_id[m_start*size + i] < edge_num)
            {
                colno[j] = edge_id[m_start*size + i] + 1;
                row[j++] = 1;
            }
        }
		if(!add_constraintex(lp, j, row, colno, EQ, 1))
            ret = 3;
		j = 0;
		/*起点的入度=0*/
		for(int i = 0; i < size; i++)
        {    
            if(edge_id[i*size + m_start] < edge_num)
            {
                colno[j] = edge_id[i*size + m_start] + 1;
                row[j++] = 1;
            }
        }
		if(!add_constraintex(lp, j, row, colno, EQ, 0))
            ret = 3;
    }
	
    if(ret == 0) 
	{
		j = 0;
		/*终点的入度=1，出度=0*/
        for(int i = 0; i < size; i++)
        {
            if(edge_id[i*size + m_end] < edge_num)
            {
                colno[j] = edge_id[i*size + m_end] + 1;
                row[j++] = 1;
            }
        }
        if(!add_constraintex(lp, j, row, colno, EQ, 1))
            ret = 3;
		j = 0;
		for(int i = 0; i < size; i++)
        {
            if(edge_id[m_end*size + i] < edge_num)
            {
                colno[j] = edge_id[m_end*size + i] + 1;
                row[j++] = 1;
            }
        }
        if(!add_constraintex(lp, j, row, colno, EQ, 0))
            ret = 3;
    }
	
	/*任意一点满足不属于起点、终点，其出度与入度之差=0*/
    if(ret == 0) 
	{
        for(int m = 0; m < size; m++)
        {
			j = 0;
			if (m == m_start || m == m_end)
				continue;
            for(int n = 0; n < size; n++)
            {
                if(edge_id[m*size + n] < edge_num)
                {
                    colno[j] = edge_id[m*size + n] + 1;
                    row[j++] = 1;
                }
                if(edge_id[n*size + m] < edge_num)
                {
                    colno[j] = edge_id[n*size + m] + 1;
                    row[j++] = -1;
                }
				
            }
            if(!add_constraintex(lp, j, row, colno, EQ, 0))
                ret = 3;
        }
    }
	/*任意一点满足不属于起点、终点和demand中的点，其入度≤1*/
    if(ret == 0) 
	{
        for(int i = 0; i < size; i++)
        {
            j = 0;
			if(i == m_end || i == m_start) continue;
            vector<int>::iterator it = find(special_node.begin(), special_node.end(), i);
            if(it != special_node.end() && i == *it) continue;
            for(int m = 0; m < size; m++)
			{
                if(edge_id[m*size + i] < edge_num)
				{
                    colno[j] = edge_id[m*size + i] + 1;
                    row[j++] = 1;
                }
            }
            if(!add_constraintex(lp, j, row, colno, LE, 1))
                    ret = 3;
        }
    }
	
	/*demand中任意一点的入度=1*/
	if(ret == 0) 
	{
        for(vector<int>::iterator it = special_node.begin(); it != special_node.end(); it++)
        {
            j = 0;
			for(int m = 0; m < size; m++)
			{
				if(edge_id[m*size + (*it)] < edge_num)
				{
					colno[j] = edge_id[m*size + (*it)] + 1;
					row[j++] = 1;
				}
			}
			if(!add_constraintex(lp, j, row, colno, EQ, 1))
					ret = 3;
        }
    }
	set_add_rowmode(lp, FALSE);
}

/*目标方程*/
void expression::CreateObj()
{
	int j = 0;
	set_add_rowmode(lp, TRUE);
	for (unsigned int i = 0; i < edge_num; ++i)
	{
		colno[j] = i + 1;
		row[j++] = pGrph->IDGetCost(i);
	}
	set_obj_fnex(lp, j, row, colno);
	set_add_rowmode(lp, FALSE);
}


/*求解模型*/
bool expression::SolveMD()
{
	int rt = solve(lp);									//求解结果
	if(rt == INFEASIBLE)								//模型无解
		return false;
	get_variables(lp, row);								//获取到结果
	memset(pGrph->rtGrph, 0, sizeof(int)*size*size);	//先清空之前的结果
	for(unsigned int i = 0; i < edge_num; ++i)			//把结果写到结果图中
	{
		if(row[i] == 0) continue;
		pGrph->rtGrph[ pGrph->Edges[i].start * size + pGrph->Edges[i].end ] = 1;
	}
	return true;
}

//#define TEST 2

/*针对大规模问题添加额外约束*/
void expression::AddExtraConstrain()
{
	int j = 0;
	set_add_rowmode(lp, TRUE);
	for (unsigned int i = 0; i < edge_num; ++i)
	{
		colno[j] = i + 1;
		row[j++] = pGrph->IDGetCost(i);
	}
	add_constraintex(lp, j, row, colno, GE, 295);
	set_add_rowmode(lp, FALSE);
}


/*求解结果*/
void expression::GetResult()
{
	handle_expression();								//处理表达式
	CreateObj();										//创建目标方程
	if(size >= 550)
		set_break_at_first(lp, TRUE);
	else 
		set_break_at_first(lp, FALSE);
	set_verbose(lp, CRITICAL);							//仅仅显示内存不足等问题
	if(size >= 550)
	{
		set_mip_gap(lp, TRUE, 1);						//设置误差
		for (unsigned int i = 1; i <= edge_num; ++i)	//设置变量类型为整型
			set_int(lp, i, TRUE);
	}
	else
	{
		set_mip_gap(lp, TRUE, 1e-11);					//设置误差（默认误差）
		for (unsigned int i = 1; i <= edge_num; ++i)	//设置变量类型为0-1
			set_binary(lp, i, TRUE);
	}
	set_minim(lp);									//求最小值模型
	set_presolve(lp, PRESOLVE_LINDEP , get_presolveloops(lp));
	//AddExtraConstrain();
	//set_mip_gap(lp, TRUE, 1); /* sets absolute mip_gap to 8.0e-14 */
	bool rt = false;
	rt = SolveMD();				//解模型
	if(rt == false)				//模型无解，直接退出
		return;
		
#ifdef TEST
	write_LP(lp, stdout);
	cout << "get_variables" << endl;
	for (unsigned int j = 0; j < edge_num; j++)
		printf("%s: %f\n", get_col_name(lp, j + 1), row[j]);
#endif
	while(SearchCircle())	//有环则添加条件，继续求解，直到无环为止
	{
		rt = SolveMD();
		if(rt == false) return;
#ifdef TEST
		write_LP(lp, stdout);
#endif
	}
#ifdef TEST
	cout<<"get_variables"<<endl;
	for (unsigned int j = 0; j < edge_num; j++)
		printf("%s: %f\n", get_col_name(lp, j + 1), row[j]);
#endif
	PrintRt();
}


bool expression::SearchCircle()
{
	int tmp = 0;
/*	handle_expression();	
	CreateObj();									//创建目标方程
	GetResult();									//得到一个结果
	*/
	bool hasCircle = false;
	memset(pArray, 0, 601*sizeof(char));			//清空标记数组
	for(int i = 0; i < size; ++i)					//遍历结果图中的每一个点
	{
		if(pArray[i] == 1)  continue;				//该点访问过，继续
		pArray[i] = 1;								//遍历过的记录下来
		int record = i;								//从record开始
		//沿着环的方向找下一个，直到没有下一个节点为止
		edges.clear();								//清空当前边集
		nodes.clear();								//清空当前点集
		while(record != -1)
		{
			nodes.push_back(record);				//记录下遍历过的点
			//在此判断有环
			if(nodes.size() > 1 && record == nodes[0])//有环
			{
				hasCircle = true;
				//添加约束，当前边集中的变量小于其个数
				int j = 0;
				set_add_rowmode(lp, TRUE); 
				for(vector<int>::iterator it = edges.begin(); it != edges.end(); it++)
				{
					colno[j] = *it + 1;
					row[j++] = 1;
				}
				add_constraintex(lp, j, row, colno, LE, nodes.size()-2);
				set_add_rowmode(lp, FALSE);
				break;								//跳出此次寻找
			}
			
			tmp = pGrph->GetNodeOut(record);		//record的下一个点
			if(tmp == -1) break;					//找到终点了，次线路结束
			
			edges.push_back(pGrph->edge_id[record*size + tmp]);	//下一点不断开记录下该边的ID
			record = tmp;
			pArray[record] = 1;				//遍历过，标记
		}
	}
	return hasCircle;
}


/*保存解结果*/
void expression::PrintRt()
{
	/*
	cout << "start: " << m_start<< endl;
	cout << "end: " << m_end << endl;
	
	for(int i=0; i<size; ++i)
	{
		for(int j=0; j<size; ++j)
		{
			cout << pGrph->rtGrph[i*size + j] << "  ";
		}
		cout << endl;
	}
	*/
	//从起点开始，得到结果
	int i = m_start;
	int tmp = -1;
	vt_rt.clear();
	while(i != m_end)
	{
		//cout<< "Node: " << i ;
		tmp = pGrph->GetNodeOut(i);
		if(tmp == -1) break;
		vt_rt.push_back(pGrph->edge_id[i*size + tmp]);
		i = tmp;
	}
	
	for(unsigned int i=0; i<vt_rt.size(); ++i)
	{
		record_result(vt_rt[i]);
#ifdef TEST
		cout << vt_rt[i] << " ";
#endif
	}
#ifdef TEST
	cout<<endl;
#endif
}







