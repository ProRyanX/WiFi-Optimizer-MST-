#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

//---------Importing Files for Connectivity---------
#include <limits.h>
#include <stdbool.h>

//---------Importing Graph Structures---------
#include "graphs.h"

//-------------Connectivity Functions-------------
bool isConnected(Graph* graph, int src, int dest);
int dijkstraDistance(Graph* graph, int src, int dest);

#endif