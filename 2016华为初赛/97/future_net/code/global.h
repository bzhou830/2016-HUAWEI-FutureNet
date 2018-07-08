#ifndef __GLOBAL_H__
#define __GLOBAL_H__


struct sEdge
{
	int ID;				//编号规则 X+边ID
	int start;			//一条边的起点
	int end;			//一条边的终点
	int cost;			//边的权重
};

//int rnd(int nLow, int nUpper);
//double rnd(double dbLow, double dbUpper);
//double ROUND(double dbA);
//
//extern  double ALPHA;				//启发因子，信息素的重要程度
//extern  double BETA;				//期望因子，城市间距离的重要程度
//extern  double ROU;					//信息素残留参数
//extern  int DBQ;


#endif // !__GLOBAL_H__


