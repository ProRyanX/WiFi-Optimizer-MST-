#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "connectivity.h"

// BFS function to check connectivity
bool isConnected(Graph* graph, int src, int dest) {
    bool visited[graph->V];
    for (int i = 0; i < graph->V; i++) visited[i] = false;

    int queue[graph->V];
    int front = 0, rear = 0;

    visited[src] = true;
    queue[rear++] = src;

    while (front < rear) {
        int current = queue[front++];
        Node* temp = graph->adjList[current];

        while (temp) {
            if (temp->vertex == dest)
                return true;
            if (!visited[temp->vertex]) {
                visited[temp->vertex] = true;
                queue[rear++] = temp->vertex;
            }
            temp = temp->next;
        }
    }
    return false;
}

// Dijkstra’s algorithm for shortest distance
void dijkstra(Graph* graph, int src, int dest) {
    int dist[graph->V];
    bool visited[graph->V];

    for (int i = 0; i < graph->V; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
    dist[src] = 0;

    for (int count = 0; count < graph->V - 1; count++) {
        int min = INT_MAX, u = -1;

        // Pick the unvisited vertex with the smallest distance
        for (int v = 0; v < graph->V; v++)
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }

        if (u == -1) break; // No reachable vertex remains
        visited[u] = true;

        Node* temp = graph->adjList[u];
        while (temp) {
            if (!visited[temp->vertex] && dist[u] != INT_MAX &&
                dist[u] + temp->distance < dist[temp->vertex]) {
                dist[temp->vertex] = dist[u] + temp->distance;
            }
            temp = temp->next;
        }
    }

    if (dist[dest] == INT_MAX)
        printf("No path exists between %s and %s.\n",
               graph->names[src], graph->names[dest]);
    else
        printf("Shortest distance between %s and %s is: %d\n",
               graph->names[src], graph->names[dest], dist[dest]);
}