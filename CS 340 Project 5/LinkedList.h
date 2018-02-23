//
//  Author:     James Callahan
//  Class:      CS 340
//  Assignment: Project 5
//  Due Date:   31 October 2017
//  File:       LinkedList.h
//

#pragma once
#include<stdlib.h>

struct node{
    node *next = NULL;
    int key;
};

class LinkedList{
public:
    LinkedList();
    int length;
    void insert(int element);
    void print();
    void deleteList();
    node getNextItem();
    void reset();
    
protected:
    node *head, *tail, *traversal, *traversalHelper;
};

