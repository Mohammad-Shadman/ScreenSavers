#include "LinkedLists.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    int i = 0;
    while(i != 1){
        printf("\n\n");
        i++;
        LL list;
        LLinitList(&list,(Circle){(Vector){11,10},4});
        LLappend(&list,(Circle){(Vector){3,5},2});
        LLinsert(&list,0,(Circle){(Vector){9,7},3});
        LLinsert(&list,1,(Circle){(Vector){14,12},7});
        NodePtr curr = list.head;
        while(curr!=NULL){
            printf("x: %.2f, y: %.2f, r: %.2f\n",curr->data.center.x,curr->data.center.y,curr->data.radius);
            curr = curr->next;
        }
        LLpop(&list);
        LLpop(&list);
        LLremoveAtIndex(&list,0);
        LLappend(&list,(Circle){(Vector){1,2},10});
        LLappend(&list,(Circle){(Vector){9,8},17});
        LLremoveAtIndex(&list,1);
        LLappend(&list,(Circle){(Vector){7,20},100});
        LLappend(&list,(Circle){(Vector){123,223},11233});
        LLremoveAtIndex(&list, -1);

        curr = list.head;
        printf("_____________\n\n");
        while(curr!=NULL){
            printf("x: %.2f, y: %.2f, r: %.2f\n",curr->data.center.x,curr->data.center.y,curr->data.radius);
            curr = curr->next;
        }
        LLfreeAll(&list);
        //printf("x: %.2f, y: %.2f, r: %.2f\n",curr->data.center.x,curr->data.center.y,curr->data.radius);
    }
    return 0;
}