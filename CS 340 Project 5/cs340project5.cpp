//
// Author:      James Callahan
// Due Date:    31 October 2017
// Class:       CS 340, Sorting Algorithms
// Assignment:  Project 5
// File:        cs340project5.cpp
//

#include <iostream>
#include <fstream>
#include <assert.h>
#include "Graph.h"

using namespace std;

void openFile(string filename, ifstream &in);

int main(int argc, const char * argv[]) {
    string filename = "graphin-weighted-DAG.txt";
    ifstream in;
    int fileNum;
    Graph g;
    
    // For selecting which file to use
    cout << "Select which file you would like to use:\n";
        cout << "\t1: Graphin Fig1\n";
        cout << "\t2: Graphin Fig2\n";
        cout << "\t3: Graphin Fig4\n";
        cout << "\t4: Graphin Fig5\n";
        cout << "\t5: Graphin-Weighted-DAG\n";
    cout << "Choice: ";
    cin >> fileNum;
    
    switch (fileNum) {
        case 1:
            filename = "graphin Fig1.txt";
            break;
        case 2:
            filename = "graphin Fig2.txt";
            break;
        case 3:
            filename = "graphin Fig4.txt";
            break;
        case 4:
            filename = "graphin Fig5.txt";
            break;
        case 5:
            filename = "graphin-weighted-DAG.txt";
            break;
            
        default:
            filename = "graphin Fig1.txt";
            break;
    }
    
    openFile(filename, in);
    
    g.populateGraph(in);
    
    in.close();
    return 0;
}

// Ensures that the file opens, or assert
void openFile(string filename, ifstream &in){
    in.open(filename);
    
    if(in.fail()){
        cout << "Error opening file " << filename << "\n";
        assert(!in.fail());
    }
}
