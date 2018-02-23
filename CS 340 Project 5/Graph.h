//
//  Author:     James Callahan
//  Class:      CS 340
//  Assignment: Project 5
//  Due Date:   31 October 2017
//  File:       Graph.h
//

#include<fstream>
#include "LinkedList.h"
#pragma once

using namespace std;

// Struct used for holding edges taken (their parent, node #, weight to them)
struct shortest{
    int node;
    int weight;
    int parent;
    bool pulled;
    shortest *next;
};

// Struct used for the head of the adjacency matrix
struct n{
    int color; // 0 - white, 1 - grey, 2 - black
    int d, f; // beginning/discovered and ending/finished time respectively
    int *adj = NULL; // Array of adjacent values
    shortest *adjList = NULL;
    int *weight = NULL; // Array of weights
    int pi; // refers to predecessor
    int key; // Value of node (1, 2, etc.)
    int localEdges;
};

// Unused for now, was going to make a list and it failed
struct graphNode{
    graphNode *pi = NULL; // refers to predecessor
    graphNode *next = NULL;
    int color = 0; // 0 - white, 1 - grey, 2 - black
    int d, f; // beginning/discovered and ending/finished time respectively
    int key;
};

// Structs for edges and linked list nodes
struct edge{
    int one;
    int two;
};

class Graph{
public:
    // Declarations
    int length, numEdges; // number of nodes in the list that have connections to otheer nodes
    struct graphNode *graphArr = NULL; // unused, for list
    struct graphNode *nodes = NULL; // unused, for list
    graphNode *NIL = new graphNode; // unused, for list
    LinkedList list;
    
    // This is the primary group of nodes.
    n *l;
    shortest *s; // For holding the edges taken and weights of them
    
    // Constructor(s)
    Graph();
    
    // Destructor(s)
    ~Graph();
    
    // Functions
    void populateGraph(ifstream &in);
    
private:
    // variables
    int t,
    bE = 0,
    cE = 0,
    numPulled = 0,
    extractedNum,
    startingIndex;
    int infinity = 999999;
    bool negEdges = false;
//    topoSort;
    
    
    // Initializes arrays of edge structs to store edges
    // Not modular, but sufficient for this project
    struct edge *back = new edge[15];
    struct edge *cross = new edge[15];
    
    
    // function prototypes
    int countLines(ifstream &in);
    
    void DFS();
    void DFSOutput();
    void DFSVisit(n *nd);
    
    void pickAlgorithm();
    void initializeSingleSource(int start);
    
    void BellmanFord(int start);
    
    void DAGShortestPaths(int start);
    
    void Dijkstra(int start);
    
    void extractMin();
    
    void relaxLL(shortest *trav);
    
    void printS();
    
    void promptForDestination();
};
