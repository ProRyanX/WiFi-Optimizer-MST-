#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphs.h"

// Function to create a new node
Node* createNode(int v, int cost, int distance) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->cost = cost;
    newNode->distance = distance;
    newNode->next = NULL;
    return newNode;
}

// Function to create a directed graph
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = vertices;

    graph->adjList = (Node**)malloc(vertices * sizeof(Node*));
    for (int i = 0; i < vertices; i++) {
        graph->adjList[i] = NULL;
    }

    return graph;
}

// Function to add a directed edge (from src to dest)
void addEdge(Graph* graph, int src, int dest, int cost, int distance) {
    Node* newNode = createNode(dest, cost, distance);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
}

// Function to print the graph
void printGraph(Graph* graph) {
    for (int v = 0; v < graph->V; v++) {
        Node* temp = graph->adjList[v];
        printf("\n%s has outgoing edges to:", graph->names[v]);
        while (temp) {
            printf(" (%s, cost=%d, dist=%d) ->", 
                   graph->names[temp->vertex], temp->cost, temp->distance);
            temp = temp->next;
        }
        printf(" NULL\n");
    }
}
