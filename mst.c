#include <stdio.h>
#include <stdlib.h>
#include "mst.h"

// Convert adjacency list graph to edge array
GraphMST* convertToEdgeArray(Graph* g) {
    int totalEdges = 0;

    // Count all edges
    for (int i = 0; i < g->V; i++) {
        Node* temp = g->adjList[i];
        while (temp) {
            totalEdges++;
            temp = temp->next;
        }
    }

    // Create GraphMST structure
    GraphMST* mstGraph = (GraphMST*)malloc(sizeof(GraphMST));
    mstGraph->V = g->V;
    mstGraph->E = totalEdges;
    mstGraph->edges = (EdgeMST*)malloc(totalEdges * sizeof(EdgeMST));

    // Fill edges
    int idx = 0;
    for (int i = 0; i < g->V; i++) {
        Node* temp = g->adjList[i];
        while (temp) {
            mstGraph->edges[idx].src = i;
            mstGraph->edges[idx].dest = temp->vertex;
            mstGraph->edges[idx].cost = temp->cost;         // cost for MST
            mstGraph->edges[idx].distance = temp->distance; // store distance too
            idx++;
            temp = temp->next;
        }
    }

    return mstGraph;
}

// ---------------- Disjoint Set functions ----------------
int findSet(int parent[], int i) {
    if (parent[i] != i) parent[i] = findSet(parent, parent[i]);
    return parent[i];
}

void unionSets(int parent[], int x, int y) {
    int xroot = findSet(parent, x);
    int yroot = findSet(parent, y);
    parent[xroot] = yroot;
}

// ---------------- Compare function for qsort ----------------
int cmpEdges(const void* a, const void* b) {
    EdgeMST* e1 = (EdgeMST*)a;
    EdgeMST* e2 = (EdgeMST*)b;
    return e1->cost - e2->cost; // Sort by cost
}

// ---------------- Kruskal MST function ----------------
int computeMST(Graph* g, EdgeMST* outEdges, int maxEdges) {
    // 1) Build edge list from adjacency list
    GraphMST* gm = convertToEdgeArray(g);

    // 2) Sort and compute MST using your existing Kruskal
    qsort(gm->edges, gm->E, sizeof(EdgeMST), cmpEdges);

    int* parent = (int*)malloc(gm->V * sizeof(int));
    for (int i = 0; i < gm->V; i++) parent[i] = i;

    int mst_cost = 0;
    int count = 0;

    for (int i = 0; i < gm->E && count < maxEdges; i++) {
        int u = gm->edges[i].src;
        int v = gm->edges[i].dest;

        int set_u = findSet(parent, u);
        int set_v = findSet(parent, v);

        if (set_u != set_v) {
            // Take this edge into the MST
            mst_cost += gm->edges[i].cost;

            outEdges[count].src      = u;
            outEdges[count].dest     = v;
            outEdges[count].cost     = gm->edges[i].cost;
            outEdges[count].distance = gm->edges[i].distance;
            count++;

            unionSets(parent, set_u, set_v);
        }
    }

    free(parent);
    free(gm->edges);
    free(gm);

    return mst_cost;     // Python gets the cost; edges are in outEdges[0..count-1]
}
