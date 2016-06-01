#ifndef __RULE_H__
#define __RULE_H__

#include "global.h"
#include "graph.h"
#include "PathSearch.h"

class Rule
{
public:
	Rule();
	~Rule();
	static void Search();

private:
	//static PathSearch SP1;
	static bool CheckThrough(int demand_num, int& id);			//�����ͨ��
	static bool search_path(PathSearch *SP1, int num);			//����
};



#endif // !__RULE_H__





