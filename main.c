#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphs.h"
#include "mst.h"
#include "connectivity.h"

int main(){
    int V, E, cost, distance;
    char b1[MAX], b2[MAX];

    printf("Enter number of buildings: ");
    scanf("%d", &V);
    getchar();

    Graph* g = createGraph(V);

    printf("Enter building names: \n");
    int i;
    for(i=0; i<V; i++){
        scanf("%s", g->names[i]);
    }

    printf("Enter number of directed paths: ");
    scanf("%d", &E);

    printf("Enter paths in format: From To Cost Distance\n");
    for(i=0; i<E; i++){
        scanf("%s %s %d %d", b1, b2, &cost, &distance);

        int u = -1, v = -1;
        int j;
        for(j=0; j<V; j++){
            if(strcmp(g->names[j],b1)==0) u = j;
            if(strcmp(g->names[j],b2)==0) v = j;
        }

        if(u != -1 && v != -1)
            addEdge(g, u, v, cost, distance);
        else
            printf("Invalid building names: %s %s\n", b1, b2);
    }

    printGraph(g);

    GraphMST* mstGraph = convertToEdgeArray(g);
    int totalCost = kruskalMST(mstGraph, g);
    printf("\nTotal minimum cost of MST: %d\n", totalCost);

    free(mstGraph->edges);
    free(mstGraph);

    // Connectivity and shortest distance check
    char start[MAX], end[MAX];
    printf("\nEnter two building names to check connectivity and shortest distance:\n");
    scanf("%s %s", start, end);

    // Find their indices
    int s = -1, d = -1;
    for (int i = 0; i < V; i++) {
        if (strcmp(g->names[i], start) == 0) s = i;
        if (strcmp(g->names[i], end) == 0) d = i;
    }

    if (s == -1 || d == -1) {
        printf("Invalid building names.\n");
        return 0;
    }

    // 1️⃣ Check connectivity
    if (isConnected(g, s, d))
        printf("%s and %s are connected.\n", start, end);
    else {
        printf("%s and %s are NOT connected.\n", start, end);
        return 0;
    }

    // 2️⃣ Find shortest distance
    dijkstra(g, s, d);

    return 0;
}