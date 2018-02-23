//
//  Author:     James Callahan
//  Class:      CS 340
//  Assignment: Project 5
//  Due Date:   31 October 2017
//  File:       Graph.cpp
//

#include "Graph.h"
#include<fstream>
#include<iostream>
#include<sstream>

using namespace std;

// Default constructor
Graph::Graph(){
}

// Destructor
Graph::~Graph(){
    list.deleteList();
    delete[] graphArr;
    delete[] nodes;
    delete NIL;
    delete[] l;
    delete[] s;
    delete[] back;
    delete[] cross;
}

// Populates graph with data
// I started out trying to implement a list, but ended up doing a matrix
void Graph::populateGraph(ifstream &in){
    string a, b;
    int row = 0;
    negEdges = false;
    numEdges = 0;
    shortest *traversal = new shortest;
    
    length = countLines(in); // get number of lines
    
    l = new n[length];
    s = new shortest[length];
    
    // matrix initialization
    // Sets all values in the adjacency lists to 0 (not connected)
    for (int i = 0; i < length; i++) {
        l[i].adj = new int [length];
        l[i].weight = new int [length];
        l[i].key = (i + 1);
        l[i].localEdges = 0;
        l[i].adjList = new shortest;
        
        // Sets all adjacencies to 0 and weights to infinity
        for (int j = 0; j < length; j++) {
            l[i].adj[j] = 0;
            l[i].weight[j] = infinity;
        }
    }
    
    // Reads in a full line of the file
    getline(in, a);
    
    while(!in.eof()){
        
        // Use stringstream on a so I can get each value from the line of text
        istringstream is(a);
        
        // Grabs the first value and essentially throws it away
        // (1:, 2:, etc. as I don't need to know which row it is, I keep track of that separately)
        is >> b;
        
        traversal = l[row].adjList;
        
        // Goes while string stream still has more data
        while (is.peek() != -1) {
            
            // Reads in the next value to be inserted into the adjacency matrix
            is >> b;
            
            // Goes through the corresponding row's adjacency list until it comes to the value of b, then changes that value to 1.
            // This means the #row has an edge to z+1 (z starts at 0, but list starts at 1)
            for (int z = 0; z < length; z++) {
                
                
                if (stoi(b) == (z + 1)) {
                    
                    l[row].adj[z] = 1;
                    l[row].localEdges++;
                    is >> b;
                    l[row].weight[z] = stoi(b);
                    
                    traversal->node = z + 1;
                    traversal->weight = stoi(b);
                    traversal->pulled = false;
                    traversal->next = new shortest;
                    traversal = traversal->next;
                    
                    numEdges++;
                    
                    // If there is a negative weight, mark a boolean (for deciding which algorithm to use)
                    if (l[row].weight[z] < 0) {
                        negEdges = true;
                    }
                    
                    break;
                }
            }
        }
        
        // Increments our row (going in sequential order)
        ++row;
        
        // Grabs our next line of data
        getline(in, a);
    }

    DFS(); // Finds if there are any cycles
    pickAlgorithm();
}

// Selects which algorithm to use
void Graph::pickAlgorithm(){
    int start;
    
    // Show a list of vertices to use as a start location
    cout << "Possible start values: ";
    for (int i = 0; i < length; i++) {
        cout << i + 1 << " ";
    }
    cout << "\nEnter the starting node: ";
    cin >> start;
    cout << endl;
    startingIndex = start;
    
    // If no back edges (use DAG-SP)
    if (bE == 0) {
        printf("Graph is a DAG. 'DAG SP Algorithm' will be used.'\n");
        DAGShortestPaths(start); // use dag sp alg
    }
    // If there are back edges, but no negative edges (use Dijkstra)
    else if (negEdges == false){
        printf("Non-DAG, no negative edges. 'Dijkstra' will be used.\n");
        Dijkstra(start); // use dijkstra
    }
    // Else use Bellman-Ford
    else{
        printf("Non-DAG, negative edges. 'Bellman-Ford' will be used.'\n");
        BellmanFord(start); // use bellman ford
    }
}

