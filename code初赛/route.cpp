#include "route.h"
#include <stdio.h>
#include "graph.h"
#include "search.h"
#include "lib_io.h"
#include<iostream>
using namespace std;

void search_route(char *topo[5000], int edge_num, char *demand)
{
	Graph grph;
	grph.CreateTables(topo, edge_num);
	grph.DealDemands(demand);
	grph.Search();
}
