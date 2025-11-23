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
int kruskalMST(GraphMST* g, Graph* gOriginal) {
    qsort(g->edges, g->E, sizeof(EdgeMST), cmpEdges);

    int* parent = (int*)malloc(g->V * sizeof(int));
    for (int i = 0; i < g->V; i++) parent[i] = i;

    int mst_cost = 0;
    printf("Edges in MST:\n");
    for (int i = 0; i < g->E; i++) {
        int u = g->edges[i].src;
        int v = g->edges[i].dest;

        int set_u = findSet(parent, u);
        int set_v = findSet(parent, v);

        if (set_u != set_v) {
            mst_cost += g->edges[i].cost;
            printf("%s - %s : cost=%d, distance=%d\n",
                   gOriginal->names[u], gOriginal->names[v],
                   g->edges[i].cost, g->edges[i].distance);
            unionSets(parent, set_u, set_v);
        }
    }

    free(parent);
    return mst_cost;
}