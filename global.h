#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <vector>
#include <list>
#include <iostream>

struct sDmd
{
	int id;
	int sum;
	int in;
	int out;
};

struct sSegment
{
	int cost;
	int start;
	int end;
};


typedef std::vector<sDmd>::iterator deItr; 
typedef std::list<sDmd>::iterator reItr;
typedef std::vector<int>::iterator nIter;
typedef std::vector<sSegment>::iterator sSegIter;


#endif // !__GLOBAL_H__


