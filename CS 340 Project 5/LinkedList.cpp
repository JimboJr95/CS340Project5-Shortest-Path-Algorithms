//
//  Author:     James Callahan
//  Class:      CS 340
//  Assignment: Project 5
//  Due Date:   31 October 2017
//  File:       LinkedList.cpp
//

#include "LinkedList.h"
#include<stdlib.h>
#include<iostream>

LinkedList::LinkedList(){
    head = NULL;
    tail = NULL;
    traversal = NULL;
    traversalHelper = NULL;
    
    length = 0;
}

void LinkedList::insert(int element){
    node *n = new node;
    n->key = element;
    n->next = NULL;
    length++;
    
    if (this->head == NULL) {
        head = n;
        tail = n;
    }
    else{
        tail = head;
        
        while (tail->next != NULL) {
            tail = tail->next;
        }
        
        tail->next = n;
    }
    traversal = head;
}

node LinkedList::getNextItem(){
    traversalHelper = traversal;
    traversal = traversal->next;
    
    return *traversalHelper;
}

void LinkedList::reset(){
    traversal = head;
}

void LinkedList::print(){
    tail = head;
    
    while (tail != NULL) {
        std::cout << tail->key << " ";
        tail = tail->next;
    }
    
    std::cout << "\n";
}

void LinkedList::deleteList(){
    node *destory = new node;
    destory = head;
    
    while (destory != NULL) {
        delete destory;
        head = head->next;
        destory = head;
    }
}