// Initializes the node selected to start at
void Graph::initializeSingleSource(int start){
    
    // Initializes the array that holds the end values with infinity values
    
    for (int i = 0; i < length; i++) {
        s[i].weight = infinity;
        s[i].parent = infinity;
        s[i].pulled = false;
        s[i].node = i + 1;
    }
    
    // The first one extracted is given a weight of 0 and given to be its own parent
    s[start - 1].parent = start;
    s[start - 1].weight = 0;
    
    numPulled++; // One vertex pulled
}

void Graph::BellmanFord(int start){
    initializeSingleSource(start);
    shortest *traverse = new shortest; // For traversing adjacency list
    bool negCycle = true; // Keeps track of if there are any negative cycles
    
    // relax all edges
    for (int i = 0; i < length; i++) {
        traverse = l[i].adjList; // sets traverse to beginning of adjacency list
        extractedNum = i + 1; // sets the number extracted to the value i + 1
        while (traverse != NULL) {
            relaxLL(traverse);
            traverse = traverse->next;
        }
    }
    
    // Check for negative cycles
    for(int i = 0; i < length; i++) {
        traverse = l[i].adjList; // traversing through adjacency list
        while (traverse != NULL) {
            
            // If there is a negative cycle, set the bool and break from the loop
            if (s[traverse->node -1 ].weight > (s[i].weight + traverse->weight) ) {
                negCycle = false;
                break;
            }
            
            traverse = traverse->next;
        }
    }
    
    printS();
    
    // if necCycle false, leave and say there are neg cycles
    if (!negCycle) {
        printf("\nNegative cycles exist. Exiting.\n");
    }
    // Else prompt user for end vertices
    else{
        promptForDestination();
    }
}

void Graph::DAGShortestPaths(int start){
    shortest *traversal = new shortest; // for traversing adjacency list
    initializeSingleSource(start);
//    LinkedList trav = list;
    
    // For each vertex
    for (int i = 0; i < length; i++) {
        traversal = l[i].adjList;
        extractedNum = i + 1;
        
        // relax all edges from vertex
        while (traversal != NULL) {
            relaxLL(traversal);
            traversal = traversal->next;
        }
    }
    
    printS();
    
    promptForDestination();
}

// Dijkstra algorithm
void Graph::Dijkstra(int start){
    extractedNum = start; // sets the last extracted number to the given start value
    initializeSingleSource(start); // initializes the source
    int *adjacent; // Used in adjacency matrix (to travers it) to get all adjacent values
    shortest *trav = new shortest;
    
    // Go until we have pulled all of the vertices
    while (numPulled < length) {
        extractMin(); // pulls minimum edge
        adjacent = l[extractedNum - 1].adj;// set adj pointer to front of arr for extracted
        trav = l[extractedNum - 1].adjList; // going through adjacency list
        
        // Relax all edges from vertex
        while(trav != NULL) {
            relaxLL(trav);
            trav = trav->next;
        }
        
    }

    printS();
    promptForDestination();
}

// Simple function to print out the final edges taken, their parents and weight to reach them
void Graph::printS(){
    // Prints out all edges (testing)
        cout << "\nFinal Edges Selected:\n";
        for (int i = 0; i < length; i++) {
            cout << "s[" << i << "]:   node: " << s[i].node << " parent: " << s[i].parent << " weight: " << s[i].weight << endl;
        }
}

// Relaxes all edges from selected vertex
void Graph::relaxLL(shortest *trav){
    if (s[trav->node - 1].weight > (s[extractedNum - 1].weight + trav->weight) ) {
        s[trav->node - 1].weight = s[extractedNum - 1].weight + trav->weight;
        s[trav->node - 1].parent = extractedNum;
        trav->pulled = true;
    }
}

// Gets the smallest edge index and extracts it
void Graph::extractMin(){
    // Holds the index of the cheapest edge
    int smallestIndex = infinity; // junk value to initialize
    
    // Gets the first non pulled vertex
    for (int i = 0; i < length; i++) {
        if (!s[i].pulled) {
            smallestIndex = i;
            break;
        }
    }
    
    // Gets the cheapest edge and sets the index to smallestIndex
    for (int i = 0; i < length; i++) {
        if ( (!s[i].pulled) && (s[i].weight < s[smallestIndex].weight) ) {
            smallestIndex = i;
        }
    }
    
    // pulls the vertex and sets its parent, sets the last extractedNum index
    s[smallestIndex].pulled = true;
    s[smallestIndex].parent = extractedNum;
    extractedNum = smallestIndex + 1;
    numPulled++;
}

