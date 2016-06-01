#include "Rule.h"
#include "PathSearch.h"

#include <algorithm>
#include <string.h>


Rule::Rule()
{
}

Rule::~Rule()
{
}




/* �������B */
/* ����ߺ� --> ���� --> ���*/
bool Sortfunction_B(const sDmd& i, const sDmd& j)
{
	if (i.sum > j.sum)
		return true;
	else if (i.sum < j.sum)
		return false;
	if (i.out > j.out)
		return true;
	else if (i.out < j.out)
		return false;

	if (i.in > j.in)
		return true;

	return false;
}

void Rule::Search()
{
	bool *cb = new bool[Graph::m_Edges.size()];							//�ر߼��
	memset(cb, 0, Graph::m_Edges.size()*sizeof(bool));
	PathSearch *sp = new PathSearch(0);
	PathSearch *sp2 = new PathSearch(1);

	int cb_count = 0;													//�ر�����
	int cost1 = 0;														//Ȩ��1
	int cost2 = 0;														//Ȩ��2
	//vector<sDmd> mm;

	while (!search_path(sp,0))											//�Ҳ�������λ��
	{
		/**
		mm.push_back(Graph::m_recDemand[0]->back());
		if (mm.size() > 4)
		{
			mm.erase(mm.begin());
			if (mm[0].id == mm[2].id && mm[1].id == mm[3].id)
			{
				cout << "�޽�" << endl;
				break;
			}
		}
		*/
		Graph::m_Demand[0].clear();
		for (reItr i = Graph::m_recDemand[0]->begin(); i != Graph::m_recDemand[0]->end(); ++i)
			Graph::m_Demand[0].push_back(*i);
	}
	cout << endl;
	//Graph::PrintGraph(Graph::m_Graph);
	int idex = Graph::m_start;
	while (idex != Graph::m_end)											//��ӡλ��			
	{
		record_result(WORK_PATH, Graph::m_ID[idex * Graph::m_size + sp->m_nPath[idex]]);

#ifdef _DEBUG	
		cout << Graph::m_ID[idex * Graph::m_size + sp->m_nPath[idex]] << " | ";
		cb[Graph::m_ID[idex * Graph::m_size + sp->m_nPath[idex]]] = true;	//�궨���Ѿ��߹�
		cost1 += Graph::m_Graph[idex * Graph::m_size + sp->m_nPath[idex]];
#endif
		Graph::m_Graph[idex * Graph::m_size + sp->m_nPath[idex]] += 300;
		idex = sp->m_nPath[idex];
	}
#ifdef _DEBUG 
	cout << endl;
	//Graph::PrintGraph(Graph::m_Graph);

	cout << endl;
	cout << endl;
	cout << "Ȩ��" << cost1 << endl;

	//�ڶ���·��
	//mm.clear();
#endif
	while (!search_path(sp2, 1))		//�Ҳ�������λ�ã��򽫲岻���ĵ����Ȳ���
	{
		//mm.push_back(Graph::m_recDemand[0]->back());
		//if (mm.size() > 4)
		//{
		//	mm.erase(mm.begin());
		//	if (mm[0].id == mm[2].id && mm[1].id == mm[3].id)
		//	{
		//		cout << "�޽�" << endl;
		//		break;
		//	}
		//}
		Graph::m_Demand[1].clear();
		for (reItr i = Graph::m_recDemand[1]->begin(); i != Graph::m_recDemand[1]->end(); ++i)
			Graph::m_Demand[1].push_back(*i);
	}

	idex = Graph::m_start;
	while (idex != Graph::m_end)		//��ӡλ��			
	{
		record_result(BACK_PATH, Graph::m_ID[idex*Graph::m_size + sp2->m_nPath[idex]]);
#ifdef _DEBUG
		cout << Graph::m_ID[idex*Graph::m_size + sp2->m_nPath[idex]] << " | ";
		if (cb[Graph::m_ID[idex * Graph::m_size + sp2->m_nPath[idex]]])
			++cb_count;
		cost2 += Graph::m_Graph[idex * Graph::m_size + sp2->m_nPath[idex]];
#endif // DEBUG

		idex = sp2->m_nPath[idex];
	}
#ifdef _DEBUG
	cout << endl;
	cout << endl;
	cout << "Ȩ�� " << cost2 << endl;
	cout << "�ر� " << cb_count << endl;
#endif
	print_time("~A");
}

/*����·��*/
/*����·������·�����*/
bool Rule::search_path(PathSearch *SP1, int num)
{
	SP1->Clear();												//����ϴν��
	if (Graph::m_Demand[num].size() == 0)						//û��ָ���ؾ���
	{
		SP1->Updata_Dem(Graph::m_start, Graph::m_end, true);	//���һ���⼴��
		return true;
	}

	//while (!CheckThrough(num, id))
	//{
	//	std::sort(Graph::m_Demand[num].begin(), Graph::m_Demand[num].end(), Sortfunction_B); //�ı�˳��
	//}

	sDmd temp = Graph::m_Demand[num].back();
	Graph::m_Demand[num].pop_back();
	SP1->m_nAllowedNode[Graph::m_end] = true;

	bool bt = SP1->Updata_Dem(Graph::m_start, temp.id, true);
	SP1->m_nAllowedNode[Graph::m_end] = false;
	SP1->Updata_Dem(temp.id, Graph::m_end, true);

	while (!Graph::m_Demand[num].empty())
	{
		temp = Graph::m_Demand[num].back();
		Graph::m_Demand[num].pop_back();
		if (!SP1->InsertDNode(temp.id))							//�Ҳ�������λ��
			return false;
	}
	return true;
}


/*·����ͨ�Բ��� demand_num��ʾ����㼯��ID, id��δ��ͨ����עδ��ͨ��*/
bool Rule::CheckThrough(int demand_num, int& id)
{
	int num = Graph::m_recDemand[demand_num]->size();

	//���������һ�������ͨ��
	if (MY_INFINITY <= Graph::m_Floyd[Graph::m_start*Graph::m_size + Graph::m_Demand[0].back().id]) //��������ͨ��
	{
		id = Graph::m_Demand[0].back().id;
		return false;
	}
		
	//���һ���ؾ������յ����ͨ��
	if (MY_INFINITY <= Graph::m_Floyd[Graph::m_Demand[0].begin()->id * Graph::m_size + Graph::m_end]) //���յ����ͨ��
	{
		id = Graph::m_Demand[0].begin()->id;
		return false;
	}
		
	//�ؾ�������һ���ؾ������ͨ��
	for (int i = num - 1; i > 0; --i)
	{
		if (MY_INFINITY <= Graph::m_Floyd[Graph::m_Demand[0][i].id * Graph::m_size + Graph::m_Demand[0][i - 1].id])
		{
			id = Graph::m_Demand[0][i].id;
			return false;
		}
	}
	return true;
}


