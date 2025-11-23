#ifndef MST_H
#define MST_H

#include "graphs.h"

// ---------------- MST Edge structure ----------------
typedef struct EdgeMST {
    int src;
    int dest;
    int cost;       // MST weight (use cost)
    int distance;   // distance between nodes (optional)
} EdgeMST;

// ---------------- MST Graph structure ----------------
typedef struct GraphMST {
    int V;          // number of vertices
    int E;          // number of edges
    EdgeMST* edges; // array of all edges
} GraphMST;

// MST Functions
GraphMST* convertToEdgeArray(Graph* g);
int kruskalMST(GraphMST* g, Graph* gOriginal);

#endif