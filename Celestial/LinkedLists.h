#ifndef LL_H
#define LL_H
#include <stdlib.h>
#include "Vec.h"
#include <stdint.h>

typedef struct Pixel32{
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
    uint8_t Alpha;
    

}Pix32, Pix, *PPix32;

typedef struct Circle{
    Vector center;
    double radius;
    Pix pixel;
} Circle,*PCircle;


typedef struct Node{
    Circle data;
    struct Node* next;
}Node,*NodePtr;

typedef struct LinkedList{
    NodePtr head;
    int len;

}LL,Linkedlist;

void LLinitList(LL *list,Circle data){
    list->head = (NodePtr)malloc(sizeof(Node));
    list->head->data = data;
    list->head->next = NULL;
}

void LLappend(LL *list, Circle data){
    if (list->head == NULL){
        LLinitList(list, data);
        return;
    }
    
    NodePtr curr = list->head;
    while(curr->next != NULL){
        curr = curr->next;
    }
    curr->next = (NodePtr)malloc(sizeof(Node));
    curr->next->data = data;
    curr->next->next = NULL;
    
    list->len++;
}

void LLinsert(LL *list, int index, Circle data){
    if (list->head == NULL){
        LLinitList(list, data);
        list->len++;
        return;
    }
    NodePtr curr = list->head;
    if (index == 0){
        NodePtr added = (NodePtr)malloc(sizeof(Node));
        added->data = data;
        added->next = list->head;
        list->head = added;
        list->len++;
        return;
    }
    int i = 0;
    while(curr->next != NULL && i < index-1){
        curr = curr->next;
        i++;
    }
    NodePtr added = (NodePtr)malloc(sizeof(Node));
    added->data = data;
    added->next = curr->next;
    curr->next = added;
    list->len++;
}

void LLpop(LL *list){
    if (list->head == NULL){
        return;
    }
    if (list->head->next == NULL){
        free(list->head);
        list->head = NULL;
        list->len--;
        return;
    }

    NodePtr curr = list->head;
    NodePtr prev;
    while(curr->next != NULL){
        prev = curr;
        curr = curr->next;
    }
    prev->next = NULL;
    list->len--;
    free(curr);
    curr = NULL;
    return;
    
    
}

//problem here and idk what
void LLremoveAtIndex(LL *list,int index){
    NodePtr curr = list->head;
    NodePtr prev;
    if(index ==0){
        prev = list->head;
        list->head = list->head->next;
        free(prev);
        prev = NULL;
        return;
    }
    int j = 0;
    while(curr->next != NULL && j < index-1){
        prev = curr;
        curr = curr->next;
        j++;
    }
    NodePtr temp = curr->next;
    if(curr->next == NULL){
        prev->next = NULL;
        free(curr);
        curr = NULL;
        list->len--;
        return;
    }
    curr->next= curr->next->next;
    free(temp);
    temp = NULL;
    
    list->len--;
}

void LLremoveAtPtr(LL *list,NodePtr ptr){
    NodePtr curr = list->head;
    NodePtr prev;
    if(ptr == list->head){
        prev = list->head;
        list->head = list->head->next;
        free((NodePtr)prev);
        return;
    }
    
    while(curr->next != NULL && curr != ptr){
        prev = curr;
        curr = curr->next;
    }
    NodePtr temp = curr->next;
    curr->next= curr->next->next;
    //prev->next = NULL;
    free((NodePtr)temp);
    list->len--;
}

void LLfreeAll(LL *list){
    NodePtr curr = list->head;
    NodePtr toBeRemoved;

    while(curr->next != NULL){
        toBeRemoved = curr;
        curr = curr->next;
        free(toBeRemoved);
        
    }
    free(curr);
}

#endif