// Prompts the user for the destination vertex
void Graph::promptForDestination(){
    int dest, weight;
    string path = "";
    
    // Gets the first destination value
    cout << "\nEnter a negative number or number greater than " << length << " to exit.\n";
    cout << "Possible Destinations: ";
    for (int i = 0; i < length; i++) {
        cout << i+1 << " ";
    }
    cout << "\nNext destination: ";
    cin >> dest;
    cout << endl;
    
    // Any number entered that isn't in the scope will leave this loop
    while ( (dest > 0) && (dest < length + 1) ) {
        weight = s[dest - 1].weight;
        
        // Goes until we break from the loop
        while(true) {
            
            // If the weight has been set (ie the node is reachable)
            if(s[dest - 1].weight != infinity){
                // add to path and total weight
                path += to_string(s[dest - 1].node);
                path += " ";
                
                // Once we are at the root node, break from the loop
                if (dest == startingIndex) {
                    break;
                }
                dest = s[dest - 1].parent;
            }
            
            // Else the node is not reachable and output that fact
            else{
                path = "Node not reachable";
                break;
            }
        }
        
        
        // output path and total weight
        printf("Length of path: %d  Path: %s\n", weight, path.c_str());
        
        cout << "Possible Destinations: ";
        for (int i = 0; i < length; i++) {
            cout << i+1 << " ";
        }
        cout << "\nNext destination: ";
        cin >> dest;
        cout << endl;
        weight = 0;
        path = "";
    }
}


// Counts the number of lines in the input fiile
int Graph::countLines(ifstream &in){
    int l = 0;
    string a;
    
    getline(in, a);
    
    while (!in.eof()) {
        getline(in, a);
        l++;
    }
    
    // http://www.cplusplus.com/reference/istream/istream/seekg/
    in.clear();
    in.seekg(0, in.beg);
    
    return l;
}

// DFS utility
void Graph::DFS(){
    
    // Sets all nodes color to white and a junk pi (predecessor) value
    for (int i = 0; i < length; i++) {
        l[i].color = 0;
        l[i].pi = infinity;
    }
    
    t = 0; // Sets time to 0 when DFS is called
    
    // For every node in the list, go through and visit everyone in its adjacency matrix
    for (int i = 0; i < length; i++) {
        if (l[i].color == 0) {
            DFSVisit(&l[i]);
        }
    }
}

void Graph::DFSVisit(n *nd){
    t++;
    nd->d = t; // set start time (discory time)
    nd->color = 1; // color gray
    
    // For each node
    for (int i = 0; i < length; i++) {
        
        // Check if it is adjacent via the matrix
        if (nd->adj[i] == 1) {
            // If adjacent and white, visit
            if (l[i].color == 0) {
                l[i].pi = nd->key;
                DFSVisit(&l[i]);
            }
            else if(l[i].color == 1){
                back[bE].one = nd->key;
                back[bE].two = (i + 1);
                bE++;
            }
        }
    }
    
    nd->color = 2; // color current node black (finished)
    t++; // increment time
    nd->f = t; // set finish time
    
    // I am adding the values to a list here instead of having a separate topological sort
    list.insert(nd->key);
}

void Graph::DFSOutput(){
    if (bE != 0) {
        cout << "\nGraph is cyclic.\n\n";
        cout << "back edges found!" << endl;
        for (int i = 0; i < bE; i++) {
            printf("Back edges: %d,%d\n", back[i].one, back[i].two);
        }
    }
    // If no back edges, then topological sort (list already made in DFS)
    else{
        cout << "\nGraph is acyclic.\n\n";
        cout << "Topologically sorted data: ";
        list.print();
    }
    
    cout << endl;
    
    // If any cross edges, output them
    if (cE != 0) {
        cout << "Cross edges found!" << endl;
        for (int i =  0; i < cE; i++) {
            printf("Cross edges: %d,%d\n", cross[i].one, cross[i].two);
        }
    }

}

