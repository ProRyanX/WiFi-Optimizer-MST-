#ifndef GRAPHS_H
#define GRAPHS_H

#define MAX 50

// Node for adjacency list
typedef struct Node {
    int vertex;       // Index of the connected building
    int cost;         // Cost weight
    int distance;     // Distance weight
    struct Node* next;
} Node;

// Graph structure
typedef struct Graph {
    int V;                 // Number of buildings (vertices)
    Node** adjList;        // Array of adjacency lists
    char names[100][MAX];  // Building names
} Graph;

// Graph Functions
Graph* createGraph(int vertices);
Node* createNode(int , int cost, int distance);
void addEdge(Graph* g, int src, int dest, int cost, int distance);
void printGraph(Graph* g);

#endif