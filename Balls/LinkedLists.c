#include "MainConsts.h"

#ifndef LL_H
#define LL_H

typedef struct Node{
    Circle data;
    struct Node* next;
}Node,*NodePtr;

typedef struct LinkedList{
    Node head;
    int len;

}LL,Linkedlist;

void LLappend(LL *list, Circle data){
    NodePtr curr = &list->head;
    if(list->head.next == NULL){
        NodePtr added = (NodePtr)malloc(sizeof(Node));
        added->data = data;
        added->next = NULL;
        list->head.next = added;
    }
    while(curr->next != NULL){
        curr = curr->next;
    }
    NodePtr added = (NodePtr)malloc(sizeof(Node));
    added->data = data;
    added->next = NULL;
    curr->next = added;
    list->len++;
}

void LLinsert(LL *list, int index, Circle data){
    NodePtr curr = &list->head;
    int i = 0;
    while(curr->next != NULL || i < index){
        curr = curr->next;
        i++;
    }
    NodePtr added = (NodePtr)malloc(sizeof(Node));
    added->data = data;
    added->next = curr->next;
    curr->next = added;
    list->len++;
}

void LLpop(LL *list, Circle data){
    NodePtr curr = &list->head;
    NodePtr prev;
    while(curr->next != NULL){
        prev = curr;
        curr = curr->next;
    }
    free((NodePtr)curr);
    prev->next = NULL;
    list->len--;
    
}

void LLremoveAt(LL *list,int index, Circle data){
    NodePtr curr = &list->head;
    NodePtr prev;
    int i = 0;
    while(curr->next != NULL || i < index){
        prev = curr;
        curr = curr->next;
        i++;
    }
    NodePtr temp = curr->next;
    curr->next= curr->next->next;
    free((NodePtr)temp);
    prev->next = NULL;
    list->len--;
}

void freeAll(LL *list){
    NodePtr curr = &list->head;
    NodePtr toBeRemoved;

    while(curr->next != NULL){
        toBeRemoved = curr;
        curr = curr->next;
        free((NodePtr)toBeRemoved);
        
    }
    free((NodePtr)curr);
}

#endif